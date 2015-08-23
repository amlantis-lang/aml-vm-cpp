#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDecimalFloatingOrFixedPointNumberState::
	FirstPassDecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDecimalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
