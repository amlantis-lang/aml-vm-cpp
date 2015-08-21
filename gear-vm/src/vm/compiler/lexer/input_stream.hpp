#ifndef __aml_vm__input_stream__
#define __aml_vm__input_stream__

#include <unicode/utypes.h>

namespace AVM {
	class InputStream {
	public:
		virtual bool moveToNextUChar32() = 0;
		virtual UChar32 getCurrentUChar32() = 0;
	};
}

#endif /* defined(__aml_vm__input_stream__) */
