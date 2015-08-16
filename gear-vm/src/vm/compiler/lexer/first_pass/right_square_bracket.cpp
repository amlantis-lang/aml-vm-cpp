#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassRightSquareBracketState::FirstPassRightSquareBracketState() {};

	void
	Lexer::
	FirstPassRightSquareBracketState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ] */
		if (machine.squareBracketCount() >= 1) {
			accept(inputChar);
			switch (machine.peekParenthesesElement()) {
				case ParenthesesElementBasic:
					rawToken.item = RawLexicalItemRightSquareBracket;
					break;
				case ParenthesesElementList:
					rawToken.item = RawLexicalItemListLiteralEnd;
					break;
				default:
					throw "Invalid parentheses elements stack state";
			}
			machine.popParenthesesElement();
			machine.decrementSquareBracketCounter();
			machine.appendToOutput(rawToken);
			machine.changeState(new Lexer::FirstPassStartState);
		} else {
			throw "Uneven right square bracket";
		}
	}

}
