/*
 * Copyright (C) 2006-2009, 2013-2015 Apple Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 * Copyright (C) 2010, 2011 Research In Motion Limited. All rights reserved.
 * Copyright (C) 2013 Patrick Gansterer <paroga@paroga.com>
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

#ifndef __gear_vm__platform__
#define __gear_vm__platform__

/* common platform adaptation macros */

/* CPU() - the target CPU architecture */
#define CPU(GVM_FEATURE) (defined GVM_CPU_##GVM_FEATURE && GVM_CPU_##GVM_FEATURE)

/* OS() - underlying operating system; only to be used for mandated low-level services like virtual memory */
#define OS(GVM_FEATURE) (defined GVM_OS_##GVM_FEATURE && GVM_OS_##GVM_FEATURE)

/* USE() - use a particular OS service */
#define USE(GVM_FEATURE) (defined GVM_USE_##GVM_FEATURE && GVM_USE_##GVM_FEATURE)

/* *** */

/* CPU() - the target CPU architecture */
/* This also defines CPU(BIG_ENDIAN) or CPU(MIDDLE_ENDIAN) or neither, as appropriate. */

/* CPU(ALPHA) - DEC Alpha */
#if defined(__alpha__)
#  define GVM_CPU_ALPHA 1
#endif

/* CPU(HPPA) - HP PA-RISC */
#if defined(__hppa__) || defined(__hppa64__)
#  define GVM_CPU_HPPA 1
#  define GVM_CPU_BIG_ENDIAN 1
#endif

/* CPU(IA64) - Itanium / IA-64 */
#if defined(__ia64__)
#  define GVM_CPU_IA64 1
/* 32-bit mode on Itanium */
#  if !defined(__LP64__)
#    define GVM_CPU_IA64_32 1
#  endif
#endif

/* CPU(MIPS) - MIPS 32-bit and 64-bit */
#if (defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_) || defined(__mips64))
#  if defined(_MIPS_SIM_ABI64) && (_MIPS_SIM == _MIPS_SIM_ABI64)
#    define GVM_CPU_MIPS64 1
#    define GVM_MIPS_ARCH __mips64
#  else
#    define GVM_CPU_MIPS 1
#    define GVM_MIPS_ARCH __mips
#  endif
#  if defined(__MIPSEB__)
#    define GVM_CPU_BIG_ENDIAN 1
#  endif
#  define GVM_MIPS_PIC (defined __PIC__)
#  define GVM_MIPS_ISA(v) (defined GVM_MIPS_ARCH && GVM_MIPS_ARCH == v)
#  define GVM_MIPS_ISA_AT_LEAST(v) (defined GVM_MIPS_ARCH && GVM_MIPS_ARCH >= v)
#  define GVM_MIPS_ARCH_REV __mips_isa_rev
#  define GVM_MIPS_ISA_REV(v) (defined GVM_MIPS_ARCH_REV && GVM_MIPS_ARCH_REV == v)
#  define GVM_MIPS_DOUBLE_FLOAT (defined __mips_hard_float && !defined __mips_single_float)
#  define GVM_MIPS_FP64 (defined __mips_fpr && __mips_fpr == 64)
/* MIPS requires allocators to use aligned memory */
#  define GVM_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1
#endif 

/* CPU(PPC) - PowerPC 32-bit */
#if (  defined(__ppc__)        \
    || defined(__PPC__)        \
    || defined(__powerpc__)    \
    || defined(__powerpc)      \
    || defined(__POWERPC__)    \
    || defined(_M_PPC)         \
    || defined(__PPC))         \
    && defined(__BYTE_ORDER__) \
    && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#  define GVM_CPU_PPC 1
#  define GVM_CPU_BIG_ENDIAN 1
#endif

/* CPU(PPC64) - PowerPC 64-bit Big Endian */
#if (  defined(__ppc64__)      \
    || defined(__PPC64__))     \
    && defined(__BYTE_ORDER__) \
    && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#  define GVM_CPU_PPC64 1
#  define GVM_CPU_BIG_ENDIAN 1
#endif

