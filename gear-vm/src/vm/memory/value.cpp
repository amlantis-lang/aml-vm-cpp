#include "value.hpp"
#include "value_inlines.hpp"

namespace GVM {

	const GValue GValue::kNil = GValue(GNil);
	const GValue GValue::kUndefined = GValue(GUndefined);
	const GValue GValue::kYes = GValue(GYes);
	const GValue GValue::kNo = GValue(GNo);
	const GValue GValue::kUnit = GValue(GUnit);

	const GValueLink GValue::kNilLink = GValue::encode(GValue::kNil);
	const GValueLink GValue::kUndefinedLink = GValue::encode(GValue::kUndefined);
	const GValueLink GValue::kYesLink = GValue::encode(GValue::kYes);
	const GValueLink GValue::kNoLink = GValue::encode(GValue::kNo);
	const GValueLink GValue::kUnitLink = GValue::encode(GValue::kUnit);

}
