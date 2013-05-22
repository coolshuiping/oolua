///////////////////////////////////////////////////////////////////////////////
///  @file oolua_c_func.h
///  @author Liam Devine.
///  \copyright 
///  See licence.txt for more details. \n 
///////////////////////////////////////////////////////////////////////////////
#ifndef OOLUA_C_FUNC_H_
#	define OOLUA_C_FUNC_H_

#	include "param_traits.h"
#	include "oolua_paramater_macros.h"
#	include "proxy_caller.h"
#	include "default_trait_caller.h"
#	include "oolua_config.h"

//* \cond INTERNAL*/

#if GCC_VA_ARGS_SYSTEM_HEADER_OOLUA == 1
#	pragma GCC system_header
#else
	GCC_PUSH_DISABLE_VA_WARNINGS_OOLUA
#endif

//proxy implementations
#define OOLUA_C_FUNCTION_N(return_value,func, ...) \
	OOLUA_PARAMS_CONCAT(__VA_ARGS__) \
	typedef OOLUA::INTERNAL::return_type_traits<return_value > R;\
	typedef R::type (funcType)( OOLUA_PARAM_TYPE_CONCAT(__VA_ARGS__) ) ;\
	OOLUA::INTERNAL::Proxy_none_member_caller<R,LVD::is_void< R::type >::value >::call<OOLUA_TPARAMS_CONCAT(__VA_ARGS__) funcType>(l,&func OOLUA_PPARAMS_CONCAT(__VA_ARGS__));\
	OOLUA_BACK_CONCAT(__VA_ARGS__) \
	return OOLUA::INTERNAL::lua_return_count< Type_list<R OOLUA_RETURNS_CONCAT(__VA_ARGS__) >::type> ::out;

#define OOLUA_C_FUNCTION_0(return_value,func) \
	typedef OOLUA::INTERNAL::return_type_traits<return_value > R;\
	typedef R::type (funcType)() ;\
	OOLUA::INTERNAL::Proxy_none_member_caller<R,LVD::is_void< R::type >::value >::call<funcType>(l,&func);\
	return OOLUA::INTERNAL::lua_return_count< Type_list<R >::type> ::out;


#define OOLUA_CFUNC_RETURN(Name)							return OOLUA::INTERNAL::proxy_c_function_with_default_traits(l,Name);

#define OOLUA_CFUNC_INTERNAL_2(FunctionName,ProxyName)		int ProxyName(lua_State* l) {  OOLUA_CFUNC_RETURN(FunctionName) }
#define OOLUA_CFUNC_INTERNAL_1(FunctionName)				OOLUA_CFUNC_INTERNAL_2(FunctionName,FunctionName)

#define OOLUA_STATIC_FUNC_INTERNAL_2(FunctionName,ProxyName)static OOLUA_CFUNC_INTERNAL_2(class_::FunctionName,ProxyName)
#define OOLUA_STATIC_FUNC_INTERNAL_1(FunctionName)			OOLUA_STATIC_FUNC_INTERNAL_2(FunctionName,FunctionName) 

/** \endcond*/

GCC_POP_VA_WARNINGS_OOLUA

#endif 
