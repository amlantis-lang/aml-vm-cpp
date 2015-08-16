#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassLeftParenthesisState::FirstPassLeftParenthesisState() {};
	
	void
	Lexer::
	FirstPassLeftParenthesisState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ( */
		accept(inputChar);
		rawToken.item = RawLexicalItemLeftParenthesis;
		machine.appendToOutput(rawToken);
		machine.pushParenthesesElement(ParenthesesElementBasic);
		machine.incrementParenthesisCounter();
		machine.changeState(new Lexer::FirstPassStartState);
	}

}
