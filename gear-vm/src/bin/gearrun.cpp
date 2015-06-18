#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Run a Gear module or Gear script inside GVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
