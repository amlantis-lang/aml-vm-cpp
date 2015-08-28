#include "value.hpp"
#include "value_inlines.hpp"

namespace AVM {

	const AValue AValue::kNil = AValue(ANil);
	const AValue AValue::kUndefined = AValue(AUndefined);
	const AValue AValue::kYes = AValue(AYes);
	const AValue AValue::kNo = AValue(ANo);
	const AValue AValue::kUnit = AValue(AUnit);

	const AValueLink AValue::kNilLink = AValue::encode(AValue::kNil);
	const AValueLink AValue::kUndefinedLink = AValue::encode(AValue::kUndefined);
	const AValueLink AValue::kYesLink = AValue::encode(AValue::kYes);
	const AValueLink AValue::kNoLink = AValue::encode(AValue::kNo);
	const AValueLink AValue::kUnitLink = AValue::encode(AValue::kUnit);

}
