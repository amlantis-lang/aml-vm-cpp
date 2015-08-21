#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassQuotedStringState::FirstPassQuotedStringState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassQuotedStringState::handle(FirstPassMachine &machine, UChar32 inputChar) {

	}
}
