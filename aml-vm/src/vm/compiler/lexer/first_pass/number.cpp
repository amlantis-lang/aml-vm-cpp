#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassNumberState::FirstPassNumberState() {};

	Lexer::
	FirstPassNumberState::FirstPassNumberState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassNumberState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				accept(inputChar);
				break;

			case 1: {
				if (Lexer::isDigitChar(inputChar)
						|| inputChar == Underscore) {
					accept(inputChar);
					machine.changeState(new FirstPassDecimalNumberState(rawToken));

				} else if (Lexer::isIntegerSuffixChar(inputChar)) {
					accept(inputChar);
					machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

				} else if (inputChar == Dot) {
					/* switch to floating/fixed-point number or operator */

				} else if (inputChar == Letter_r) {
					/* 0r etc. */
					accept(inputChar);
					rawToken.item = RawLexicalItemRationalDenominatorLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);

				} else if (inputChar == Letter_i) {
					/* 0i etc. */
					accept(inputChar);
					rawToken.item = RawLexicalItemComplexImaginaryLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);

				} else if (rawToken.rawValue.at(0) == Digit_0) {
					if (inputChar == Letter_x) {
						accept(inputChar);
						machine.changeState(new FirstPassHexadecimalNumberState(rawToken));

					} else if (inputChar == Letter_o) {
						accept(inputChar);
						machine.changeState(new FirstPassOctalNumberState(rawToken));

					} else if (inputChar == Letter_b) {
						accept(inputChar);
						machine.changeState(new FirstPassBinaryNumberState(rawToken));

					} else if (inputChar == Letter_s) {
						accept(inputChar);
						machine.changeState(new FirstPassSexagesimalNumberState(rawToken));

					} else if (inputChar == Letter_d) {
						accept(inputChar);
						machine.changeState(new FirstPassDuodecimalNumberState(rawToken));

					} else {
						rawToken.item = RawLexicalItemIntegerLiteral;
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);
						machine.handle(inputChar);
					}

				} else {
					rawToken.item = RawLexicalItemIntegerLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				}
				break;
			}

			default:
				throw "Illegal state";
		}
	}
}
