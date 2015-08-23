#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassBinaryFloatingOrFixedPointNumberState
	::FirstPassBinaryFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassBinaryFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
