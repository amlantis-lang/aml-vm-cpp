#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassIntegerSuffixState::FirstPassIntegerSuffixState(
			RawLexicalToken rawToken, RawLexicalItem associatedItem)
		:	FirstPassState(rawToken),
			associatedItem(associatedItem) {};

	void
	Lexer::
	FirstPassIntegerSuffixState::handle(FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isDigitChar(inputChar)) {
			accept(inputChar);
		} else {
			rawToken.item = associatedItem;
			machine.appendToOutput(rawToken);
			machine.changeState(Lexer::StartState1);
			machine.handle(inputChar);
		}
	}
}
