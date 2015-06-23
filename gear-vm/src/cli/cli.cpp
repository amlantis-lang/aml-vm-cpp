#include <iostream>
#include <atomic>
#include <utility>
#include <iostream>
#include "cli.hpp"
#include "../vm/memory/value.hpp"
#include "../vm/memory/value_inlines.hpp"
#include "../vm/memory/variable.hpp"
#include "../vm/memory/variable_inlines.hpp"
#include "../vm/memory/reference.hpp"
#include "../vm/memory/reference_inlines.hpp"

#include "../vm/compiler/lexer/lexer.hpp"
#include "../vm/compiler/lexer/file_input_stream.hpp"


namespace GVM {

	Cli::Cli(const char *cmd, const int argc, const char **argv)
		: cmd(cmd)
	{
		input = new Argv_Input(argc, argv, nullptr);
	}

	Cli::~Cli() {
		delete input;
	}

	int
	Cli::run(void) {
		std::cout << cmd << " 0.2.0" << std::endl;

		auto finputStream = new FileInputStream("/Users/themarketka/test.gear", "UTF-8");
		auto lexer = new Lexer;
		auto parsed = lexer->parseInputStream(finputStream);

		delete finputStream;
		delete parsed;

		return 0;
	}

}
