#ifndef __gear_vm__reference_inlines__
#define __gear_vm__reference_inlines__

#include "reference.hpp"

#include <cassert>

namespace GVM {

	inline
	GReference::GReference(union GReferenceValue *ref_value)
		:	referenced_value(ref_value),
			strong_reference_count(1),
			/* all strong references share one implicit weak reference, 
			 * to prevent this object from being deleted too soon */
			weak_reference_count(1) {}

	inline
	GReference::~GReference() noexcept {
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		assert(weak_reference_count.load(std::memory_order_seq_cst) == 0);
	}

	inline
	void
	GReference::retain(void *env) {
		strong_reference_count.fetch_add(1, std::memory_order_relaxed);
	}

	inline
	void
	GReference::release(void *env) {
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
	GValue
	GReference::downgrade(void *env) {
		GWeakPointer *weak_pointer = new(std::nothrow) GWeakPointer(this);
		if (weak_pointer != nullptr) {
			const GReference *weak_pointer_reference
				= new(std::nothrow) GReference(reinterpret_cast<GReferenceValue *>(weak_pointer));
			if (weak_pointer_reference != nullptr) {
				weak_reference_count.fetch_add(1, std::memory_order_relaxed);
				return GValue(weak_pointer_reference);
			} else {
				delete weak_pointer; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return GValue::kNil; /* we failed to allocate the whole weak pointer, or just the ref. */
	}

	inline
	STRONG_REFERENCE_COUNT_TYPE
	GReference::strong_count() const noexcept {
		return strong_reference_count.load(std::memory_order_seq_cst);
	}

	inline
	WEAK_REFERENCE_COUNT_TYPE
	GReference::weak_count() const noexcept {
		/* we subtract 1 to hide the implicit weak reference held by all strong references */
		return weak_reference_count.load(std::memory_order_seq_cst) - 1;
	}

	inline
	GReferenceCommon::GReferenceCommon(GReferenceType ref_type)
		: reference_type(ref_type) { }

	inline
	GObject::GObject(PsiElement *psi_type, unsigned_integer_8 ivars_count)
		:	common(GReferenceTypeObject),
			psi_type(psi_type),
			dynamic_ivars(nullptr),
			ivars_count(ivars_count),
			ivars(ivars_count >= 1 ? new GVariable[ivars_count] : nullptr)
	{
		for (unsigned_integer_16 i = 0; i < ivars_count; i += 1) {
			ivars[i] = GValue::kUndefined;
		}
	}

	inline
	GObject::~GObject() {
		// TODO: delete dynamic_ivars
	}

	inline
	GUnmanagedUnsafe::GUnmanagedUnsafe(void *pointer, void (*deallocator)(void *))
		:	common(GReferenceTypeUnmanagedUnsafe),
			pointer(pointer),
			deallocator(deallocator) {};

	inline
	GUnmanagedUnsafe::~GUnmanagedUnsafe() {
		if (deallocator != nullptr) {
			/* the deallocator function is supposed to determine whether or not to deallocate the value */
			deallocator(pointer);
		}
	}

	inline
	GInteger64::GInteger64(const integer_64 value) noexcept
		:	common(GReferenceTypeInteger64),
			value(value) {};

	inline
	GInteger64Unsigned::GInteger64Unsigned(const unsigned_integer_64 value) noexcept
		:	common(GReferenceTypeUnsignedInteger64),
			value(value) {};

	inline
	GInteger128::GInteger128(const integer_128 value) noexcept
		:	common(GReferenceTypeInteger128),
			value(value) {};

	inline
	GInteger128Unsigned::GInteger128Unsigned(const unsigned_integer_128 value) noexcept
		:	common(GReferenceTypeUnsignedInteger128),
			value(value) {};

	inline
	GFloat128::GFloat128(const float_128 value) noexcept
		:	common(GReferenceTypeFloat128),
			value(value) {};

	inline
	GComplex::GComplex(const GValue real, const GValue imaginary) noexcept
		:	common(GReferenceTypeComplex),
			real(real),
			imaginary(imaginary) {};

	inline
	GRational::GRational(const GValue numerator, const GValue denominator) noexcept
		:	common(GReferenceTypeRational),
			numerator(numerator),
			denominator(denominator) {};

	inline
	GDecimalLimited::GDecimalLimited(const unsigned_integer_128 value,
	                                 const unsigned_integer_8 decimal_digits) noexcept
		:	common(GReferenceTypeDecimalLimited),
			value(value),
			decimal_digits(decimal_digits) {}

	inline
	GDecimalUnlimited::GDecimalUnlimited(unsigned_integer_8 *const individual_digits,
	                                     const size_t digits_count,
	                                     const unsigned_integer_64 decimal_digits) noexcept
		:	common(GReferenceTypeDecimalUnlimited),
			individual_digits(individual_digits), /* here we take over ownership of the digits */
			digits_count(digits_count),
			decimal_digits(decimal_digits) {}

	inline
	GDecimalUnlimited::~GDecimalUnlimited() noexcept {
		delete[] individual_digits;
	}

	inline
	GWeakPointer::GWeakPointer(GReference *ptr)
		:	common(GReferenceTypeWeakPointer),
			pointer(ptr) {}

	inline
	void
	GWeakPointer::on_release() noexcept {
		GReference *ptr = pointer.load(std::memory_order_seq_cst);
		assert(ptr != nullptr);
		if (ptr->weak_reference_count.fetch_sub(1, std::memory_order_release) == 1) {
			/* this only happens when this is the last weak reference */
			std::atomic_thread_fence(std::memory_order_acquire);
			delete ptr;
		}
	}

	inline
	GValue
	GWeakPointer::upgrade(void *env) {
		GReference *ptr = pointer.load(std::memory_order_seq_cst);
		assert(ptr != nullptr);
		while (true) {
			STRONG_REFERENCE_COUNT_TYPE ref_count
				= ptr->strong_reference_count.load(std::memory_order_seq_cst);
			if (ref_count == 0) {
				return GValue::kNil;
			} else if (ptr->strong_reference_count
					.compare_exchange_weak(ref_count, ref_count + 1, std::memory_order_seq_cst)) {
				return GValue(ptr);
			}
		}
	}
}

#endif
