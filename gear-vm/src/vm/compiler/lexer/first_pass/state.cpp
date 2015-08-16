#include "../lexer.hpp"

namespace GVM {

	Lexer::
	FirstPassState::FirstPassState() {};

	Lexer::
	FirstPassState::FirstPassState(RawLexicalToken rawToken)
		:	rawToken(rawToken) {}

}
