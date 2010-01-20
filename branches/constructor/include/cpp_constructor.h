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
		
		
		template<typename T>
		struct Is_lua_number_type
		{
			typedef Type_list<
			char,unsigned char, signed char,
			short,unsigned short, signed short,
			int,unsigned int, signed int,
			long, unsigned long, signed long, LVD::int64, LVD::uint64,
			float,
			double, long double>::type Lua_number;
			enum {value = TYPELIST::IndexOf<Lua_number,T>::value == -1 ? 0 : 1};
		};
		
		template<typename T>
		struct Is_lua_string_type
		{
			typedef Type_list<
							char*,std::string
							>::type Lua_string;
			enum {value = TYPELIST::IndexOf<Lua_string,T>::value == -1 ? 0 : 1};
		};
		
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
		
		template<typename Class,typename Param1WithTraits,typename Param1>
		struct Constructor1<Class,Param1WithTraits,Param1,1>
		{
			static int construct(lua_State* l)
			{
				int type = lua_type(l,1);
//				switch (type) {
//					case LUA_TBOOLEAN:
//						return if_valid_construct_else_return_0<LUA_TBOOLEAN>(l);
//						break;
//					case LUA_TNUMBER:
//						return if_valid_construct_else_return_0<LUA_TNUMBER>(l);
//						break;
//					case LUA_TSTRING:
//						return if_valid_construct_else_return_0<LUA_TSTRING>(l);
//						break;
//					default:
//						return 0;
//						break;
//				}
				
				if( type == LUA_TBOOLEAN && lua_type_is_cpp_type<typename Param1WithTraits::raw_type,LUA_TBOOLEAN>::value )
				{
					typename Param1WithTraits::pull_type p1;
					OOLUA::Member_func_helper<Param1WithTraits,Param1WithTraits::owner>::pull2cpp(l,p1);
					OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::type /*Param1*/> p1_(p1);
					Class* obj = new Class( p1_ );
					OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
					ud->gc = true;
					return 1;
				}
				if( type == LUA_TNUMBER  && lua_type_is_cpp_type<typename Param1WithTraits::raw_type,LUA_TNUMBER>::value )
				{
					typename Param1WithTraits::pull_type p1;
					OOLUA::Member_func_helper<Param1WithTraits,Param1WithTraits::owner>::pull2cpp(l,p1);
					OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::type /*Param1*/> p1_(p1);
					Class* obj = new Class( p1_ );
					OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
					ud->gc = true;
					return 1;
				}
				if( type == LUA_TSTRING && lua_type_is_cpp_type<typename Param1WithTraits::raw_type,LUA_TSTRING>::value )
				{
					typename Param1WithTraits::pull_type p1;
					OOLUA::pull2cpp(l,p1);
					OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::type > p1_(p1);
					Class* obj = new Class( p1_ );
					OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
					ud->gc = true;
					return 1;
				}

				return 0;
			}
			template<int LuaType>
			static int if_valid_construct_else_return_0(lua_State* l)
			{
				if( lua_type_is_cpp_type<typename Param1WithTraits::raw_type,LuaType>::value )
				{
					typename Param1WithTraits::pull_type p1;
					OOLUA::Member_func_helper<Param1WithTraits,Param1WithTraits::owner>::pull2cpp(l,p1);
					OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::typeintegral  /*Param1*/> p1_(p1);
					Class* obj = new Class( p1_ );
					OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
					ud->gc = true;
					return 1;
				}
				return 0;
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

//#define OOLUA_CONSTUCTOR_1(p1) 

#endif





