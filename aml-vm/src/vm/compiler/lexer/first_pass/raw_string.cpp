#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassRawStringState::FirstPassRawStringState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassRawStringState::handle(FirstPassMachine &machine, UChar32 inputChar) {

	}
}
