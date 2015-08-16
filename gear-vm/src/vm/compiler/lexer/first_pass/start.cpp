#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassStartState::FirstPassStartState() {};

	void
	Lexer::
	FirstPassStartState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isLineBreakStart(inputChar)) {
			machine.changeState(new Lexer::FirstPassNewlineState);
			machine.handle(inputChar);
		} else if (inputChar == LeftParens) {
			machine.changeState(new Lexer::FirstPassLeftParenthesisState);
			machine.handle(inputChar);
		} else if (inputChar == RightParens) {
			machine.changeState(new Lexer::FirstPassRightParenthesisState);
			machine.handle(inputChar);
		} else if (inputChar == LeftSquareBracket) {
			machine.changeState(new Lexer::FirstPassLeftSquareBracketState);
			machine.handle(inputChar);
		} else if (inputChar == RightSquareBracket) {
			machine.changeState(new Lexer::FirstPassRightSquareBracketState);
			machine.handle(inputChar);
		} else if (inputChar == LeftBrace) {
			machine.changeState(new Lexer::FirstPassLeftBraceState);
			machine.handle(inputChar);
		} else if (inputChar == RightBrace) {
			machine.changeState(new Lexer::FirstPassRightBraceState);
			machine.handle(inputChar);
		} else if (inputChar == PercentSign) {
			machine.changeState(new Lexer::FirstPassOperatorOrLiteralState);
			machine.handle(inputChar);
		}
	};

}
