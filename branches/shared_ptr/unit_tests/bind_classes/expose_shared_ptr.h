#ifndef OOLUA_EXPOSE_SHARED_PTR_H_
#define OOLUA_EXPOSE_SHARED_PTR_H_


#include "oolua.h"
#include "expose_stub_classes.h"

#	include "/usr/local/include/boost/shared_ptr.hpp"

namespace OOLUA
{
	typedef boost::shared_ptr<Stub1> Stub1_shared_ptr;
}

OOLUA_PROXY_CLASS(Stub1_shared_ptr,Stub1)
	OOLUA_NO_TYPEDEFS
	OOLUA_ONLY_DEFAULT_CONSTRUCTOR
OOLUA_CLASS_END

struct Stub_for_shared_ptr{ int method()const{return 1;} };

OOLUA_PROXY_CLASS(Stub_for_shared_ptr)
	OOLUA_NO_TYPEDEFS
	OOLUA_ONLY_DEFAULT_CONSTRUCTOR
	OOLUA_MEM_FUNC_CONST(int,method)
OOLUA_CLASS_END

namespace OOLUA
{
	typedef boost::shared_ptr<Stub_for_shared_ptr> Shared_ptr_stub;
}

OOLUA_PROXY_CLASS(Shared_ptr_stub,Stub_for_shared_ptr)
	OOLUA_NO_TYPEDEFS
	OOLUA_ONLY_DEFAULT_CONSTRUCTOR
OOLUA_CLASS_END

#endif

