/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2007, 2008, 2009, 2012 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef __coral_vm__value__
#define __coral_vm__value__

#include <cstdint>
#include <cstddef>

#include "../../lib/predef.hpp"

namespace CVM {
	typedef int64_t CValueLink;
	struct CReference;

	union EncodedValueDescriptor {
		int64_t as_int_64;
# if USE(CVALUE32_64)
		double as_double;
# elif USE(CVALUE64)
		struct CReference *as_pointer;
# endif
# if CPU(BIG_ENDIAN)
		struct {
			uint32_t tag;
			int32_t payload;
		} as_bits;
# else
		struct {
			int32_t payload;
			uint32_t tag;
		} as_bits;
# endif
	};

	class CValue {
	public:

#  if USE(CVALUE32_64)
		enum { Int32Tag =        0xffffffff };
		enum { BooleanTag =      0xfffffffe };
		enum { NilTag =          0xfffffffd };
		enum { UndefinedTag =    0xfffffffc };
		enum { ReferenceTag =    0xfffffffb };
		enum { UnitValueTag =    0xfffffffa };

		enum { LowestTag =     UnitValueTag };
#  endif

		static CValueLink
		encode(CValue);

		static CValue
		decode(CValueLink);

		enum CNilTag { CNil };
		enum CUndefinedTag { CUndefined };
		enum CYesTag { CYes };
		enum CNoTag { CNo };
		enum CUnitTag { CUnit };
		enum EncodeAsFloat64Tag { EncodeAsFloat64 };

		/* non-numeric values */
		CValue();
		CValue(CNilTag);
		CValue(CUndefinedTag);
		CValue(CYesTag);
		CValue(CNoTag);
		CValue(CUnitTag);
		CValue(CReference *ptr);
		CValue(const CReference *ptr);

		/* numeric values */
		CValue(EncodeAsFloat64Tag, float_64);

		// TODO: add more constructors, per each reference subtype

		bool
		operator==(const CValue &other) const;

		bool
		operator!=(const CValue &other) const;

		bool
		is_number() const;

		float_64
		as_number() const;

		bool
		is_integer_32() const;

		integer_32
		as_integer_32() const;

		bool
		is_unsigned_integer_32() const;

		unsigned_integer_32
		as_unsigned_integer_32() const;

		bool
		is_float_64() const;

		float_64
		as_float_64() const;

		bool
		is_a_yes() const;

		bool
		is_a_no() const;

		bool
		is_boolean() const;

		bool
		as_boolean() const;

		bool
		is_unit() const; // also getter

		bool
		is_nil() const; // also getter

		bool
		is_undefined() const; // also getter

		bool
		is_nil_or_undefined() const;

		bool
		is_reference() const;

