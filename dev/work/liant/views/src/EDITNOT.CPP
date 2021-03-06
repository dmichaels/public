// $Header $

//  editnot.cpp
//
//	VEditLineNotify implementation. [Common]
//
//	Allegris Foundation 1.1.00
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
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

#include "editnot.h"
#include "str.h"
#include "notifier.h"

extern "C" {
#   include <string.h>
}

defineClass(VEditLineNotify, VEditLine)

// ---------------------------------------------------------------------------
//  Construct a VEditLineNotify with no presentation space.
//
VEditLineNotify::VEditLineNotify()
{

}

VEditLineNotify::VEditLineNotify(const VFrame& frame, VWindow *win, const VStyle& style)
{
    useTabs = FALSE;
    (*this)(frame, win, style);
}

// ---------------------------------------------------------------------------
//  Destroy this VEditLine.
//
VEditLineNotify::~VEditLineNotify()
{
    ;
}
// ------------------------------------------------------------
boolean VEditLineNotify::enterHit()
{
   if ((acceptClient != 0) && (acceptMethod != 0)) {
     acceptClient->perform(acceptMethod, (long) this);
   }

   return TRUE;
}

//-------------------------------------------------------------
boolean VEditLineNotify::escapeHit()
{
   if ((cancelClient != 0) && (cancelMethod != 0)) {
     cancelClient->perform(cancelMethod, (long) this);
   }

   return TRUE;
}

 //-------------------------------------------------------------
boolean VEditLineNotify::clearFocus()
{
   enterHit();

   return FALSE;
}
//-------------------------------------------------------------
void VEditLineNotify::uponAccept(VObject* clnt, method mthd)
{
   acceptClient = clnt;
   acceptMethod = mthd;
}

//-------------------------------------------------------------
void VEditLineNotify::uponCancel(VObject* clnt, method mthd)
{
   cancelClient = clnt;
   cancelMethod = mthd;
}

