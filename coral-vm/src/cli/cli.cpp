#include <iostream>
#include "cli.hpp"
#include "../vm/memory/value.hpp"

cvm::cli::Cli::Cli(const int argc, const char **argv) {
	input = new Argv_Input(argc, argv, nullptr);
}

cvm::cli::Cli::~Cli() {
	delete input;
}

int
cvm::cli::Cli::run(void) {
	return 0;
}
