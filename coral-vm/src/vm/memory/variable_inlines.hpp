#ifndef __coral_vm__variable_inlines__
#define __coral_vm__variable_inlines__

#include "variable.hpp"

namespace CVM {

	inline
	CTypedVariable::CTypedVariable(CValue value, void *psi_type)
		: value(value), psi_type(psi_type) { };

}

#endif
