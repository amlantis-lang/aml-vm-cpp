#ifndef __gear_vm__reference__
#define __gear_vm__reference__

#include "../../lib/predef.hpp"
#include "../../config/config.hpp"
#include "variable.hpp"

#include <mutex>

namespace GVM {
	enum GReferenceType : unsigned_integer_8 {

		/* main point of interest */
		GReferenceTypeObject =             0x00,

		/* type for native extensions and libraries */
		GReferenceTypeUnmanagedUnsafe =    0x01,

		/* extended numeric types that do not fit into GValue */
		GReferenceTypeInteger64 =          0x02,
		GReferenceTypeUnsignedInteger64 =  0x03,
		GReferenceTypeInteger128 =         0x04,
		GReferenceTypeUnsignedInteger128 = 0x05,
		GReferenceTypeFloat128 =           0x06,

		/* composite numeric types */
		GReferenceTypeComplex =            0x07,
		GReferenceTypeRational =           0x08,
		GReferenceTypeDecimalLimited =     0x09,
		GReferenceTypeDecimalUnlimited =   0x0a,

		/* array type */
		GReferenceTypeArray =              0x0b,
		/* when a GValue pointer did not fit into 48 bits, the future is upon us, or more likely Solaris */
		GReferenceTypeNestedPointer =      0x0c,
		/* to break strong reference cycles */
		GReferenceTypeWeakPointer =        0x0d
	};

	union GReferenceValue;

	struct GTypedVariableLink {
	public:
		const char *name;
		struct GTypedVariable variable;
		struct GTypedVariableLink *next;
	};

	/* Here's an optimization idea: pre-allocate an array of GReferences to be used whenever a new one
	 * would be needed, automatically grow it etc. (like std::vector<GReference> or something), to prevent
	 * some random memory accesses (also that could open the possibility of garbage collection...)
	 */
	struct GReference {
	public:
		struct std::atomic<union GReferenceValue *> const referenced_value;
		struct std::atomic<STRONG_REFERENCE_COUNT_TYPE> strong_reference_count;
		struct std::atomic<WEAK_REFERENCE_COUNT_TYPE> weak_reference_count;

		GReference(union GReferenceValue *);
		~GReference() noexcept;

		void
		retain(void * /* env */);

		void
		release(void * /* env */);

		GValue /* tagged pointer to GReference to CWeakReference */
		downgrade(void * /* env */);

		STRONG_REFERENCE_COUNT_TYPE
		strong_count() const noexcept;

		WEAK_REFERENCE_COUNT_TYPE
		weak_count() const noexcept;

	private:
		void
		release_slow(void * /* env */);
	};

	struct GReferenceCommon {
		const GReferenceType reference_type;

		GReferenceCommon(GReferenceType);
	};

	struct GObject {
	public:
		struct GReferenceCommon common;
		void *psi_type;
		struct GTypedVariableLink *dynamic_ivars;
		unsigned_integer_8 ivars_count;
		GVariable *ivars;

		static
		GValue
		allocate(void * /* psi_type */, void * /* env */) noexcept;

		GObject(void * /* psi_type */, unsigned_integer_8 /* ivars_count */);
		~GObject();
	};

	struct GUnmanagedUnsafe {
	public:
		struct GReferenceCommon common;
		void *pointer;
		void (*deallocator)(void *);

		GUnmanagedUnsafe(void * /* pointer */, void (*)(void *) /* deallocator */);
		~GUnmanagedUnsafe();

		static
		GValue
		allocate(void * /* pointer */, void (*)(void *) /* deallocator */, void * /* env */) noexcept;
	};

	struct GInteger64 {
	public:
		struct GReferenceCommon common;
		const integer_64 value;

		GInteger64(const integer_64) noexcept;

		static
		GValue
		allocate(const integer_64, void * /* env */) noexcept;
	};

	struct GInteger64Unsigned {
	public:
		struct GReferenceCommon common;
		const unsigned_integer_64 value;

		GInteger64Unsigned(const unsigned_integer_64) noexcept;

		static
		GValue
		allocate(const unsigned_integer_64, void * /* env */) noexcept;
	};

	struct GInteger128 {
	public:
		struct GReferenceCommon common;
		const integer_128 value;

		GInteger128(const integer_128) noexcept;

		static
		GValue
		allocate(const integer_128, void * /* env */) noexcept;
	};

	struct GInteger128Unsigned {
	public:
		struct GReferenceCommon common;
		const unsigned_integer_128 value;

