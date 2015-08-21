#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassOctNumberState::FirstPassOctNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassOctNumberState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
