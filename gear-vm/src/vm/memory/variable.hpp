#ifndef __gear_vm__variable__
#define __gear_vm__variable__

#include "value.hpp"
#include "../psi/psi.hpp"

#include <atomic>

namespace GVM {
	typedef struct std::atomic<GValue> GVariable;

	struct GTypedVariable {
	public:
		GVariable value;
		PsiElement *const psi_type;

		GTypedVariable(GValue, PsiElement * /* psi_type */);
	};
}

#endif /* defined(__gear_vm__variable__) */
