#include "reference.hpp"
#include "reference_inlines.hpp"
#include "value.hpp"
#include "value_inlines.hpp"

#include <cassert>

namespace AVM {
	static AReferenceLock ReferenceLocks[REFERENCE_LOCKS_COUNT];

	AReferenceLock &
	lock_for_reference(union AReferenceValue *reference) noexcept {
		return ReferenceLocks[REFERENCE_LOCK_INDEX(reference)];
	}

	void
	AReference::release_slow(void *env) {
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		/* since the release has come to this point, there are no more strong references */
		AReferenceValue *pointer = referenced_value.load(std::memory_order_relaxed);

		/* TODO: call sth. like env->destruct(pointer); !!! (maybe better in each switch case, to save a check) */

		switch (pointer->common.reference_type) {
			case AReferenceTypeObject: {
				AObject *real_pointer = reinterpret_cast<AObject *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeUnmanagedUnsafe: {
				AUnmanagedUnsafe *real_pointer = reinterpret_cast<AUnmanagedUnsafe *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeInteger64: {
				AInteger64 *real_pointer = reinterpret_cast<AInteger64 *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeUnsignedInteger64: {
				AInteger64Unsigned *real_pointer = reinterpret_cast<AInteger64Unsigned *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeInteger128: {
				AInteger128 *real_pointer = reinterpret_cast<AInteger128 *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeUnsignedInteger128: {
				AInteger128Unsigned *real_pointer = reinterpret_cast<AInteger128Unsigned *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeFloat128: {
				AFloat128 *real_pointer = reinterpret_cast<AFloat128 *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeComplex: {
				AComplex *real_pointer = reinterpret_cast<AComplex *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeRational: {
				ARational *real_pointer = reinterpret_cast<ARational *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeDecimalLimited: {
				ADecimalLimited *real_pointer = reinterpret_cast<ADecimalLimited *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeDecimalUnlimited: {
				ADecimalUnlimited *real_pointer = reinterpret_cast<ADecimalUnlimited *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeArray: {
				AArray *real_pointer = reinterpret_cast<AArray *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeNestedPointer: {
				ANestedPointer *real_pointer = reinterpret_cast<ANestedPointer *>(pointer);
				delete real_pointer;
				break;
			}
			case AReferenceTypeWeakPointer: {
				AWeakPointer *real_pointer = reinterpret_cast<AWeakPointer *>(pointer);
				/* maybe move this to destructor from *env */
				real_pointer->on_release();
				delete real_pointer;
				break;
			}
			case AReferenceTypePsiBearer: {
				APsiBearer *real_pointer = reinterpret_cast<APsiBearer *>(pointer);
				delete real_pointer;
				break;
			}
		}
		/* just once again, to check that env did not resurrect the referenced value */
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		if (weak_reference_count.fetch_sub(1, std::memory_order_relaxed) == 1) {
			/* if we just released the last strong reference, 
			 * then also the (last) implicit weak reference is released */
			std::atomic_thread_fence(std::memory_order_acquire);
			delete this;
		}
	}

	AValue
	AObject::allocate(PsiElement *psi_type, void *env) noexcept {
		/* TODO: read number of ivars from psi_type */
		AObject *object = new(std::nothrow) AObject(psi_type, 0);
		if (object != nullptr) {
			AReference *object_reference
				= new(std::nothrow) AReference(reinterpret_cast<AReferenceValue *>(object));
			if (object_reference != nullptr) {
				return AValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return AValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

	AValue
	AUnmanagedUnsafe::allocate(void *pointer, void (*deallocator)(void *), void *env) noexcept {
		AUnmanagedUnsafe *unmanaged = new(std::nothrow) AUnmanagedUnsafe(pointer, deallocator);
		if (unmanaged != nullptr) {
			AReference *unmanaged_reference
				= new(std::nothrow) AReference(reinterpret_cast<AReferenceValue *>(unmanaged));
			if (unmanaged_reference != nullptr) {
				return AValue(unmanaged_reference);
			} else {
				delete unmanaged; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return AValue::kNil; /* we failed to allocate the whole unmanaged object, or just the ref. */
	}

	AValue
	AInteger64::allocate(const integer_64 value, void *env) noexcept {
		AInteger64 *object = new(std::nothrow) AInteger64(value);
		if (object != nullptr) {
			AReference *object_reference
				= new(std::nothrow) AReference(reinterpret_cast<AReferenceValue *>(object));
			if (object_reference != nullptr) {
				return AValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return AValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

	AValue
	AInteger64Unsigned::allocate(const unsigned_integer_64 value, void *env) noexcept {
		AInteger64Unsigned *object = new(std::nothrow) AInteger64Unsigned(value);
		if (object != nullptr) {
			AReference *object_reference
				= new(std::nothrow) AReference(reinterpret_cast<AReferenceValue *>(object));
			if (object_reference != nullptr) {
				return AValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return AValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

}
