#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDecimalNumberState::FirstPassDecimalNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDecimalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (Lexer::isDigitChar(lastChar)) {
			if (Lexer::isDigitChar(inputChar)) {
				/* ### */
				accept(inputChar);

			} else if (inputChar == Underscore) {
				/* ###_ */
				accept(inputChar);

			} else if (Lexer::isIntegerSuffixChar(inputChar)) {
				/* ###Z */
				accept(inputChar);
				machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

			} else if (inputChar == Letter_r) {
				/* ###r */
				accept(inputChar);

			} else if (inputChar == Letter_i) {
				/* ###i */
				accept(inputChar);
				rawToken.item = RawLexicalItemComplexImaginaryLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Dot) {
				/* ###. */
				/* the ffp state might need to back off and cut the dot, if this was a mistake */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else if (Lexer::isFpSuffixChar(inputChar)) {
				/* ###e */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else {
				/* ### */
				rawToken.item = RawLexicalItemIntegerLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);
				machine.handle(inputChar);
			}

		} else if (lastChar == Underscore) {
			if (Lexer::isDigitChar(inputChar)) {
				/* ###_# */
				accept(inputChar);

			} else {
				throw "Unexpected input character";
			}

		} else if (lastChar == Letter_r) {
			if (inputChar == Letter_i) {
				/* ###ri */
				accept(inputChar);

			} else {
				/* ###r */
				rawToken.item = RawLexicalItemRationalDenominatorLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);
				machine.handle(inputChar);
			}

		} else {
			/* only digit chars and underscore may appear in this state */
			throw "Illegal state";
		}
	}
}
