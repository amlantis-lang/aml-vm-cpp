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

#ifndef __coral_vm__value_inlines__
#define __coral_vm__value_inlines__

#include "value.hpp"
#include "../../lib/predef.hpp"
#include "../../lib/nan.hpp"

#include <cassert>

namespace CVM {

	inline
	CValueLink
	CValue::encode(CValue value) {
		return value.evd.as_int_64;
	};

	inline
	CValue
	CValue::decode(CValueLink encoded) {
		CValue decoded;
		decoded.evd.as_int_64 = encoded;
		return decoded;
	};

	inline
	float_64
	CValue::as_number() const {
		return as_float_64();
	}

	inline
	bool
	CValue::is_unsigned_integer_32() const {
		// technically, it would be only unsigned integer 31
		return is_integer_32() && as_integer_32() >= 0;
	}

	inline
	unsigned_integer_32
	CValue::as_unsigned_integer_32() const {
		assert(is_unsigned_integer_32());
		return as_integer_32();
	}

#if USE(CVALUE32_64)

	inline
	CValue::CValue() {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	CValue::CValue(CNilTag) {
		evd.as_bits.tag = NilTag;
		evd.as_bits.payload = 0;
	}

	inline
	CValue::CValue(CUndefinedTag) {
		evd.as_bits.tag = UndefinedTag;
		evd.as_bits.payload = 0;
	}

	inline
	CValue::CValue(CYesTag) {
		evd.as_bits.tag = BooleanTag;
		evd.as_bits.payload = 1;
	}

	inline
	CValue::CValue(CNoTag) {
		evd.as_bits.tag = BooleanTag;
		evd.as_bits.payload = 0;
	}

	inline
	CValue::CValue(CUnitTag) {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	CValue::CValue(CReference *ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(ptr);
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	CValue::CValue(const CReference *ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(const_cast<CReference *>(ptr));
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	CValue::CValue(EncodeAsFloat64Tag, float_64 value) {
		assert(!is_impure_NaN(value));
		evd.as_double = value;
	}

	inline
	bool
	CValue::operator==(const CValue &other) const {
		return evd.as_int_64 == other.evd.as_int_64;
	}

	inline
	bool
	CValue::operator!=(const CValue &other) const {
		return evd.as_int_64 != other.evd.as_int_64;
	}

	inline
	bool
	CValue::is_number() const {
		return is_integer_32() || is_float_64();
	}

	inline
	bool
	CValue::is_integer_32() const {
		return tag() == Int32Tag;
	}

	inline
	integer_32
	CValue::as_integer_32() const {
		assert(is_integer_32());
		return payload();
	}

	inline
	bool
	CValue::is_float_64() const {
		return tag() < LowestTag;
	}

	inline
	float_64
	CValue::as_float_64() const {
		assert(is_float_64());
		return evd.as_double;
	}

	inline
	bool
	CValue::is_a_yes() const {
		return is_boolean() && as_boolean();
	}

	inline
	bool
	CValue::is_a_no() const {
		return is_boolean() && !as_boolean();
	}

	inline
	bool
	CValue::is_boolean() const {
		return tag() == BooleanTag;
	}

	inline
	bool
	CValue::as_boolean() const {
		assert(is_boolean());
		return payload();
	}

	inline
	bool
	CValue::is_unit() const {
		return tag() == UnitValueTag;
	}

	inline
	bool
	CValue::is_nil() const {
		return tag() == NilTag;
	}

	inline
	bool
	CValue::is_undefined() const {
		return tag() == UndefinedTag;
	}

	inline
	bool
	CValue::is_nil_or_undefined() const {
		return is_nil() || is_undefined();
	}

	inline
	bool
	CValue::is_reference() const {
		return tag() == ReferenceTag;
	}

	inline
	CReference *
	CValue::as_reference() const {
		assert(is_reference());
		return reinterpret_cast<CReference *>(payload());
	}

	inline
	uint32_t
	CValue::tag() const {
		return evd.as_bits.tag;
	}

	inline
	int32_t
	CValue::payload() const {
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
	CValue::CValue() {
		evd.as_int_64 = ValueUnit;
	}

	inline
	CValue::CValue(CNilTag) {
		evd.as_int_64 = ValueNil;
	}

	inline
	CValue::CValue(CUndefinedTag) {
		evd.as_int_64 = ValueUndefined;
	}

	inline
	CValue::CValue(CYesTag) {
		evd.as_int_64 = ValueYes;
	}

	inline
	CValue::CValue(CNoTag) {
		evd.as_int_64 = ValueNo;
	}

	inline
	CValue::CValue(CUnitTag) {
		evd.as_int_64 = ValueUnit;
	}

	inline
	CValue::CValue(CReference *ptr) {
		if (ptr) {
			evd.as_int_64 = reinterpret_cast<uintptr_t>(ptr);
		} else {
			evd.as_int_64 = ValueNil;
		}
	}

	inline
	CValue::CValue(const CReference *ptr) {
		if (ptr) {
			evd.as_int_64 = reinterpret_cast<uintptr_t>(const_cast<CReference *>(ptr));
		} else {
			evd.as_int_64 = ValueNil;
		}
	}

	inline
	CValue::CValue(EncodeAsFloat64Tag, float_64 value) {
		assert(!is_impure_NaN(value));
		evd.as_int_64 = reinterpret_float_64_to_integer_64(value) + DoubleEncodeOffset;
	}

	inline
	bool
	CValue::operator==(const CValue &other) const {
		return evd.as_int_64 == other.evd.as_int_64;
	}

	inline
	bool
	CValue::operator!=(const CValue &other) const {
		return evd.as_int_64 != other.evd.as_int_64;
	}

	inline
	bool
	CValue::is_number() const {
		return evd.as_int_64 & TagTypeNumber;
	}

	inline
	bool
	CValue::is_integer_32() const {
		return (evd.as_int_64 & TagTypeNumber) == TagTypeNumber;
	}

	inline
	integer_32
	CValue::as_integer_32() const {
		assert(is_integer_32());
		return static_cast<integer_32>(evd.as_int_64);
	}

	inline
	bool
	CValue::is_float_64() const {
		return is_number() && !is_integer_32();
	}

	inline
	float_64
	CValue::as_float_64() const {
		assert(is_float_64());
		return reinterpret_integer_64_to_float_64(evd.as_int_64 - DoubleEncodeOffset);
	}

	inline
	bool
	CValue::is_a_yes() const {
		return as_value() == CValue::kYes;
	}

	inline
	bool
	CValue::is_a_no() const {
		return as_value() == CValue::kNo;
	}

	inline
	bool
	CValue::is_boolean() const {
		return (evd.as_int_64 & ~1) == ValueNo;
	}

	inline
	bool
	CValue::as_boolean() const {
		assert(is_boolean());
		return as_value() == CValue::kYes;
	}

	inline
	bool
	CValue::is_unit() const {
		return evd.as_int_64 == ValueUnit;
	}

	inline
	bool
	CValue::is_nil() const {
		return as_value() == CValue::kNil;
	}

	inline
	bool
	CValue::is_undefined() const {
		return as_value() == CValue::kUndefined;
	}

	inline
	bool
	CValue::is_nil_or_undefined() const {
		return is_nil() || is_undefined();
	}

	inline
	bool
	CValue::is_reference() const {
		return !(evd.as_int_64 & TagMask);
	}

	inline
	CReference *
	CValue::as_reference() const {
		assert(is_reference());
		return evd.as_pointer;
	}

#endif /* USE(CVALUE64) */
	
}

#endif
