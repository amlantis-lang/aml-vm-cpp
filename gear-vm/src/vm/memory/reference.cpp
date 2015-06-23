#include "reference.hpp"
#include "reference_inlines.hpp"
#include "value.hpp"
#include "value_inlines.hpp"

#include <cassert>

namespace GVM {
	static GReferenceLock ReferenceLocks[REFERENCE_LOCKS_COUNT];

	GReferenceLock &
	lock_for_reference(union GReferenceValue *reference) noexcept {
		return ReferenceLocks[REFERENCE_LOCK_INDEX(reference)];
	}

	void
	GReference::release_slow(void *env) {
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		/* since the release has come to this point, there are no more strong references */
		GReferenceValue *pointer = referenced_value.load(std::memory_order_relaxed);

		/* TODO: call sth. like env->destruct(pointer); !!! (maybe better in each switch case, to save a check) */

		switch (pointer->common.reference_type) {
			case GReferenceTypeObject: {
				GObject *real_pointer = reinterpret_cast<GObject *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeUnmanagedUnsafe: {
				GUnmanagedUnsafe *real_pointer = reinterpret_cast<GUnmanagedUnsafe *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeInteger64: {
				GInteger64 *real_pointer = reinterpret_cast<GInteger64 *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeUnsignedInteger64: {
				GInteger64Unsigned *real_pointer = reinterpret_cast<GInteger64Unsigned *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeInteger128: {
				GInteger128 *real_pointer = reinterpret_cast<GInteger128 *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeUnsignedInteger128: {
				GInteger128Unsigned *real_pointer = reinterpret_cast<GInteger128Unsigned *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeFloat128: {
				GFloat128 *real_pointer = reinterpret_cast<GFloat128 *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeComplex: {
				GComplex *real_pointer = reinterpret_cast<GComplex *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeRational: {
				GRational *real_pointer = reinterpret_cast<GRational *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeDecimalLimited: {
				GDecimalLimited *real_pointer = reinterpret_cast<GDecimalLimited *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeDecimalUnlimited: {
				GDecimalUnlimited *real_pointer = reinterpret_cast<GDecimalUnlimited *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeArray: {
				GArray *real_pointer = reinterpret_cast<GArray *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeNestedPointer: {
				GNestedPointer *real_pointer = reinterpret_cast<GNestedPointer *>(pointer);
				delete real_pointer;
				break;
			}
			case GReferenceTypeWeakPointer: {
				GWeakPointer *real_pointer = reinterpret_cast<GWeakPointer *>(pointer);
				/* maybe move this to destructor from *env */
				real_pointer->on_release();
				delete real_pointer;
				break;
			}
			case GReferenceTypePsiBearer: {
				GPsiBearer *real_pointer = reinterpret_cast<GPsiBearer *>(pointer);
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

	GValue
	GObject::allocate(PsiElement *psi_type, void *env) noexcept {
		/* TODO: read number of ivars from psi_type */
		GObject *object = new(std::nothrow) GObject(psi_type, 0);
		if (object != nullptr) {
			GReference *object_reference
				= new(std::nothrow) GReference(reinterpret_cast<GReferenceValue *>(object));
			if (object_reference != nullptr) {
				return GValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return GValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

	GValue
	GUnmanagedUnsafe::allocate(void *pointer, void (*deallocator)(void *), void *env) noexcept {
		GUnmanagedUnsafe *unmanaged = new(std::nothrow) GUnmanagedUnsafe(pointer, deallocator);
		if (unmanaged != nullptr) {
			GReference *unmanaged_reference
				= new(std::nothrow) GReference(reinterpret_cast<GReferenceValue *>(unmanaged));
			if (unmanaged_reference != nullptr) {
				return GValue(unmanaged_reference);
			} else {
				delete unmanaged; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return GValue::kNil; /* we failed to allocate the whole unmanaged object, or just the ref. */
	}

	GValue
	GInteger64::allocate(const integer_64 value, void *env) noexcept {
		GInteger64 *object = new(std::nothrow) GInteger64(value);
		if (object != nullptr) {
			GReference *object_reference
				= new(std::nothrow) GReference(reinterpret_cast<GReferenceValue *>(object));
			if (object_reference != nullptr) {
				return GValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return GValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

	GValue
	GInteger64Unsigned::allocate(const unsigned_integer_64 value, void *env) noexcept {
		GInteger64Unsigned *object = new(std::nothrow) GInteger64Unsigned(value);
		if (object != nullptr) {
			GReference *object_reference
				= new(std::nothrow) GReference(reinterpret_cast<GReferenceValue *>(object));
			if (object_reference != nullptr) {
				return GValue(object_reference);
			} else {
				delete object; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return GValue::kNil; /* we failed to allocate the whole object, or just the ref. */
	}

}
