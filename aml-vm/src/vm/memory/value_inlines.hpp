/*
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __aml_vm__value_inlines__
#define __aml_vm__value_inlines__

#include "value.hpp"
#include "../../lib/predef.hpp"
#include "../../lib/nan.hpp"

#include <cassert>

namespace AVM {

	inline
	AValueLink
	AValue::encode(AValue value) {
		return value.evd.as_int_64;
	};

	inline
	AValue
	AValue::decode(AValueLink encoded) {
		AValue decoded;
		decoded.evd.as_int_64 = encoded;
		return decoded;
	};

	inline
	float_64
	AValue::as_number() const {
		return as_float_64();
	}

	inline
	bool
	AValue::is_unsigned_integer_32() const {
		// technically, it would be only unsigned integer 31
		return is_integer_32() && as_integer_32() >= 0;
	}

	inline
	unsigned_integer_32
	AValue::as_unsigned_integer_32() const {
		assert(is_unsigned_integer_32());
		return as_integer_32();
	}

#if USE(AVALUE32_64)

	inline
	AValue::AValue() {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	AValue::AValue(ANilTag) {
		evd.as_bits.tag = NilTag;
		evd.as_bits.payload = 0;
	}

	inline
	AValue::AValue(AUndefinedTag) {
		evd.as_bits.tag = UndefinedTag;
		evd.as_bits.payload = 0;
	}

	inline
	AValue::AValue(AYesTag) {
		evd.as_bits.tag = BooleanTag;
		evd.as_bits.payload = 1;
	}

	inline
	AValue::AValue(ANoTag) {
		evd.as_bits.tag = BooleanTag;
		evd.as_bits.payload = 0;
	}

	inline
	AValue::AValue(AUnitTag) {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	AValue::AValue(AReference *ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(ptr);
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	AValue::AValue(const AReference *ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(const_cast<AReference *>(ptr));
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	AValue::AValue(EncodeAsFloat64Tag, float_64 value) {
		assert(!is_impure_NaN(value));
		evd.as_double = value;
	}

	inline
	bool
	AValue::operator==(const AValue &other) const {
		return evd.as_int_64 == other.evd.as_int_64;
	}

	inline
	bool
	AValue::operator!=(const AValue &other) const {
		return evd.as_int_64 != other.evd.as_int_64;
	}

	inline
	bool
	AValue::is_number() const {
		return is_integer_32() || is_float_64();
	}

	inline
	bool
	AValue::is_integer_32() const {
		return tag() == Int32Tag;
	}

	inline
	integer_32
	AValue::as_integer_32() const {
		assert(is_integer_32());
		return payload();
	}

	inline
	bool
	AValue::is_float_64() const {
		return tag() < LowestTag;
	}

	inline
	float_64
	AValue::as_float_64() const {
		assert(is_float_64());
		return evd.as_double;
	}

	inline
	bool
	AValue::is_a_yes() const {
		return is_boolean() && as_boolean();
	}

	inline
	bool
	AValue::is_a_no() const {
		return is_boolean() && !as_boolean();
	}

	inline
	bool
	AValue::is_boolean() const {
		return tag() == BooleanTag;
	}

	inline
	bool
	AValue::as_boolean() const {
		assert(is_boolean());
		return payload();
	}

	inline
	bool
	AValue::is_unit() const {
		return tag() == UnitValueTag;
	}

	inline
	bool
	AValue::is_nil() const {
		return tag() == NilTag;
	}

	inline
	bool
	AValue::is_undefined() const {
		return tag() == UndefinedTag;
	}

	inline
	bool
	AValue::is_nil_or_undefined() const {
		return is_nil() || is_undefined();
	}

	inline
	bool
	AValue::is_reference() const {
		return tag() == ReferenceTag;
	}

	inline
	AReference *
	AValue::as_reference() const {
		assert(is_reference());
		return reinterpret_cast<AReference *>(payload());
	}

	inline
	uint32_t
	AValue::tag() const {
		return evd.as_bits.tag;
	}

	inline
	int32_t
	AValue::payload() const {
		return evd.as_bits.payload;
	}

#elif USE(AVALUE64)

	inline
	integer_64
	reinterpret_float_64_to_integer_64(float_64 value) {
		return bitwise_cast<integer_64>(value);
	}

	inline
	float_64
	reinterpret_integer_64_to_float_64(integer_64 value) {
		return bitwise_cast<float_64>(value);
	}

	// 0x0 can never occur naturally because it has a tag of 00, indicating a pointer value, but a payload of 0x0, which is in the (invalid) zero page.
	inline
	AValue::AValue() {
		evd.as_int_64 = ValueUnit;
	}

	inline
	AValue::AValue(ANilTag) {
		evd.as_int_64 = ValueNil;
	}

	inline
	AValue::AValue(AUndefinedTag) {
		evd.as_int_64 = ValueUndefined;
	}

	inline
	AValue::AValue(AYesTag) {
		evd.as_int_64 = ValueYes;
	}

	inline
	AValue::AValue(ANoTag) {
		evd.as_int_64 = ValueNo;
	}

	inline
	AValue::AValue(AUnitTag) {
		evd.as_int_64 = ValueUnit;
	}

	inline
	AValue::AValue(struct AReference *ptr) {
		if (ptr) {
			evd.as_int_64 = reinterpret_cast<uintptr_t>(ptr);
		} else {
			evd.as_int_64 = ValueNil;
		}
	}

	inline
	AValue::AValue(const struct AReference *ptr) {
		if (ptr) {
			evd.as_int_64 = reinterpret_cast<uintptr_t>(const_cast<AReference *>(ptr));
		} else {
			evd.as_int_64 = ValueNil;
		}
	}

	inline
	AValue::AValue(EncodeAsFloat64Tag, float_64 value) {
		assert(!is_impure_NaN(value));
		evd.as_int_64 = reinterpret_float_64_to_integer_64(value) + DoubleEncodeOffset;
	}

	inline
	bool
	AValue::operator==(const AValue &other) const {
		return evd.as_int_64 == other.evd.as_int_64;
	}

	inline
	bool
	AValue::operator!=(const AValue &other) const {
		return evd.as_int_64 != other.evd.as_int_64;
	}

	inline
	bool
	AValue::is_number() const {
		return evd.as_int_64 & TagTypeNumber;
	}

	inline
	bool
	AValue::is_integer_32() const {
		return (evd.as_int_64 & TagTypeNumber) == TagTypeNumber;
	}

	inline
	integer_32
	AValue::as_integer_32() const {
		assert(is_integer_32());
		return static_cast<integer_32>(evd.as_int_64);
	}

	inline
	bool
	AValue::is_float_64() const {
		return is_number() && !is_integer_32();
	}

	inline
	float_64
	AValue::as_float_64() const {
		assert(is_float_64());
		return reinterpret_integer_64_to_float_64(evd.as_int_64 - DoubleEncodeOffset);
	}

	inline
	bool
	AValue::is_a_yes() const {
		return as_value() == AValue::kYes;
	}

	inline
	bool
	AValue::is_a_no() const {
		return as_value() == AValue::kNo;
	}

	inline
	bool
	AValue::is_boolean() const {
		return (evd.as_int_64 & ~1) == ValueNo;
	}

	inline
	bool
	AValue::as_boolean() const {
		assert(is_boolean());
		return as_value() == AValue::kYes;
	}

	inline
	bool
	AValue::is_unit() const {
		return evd.as_int_64 == ValueUnit;
	}

	inline
	bool
	AValue::is_nil() const {
		return as_value() == AValue::kNil;
	}

	inline
	bool
	AValue::is_undefined() const {
		return as_value() == AValue::kUndefined;
	}

	inline
	bool
	AValue::is_nil_or_undefined() const {
		return is_nil() || is_undefined();
	}

	inline
	bool
	AValue::is_reference() const {
		return !(evd.as_int_64 & TagMask);
	}

	inline
	AReference *
	AValue::as_reference() const {
		assert(is_reference());
		return evd.as_pointer;
	}

#endif /* USE(AVALUE64) */
	
}

#endif
