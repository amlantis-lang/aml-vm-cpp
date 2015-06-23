#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Compile and optimize a Gear module or Gear script for GVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("gearopt", argc, argv))->run();
}
