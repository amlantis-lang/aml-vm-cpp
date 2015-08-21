#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassCommaState::FirstPassCommaState() {};

	void
	Lexer::
	FirstPassCommaState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		accept(inputChar);
		rawToken.item = RawLexicalItemOperatorOrDelimiter;
		machine.appendToOutput(rawToken);
		machine.changeState(new FirstPassStartState);
	}
}
