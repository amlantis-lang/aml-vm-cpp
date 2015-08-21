#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Inspect a compiled Aml module or script.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amlp", argc, argv))->run();
}
