#include <iostream>
#include <atomic>
#include "cli.hpp"
#include "../vm/memory/value.hpp"

CVM::Cli::Cli(const int argc, const char **argv) {
	input = new Argv_Input(argc, argv, nullptr);
}

CVM::Cli::~Cli() {
	delete input;
}

int
CVM::Cli::run(void) {
	std::cout << std::boolalpha;
	std::cout << "sizeof CValue: " << sizeof(CVM::CValue) << std::endl;
	std::cout << "sizeof CValueLink: " << sizeof(CVM::CValueLink) << std::endl;
	std::cout << "sizeof atomic<CValueLink>: " << sizeof(std::atomic<CVM::CValue>) << std::endl;
	std::cout << "sizeof atomic_llong: " << sizeof(std::atomic_llong) << std::endl;
	std::cout << "sizeof atomic_long: " << sizeof(std::atomic_long) << std::endl;
	std::atomic<CVM::CValueLink> val1;
	std::atomic_llong val2;
	std::atomic_long val3;
	std::cout << "lock free atomic<CValueLink>: " << val1.is_lock_free() << std::endl;
	std::cout << "lock free atomic_llong: " << val2.is_lock_free() << std::endl;
	std::cout << "lock free atomic_long: " << val3.is_lock_free() << std::endl;

	return 0;
}
