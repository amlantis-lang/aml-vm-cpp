#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Compile a Gear module or Gear script for GVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
