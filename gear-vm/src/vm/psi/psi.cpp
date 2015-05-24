#include "psi.hpp"

#include <string>
#include <cassert>

namespace GVM {
	PsiElement::PsiElement(PsiElement::Kind kind, const char *name)
		:	kind(kind),
			name(name),
			version(0),
			parent(nullptr),
			enclosing(nullptr),
 			children(nullptr) {}

	unsigned_integer_32
	PsiElement::get_version() const {
		return version;
	}

	class PsiElement *
	PsiElement::get_child_named(const char *name) const {
		assert(name != nullptr);
		PsiElementChild *child = children;
		while (child != nullptr) {
			if (strcasecmp(name, child->name)) {
				return child->child;
			} else {
				child = child->next;
			}
		}
		return nullptr;
	}

	void
	PsiElement::add_child_named(PsiElement *child, const char *name) {
		assert(child != nullptr);
		assert(name != nullptr);
		assert(get_child_named(name) == nullptr);
		struct PsiElementChild *const new_child = new PsiElementChild {
			.name = name,
			.child = child,
			.next = children
		};
		children = new_child;
	}
}
