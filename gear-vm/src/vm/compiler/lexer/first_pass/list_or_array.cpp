#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassListOrArrayState::FirstPassListOrArrayState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

}
