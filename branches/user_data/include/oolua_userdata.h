#ifndef OOLUA_USERDATA_H_
#	define OOLUA_USERDATA_H_

#include "oolua_config.h"

struct lua_State;
namespace OOLUA
{
    namespace INTERNAL
    {
		struct Lua_ud;
		typedef bool (*oolua_function_check_base)(lua_State* const l,INTERNAL::Lua_ud*,int const&);

        struct Lua_ud
        {
			void* void_class_ptr;
			oolua_function_check_base base_checker;
#if OOLUA_CHECK_EVERY_USERDATA_IS_CREATED_BY_OOLUA == 1 && \
					( OOLUA_LUA_USES_DEFAULT_CONFIG_FOR_LUA_514 == 1 \
						|| OOLUA_USING_DEFAULT_CONFIG_FOR_LUAJIT_20 == 1 )
			lua_State* created_by_state;
#endif
			unsigned int flags;
      };
		void userdata_const_value(Lua_ud* ud, bool value);
		bool userdata_is_constant(Lua_ud const* ud);
		bool userdata_is_to_be_gced(Lua_ud const * ud);
		void userdata_gc_value(Lua_ud* ud,bool value);
		
		enum UD_FLAGS {CONST_FLAG = 0x01,GC_FLAG = 0x02};
		
		inline void userdata_const_value(Lua_ud* ud, bool value)
		{
			if(value)ud->flags |= CONST_FLAG;
			else ud->flags &= (~CONST_FLAG);
		}
		inline bool userdata_is_constant(Lua_ud const* ud)
		{
			return (ud->flags & CONST_FLAG) != 0;
		}
		inline bool userdata_is_to_be_gced(Lua_ud const* ud)
		{
			return (ud->flags & GC_FLAG ) !=0;
		}
		inline void userdata_gc_value(Lua_ud* ud,bool value)
		{
			if(value) ud->flags |= GC_FLAG;
			else ud->flags &= (~ GC_FLAG);
		}	

    }
}

#endif
