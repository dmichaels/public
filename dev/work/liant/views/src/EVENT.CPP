// $Header:   Q:/views/win/vcs/event.cpv   1.5   Mar 04 1997 17:55:04   gregm  $ 

//	event.cpp
//
//	VEvent implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
// ----------------------------------------------------------------------------

#include "event.h"

// ----------------------------------------------------------------------------
//	Construct an `VEvent` object.
//
VEvent::VEvent() : msg(0), wParam(0), lParam(0), returnValue(0)
{

}

// ----------------------------------------------------------------------------
//
VEvent::~VEvent()
{
	;
}


// ----------------------------------------------------------------------------
//	Construct an `VEvent` object with MS Windows message number 'm', 
//	wParam 'w', and lParam 'l'.
//
VEvent::VEvent(unsigned m, WPARAM w, LPARAM l) : msg(m), wParam(w), lParam(l), returnValue(0)
{

}

