// $Header:   Q:/views/win/vcs/event.h_v   1.5   Mar 04 1997 16:32:22   gregm  $ 

//	event.h
//
//	VEvent class interface
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
// ---------------------------------------------------------------------------

#ifndef EVENT_H
#define EVENT_H

#include "defs.h"
#include "winclass.h"

//  The `VEvent` class is designed to encapsulate system dependent event
//  dispatch information.  The attributes of this class will vary between the
//  various platforms with implementations of Allegris  The `VEvent` class is
//  used to pass system dependent event information to the virtual `winDispatch()`
//  member function defined in the base `VWindow` class.  For specific meaning and
//  use of the `VEvent` class, see the description of the `winDispatch()` method
//  in the `VWindow` class.


CLASS VEvent {

	public:
	long  	returnValue;
	VEvent(unsigned, WPARAM, LPARAM);
	unsigned 	msg;
	WPARAM 	wParam;
	LPARAM 	lParam;

	VEvent();
	~VEvent();
};

#endif
