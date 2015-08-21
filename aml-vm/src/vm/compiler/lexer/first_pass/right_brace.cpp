#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassRightBraceState::FirstPassRightBraceState() {};

	void
	Lexer::
	FirstPassRightBraceState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* } */
		if (machine.braceCount() >= 1) {
			if (inputChar == RightBrace) {
				accept(inputChar);
			} else {
				/* might happen on bad multimap end, e.g. */
				throw "Unexpected input character, expected right brace";
			}
			switch (machine.peekParenthesesElement()) {
				case ParenthesesElementBasic:
					rawToken.item = RawLexicalItemRightBrace;
					break;
				case ParenthesesElementDictionary:
					rawToken.item = RawLexicalItemDictionaryLiteralEnd;
					break;
				case ParenthesesElementMultimap:
					if (rawToken.rawValue.size() == 1) {
						rawToken.item = RawLexicalItemMultimapLiteralEnd;
						/* decrement the second closing brace */
						machine.popParenthesesElement();
						machine.decrementBraceCounter();
						return; /* prevent state change, there is one more brace to come */
					}
					break;
				case ParenthesesElementInterpolatedString:
					/* TODO: interpolated string continuation */
					break;
				case ParenthesesElementInterpolatedSymbol:
					/* TODO: interpolated symbol continuation */
					break;
				case ParenthesesElementInterpolatedRegexp:
					/* TODO: interpolated regexp continuation */
					break;
				default:
					throw "Uneven right brace";
			}
			machine.popParenthesesElement();
			machine.decrementBraceCounter();
			machine.appendToOutput(rawToken);
			machine.changeState(new Lexer::FirstPassStartState);
		}
	}

}
