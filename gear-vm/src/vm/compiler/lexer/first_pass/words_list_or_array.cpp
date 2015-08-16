#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassWordsListOrArrayState::FirstPassWordsListOrArrayState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassWordsListOrArrayState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		
	}
}
