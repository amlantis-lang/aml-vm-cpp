#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Profiles Aml programs.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amlprof", argc, argv))->run();
}
