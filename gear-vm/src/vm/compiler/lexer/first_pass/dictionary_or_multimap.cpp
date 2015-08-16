#include "../lexer.hpp"

namespace GVM {
  
	Lexer::
	FirstPassDictionaryOrMultimapState::FirstPassDictionaryOrMultimapState(RawLexicalToken rawToken)
		:	FirstPassState(rawToken) {}

}
