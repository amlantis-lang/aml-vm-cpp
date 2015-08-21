#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassOperatorState::FirstPassOperatorState() {}

	void
	Lexer::
	FirstPassOperatorState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isAmlOperatorChar(inputChar)) {
			/* next operator character */
			accept(inputChar);
		} else if (inputChar == RightSquareBracket
				&& machine.peekParenthesesElement() == ParenthesesElementArray
				&& rawToken.rawValue.size() == 1
				&& rawToken.rawValue.at(0) == VerticalLine) {
			/* array literal end: |] */
			accept(inputChar);
			rawToken.item = RawLexicalItemArrayLiteralEnd;
			machine.appendToOutput(rawToken);
			machine.popParenthesesElement();
			machine.decrementSquareBracketCounter();
			machine.changeState(new FirstPassStartState);
		} else {
			/* an operator + something else */
			rawToken.item = RawLexicalItemOperatorOrDelimiter;
			machine.appendToOutput(rawToken);
			machine.changeState(new FirstPassStartState);
			machine.handle(inputChar);
		}
	}
}
