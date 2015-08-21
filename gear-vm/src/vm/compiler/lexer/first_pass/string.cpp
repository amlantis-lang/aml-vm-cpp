#include "../lexer.hpp"

namespace AVM {
  Lexer::
	FirstPassStringState::FirstPassStringState() {}

	Lexer::
	FirstPassStringState::FirstPassStringState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassStringState::handle(FirstPassMachine &machine, UChar32 inputChar) {

	}
}
