#include "oolua_push_pull.h"


//if called by Lua
//	luaL_error - as it throw an exception if Lua compiled as C++
//				- returns an error code to the caller if compiled as C

//if called from cpp
//	using exceptions
//	throw

//	storing last error
//		store error
//		return false

#include "oolua_storage.h"
#include "oolua_error.h"

namespace OOLUA
{
	namespace INTERNAL
	{
		void local_function_to_set_owner( lua_State* l,void* ptr, Owner own)
		{
			set_owner(l,ptr,own);
		}
		
		void handle_cpp_pull_fail(lua_State* l,char const * lookingFor)
		{
#	if OOLUA_USE_EXCEPTIONS == 1
			std::string message( std::string("Stack type is not a ") + lookingFor );
			std::string stackType = lua_gettop(l) ? lua_typename(l, lua_type(l,-1) ) : "empty stack";
			message += std::string(", yet ") + stackType;
			throw OOLUA::Type_error(message);
#	elif OOLUA_STORE_LAST_ERROR == 1
			lua_pushfstring(l, "Stack type is not a %s, yet %s"
							,lookingFor
							,lua_gettop(l) ? lua_typename(l, lua_type(l,-1) ) : "empty stack" );
			
			OOLUA::INTERNAL::set_error_from_top_of_stack_and_pop_the_error(l);
			return ;
#	else
			(void)l;
			(void)lookingFor;
			return ;
#	endif			
		}
		
		
		bool cpp_runtime_type_check_of_top(lua_State* l, int looking_for_lua_type, char const * type)
		{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
			if ( lua_gettop(l) == 0 || lua_type(l,-1) != looking_for_lua_type )
			{
				handle_cpp_pull_fail(l,type);
#	if OOLUA_USE_EXCEPTIONS == 0//prevent vs warnings
				return false;//dependant on config if the flow can return through this path
#	endif
			}
#else
			(void)l;
			(void)looking_for_lua_type;
			(void)type;	
#endif
			return true;
		}
		
		
		bool cpp_runtime_type_check_of_top(lua_State* l, compare_lua_type_func_sig compareFunc, char const * type)
		{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
			if ( ! compareFunc(l,-1) )
			{
				handle_cpp_pull_fail(l,type);
#	if OOLUA_USE_EXCEPTIONS == 0//prevent vs warnings
				return false;//dependant on config if the flow can return through this path
#	endif
			}
#else
			(void)l;
			(void)compareFunc;
			(void)type;	
#endif
			return true;
		}
		
		
	}
	
	
	
	
	bool push(lua_State* const s, bool const& value)
	{
		assert(s);
		lua_pushboolean(s, (value? 1 : 0) );
		return true;
	}
	bool push(lua_State* const s, std::string const& value)
	{
		assert(s);
		lua_pushlstring(s, value.data(), value.size() );
		return true;
	}
	bool push(lua_State* const s, char const * const& value)
	{
		assert(s && value);
		lua_pushstring (s, value);
		return true;
	}
	bool push(lua_State* const s, char * const& value)
	{
		assert(s && value);
		lua_pushstring (s, value);
		return true;
	}
	bool push(lua_State* const s, double const& value)
	{
		assert(s);
		lua_pushnumber(s, value);
		return true;
	}
	bool push(lua_State* const s, float const&  value)
	{
		assert(s);
		lua_pushnumber(s, value);
		return true;
	}
	bool push(lua_State* const s, lua_CFunction const &  value)
	{
		assert(s );
		lua_pushcclosure(s,value,0);
		return true;
	}
	bool push(lua_State* const s, Table const &  value)
	{
		assert(s);
		return value.push_on_stack(s);
	}
	
	bool pull(lua_State* const s, bool& value)
	{
		/*
		If it is allowed to pull a bool from an int, check for number instead of boolean
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,lua_isnumber,"bool") ) return false;
		 */
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TBOOLEAN,"bool") ) return false;
		value =  lua_toboolean( s, -1) ? true : false;
		lua_pop( s, 1);
		return true;
	}
	bool pull(lua_State* const s, std::string& value)
	{
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TSTRING,"string") ) return false;
		size_t len(0);
		char const* str = lua_tolstring(s,-1,&len);
		value = std::string(str, len);
		lua_pop( s, 1);
		return true;
	}
	
	bool pull(lua_State* const s, double& value)
	{
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TNUMBER,"double") ) return false;
		value = static_cast<double>( lua_tonumber( s, -1) );
		lua_pop( s, 1);
		return true;
	}
	bool pull(lua_State* const s, float& value)
	{
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TNUMBER,"float") ) return false;
		value = static_cast<float>( lua_tonumber( s, -1) );
		lua_pop( s, 1);
		return true;
	}
	bool pull(lua_State* const s, lua_CFunction& value)
	{
		if (! INTERNAL::cpp_runtime_type_check_of_top(s,lua_iscfunction,"lua_CFunction") ) return false;
		value = lua_tocfunction( s, -1);
		lua_pop( s, 1);
		return true;
	}
	
	bool pull(lua_State* const s, Table&  value)
	{
		return value.pull_from_stack(s);
	}

	
	
	
	namespace INTERNAL 
	{

		namespace LUA_CALLED
		{
			
			void pull_class_type_error(lua_State* const s,char const* type)
			{
				luaL_error(s,"%s %s %s","tried to pull type"
						   ,type
						   ,"which is not the type or a base of the type on the stack");
			}

			void pull_error(lua_State* l, int idx, char const* when_pulling_this_type)
			{
				luaL_error(l,"trying to pull %s when %s is on stack"
						   ,when_pulling_this_type
						   , lua_typename(l, lua_type(l,idx)) );
			}

			void get(lua_State* const s, int idx, bool& value)
			{
				
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isboolean(s,idx) )pull_error(s,idx,"bool");
#endif	
				value =  lua_toboolean( s, idx) ? true : false;
			}
			void get(lua_State* const s, int idx, char const*& value)
			{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if( lua_type(s,idx) != LUA_TSTRING )pull_error(s,idx,"char const*");
#endif
				value = lua_tolstring(s,idx,0);
			}
			void get(lua_State* const s, int idx, char *& value)
			{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isstring(s,idx) )pull_error(s,idx,"char*");//is this ever called?
#endif
				value = (char*)lua_tolstring(s,idx,0);
			}
//#if OOLUA_STD_STRING_IS_INTEGRAL == 1				
			void get(lua_State* const s, int idx, std::string& value)
			{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(lua_type(s,idx) != LUA_TSTRING )pull_error(s,idx,"std::string");
#endif
				size_t len(0);
				char const* str = lua_tolstring(s,idx,&len);
				value = std::string(str, len);
			}
//#endif			
			void get(lua_State* const s, int idx, double& value)
			{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isnumber(s,idx) )pull_error(s,idx,"double");
#endif
				value = static_cast<double>( lua_tonumber( s, idx) );
			}
			void get(lua_State* const s, int idx, float& value)
			{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isnumber(s,idx) )pull_error(s,idx,"float");
#endif
				value = static_cast<float>( lua_tonumber( s, idx) );
			}
			void get(lua_State* const s, int idx, lua_CFunction& value)
			{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_iscfunction(s,idx) )pull_error(s,idx,"lua_CFunction");
#endif
				value = lua_tocfunction( s, idx);
			}

			void get(lua_State* const s, int idx, Table&  value)
			{
				value.lua_get(s,idx);
			}	
		}
	}
}
