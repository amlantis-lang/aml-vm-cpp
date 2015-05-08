#ifndef __coral_vm__config__
#define __coral_vm__config__

#include "../lib/predef.hpp"
#include <cstdint>

/* Edit these constants to your platform's needs. */

#if USE(CVALUE64)
#  define STRONG_REFERENCE_COUNT_TYPE unsigned_integer_64
#  define WEAK_REFERENCE_COUNT_TYPE STRONG_REFERENCE_COUNT_TYPE
#elif USE(CVALUE32_64)
#  define STRONG_REFERENCE_COUNT_TYPE unsigned_integer_32
#  define WEAK_REFERENCE_COUNT_TYPE STRONG_REFERENCE_COUNT_TYPE
#endif

#if USE(CVALUE64)
#  define REFERENCE_LOCKS_COUNT 16
#  define REFERENCE_LOCK_INDEX(R) (((uintptr_t) (R) >> 8) & 0xe)
#elif USE(CVALUE32_64)
#  define REFERENCE_LOCKS_COUNT 8
#  define REFERENCE_LOCK_INDEX(R) (((uintptr_t) (R) >> 8) & 0x7)
#endif

#endif
