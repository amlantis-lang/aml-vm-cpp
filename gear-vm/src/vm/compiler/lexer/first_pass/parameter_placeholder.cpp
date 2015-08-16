#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassParameterPlaceholderState::FirstPassParameterPlaceholderState(RawLexicalToken rawToken)
 		:	FirstPassState(rawToken) {}

}
