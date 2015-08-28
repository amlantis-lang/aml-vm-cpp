#ifndef __aml_vm__internal__
#define __aml_vm_internal__

#ifdef __builtin_expect
# define likely(x) (__builtin_expect(!!(x), 1))
# define unlikely(x) (__builtin_expect(!!(x), 0))
#else
# define likely(x) (x)
# define unlikely(x) (x)
#endif

#endif
