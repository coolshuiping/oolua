#ifndef CPP_CONSTRUCTOR_H_
#	define CPP_CONSTRUCTOR_H_



/*want to write something like
 
 OOLUA_CLASS_NO_BASES(foo)
	OOLUA_NO_TYPEDEFS
	OOLUA_CONSTRUCTORS_BEGIN
		CONSTRUCTOR(int)
		CONSTRUCTOR(Bar*)
	OOLUA_CONSTRUCTORS_END
 OOLUA_CLASS_END
 
 
*/
#include "lua_includes.h"
#include "oolua_storage.h"
#include "param_traits.h"
#include "oolua_userdata.h"
#include "oolua_push_pull.h"
#include "lvd_types.h"
#include "Type_list.h"

namespace OOLUA
{
	namespace CREATE
	{
		template<typename Type,int HasNoDefaultTypedef>
		struct Constructor
		{
			static int construct(lua_State * l)
			{
				Type* obj = new Type;
				OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
				ud->gc = true;
				return 1;
			}
		};
		
		template<typename Type>
		struct Constructor<Type,1>
		{
			static int construct(lua_State * l)
			{
				luaL_error(l,"%s %s %s","No parameters passed to the constructor of the type"
						   ,OOLUA::Proxy_class<Type>::class_name
						   ,"which does not have a default constructor.");
				return 0;
			}
		};
		
		template<typename Class,typename Param1WithTraits,typename Param1,int is_intergal>struct Constructor1;

		/*
		template<typename Class,typename Param1WithTraits,typename Param1>
		struct Constructor1<Class,Param1WithTraits,Param1,0>
		{
			static int construct(lua_State* l)
			{
				if( lua_type(l,1) == LUA_TUSERDATA )
				{
					typename Param1WithTraits::raw_type* type_is_valid = OOLUA::INTERNAL::class_from_index<typename Param1WithTraits::raw_type>(l,1);
					if(type_is_valid)
					{
						lua_remove(l,1);
						OOLUA::Converter<typename Param1WithTraits::raw_type*, Param1> convert1 (type_is_valid);
						Class* obj = new Class( convert1() );
						OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
						ud->gc = true;
						return 1;
					}
				}
				return 0;
			}
		};
		*/
		
		
		template<typename Cpp_type,int Lua_type>
		struct lua_type_is_cpp_type;
		
		template<typename Cpp_type>
		struct lua_type_is_cpp_type<Cpp_type,LUA_TNUMBER>
		{
			typedef Type_list<
			char,unsigned char, signed char,
			short,unsigned short, signed short,
			int,unsigned int, signed int,
			long, unsigned long, signed long, LVD::int64, LVD::uint64,
			float,
			double, long double>::type Lua_number;
			enum {value = TYPELIST::IndexOf<Lua_number,Cpp_type>::value == -1 ? 0 : 1};
		};
		
		template<typename Cpp_type>
		struct lua_type_is_cpp_type<Cpp_type,LUA_TSTRING>
		{
			typedef Type_list<
			char*,std::string
			>::type Lua_string;
			enum {value = TYPELIST::IndexOf<Lua_string,Cpp_type>::value == -1 ? 0 : 1};
		};

		template<typename Cpp_type>
		struct lua_type_is_cpp_type<Cpp_type,LUA_TBOOLEAN>
		{
			enum {value = LVD::is_same<bool,Cpp_type>::value};
		};
		
		
		template<typename ParamWithTraits>
		int param_is_of_type(int const& lua_stack_type)
		{
			switch (lua_stack_type) 
			{
				case LUA_TBOOLEAN:
					return lua_type_is_cpp_type<typename ParamWithTraits::raw_type,LUA_TBOOLEAN>::value;
					break;
				case LUA_TNUMBER:
					return lua_type_is_cpp_type<typename ParamWithTraits::raw_type,LUA_TNUMBER>::value;
					break;
				case LUA_TSTRING:
					return lua_type_is_cpp_type<typename ParamWithTraits::raw_type,LUA_TSTRING>::value;
				default:
					return 0;
					break;
			}
		}
		
		template<typename Class,typename Param1WithTraits,typename Param1>
		struct Constructor1<Class,Param1WithTraits,Param1,1>
		{
			static int construct(lua_State* l)
			{
				int type1 = lua_type(l,1);
				if( param_is_of_type<Param1WithTraits>(type1) )
				{
					valid_construct(l);
					return 1;
				}
				return 0;
			}
			static void valid_construct(lua_State* l)
			{
				typename Param1WithTraits::pull_type p1;
				OOLUA::Member_func_helper<Param1WithTraits,Param1WithTraits::owner>::pull2cpp(l,p1);
				OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::type> p1_(p1);
				Class* obj = new Class( p1_ );
				OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
				ud->gc = true;				
			}
		};
		
		template<typename Class,typename Param1WithTraits,typename Param2WithTraits>
		struct Constructor2;
		
		template<typename Class,typename Param1WithTraits,typename Param2WithTraits>
		struct Constructor2
		{
			static int construct(lua_State* l)
			{
				int type1 = lua_type(l,1);
				int type2 = lua_type(l,2);
				
				if(param_is_of_type<Param1WithTraits>(type1) 
				   && param_is_of_type<Param2WithTraits>(type2) )
				{	
					valid_construct(l);
					return 1;
				}
				
				return 0;
			}
			static void valid_construct(lua_State* l)
			{
				typename Param1WithTraits::pull_type p1;
				typename Param2WithTraits::pull_type p2;
				
				OOLUA::Member_func_helper<Param2WithTraits,Param2WithTraits::owner>::pull2cpp(l,p2);
				OOLUA::Member_func_helper<Param1WithTraits,Param1WithTraits::owner>::pull2cpp(l,p1);

				OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::type> p1_(p1);
				OOLUA::Converter<typename Param2WithTraits::pull_type,typename Param2WithTraits::type> p2_(p2);
				Class* obj = new Class( p1_,p2_ );
				OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
				ud->gc = true;
			}
		};
	}
}
#include "lua_stack_dump.h"

#define OOLUA_CONSTRUCTORS_BEGIN \
static int oolua_factory_function(lua_State* l) \
{\
	lua_remove(l, 1);/*remove class type*/ \
	int stack_count = lua_gettop(l);\
	if(stack_count == 0 )\
	{\
		return CREATE::Constructor<class_,has_typedef<this_type, No_default_constructor>::Result>::construct(l);\
	}

#define OOLUA_CONSTRUCTOR_1(param1Type)\
	if(stack_count == 1)\
	{\
		if( OOLUA::CREATE::Constructor1<class_,param_type<param1Type>,param1Type,param_type<param1Type>::is_integral>::construct(l) ) return 1;\
	}

#define OOLUA_CONSTRUCTOR_2(param1Type,param2Type)\
	if(stack_count == 2)\
	{\
		if( OOLUA::CREATE::Constructor2<class_,param_type<param1Type>,param_type<param2Type> >::construct(l) ) return 1;\
	}

#define OOLUA_CONSTRUCTORS_END\
	luaL_error(l,"%s %s %s","Could not match constructor for type",class_name);\
	return 0;\
}

/*
 
LUA_TBOOLEAN
LUA_TNUMBER
LUA_TSTRING
LUA_TUSERDATA
 
LUA_TNIL
LUA_TBOOLEAN
LUA_TLIGHTUSERDATA
LUA_TNUMBER
LUA_TSTRING
LUA_TTABLE
LUA_TFUNCTION
LUA_TUSERDATA
LUA_TTHREAD
*/


#endif





