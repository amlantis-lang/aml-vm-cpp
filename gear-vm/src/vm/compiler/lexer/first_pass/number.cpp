#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassNumberState::FirstPassNumberState() {};

	void
	Lexer::
	FirstPassNumberState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				accept(inputChar);
				break;

			case 1: {
				if (Lexer::isDigitChar(inputChar)) {
					accept(inputChar);
					machine.changeState(new FirstPassDecNumberState(rawToken));
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
						throw "Unexpected input character";
					}
				} else if (Lexer::isLetterChar(inputChar)) {
					throw "Unexpected input character";
				} else if (Lexer::isGearDelimiterChar(inputChar)
						|| Lexer::isGearOperatorChar(inputChar)
						|| u_isblank_55(inputChar)) {
					rawToken.item = RawLexicalItemIntegerLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				} else {
					throw "Unexpected input character";
				}
				break;
			}

			default:
				throw "Illegal state";
		}
	}
}
