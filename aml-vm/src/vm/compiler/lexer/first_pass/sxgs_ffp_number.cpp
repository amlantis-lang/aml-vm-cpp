#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassSexagesimalFloatingOrFixedPointNumberState::
	FirstPassSexagesimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassSexagesimalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
