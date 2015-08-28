#ifndef __aml_vm__variable_inlines__
#define __aml_vm__variable_inlines__

#include "variable.hpp"

namespace AVM {

	inline
	ATypedVariable::ATypedVariable(AValue value, PsiElement *psi_type)
		:	value(value),
 			psi_type(psi_type) {};

}

#endif
