#ifndef __aml_vm__lexer__
#define __aml_vm__lexer__

#include "input_stream.hpp"
#include "../../../lib/predef.hpp"

#include <unicode/ustdio.h>
#include <vector>
#include <stack>

namespace AVM {
	typedef enum : uint8_t {
		RawLexicalItemIdentifier,
		RawLexicalItemOperatorOrDelimiter,
		RawLexicalItemLeftParenthesis,
		RawLexicalItemRightParenthesis,
		RawLexicalItemLeftSquareBracket,
		RawLexicalItemRightSquareBracket,
		RawLexicalItemLeftBrace,
		RawLexicalItemRightBrace,
		RawLexicalItemNewline,
		RawLexicalItemSemicolon,
		RawLexicalItemDoubleSemicolon,
		RawLexicalItemIntegerLiteral,
		RawLexicalItemRealLiteral,
		RawLexicalItemRationalDenominatorLiteral,
		RawLexicalItemComplexImaginaryLiteral,
		RawLexicalItemCharacterLiteral,
		RawLexicalItemBooleanLiteral,

		RawLexicalItemStringLiteral,
		RawLexicalItemStringLiteralContinuation,
		RawLexicalItemSymbolLiteral,
		RawLexicalItemSymbolLiteralContinuation,
		RawLexicalItemRegexpLiteral,
		RawLexicalItemRegexpLiteralContinuation,

		RawLexicalItemParameterPlaceholder,
		RawLexicalItemPolymorphicTypeStart,
		RawLexicalItemPolymorphicTypeEnd,
		RawLexicalItemPolymorphicTagName,

		RawLexicalItemArrayLiteralStart,
		RawLexicalItemArrayLiteralEnd,
		RawLexicalItemListLiteralStart,
		RawLexicalItemListLiteralEnd,
		RawLexicalItemDictionaryLiteralStart,
		RawLexicalItemDictionaryLiteralEnd,
		RawLexicalItemMultimapLiteralStart,
		RawLexicalItemMultimapLiteralEnd,
		RawLexicalItemBagLiteralStart,
		RawLexicalItemBagLiteralEnd,

		RawLexicalItemComment,

		RawLexicalItemWhitespace,

		RawLexicalItemUnknown,
		RawLexicalItemBad
	} RawLexicalItem;

