#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassOctalFloatingOrFixedPointNumberState::
	FirstPassOctalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassOctalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
