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
			|| (Lexer::isDigitChar(inputChar));
	}

	static
	bool
	isLetterChar(UChar32 inputChar) {
		const UCharCategory category = (UCharCategory) u_charType_55(inputChar);
		return category == U_UPPERCASE_LETTER
			|| category == U_LOWERCASE_LETTER
			|| category == U_TITLECASE_LETTER
			|| category == U_OTHER_LETTER
			|| category == U_MODIFIER_LETTER
			|| category == U_LETTER_NUMBER;
	}

	static
	bool
	isConnectingChar(UChar32 inputChar) {
		return u_charType_55(inputChar) == U_CONNECTOR_PUNCTUATION;
	}

	static
	bool
	isCombiningChar(UChar32 inputChar) {
		const UCharCategory category = (UCharCategory) u_charType_55(inputChar);
		return category == U_NON_SPACING_MARK
			|| category == U_COMBINING_SPACING_MARK;
	}

	static
	bool
	isFormattingChar(UChar32 inputChar) {
		return u_charType_55(inputChar) == U_FORMAT_CHAR;
	}

	static
	bool
	isLowerChar(UChar32 inputChar) {
		return u_charType_55(inputChar) == U_LOWERCASE_LETTER
			|| inputChar == Underscore;
	}

	static
	bool
	isUpperChar(UChar32 inputChar) {
		return (isLetterChar(inputChar) && !isLowerChar(inputChar))
			|| inputChar == DollarSign
			|| inputChar == SectionSign;
	}

	static
	bool
	isGearIdentifierChar(UChar32 inputChar) {
		return isLetterChar(inputChar)
			|| Lexer::isDigitChar(inputChar)
			|| isConnectingChar(inputChar)
			|| isCombiningChar(inputChar)
			|| isFormattingChar(inputChar)
			|| inputChar == PlusSign
			|| inputChar == HyphenMinusSign
			|| inputChar == Solidus
			|| inputChar == Underscore;
	}

	bool
	Lexer::isDigitChar(UChar32 inputChar) {
		return inputChar >= Digit_0 && inputChar <= Digit_9;
	}

	bool
	Lexer::isGearIdentifierStart(UChar32 inputChar) {
		return isLowerChar(inputChar)
			|| isUpperChar(inputChar);
	}

	bool
	Lexer::isGearIdentifierPart(UChar32 inputChar) {
		return isGearIdentifierChar(inputChar);
	}

	bool
	Lexer::isGearIdentifierEnd(UChar32 inputChar) {
		return inputChar == QuestionMark
			|| inputChar == ExclamationMark;
	}

	bool
	Lexer::isGearIdentifierRepeatableEnd(UChar32 inputChar) {
		return isGearIdentifierChar(inputChar)
			|| inputChar == Apostrophe;
	}

	bool
	Lexer::isGearOperatorChar(UChar32 inputChar) {
		const UCharCategory category = (UCharCategory) u_charType_55(inputChar);
		return category == U_MATH_SYMBOL
			|| category == U_OTHER_SYMBOL
			|| inputChar == ExclamationMark
			|| inputChar == HashSign
			|| inputChar == PercentSign
			|| inputChar == Ampersand
			|| inputChar == Asterisk
			|| inputChar == PlusSign
			|| inputChar == HyphenMinusSign
			|| inputChar == Dot
			|| inputChar == Solidus
			|| inputChar == Colon
			|| inputChar == LessThanSign
			|| inputChar == EqualsSign
			|| inputChar == ExclamationMark
			|| inputChar == GreaterThanSign
			|| inputChar == QuestionMark
			|| inputChar == AtSign
			|| inputChar == ReverseSolidus
			|| inputChar == GraveAccent
			|| inputChar == VerticalLine
			|| inputChar == Tilde;
	}
}

