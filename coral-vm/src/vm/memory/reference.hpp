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

	/* Here's an optimization idea: pre-allocate an array of CReferences to be used whenever a new one
	 * would be needed, automatically grow it etc. (like std::vector<CReference> or something), to prevent
	 * some random memory accesses (also that could open the possibility of garbage collection...)
	 */
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

	private:
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

		static
		CValue
		allocate(void * /* psi_type */, void * /* env */) noexcept;

		CObject(void * /* psi_type */, unsigned_integer_8 /* ivars_count */);
		~CObject();
	};

	struct CUnmanagedUnsafe {
	public:
		struct CReferenceCommon common;
		void *pointer;
		void (*deallocator)(void *);

		CUnmanagedUnsafe(void * /* pointer */, void (*)(void *) /* deallocator */);
		~CUnmanagedUnsafe();

		static
		CValue
		allocate(void * /* pointer */, void (*)(void *) /* deallocator */, void * /* env */) noexcept;
	};

	struct CInteger64 {
	public:
		struct CReferenceCommon common;
		const integer_64 value;

		CInteger64(const integer_64) noexcept;

		static
		CValue
		allocate(const integer_64, void * /* env */) noexcept;
	};

	struct CInteger64Unsigned {
	public:
		struct CReferenceCommon common;
		const unsigned_integer_64 value;

		CInteger64Unsigned(const unsigned_integer_64) noexcept;

		static
		CValue
		allocate(const unsigned_integer_64, void * /* env */) noexcept;
	};

	struct CInteger128 {
	public:
		struct CReferenceCommon common;
		const integer_128 value;

		CInteger128(const integer_128) noexcept;

		static
		CValue
		allocate(const integer_128, void * /* env */) noexcept;
	};

	struct CInteger128Unsigned {
	public:
		struct CReferenceCommon common;
		const unsigned_integer_128 value;

		CInteger128Unsigned(const unsigned_integer_128) noexcept;

		static
		CValue
		allocate(const unsigned_integer_128, void * /* env */) noexcept;
	};

	struct CFloat128 {
	public:
		struct CReferenceCommon common;
		const float_128 value;

		CFloat128(const float_128) noexcept;

		static
		CValue
		allocate(const float_128, void * /* env */) noexcept;
	};

	struct CComplex {
	public:
		struct CReferenceCommon common;
		const CValue real;
		const CValue imaginary;

		CComplex(const CValue, const CValue) noexcept;

		static
		CValue
		allocate(const CValue /* real */, const CValue /* imaginary, obviously */, void * /* env */) noexcept;
	};

	struct CRational {
	public:
		struct CReferenceCommon common;
		const CValue numerator;
		const CValue denominator;

		CRational(const CValue, const CValue) noexcept;

		static
		CValue
		allocate(const CValue /* numerator */, const CValue /* denominator, obviously */, void * /* env */) noexcept;
	};

	struct CDecimalLimited {
	public:
		struct CReferenceCommon common;
		const unsigned_integer_128 value;
		/* Here's the idea: unsigned 128bit integer has at most 39 total digits,
		 * thus that should fit easily into max 2^7 - 1 decimal digits of them, thus we can safely use
		 * the first bit as the sign */
		/** the number of digits *after* the decimal point; first bit is the sign */
		const unsigned_integer_8 decimal_digits;

		CDecimalLimited(const unsigned_integer_128, const unsigned_integer_8) noexcept;

		static
		CValue
		allocate(const unsigned_integer_128, const unsigned_integer_8, void * /* env */) noexcept;
	};

	/* Stop right there for a moment: CDecimalLimited has the ability to save a number as big as
	 * 340 undecillion (340_282_366_920_938_463_463_374_607_431_768_211_455), which is more than 
	 * memory addressable in the full 64bit range (16 EiB), which is A LOT, are you 100% sure
	 * you need more? */
	struct CDecimalUnlimited {
	public:
		struct CReferenceCommon common;
		/* Discussion: each unsigned_integer_8 can hold two base-10 digits without bits interleaving */
		unsigned_integer_8 *const individual_digits;
		const size_t digits_count;
		/** the number of digits *after* the decimal point; first bit is the sign */
		const unsigned_integer_64 decimal_digits; /* we say "unlimited", but 63bit decimal digits is the limit */

		CDecimalUnlimited(unsigned_integer_8 *const, const size_t, const unsigned_integer_64) noexcept;
		~CDecimalUnlimited() noexcept;

		static
		CValue
		allocate(unsigned_integer_8 *const, const size_t, const unsigned_integer_64, void * /* env */) noexcept;
	};

	struct CArray {
	public:
		struct CReferenceCommon common;
		unsigned_integer_64 elements_count;
		CValue *elements;

		static
		CValue
		allocate(const unsigned_integer_64 /* elements_count */, void * /* env */) noexcept;
	};

	/* just do not use this yet */
	struct CNestedPointer {
	public:
		struct CReferenceCommon common;
		union CReferenceValue *pointer;

		static
		CValue
		allocate(union CReferenceValue * /* pointer */, void * /* env */) = delete;
	};

	/* only allocate from an existing strong reference, using downgrade() on CReference */
	struct CWeakPointer {
	public:
		struct CReferenceCommon common;
		std::atomic<CReference *> const pointer;

		CWeakPointer(CReference *);

		CValue
		upgrade(void * /* env */);

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
