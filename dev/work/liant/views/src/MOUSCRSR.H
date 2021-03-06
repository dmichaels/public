// $Header:   Q:/views/win/vcs/mouscrsr.h_v   1.9   Mar 04 1997 16:32:40   gregm  $ 

//	mouscrsr.h
//
//	VMouseCursor interface
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
//	10/27/92 jmd	added new Size mouse cursors
//	08/09/93 jmd	removed free
//	08/12/93 pat	added extern VClass *VMouseCursorCls, and iam().
//	03/05/94 pat	preened.
//	03/05/94 pat	STRICT:  hpCursor is now HCURSOR, not HANDLE.
// ----------------------------------------------------------------------------

#ifndef VMOUSCRSR_H
#define VMOUSCRSR_H

#include "object.h"
#include "winclass.h"

enum CursorShape { Arrow, Cross, FourArrow, Ibeam, UpArrow, HourGlass,
				   SizeNWSE, SizeNESW, SizeNS, SizeWE, 
				   AppStarting, Icon, No, Help };

extern VClass *VMouseCursorCls;

CLASS VWindow;

//	The VMouseCursor represents the visual appearance of the GUI
//	system's pointing device on the display screen.  VMouseCursor 
//	objects can	be constructed from a set of predefined shapes defined 
//	in enum CursorShape	as: 
//	Arrow, Cross, FourArrow, Ibeam, 
//	UpArrow, and HourGlass. 
//	Additionally, user defined VMouseCursor objects can be constructed from a
//	system resource through the VMouseCursor(char *) constructor.
	

CLASS VMouseCursor : public VObject {

public:

				VMouseCursor();
				VMouseCursor(CursorShape);
				VMouseCursor(char*);
				~VMouseCursor();

	VClass 	   *iam();

	void 		getCursorPos(int*, int*);
	void 		setCursorPos(int, int);
	void 		showCursor(boolean b);
	void 		show();
	void 		hide();
	void 		attachTo(VWindow *win);
	VWindow    *getWin() const;
	HCURSOR		getHndl() const;

protected:

	HCURSOR		hpCursor;
	VWindow    *cursWin;
	boolean		idcCursor;

};

// ---------------------------------------------------------------------------
inline void VMouseCursor::showCursor(boolean b) 
{ 
	ShowCursor(b); 
}
// ---------------------------------------------------------------------------
inline void VMouseCursor::show() 
{ 
	showCursor(TRUE); 
}
// ---------------------------------------------------------------------------
inline void VMouseCursor::hide() 
{ 
	showCursor(FALSE); 
}
// ---------------------------------------------------------------------------
inline void VMouseCursor::attachTo(VWindow *win) 
{ 
	cursWin = win; 
}
// ---------------------------------------------------------------------------
inline VWindow *VMouseCursor::getWin() const
{ 
	return cursWin;  
}
// ---------------------------------------------------------------------------
inline HCURSOR VMouseCursor::getHndl() const
{ 
	return hpCursor; 
}
// ---------------------------------------------------------------------------

#endif 	//	VMOUSCRSR_H
