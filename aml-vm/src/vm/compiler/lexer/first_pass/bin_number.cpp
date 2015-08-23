#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassBinaryNumberState::FirstPassBinaryNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassBinaryNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (Lexer::isBinDigitChar(lastChar)) {
			if (Lexer::isBinDigitChar(inputChar)) {
				/* 0b### */
				accept(inputChar);

			} else if (inputChar == Underscore) {
				/* 0b###_ */
				accept(inputChar);

			} else if (Lexer::isIntegerSuffixChar(inputChar)) {
				/* 0b###Z */
				accept(inputChar);
				machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

			} else if (inputChar == Letter_r) {
				/* 0b###r */
				accept(inputChar);
				rawToken.item = RawLexicalItemRationalDenominatorLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Letter_i) {
				/* 0b###i */
				accept(inputChar);
				rawToken.item = RawLexicalItemComplexImaginaryLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Dot) {
				/* 0b###. */
				/* the ffp state might need to back off and cut the dot, if this was a mistake */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else if (Lexer::isFpSuffixChar(inputChar)) {
				/* 0b###e */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else {
				if (rawToken.rawValue.size() > 2) {
					/* 0b### */
					rawToken.item = RawLexicalItemIntegerLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);

				} else {
					throw "Unexpected end of binary number literal";
				}
			}

		} else if (lastChar == Underscore || lastChar == Letter_b) {
			if (Lexer::isBinDigitChar(inputChar)) {
				/* 0b###_# */
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
