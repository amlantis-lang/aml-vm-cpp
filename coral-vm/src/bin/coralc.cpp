#include "../cli/cli.hpp"

using CVM::Cli;

/**
 * Compile a Coral module or Coral script for CVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
