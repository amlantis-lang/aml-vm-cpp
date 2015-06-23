#ifndef __gear_vm__cli__
#define __gear_vm__cli__

#include "argv_input.hpp"
#include "input_definition.hpp"

#pragma GCC visibility push(default)

namespace GVM {
	class Cli {
		Argv_Input *input;
		const char *cmd;

	public:
		Cli(const char *cmd, const int argc, const char **argv);
		~Cli(void);

		int run(void);
	};
}

#pragma GCC visibility pop

#endif /* defined(__gear_vm__cli__) */
