#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Run a Aml module or Aml script inside AVM.
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amlrun", argc, argv))->run();
}
