#ifndef __gear_vm__variable_inlines__
#define __gear_vm__variable_inlines__

#include "variable.hpp"

namespace GVM {

	inline
	GTypedVariable::GTypedVariable(GValue value, PsiElement *psi_type)
		:	value(value),
 			psi_type(psi_type) {};

}

#endif
