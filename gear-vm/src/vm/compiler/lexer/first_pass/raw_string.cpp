#include "../lexer.hpp"

namespace GVM {

	Lexer::
	FirstPassRawStringState::FirstPassRawStringState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassRawStringState::handle(FirstPassMachine &machine, UChar32 inputChar) {

	}
}
