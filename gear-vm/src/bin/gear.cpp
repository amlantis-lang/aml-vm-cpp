#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Run Gear interactively.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("gear", argc, argv))->run();
}
