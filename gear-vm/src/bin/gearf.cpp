#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Run various bundling tools to create, assemble and compile a Gear module and its dependencies,
 * and to publish a Gear module and manage it.
 *
 * Name comes from "Gear Factory"
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
