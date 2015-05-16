#include <iostream>
#include <atomic>
#include <utility>
#include "cli.hpp"
#include "../vm/memory/value.hpp"
#include "../vm/memory/value_inlines.hpp"
#include "../vm/memory/variable.hpp"
#include "../vm/memory/variable_inlines.hpp"
#include "../vm/memory/reference.hpp"
#include "../vm/memory/reference_inlines.hpp"

namespace GVM {

	Cli::Cli(const int argc, const char **argv) {
		input = new Argv_Input(argc, argv, nullptr);
	}

	Cli::~Cli() {
		delete input;
	}

	int
	Cli::run(void) {
		std::cout << std::boolalpha;
		std::cout << "sizeof GValue: " << sizeof(GValue) << std::endl;
		std::cout << "sizeof GValueLink: " << sizeof(GValueLink) << std::endl;
		std::cout << "sizeof atomic<GValue>: " << sizeof(std::atomic<GValue>) << std::endl;
		std::cout << "sizeof atomic<GValueLink>: " << sizeof(std::atomic<GValueLink>) << std::endl;
		std::cout << "sizeof pair<GValue, void *>: " << sizeof(std::pair<GValue, void *>) << std::endl;
		std::cout << "sizeof GVariable: " << sizeof(GVariable) << std::endl;
		std::cout << "sizeof GTypedVariable: " << sizeof(GTypedVariable) << std::endl;
		std::cout << "sizeof atomic_llong: " << sizeof(std::atomic_llong) << std::endl;
		std::cout << "sizeof atomic_long: " << sizeof(std::atomic_long) << std::endl;
		std::cout << "sizeof GObject: " << sizeof(GObject) << std::endl;
		std::atomic<GValueLink> val1;
		std::atomic_llong val2;
		std::atomic_long val3;
		std::cout << "lock free atomic<GValueLink>: " << val1.is_lock_free() << std::endl;
		std::cout << "lock free atomic_llong: " << val2.is_lock_free() << std::endl;
		std::cout << "lock free atomic_long: " << val3.is_lock_free() << std::endl;
		std::cout << "value of kNilLink: " << GValue::kNilLink << std::endl;
		std::cout << "value of kUndefinedLink: " << GValue::kUndefinedLink << std::endl;
		std::cout << "value of kYesLink: " << GValue::kYesLink << std::endl;
		std::cout << "value of kNoLink: " << GValue::kNoLink << std::endl;
		std::cout << "value of kUnitLink: " << GValue::kUnitLink << std::endl;
		std::cout << "kUnit is float: " << GValue::kUnit.is_float_64() << std::endl;
		std::cout << "kUndefined is float: " << GValue::kUndefined.is_float_64() << std::endl;
		std::cout << "kUndefined is number: " << GValue::kUndefined.is_number() << std::endl;
		std::cout << "number is number: " << GValue(GValue::EncodeAsFloat64, 1.42).is_number() << std::endl;

		GValue some_object = GObject::allocate(nullptr, nullptr);
		GReference *reference = some_object.as_reference();
		assert(reference->strong_count() == 1);
		assert(reference->weak_count() == 0);
		reference->retain(nullptr);
		assert(reference->strong_count() == 2);
		assert(reference->weak_count() == 0);
		GValue weak_reference = reference->downgrade(nullptr);
		assert(reference->strong_count() == 2);
		assert(reference->weak_count() == 1);
		GReference *weak_reference_reference = weak_reference.as_reference();
		assert(weak_reference_reference->strong_count() == 1);
		assert(weak_reference_reference->weak_count() == 0);
		GReferenceValue *value = weak_reference_reference->referenced_value.load(std::memory_order_relaxed);
		assert(value->common.reference_type == GReferenceTypeWeakPointer);
		GWeakPointer *weak_pointer = reinterpret_cast<GWeakPointer *>(value);
		GValue upgraded = weak_pointer->upgrade(nullptr);
		assert(reference->strong_count() == 3);
		assert(reference->weak_count() == 1);
		assert(upgraded.is_reference());

		return 0;
	}

}
