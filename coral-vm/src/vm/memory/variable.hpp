#ifndef __coral_vm__variable__
#define __coral_vm__variable__

#include <memory>
#include "value.hpp"

namespace cvm {
	namespace vm {
		namespace memory {
			union value_link;
			struct variable {
				class std::shared_ptr<const value_link> value_link;
				//void *psi_type; // TODO: change for real pointer to Psi
			};
		}
	}
}

#endif /* defined(__coral_vm__variable__) */
