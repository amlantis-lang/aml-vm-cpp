#include "lexer.hpp"

namespace GVM {

	Lexer::Lexer() {};

	std::vector<struct RawLexicalToken> *
	Lexer::parseInputStream(class InputStream *inputStream) {
		auto rawTokens = new std::vector<struct RawLexicalToken>();

		FirstPassMachine machine(rawTokens);
		while (inputStream->moveToNextUChar32()) {
			UChar32 inputChar = inputStream->getCurrentUChar32();
			machine.handle(inputChar);
		}
		machine.handle(inputStream->getCurrentUChar32()); /* U_EOF */

		return rawTokens; // change to real tokens after second pass
	};

	Lexer::
	FirstPassState::FirstPassState() {};

	Lexer::
	FirstPassState::FirstPassState(RawLexicalToken rawToken)
		:	rawToken(rawToken) {}

	Lexer::
	FirstPassStartState::FirstPassStartState() {};

	Lexer::
	FirstPassMachine::FirstPassMachine(std::vector<RawLexicalToken> *output)
		:	state(new Lexer::FirstPassStartState),
 			output(output) {};

	void
	Lexer::
	FirstPassMachine::handle(UChar32 inputChar) {
		state->handle(*this, inputChar);
	}

	void
	Lexer::
	FirstPassMachine::changeState(GVM::Lexer::FirstPassState *newState) {
		state.reset(newState);
	}

	void
	Lexer::
	FirstPassMachine::appendToOutput(GVM::RawLexicalToken rawToken) {
		output->push_back(rawToken);
	}

	static
	bool
	isLineBreakStart(UChar32 inputChar) {
		return inputChar == LF
			|| inputChar == CR
			|| inputChar == VT
			|| inputChar == FF
			|| inputChar == NEL
			|| inputChar == LS
			|| inputChar == PS
		;
	}

	static
	bool
	isUnicodeNamePart(UChar32 inputChar) {
		return (inputChar >= Letter_A && inputChar <= Letter_Z)
			|| (inputChar >= Letter_a && inputChar <= Letter_z)
			|| (inputChar >= Digit_0 && inputChar <= Digit_9);
	}

