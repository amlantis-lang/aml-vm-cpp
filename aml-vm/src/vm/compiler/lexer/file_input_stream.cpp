#include "file_input_stream.hpp"

namespace AVM {

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
	FileInputStream::canReadInput() {
		return file != nullptr;
	}
	
	bool
	FileInputStream::moveToNextUChar32() {
		if (canReadInput()) {
			current = u_fgetcx_55(file);
			return current != U_EOF
				&& current != 0xffffffff;
		} else {
			return false;
		}
	}

	UChar32
	FileInputStream::getCurrentUChar32() {
		return current;
	}
}
