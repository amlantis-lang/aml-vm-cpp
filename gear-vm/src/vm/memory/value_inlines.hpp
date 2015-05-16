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

#ifndef __gear_vm__value_inlines__
#define __gear_vm__value_inlines__

#include "value.hpp"
#include "../../lib/predef.hpp"
#include "../../lib/nan.hpp"

#include <cassert>

namespace GVM {

	inline
	GValueLink
	GValue::encode(GValue value) {
		return value.evd.as_int_64;
	};

	inline
	GValue
	GValue::decode(GValueLink encoded) {
		GValue decoded;
		decoded.evd.as_int_64 = encoded;
		return decoded;
	};

	inline
	float_64
	GValue::as_number() const {
		return as_float_64();
	}

	inline
	bool
	GValue::is_unsigned_integer_32() const {
		// technically, it would be only unsigned integer 31
		return is_integer_32() && as_integer_32() >= 0;
	}

	inline
	unsigned_integer_32
	GValue::as_unsigned_integer_32() const {
		assert(is_unsigned_integer_32());
		return as_integer_32();
	}

#if USE(CVALUE32_64)

	inline
	GValue::GValue() {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	GValue::GValue(GNilTag) {
		evd.as_bits.tag = NilTag;
		evd.as_bits.payload = 0;
	}

	inline
	GValue::GValue(GUndefinedTag) {
		evd.as_bits.tag = UndefinedTag;
		evd.as_bits.payload = 0;
	}

	inline
	GValue::GValue(GYesTag) {
		evd.as_bits.tag = BooleanTag;
		evd.as_bits.payload = 1;
	}

	inline
	GValue::GValue(GNoTag) {
		evd.as_bits.tag = BooleanTag;
		evd.as_bits.payload = 0;
	}

	inline
	GValue::GValue(GUnitTag) {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	GValue::GValue(GReference *ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(ptr);
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	GValue::GValue(const GReference *ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(const_cast<GReference *>(ptr));
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	GValue::GValue(EncodeAsFloat64Tag, float_64 value) {
		assert(!is_impure_NaN(value));
		evd.as_double = value;
	}

	inline
	bool
	GValue::operator==(const GValue &other) const {
		return evd.as_int_64 == other.evd.as_int_64;
	}

	inline
	bool
	GValue::operator!=(const GValue &other) const {
		return evd.as_int_64 != other.evd.as_int_64;
	}

	inline
	bool
	GValue::is_number() const {
		return is_integer_32() || is_float_64();
	}

	inline
	bool
	GValue::is_integer_32() const {
		return tag() == Int32Tag;
	}

	inline
	integer_32
	GValue::as_integer_32() const {
		assert(is_integer_32());
		return payload();
	}

	inline
	bool
	GValue::is_float_64() const {
		return tag() < LowestTag;
	}

	inline
	float_64
	GValue::as_float_64() const {
		assert(is_float_64());
		return evd.as_double;
	}

	inline
	bool
	GValue::is_a_yes() const {
		return is_boolean() && as_boolean();
	}

	inline
	bool
	GValue::is_a_no() const {
		return is_boolean() && !as_boolean();
	}

	inline
	bool
	GValue::is_boolean() const {
		return tag() == BooleanTag;
	}

	inline
	bool
	GValue::as_boolean() const {
		assert(is_boolean());
		return payload();
	}

	inline
	bool
	GValue::is_unit() const {
		return tag() == UnitValueTag;
	}

	inline
	bool
	GValue::is_nil() const {
		return tag() == NilTag;
	}

	inline
	bool
	GValue::is_undefined() const {
		return tag() == UndefinedTag;
	}

	inline
	bool
	GValue::is_nil_or_undefined() const {
		return is_nil() || is_undefined();
	}

	inline
	bool
	GValue::is_reference() const {
		return tag() == ReferenceTag;
	}

	inline
	GReference *
	GValue::as_reference() const {
		assert(is_reference());
		return reinterpret_cast<GReference *>(payload());
	}

	inline
	uint32_t
	GValue::tag() const {
		return evd.as_bits.tag;
	}

	inline
	int32_t
	GValue::payload() const {
		return evd.as_bits.payload;
	}

#elif USE(CVALUE64)

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
	GValue::GValue() {
		evd.as_int_64 = ValueUnit;
	}

	inline
	GValue::GValue(GNilTag) {
		evd.as_int_64 = ValueNil;
	}

	inline
	GValue::GValue(GUndefinedTag) {
		evd.as_int_64 = ValueUndefined;
	}

	inline
	GValue::GValue(GYesTag) {
		evd.as_int_64 = ValueYes;
	}

	inline
	GValue::GValue(GNoTag) {
		evd.as_int_64 = ValueNo;
	}

	inline
	GValue::GValue(GUnitTag) {
		evd.as_int_64 = ValueUnit;
	}

	inline
	GValue::GValue(struct GReference *ptr) {
		if (ptr) {
			evd.as_int_64 = reinterpret_cast<uintptr_t>(ptr);
		} else {
			evd.as_int_64 = ValueNil;
		}
	}

	inline
	GValue::GValue(const struct GReference *ptr) {
		if (ptr) {
			evd.as_int_64 = reinterpret_cast<uintptr_t>(const_cast<GReference *>(ptr));
		} else {
			evd.as_int_64 = ValueNil;
		}
	}

	inline
	GValue::GValue(EncodeAsFloat64Tag, float_64 value) {
		assert(!is_impure_NaN(value));
		evd.as_int_64 = reinterpret_float_64_to_integer_64(value) + DoubleEncodeOffset;
	}

	inline
	bool
	GValue::operator==(const GValue &other) const {
		return evd.as_int_64 == other.evd.as_int_64;
	}

	inline
	bool
	GValue::operator!=(const GValue &other) const {
		return evd.as_int_64 != other.evd.as_int_64;
	}

	inline
	bool
	GValue::is_number() const {
		return evd.as_int_64 & TagTypeNumber;
	}

	inline
	bool
	GValue::is_integer_32() const {
		return (evd.as_int_64 & TagTypeNumber) == TagTypeNumber;
	}

	inline
	integer_32
	GValue::as_integer_32() const {
		assert(is_integer_32());
		return static_cast<integer_32>(evd.as_int_64);
	}

	inline
	bool
	GValue::is_float_64() const {
		return is_number() && !is_integer_32();
	}

	inline
	float_64
	GValue::as_float_64() const {
		assert(is_float_64());
		return reinterpret_integer_64_to_float_64(evd.as_int_64 - DoubleEncodeOffset);
	}

	inline
	bool
	GValue::is_a_yes() const {
		return as_value() == GValue::kYes;
	}

	inline
	bool
	GValue::is_a_no() const {
		return as_value() == GValue::kNo;
	}

	inline
	bool
	GValue::is_boolean() const {
		return (evd.as_int_64 & ~1) == ValueNo;
	}

	inline
	bool
	GValue::as_boolean() const {
		assert(is_boolean());
		return as_value() == GValue::kYes;
	}

	inline
	bool
	GValue::is_unit() const {
		return evd.as_int_64 == ValueUnit;
	}

	inline
	bool
	GValue::is_nil() const {
		return as_value() == GValue::kNil;
	}

	inline
	bool
	GValue::is_undefined() const {
		return as_value() == GValue::kUndefined;
	}

	inline
	bool
	GValue::is_nil_or_undefined() const {
		return is_nil() || is_undefined();
	}

	inline
	bool
	GValue::is_reference() const {
		return !(evd.as_int_64 & TagMask);
	}

	inline
	GReference *
	GValue::as_reference() const {
		assert(is_reference());
		return evd.as_pointer;
	}

#endif /* USE(CVALUE64) */
	
}

#endif
