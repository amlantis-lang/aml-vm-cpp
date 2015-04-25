#ifndef __coral_vm__cli__
#define __coral_vm__cli__

#include "argv_input.hpp"
#include "input_definition.hpp"

namespace cvm {
	namespace cli {

		class Cli {
			Argv_Input *input;

		public:
			Cli(const int argc, const char **argv);
			~Cli(void);

			int run(void);
		};

	}
}

#endif /* defined(__coral_vm__cli__) */
