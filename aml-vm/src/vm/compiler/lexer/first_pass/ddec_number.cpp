#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDuodecimalNumberState::FirstPassDuodecimalNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassDuodecimalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (Lexer::isDdecDigitChar(lastChar)) {
			if (Lexer::isDdecDigitChar(inputChar)) {
				/* 0d### */
				accept(inputChar);

			} else if (inputChar == Underscore) {
				/* 0d###_ */
				accept(inputChar);

			} else if (Lexer::isIntegerSuffixChar(inputChar)) {
				/* 0d###Z */
				accept(inputChar);
				machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

			} else if (inputChar == Letter_r) {
				/* 0d###r */
				accept(inputChar);
				rawToken.item = RawLexicalItemRationalDenominatorLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Letter_i) {
				/* 0d###i */
				accept(inputChar);
				rawToken.item = RawLexicalItemComplexImaginaryLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Dot) {
				/* 0d###. */
				/* the ffp state might need to back off and cut the dot, if this was a mistake */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else if (Lexer::isFpSuffixChar(inputChar)) {
				/* 0d###e */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else {
				if (rawToken.rawValue.size() > 2) {
					/* 0d### */
					rawToken.item = RawLexicalItemIntegerLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);

				} else {
					throw "Unexpected end of binary number literal";
				}
			}

		} else if (lastChar == Underscore || lastChar == Letter_d) {
			if (Lexer::isDdecDigitChar(inputChar)) {
				/* 0d###_# */
				accept(inputChar);

			} else {
				throw "Unexpected input character";
			}

		} else {
			/* only digit chars and underscores may appear in this state */
			throw "Illegal state";
		}
	}
}
