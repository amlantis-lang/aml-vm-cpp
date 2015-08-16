#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassLeftSquareBracketState::FirstPassLeftSquareBracketState() {};

	void
	Lexer::
	FirstPassLeftSquareBracketState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				/* [ */
				accept(inputChar);
				break;
			case 1:
				if (inputChar == LeftSquareBracket) {
					/* [[ */
					accept(inputChar);
				} else {
					/* just [ + something else */
					rawToken.item = RawLexicalItemLeftSquareBracket;
					machine.appendToOutput(rawToken);
					machine.pushParenthesesElement(ParenthesesElementBasic);
					machine.incrementSquareBracketCounter();
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				}
				break;
			case 2:
				if (inputChar == GreaterThanSign) {
					/* [[> */
					accept(inputChar);
					rawToken.item = RawLexicalItemPolymorphicTypeStart;
					machine.appendToOutput(rawToken);

					/* increment once for each square bracket */
					machine.pushParenthesesElement(ParenthesesElementPolymorphicType);
					machine.incrementSquareBracketCounter();
					machine.pushParenthesesElement(ParenthesesElementPolymorphicType);
					machine.incrementSquareBracketCounter();

					machine.changeState(new FirstPassStartState);
				} else if (inputChar == LessThanSign) {
					/* [[< */
					accept(inputChar);
					rawToken.item = RawLexicalItemPolymorphicTypeStart;
					machine.appendToOutput(rawToken);

					/* increment once for each square bracket */
					machine.pushParenthesesElement(ParenthesesElementPolymorphicType);
					machine.incrementSquareBracketCounter();
					machine.pushParenthesesElement(ParenthesesElementPolymorphicType);
					machine.incrementSquareBracketCounter();

					machine.changeState(new FirstPassStartState);
				} else {
					/* just [[ + something else */
					rawToken.item = RawLexicalItemPolymorphicTypeStart;
					machine.appendToOutput(rawToken);

					/* increment once for each square bracket */
					machine.pushParenthesesElement(ParenthesesElementPolymorphicType);
					machine.incrementSquareBracketCounter();
					machine.pushParenthesesElement(ParenthesesElementPolymorphicType);
					machine.incrementSquareBracketCounter();

					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				}
				break;
			default:
				throw "Illegal state";
		}
	}

}
