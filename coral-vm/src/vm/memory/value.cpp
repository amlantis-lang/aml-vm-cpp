#include "value.hpp"
#include "value_inlines.hpp"

namespace CVM {

	const CValue CValue::kNil = CValue(CNil);
	const CValue CValue::kUndefined = CValue(CUndefined);
	const CValue CValue::kYes = CValue(CYes);
	const CValue CValue::kNo = CValue(CNo);
	const CValue CValue::kUnit = CValue(CUnit);

	const CValueLink CValue::kNilLink = CValue::encode(CValue::kNil);
	const CValueLink CValue::kUndefinedLink = CValue::encode(CValue::kUndefined);
	const CValueLink CValue::kYesLink = CValue::encode(CValue::kYes);
	const CValueLink CValue::kNoLink = CValue::encode(CValue::kNo);
	const CValueLink CValue::kUnitLink = CValue::encode(CValue::kUnit);

}
