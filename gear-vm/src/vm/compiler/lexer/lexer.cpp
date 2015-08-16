#include "lexer.hpp"

#include <cassert>

namespace GVM {

	Lexer::Lexer() {};

	std::vector<struct RawLexicalToken> *
	Lexer::parseInputStream(class InputStream *inputStream) {
		auto rawTokens = new std::vector<struct RawLexicalToken>();

		FirstPassMachine machine(rawTokens);
		while (inputStream->moveToNextUChar32()) {
			UChar32 inputChar = inputStream->getCurrentUChar32();
			machine.handle(inputChar);
		}
		machine.handle(inputStream->getCurrentUChar32()); /* U_EOF */

		return rawTokens; // change to real tokens after second pass
	};
	
	Lexer::
	FirstPassMachine::FirstPassMachine(std::vector<RawLexicalToken> *output)
		:	state(new Lexer::FirstPassStartState),
 			output(output),
 			parenthesisCounter(0),
 			squareBracketCounter(0),
			braceCounter(0) {};

	void
	Lexer::
	FirstPassMachine::handle(UChar32 inputChar) {
		state->handle(*this, inputChar);
	}

	void
	Lexer::
	FirstPassMachine::changeState(GVM::Lexer::FirstPassState *newState) {
		state.reset(newState);
	}

	void
	Lexer::
	FirstPassMachine::appendToOutput(GVM::RawLexicalToken rawToken) {
		output->push_back(rawToken);
	}

	void
	Lexer::
	FirstPassMachine::pushParenthesesElement(ParenthesesElement element) {
		parenthesesElements.push(element);
	}

	void
	Lexer::
	FirstPassMachine::popParenthesesElement() {
		parenthesesElements.pop();
	}

	ParenthesesElement
	Lexer::
	FirstPassMachine::peekParenthesesElement() const {
		return parenthesesElements.top();
	}

	unsigned_integer_32
	Lexer::
	FirstPassMachine::parenthesisCount() const {
		return parenthesisCounter;
	}

	void
	Lexer::
	FirstPassMachine::incrementParenthesisCounter() {
		parenthesisCounter += 1;
		assert(parenthesisCounter + squareBracketCounter + braceCounter
		       == parenthesesElements.size());
	}

	void
	Lexer::
	FirstPassMachine::decrementParenthesisCounter() {
		assert(parenthesisCounter >= 1);
		parenthesisCounter -= 1;
	}

	unsigned_integer_32
	Lexer::
	FirstPassMachine::squareBracketCount() const {
		return squareBracketCounter;
	}

	void
	Lexer::
	FirstPassMachine::incrementSquareBracketCounter() {
		squareBracketCounter += 1;
		assert(parenthesisCounter + squareBracketCounter + braceCounter
		       == parenthesesElements.size());
	}

	void
	Lexer::
	FirstPassMachine::decrementSquareBracketCounter() {
		assert(squareBracketCounter >= 1);
		squareBracketCounter -= 1;
	}

	unsigned_integer_32
	Lexer::
	FirstPassMachine::braceCount() const {
		return braceCounter;
	}

	void
	Lexer::
	FirstPassMachine::incrementBraceCounter() {
		braceCounter += 1;
		assert(parenthesisCounter + squareBracketCounter + braceCounter
		       == parenthesesElements.size());
	}

	void
	Lexer::
	FirstPassMachine::decrementBraceCounter() {
		assert(braceCounter >= 1);
		braceCounter -= 1;
	}

	bool
	Lexer::isLineBreakStart(UChar32 inputChar) {
		return inputChar == LF
			|| inputChar == CR
			|| inputChar == VT
			|| inputChar == FF
			|| inputChar == NEL
			|| inputChar == LS
			|| inputChar == PS
		;
	}

	bool
	Lexer::isUnicodeNamePart(UChar32 inputChar) {
		return (inputChar >= Letter_A && inputChar <= Letter_Z)
			|| (inputChar >= Letter_a && inputChar <= Letter_z)
			|| (inputChar >= Digit_0 && inputChar <= Digit_9);
	}

}

