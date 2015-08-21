#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassOctNumberState::FirstPassOctNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassOctNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
