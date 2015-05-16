#ifndef __gear_vm__variable_inlines__
#define __gear_vm__variable_inlines__

#include "variable.hpp"

namespace GVM {

	inline
	GTypedVariable::GTypedVariable(GValue value, void *psi_type)
		: value(value), psi_type(psi_type) { };

}

#endif
