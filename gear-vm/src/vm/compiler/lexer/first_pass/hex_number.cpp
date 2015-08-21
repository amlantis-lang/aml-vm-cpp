#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassHexNumberState::FirstPassHexNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassHexNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
