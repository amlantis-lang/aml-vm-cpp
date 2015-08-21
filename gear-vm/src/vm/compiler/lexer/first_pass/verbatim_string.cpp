#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassVerbatimStringState::FirstPassVerbatimStringState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassVerbatimStringState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		
	}
}
