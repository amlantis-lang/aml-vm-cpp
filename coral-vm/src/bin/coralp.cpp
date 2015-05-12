#include "../cli/cli.hpp"

using CVM::Cli;

/**
 * Inspect a compiled Coral module or script.
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
