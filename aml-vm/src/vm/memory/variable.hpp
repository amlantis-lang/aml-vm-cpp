#ifndef __aml_vm__variable__
#define __aml_vm__variable__

#include "value.hpp"
#include "../psi/psi.hpp"

#include <atomic>

namespace AVM {
	typedef struct std::atomic<AValue> AVariable;

	struct ATypedVariable {
	public:
		AVariable value;
		PsiElement *const psi_type;

		ATypedVariable(AValue, PsiElement * /* psi_type */);
	};
}

#endif /* defined(__aml_vm__variable__) */
