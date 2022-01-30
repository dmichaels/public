// $Header:   Q:/views/win/vcs/sysmenu.h_v   1.6   Mar 04 1997 16:33:08   gregm  $ 

//	sysmenu.h
//
//	VSysMenu interface
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
//	09/29/95 dgm	Added default argument to minSet() to allow caller to
//					specify that the "Size" menu-item should be kept too.
// ----------------------------------------------------------------------------

#ifndef VSYSMENU_H
#define VSYSMENU_H

#include "popupmen.h"

CLASS  VView;

//  The VSysMenu class embodies the system menu associated with the setting of 
//  the close box on a VView object.  A VSysMenu object is constructed by providing a 
//  reference to the VView object containing the 'close' box.  The method minSet()
//  removes all functions from a VSysMenu object except for 'Move' and 'Close'.  This is used 
//  by VDialog objects to provide a restricted 'system menu' in support of SAA/CUA standards. 
//  The reset() method can then be used to set a VSysMenu object back to its default contents.

CLASS VSysMenu : public VPopupMenu {
	protected:
	boolean set();
	public:
	VClass *iam();

	VSysMenu(VView *);
	VSysMenu();
	~VSysMenu();
	void reset();
	boolean minSet(boolean = FALSE);
};

extern VClass *VSysMenuCls;

#endif // VSYSMENU_H
