#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassListOrArrayState::FirstPassListOrArrayState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassListOrArrayState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (inputChar == VerticalLine) {
			accept(inputChar);
			rawToken.item = RawLexicalItemArrayLiteralStart;
			machine.pushParenthesesElement(ParenthesesElementArray);
			machine.incrementSquareBracketCounter();
			machine.appendToOutput(rawToken);
			machine.changeState(new FirstPassStartState);
		} else {
			rawToken.item = RawLexicalItemListLiteralStart;
			machine.pushParenthesesElement(ParenthesesElementList);
			machine.incrementSquareBracketCounter();
			machine.appendToOutput(rawToken);
			machine.changeState(new FirstPassStartState);
			machine.handle(inputChar);
		}
	}
}
