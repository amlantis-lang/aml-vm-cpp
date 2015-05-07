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

#include "value.hpp"
#include "../../lib/predef.hpp"

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
		evd.as_bits.payload = 1;
	}

	inline
	CValue::CValue(CUnitTag) {
		evd.as_bits.tag = UnitValueTag;
		evd.as_bits.payload = 0;
	}

	inline
	CValue::CValue(void* ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(ptr);
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
	}

	inline
	CValue::CValue(const void* ptr) {
		if (ptr) {
			evd.as_bits.tag = ReferenceTag;
			evd.as_bits.payload = reinterpret_cast<int32_t>(const_cast<void *>(ptr));
		} else {
			evd.as_bits.tag = NilTag;
			evd.as_bits.payload = 0;
		}
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

#elif USE(CVALUE64)

#endif

}
