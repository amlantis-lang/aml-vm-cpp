#include <unordered_map>
#include <limits>
#include <iostream>
#include "value.hpp"
#include "../../lib/predef.hpp"

using cvm::vm::memory::value_link;
using cvm::vm::memory::value_type_nil;
using cvm::vm::memory::value_type_integer_8;
using cvm::vm::memory::nil_or_undefined;
using cvm::vm::memory::integer_8_type;

const value_link *const Const_nil_value = (value_link *) new nil_or_undefined {
	.value_type_tag = value_type_nil,
	.nil_value = 0
};

const value_link *const Const_undefined_value = (value_link *) new nil_or_undefined {
	.value_type_tag = value_type_nil,
	.nil_value = 1
};

void
cvm::vm::memory::reference_counting_aware_deleter::operator()(const value_link *value_link) {
	if (value_link) {
		if (value_link->value_type_tag == value_type_reference) {
			const reference_type *object_value = reinterpret_cast<const reference_type *>(value_link);
			if (object_value->reference_value->reference_count == 0) {
				delete object_value->reference_value;
				delete value_link;
			} else {
				std::cerr << "Attempted to delete an unreleased object @ "
					<< value_link
					<< " @@ "
					<< object_value->reference_value
					<< std::endl;
			}
		} else {
			delete value_link;
		}
	}
}

void
cvm::vm::memory::thread_aware_deleter::operator()(const value_link *value_link) {
	if (value_link) {
		if (value_link->value_type_tag == value_type_thread) {
			const thread_type *thread_object = reinterpret_cast<const thread_type *>(value_link);
			if (thread_object->thread_value) {
				if (thread_object->thread_value->joinable()) {
					thread_object->thread_value->join();
					delete thread_object->thread_value;
				}
				delete thread_object;
			}
		} else {
			delete value_link;
		}
	}
}

static std::unordered_map<integer_8, std::shared_ptr<const value_link>>
initialize_small_integral_links() {
	std::unordered_map<
		integer_8,
		std::shared_ptr<const value_link>
	> map;

	integer_8 i = std::numeric_limits<integer_8>::min();
	const integer_8 N = std::numeric_limits<integer_8>::max();

	for (; i <= N; i += 1) {
		map.insert({
			i,
			std::shared_ptr<const value_link>((const value_link *) new integer_8_type {
				.value_type_tag = value_type_integer_8,
				.integer_8_value = i })
		});
		if (i == N) break;
	}
	return map;
}

std::unordered_map<
	integer_8,
	std::shared_ptr<const value_link>
> Const_small_integral_links = initialize_small_integral_links();

std::shared_ptr<const value_link>
cvm::vm::memory::get_link_for_number(integer_8 number) {
	return std::shared_ptr<const value_link>(Const_small_integral_links[number]);
};

std::shared_ptr<const value_link>
cvm::vm::memory::get_link_for_number(unsigned_integer_8 number) {
	if (number <= std::numeric_limits<integer_8>::max()) {
		return std::shared_ptr<const value_link>(Const_small_integral_links[static_cast<integer_8>(number)]);
	} else {
		return std::shared_ptr<const value_link>((const value_link *) new integer_8_unsigned_type {
			.value_type_tag = value_type_unsigned_integer_8,
			.integer_8_unsigned_value = number });
	}
};

std::shared_ptr<const value_link>
cvm::vm::memory::get_link_for_number(integer_16 number) {
	if (number <= std::numeric_limits<integer_8>::max()
			&& number >= std::numeric_limits<integer_8>::min()) {
		return std::shared_ptr<const value_link>(Const_small_integral_links[static_cast<integer_8>(number)]);
	} else {
		return std::shared_ptr<const value_link>((const value_link *) new integer_16_type {
			.value_type_tag = value_type_integer_16,
			.integer_16_value = number });
	}
};

std::shared_ptr<const value_link>
cvm::vm::memory::get_link_for_number(unsigned_integer_16 number) {
	if (number <= std::numeric_limits<integer_8>::max()) {
		return std::shared_ptr<const value_link>(Const_small_integral_links[static_cast<integer_8>(number)]);
	} else {
		return std::shared_ptr<const value_link>((const value_link *) new integer_16_unsigned_type {
			.value_type_tag = value_type_unsigned_integer_16,
			.integer_16_unsigned_value = number });
	}
};

cvm::vm::memory::object::object() {
	reference_count = 1;
	ivars_count = 0;
	dynamic_ivars = nullptr;
}

cvm::vm::memory::object::~object() {
}

