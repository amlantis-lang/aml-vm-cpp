#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassSemicolonState::FirstPassSemicolonState() {};

	void
	Lexer::
	FirstPassSemicolonState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		switch (rawToken.rawValue.size()) {
			case 0:
				accept(inputChar);
				break;

			case 1:
				if (inputChar == Semicolon) {
					accept(inputChar);
					rawToken.item = RawLexicalItemDoubleSemicolon;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
				} else {
					rawToken.item = RawLexicalItemSemicolon;
					machine.appendToOutput(rawToken);
					machine.changeState(new FirstPassStartState);
					machine.handle(inputChar);
				}
				break;

			default:
				throw "Illegal state";
		}
	}
}
