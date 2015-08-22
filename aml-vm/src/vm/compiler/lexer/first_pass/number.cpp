#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassNumberState::FirstPassNumberState()
		:	hasIntegerSuffix(false) {};

	Lexer::
	FirstPassNumberState::FirstPassNumberState(RawLexicalToken rawToken)
		:	hasIntegerSuffix(false),
			FirstPassState(rawToken)
	{
		if (rawToken.rawValue.size() >= 2
				&& Lexer::isIntegerSuffixChar(rawToken.rawValue.at(1))) {
			hasIntegerSuffix = true; /* only happens with decimal integers */
		}
	};

	void
	Lexer::
	FirstPassNumberState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				accept(inputChar);
				break;

			case 1: {
				if (Lexer::isDigitChar(inputChar)
						|| inputChar == Underscore
						|| Lexer::isIntegerSuffixChar(inputChar)) {
					accept(inputChar);
					machine.changeState(new FirstPassDecNumberState(rawToken));
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
						machine.changeState(new FirstPassHexNumberState(rawToken));
					} else if (inputChar == Letter_o) {
						accept(inputChar);
						machine.changeState(new FirstPassOctNumberState(rawToken));
					} else if (inputChar == Letter_b) {
						accept(inputChar);
						machine.changeState(new FirstPassBinNumberState(rawToken));
					} else if (inputChar == Letter_s) {
						accept(inputChar);
						machine.changeState(new FirstPassSxgsNumberState(rawToken));
					} else if (inputChar == Letter_d) {
						accept(inputChar);
						machine.changeState(new FirstPassDdecNumberState(rawToken));
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
