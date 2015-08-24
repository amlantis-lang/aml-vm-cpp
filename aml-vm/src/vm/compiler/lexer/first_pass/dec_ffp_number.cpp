#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassDecimalFloatingOrFixedPointNumberState::
	FirstPassDecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken)
		: FirstPassState(rawToken)
	{
		UChar32 const &lastChar = rawToken.rawValue.back();
		if (lastChar == Dot) {
			flavour = FloatingOrFixedPointFlavourDot;
		} else if (lastChar == Letter_e) {
			flavour = FloatingOrFixedPointFlavourExponentPart;
		} else {
			flavour = FloatingOrFixedPointFlavourTypeSuffix;
		}
	};

	void
	Lexer::
	FirstPassDecimalFloatingOrFixedPointNumberState::handle(
			AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		UChar32 const &lastChar = rawToken.rawValue.back();
		switch (flavour) {
			case FloatingOrFixedPointFlavourDot:
				if (Lexer::isDigitChar(lastChar)) {
					if (Lexer::isDigitChar(inputChar)) {
						/* ###.### */
						accept(inputChar);

					} else if (inputChar == Underscore) {
						/* ###.###_ */
						accept(inputChar);

					} else if (inputChar == Letter_r) {
						/* ###.###r */
						accept(inputChar);

					} else if (inputChar == Letter_i) {
						/* ###.###i */
						accept(inputChar);
						rawToken.item = RawLexicalItemComplexImaginaryLiteral;
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);

					} else if (inputChar == Letter_e) {
						/* ###.###e */
						accept(inputChar);
						flavour = FloatingOrFixedPointFlavourExponentPart;

					} else if (Lexer::isFpTypeSuffixChar(inputChar)) {
						/* ###.###f */
						accept(inputChar);
						flavour = FloatingOrFixedPointFlavourTypeSuffix;

					} else {
						/* ###.### + something else */
						rawToken.item = RawLexicalItemRealLiteral;
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);
						machine.handle(inputChar);
					}
				} else if (lastChar == Underscore || lastChar == Dot) {
					if (Lexer::isDigitChar(inputChar)) {
						/* ###.###_ or ###.# */
						accept(inputChar);

					} else {
						throw "Unexpected input character";
					}

				} else {
					/* only digit chars, dot and underscore may appear in this state */
					throw "Illegal state";
				}
				break;

			case FloatingOrFixedPointFlavourExponentPart:
				if (Lexer::isDigitChar(lastChar)) {
					if (Lexer::isDigitChar(inputChar)) {
						/* ###e# */
						accept(inputChar);

					} else if (inputChar == Underscore) {
						/* ###e###_ */
						accept(inputChar);

					} else if (inputChar == Letter_r) {
						/* ###e###r */
						accept(inputChar);

					} else if (inputChar == Letter_i) {
						/* ###e###i */
						accept(inputChar);
						rawToken.item = RawLexicalItemComplexImaginaryLiteral;
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);

					} else if (Lexer::isFpTypeSuffixChar(inputChar)) {
						/* ###e###f */
						accept(inputChar);
						flavour = FloatingOrFixedPointFlavourTypeSuffix;

					} else {
						/* ###e### + something else */
						rawToken.item = RawLexicalItemRealLiteral;
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);
						machine.handle(inputChar);
					}

				} else if (lastChar == Underscore) {
					if (Lexer::isDigitChar(inputChar)) {
						/* ###e###_ */
						accept(inputChar);

					} else {
						throw "Unexpected input character";
					}

				} else if (lastChar == PlusSign || lastChar == HyphenMinusSign) {
					if (Lexer::isDigitChar(inputChar)) {
						/* ###e+### or ###e-### */
						accept(inputChar);

					} else {
						throw "Unexpected input character";
					}

				} else if (lastChar == Letter_r) {
					if (inputChar == Letter_i) {
						/* ###e###ri */
						accept(inputChar);

					} else {
						/* ###e###r */
						rawToken.item = RawLexicalItemRationalDenominatorLiteral;
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);
						machine.handle(inputChar);
					}

				} else {
					/* only digit chars, +, - and underscore may appear in this state */
					throw "Illegal state";
				}
				break;

			case FloatingOrFixedPointFlavourTypeSuffix:
				if (inputChar == Letter_r && lastChar != Letter_r) {
					/* ###fr */
					accept(inputChar);

				} else if (inputChar == Letter_i) {
					/* ###fi or ###ri */
					accept(inputChar);
					rawToken.item = RawLexicalItemComplexImaginaryLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);

				} else {
					/* ###f or ###r */
					rawToken.item = (lastChar == Letter_r)
						? RawLexicalItemRationalDenominatorLiteral
						: RawLexicalItemRealLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				}
				break;
		}
	}
}
