#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Compile a Aml module or Aml script for AVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amlc", argc, argv))->run();
}
