#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDuodecimalFloatingOrFixedPointNumberState::
	FirstPassDuodecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDuodecimalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
