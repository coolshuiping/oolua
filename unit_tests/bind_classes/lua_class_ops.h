#ifndef LUA_CLASS_OPS_H_
#	define LUA_CLASS_OPS_H_

#include "cpp_class_ops.h"
#include "oolua.h"

/*[ClassOpsExpose]*/
OOLUA_PROXY(Class_ops)
	OOLUA_TAGS(
		Equal_op,
		Less_op,
		Less_equal_op,
		Add_op,
		Sub_op,
		Mul_op,
		Div_op
	)
	OOLUA_CTORS()
	OOLUA_MFUNC_CONST(geti)
OOLUA_PROXY_END
/*[ClassOpsExpose]*/

#endif
