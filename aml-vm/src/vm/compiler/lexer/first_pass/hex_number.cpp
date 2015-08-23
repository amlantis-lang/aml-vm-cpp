#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassHexadecimalNumberState::FirstPassHexadecimalNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassHexadecimalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (Lexer::isHexDigitChar(lastChar)) {
			if (Lexer::isHexDigitChar(inputChar)) {
				/* 0x### */
				accept(inputChar);

			} else if (inputChar == Underscore) {
				/* 0x###_ */
				accept(inputChar);

			} else if (Lexer::isIntegerSuffixChar(inputChar)) {
				/* 0x###Z */
				accept(inputChar);
				machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

			} else if (inputChar == Letter_r) {
				/* 0x###r */
				accept(inputChar);
				rawToken.item = RawLexicalItemRationalDenominatorLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Letter_i) {
				/* 0x###i */
				accept(inputChar);
				rawToken.item = RawLexicalItemComplexImaginaryLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Dot) {
				/* 0x###. */
				/* the ffp state might need to back off and cut the dot, if this was a mistake */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else if (Lexer::isFpHexSuffixChar(inputChar)) {
				/* 0x###p */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else {
				if (rawToken.rawValue.size() > 2) {
					/* 0x### */
					rawToken.item = RawLexicalItemIntegerLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);

				} else {
					throw "Unexpected end of hexadecimal number literal";
				}
			}

		} else if (lastChar == Underscore || lastChar == Letter_x) {
			if (Lexer::isHexDigitChar(inputChar)) {
				/* 0x###_# */
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
