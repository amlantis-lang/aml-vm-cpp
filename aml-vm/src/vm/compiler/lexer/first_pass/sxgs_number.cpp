#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassSexagesimalNumberState::FirstPassSexagesimalNumberState(RawLexicalToken rawToken)
		: FirstPassNumberState(rawToken) {};

	void
	Lexer::
	FirstPassSexagesimalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isSxgsDigitChar(inputChar)) {
			/* sexagesimal numbers are groups of 1-2 digits in range 0-59, therefore the first digit
			   can only be 1 to 5, if one more is to come; however, we allow two zeros to appear 
			   successively, because that is the only way to indent sexagesimal numbers, where underscores
			   are not allowed to separate digits */
			if (Lexer::isSxgsDigitChar(rawToken.rawValue.back())
					&& rawToken.rawValue.back() >= Digit_0
					&& rawToken.rawValue.back() <= Digit_5) {
				accept(inputChar);
			} else if (rawToken.rawValue.back() == Comma) {
				accept(inputChar);
			} else if (rawToken.rawValue.size() == 2) {
				accept(inputChar);
			} else {
				throw "Unexpected sexagesimal digit";
			}
		} else if (Lexer::isIntegerSuffixChar(inputChar)) {
			if (hasIntegerSuffix) {
				throw "Already has integer suffix";
			} else if (rawToken.rawValue.at(inputChar) == Comma) {
				throw "Comma not allowed before integer suffix";
			} else {
				accept(inputChar);
				hasIntegerSuffix = true;
			}
		} else if (hasIntegerSuffix && Lexer::isDigitChar(inputChar)) {
			accept(inputChar);
		} else if (inputChar == Semicolon) {
			/* switch to floating/fixed-point number or operator */
		} else if (Lexer::isFpSuffixChar(inputChar)) {
			/* switch to floating/fixed-point number or operator */
		} else if (inputChar == Comma) {
			if (rawToken.rawValue.back() != Comma
					&& rawToken.rawValue.back() != Letter_s) {
				accept(inputChar);
			} else {
				throw "Unexpected comma";
			}
		} else {
			rawToken.item = RawLexicalItemIntegerLiteral;
			if (rawToken.rawValue.back() == Comma) {
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