/* CPU(PPC64) - PowerPC 64-bit Little Endian */
#if (  defined(__ppc64__)     \
    || defined(__PPC64__)      \
    || defined(__ppc64le__)    \
    || defined(__PPC64LE__))   \
    && defined(__BYTE_ORDER__) \
    && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define CPU_CPU_PPC64LE 1
#endif

/* CPU(SH4) - SuperH SH-4 */
#if defined(__SH4__)
#  define GVM_CPU_SH4 1
#endif

/* CPU(S390X) - S390 64-bit */
#if defined(__s390x__)
#  define GVM_CPU_S390X 1
#  define GVM_CPU_BIG_ENDIAN 1
#endif

/* CPU(S390) - S390 32-bit */
#if defined(__s390__)
#  define GVM_CPU_S390 1
#  define GVM_CPU_BIG_ENDIAN 1
#endif

/* CPU(X86) - i386 / x86 32-bit */
#if   defined(__i386__) \
   || defined(i386)     \
   || defined(_M_IX86)  \
   || defined(_X86_)    \
   || defined(__THW_INTEL)
#  define GVM_CPU_X86 1
#endif

/* CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if   defined(__x86_64__) \
   || defined(_M_X64)
#  define GVM_CPU_X86_64 1
#endif

/* CPU(ARM64) - Apple */
#if (defined(__arm64__) && defined(__APPLE__)) || defined(__aarch64__)
#  define GVM_CPU_ARM64 1
#endif

/* CPU(ARM) - ARM, any version */
#define GVM_ARM_ARCH_AT_LEAST(N) (CPU(ARM) && GVM_ARM_ARCH_VERSION >= N)
#if   defined(arm)     \
   || defined(__arm__) \
   || defined(ARM)     \
   || defined(_ARM_)
#  define GVM_CPU_ARM 1
#  if defined(__ARM_PCS_VFP)
#    define GVM_CPU_ARM_HARDFP 1
#  endif
#  if defined(__ARMEB__)
#    define GVM_CPU_BIG_ENDIAN 1
#  elif !defined(__ARM_EABI__) \
     && !defined(__EABI__)     \
     && !defined(__VFP_FP__)   \
     && !defined(_WIN32_WCE)
#    define GVM_CPU_MIDDLE_ENDIAN 1
#  endif

/* Set GVM_ARM_ARCH_VERSION */
#if   defined(__ARM_ARCH_4__)  \
   || defined(__ARM_ARCH_4T__) \
   || defined(__MARM_ARMV4__)
#  define GVM_ARM_ARCH_VERSION 4
#elif defined(__ARM_ARCH_5__)  \
   || defined(__ARM_ARCH_5T__) \
   || defined(__MARM_ARMV5__)
#  define GVM_ARM_ARCH_VERSION 5
#elif defined(__ARM_ARCH_5E__)  \
   || defined(__ARM_ARCH_5TE__) \
   || defined(__ARM_ARCH_5TEJ__)
#  define GVM_ARM_ARCH_VERSION 5
/* ARMv5TE requires allocators to use aligned memory */
#  define GVM_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1
#elif defined(__ARM_ARCH_6__)   \
   || defined(__ARM_ARCH_6J__)  \
   || defined(__ARM_ARCH_6K__)  \
   || defined(__ARM_ARCH_6Z__)  \
   || defined(__ARM_ARCH_6ZK__) \
   || defined(__ARM_ARCH_6T2__) \
   || defined(__ARMV6__)
#  define GVM_ARM_ARCH_VERSION 6
#elif defined(__ARM_ARCH_7A__) \
   || defined(__ARM_ARCH_7K__) \
   || defined(__ARM_ARCH_7R__) \
   || defined(__ARM_ARCH_7S__)
