#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassOctalNumberState::FirstPassOctalNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken) {};

	void
	Lexer::
	FirstPassOctalNumberState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (Lexer::isOctDigitChar(lastChar)) {
			if (Lexer::isOctDigitChar(inputChar)) {
				/* 0o### */
				accept(inputChar);

			} else if (inputChar == Underscore) {
				/* 0o###_ */
				accept(inputChar);

			} else if (Lexer::isIntegerSuffixChar(inputChar)) {
				/* 0o###Z */
				accept(inputChar);
				machine.changeState(new FirstPassIntegerSuffixState(rawToken, RawLexicalItemIntegerLiteral));

			} else if (inputChar == Letter_r) {
				/* 0o###r */
				accept(inputChar);
				rawToken.item = RawLexicalItemRationalDenominatorLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Letter_i) {
				/* 0o###i */
				accept(inputChar);
				rawToken.item = RawLexicalItemComplexImaginaryLiteral;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == Dot) {
				/* 0o###. */
				/* the ffp state might need to back off and cut the dot, if this was a mistake */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else if (Lexer::isFpSuffixChar(inputChar)) {
				/* 0o###e */
				accept(inputChar);
				machine.changeState(new FirstPassDecimalFloatingOrFixedPointNumberState(rawToken));

			} else {
				if (rawToken.rawValue.size() > 2) {
					/* 0o### */
					rawToken.item = RawLexicalItemIntegerLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);

				} else {
					throw "Unexpected end of octal number literal";
				}
			}

		} else if (lastChar == Underscore || lastChar == Letter_o) {
			if (Lexer::isOctDigitChar(inputChar)) {
				/* 0o###_# */
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
