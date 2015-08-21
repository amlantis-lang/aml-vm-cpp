#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDecNumberState::FirstPassDecNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDecNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		
	}
}
