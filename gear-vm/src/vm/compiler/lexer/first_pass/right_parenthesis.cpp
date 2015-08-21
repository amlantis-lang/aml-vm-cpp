#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassRightParenthesisState::FirstPassRightParenthesisState() {};

	void
	Lexer::
	FirstPassRightParenthesisState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ) */
		if (machine.parenthesisCount() >= 1) {
			accept(inputChar);
			switch (machine.peekParenthesesElement()) {
				case ParenthesesElementBasic:
					rawToken.item = RawLexicalItemRightParenthesis;
					break;
				case ParenthesesElementBag:
					rawToken.item = RawLexicalItemBagLiteralEnd;
					break;
				default:
					throw "Invalid parentheses elements stack state";
			}
			machine.popParenthesesElement();
			machine.decrementParenthesisCounter();
			machine.appendToOutput(rawToken);
			machine.changeState(new Lexer::FirstPassStartState);
		} else {
			throw "Uneven right parenthesis";
		}
	}

}