		GInteger128Unsigned(const unsigned_integer_128) noexcept;

		static
		GValue
		allocate(const unsigned_integer_128, void * /* env */) noexcept;
	};

	struct GFloat128 {
	public:
		struct GReferenceCommon common;
		const float_128 value;

		GFloat128(const float_128) noexcept;

		static
		GValue
		allocate(const float_128, void * /* env */) noexcept;
	};

	struct GComplex {
	public:
		struct GReferenceCommon common;
		const GValue real;
		const GValue imaginary;

		GComplex(const GValue, const GValue) noexcept;

		static
		GValue
		allocate(const GValue /* real */, const GValue /* imaginary, obviously */, void * /* env */) noexcept;
	};

	struct GRational {
	public:
		struct GReferenceCommon common;
		const GValue numerator;
		const GValue denominator;

		GRational(const GValue, const GValue) noexcept;

		static
		GValue
		allocate(const GValue /* numerator */, const GValue /* denominator, obviously */, void * /* env */) noexcept;
	};

	struct GDecimalLimited {
	public:
		struct GReferenceCommon common;
		const unsigned_integer_128 value;
		/* Here's the idea: unsigned 128bit integer has at most 39 total digits,
		 * thus that should fit easily into max 2^7 - 1 decimal digits of them, thus we can safely use
		 * the first bit as the sign */
		/** the number of digits *after* the decimal point; first bit is the sign */
		const unsigned_integer_8 decimal_digits;

		GDecimalLimited(const unsigned_integer_128, const unsigned_integer_8) noexcept;

		static
		GValue
		allocate(const unsigned_integer_128, const unsigned_integer_8, void * /* env */) noexcept;
	};

	/* Stop right there for a moment: GDecimalLimited has the ability to save a number as big as
	 * 340 undecillion (340_282_366_920_938_463_463_374_607_431_768_211_455), which is more than 
	 * memory addressable in the full 64bit range (16 EiB), which is A LOT, are you 100% sure
	 * you need more? */
	struct GDecimalUnlimited {
	public:
		struct GReferenceCommon common;
		/* Discussion: each unsigned_integer_8 can hold two base-10 digits without bits interleaving */
		unsigned_integer_8 *const individual_digits;
		const size_t digits_count;
		/** the number of digits *after* the decimal point; first bit is the sign */
		const unsigned_integer_64 decimal_digits; /* we say "unlimited", but 63bit decimal digits is the limit */

		GDecimalUnlimited(unsigned_integer_8 *const, const size_t, const unsigned_integer_64) noexcept;
		~GDecimalUnlimited() noexcept;

		static
		GValue
		allocate(unsigned_integer_8 *const, const size_t, const unsigned_integer_64, void * /* env */) noexcept;
	};

	struct GArray {
	public:
		struct GReferenceCommon common;
		unsigned_integer_64 elements_count;
		GValue *elements;

		static
		GValue
		allocate(const unsigned_integer_64 /* elements_count */, void * /* env */) noexcept;
	};

	/* just do not use this yet */
	struct GNestedPointer {
	public:
		struct GReferenceCommon common;
		union GReferenceValue *pointer;

		static
		GValue
		allocate(union GReferenceValue * /* pointer */, void * /* env */) = delete;
	};

	/* only allocate from an existing strong reference, using downgrade() on GReference */
	struct GWeakPointer {
	public:
		struct GReferenceCommon common;
		std::atomic<GReference *> const pointer;

		GWeakPointer(GReference *);

		GValue
		upgrade(void * /* env */);

		void
		on_release() noexcept;
	};

	union GReferenceValue {
		struct GReferenceCommon common;
		struct GObject object;
		struct GUnmanagedUnsafe unmanaged_unsafe;
		struct GInteger64 integer_64;
		struct GInteger64Unsigned integer_64_unsigned;
		struct GInteger128 integer_128;
		struct GInteger128Unsigned integer_128_unsigned;
		struct GFloat128 float_128;
		struct GComplex complex;
		struct GRational rational;
		struct GDecimalLimited decimal_limited;
		struct GDecimalUnlimited decimal_unlimited;
		struct GArray array;
		struct GNestedPointer nested_pointer;
		struct GWeakPointer weak_pointer;
	};

	typedef struct std::mutex GReferenceLock;

	GReferenceLock &
	lock_for_reference(union GReferenceValue *) noexcept;
}

#endif /* defined(__gear_vm__reference__) */
