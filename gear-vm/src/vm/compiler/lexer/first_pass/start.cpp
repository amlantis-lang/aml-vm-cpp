#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassStartState::FirstPassStartState() {};

	void
	Lexer::
	FirstPassStartState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isLineBreakStart(inputChar)) {
			machine.changeState(new FirstPassNewlineState);
			machine.handle(inputChar);
		} else if (inputChar == LeftParens) {
			machine.changeState(new FirstPassLeftParenthesisState);
			machine.handle(inputChar);
		} else if (inputChar == RightParens) {
			machine.changeState(new FirstPassRightParenthesisState);
			machine.handle(inputChar);
		} else if (inputChar == LeftSquareBracket) {
			machine.changeState(new FirstPassLeftSquareBracketState);
			machine.handle(inputChar);
		} else if (inputChar == RightSquareBracket) {
			machine.changeState(new FirstPassRightSquareBracketState);
			machine.handle(inputChar);
		} else if (inputChar == LeftBrace) {
			machine.changeState(new FirstPassLeftBraceState);
			machine.handle(inputChar);
		} else if (inputChar == RightBrace) {
			machine.changeState(new FirstPassRightBraceState);
			machine.handle(inputChar);
		} else if (inputChar == PercentSign) {
			machine.changeState(new FirstPassOperatorOrLiteralState);
			machine.handle(inputChar);
		} else if (Lexer::isGearIdentifierStart(inputChar)) {
			machine.changeState(new FirstPassIdentifierState);
			machine.handle(inputChar);
		} else if (inputChar == Backtick) {
			machine.changeState(new FirstPassQuotedIdentifierState);
			machine.handle(inputChar);
		} else if (Lexer::isGearOperatorChar(inputChar)) {
			machine.changeState(new FirstPassOperatorState);
			machine.handle(inputChar);
		} else if (inputChar == Comma) {
			machine.changeState(new FirstPassCommaState);
			machine.handle(inputChar);
		} else if (Lexer::isDigitChar(inputChar)) {
			machine.changeState(new FirstPassNumberState);
			machine.handle(inputChar);
		}
	};

}
