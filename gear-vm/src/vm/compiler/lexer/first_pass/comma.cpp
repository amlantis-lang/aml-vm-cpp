#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassCommaState::FirstPassCommaState() {};

	void
	Lexer::
	FirstPassCommaState::handle(GVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		accept(inputChar);
		rawToken.item = RawLexicalItemOperatorOrDelimiter;
		machine.appendToOutput(rawToken);
		machine.changeState(new FirstPassStartState);
	}
}
