#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassCharacterState::FirstPassCharacterState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken),
 			doingEscapeSequence(false),
 			completedEscapeSequence(false),
 			escapeExtendedType(ExtendedCharacterEscapeTypeInvalid) {};

	void
	Lexer::
	FirstPassCharacterState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (rawToken.rawValue.size() == 2) {
			/* contains: %' */
			if (inputChar == ReverseSolidus) {
				/* escape sequence */
				accept(inputChar);
				doingEscapeSequence = true;

			} else if (inputChar != Apostrophe) {
				accept(inputChar);

			} else {
				/* TODO: change to bad raw token with explanation */
				throw "Unexpected input character in character literal";
			}
		} else if (rawToken.rawValue.size() == 3) {
			/* contains: %' + reverse solidus or other character */
			if (inputChar == Apostrophe) {
				if (doingEscapeSequence) {
					/* escaped apostrophe, thus not yet finished */
					accept(inputChar);
					completedEscapeSequence = true;

				} else {
					/* finished character literal */
					accept(inputChar);
					rawToken.item = RawLexicalItemCharacterLiteral;
					machine.appendToOutput(rawToken);
					machine.changeState(new Lexer::FirstPassStartState);
				}
			} else if (doingEscapeSequence) {
				switch (inputChar) {
					case QuotationMark:
					case QuestionMark:
					case ReverseSolidus:
					case HashSign:
					case DollarSign:
					case Digit_0: /* NUL */
					case Letter_a: /* BEL */
					case Letter_b: /* Backspace */
					case Letter_f: /* Form Feed, FF */
					case Letter_n: /* Line Feed, LF */
					case Letter_r: /* Carriage Return, CR */
					case Letter_t: /* Horizontal TAB */
					case Letter_v: /* Vertical TAB */
						accept(inputChar);
						completedEscapeSequence = true;
						break;

					case Letter_x: /* Begin hex escape */
						accept(inputChar);
						escapeExtendedType = ExtendedCharacterEscapeTypeHex;
						break;

					case Letter_u: /* Begin 4-hex-digit Unicode escape */
						accept(inputChar);
						escapeExtendedType = ExtendedCharacterEscapeTypeUnicode4;
						break;

					case Letter_U: /* Begin 8-hex-digit Unicode escape */
						accept(inputChar);
						escapeExtendedType = ExtendedCharacterEscapeTypeUnicode8;
						break;

					case LeftBrace: /* Begin named Unicode escape */
						accept(inputChar);
						escapeExtendedType = ExtendedCharacterEscapeTypeUnicodeNamed;
						break;

					case Ampersand: /* Begin named entity escape */
						accept(inputChar);
						escapeExtendedType = ExtendedCharacterEscapeTypeEntity;
						break;

					default:
						/* TODO: replace with bad token */
						throw "Unexpected input character in character literal";
				}
			} else {
				/* TODO: handle extended grapheme cluster, if that is the case */
				throw "Unexpected input character in character literal";
			}
		} else {
			switch (rawToken.rawValue.size()) {
				case 4:
				case 5:
					switch (escapeExtendedType) {
						case ExtendedCharacterEscapeTypeHex:
						case ExtendedCharacterEscapeTypeUnicode4:
						case ExtendedCharacterEscapeTypeUnicode8:
							if (u_hasBinaryProperty_55(inputChar, UCHAR_ASCII_HEX_DIGIT)) {
								accept(inputChar);
								if (rawToken.rawValue.size() == 5
										&& escapeExtendedType == ExtendedCharacterEscapeTypeHex) {
									completedEscapeSequence = true;
								}

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeEntity:
							if (Lexer::isUnicodeNamePart(inputChar)) {
								accept(inputChar);

							} else if (inputChar == Semicolon) {
								accept(inputChar);
								completedEscapeSequence = true;
								escapeExtendedType = ExtendedCharacterEscapeTypeInvalid;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeUnicodeNamed:
							if (Lexer::isUnicodeNamePart(inputChar) || inputChar == Space) {
								accept(inputChar);

							} else if (inputChar == RightBrace) {
								accept(inputChar);
								completedEscapeSequence = true;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeInvalid:
							if (completedEscapeSequence && inputChar == Apostrophe) {
								accept(inputChar);
								rawToken.item = RawLexicalItemCharacterLiteral;
								machine.appendToOutput(rawToken);
								machine.changeState(new Lexer::FirstPassStartState);
							} else {
								throw "Unexpected input character in character literal";
							}
					}
					break;

				case 6:
				case 7:
					switch (escapeExtendedType) {
						case ExtendedCharacterEscapeTypeUnicode4:
						case ExtendedCharacterEscapeTypeUnicode8:
							if (u_hasBinaryProperty_55(inputChar, UCHAR_ASCII_HEX_DIGIT)) {
								accept(inputChar);
								if (rawToken.rawValue.size() == 7
										&& escapeExtendedType == ExtendedCharacterEscapeTypeUnicode4) {
									completedEscapeSequence = true;
								}
							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeEntity:
							if (Lexer::isUnicodeNamePart(inputChar)) {
								accept(inputChar);

							} else if (inputChar == Semicolon) {
								accept(inputChar);
								completedEscapeSequence = true;
								escapeExtendedType = ExtendedCharacterEscapeTypeInvalid;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeUnicodeNamed:
							if (Lexer::isUnicodeNamePart(inputChar) || inputChar == Space) {
								accept(inputChar);

							} else if (inputChar == RightBrace) {
								accept(inputChar);
								completedEscapeSequence = true;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeHex:
						case ExtendedCharacterEscapeTypeInvalid:
							if (completedEscapeSequence && inputChar == Apostrophe) {
								accept(inputChar);
								rawToken.item = RawLexicalItemCharacterLiteral;
								machine.appendToOutput(rawToken);
								machine.changeState(new Lexer::FirstPassStartState);
							} else {
								throw "Unexpected input character in character literal";
							}
					}
					break;

				case 8:
				case 9:
				case 10:
				case 11:
					switch (escapeExtendedType) {
						case ExtendedCharacterEscapeTypeUnicode8:
							if (u_hasBinaryProperty_55(inputChar, UCHAR_ASCII_HEX_DIGIT)) {
								accept(inputChar);
								if (rawToken.rawValue.size() == 11) {
									completedEscapeSequence = true;
								}
							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeEntity:
							if (Lexer::isUnicodeNamePart(inputChar)) {
								accept(inputChar);

							} else if (inputChar == Semicolon) {
								accept(inputChar);
								completedEscapeSequence = true;
								escapeExtendedType = ExtendedCharacterEscapeTypeInvalid;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeUnicodeNamed:
							if (Lexer::isUnicodeNamePart(inputChar) || inputChar == Space) {
								accept(inputChar);

							} else if (inputChar == RightBrace) {
								accept(inputChar);
								completedEscapeSequence = true;
								escapeExtendedType = ExtendedCharacterEscapeTypeInvalid;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeHex:
						case ExtendedCharacterEscapeTypeUnicode4:
						case ExtendedCharacterEscapeTypeInvalid:
							if (completedEscapeSequence && inputChar == Apostrophe) {
								accept(inputChar);
								rawToken.item = RawLexicalItemCharacterLiteral;
								machine.appendToOutput(rawToken);
								machine.changeState(new Lexer::FirstPassStartState);
							} else {
								throw "Unexpected input character in character literal";
							}
							break;
					}
					break;

				default:
					switch (escapeExtendedType) {
						case ExtendedCharacterEscapeTypeEntity:
							if (Lexer::isUnicodeNamePart(inputChar)) {
								accept(inputChar);

							} else if (inputChar == Semicolon) {
								accept(inputChar);
								completedEscapeSequence = true;
								escapeExtendedType = ExtendedCharacterEscapeTypeInvalid;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeUnicodeNamed:
							if (Lexer::isUnicodeNamePart(inputChar) || inputChar == Space) {
								accept(inputChar);

							} else if (inputChar == RightBrace) {
								accept(inputChar);
								completedEscapeSequence = true;
								escapeExtendedType = ExtendedCharacterEscapeTypeInvalid;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						default:
							if (completedEscapeSequence && inputChar == Apostrophe) {
								accept(inputChar);
								rawToken.item = RawLexicalItemCharacterLiteral;
								machine.appendToOutput(rawToken);
								machine.changeState(new Lexer::FirstPassStartState);
							} else {
								throw "Unexpected input character in character literal";
							}
					}
			}
		}
	}

}
