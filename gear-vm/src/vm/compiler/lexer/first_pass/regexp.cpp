#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassRegexpState::FirstPassRegexpState(RawLexicalToken rawToken, UChar32 delimiter)
		:	FirstPassState(rawToken),
			delimiter(delimiter) {};

	void
	Lexer::
	FirstPassRegexpState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
