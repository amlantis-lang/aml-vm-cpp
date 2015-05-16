/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __gear_vm__nan__
#define __gear_vm__nan__

#include "predef.hpp"

namespace GVM {

	/* NaN (not-a-number) float_64 values are central to how GVM encodes GVM
	 * values (GValues). All values, including integers and non-numeric values, are always
	 * encoded using the IEEE 754 binary float_64 format. Non-float_64 values are encoded using
	 * a NaN with the sign bit set. The 51-bit payload is then used for encoding the actual
	 * value - be it an integer or a pointer to an object, or something else. But we only
	 * make use of the low 49 bits and the top 15 bits being all set to 1 is the indicator
	 * that a value is not a float_64. Top 15 bits being set to 1 also indicate a signed
	 * signaling NaN with some additional NaN payload bits.
	 *
	 * Our use of NaN encoding means that we have to be careful with how we use NaNs for
	 * ordinary float_64s. For example, it would be wrong to ever use a NaN that has the top
	 * 15 bits set, as that would look like a non-float_64 value to GVM.
	 *
	 * We can trust that on all of the hardware/OS combinations that we care about,
	 * NaN-producing math operations never produce a NaN that looks like a tagged value. But
	 * if we're ever in a situation where we worry about it, we can use purify_NaN() to get a
	 * NaN that doesn't look like a tagged non- float_64 value. The Gear language doesn't
	 * distinguish between different flavors of NaN and there is no way to detect what kind
	 * of NaN you have - hence so long as all float_64 NaNs are purified, then our tagging
	 * scheme remains sound.
	 *
	 * It's worth noting that there are cases, like sin(), that will almost produce a NaN
	 * that breaks us. sin(-inf) returns 0xfff8000000000000. This doesn't break us because
	 * not all of the top 15 bits are set. But it's very close. Hence our assumptions about
	 * NaN are just about the most aggressive assumptions we could possibly make without
	 * having to call purify_NaN() in surprising places.
	 *
	 * For naming purposes, we say that a NaN is "pure" if it is safe to tag, in the sense
	 * that doing so would result in a tagged value that would pass the "are you a float_64"
	 * test. We say that a NaN is "impure" if attempting to tag it would result in a value
	 * that would look like something other than a float_64.
	 */

	/* Returns some kind of pure NaN. */
	inline
	float_64
	pure_NaN() {
		/* Be sure that we return exactly the kind of NaN that is safe. We engineer the bits
		 * ourselves to ensure that it's !isImpureNaN(). FWIW, this is what
		 * numeric_limits<float_64>::quiet_NaN() returns on Mac/X86_64. But AFAICT there is
		 * no guarantee that quiet_NaN would return a pureNaN on all platforms. For example,
		 * the docs appear to imply that quiet_NaN could even return a float_64 with the
		 * signaling bit set on hardware that doesn't do signaling. That would probably
		 * never happen, but it's healthy to be paranoid.
		 */
		return bitwise_cast<float_64>(0x7ff8000000000000ll);
	}

# define PNaN (pure_NaN())

	/* Tests if the float_64 value would break CVALUE64 encoding, which is the most
	 * aggressive kind of encoding that we currently use.
	 */
	inline
	bool
	is_impure_NaN(float_64 value) {
		return bitwise_cast<uint64_t>(value) >= 0xfffe000000000000llu;
	}

	/* If the given value is NaN, then return a NaN that is known to be pure. */
	inline
	float_64
	purify_NaN(float_64 value) {
		if (value != value) {
			return PNaN;
		} else {
			return value;
		}
	}

}

#endif
