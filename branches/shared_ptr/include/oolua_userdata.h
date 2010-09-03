#ifndef OOLUA_USERDATA_H_
#	define OOLUA_USERDATA_H_

#include "oolua_config.h"
#include <cstring>
namespace OOLUA
{
    namespace INTERNAL
    {
        struct Lua_ud
        {
			Lua_ud()
				:void_class_ptr(0)
				,name(0)
				,none_const_name(0)
				,name_size(0)
				,void_shared_ptr(0)
				,gc(false)
			{}
			void* void_class_ptr;
			char* name;
			char* none_const_name;//none constant name of the class
			int name_size;//size of name
			void* void_shared_ptr;//if a shared pointer then the instance we are keeping alive
            bool gc;//should it be garbage collected
        };
		
		
		inline bool id_is_const(Lua_ud* ud)
		{
			return ud->name != ud->none_const_name;
		}

#if OOLUA_SAFE_ID_COMPARE == 1
		inline bool ids_equal(char* lhsName,int lhsSize,char* rhsName,int rhsSize)
		{
			if(lhsSize != rhsSize)return false;
			return memcmp(lhsName,rhsName,lhsSize) == 0;
		}
#else
		inline bool ids_equal(char* lhsName,int /*lhsSize*/,char* rhsName,int /*rhsSize*/)
		{
			return lhsName == rhsName;
		}		
#endif
		
    }
}
#endif
