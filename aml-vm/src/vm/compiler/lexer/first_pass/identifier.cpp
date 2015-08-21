#include "../lexer.hpp"

namespace AVM {

	Lexer::
	FirstPassIdentifierState::FirstPassIdentifierState() {};

	void
	Lexer::
	FirstPassIdentifierState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
    	case 0:
				/* correctness handled by the start state */
				accept(inputChar);
				break;
			default: {
				const size_t lastIndex = rawToken.rawValue.size() - 1;
				if (Lexer::isAmlIdentifierPart(inputChar)
						|| Lexer::isAmlIdentifierEnd(inputChar)) {
					accept(inputChar);
				} else if (Lexer::isAmlIdentifierEnd(rawToken.rawValue.at(lastIndex))) {
					rawToken.item = RawLexicalItemIdentifier;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				} else if (Lexer::isAmlIdentifierRepeatableEnd(rawToken.rawValue.at(lastIndex))
						&& Lexer::isAmlIdentifierRepeatableEnd(inputChar)) {
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
