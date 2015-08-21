#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassBinNumberState::FirstPassBinNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassBinNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}
