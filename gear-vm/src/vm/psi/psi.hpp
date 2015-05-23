#ifndef __gear_vm__psi__
#define __gear_vm__psi__

#include "../../lib/predef.hpp"
#include "../memory/value.hpp"

#include <shared_mutex>

namespace GVM {
	class PsiElement {
	public:
		enum Kind {
			Blueprint =   0b00000001, /* classes, metaclasses, prototypes */
			Object =      0b00000010, /* singleton objects, modules, namespaces */
			Function =    0b00000100, /* functions, methods, operators */
			Union =       0b00001000, /* union type (set of multiple psi elements) */
			Constrained = 0b00010000, /* dependent type (composite of blueprint + function) */
			Include =     0b00100000, /* pointer to blueprint type */
			Alias =       0b01000000, /* alias to another psi element, in *parent */

			Trait =   Blueprint | Include,
			Functor = Object | Function
		};

		PsiElement(PsiElement::Kind, const char *);

		const Kind kind;
		const char *name;
		class PsiElement *const parent;
		class PsiElement *const enclosing;

		unsigned_integer_32
		get_version() const;

		class PsiElement * /* or nullptr if not found */
		get_child_named(const char *) const;

		void
		add_child_named(PsiElement *, const char *);

		static std::shared_timed_mutex lock;

	private:
		unsigned_integer_32 version;
		GValue associated_value;
		PsiElement *replaced_by;

		struct PsiElementChild {
			const char *name;
			PsiElement *const child;
			struct PsiElementChild *next;
		};

		struct PsiElementChild *children;

		// TODO: add more meta data
	};
}

#endif /* defined(__gear_vm__psi__) */
