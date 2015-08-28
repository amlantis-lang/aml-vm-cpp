#ifndef __aml_vm__reference__
#define __aml_vm__reference__

#include "../../lib/predef.hpp"
#include "../../config/config.hpp"
#include "variable.hpp"
#include "../psi/psi.hpp"

#include <mutex>

namespace AVM {
	enum AReferenceType : unsigned_integer_8 {

		/* main point of interest */
		AReferenceTypeObject =             0x00,

		/* type for native extensions and libraries */
		AReferenceTypeUnmanagedUnsafe =    0x01,

		/* extended numeric types that do not fit into AValue */
		AReferenceTypeInteger64 =          0x02,
		AReferenceTypeUnsignedInteger64 =  0x03,
		AReferenceTypeInteger128 =         0x04,
		AReferenceTypeUnsignedInteger128 = 0x05,
		AReferenceTypeFloat128 =           0x06,

		/* composite numeric types */
		AReferenceTypeComplex =            0x07,
		AReferenceTypeRational =           0x08,
		AReferenceTypeDecimalLimited =     0x09,
		AReferenceTypeDecimalUnlimited =   0x0a,

		/* array type */
		AReferenceTypeArray =              0x0b,

		/* when a AValue pointer did not fit into 48 bits, the future is upon us, or more likely Solaris */
		AReferenceTypeNestedPointer =      0x0c,

		/* to break strong reference cycles */
		AReferenceTypeWeakPointer =        0x0d,

		/* an object associated with a PSI element */
		AReferenceTypePsiBearer =          0x0e
	};

	union AReferenceValue;

	struct ATypedVariableLink {
	public:
		const char *name;
		struct ATypedVariable variable;
		struct ATypedVariableLink *next;
	};

	/* Here's an optimization idea: pre-allocate an array of AReferences to be used whenever a new one
	 * would be needed, automatically grow it etc. (like std::vector<AReference> or something), to prevent
	 * some random memory accesses (also that could open the possibility of garbage collection...)
	 */
	struct AReference {
	public:
		struct std::atomic<union AReferenceValue *> const referenced_value;
		struct std::atomic<STRONG_REFERENCE_COUNT_TYPE> strong_reference_count;
		struct std::atomic<WEAK_REFERENCE_COUNT_TYPE> weak_reference_count;

		AReference(union AReferenceValue *);
		~AReference() noexcept;

		void
		retain(void * /* env */);

		void
		release(void * /* env */);

		AValue /* tagged pointer to AReference to CWeakReference */
		downgrade(void * /* env */);

		STRONG_REFERENCE_COUNT_TYPE
		strong_count() const noexcept;

		WEAK_REFERENCE_COUNT_TYPE
		weak_count() const noexcept;

	private:
		void
		release_slow(void * /* env */);
	};

	struct AReferenceCommon {
		const AReferenceType reference_type;

		AReferenceCommon(AReferenceType);
	};

	struct AObject {
	public:
		struct AReferenceCommon common;
		/* non-const psi_type, because hot code loading may upgrade it to new version */
		PsiElement *psi_type;
		struct ATypedVariableLink *dynamic_ivars;
		unsigned_integer_8 ivars_count;
		AVariable *ivars;

		static
		AValue
		allocate(PsiElement * /* psi_type */, void * /* env */) noexcept;

		AObject(PsiElement * /* psi_type */, unsigned_integer_8 /* ivars_count */);
		~AObject();
	};

	struct AUnmanagedUnsafe {
	public:
		struct AReferenceCommon common;
		void *pointer;
		void (*deallocator)(void *);

		AUnmanagedUnsafe(void * /* pointer */, void (*)(void *) /* deallocator */);
		~AUnmanagedUnsafe();

		static
		AValue
		allocate(void * /* pointer */, void (*)(void *) /* deallocator */, void * /* env */) noexcept;
	};

	struct AInteger64 {
	public:
		struct AReferenceCommon common;
		const integer_64 value;

		AInteger64(const integer_64) noexcept;

		static
		AValue
		allocate(const integer_64, void * /* env */) noexcept;
	};

	struct AInteger64Unsigned {
	public:
		struct AReferenceCommon common;
		const unsigned_integer_64 value;

		AInteger64Unsigned(const unsigned_integer_64) noexcept;

		static
		AValue
		allocate(const unsigned_integer_64, void * /* env */) noexcept;
	};

	struct AInteger128 {
	public:
		struct AReferenceCommon common;
		const integer_128 value;

		AInteger128(const integer_128) noexcept;

		static
		AValue
		allocate(const integer_128, void * /* env */) noexcept;
	};

	struct AInteger128Unsigned {
	public:
		struct AReferenceCommon common;
		const unsigned_integer_128 value;

		AInteger128Unsigned(const unsigned_integer_128) noexcept;

		static
		AValue
		allocate(const unsigned_integer_128, void * /* env */) noexcept;
	};

