#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassOperatorOrLiteralState::FirstPassOperatorOrLiteralState() {};

	void
	Lexer::
	FirstPassOperatorOrLiteralState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (rawToken.rawValue.size() == 0) {
			/* leading `%' */
			accept(inputChar);

		} else if (rawToken.rawValue.size() == 1) {
			if (inputChar == Solidus) {
				/* regexp */
				accept(inputChar);
				rawToken.item = RawLexicalItemRegexpLiteral;
				machine.changeState(new Lexer::FirstPassRegexpState(rawToken, inputChar));

			} else if (inputChar == Letter_r) {
				/* regexp or raw string: %r */
				accept(inputChar);

			} else if (inputChar == Letter_m) {
				/* mutable string or mutable collection: %m */
				accept(inputChar);

			} else if (inputChar == Letter_i) {
				/* immutable string or immutable collection: %i */
				accept(inputChar);

			} else if (inputChar == Letter_l) {
				/* linked collection: %l */
				accept(inputChar);

			} else if (inputChar == Letter_q) {
				/* quoted string literal: %q */
				accept(inputChar);
				machine.changeState(new FirstPassQuotedStringState(rawToken));

			} else if (inputChar == Letter_s) {
				/* set collection or regexp substitution: %s */
				accept(inputChar);

			} else if (inputChar == Letter_w) {
				/* words list or array: %w */
				accept(inputChar);
				machine.changeState(new FirstPassWordsListOrArrayState(rawToken));

			} else if (inputChar == Apostrophe) {
				/* character literal: %' */
				accept(inputChar);
				machine.changeState(new FirstPassCharacterState(rawToken));

			} else if (inputChar == QuotationMark) {
				/* string literal without modifiers: %" */
				accept(inputChar);
				machine.changeState(new FirstPassStringState(rawToken));

			} else if (inputChar == PercentSign) {
				/* splice operator: %% */
				accept(inputChar);
				rawToken.item = RawLexicalItemOperatorOrDelimiter;
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == LeftParens) {
				/* modifier-less bag start: %( */
				accept(inputChar);
				rawToken.item = RawLexicalItemBagLiteralStart;
				machine.pushParenthesesElement(ParenthesesElementBag);
				machine.incrementParenthesisCounter();
				machine.appendToOutput(rawToken);
				machine.changeState(new FirstPassStartState);

			} else if (inputChar == LeftSquareBracket) {
				/* modifier-less list or array start: %[ */
				accept(inputChar);
				machine.changeState(new FirstPassListOrArrayState(rawToken));

			} else if (inputChar == LeftBrace) {
				/* modifier-less dictionary or multimap start: %{ */
				accept(inputChar);
				machine.changeState(new FirstPassDictionaryOrMultimapState(rawToken));

			} else if (inputChar >= Digit_0 && inputChar <= Digit_9) {
				/* anynymous function parameter placeholder: %1...%9 */
				accept(inputChar);
				machine.changeState(new FirstPassParameterPlaceholderState(rawToken));

			} else {
				throw "Unexpected input character";
			}
		} else if (rawToken.rawValue.size() == 2) {
			switch (inputChar) {
				case Letter_i:
					/* immutable raw string literal: %ri */
					if (rawToken.rawValue.at(1) == Letter_r) {
						accept(inputChar);
					} else {
						throw "Unexpected input character";
					}
					break;
				case Letter_m:
					/* mutable raw string literal: %rm */
					if (rawToken.rawValue.at(1) == Letter_r) {
						accept(inputChar);
					} else {
						throw "Unexpected input character";
					}
					break;
				case Letter_l:
					/* linked collection: %il, %ml */
					if (   rawToken.rawValue.at(1) == Letter_i
							|| rawToken.rawValue.at(1) == Letter_m) {
						accept(inputChar);
					} else {
						throw "Unexpected collection literal modifier";
					}
					break;
				case Letter_r:
					/* substitution regexp literal: %sr */
					if (rawToken.rawValue.at(1) == Letter_s) {
						accept(inputChar);
					} else {
						throw "Unexpected input character";
					}
					break;
				case Letter_s:
					/* set with modifier: %is, %ms, %ls */
					if (   rawToken.rawValue.at(1) == Letter_i
							|| rawToken.rawValue.at(1) == Letter_m
							|| rawToken.rawValue.at(1) == Letter_l) {
						accept(inputChar);
					} else {
						throw "Unexpected collection literal modifier";
					}
					break;
				case Letter_x:
					if (rawToken.rawValue.at(1) == Letter_r) {
						/* regexp literal: %rx */
						accept(inputChar);
					} else {
						throw "Unexpected input character";
					}
					break;
				case LeftParens:
					if (   rawToken.rawValue.at(1) == Letter_i
							|| rawToken.rawValue.at(1) == Letter_m
							|| rawToken.rawValue.at(1) == Letter_s) {
						/* bag/set literal with some modifier: %i(, %m(, %s( */
						accept(inputChar);
						rawToken.item = RawLexicalItemBagLiteralStart;
						machine.pushParenthesesElement(ParenthesesElementBag);
						machine.incrementParenthesisCounter();
						machine.appendToOutput(rawToken);
						machine.changeState(new FirstPassStartState);
					} else {
						throw "Unexpected input character";
					}
					break;
				case LeftSquareBracket:
					if (   rawToken.rawValue.at(1) == Letter_i
							|| rawToken.rawValue.at(1) == Letter_m) {
						/* list/array literal with some modifier: %i[, %m[ */
						accept(inputChar);
						machine.changeState(new FirstPassListOrArrayState(rawToken));
					} else {
						throw "Unexpected input character";
					}
					break;
				case LeftBrace:
					if (rawToken.rawValue.at(1) == Letter_i
							|| rawToken.rawValue.at(1) == Letter_m) {
						/* dictionary/multimap literal with some modifier: %i{, %m{ */
						accept(inputChar);
						machine.changeState(new FirstPassDictionaryOrMultimapState(rawToken));
					} else {
						throw "Unexpected input character";
					}
					break;
				case Solidus:
					if (rawToken.rawValue.at(1) == Letter_s) {
						/* substitution regexp: %s/ */
						accept(inputChar);
						machine.changeState(new FirstPassSubstitutionRegexpState(rawToken, inputChar));
					} else {
						throw "Unexpected input character";
					}
					break;
				case QuotationMark:
					if (   rawToken.rawValue.at(1) == Letter_i
							|| rawToken.rawValue.at(1) == Letter_m) {
						/* string literal with modifier: %i", %m" */
						accept(inputChar);
						machine.changeState(new FirstPassStringState(rawToken));
					} else {
						throw "Unexpected string modifier";
					}
					break;
				default:
					throw "Unexpected input character at collection literal";
			}
		} else if (rawToken.rawValue.size() == 3) {
			/* not many literal forms can start with this many characters */
			switch (inputChar) {
				case Letter_s:
					if (   (   rawToken.rawValue.at(1) == Letter_i
							    || rawToken.rawValue.at(1) == Letter_m)
							&& rawToken.rawValue.at(2) == Letter_l) {
						/* bag literal with modifiers: %ils, %mls */
						accept(inputChar);
					} else {
						throw "Unexpected input character";
					}
					break;
				case QuotationMark:
					/* immutable raw string literal: %ri", %rm" */
					if (rawToken.rawValue.at(1) == Letter_r
							&& (   rawToken.rawValue.at(2) == Letter_i
									|| rawToken.rawValue.at(2) == Letter_m)) {
						accept(inputChar);
						machine.changeState(new FirstPassRawStringState(rawToken));
					} else {
						throw "Unexpected input character";
					}
					break;
				default:
					break;
			}
		}
	}

}
