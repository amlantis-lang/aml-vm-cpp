#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassLeftBraceState::FirstPassLeftBraceState() {};

	void
	Lexer::
	FirstPassLeftBraceState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* { */
		accept(inputChar);
		rawToken.item = RawLexicalItemLeftBrace;
		machine.appendToOutput(rawToken);
		machine.pushParenthesesElement(ParenthesesElementBasic);
		machine.incrementBraceCounter();
		machine.changeState(new Lexer::FirstPassStartState);
	}

}
