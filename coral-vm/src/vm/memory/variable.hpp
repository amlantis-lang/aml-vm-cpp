#ifndef __coral_vm__variable__
#define __coral_vm__variable__

#include "value.hpp"

#include <atomic>

namespace CVM {
	typedef struct std::atomic<CValue> CVariable;

	struct CTypedVariable {
	public:
		CVariable value;
		void *const psi_type;

		CTypedVariable(CValue, void * /* psi_type */);
	};
}

#endif /* defined(__coral_vm__variable__) */
