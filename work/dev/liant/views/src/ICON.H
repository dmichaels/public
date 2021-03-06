// $Header:   Q:/views/win/vcs/icon.h_v   1.8   Mar 04 1997 16:32:28   gregm  $ 

//	icon.h
//
//	VIcon class interface
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
//	08/09/93 jmd	removed free
//	03/04/94 pmcm	STRICT: hpIcon is now HICON, not HANDLE
//	02/16/96 pkt	changed constructor to take a const char*.
// ----------------------------------------------------------------------------

#ifndef ICON_H
#define ICON_H
#include "object.h"
#include "wintypes.h"

//	Introduction
//
//	An VIcon represents a set of small bitmaps that are used primarily
//  as a pictoral representation of an application. VIcon objects are also
//  used by the VIconButton class to form graphical push buttons for use in
//  an application\'s dialogs, tool bars, and button ribbons.  An VIcon object
//  is given to an application\'s top level VAppView object through the
//  setAppIcon() member of VAppView.  Hence, when that application is minimized, 
//  that icon will appear at the bottom of the screen.  Additonally, an VIcon
//  object is provided to an VIconButton object either with its constructor or
//  with its setIcon() member.  The drawIcon() member of the VPort class
//  can be used to draw VIcon objects on VDisplay objects. VIcon objects are
//  constructed from the name of the resource (or file) containing the icon
//  description.  These icons are typically designed with VIcon Editors that are
//  often part of the underlying GUI system.


CLASS VIcon : public VObject {

	protected:

	HICON  hpIcon;
	public:

	VIcon(const char*);
	VIcon();
	~VIcon();

	HICON handle() { return(hpIcon); }
	void sizeOfIcon(int*, int*);
	VClass *iam();

};

extern VClass *VIconCls;
#endif
