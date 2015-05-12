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

namespace CVM {

	Cli::Cli(const int argc, const char **argv) {
		input = new Argv_Input(argc, argv, nullptr);
	}

	Cli::~Cli() {
		delete input;
	}

	int
	Cli::run(void) {
		std::cout << std::boolalpha;
		std::cout << "sizeof CValue: " << sizeof(CValue) << std::endl;
		std::cout << "sizeof CValueLink: " << sizeof(CValueLink) << std::endl;
		std::cout << "sizeof atomic<CValue>: " << sizeof(std::atomic<CValue>) << std::endl;
		std::cout << "sizeof atomic<CValueLink>: " << sizeof(std::atomic<CValueLink>) << std::endl;
		std::cout << "sizeof pair<CValue, void *>: " << sizeof(std::pair<CValue, void *>) << std::endl;
		std::cout << "sizeof CVariable: " << sizeof(CVariable) << std::endl;
		std::cout << "sizeof CTypedVariable: " << sizeof(CTypedVariable) << std::endl;
		std::cout << "sizeof atomic_llong: " << sizeof(std::atomic_llong) << std::endl;
		std::cout << "sizeof atomic_long: " << sizeof(std::atomic_long) << std::endl;
		std::cout << "sizeof CObject: " << sizeof(CObject) << std::endl;
		std::atomic<CValueLink> val1;
		std::atomic_llong val2;
		std::atomic_long val3;
		std::cout << "lock free atomic<CValueLink>: " << val1.is_lock_free() << std::endl;
		std::cout << "lock free atomic_llong: " << val2.is_lock_free() << std::endl;
		std::cout << "lock free atomic_long: " << val3.is_lock_free() << std::endl;
		std::cout << "value of kNilLink: " << CValue::kNilLink << std::endl;
		std::cout << "value of kUndefinedLink: " << CValue::kUndefinedLink << std::endl;
		std::cout << "value of kYesLink: " << CValue::kYesLink << std::endl;
		std::cout << "value of kNoLink: " << CValue::kNoLink << std::endl;
		std::cout << "value of kUnitLink: " << CValue::kUnitLink << std::endl;
		std::cout << "kUnit is float: " << CValue::kUnit.is_float_64() << std::endl;
		std::cout << "kUndefined is float: " << CValue::kUndefined.is_float_64() << std::endl;
		std::cout << "kUndefined is number: " << CValue::kUndefined.is_number() << std::endl;
		std::cout << "number is number: " << CValue(CValue::EncodeAsFloat64, 1.42).is_number() << std::endl;

		CValue some_object = CObject::allocate(nullptr, nullptr);
		CReference *reference = some_object.as_reference();
		assert(reference->strong_count() == 1);
		assert(reference->weak_count() == 0);
		reference->retain(nullptr);
		assert(reference->strong_count() == 2);
		assert(reference->weak_count() == 0);
		CValue weak_reference = reference->downgrade(nullptr);
		assert(reference->strong_count() == 2);
		assert(reference->weak_count() == 1);
		CReference *weak_reference_reference = weak_reference.as_reference();
		assert(weak_reference_reference->strong_count() == 1);
		assert(weak_reference_reference->weak_count() == 0);
		CReferenceValue *value = weak_reference_reference->referenced_value.load(std::memory_order_relaxed);
		assert(value->common.reference_type == CReferenceTypeWeakPointer);
		CWeakPointer *weak_pointer = reinterpret_cast<CWeakPointer *>(value);
		CValue upgraded = weak_pointer->upgrade(nullptr);
		assert(reference->strong_count() == 3);
		assert(reference->weak_count() == 1);
		assert(upgraded.is_reference());

		return 0;
	}

}
