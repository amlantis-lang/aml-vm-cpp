#ifndef __coral_vm__argv_input__
#define __coral_vm__argv_input__

#include "input_definition.cpp"

namespace cvm {
	namespace cli {
		class Argv_Input {
			Input_Definition *input_definition;

		public:
			Argv_Input(const int argc,
								 const char **argv,
								 Input_Definition *input_definition);
		};
	}
}

#endif /* defined(__coral_vm__argv_input__) */
