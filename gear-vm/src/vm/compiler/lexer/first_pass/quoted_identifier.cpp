#include "../lexer.hpp"

namespace GVM {

	Lexer::
	FirstPassQuotedIdentifierState::FirstPassQuotedIdentifierState() {};

	void
	Lexer::
	FirstPassQuotedIdentifierState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
    	case 0:
				/* correctness handled by the start state */
				accept(inputChar);
				break;
			default: {
				const size_t lastIndex = rawToken.rawValue.size() - 1;
				if (Lexer::isGearIdentifierPart(inputChar)
						|| Lexer::isGearIdentifierEnd(inputChar)) {
					accept(inputChar);
				} else if (Lexer::isGearIdentifierEnd(rawToken.rawValue.at(lastIndex))) {
					rawToken.item = RawLexicalItemIdentifier;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				} else if (Lexer::isGearIdentifierRepeatableEnd(rawToken.rawValue.at(lastIndex))
						&& Lexer::isGearIdentifierRepeatableEnd(inputChar)) {
					accept(inputChar);
				} else {
					rawToken.item = RawLexicalItemIdentifier;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				}
			}
		}
	}
}
