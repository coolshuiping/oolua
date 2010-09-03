/*
 *  expose_shared_ptr.cpp
 *  test.unit.using_exceptions
 *
 *  Created by Liam Devine on 02/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "expose_shared_ptr.h"

EXPORT_OOLUA_FUNCTIONS_NON_CONST(Stub1_shared_ptr)
EXPORT_OOLUA_FUNCTIONS_CONST(Stub1_shared_ptr)

EXPORT_OOLUA_FUNCTIONS_NON_CONST(Stub_for_shared_ptr)
EXPORT_OOLUA_FUNCTIONS_CONST(Stub_for_shared_ptr,method)

EXPORT_OOLUA_FUNCTIONS_NON_CONST(Shared_ptr_stub)
EXPORT_OOLUA_FUNCTIONS_CONST(Shared_ptr_stub)
