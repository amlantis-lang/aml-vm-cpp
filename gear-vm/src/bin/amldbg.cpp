#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Debugs Aml programs.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amldbg", argc, argv))->run();
}
