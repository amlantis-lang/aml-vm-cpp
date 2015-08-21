#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassDdecNumberState::FirstPassDdecNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDdecNumberState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
