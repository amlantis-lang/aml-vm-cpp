#ifndef __gear_vm__file_input_stream__
#define __gear_vm__file_input_stream__

#include "input_stream.hpp"

#include <unicode/ustdio.h>

namespace GVM {
	class FileInputStream : public InputStream {
	public:
		FileInputStream(const char *filename, const char *codepage);
		~FileInputStream();

		bool moveToNextUChar32();
		UChar32 getCurrentUChar32();

	private:
		UChar32 current;
		UFILE *file;
	};
}

#endif /* defined(__gear_vm__file_input_stream__) */
