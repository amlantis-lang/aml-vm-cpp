#ifndef __coral_vm__value__
#define __coral_vm__value__

#include "../../lib/predef.h"

namespace cvm {
	namespace vm {
		namespace memory {
			enum value_type : short {
				value_type_nil = 0x00,
				value_type_reference = 0x01,
				value_type_integer_8 = 0x02,
				value_type_unsigned_integer_8 = 0x03,
				value_type_integer_16 = 0x04,
				value_type_unsigned_integer_16 = 0x05,
				value_type_integer_32 = 0x06,
				value_type_unsigned_integer_32 = 0x07,
				value_type_integer_64 = 0x08,
				value_type_unsigned_integer_64 = 0x09
			};

			struct nil_or_undefined {
				enum value_type const value_type_tag;
				unsigned int const nil_value:1;
			};

#     pragma pack(2)
			struct reference_type {
				enum value_type const value_type_tag;
				void *reference_value;
			};

#     pragma pack(1)
			struct integer_8_type {
				enum value_type const value_type_tag;
				integer_8 integer_8_value;
			};

#     pragma pack(2)
			struct integer_16_type {
				enum value_type const value_type_tag;
				integer_16 integer_16_value;
			};

			struct integer_32_type {
				enum value_type const value_type_tag;
				integer_32 integer_32_value;
			};

			struct integer_64_type {
				enum value_type const value_type_tag;
				integer_64 integer_64_value;
			};

			struct integer_128_type {
				enum value_type const value_type_tag;
				integer_128 integer_64_value;
			};

			union value_link {
				struct nil_or_undefined;
				struct reference_type;
				struct integer_32_type;
			};
		}
	}
}

#endif /* defined(__coral_vm__value__) */
