#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassState::FirstPassState() {};

	Lexer::
	FirstPassState::FirstPassState(RawLexicalToken rawToken)
		:	rawToken(rawToken) {}

}
