#ifndef __aml_vm__predef__
#define __aml_vm__predef__

#include "platform.hpp"

// integral types
typedef signed char integer_8;
typedef unsigned char unsigned_integer_8;
typedef signed short integer_16;
typedef unsigned short unsigned_integer_16;
typedef signed int integer_32;
typedef unsigned int unsigned_integer_32;
typedef signed long integer_64;
typedef unsigned long unsigned_integer_64;
#ifdef __int128
#  define AVM_USE_INT128 1
typedef signed __int128 integer_128;
typedef unsigned __int128 unsigned_integer_128;
#else
typedef struct {integer_64 first;          unsigned_integer_64 second;} integer_128;
typedef struct {unsigned_integer_64 first; unsigned_integer_64 second;} unsigned_integer_128;
#endif

// floating point types
typedef unsigned_integer_16 float_16;
typedef float float_32;
typedef double float_64;
#ifndef __float128
typedef unsigned_integer_128 float_128;
#else
#  define AVM_USE_FLOAT128 1
typedef __float128 float_128;
#endif

#endif
