#include "cli/cli.hpp"

#include <iostream>

using cvm::cli::Cli;

int
main(const int argc, const char **argv) {
	try {
		return (new Cli(argc, argv))->run();
	} catch (const char *exception) {
		std::cout << exception << std::endl;
		return 1;
	}
}
