#ifndef __coral_vm__reference__
#define __coral_vm__reference__

#include "../../lib/predef.hpp"
#include "variable.hpp"

namespace CVM {
	enum CReferenceType : unsigned_integer_8 {

		/* main point of interest */
		CReferenceTypeObject =             0x00,

		/* type for native extensions and libraries */
		CReferenceTypeUnmanagedUnsafe =    0x01,

		/* extended numeric types that do not fit into CValue */
		CReferenceTypeInteger64 =          0x02,
		CReferenceTypeUnsignedInteger64 =  0x03,
		CReferenceTypeInteger128 =         0x04,
		CReferenceTypeUnsignedInteger128 = 0x05,
		CReferenceTypeFloat128 =           0x06,

		/* composite numeric types */
		CReferenceTypeComplex =            0x07,
		CReferenceTypeRational =           0x08,
		CReferenceTypeDecimalLimited =     0x09,
		CReferenceTypeDecimalUnlimited =   0x0a,

		/* array type */
		CReferenceTypeArray =              0x0b,
		/* when a CValue pointer did not fit into 48 bits, the future is upon us */
		CReferenceTypeNestedPointer =      0x0c
	};

	union CReferenceValue;

	struct CTypedVariableLink {
	public:
		const char *name;
		struct CTypedVariable variable;
		struct CTypedVariableLink *next;
	};

	struct CObject {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		void *psi_type;
		struct CTypedVariableLink *dynamic_ivars;
		const unsigned_integer_8 ivars_count;
		CVariable ivars[];
	};

	struct CUnmanagedUnsafe {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		void *pointer;
		void (*deallocator)(void *);
	};

	struct CInteger64 {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const integer_64 value;
	};

	struct CInteger64Unsigned {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const unsigned_integer_64 value;
	};

	struct CInteger128 {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const integer_128 value;
	};

	struct CInteger128Unsigned {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const unsigned_integer_128 value;
	};

	struct CFloat128 {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const float_128 value;
	};

	struct CComplex {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const CValue real;
		const CValue imaginary;
	};

	struct CRational {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		const CValue numerator;
		const CValue denominator;
	};

	struct CDecimalLimited {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
	};

	struct CDecimalUnlimited {
	public:
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
	};

	struct CArray {
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		unsigned_integer_64 elements_count;
		CValue elements[];
	};

	struct CNestedPointer {
		const CReferenceType reference_type;
		struct std::atomic<unsigned_integer_32> reference_count;
		union CReferenceValue *pointer;
	};

	union CReferenceValue {
		struct {
			const CReferenceType reference_type;
			struct std::atomic<unsigned_integer_32> reference_count;
		} common;
		struct CObject object;
		struct CUnmanagedUnsafe unmanaged_unsafe;
		struct CInteger64 integer_64;
		struct CInteger64Unsigned integer_64_unsigned;
		struct CInteger128 integer_128;
		struct CInteger128Unsigned integer_128_unsigned;
		struct CFloat128 float_128;
		struct CComplex complex;
		struct CRational rational;
		struct CDecimalLimited decimal_limited;
		struct CDecimalUnlimited decimal_unlimited;
		struct CArray array;
		struct CNestedPointer nested_pointer;
	};
}

#endif /* defined(__coral_vm__reference__) */