	typedef enum : UChar32 {
		/* Newlines as defined by Aml */
		LF = 0x000a,
		CR = 0x000d,
		VT = 0x000b,
		FF = 0x000c,
		NEL = 0x0085,
		LS = 0x2028,
		PS = 0x2029,

		/* Parentheses */
		LeftParens = 0x0028,
		RightParens = 0x0029,
		LeftSquareBracket = 0x005b,
		RightSquareBracket = 0x005d,
		LeftBrace = 0x007b,
		RightBrace = 0x007d,

		/* Operators, delimiters */
		Space = 0x0020,
		ExclamationMark = 0x0021,
		QuotationMark = 0x0022,
		NumberSign = 0x0023,
		HashSign = NumberSign,
		DollarSign = 0x0024,
		PercentSign = 0x0025,
		Ampersand = 0x0026,
		Apostrophe = 0x0027,
		Asterisk = 0x002a,
		PlusSign = 0x002b,
		Comma = 0x002c,
		HyphenMinusSign = 0x002d,
		FullStop = 0x002e,
		Dot = FullStop,
		Solidus = 0x002f,
		Colon = 0x003a,
		Semicolon = 0x003b,
		LessThanSign = 0x003c,
		EqualsSign = 0x003d,
		GreaterThanSign = 0x003e,
		QuestionMark = 0x003f,
		AtSign = 0x0040,
		ReverseSolidus = 0x005c,
		CircumflexAccent = 0x005e,
		LowLine = 0x005f,
		Underscore = LowLine,
		GraveAccent = 0x0060,
		Backtick = GraveAccent,
		VerticalLine = 0x007c,
		Tilde = 0x007e,
		SectionSign = 0x00a7,
		DivisionSign = 0x00f7,

		/* Letters */
		Letter_A = 0x0041,
		Letter_B = 0x0042,
		Letter_C = 0x0043,
		Letter_D = 0x0044,
		Letter_E = 0x0045,
		Letter_F = 0x0046,
		Letter_G = 0x0047,
		Letter_H = 0x0048,
		Letter_I = 0x0049,
		Letter_J = 0x004a,
		Letter_K = 0x004b,
		Letter_L = 0x004c,
		Letter_M = 0x004d,
		Letter_N = 0x004e,
		Letter_O = 0x004f,
		Letter_P = 0x0050,
		Letter_Q = 0x0051,
		Letter_R = 0x0052,
		Letter_S = 0x0053,
		Letter_T = 0x0054,
		Letter_U = 0x0055,
		Letter_V = 0x0056,
		Letter_W = 0x0057,
		Letter_X = 0x0058,
		Letter_Y = 0x0059,
		Letter_Z = 0x005a,

		Letter_a = 0x0061,
		Letter_b = 0x0062,
		Letter_c = 0x0063,
		Letter_d = 0x0064,
		Letter_e = 0x0065,
		Letter_f = 0x0066,
		Letter_g = 0x0067,
		Letter_h = 0x0068,
		Letter_i = 0x0069,
		Letter_j = 0x006a,
		Letter_k = 0x006b,
		Letter_l = 0x006c,
		Letter_m = 0x006d,
		Letter_n = 0x006e,
		Letter_o = 0x006f,
		Letter_p = 0x0070,
		Letter_q = 0x0071,
		Letter_r = 0x0072,
		Letter_s = 0x0073,
		Letter_t = 0x0074,
		Letter_u = 0x0075,
		Letter_v = 0x0076,
		Letter_w = 0x0077,
		Letter_x = 0x0078,
		Letter_y = 0x0079,
		Letter_z = 0x007a,

		/* Digits */
		Digit_0 = 0x0030,
		Digit_1 = 0x0031,
		Digit_2 = 0x0032,
		Digit_3 = 0x0033,
		Digit_4 = 0x0034,
		Digit_5 = 0x0035,
		Digit_6 = 0x0036,
		Digit_7 = 0x0037,
		Digit_8 = 0x0038,
		Digit_9 = 0x0039
	} KnownCharacters;

	typedef enum : unsigned_integer_8 {
		ParenthesesElementBasic = 0,
		ParenthesesElementInterpolatedString = 1,
		ParenthesesElementInterpolatedRegexp = 2,
		ParenthesesElementInterpolatedSymbol = 3,
		ParenthesesElementList = 4,
		ParenthesesElementArray = 5,
		ParenthesesElementDictionary = 6,
		ParenthesesElementMultimap = 7,
		ParenthesesElementBag = 8,
		ParenthesesElementPolymorphicType = 9
	} ParenthesesElement;

	struct RawLexicalToken {
		RawLexicalItem item;
		class std::vector<UChar32> rawValue;
		char flags[4];
	};

	class Lexer {
	public:
		Lexer();

		std::vector<struct RawLexicalToken> *
		parseInputStream(class InputStream *inputStream);

		static bool isLineBreakStart(UChar32 inputChar);
		static bool isUnicodeNamePart(UChar32 inputChar);
		static bool isAmlIdentifierStart(UChar32 inputChar);
		static bool isAmlIdentifierPart(UChar32 inputChar);
		static bool isAmlIdentifierEnd(UChar32 inputChar);
		static bool isAmlIdentifierRepeatableEnd(UChar32 inputChar);
		static bool isAmlOperatorChar(UChar32 inputChar);
		static bool isDigitChar(UChar32 inputChar);
		static bool isHexDigitChar(UChar32 inputChar);
		static bool isOctDigitChar(UChar32 inputChar);
		static bool isBinDigitChar(UChar32 inputChar);
		static bool isSxgsDigitChar(UChar32 inputChar);
		static bool isDdecDigitChar(UChar32 inputChar);
		static bool isIntegerSuffixChar(UChar32 inputChar);
		static bool isFpSuffixChar(UChar32 inputChar);
		static bool isFpHexSuffixChar(UChar32 inputChar);
		static bool isLetterChar(UChar32 inputChar);
		static bool isAmlDelimiterChar(UChar32 inputChar);

