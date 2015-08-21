#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassSxgsNumberState::FirstPassSxgsNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassSxgsNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
