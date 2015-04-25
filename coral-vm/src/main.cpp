#include "cli/cli.hpp"

#include <iostream>

using cvm::cli::Cli;

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
