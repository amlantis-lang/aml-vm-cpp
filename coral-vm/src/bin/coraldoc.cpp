#include "../cli/cli.hpp"

using CVM::Cli;

/**
 * Generate documentation for a Coral module or Coral script.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
