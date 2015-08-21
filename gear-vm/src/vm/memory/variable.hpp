#ifndef __aml_vm__variable__
#define __aml_vm__variable__

#include "value.hpp"
#include "../psi/psi.hpp"

#include <atomic>

namespace AVM {
	typedef struct std::atomic<GValue> GVariable;

	struct GTypedVariable {
	public:
		GVariable value;
		PsiElement *const psi_type;

		GTypedVariable(GValue, PsiElement * /* psi_type */);
	};
}

#endif /* defined(__aml_vm__variable__) */
