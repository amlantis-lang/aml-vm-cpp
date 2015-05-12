#include "../cli/cli.hpp"

using CVM::Cli;

/**
 * Run a Coral module or Coral script inside a CVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
