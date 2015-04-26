#ifndef __coral_vm__value__
#define __coral_vm__value__

#include <memory>
#include <thread>
#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../lib/predef.hpp"
#include "variable.hpp"

namespace cvm {
	namespace vm {
		namespace memory {
			union value_link;
			union integral_number_value_link;
			union any_real_number_value_link;
			class object;
			struct variable;

			enum value_type : unsigned_integer_8 {
				value_type_nil = 0x00,
				value_type_reference = 0x01,
				value_type_integer_8 = 0x02,
				value_type_unsigned_integer_8 = 0x03,
				value_type_integer_16 = 0x04,
				value_type_unsigned_integer_16 = 0x05,
				value_type_integer_32 = 0x06,
				value_type_unsigned_integer_32 = 0x07,
				value_type_integer_64 = 0x08,
				value_type_unsigned_integer_64 = 0x09,
				value_type_float_16 = 0x10,
				value_type_float_32 = 0x11,
				value_type_float_64 = 0x12,
				value_type_float_128 = 0x13,
				value_type_rational = 0x14,
				value_type_complex = 0x15,
				value_type_decimal_limited = 0x16,
				value_type_decimal_unlimited = 0x17,
				value_type_thread = 0x18
			};

			/**
			 * nil_value of 0 = nil
			 * nil_value of 1 = undefined
			 */
			struct nil_or_undefined {
				enum value_type const value_type_tag;
				unsigned int const nil_value:1;
			};

#     pragma pack(1)
			struct reference_type {
				enum value_type const value_type_tag;
				class object *const reference_value;
			};

#     pragma pack(1)
			struct integer_8_type {
				enum value_type const value_type_tag;
				integer_8 const integer_8_value;
			};

#     pragma pack(1)
			struct integer_8_unsigned_type {
				enum value_type const value_type_tag;
				unsigned_integer_8 const integer_8_unsigned_value;
			};

#     pragma pack(1)
			struct integer_16_type {
				enum value_type const value_type_tag;
				integer_16 const integer_16_value;
			};

#     pragma pack(1)
			struct integer_16_unsigned_type {
				enum value_type const value_type_tag;
				unsigned_integer_16 const integer_16_unsigned_value;
			};

			struct integer_32_type {
				enum value_type const value_type_tag;
				integer_32 const integer_32_value;
			};

			struct integer_32_unsigned_type {
				enum value_type const value_type_tag;
				unsigned_integer_32 const integer_32_unsigned_value;
			};

			struct integer_64_type {
				enum value_type const value_type_tag;
				integer_64 const integer_64_value;
			};

			struct integer_64_unsigned_type {
				enum value_type const value_type_tag;
				unsigned_integer_64 const integer_64_unsigned_value;
			};

			struct integer_128_type {
				enum value_type const value_type_tag;
				integer_128 const integer_64_value;
			};

			struct integer_128_unsigned_type {
				enum value_type const value_type_tag;
				unsigned_integer_128 const integer_64_unsigned_value;
			};

#     pragma pack(1)
			struct float_16_type {
				enum value_type const value_type_tag;
				float_16 const float_16_value;
			};

			struct float_32_type {
				enum value_type const value_type_tag;
				float_32 const float_32_value;
			};

			struct float_64_type {
				enum value_type const value_type_tag;
				float_64 const float_64_value;
			};

			struct float_128_type {
				enum value_type const value_type_tag;
				float_128 const float_128_value;
			};

			struct decimal_limited_type {
				enum value_type const value_type_tag;
				unsigned_integer_128 const decimal_limited_value;
				unsigned_integer_8 const decimal_digits;
			};

			struct decimal_unlimited_type {
				enum value_type const value_type_tag;
				unsigned_integer_8 *const decimal_unlimited_value;
				size_t const decimal_unlimited_value_size;
				unsigned_integer_64 const decimal_digits;
			};

			struct rational_type {
				enum value_type const value_type_tag;
				union integral_number_value_link *const numerator;
				union integral_number_value_link *const denominator;
			};

			struct complex_type {
				enum value_type const value_type_tag;
				union any_real_number_value_link *const real_part;
				union any_real_number_value_link *const imaginary_part;
			};

			struct thread_type {
				enum value_type const value_type_tag;
				class std::thread *thread_value;
			};

			union value_link {
				enum value_type const value_type_tag;
				struct nil_or_undefined;
				struct reference_type;
				union any_real_number_value_link;
				struct complex_type;
			};

			union integral_number_value_link {
				enum value_type const value_type_tag;
				struct integer_8_type;
				struct integer_8_unsigned_type;
				struct integer_16_type;
				struct integer_16_unsigned_type;
				struct integer_32_type;
				struct integer_32_unsigned_type;
				struct integer_64_type;
				struct integer_64_unsigned_type;
				struct integer_128_type;
				struct integer_128_unsigned_type;
				struct decimal_limited_type; // when used as integral, decimal_digits limited to 0
				struct decimal_unlimited_type; // when used as integral, decimal_digits limited to 0
			};

			union any_real_number_value_link {
				enum value_type const value_type_tag;
				union integral_number_value_link;
				struct float_16_type;
				struct float_32_type;
				struct float_64_type;
				struct float_128_type;
				struct rational_type;
			};

#     pragma pack(2)
			class object { // TODO: make more layouts? maybe as subclasses?
			public:
				object();
				~object();
				class std::bitset<2> flags;
				unsigned_integer_64 reference_count;
			private:
				unsigned_integer_16 ivars_count;
				class std::unordered_map<std::string, variable> *dynamic_ivars;
				class std::vector<variable *> weaker_references;
				struct variable *ivars[];
			};

			struct reference_counting_aware_deleter {
				void
				operator()(const value_link *);
			};

			struct thread_aware_deleter {
				void
				operator()(const value_link *);
			};

			std::shared_ptr<const value_link>
			get_link_for_number(integer_8 number);

			std::shared_ptr<const value_link>
			get_link_for_number(unsigned_integer_8 number);

			std::shared_ptr<const value_link>
			get_link_for_number(integer_16 number);

			std::shared_ptr<const value_link>
			get_link_for_number(unsigned_integer_16 number);
		}
	}
}

extern const cvm::vm::memory::value_link *const Const_nil_value;
extern const cvm::vm::memory::value_link *const Const_undefined_value;

#endif /* defined(__coral_vm__value__) */
