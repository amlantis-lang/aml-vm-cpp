#include "../lexer.hpp"

namespace GVM {

	Lexer::
	FirstPassSubstitutionRegexpState::FirstPassSubstitutionRegexpState(RawLexicalToken rawToken, UChar32 delimiter)
		:	FirstPassState(rawToken),
			delimiter(delimiter) {};

}
