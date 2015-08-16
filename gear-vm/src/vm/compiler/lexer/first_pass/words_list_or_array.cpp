#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassWordsListOrArrayState::FirstPassWordsListOrArrayState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

}
