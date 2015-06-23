#ifndef __gear_vm__lexer__
#define __gear_vm__lexer__

#include "input_stream.hpp"

#include <unicode/ustdio.h>
#include <vector>

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
		RawLexicalItemSymbolLiteral,
		RawLexicalItemRegexpLiteral,
		RawLexicalItemArrayLiteralStart,
		RawLexicalItemListLiteralStart,
		RawLexicalItemDictionaryLiteralStart,
		RawLexicalItemBagLiteralStart,

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
		Solidus = 0x002f,
		ReverseSolidus = 0x005c,
		Apostrophe = 0x0027,
		QuotationMark = 0x0022,
		QuestionMark = 0x003f,
		NumberSign = 0x0023,
		HashSign = NumberSign,
		DollarSign = 0x0024,
		Ampersand = 0x0026,
		Space = 0x0020,
		Semicolon = 0x003b,
		PercentSign = 0x0025,

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
		Letter_r = 0x0072,
		Letter_s = 0x0073,
		Letter_t = 0x0074,
		Letter_u = 0x0075,
		Letter_v = 0x0076,
		Letter_x = 0x0078,
		Letter_z = 0x007a,

		/* Digits */
		Digit_0 = 0x0030,
		Digit_9 = 0x0039
	} KnownCharacters;

	struct RawLexicalToken {
		RawLexicalItem item;
		class UnicodeString rawValue;
		char flags[4];
	};

	class Lexer {
	public:
		Lexer();

		std::vector<struct RawLexicalToken> *
		parseInputStream(class InputStream *inputStream);

		class FirstPassState;

		class FirstPassMachine {
		public:
			FirstPassMachine(std::vector<RawLexicalToken> *output);
			void handle(UChar32 inputChar);
			void changeState(FirstPassState *newState);
			void appendToOutput(RawLexicalToken rawToken);
		private:
			std::unique_ptr<FirstPassState> state;
			std::vector<RawLexicalToken> *output;
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
				rawToken.rawValue.append(inputChar);
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

		class FirstPassStringLiteralState : public FirstPassState {
		public:
			FirstPassStringLiteralState();
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		};

		typedef enum : int8_t {
			ExtendedCharacterEscapeTypeInvalid = 0,

			ExtendedCharacterEscapeTypeHex = 1,
			ExtendedCharacterEscapeTypeUnicode4,
			ExtendedCharacterEscapeTypeUnicode8,
			ExtendedCharacterEscapeTypeEntity,
			ExtendedCharacterEscapeTypeUnicodeNamed
		} ExtendedCharacterEscapeType;

		class FirstPassCharacterLiteralState : public FirstPassState {
		public:
			FirstPassCharacterLiteralState(RawLexicalToken rawToken);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			bool doingEscapeSequence;
			bool completedEscapeSequence;
			ExtendedCharacterEscapeType escapeExtendedType;
		};

		class FirstPassRegexpLiteralState : public FirstPassState {
		public:
			FirstPassRegexpLiteralState(RawLexicalToken rawToken, UChar32 delimiter);
			void handle(FirstPassMachine &machine, UChar32 inputChar);
		private:
			UChar32 delimiter;
		};

	};
}

#endif /* defined(__gear_vm__lexer__) */
