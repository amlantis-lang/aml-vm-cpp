#include "../lexer.hpp"

namespace GVM {

	Lexer::
	FirstPassQuotedStringState::FirstPassQuotedStringState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassQuotedStringState::handle(FirstPassMachine &machine, UChar32 inputChar) {

	}
}
