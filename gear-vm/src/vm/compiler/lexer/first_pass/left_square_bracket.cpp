#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassLeftSquareBracketState::FirstPassLeftSquareBracketState() {};

	void
	Lexer::
	FirstPassLeftSquareBracketState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* [ */
		accept(inputChar);
		rawToken.item = RawLexicalItemLeftSquareBracket;
		machine.appendToOutput(rawToken);
		machine.pushParenthesesElement(ParenthesesElementBasic);
		machine.incrementSquareBracketCounter();
		machine.changeState(new Lexer::FirstPassStartState);
	}

}
