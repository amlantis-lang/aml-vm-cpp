#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassSexagesimalNumberState::FirstPassSexagesimalNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassSexagesimalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (Lexer::isSxgsDigitChar(inputChar)) {
			/* sexagesimal numbers are groups of 1-2 digits in range 0-59, therefore the first digit
			   can only be 1 to 5, if one more is to come; however, we allow two zeros to appear 
			   successively, because that is the only way to indent sexagesimal numbers, where underscores
			   are not allowed to separate digits */
			if (Lexer::isSxgsDigitChar(lastChar)
					&& lastChar >= Digit_0
					&& lastChar <= Digit_5) {
				accept(inputChar);

			} else if (lastChar == Comma) {
				accept(inputChar);

			} else if (lastChar == Letter_s) {
				accept(inputChar);

			} else {
				throw "Unexpected sexagesimal digit";
			}
		} else if (Lexer::isIntegerSuffixChar(inputChar)) {
			accept(inputChar);
			machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

		} else if (inputChar == Semicolon) {
			/* switch to floating/fixed-point number or operator */

		} else if (Lexer::isFpSuffixChar(inputChar)) {
			/* switch to floating/fixed-point number or operator */

		} else if (inputChar == Comma) {
			if (Lexer::isSxgsDigitChar(lastChar)) {
				accept(inputChar);
			} else {
				throw "Unexpected comma";
			}
		} else {
			rawToken.item = RawLexicalItemIntegerLiteral;
			if (lastChar == Comma) {
				rawToken.rawValue.pop_back();
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassCommaState);
				machine.handle(Comma);
			} else {
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);
			}
			machine.handle(inputChar);
		}
	}
}
