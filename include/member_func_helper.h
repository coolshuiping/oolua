///////////////////////////////////////////////////////////////////////////////
///  @file member_func_helper.h
///  @author Liam Devine
///  \copyright
///  See licence.txt for more details.
///////////////////////////////////////////////////////////////////////////////
#ifndef MEMBER_FUNC_HELPER_H_
#	define MEMBER_FUNC_HELPER_H_

#	include "lua_includes.h"
#	include "fwd_push_pull.h"
#	include "oolua_error.h"

#	include <cassert>
#	include "push_pointer_internal.h"

namespace OOLUA
{

	namespace
	{
		template<typename Raw, typename T, int is_by_value = 0 >
		struct proxy_maybe_by_ref
		{
			static void push(lua_State* const s, T& value, Owner owner)
			{
				OOLUA::INTERNAL::push_pointer_which_has_a_proxy_class<Raw>(s, &value, owner);
			}
		};

		template<typename Raw, typename T>
		struct proxy_maybe_by_ref<Raw, T, 1>
		{
			static void push(lua_State* const s, T& value, Owner /*owner*/)
			{
				Raw* ptr = new Raw(value);
				OOLUA::INTERNAL::push_pointer_which_has_a_proxy_class<Raw>(s, ptr, Lua);
			}
		};


		template<typename T, typename WT, int is_integral = 1>
		struct maybe_integral
		{
			static void push(lua_State* const s, T& value)
			{
				OOLUA::push(s, value);
			}
		};

		//user defined type
		template<typename T, typename WT>
		struct maybe_integral<T, WT, 0>
		{
			static void push(lua_State* const s, T& value)
			{
				proxy_maybe_by_ref<typename WT::raw, T, WT::is_by_value>::push(s, value, (Owner)WT::owner);
			}
		};
	} // namespace //NOLINT



	namespace INTERNAL
	{

		template<typename TypeWithTraits, int owner>struct Member_func_helper;

		/*
		Specialisation for the maybe_null type.
		If NULL it pushes nil to the stack else calls the normal helper static function.
		*/
		template<typename MaybeNullType>
		struct Member_func_helper<maybe_null<MaybeNullType>, No_change>
		{
			static void push2lua(lua_State* l, MaybeNullType ptr)
			{
				if (ptr)
					Member_func_helper<function_return<MaybeNullType>, No_change>::push2lua(l, ptr);
				else
					lua_pushnil(l);
			}
		};

		/*
		Specialisation for the lua_maybe_null type.
		If NULL it pushes nil to the stack else calls the normal helper static function.
		*/
		template<typename MaybeNullType>
		struct Member_func_helper<lua_maybe_null<MaybeNullType>, Lua>
		{
			static void push2lua(lua_State* l, MaybeNullType ptr)
			{
				if (ptr)
					Member_func_helper<lua_return<MaybeNullType>, Lua>::push2lua(l, ptr);
				else
					lua_pushnil(l);
			}
		};

		template<typename TypeWithTraits>
		struct Member_func_helper<TypeWithTraits, No_change>
		{
			template<typename T>
			static void get(lua_State* const s, int idx, T& value)
			{
				OOLUA::INTERNAL::LUA_CALLED::get(s, idx, value);
			}

			template<typename T>
			static void get(int& idx, lua_State* const s, T& value)
			{
				get(s, idx, value);
				++idx;
			}

			static void get(lua_State* const s, int /*idx*/, lua_State*& l)
			{
				typedef char type_has_to_be_calling_lua_state[LVD::is_same<param_type<calling_lua_state>, TypeWithTraits>::value ? 1 : -1];
				l = s;
			}

			static void get(int& /*idx*/, lua_State* const s, lua_State*& l)
			{
				typedef char type_has_to_be_calling_lua_state[LVD::is_same<param_type<calling_lua_state>, TypeWithTraits>::value ? 1 : -1];
				l = s;
			}

			template<typename T>
			static void push2lua(lua_State* const s, T& value)
			{
				maybe_integral<T, TypeWithTraits, TypeWithTraits::is_integral>::push(s, value);
			}

			template<typename T>
			static void push2lua(lua_State* const s, T*& value)
			{
				OOLUA::push(s, value, No_change);
			}

			///special case "T* const" and "T const * const"
			template<typename T>
			static void push2lua(lua_State* const s, T*const& value)
			{
				OOLUA::push(s, value, No_change);
			}

			static void push2lua(lua_State* const , lua_State *& )
			{
				assert(0 && "this function should not be called");
			}
		};

		template<typename TypeWithTraits>
		struct Member_func_helper<TypeWithTraits, Cpp>
		{
			template<typename T>
			static void get(lua_State* const s, int idx, T*& value)
			{
				OOLUA::cpp_acquire_ptr<typename TypeWithTraits::type > p;
				OOLUA::INTERNAL::LUA_CALLED::get(s, idx, p);
				value = p.m_ptr;
			}

			template<typename T>
			static void get(int& idx, lua_State* const s, T*& value)
			{
				get(s, idx, value);
				++idx;
			}

			template<typename T>
			static void push2lua(lua_State* const /*s*/, T*& /*value*/)//noop
			{
				assert(0 && "this function should never be called");
			}
		};

		template<typename TypeWithTraits>
		struct Member_func_helper<TypeWithTraits, Lua>
		{
			template<typename T>
			static void push2lua(lua_State* const s, T*& value)
			{
				OOLUA::lua_acquire_ptr<typename TypeWithTraits::type> p(value);
				OOLUA::push(s, p);
			}

			template<typename T>
			static void push2lua(lua_State* const s, T& value)
			{
				proxy_maybe_by_ref<typename TypeWithTraits::raw
								, T
								, TypeWithTraits::is_by_value>
							::push(s, value, (Owner)TypeWithTraits::owner);
			}

			template<typename T>
			static void get(lua_State* const /*s*/, int /*idx*/, T*& /*value*/)//noop
			{
				assert(0 && "this function should never be called");
			}

			template<typename T>
			static void get(int& /*idx*/,lua_State* const /*s*/,  T& /*value*/)//noop
			{
				assert(0 && "this function should never be called");
			}
		};

	} // namespace INTERNAL //NOLINT
} // namespace OOLUA

#endif
