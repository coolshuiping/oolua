#ifndef OOLUA_CPP_CONSTRUCTOR_H_
#	define OOLUA_CPP_CONSTRUCTOR_H_

///////////////////////////////////////////////////////////////////////////////
///  @file cpp_constructor.h
///  @remarks Warning this file was generated, edits to it will not persist if 
///  the file is regenerated.
///  @author Liam Devine
///  @email
///  See http://www.liamdevine.co.uk for contact details.
///  @licence 
///  See licence.txt for more details. \n 
///////////////////////////////////////////////////////////////////////////////
#	include "lua_includes.h"
#	include "oolua_storage.h"
#	include "param_traits.h"
#	include "oolua_userdata.h"
#	include "member_func_helper.h"
#	include "oolua_parameter_helper.h"
namespace OOLUA
{
namespace INTERNAL
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
template<typename Class,typename Param1WithTraits >
struct Constructor1
{
	static int construct(lua_State* l) 
	{
		if(param_is_of_type<Param1WithTraits >(l,1))
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
		Class* obj = new Class( p1_);
		OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
		ud->gc = true;
	}
};
template<typename Class,typename Param1WithTraits, typename Param2WithTraits >
struct Constructor2
{
	static int construct(lua_State* l) 
	{
		if(param_is_of_type<Param1WithTraits >(l,1)
			&& param_is_of_type<Param2WithTraits >(l,2))
		{
			valid_construct(l);
			return 1;
		}
		return 0;
	}
	static void valid_construct(lua_State* l)
	{
		typename Param2WithTraits::pull_type p2;
		OOLUA::Member_func_helper<Param2WithTraits,Param2WithTraits::owner>::pull2cpp(l,p2);
		OOLUA::Converter<typename Param2WithTraits::pull_type,typename Param2WithTraits::type> p2_(p2);
		typename Param1WithTraits::pull_type p1;
		OOLUA::Member_func_helper<Param1WithTraits,Param1WithTraits::owner>::pull2cpp(l,p1);
		OOLUA::Converter<typename Param1WithTraits::pull_type,typename Param1WithTraits::type> p1_(p1);
		Class* obj = new Class( p1_,p2_);
		OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::add_ptr(l,obj,false);
		ud->gc = true;
	}
};

}
}
#define OOLUA_CONSTRUCTORS_BEGIN \
static int oolua_factory_function(lua_State* l) \
{ \
	lua_remove(l, 1);/*remove class type*/ \
	int stack_count = lua_gettop(l); \
	if(stack_count == 0 ) \
	{ \
		return OOLUA::INTERNAL::Constructor<class_,has_typedef<this_type, No_default_constructor>::Result>::construct(l); \
	}

#define OOLUA_CONSTRUCTOR_1(param1Type) \
	if(stack_count == 1) \
	{ \
		if(OOLUA::INTERNAL::Constructor1<class_,param_type<param1Type > >::construct(l) ) return 1; \
	}

#define OOLUA_CONSTRUCTOR_2(param1Type,param2Type) \
	if(stack_count == 2) \
	{ \
		if(OOLUA::INTERNAL::Constructor2<class_,param_type<param1Type >,param_type<param2Type > >::construct(l) ) return 1; \
	}

#define OOLUA_CONSTRUCTORS_END \
	luaL_error(l,"%s %d %s %s","Could not match",stack_count,"parameter constructor for type",class_name); \
	return 0; \
}

#define OOLUA_ONLY_DEFAULT_CONSTRUCTOR \
OOLUA_CONSTRUCTORS_BEGIN \
OOLUA_CONSTRUCTORS_END



#endif 
