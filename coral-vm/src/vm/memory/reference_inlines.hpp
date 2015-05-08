#ifndef __coral_vm__reference_inlines__
#define __coral_vm__reference_inlines__

#include "reference.hpp"

#include <cassert>

namespace CVM {

	inline
	CReference::CReference(union CReferenceValue *ref_value)
		:	referenced_value(ref_value),
			strong_reference_count(1),
			/* all strong references share one implicit weak reference, 
			 * to prevent this object from being deleted too soon */
			weak_reference_count(1) {}

	inline
	CReference::~CReference() noexcept {
		assert(strong_reference_count.load(std::memory_order_seq_cst) == 0);
		assert(weak_reference_count.load(std::memory_order_seq_cst) == 0);
		delete referenced_value.load(std::memory_order_acquire);
	}

	inline
	void
	CReference::retain(void *env) {
		strong_reference_count.fetch_add(1, std::memory_order_relaxed);
	}

	inline
	void
	CReference::release(void *env) {
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
	CValue
	CReference::downgrade(void *env) {
		CWeakPointer *weak_pointer = new(std::nothrow) CWeakPointer(this);
		if (weak_pointer != nullptr) {
			const CReference *weak_pointer_reference
				= new(std::nothrow) CReference(reinterpret_cast<CReferenceValue *>(weak_pointer));
			if (weak_pointer_reference != nullptr) {
				weak_reference_count.fetch_add(1, std::memory_order_relaxed);
				return CValue(weak_pointer_reference);
			} else {
				delete weak_pointer; /* we failed to allocate the reference */
			}
		}
		// TODO: call sth. like env->signal_out_of_memory()
		return CValue::kNil; /* we failed to allocate the whole weak pointer, or just the ref. */
	}

	inline
	CReferenceCommon::CReferenceCommon(CReferenceType ref_type)
		: reference_type(ref_type) { }

	inline
	CObject::CObject(void *psi_type, unsigned_integer_8 ivars_count)
		:	common(CReferenceTypeObject),
			psi_type(psi_type),
			dynamic_ivars(nullptr),
			ivars_count(ivars_count),
			ivars(new CVariable[ivars_count])
	{
		for (unsigned_integer_16 i = 0; i < ivars_count; i += 1) {
			ivars[i] = CValue::kUndefined;
		}
	}

	inline
	CWeakPointer::CWeakPointer(CReference *ptr)
		:	common(CReferenceTypeWeakPointer),
 			pointer(ptr) {}

	inline
	void
	CWeakPointer::on_release() noexcept {
		CReference *ptr = pointer.load(std::memory_order_seq_cst);
		assert(ptr != nullptr);
		if (ptr->weak_reference_count.fetch_sub(1, std::memory_order_release) == 1) {
			/* this only happens when this is the last weak reference */
			std::atomic_thread_fence(std::memory_order_acquire);
			delete ptr;
		}
	}

	inline
	CValue
	CWeakPointer::upgrade(void *env) {
		CReference *ptr = pointer.load(std::memory_order_seq_cst);
		assert(ptr != nullptr);
		while (true) {
			STRONG_REFERENCE_COUNT_TYPE ref_count = ptr->strong_reference_count.load(std::memory_order_seq_cst);
			if (ref_count == 0) {
				return CValue::kNil;
			} else if (ptr->strong_reference_count
								 .compare_exchange_weak(ref_count, ref_count + 1, std::memory_order_seq_cst)) {
				return CValue(ptr);
			}
		}
	}
}

#endif