		class FirstPassState;
		class FirstPassStartState;

		static FirstPassStartState *const StartState1;

		class FirstPassMachine {
		public:
			FirstPassMachine(std::vector<RawLexicalToken> *output);
			void handle(UChar32 inputChar);
			void changeState(FirstPassState *newState);
			void appendToOutput(RawLexicalToken rawToken);
			void pushParenthesesElement(ParenthesesElement element);
			void popParenthesesElement();
			ParenthesesElement peekParenthesesElement() const;
			unsigned_integer_32 parenthesisCount() const;
			void incrementParenthesisCounter();
			void decrementParenthesisCounter();
			unsigned_integer_32 squareBracketCount() const;
			void incrementSquareBracketCounter();
			void decrementSquareBracketCounter();
			unsigned_integer_32 braceCount() const;
			void incrementBraceCounter();
			void decrementBraceCounter();
		private:
			std::unique_ptr<FirstPassState> state;
			std::vector<RawLexicalToken> *output;
			std::stack<ParenthesesElement> parenthesesElements;
			unsigned_integer_32 parenthesisCounter;
			unsigned_integer_32 squareBracketCounter;
			unsigned_integer_32 braceCounter;
		};

		class FirstPassState {
		public:
			FirstPassState();
			FirstPassState(RawLexicalToken rawToken);

			virtual void
			handle(FirstPassMachine &machine, UChar32 inputChar) = 0;

		protected:
			RawLexicalToken rawToken;

			void
			accept(UChar32 inputChar) {
				rawToken.rawValue.push_back(inputChar);
			};
		};

