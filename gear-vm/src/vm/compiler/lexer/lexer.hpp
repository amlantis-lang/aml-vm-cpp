#ifndef __gear_vm__lexer__
#define __gear_vm__lexer__

#include "input_stream.hpp"
#include "../../../lib/predef.hpp"

#include <unicode/ustdio.h>
#include <vector>
#include <stack>

namespace GVM {
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

		RawLexicalItemCommentStart,
		RawLexicalItemCommentEnd,

		RawLexicalItemWhitespace,

		RawLexicalItemUnknown,
		RawLexicalItemBad
	} RawLexicalItem;

	typedef enum : UChar32 {
		/* Newlines as defined by Gear */
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
		Letter_U = 0x0055,
		Letter_Z = 0x005a,

		Letter_a = 0x0061,
		Letter_b = 0x0062,
		Letter_f = 0x0066,
		Letter_i = 0x0069,
		Letter_l = 0x006c,
		Letter_m = 0x006d,
		Letter_n = 0x006e,
		Letter_q = 0x0071,
		Letter_r = 0x0072,
		Letter_s = 0x0073,
		Letter_t = 0x0074,
		Letter_u = 0x0075,
		Letter_v = 0x0076,
		Letter_w = 0x0077,
		Letter_x = 0x0078,
		Letter_z = 0x007a,

		/* Digits */
		Digit_0 = 0x0030,
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
		static bool isGearIdentifierStart(UChar32 inputChar);
		static bool isGearIdentifierPart(UChar32 inputChar);
		static bool isGearIdentifierEnd(UChar32 inputChar);
		static bool isGearIdentifierRepeatableEnd(UChar32 inputChar);
		static bool isGearOperatorChar(UChar32 inputChar);
		static bool isDigitChar(UChar32 inputChar);

		class FirstPassState;

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

	};
}

#endif /* defined(__gear_vm__lexer__) */