#  define GVM_ARM_ARCH_VERSION 7
#elif defined(__ARM_ARCH_8__)
#  define GVM_ARM_ARCH_VERSION 8
/* MSVC sets _M_ARM */
#elif defined(_M_ARM)
#  define GVM_ARM_ARCH_VERSION _M_ARM
/* RVCT sets _TARGET_ARCH_ARM */
#elif defined(__TARGET_ARCH_ARM)
#  define GVM_ARM_ARCH_VERSION __TARGET_ARCH_ARM
#  if   defined(__TARGET_ARCH_5E)  \
     || defined(__TARGET_ARCH_5TE) \
     || defined(__TARGET_ARCH_5TEJ)
/* ARMv5TE requires allocators to use aligned memory */
#    define GVM_USE_ARENA_ALLOC_ALIGNMENT_INTEGER 1
#  endif
#else
#  define GVM_ARM_ARCH_VERSION 0
#endif

/* Set GVM_THUMB_ARCH_VERSION */
#if defined(__ARM_ARCH_4T__)
#  define GVM_THUMB_ARCH_VERSION 1

#elif defined(__ARM_ARCH_5T__)  \
   || defined(__ARM_ARCH_5TE__) \
   || defined(__ARM_ARCH_5TEJ__)
#  define GVM_THUMB_ARCH_VERSION 2

#elif defined(__ARM_ARCH_6J__) \
   || defined(__ARM_ARCH_6K__) \
   || defined(__ARM_ARCH_6Z__) \
   || defined(__ARM_ARCH_6ZK__) \
   || defined(__ARM_ARCH_6M__)
#  define GVM_THUMB_ARCH_VERSION 3

#elif defined(__ARM_ARCH_6T2__) \
   || defined(__ARM_ARCH_7__)   \
   || defined(__ARM_ARCH_7A__)  \
   || defined(__ARM_ARCH_7K__)  \
   || defined(__ARM_ARCH_7M__)  \
   || defined(__ARM_ARCH_7R__)  \
   || defined(__ARM_ARCH_7S__)
#  define GVM_THUMB_ARCH_VERSION 4
/* RVCT sets __TARGET_ARCH_THUMB */
#elif defined(__TARGET_ARCH_THUMB)
#  define GVM_THUMB_ARCH_VERSION __TARGET_ARCH_THUMB
#else
#  define GVM_THUMB_ARCH_VERSION 0
#endif

/* CPU(ARMV5_OR_LOWER) - ARM instruction set v5 or earlier */
/* On ARMv5 and below the natural alignment is required. And there are some other differences for v5 or earlier. */
#if !defined(ARMV5_OR_LOWER) && !GVM_ARM_ARCH_AT_LEAST(6)
#  define GVM_CPU_ARMV5_OR_LOWER 1
#endif

/* CPU(ARM_TRADITIONAL) - Thumb2 is not available, only traditional ARM (v4 or greater) */
/* CPU(ARM_THUMB2) - Thumb2 instruction set is available */
/* Only one of these will be defined. */
#if !defined(GVM_CPU_ARM_TRADITIONAL) && !defined(GVM_CPU_ARM_THUMB2)
#  if      defined(thumb2) || defined(__thumb2__) \
      || ((defined(__thumb) || defined(__thumb__)) && GVM_THUMB_ARCH_VERSION == 4)
#    define GVM_CPU_ARM_TRADITIONAL 0
#    define GVM_CPU_ARM_THUMB2 1
#  elif GVM_ARM_ARCH_AT_LEAST(4)
#    define GVM_CPU_ARM_TRADITIONAL 1
#    define GVM_CPU_ARM_THUMB2 0
#  else
#    error "Not supported ARM architecture"
#  endif
#elif CPU(ARM_TRADITIONAL) && CPU(ARM_THUMB2) /* Sanity Check */
#  error "Cannot use both of GVM_CPU_ARM_TRADITIONAL and GVM_CPU_ARM_THUMB2 platforms"
#endif

#if defined(__ARM_NEON__) && !defined(GVM_CPU_ARM_NEON)
#  define GVM_CPU_ARM_NEON 1
#endif

#if CPU(ARM_NEON)
/* All NEON intrinsics usage can be disabled by this macro. */
#  define GVM_HAVE_ARM_NEON_INTRINSICS 1
#endif

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
#  define GVM_CPU_ARM_VFP 1
#endif

