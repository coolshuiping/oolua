#ifndef OOLUA_CONSTUCT_H_
#	define OOLUA_CONSTUCT_H_

#include "lua_includes.h"
#include "oolua_storage.h"
#include "oolua_userdata.h"

namespace OOLUA
{	
	namespace INTERNAL
	{
		template<typename T>
		inline int default_constructor(lua_State * /*const*/ l)
		{
			lua_remove(l, 1);
			T* obj = new T;
			Lua_ud* ud = add_ptr(l,obj,false);
			ud->gc = true;
			return 1;
		}
	}
}
#endif

