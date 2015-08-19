#include "../lexer.hpp"

namespace GVM {

	Lexer::
	FirstPassQuotedIdentifierState::FirstPassQuotedIdentifierState()
		:	identifierType(QuotedIdentifierTypeUndecided) {};

	void
	Lexer::
	FirstPassQuotedIdentifierState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				/* just leading ` */
				accept(inputChar);
				break;
			case 1:
				/* decide state */
				if (Lexer::isGearIdentifierStart(inputChar)) {
					identifierType = QuotedIdentifierTypeNamed;
					accept(inputChar);
				} else if (inputChar == Backtick) {
					identifierType = QuotedIdentifierTypeDoubleQuoted;
					accept(inputChar);
				} else if (Lexer::isGearOperatorChar(inputChar)) {
					identifierType = QuotedIdentifierTypeOperator;
					accept(inputChar);
				} else {
					throw "Unexpected input character";
				}
				break;
			default: {
				switch (identifierType) {
					case QuotedIdentifierTypeNamed: {
						const size_t lastIndex = rawToken.rawValue.size() - 1;
						if (rawToken.rawValue.at(lastIndex) == Backtick) {
							/* finished quoted identifier: `id` */
							rawToken.item = RawLexicalItemIdentifier;
							machine.appendToOutput(rawToken);
							machine.changeState(new FirstPassStartState);
							machine.handle(inputChar);
						} else if (inputChar == Backtick) {
							/* the next accept will finish the quoted identifier */
							accept(inputChar);
						} else if ((   Lexer::isGearIdentifierPart(inputChar)
								        || Lexer::isGearIdentifierEnd(inputChar))
								&& !Lexer::isGearIdentifierEnd(rawToken.rawValue.at(lastIndex))) {
							accept(inputChar);
						} else if (Lexer::isGearIdentifierRepeatableEnd(rawToken.rawValue.at(lastIndex))
								&& Lexer::isGearIdentifierRepeatableEnd(inputChar)) {
							accept(inputChar);
						} else {
							/* the special case when identifier starts with backtick, 
							   but is not ended with one: `Tag_Name */
							rawToken.item = RawLexicalItemPolymorphicTagName;
							machine.appendToOutput(rawToken);
							machine.changeState(new FirstPassStartState);
							machine.handle(inputChar);
						}
						break;
					}

					case QuotedIdentifierTypeOperator:
						if (Lexer::isGearOperatorChar(inputChar)) {
							accept(inputChar);
						} else if (inputChar == Backtick) {
							accept(inputChar);
							rawToken.item = RawLexicalItemIdentifier;
							/* TODO: add to flags that this is an operator identifier, if found useful */
							machine.appendToOutput(rawToken);
							machine.changeState(new FirstPassStartState);
						} else {
							throw "Invalid input character at quoted operator identifier";
						}
						break;

					case QuotedIdentifierTypeDoubleQuoted: {
						const size_t lastIndex = rawToken.rawValue.size() - 1;
						const size_t penultimateIndex = rawToken.rawValue.size() - 2;
						if (   rawToken.rawValue.size() >= 5
								&& rawToken.rawValue.at(penultimateIndex) == Backtick
								&& rawToken.rawValue.at(lastIndex) == Backtick) {
							rawToken.item = RawLexicalItemIdentifier;
							machine.appendToOutput(rawToken);
							machine.changeState(new FirstPassStartState);
							machine.handle(inputChar);
						} else {
							/* TODO: if GFR-0 would restrict further the possible Unicode character classes here,
								 update this code to reflect that */
							accept(inputChar);
						}
						break;
					}

					case QuotedIdentifierTypeUndecided:
						throw "Illegal state, the state should be already decided at this consumed length";
						break;
				}
				break;
			}
		}
	}
}