#if defined(__ARM_ARCH_7K__)
#  define GVM_CPU_APPLE_ARMV7K 1
#endif

#if defined(__ARM_ARCH_7S__)
#  define GVM_CPU_APPLE_ARMV7S 1
#endif

#if defined(__ARM_ARCH_EXT_IDIV__) || CPU(APPLE_ARMV7S)
#  define GVM_HAVE_ARM_IDIV_INSTRUCTIONS 1
#endif

#endif /* ARM */

/* *** */

/* OS() - underlying operating system; only to be used for mandated low-level services like virtual memory */

/* OS(AIX) - AIX */
#ifdef _AIX
#  define GVM_OS_AIX 1
#endif

/* OS(DARWIN) - Any Darwin-based OS, including Mac OS X and iPhone OS */
#ifdef __APPLE__
#  define GVM_OS_DARWIN 1

#  include <Availability.h>
#  include <AvailabilityMacros.h>
#  include <TargetConditionals.h>
#endif

/* OS(IOS) - iOS */
/* OS(MAC_OS_X) - Mac OS X (not including iOS) */
#if OS(DARWIN) && ((defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED) \
    || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)                 \
    || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR))
#  define GVM_OS_IOS 1
#elif OS(DARWIN) && defined(TARGET_OS_MAC) && TARGET_OS_MAC
#  define GVM_OS_MAC_OS_X 1
#endif

/* OS(FREEBSD) - FreeBSD */
#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  define GVM_OS_FREEBSD 1
#endif

/* OS(HURD) - GNU/Hurd */
#ifdef __GNU__
#  define GVM_OS_HURD 1
#endif

/* OS(LINUX) - GNU/Linux */
#ifdef __linux__
#  define GVM_OS_LINUX 1
#endif

/* OS(NETBSD) - NetBSD */
#if defined(__NetBSD__)
#  define GVM_OS_NETBSD 1
#endif

/* OS(OPENBSD) - OpenBSD */
#ifdef __OpenBSD__
#  define GVM_OS_OPENBSD 1
#endif

/* OS(SOLARIS) - Solaris */
#if defined(sun) || defined(__sun)
#  define GVM_OS_SOLARIS 1
#endif

/* OS(WINDOWS) - Any version of Windows */
#if defined(WIN32) || defined(_WIN32)
#  define GVM_OS_WINDOWS 1
#endif

/* OS(UNIX) - Any Unix-like system */
#if    OS(AIX)              \
    || OS(DARWIN)           \
    || OS(FREEBSD)          \
    || OS(HURD)             \
    || OS(LINUX)            \
    || OS(NETBSD)           \
    || OS(OPENBSD)          \
    || OS(SOLARIS)          \
    || defined(unix)        \
    || defined(__unix)      \
    || defined(__unix__)
#  define GVM_OS_UNIX 1
#endif

/* *** */

/* CVALUE64, CVALUE32_64 */

#if !defined(GVM_USE_CVALUE64) && !defined(GVM_USE_CVALUE32_64)
#  if (CPU(X86_64) && (OS(UNIX) || OS(WINDOWS))) \
      || (CPU(IA64) && !CPU(IA64_32)) \
      || CPU(ALPHA)    \
      || CPU(ARM64)    \
      || CPU(S390X)    \
      || CPU(MIPS64)   \
      || CPU(PPC64)    \
      || CPU(PPC64LE)
#    define GVM_USE_CVALUE64 1
#  else
#    define GVM_USE_CVALUE32_64 1
#  endif
#endif

/* bitwise_cast */

namespace GVM {

	/*
	 * C++'s idea of a reinterpret_cast lacks sufficient cojones.
	 */
	template<typename ToType, typename FromType>
	inline
	ToType
	bitwise_cast(FromType from) {
		static_assert(sizeof(FromType) == sizeof(ToType), "bitwise_cast size of FromType and ToType must be equal!");
		union {
			FromType from;
			ToType to;
		} u;
		u.from = from;
		return u.to;
	}
}

using GVM::bitwise_cast;

#endif

