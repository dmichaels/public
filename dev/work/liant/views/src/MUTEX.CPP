// $Header:   Q:/views/common/vcs/mutex.cpv   1.2   May 30 1997 17:15:54   scottt  $

//	mutex.cpp
//
//	VMutex implementation
//
//	Allegris Foundation 1.1.00 [Common]
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	03/13/97 tlf	Original.  This code was split out of cvthread.cpp.
//	--------------------------------------------------------------------------

#include "defs.h"
#include "mutex.h"



VMutex::VMutex(void) {
	VMUT_INIT_LOCK(currentLock);
}

VMutex::~VMutex(void) {
	VMUT_DELETE_LOCK(currentLock);
}

void VMutex::lock(void) {
	VMUT_LOCK(currentLock);
}

void VMutex::unlock(void) {
	VMUT_UNLOCK(currentLock);
}
