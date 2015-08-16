#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassWhitespaceState::FirstPassWhitespaceState() {};
	
	void
	Lexer::
	FirstPassWhitespaceState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (u_isblank_55(inputChar)) {
			accept(inputChar);

		} else {
			if (rawToken.rawValue.size() >= 1) {
				/* do not append empty whitespace, that would be utterly silly */
				rawToken.item = RawLexicalItemWhitespace;
				machine.appendToOutput(rawToken);
			}
			machine.changeState(new Lexer::FirstPassStartState);
			machine.handle(inputChar);
		}
	}

}