		class FirstPassStartState : public FirstPassState {
		public:
			FirstPassStartState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassWhitespaceState : public FirstPassState {
		public:
			FirstPassWhitespaceState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassNewlineState : public FirstPassState {
		public:
			FirstPassNewlineState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassLeftParenthesisState : public FirstPassState {
		public:
			FirstPassLeftParenthesisState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassRightParenthesisState : public FirstPassState {
		public:
			FirstPassRightParenthesisState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassLeftSquareBracketState : public FirstPassState {
		public:
			FirstPassLeftSquareBracketState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassRightSquareBracketState : public FirstPassState {
		public:
			FirstPassRightSquareBracketState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassLeftBraceState : public FirstPassState {
		public:
			FirstPassLeftBraceState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassRightBraceState : public FirstPassState {
		public:
			FirstPassRightBraceState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassParameterPlaceholderState : public FirstPassState {
		public:
			FirstPassParameterPlaceholderState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassOperatorOrLiteralState : public FirstPassState {
		public:
			FirstPassOperatorOrLiteralState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassOperatorOrIdentifierState : public FirstPassState {
		public:
			FirstPassOperatorOrIdentifierState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassOperatorState : public FirstPassState {
		public:
			FirstPassOperatorState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassIdentifierState : public FirstPassState {
		public:
			FirstPassIdentifierState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		typedef enum : unsigned_integer_8 {
			/* not yet in a state that can safely decide */
			QuotedIdentifierTypeUndecided,
			/* quoted keyword or identifier, or also polymorphic variant tag name */
			QuotedIdentifierTypeNamed,
			/* quoted operator name */
			QuotedIdentifierTypeOperator,
			/* arbitrary quoted identifier */
			QuotedIdentifierTypeDoubleQuoted
		} QuotedIdentifierType;

		class FirstPassQuotedIdentifierState : public FirstPassState {
		public:
			FirstPassQuotedIdentifierState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			QuotedIdentifierType identifierType;
		};

		class FirstPassStringState : public FirstPassState {
		public:
			FirstPassStringState();
			FirstPassStringState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassRawStringState : public FirstPassState {
		public:
			FirstPassRawStringState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassQuotedStringState : public FirstPassState {
		public:
			FirstPassQuotedStringState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassVerbatimStringState : public FirstPassState {
		public:
			FirstPassVerbatimStringState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		typedef enum : unsigned_integer_8 {
			ExtendedCharacterEscapeTypeInvalid = 0,

			ExtendedCharacterEscapeTypeHex = 1,
			ExtendedCharacterEscapeTypeUnicode4,
			ExtendedCharacterEscapeTypeUnicode8,
			ExtendedCharacterEscapeTypeEntity,
			ExtendedCharacterEscapeTypeUnicodeNamed
		} ExtendedCharacterEscapeType;

		class FirstPassCharacterState : public FirstPassState {
		public:
			FirstPassCharacterState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			bool doingEscapeSequence;
			bool completedEscapeSequence;
			ExtendedCharacterEscapeType escapeExtendedType;
		};

		class FirstPassWordsListOrArrayState : public FirstPassState {
		public:
			FirstPassWordsListOrArrayState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassListOrArrayState : public FirstPassState {
		public:
			FirstPassListOrArrayState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassDictionaryOrMultimapState : public FirstPassState {
		public:
			FirstPassDictionaryOrMultimapState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassRegexpState : public FirstPassState {
		public:
			FirstPassRegexpState(RawLexicalToken rawToken, UChar32 delimiter);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			UChar32 delimiter;
		};

		class FirstPassSubstitutionRegexpState : public FirstPassState {
		public:
			FirstPassSubstitutionRegexpState(RawLexicalToken rawToken, UChar32 delimiter);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			UChar32 delimiter;
		};

		class FirstPassCommaState : public FirstPassState {
		public:
			FirstPassCommaState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassSemicolonState : public FirstPassState {
		public:
			FirstPassSemicolonState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassNumberState : public FirstPassState {
		public:
			FirstPassNumberState();
			FirstPassNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassDecimalNumberState : public FirstPassState {
		public:
			FirstPassDecimalNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassDecimalFloatingOrFixedPointNumberState : public FirstPassState {
		public:
			FirstPassDecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassHexadecimalNumberState : public FirstPassState {
		public:
			FirstPassHexadecimalNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassHexadecimalFloatingOrFixedPointNumberState : public FirstPassState {
		public:
			FirstPassHexadecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassOctalNumberState : public FirstPassState {
		public:
			FirstPassOctalNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassOctalFloatingOrFixedPointNumberState : public FirstPassState {
		public:
			FirstPassOctalFloatingOrFixedPointNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassBinaryNumberState : public FirstPassState {
		public:
			FirstPassBinaryNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassBinaryFloatingOrFixedPointNumberState : public FirstPassState {
		public:
			FirstPassBinaryFloatingOrFixedPointNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassSexagesimalNumberState : public FirstPassState {
		public:
			FirstPassSexagesimalNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassSexagesimalFloatingOrFixedPointNumberState : public FirstPassState {
		public:
			FirstPassSexagesimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassDuodecimalNumberState : public FirstPassState {
		public:
			FirstPassDuodecimalNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassDuodecimalFloatingOrFixedPointNumberState : public FirstPassState {
		public:
			FirstPassDuodecimalFloatingOrFixedPointNumberState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		class FirstPassIntegerSuffixState : public FirstPassState {
		public:
			FirstPassIntegerSuffixState(RawLexicalToken rawToken, RawLexicalItem associatedItem);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			const RawLexicalItem associatedItem;
		};

		class FirstPassCommentState : public FirstPassState {
		public:
			FirstPassCommentState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			unsigned_integer_8 nestingLevel;
		};

	};
}

#endif /* defined(__aml_vm__lexer__) */
