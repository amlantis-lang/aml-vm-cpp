#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassWordsListOrArrayState::FirstPassWordsListOrArrayState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassWordsListOrArrayState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		
	}
}
