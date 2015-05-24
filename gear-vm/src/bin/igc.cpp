#include "../cli/cli.hpp"

using GVM::Cli;

/**
 * Runs Gear programs interactively.
 *
 * Name comes from "Interactive Gear Console"
 */

int
main(const int argc, const char **argv) {
	return (new Cli(argc, argv))->run();
}
