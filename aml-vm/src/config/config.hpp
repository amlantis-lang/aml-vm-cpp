#ifndef __aml_vm__config__
#define __aml_vm__config__

#include "../lib/predef.hpp"
#include <cstdint> /* for uintptr_t */

/* Edit these constants to your platform's needs. */

#if USE(AVALUE64)
   /* (2^32 - 1) strong references should be enough for everybody */
#  define STRONG_REFERENCE_COUNT_TYPE unsigned_integer_32
#  define WEAK_REFERENCE_COUNT_TYPE STRONG_REFERENCE_COUNT_TYPE
#elif USE(AVALUE32_64)
#  define STRONG_REFERENCE_COUNT_TYPE unsigned_integer_32
#  define WEAK_REFERENCE_COUNT_TYPE STRONG_REFERENCE_COUNT_TYPE
#endif

#if USE(AVALUE64)
#  define REFERENCE_LOCKS_COUNT 16
#  define REFERENCE_LOCK_INDEX(R) (((uintptr_t) (R) >> 8) & 0xe)
#elif USE(AVALUE32_64)
#  define REFERENCE_LOCKS_COUNT 8
#  define REFERENCE_LOCK_INDEX(R) (((uintptr_t) (R) >> 8) & 0x7)
#endif

#endif
