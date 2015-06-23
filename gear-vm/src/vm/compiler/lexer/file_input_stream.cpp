#include "file_input_stream.hpp"

namespace GVM {

	FileInputStream::FileInputStream(const char *filename, const char *codepage) {
		file = u_fopen_55(filename, "r", nullptr, codepage);
		current = U_EOF; /* use moveToNextUChar32() first */
	}

	FileInputStream::~FileInputStream() {
		if (file != nullptr) {
			u_fclose_55(file);
		}
	}

	bool
	FileInputStream::moveToNextUChar32() {
		current = u_fgetc_55(file);
		return current != U_EOF
			&& current != 0xffffffff;
	}

	UChar32
	FileInputStream::getCurrentUChar32() {
		return current;
	}
}
