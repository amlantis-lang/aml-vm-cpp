#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassParameterPlaceholderState::FirstPassParameterPlaceholderState(RawLexicalToken rawToken)
 		:	FirstPassState(rawToken) {}

	void
	Lexer::
	FirstPassParameterPlaceholderState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (Lexer::isDigitChar(inputChar)) {
			accept(inputChar);
		} else {
			rawToken.item = RawLexicalItemParameterPlaceholder;
			machine.appendToOutput(rawToken);
			machine.changeState(new FirstPassStartState);
			machine.handle(inputChar);
		}
	}
}
