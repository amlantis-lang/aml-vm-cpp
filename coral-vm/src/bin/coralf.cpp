#include "../cli/cli.hpp"

using CVM::Cli;

/**
 * Run various bundling tools to create, assemble and compile a Coral module and its dependencies,
 * and to publish a Coral module and manage it.
 *
 * Name comes from "Faceting machine"
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
