#include "reference.hpp"
#include "reference_inlines.hpp"

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
}
