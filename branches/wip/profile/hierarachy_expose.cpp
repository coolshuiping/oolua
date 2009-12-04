#include "hierarachy_expose.h"

#ifdef OOLUA_LUABIND_COMPARE
#	include "luabind/luabind.hpp"
#	include "luabind/operator.hpp"
#endif

EXPORT_OOLUA_FUNCTIONS_1_NON_CONST(ProfileBase,increment_a_base)
EXPORT_OOLUA_FUNCTIONS_0_CONST(ProfileBase)

EXPORT_OOLUA_NO_FUNCTIONS(ProfileAnotherBase)

EXPORT_OOLUA_NO_FUNCTIONS(ProfileDerived)

EXPORT_OOLUA_NO_FUNCTIONS(ProfileMultiBases)

void open_Luabind_hierarchy(lua_State* l)
{
#ifdef OOLUA_LUABIND_COMPARE
	luabind::open(l);
	luabind::module(l)
		[
			luabind::class_<ProfileBase>("ProfileBase")
			.def(luabind::constructor<>())
			.def("increment_a_base",&ProfileBase::increment_a_base)
		];
	luabind::module(l)
		[
			luabind::class_<ProfileAnotherBase>("ProfileAnotherBase")
			.def(luabind::constructor<>())
		];
	luabind::module(l)
		[
			luabind::class_<ProfileDerived,ProfileBase>("ProfileDerived")
			.def(luabind::constructor<>())
		];
	luabind::module(l)
		[
			luabind::class_<ProfileMultiBases,luabind::bases<ProfileAnotherBase,ProfileBase> >("ProfileMultiBases")
			.def(luabind::constructor<>())
		];
#else
	(void)l;
#endif
}
