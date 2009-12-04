
#define OOLUA_LUABIND_COMPARE
#define LUABIND_NO_ERROR_CHECKING
#define LUABIND_DONT_COPY_STRINGS
#define OOLUA_SWIG_COMPARE

#include "oolua.h"
#include "set_n_get_expose.h"
#include "hierarachy_expose.h"
#include <iostream>



#ifdef OOLUA_SWIG_COMPARE
extern "C"
{
	extern int luaopen_swig_profile(lua_State* L);
};
#endif


int main()
{
	{
		OOLUA::Script L;
		L.register_class<Set_get>();
		if (! L.run_file("./oolua_set_and_get.lua") )
		{
			std::cout <<OOLUA::get_last_error(L);
		}
	}
	{
		OOLUA::Script L;
		L.register_class<ProfileDerived>();
		OOLUA::register_class_and_bases<ProfileMultiBases>(L);
		if (! L.run_file("./oolua_derived.lua") )
		{
			std::cout <<OOLUA::get_last_error(L);
		}
	}

#ifdef OOLUA_SWIG_COMPARE
	{
		OOLUA::Script L;
		luaopen_swig_profile(L);
		if (! L.run_file("./swig_set_and_get.lua") )
		{
			std::cout <<OOLUA::get_last_error(L);
		}
	}
	{
		OOLUA::Script L;
		luaopen_swig_profile(L);
		if (! L.run_file("./swig_derived.lua") )
		{
			std::cout <<OOLUA::get_last_error(L);
		}
	}
#endif

#ifdef OOLUA_LUABIND_COMPARE
	{
		OOLUA::Script L;
		open_Luabind_set_n_get(L);
		if (! L.run_file("./luabind_set_and_get.lua") )
		{
			std::cout <<OOLUA::get_last_error(L);
		}
	}
	{
		OOLUA::Script L;
		open_Luabind_hierarchy(L);
		if (! L.run_file("./luabind_derived.lua") )
		{
			std::cout <<OOLUA::get_last_error(L);
		}
	}
#endif

}