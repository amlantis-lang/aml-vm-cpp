#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassRegexpState::FirstPassRegexpState(RawLexicalToken rawToken, UChar32 delimiter)
		:	FirstPassState(rawToken),
			delimiter(delimiter) {};

	void
	Lexer::
	FirstPassRegexpState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
