#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassSxgsNumberState::FirstPassSxgsNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassSxgsNumberState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
