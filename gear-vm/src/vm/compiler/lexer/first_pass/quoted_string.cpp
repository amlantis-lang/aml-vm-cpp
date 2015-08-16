#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassQuotedStringState::FirstPassQuotedStringState(RawLexicalToken rawToken)
 		:	FirstPassState(rawToken) {}

}
