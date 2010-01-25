#ifndef OOLUA_CONSTUCT_H_
#	define OOLUA_CONSTUCT_H_

#include "lua_includes.h"
#include "oolua_storage.h"
#include "oolua_userdata.h"
#include "param_traits.h"
#include "member_func_helper.h"

namespace OOLUA
{	
	namespace INTERNAL
	{
		/*
		template<typename T>
		struct Constructor
		{
			template<typename P1>
			static int with_params(lua_State * const l)
			{
				lua_remove(l, 1);
				typedef param_type<P1> Param1;
				typename Param1::pull_type p1;
				OOLUA::Member_func_helper<Param1,Param1::owner>::pull2cpp(l,p1);
				T* obj = new T(p1);
				Lua_ud* ud = add_ptr(l,obj,false);
				ud->gc = true;
				return 1;
			}
		};
		*/
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

