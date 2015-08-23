#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassHexadecimalFloatingOrFixedPointNumberState::
	FirstPassHexadecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassHexadecimalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
