#include "reference.hpp"
#include "reference_inlines.hpp"
#include "value.hpp"
#include "value_inlines.hpp"

#include <cassert>

namespace CVM {
	static CReferenceLock ReferenceLocks[REFERENCE_LOCKS_COUNT];

	CReferenceLock &
	lock_for_reference(union CReferenceValue *reference) noexcept {
		return ReferenceLocks[REFERENCE_LOCK_INDEX(reference)];
	}

	void
	CReference::release_slow(void *env) {
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		/* since the release has come to this point, there are no more strong references */
		CReferenceValue *pointer = referenced_value.load(std::memory_order_relaxed);

		/* TODO: call sth. like env->destruct(pointer); !!! (maybe better in each switch case, to save a check) */

		switch (pointer->common.reference_type) {
			case CReferenceTypeObject: {
				CObject *real_pointer = reinterpret_cast<CObject *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeUnmanagedUnsafe: {
				CUnmanagedUnsafe *real_pointer = reinterpret_cast<CUnmanagedUnsafe *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeInteger64: {
				CInteger64 *real_pointer = reinterpret_cast<CInteger64 *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeUnsignedInteger64: {
				CInteger64Unsigned *real_pointer = reinterpret_cast<CInteger64Unsigned *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeInteger128: {
				CInteger128 *real_pointer = reinterpret_cast<CInteger128 *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeUnsignedInteger128: {
				CInteger128Unsigned *real_pointer = reinterpret_cast<CInteger128Unsigned *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeFloat128: {
				CFloat128 *real_pointer = reinterpret_cast<CFloat128 *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeComplex: {
				CComplex *real_pointer = reinterpret_cast<CComplex *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeRational: {
				CRational *real_pointer = reinterpret_cast<CRational *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeDecimalLimited: {
				CDecimalLimited *real_pointer = reinterpret_cast<CDecimalLimited *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeDecimalUnlimited: {
				CDecimalUnlimited *real_pointer = reinterpret_cast<CDecimalUnlimited *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeArray: {
				CArray *real_pointer = reinterpret_cast<CArray *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeNestedPointer: {
				CNestedPointer *real_pointer = reinterpret_cast<CNestedPointer *>(pointer);
				delete real_pointer;
				break;
			}
			case CReferenceTypeWeakPointer: {
				CWeakPointer *real_pointer = reinterpret_cast<CWeakPointer *>(pointer);
				/* maybe move this to destructor from *env */
				real_pointer->on_release();
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

	CValue
	CObject::allocate(void *psi_type, void *env) noexcept {
		/* TODO: read number of ivars from psi_type */
		CObject *object = new(std::nothrow) CObject(psi_type, 0);
		if (object != nullptr) {
			CReference *object_reference
				= new(std::nothrow) CReference(reinterpret_cast<CReferenceValue *>(object));
			if (object_reference != nullptr) {
				return CValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return CValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

	CValue
	CUnmanagedUnsafe::allocate(void *pointer, void (*deallocator)(void *), void *env) noexcept {
		CUnmanagedUnsafe *unmanaged = new(std::nothrow) CUnmanagedUnsafe(pointer, deallocator);
		if (unmanaged != nullptr) {
			CReference *unmanaged_reference
				= new(std::nothrow) CReference(reinterpret_cast<CReferenceValue *>(unmanaged));
			if (unmanaged_reference != nullptr) {
				return CValue(unmanaged_reference);
			} else {
				delete unmanaged; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return CValue::kNil; /* we failed to allocate the whole unmanaged object, or just the ref. */
	}

	CValue
	CInteger64::allocate(const integer_64 value, void *env) noexcept {
		CInteger64 *object = new(std::nothrow) CInteger64(value);
		if (object != nullptr) {
			CReference *object_reference
				= new(std::nothrow) CReference(reinterpret_cast<CReferenceValue *>(object));
			if (object_reference != nullptr) {
				return CValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return CValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

	CValue
	CInteger64Unsigned::allocate(const unsigned_integer_64 value, void *env) noexcept {
		CInteger64Unsigned *object = new(std::nothrow) CInteger64Unsigned(value);
		if (object != nullptr) {
			CReference *object_reference
				= new(std::nothrow) CReference(reinterpret_cast<CReferenceValue *>(object));
			if (object_reference != nullptr) {
				return CValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return CValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

}
