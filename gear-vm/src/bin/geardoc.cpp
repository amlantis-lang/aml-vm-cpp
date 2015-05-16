#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Generate documentation for a Gear module or Gear script.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