	struct AFloat128 {
	public:
		struct AReferenceCommon common;
		const float_128 value;

		AFloat128(const float_128) noexcept;

		static
		AValue
		allocate(const float_128, void * /* env */) noexcept;
	};

	struct AComplex {
	public:
		struct AReferenceCommon common;
		const AValue real;
		const AValue imaginary;

		AComplex(const AValue, const AValue) noexcept;

		static
		AValue
		allocate(const AValue /* real */, const AValue /* imaginary, obviously */, void * /* env */) noexcept;
	};

	struct ARational {
	public:
		struct AReferenceCommon common;
		const AValue numerator;
		const AValue denominator;

		ARational(const AValue, const AValue) noexcept;

		static
		AValue
		allocate(const AValue /* numerator */, const AValue /* denominator, obviously */, void * /* env */) noexcept;
	};

	struct ADecimalLimited {
	public:
		struct AReferenceCommon common;
		const unsigned_integer_128 value;
		/* Here's the idea: unsigned 128bit integer has at most 39 total digits,
		 * thus that should fit easily into max 2^7 - 1 decimal digits of them, thus we can safely use
		 * the first bit as the sign */
		/** the number of digits *after* the decimal point; first bit is the sign */
		const unsigned_integer_8 decimal_digits;

		ADecimalLimited(const unsigned_integer_128, const unsigned_integer_8) noexcept;

		static
		AValue
		allocate(const unsigned_integer_128, const unsigned_integer_8, void * /* env */) noexcept;
	};

	/* Stop right there for a moment: ADecimalLimited has the ability to save a number as big as
	 * 340 undecillion (340_282_366_920_938_463_463_374_607_431_768_211_455), which is more than 
	 * memory addressable in the full 64bit range (16 EiB), which is A LOT, are you 100% sure
	 * you need more? */
	struct ADecimalUnlimited {
	public:
		struct AReferenceCommon common;
		/* Discussion: each unsigned_integer_8 can hold two base-10 digits without bits interleaving */
		unsigned_integer_8 *const individual_digits;
		const size_t digits_count;
		/** the number of digits *after* the decimal point; first bit is the sign */
		const unsigned_integer_64 decimal_digits; /* we say "unlimited", but 63bit decimal digits is the limit */

		ADecimalUnlimited(unsigned_integer_8 *const, const size_t, const unsigned_integer_64) noexcept;
		~ADecimalUnlimited() noexcept;

		static
		AValue
		allocate(unsigned_integer_8 *const, const size_t, const unsigned_integer_64, void * /* env */) noexcept;
	};

	struct AArray {
	public:
		struct AReferenceCommon common;
		unsigned_integer_64 elements_count;
		AValue *elements;

		static
		AValue
		allocate(const unsigned_integer_64 /* elements_count */, void * /* env */) noexcept;
	};

	/* just do not use this yet */
	struct ANestedPointer {
	public:
		struct AReferenceCommon common;
		union AReferenceValue *pointer;

		static
		AValue
		allocate(union AReferenceValue * /* pointer */, void * /* env */) = delete;
	};

	/* only allocate from an existing strong reference, using downgrade() on AReference */
	struct AWeakPointer {
	public:
		struct AReferenceCommon common;
		std::atomic<AReference *> const pointer;

		AWeakPointer(AReference *);

		AValue
		upgrade(void * /* env */);

		void
		on_release() noexcept;
	};

	struct APsiBearer {
	public:
		struct AReferenceCommon common;
		/* non-const psi_type, because hot code loading may upgrade it to new version */
		PsiElement *psi_element;
		struct ATypedVariableLink *dynamic_ivars;
		unsigned_integer_8 ivars_count;
		AVariable *ivars;

		static
		AValue
		allocate(PsiElement * /* psi_element */, void * /* env */) noexcept;

		APsiBearer(PsiElement * /* psi_element */, unsigned_integer_8 /* ivars_count */);
		~APsiBearer();
	};

	union AReferenceValue {
		struct AReferenceCommon common;
		struct AObject object;
		struct AUnmanagedUnsafe unmanaged_unsafe;
		struct AInteger64 integer_64;
		struct AInteger64Unsigned integer_64_unsigned;
		struct AInteger128 integer_128;
		struct AInteger128Unsigned integer_128_unsigned;
		struct AFloat128 float_128;
		struct AComplex complex;
		struct ARational rational;
		struct ADecimalLimited decimal_limited;
		struct ADecimalUnlimited decimal_unlimited;
		struct AArray array;
		struct ANestedPointer nested_pointer;
		struct AWeakPointer weak_pointer;
		struct APsiBearer psi_bearer;
	};

	typedef struct std::mutex AReferenceLock;

	AReferenceLock &
	lock_for_reference(union AReferenceValue *) noexcept;
}

#endif /* defined(__aml_vm__reference__) */
