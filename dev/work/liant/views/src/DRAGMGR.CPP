// $Header:   Q:/views/common/vcs/DRAGMGR.CPv   1.2   Mar 04 1997 14:12:30   gregm  $ 

//	dragmgr.cpp
//
//	VDragManager global object definition
//
//	Allegris Foundation 1.1.00 for Windows and OS/2 Presentation Manager
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	06/23/95 dgm	Original check-in; original code from kerstinb.
//	01/07/97 djs	Added support for allegris IN_DLL
// --------------------------------------------------------------------------

#include "defs.h"
#include "drag.h"

// --------------------------------------------------------------------------
//	See comments in GLOBALS.CPP for explanation of global objects in
// Dynamic Link Libraries.
//
#ifdef DLL
   VDragManager CV_NEARDATA g_DragManager(1);
#else
   VDragManager CV_NEARDATA g_DragManager;
#endif
