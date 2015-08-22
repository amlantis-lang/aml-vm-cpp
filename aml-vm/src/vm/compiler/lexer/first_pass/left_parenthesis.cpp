#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassLeftParenthesisState::FirstPassLeftParenthesisState() {};
	
	void
	Lexer::
	FirstPassLeftParenthesisState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				/* ( */
				accept(inputChar);
				break;

			case 1:
				if (inputChar == Asterisk) {
					/* (* */
					accept(inputChar);
				} else {
					rawToken.item = RawLexicalItemLeftParenthesis;
					machine.appendToOutput(rawToken);
					machine.pushParenthesesElement(ParenthesesElementBasic);
					machine.incrementParenthesisCounter();
					machine.changeState(new Lexer::FirstPassStartState);
				}
				break;

			case 2:
				if (u_isblank_55(inputChar)) {
					/* (*_ */
					accept(inputChar);
					machine.changeState(new FirstPassCommentState(rawToken));
				} else {
					rawToken.rawValue.pop_back();
					rawToken.item = RawLexicalItemLeftParenthesis;
					machine.appendToOutput(rawToken);
					machine.pushParenthesesElement(ParenthesesElementBasic);
					machine.incrementParenthesisCounter();

					machine.changeState(new FirstPassOperatorState);
					machine.handle(Asterisk);
				}
				break;

			default:
				throw "Illegal state";
		}
	}

}
