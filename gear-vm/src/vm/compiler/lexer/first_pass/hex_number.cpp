#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassDecNumberState::FirstPassDecNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDecNumberState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
