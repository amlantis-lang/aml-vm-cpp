#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassSubstitutionRegexpState::FirstPassSubstitutionRegexpState(RawLexicalToken rawToken, UChar32 delimiter)
		:	FirstPassState(rawToken),
			delimiter(delimiter) {};

	void
	Lexer::
	FirstPassSubstitutionRegexpState::handle(FirstPassMachine &machine, UChar32 inputChar) {

	}
}
