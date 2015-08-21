#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Run Aml interactively.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("aml", argc, argv))->run();
}
