#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDdecNumberState::FirstPassDdecNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDdecNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
