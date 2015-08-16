#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassDictionaryOrMultimapState::FirstPassDictionaryOrMultimapState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassDictionaryOrMultimapState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (inputChar == LeftBrace) {
			accept(inputChar);
			rawToken.item = RawLexicalItemMultimapLiteralStart;
			machine.appendToOutput(rawToken);

			/* increment once for each brace */
			machine.pushParenthesesElement(ParenthesesElementMultimap);
			machine.incrementBraceCounter();
			machine.pushParenthesesElement(ParenthesesElementMultimap);
			machine.incrementBraceCounter();

			machine.changeState(new FirstPassStartState);
		} else {
			rawToken.item = RawLexicalItemDictionaryLiteralStart;
			machine.appendToOutput(rawToken);

			machine.pushParenthesesElement(ParenthesesElementDictionary);
			machine.incrementBraceCounter();

			machine.changeState(new FirstPassStartState);
			machine.handle(inputChar);
		}
	}
}
