#include "../lexer.hpp"

namespace AVM {
  
	Lexer::
	FirstPassCommentState::FirstPassCommentState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken),
			nestingLevel(0) {};

	void
	Lexer::
	FirstPassCommentState::handle(AVM::Lexer::FirstPassMachine &machine, UChar32 inputChar) {
		accept(inputChar);
		size_t const lastIndex = rawToken.rawValue.size() - 1;
		size_t const penultimateIndex = lastIndex - 1;
		size_t const antepenultimateIndex = penultimateIndex - 1;
		if (   rawToken.rawValue.at(lastIndex) == RightParens
				&& rawToken.rawValue.at(penultimateIndex) == Asterisk
				&& u_isblank_55(rawToken.rawValue.at(antepenultimateIndex))
				&& nestingLevel == 0) {
			rawToken.item = RawLexicalItemComment;
			machine.appendToOutput(rawToken);
			machine.changeState(new FirstPassStartState);
		} else if (rawToken.rawValue.at(penultimateIndex) == LeftParens
				&& rawToken.rawValue.at(lastIndex) == Asterisk) {
			nestingLevel += 1;
		} else if (rawToken.rawValue.at(lastIndex) == RightParens
				&& rawToken.rawValue.at(penultimateIndex) == Asterisk) {
			if (nestingLevel == 0) {
				throw "Unexpected comment end";
			} else {
				nestingLevel -= 1;
			}
		}
	}
}
