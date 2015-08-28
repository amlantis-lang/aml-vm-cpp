#ifndef __aml_vm__reference_inlines__
#define __aml_vm__reference_inlines__

#include "reference.hpp"

#include <cassert>

namespace AVM {

	inline
	AReference::AReference(union AReferenceValue *ref_value)
		:	referenced_value(ref_value),
			strong_reference_count(1),
			/* all strong references share one implicit weak reference, 
			 * to prevent this object from being deleted too soon */
			weak_reference_count(1) {}

	inline
	AReference::~AReference() noexcept {
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		assert(weak_reference_count.load(std::memory_order_seq_cst) == 0);
	}

	inline
	void
	AReference::retain(void *env) {
		strong_reference_count.fetch_add(1, std::memory_order_relaxed);
	}

	inline
	void
	AReference::release(void *env) {
		if (referenced_value.load(std::memory_order_relaxed) == nullptr) {
			// TODO: call sth. like env->signal_over_released()
			return; /* prevent over-release */
		}
		if (strong_reference_count.fetch_sub(1, std::memory_order_release) != 1) {
			return; /* not yet the last strong reference */
		} else {
			std::atomic_thread_fence(std::memory_order_acquire);
			release_slow(env);
		}
	}

	inline
	AValue
	AReference::downgrade(void *env) {
		AWeakPointer *weak_pointer = new(std::nothrow) AWeakPointer(this);
		if (weak_pointer != nullptr) {
			const AReference *weak_pointer_reference
				= new(std::nothrow) AReference(reinterpret_cast<AReferenceValue *>(weak_pointer));
			if (weak_pointer_reference != nullptr) {
				weak_reference_count.fetch_add(1, std::memory_order_relaxed);
				return AValue(weak_pointer_reference);
			} else {
				delete weak_pointer; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return AValue::kNil; /* we failed to allocate the whole weak pointer, or just the ref. */
	}

	inline
	STRONG_REFERENCE_COUNT_TYPE
	AReference::strong_count() const noexcept {
		return strong_reference_count.load(std::memory_order_seq_cst);
	}

	inline
	WEAK_REFERENCE_COUNT_TYPE
	AReference::weak_count() const noexcept {
		/* we subtract 1 to hide the implicit weak reference held by all strong references */
		return weak_reference_count.load(std::memory_order_seq_cst) - 1;
	}

	inline
	AReferenceCommon::AReferenceCommon(AReferenceType ref_type)
		: reference_type(ref_type) { }

	inline
	AObject::AObject(PsiElement *psi_type, unsigned_integer_8 ivars_count)
		:	common(AReferenceTypeObject),
			psi_type(psi_type),
			dynamic_ivars(nullptr),
			ivars_count(ivars_count),
			ivars(ivars_count >= 1 ? new AVariable[ivars_count] : nullptr)
	{
		for (unsigned_integer_16 i = 0; i < ivars_count; i += 1) {
			ivars[i] = AValue::kUndefined;
		}
	}

	inline
	AObject::~AObject() {
		// TODO: delete dynamic_ivars
	}

	inline
	AUnmanagedUnsafe::AUnmanagedUnsafe(void *pointer, void (*deallocator)(void *))
		:	common(AReferenceTypeUnmanagedUnsafe),
			pointer(pointer),
			deallocator(deallocator) {};

	inline
	AUnmanagedUnsafe::~AUnmanagedUnsafe() {
		if (deallocator != nullptr) {
			/* the deallocator function is supposed to determine whether or not to deallocate the value */
			deallocator(pointer);
		}
	}

	inline
	AInteger64::AInteger64(const integer_64 value) noexcept
		:	common(AReferenceTypeInteger64),
			value(value) {};

	inline
	AInteger64Unsigned::AInteger64Unsigned(const unsigned_integer_64 value) noexcept
		:	common(AReferenceTypeUnsignedInteger64),
			value(value) {};

	inline
	AInteger128::AInteger128(const integer_128 value) noexcept
		:	common(AReferenceTypeInteger128),
			value(value) {};

	inline
	AInteger128Unsigned::AInteger128Unsigned(const unsigned_integer_128 value) noexcept
		:	common(AReferenceTypeUnsignedInteger128),
			value(value) {};

	inline
	AFloat128::AFloat128(const float_128 value) noexcept
		:	common(AReferenceTypeFloat128),
			value(value) {};

	inline
	AComplex::AComplex(const AValue real, const AValue imaginary) noexcept
		:	common(AReferenceTypeComplex),
			real(real),
			imaginary(imaginary) {};

	inline
	ARational::ARational(const AValue numerator, const AValue denominator) noexcept
		:	common(AReferenceTypeRational),
			numerator(numerator),
			denominator(denominator) {};

	inline
	ADecimalLimited::ADecimalLimited(const unsigned_integer_128 value,
	                                 const unsigned_integer_8 decimal_digits) noexcept
		:	common(AReferenceTypeDecimalLimited),
			value(value),
			decimal_digits(decimal_digits) {}

	inline
	ADecimalUnlimited::ADecimalUnlimited(unsigned_integer_8 *const individual_digits,
	                                     const size_t digits_count,
	                                     const unsigned_integer_64 decimal_digits) noexcept
		:	common(AReferenceTypeDecimalUnlimited),
			individual_digits(individual_digits), /* here we take over ownership of the digits */
			digits_count(digits_count),
			decimal_digits(decimal_digits) {}

	inline
	ADecimalUnlimited::~ADecimalUnlimited() noexcept {
		delete[] individual_digits;
	}

	inline
	AWeakPointer::AWeakPointer(AReference *ptr)
		:	common(AReferenceTypeWeakPointer),
			pointer(ptr) {}

	inline
	void
	AWeakPointer::on_release() noexcept {
		AReference *ptr = pointer.load(std::memory_order_seq_cst);
		assert(ptr != nullptr);
		if (ptr->weak_reference_count.fetch_sub(1, std::memory_order_release) == 1) {
			/* this only happens when this is the last weak reference */
			std::atomic_thread_fence(std::memory_order_acquire);
			delete ptr;
		}
	}

	inline
	AValue
	AWeakPointer::upgrade(void *env) {
		AReference *ptr = pointer.load(std::memory_order_seq_cst);
		assert(ptr != nullptr);
		while (true) {
			STRONG_REFERENCE_COUNT_TYPE ref_count
				= ptr->strong_reference_count.load(std::memory_order_seq_cst);
			if (ref_count == 0) {
				return AValue::kNil;
			} else if (ptr->strong_reference_count
					.compare_exchange_weak(ref_count, ref_count + 1, std::memory_order_seq_cst)) {
				return AValue(ptr);
			}
		}
	}
}

#endif
