#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassBinNumberState::FirstPassBinNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassBinNumberState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