	void
	Lexer::
	FirstPassStartState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		if (isLineBreakStart(inputChar)) {
			machine.changeState(new Lexer::FirstPassNewlineState);
			machine.handle(inputChar);
		} else if (inputChar == LeftParens) {
			machine.changeState(new Lexer::FirstPassLeftParenthesisState);
			machine.handle(inputChar);
		} else if (inputChar == RightParens) {
			machine.changeState(new Lexer::FirstPassRightParenthesisState);
			machine.handle(inputChar);
		} else if (inputChar == LeftSquareBracket) {
			machine.changeState(new Lexer::FirstPassLeftSquareBracketState);
			machine.handle(inputChar);
		} else if (inputChar == RightSquareBracket) {
			machine.changeState(new Lexer::FirstPassRightSquareBracketState);
			machine.handle(inputChar);
		} else if (inputChar == LeftBrace) {
			machine.changeState(new Lexer::FirstPassLeftBraceState);
			machine.handle(inputChar);
		} else if (inputChar == RightBrace) {
			machine.changeState(new Lexer::FirstPassRightBraceState);
			machine.handle(inputChar);
		} else if (inputChar == PercentSign) {
			machine.changeState(new Lexer::FirstPassOperatorOrLiteralState);
			machine.handle(inputChar);
		}
	};

	Lexer::
	FirstPassNewlineState::FirstPassNewlineState() {};

	Lexer::
	FirstPassWhitespaceState::FirstPassWhitespaceState() {};

	Lexer::
	FirstPassLeftParenthesisState::FirstPassLeftParenthesisState() {};

	Lexer::
	FirstPassRightParenthesisState::FirstPassRightParenthesisState() {};

	Lexer::
	FirstPassLeftSquareBracketState::FirstPassLeftSquareBracketState() {};

	Lexer::
	FirstPassRightSquareBracketState::FirstPassRightSquareBracketState() {};

	Lexer::
	FirstPassLeftBraceState::FirstPassLeftBraceState() {};

	Lexer::
	FirstPassRightBraceState::FirstPassRightBraceState() {};

	Lexer::
	FirstPassOperatorOrLiteralState::FirstPassOperatorOrLiteralState() {};

	Lexer::
	FirstPassCharacterLiteralState::FirstPassCharacterLiteralState(RawLexicalToken rawToken)
		: FirstPassState(rawToken),
 			doingEscapeSequence(false),
 			completedEscapeSequence(false),
 			escapeExtendedType(ExtendedCharacterEscapeTypeInvalid) {};

	Lexer::
	FirstPassRegexpLiteralState::FirstPassRegexpLiteralState(RawLexicalToken rawToken, UChar32 delimiter)
		:	FirstPassState(rawToken),
			delimiter(delimiter) {};

	void
	Lexer::
	FirstPassNewlineState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (rawToken.rawValue.length() == 0) {
			accept(inputChar);
			if (inputChar != LF && inputChar != CR) {
				/* Neither CR nor LF - these can be followed by LF or CR resp. */
				rawToken.item = RawLexicalItemNewline;
				machine.appendToOutput(rawToken);
				machine.changeState(new Lexer::FirstPassStartState);
			}

		} else if (rawToken.rawValue.length() == 1) {
			if (rawToken.rawValue.char32At(0) == CR && inputChar == LF) {
				/* CR+LF */
				accept(inputChar);
				rawToken.item = RawLexicalItemNewline;
				machine.appendToOutput(rawToken);
				machine.changeState(new Lexer::FirstPassStartState);

			} else if (rawToken.rawValue.char32At(0) == LF && inputChar == CR) {
				/* LF+CR */
				accept(inputChar);
				rawToken.item = RawLexicalItemNewline;
				machine.appendToOutput(rawToken);
				machine.changeState(new Lexer::FirstPassStartState);

			} else {
				/* Either LF or CR */
				rawToken.item = RawLexicalItemNewline;
				machine.appendToOutput(rawToken);
				machine.changeState(new Lexer::FirstPassStartState);
				machine.handle(inputChar);
			}

		} else {
			throw "Illegal 1st pass newline state";
		}
	}

	void
	Lexer::
	FirstPassWhitespaceState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (u_isblank_55(inputChar)) {
			accept(inputChar);

		} else {
			if (rawToken.rawValue.length() >= 1) {
				/* do not append empty whitespace, that would be utterly silly */
				rawToken.item = RawLexicalItemWhitespace;
				machine.appendToOutput(rawToken);
			}
			machine.changeState(new Lexer::FirstPassStartState);
			machine.handle(inputChar);
		}
	}

	void
	Lexer::
	FirstPassLeftParenthesisState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ( */
		accept(inputChar);
		rawToken.item = RawLexicalItemLeftParenthesis;
		machine.appendToOutput(rawToken);
		machine.changeState(new Lexer::FirstPassStartState);
	}

	void
	Lexer::
	FirstPassRightParenthesisState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ) */
		accept(inputChar);
		rawToken.item = RawLexicalItemRightParenthesis;
		machine.appendToOutput(rawToken);
		machine.changeState(new Lexer::FirstPassStartState);
	}

	void
	Lexer::
	FirstPassLeftSquareBracketState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* [ */
		accept(inputChar);
		rawToken.item = RawLexicalItemLeftSquareBracket;
		machine.appendToOutput(rawToken);
		machine.changeState(new Lexer::FirstPassStartState);
	}

	void
	Lexer::
	FirstPassRightSquareBracketState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* ] */
		accept(inputChar);
		rawToken.item = RawLexicalItemRightSquareBracket;
		machine.appendToOutput(rawToken);
		machine.changeState(new Lexer::FirstPassStartState);
	}

	void
	Lexer::
	FirstPassLeftBraceState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* { */
		accept(inputChar);
		rawToken.item = RawLexicalItemLeftBrace;
		machine.appendToOutput(rawToken);
		machine.changeState(new Lexer::FirstPassStartState);
	}

	void
	Lexer::
	FirstPassRightBraceState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		/* } */
		accept(inputChar);
		rawToken.item = RawLexicalItemRightBrace;
		machine.appendToOutput(rawToken);
		machine.changeState(new Lexer::FirstPassStartState);
	}

	void
	Lexer::
	FirstPassOperatorOrLiteralState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (rawToken.rawValue.length() == 0) {
			/* leading `%' */
			accept(inputChar);

		} else if (rawToken.rawValue.length() == 1) {
			if (inputChar == Solidus) {
				/* regexp */
				accept(inputChar);
				rawToken.item = RawLexicalItemRegexpLiteral;
				machine.changeState(new Lexer::FirstPassRegexpLiteralState(rawToken, inputChar));

			} else if (inputChar == Letter_r) {
				/* regexp or raw string */
				accept(inputChar);

			} else if (inputChar == Letter_m) {
				/* mutable string or mutable collection */
				accept(inputChar);

			} else if (inputChar == Letter_i) {
				/* immutable string or immutable collection */
				accept(inputChar);

			} else if (inputChar == Letter_l) {
				/* linked collection */
				accept(inputChar);

			} else if (inputChar == Letter_s) {
				/* set collection */
				accept(inputChar);

			} else if (inputChar == Apostrophe) {
				/* character literal */
				accept(inputChar);
				machine.changeState(new FirstPassCharacterLiteralState(rawToken));

			}
		}
	}

	void
	Lexer::
	FirstPassCharacterLiteralState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (rawToken.rawValue.length() == 2) {
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
		} else if (rawToken.rawValue.length() == 3) {
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
			switch (rawToken.rawValue.length()) {
				case 4:
				case 5:
					switch (escapeExtendedType) {
						case ExtendedCharacterEscapeTypeHex:
						case ExtendedCharacterEscapeTypeUnicode4:
						case ExtendedCharacterEscapeTypeUnicode8:
							if (u_hasBinaryProperty_55(inputChar, UCHAR_ASCII_HEX_DIGIT)) {
								accept(inputChar);
								if (rawToken.rawValue.length() == 5
										&& escapeExtendedType == ExtendedCharacterEscapeTypeHex) {
									completedEscapeSequence = true;
								}

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeEntity:
							if (isUnicodeNamePart(inputChar)) {
								accept(inputChar);

							} else if (inputChar == Semicolon) {
								accept(inputChar);
								completedEscapeSequence = true;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeUnicodeNamed:
							if (isUnicodeNamePart(inputChar) || inputChar == Space) {
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
								if (rawToken.rawValue.length() == 7
										&& escapeExtendedType == ExtendedCharacterEscapeTypeUnicode4) {
									completedEscapeSequence = true;
								}
							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeEntity:
							if (isUnicodeNamePart(inputChar)) {
								accept(inputChar);

							} else if (inputChar == Semicolon) {
								accept(inputChar);
								completedEscapeSequence = true;

							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeUnicodeNamed:
							if (isUnicodeNamePart(inputChar) || inputChar == Space) {
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
								if (rawToken.rawValue.length() == 11) {
									completedEscapeSequence = true;
								}
							} else {
								throw "Unexpected input character in character literal";
							}
							break;

						case ExtendedCharacterEscapeTypeEntity:
							if (isUnicodeNamePart(inputChar)) {
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
							if (isUnicodeNamePart(inputChar) || inputChar == Space) {
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
					}
					break;

				default:
					switch (escapeExtendedType) {
						case ExtendedCharacterEscapeTypeEntity:
							if (isUnicodeNamePart(inputChar)) {
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
							if (isUnicodeNamePart(inputChar) || inputChar == Space) {
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

	void
	Lexer::
	FirstPassRegexpLiteralState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {

	}
}

