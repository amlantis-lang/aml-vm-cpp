#include "../cli/cli.hpp"

using AVM::Cli;

/**
 * Run various bundling tools to create, assemble and compile a Aml module and its dependencies,
 * and to publish a Aml module and manage it.
 *
 * Name comes from "Aml Factory"
 */

int
main(const int argc, const char **argv) {
	return (new Cli("amlf", argc, argv))->run();
}
