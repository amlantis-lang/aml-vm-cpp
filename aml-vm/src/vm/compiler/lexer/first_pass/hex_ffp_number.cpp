#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassHexadecimalFloatingOrFixedPointNumberState::
	FirstPassHexadecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken)
	{
		UChar32 const &lastChar = rawToken.rawValue.back();
	};

	void
	Lexer::
	FirstPassHexadecimalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
