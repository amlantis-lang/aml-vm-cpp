#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassRightSquareBracketState::FirstPassRightSquareBracketState() {};

	void
	Lexer::
	FirstPassRightSquareBracketState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ] */
		if (machine.squareBracketCount() >= 1) {
			if (inputChar == RightSquareBracket) {
				accept(inputChar);
			} else {
				throw "Unexpected input character, expected right square bracket";
			}
			switch (machine.peekParenthesesElement()) {
				case ParenthesesElementBasic:
					rawToken.item = RawLexicalItemRightSquareBracket;
					break;
				case ParenthesesElementList:
					rawToken.item = RawLexicalItemListLiteralEnd;
					break;
				case ParenthesesElementPolymorphicType:
					if (rawToken.rawValue.size() == 1) {
						rawToken.item = RawLexicalItemPolymorphicTypeEnd;
						machine.popParenthesesElement();
						machine.decrementSquareBracketCounter();
						return; /* prevent state change, there is one more square bracket to come */
					}
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
