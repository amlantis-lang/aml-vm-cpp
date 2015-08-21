#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassNewlineState::FirstPassNewlineState() {};

	void
	Lexer::
	FirstPassNewlineState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		if (rawToken.rawValue.size() == 0) {
			accept(inputChar);
			if (inputChar != LF && inputChar != CR) {
				/* Neither CR nor LF - these can be followed by LF or CR resp. */
				rawToken.item = RawLexicalItemNewline;
				machine.appendToOutput(rawToken);
				machine.changeState(new Lexer::FirstPassStartState);
			}

		} else if (rawToken.rawValue.size() == 1) {
			if (rawToken.rawValue.at(0) == CR && inputChar == LF) {
				/* CR+LF */
				accept(inputChar);
				rawToken.item = RawLexicalItemNewline;
				machine.appendToOutput(rawToken);
				machine.changeState(new Lexer::FirstPassStartState);

			} else if (rawToken.rawValue.at(0) == LF && inputChar == CR) {
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

}