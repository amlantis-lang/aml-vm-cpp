#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Generate documentation for a Aml module or Aml script.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amldoc", argc, argv))->run();
}
