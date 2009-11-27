#include "oolua_error.h"

#ifdef OOLUA_STORE_ERROR

#   include "lua_includes.h"

namespace OOLUA
{
    char last_error_string[] = {"oolua_last_error"};
    void reset_error_value(lua_State*l)
    {
        //lua_pushliteral(l,last_error_string);
        lua_pushstring(l,last_error_string);
        lua_pushnil(l);
        lua_settable(l, LUA_REGISTRYINDEX);
    }
    std::string get_last_error(lua_State*l)
    {
        //lua_pushliteral(l,last_error_string);
        lua_pushstring(l,last_error_string);
        lua_settable(l, LUA_REGISTRYINDEX);
        std::string error;
        if(! lua_isnil(l,lua_gettop(l) ) )
            error = lua_tolstring(l,-1,0);
        lua_pop( l, 1);
        return error;
    }
    namespace INTERNAL
    {
        void set_error_from_top_of_stack(lua_State*l)
        {
            int error_index = lua_gettop(l);
            //lua_pushliteral(l,last_error_string);
            lua_pushstring(l,last_error_string);
            lua_pushvalue(l,error_index);
            lua_settable(l, LUA_REGISTRYINDEX);
        }
    }
}
#endif
