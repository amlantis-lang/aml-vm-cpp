#ifndef __coral_vm__reference__
#define __coral_vm__reference__

#include "../../lib/predef.hpp"
#include "../../config/config.hpp"
#include "variable.hpp"

#include <mutex>

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
		/* when a CValue pointer did not fit into 48 bits, the future is upon us, or more likely Solaris */
		CReferenceTypeNestedPointer =      0x0c,
		/* to break strong reference cycles */
		CReferenceTypeWeakPointer =        0x0d
	};

	union CReferenceValue;

	struct CTypedVariableLink {
	public:
		const char *name;
		struct CTypedVariable variable;
		struct CTypedVariableLink *next;
	};

	struct CReference {
	public:
		struct std::atomic<union CReferenceValue *> const referenced_value;
		struct std::atomic<STRONG_REFERENCE_COUNT_TYPE> strong_reference_count;
		struct std::atomic<WEAK_REFERENCE_COUNT_TYPE> weak_reference_count;

		CReference(union CReferenceValue *);
		~CReference() noexcept;

		void
		retain(void * /* env */);

		void
		release(void * /* env */);

		CValue /* tagged pointer to CReference to CWeakReference */
		downgrade(void * /* env */);

		STRONG_REFERENCE_COUNT_TYPE
		strong_count() const noexcept;

		WEAK_REFERENCE_COUNT_TYPE
		weak_count() const noexcept;

		void
		release_slow(void * /* env */);
	};

	struct CReferenceCommon {
		const CReferenceType reference_type;

		CReferenceCommon(CReferenceType);
	};

	struct CObject {
	public:
		struct CReferenceCommon common;
		void *psi_type;
		struct CTypedVariableLink *dynamic_ivars;
		unsigned_integer_8 ivars_count;
		CVariable *ivars;

		CObject(void * /* psi_type */, unsigned_integer_8 /* ivars_count */);
		//~CObject();
	};

	struct CUnmanagedUnsafe {
	public:
		struct CReferenceCommon common;
		void *pointer;
		void (*deallocator)(void *);
	};

	struct CInteger64 {
	public:
		struct CReferenceCommon common;
		const integer_64 value;
	};

	struct CInteger64Unsigned {
	public:
		struct CReferenceCommon common;
		const unsigned_integer_64 value;
	};

	struct CInteger128 {
	public:
		struct CReferenceCommon common;
		const integer_128 value;
	};

	struct CInteger128Unsigned {
	public:
		struct CReferenceCommon common;
		const unsigned_integer_128 value;
	};

	struct CFloat128 {
	public:
		struct CReferenceCommon common;
		const float_128 value;
	};

	struct CComplex {
	public:
		struct CReferenceCommon common;
		const CValue real;
		const CValue imaginary;
	};

	struct CRational {
	public:
		struct CReferenceCommon common;
		const CValue numerator;
		const CValue denominator;
	};

	struct CDecimalLimited {
	public:
		struct CReferenceCommon common;
	};

	struct CDecimalUnlimited {
	public:
		struct CReferenceCommon common;
	};

	struct CArray {
	public:
		struct CReferenceCommon common;
		unsigned_integer_64 elements_count;
		CValue elements[];
	};

	struct CNestedPointer {
	public:
		struct CReferenceCommon common;
		union CReferenceValue *pointer;
	};

	struct CWeakPointer {
	public:
		struct CReferenceCommon common;
		std::atomic<CReference *> const pointer;

		CWeakPointer(CReference *);

		CValue
		upgrade(void * /* env */);

	private:
		void
		on_release() noexcept;
	};

	union CReferenceValue {
		struct CReferenceCommon common;
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
		struct CWeakPointer weak_pointer;
	};

	typedef struct std::mutex CReferenceLock;

	CReferenceLock &
	lock_for_reference(union CReferenceValue *) noexcept;
}

#endif /* defined(__coral_vm__reference__) */
