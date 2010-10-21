#include "oolua_userdata.h"

namespace OOLUA
{
    namespace INTERNAL
    {
		/*
		enum UD_FLAGS {CONST_FLAG = 0x01,GC_FLAG = 0x02};
		
		void userdata_const_value(Lua_ud* ud, bool value)
		{
			if(value)ud->flags |= CONST_FLAG;
			else ud->flags &= (~CONST_FLAG);
		}
		bool userdata_is_constant(Lua_ud* ud)
		{
			return (ud->flags & CONST_FLAG) == CONST_FLAG;
		}
		bool userdata_is_to_be_gced(Lua_ud* ud)
		{
			return (ud->flags & GC_FLAG ) == GC_FLAG;
		}
		void userdata_gc_value(Lua_ud* ud,bool value)
		{
			if(value) ud->flags |= GC_FLAG;
			else ud->flags &= (~ GC_FLAG);
		}		
		 */
	}
}