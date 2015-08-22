#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassHexadecimalNumberState::FirstPassHexadecimalNumberState(RawLexicalToken rawToken)
		: FirstPassNumberState(rawToken) {};

	void
	Lexer::
	FirstPassHexadecimalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isHexDigitChar(inputChar)) {
			accept(inputChar);
		} else if (Lexer::isIntegerSuffixChar(inputChar)) {
			if (hasIntegerSuffix) {
				throw "Already has integer suffix";
			} else if (rawToken.rawValue.at(inputChar) == Underscore) {
				throw "Underscore not allowed before integer suffix";
			} else {
				accept(inputChar);
				hasIntegerSuffix = true;
			}
		} else if (hasIntegerSuffix && Lexer::isDigitChar(inputChar)) {
			accept(inputChar);
		} else if (inputChar == Dot) {
			/* switch to floating/fixed-point number or operator */
		} else if (Lexer::isFpHexSuffixChar(inputChar)) {
			/* switch to floating/fixed-point number or operator */
		} else if (inputChar == Underscore) {
			if (rawToken.rawValue.back() == Underscore) {
				throw "Only one underscore is allowed between digits";
			} else if (hasIntegerSuffix) {
				throw "Underscores not allowed in integer suffix";
			} else {
				accept(inputChar);
			}
		} else {
			if (rawToken.rawValue.back() != Underscore) {
				rawToken.item = RawLexicalItemIntegerLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);
				machine.handle(inputChar);
			} else {
				throw "Number literal cannot end with an underscore";
			}
		}
	}
}
