#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Inspect a compiled Gear module or script.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("gearp", argc, argv))->run();
}