		struct CReference *
		as_reference() const;

#  if USE(CVALUE32_64)
		/* On 32-bit platforms USE(CVALUE32_64) should be defined, and we use a NaN-encoded
		 * form for immediates.
		 *
		 * The encoding makes use of unused NaN space in the IEEE754 representation. Any value
		 * with the top 13 bits set represents a QNaN (with the sign bit set). qNaN values
		 * can encode a 51-bit payload. Hardware produced and C-library payloads typically
		 * have a payload of zero. We assume that non-zero payloads are available to encode
		 * pointer and integer values. Since any 64-bit bit pattern where the top 15 bits are
		 * all set represents a NaN with a non-zero payload, we can use this space in the NaN
		 * ranges to encode other values (however there are also other ranges of NaN space that
		 * could have been selected - let's ignore these for now).
		 *
		 * For CValues that do not contain a double value, the high 32 bits contain the tag
		 * values listed in the enums below, which all correspond to NaN-space. In the case of
		 * reference, integer and boolean values, the lower 32 bits (the 'payload') contain the pointer,
		 * integer or boolean value; in the case of all other tags the payload is 0.
		 */
		uint32_t tag() const;
		int32_t payload() const;

#  elif USE(CVALUE64)
		/* On 64-bit platforms USE(CVALUE64) should be defined, and we use a NaN-encoded
		 * form for immediates.
		 *
		 * The encoding makes use of unused NaN space in the IEEE754 representation. Any value
		 * with the top 13 bits set represents a QNaN (with the sign bit set). qNaN values
		 * can encode a 51-bit payload. Hardware produced and C-library payloads typically
		 * have a payload of zero. We assume that non-zero payloads are available to encode
		 * pointer and integer values. Since any 64-bit bit pattern where the top 15 bits are
		 * all set represents a NaN with a non-zero payload, we can use this space in the NaN
		 * ranges to encode other values (however there are also other ranges of NaN space that
		 * could have been selected - let's ignore these for now).
		 *
		 * This range of NaN space is represented by 64-bit numbers begining with the 16-bit
		 * hex patterns 0xFFFE and 0xFFFF - we rely on the fact that no valid double-precision
		 * numbers will fall in these ranges.
		 *
		 * The top 16-bits denote the type of the encoded CValue:
		 *
		 *     Pointer {  0000:PPPP:PPPP:PPPP
		 *              / 0001:****:****:****
		 *     Double  {         ...
		 *              \ FFFE:****:****:****
		 *     Integer {  FFFF:0000:IIII:IIII
		 *
		 * The scheme we have implemented encodes double precision values by performing a
		 * 64-bit integer addition of the value 2^48 to the number. After this manipulation
		 * no encoded double-precision value will begin with the pattern 0x0000 or 0xFFFF.
		 * Values must be decoded by reversing this operation before subsequent floating point
		 * operations may be peformed.
		 *
		 * 32-bit signed integers are marked with the 16-bit tag 0xFFFF.
		 *
		 * The tag 0x0000 denotes a pointer, or another form of tagged immediate. Boolean,
		 * null and undefined values are represented by specific, invalid pointer values:
		 *
		 *     No:        0x06
		 *     Yes:       0x07
		 *     Undefined: 0x0a
		 *     Nil:       0x02
		 *
		 * These values have the following properties:
		 * - Bit 1 (TagBitTypeOther) is set for all four values, allowing real pointers to be
		 *   quickly distinguished from all immediate values, including these invalid pointers.
		 * - With bit 3 is masked out (TagBitUndefined) Undefined and Nil share the
		 *   same value, allowing null & undefined to be quickly detected.
		 *
		 * No valid CValue will have the bit pattern 0x0, this is used to represent Unit, 
		 * and as a C++ 'no value' result (e.g. CValue() has an internal value of 0).
		 */

		// These values are #defines since using static const integers here is a ~1% regression!

		// This value is 2^48, used to encode doubles such that the encoded value will begin
		// with a 16-bit pattern within the range 0x0001..0xFFFE.
#    define DoubleEncodeOffset 0x1000000000000ll
		// If all bits in the mask are set, this indicates an integer number,
		// if any but not all are set this value is a double precision number.
#    define TagTypeNumber 0xffff000000000000ll

		// All non-numeric (boolean, null, undefined) immediates have bit 2 set.
#    define TagBitTypeOther 0x2ll
#    define TagBitBoolean   0x4ll
#    define TagBitUndefined 0x8ll
		// Combined integer value for non-numeric immediates.
#    define ValueNo        (TagBitTypeOther | TagBitBoolean | false)
#    define ValueYes       (TagBitTypeOther | TagBitBoolean | true)
#    define ValueUndefined (TagBitTypeOther | TagBitUndefined)
#    define ValueNil       (TagBitTypeOther)
#    define ValueUnit      0x0ll

		// TagMask is used to check for all types of immediate values (either number or 'other').
#    define TagMask (TagTypeNumber | TagBitTypeOther)

#  endif

	private:
		inline const CValue as_value() const {
			return *this;
		}

		EncodedValueDescriptor evd;

	public:
		static const CValueLink kNilLink;
		static const CValueLink kUndefinedLink;
		static const CValueLink kYesLink;
		static const CValueLink kNoLink;
		static const CValueLink kUnitLink;

		static const CValue kNil;
		static const CValue kUndefined;
		static const CValue kYes;
		static const CValue kNo;
		static const CValue kUnit;
	};
}

#endif /* defined(__coral_vm__value__) */
