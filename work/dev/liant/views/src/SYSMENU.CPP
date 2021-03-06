// $Header:   Q:/views/win/vcs/sysmenu.cpv   1.7   Mar 04 1997 17:55:54   gregm  $ 

//	sysmenu.cpp
//
//	VSysMenu implementation
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
//	03/08/94 pat	STRICT:  fixed return in set().
//	09/29/95 dgm	Added default argument to minSet() to allow caller to
//					specify that the "Size" menu-item should be kept too.
//	08/13/96 dgm	Fixes to to minSet(); found with BoundsChecker.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "sysmenu.h"
#include "view.h"

defineClass(VSysMenu, VPopupMenu)

VSysMenu::VSysMenu()
{
	menu = 0;

	if (hMenu) {
		DestroyMenu(hMenu);
		hMenu = 0;
	}
}
/*
	Destroy the copy of the MS Windows system menu handle obtained in the
	constructor of this VSysMenu object.
*/
VSysMenu::~VSysMenu()
{
	reset();
}

/*@Method
	Construct a VSysMenu object to represent the 'system menu' associated with
VView object 'v'.
*/
VSysMenu::VSysMenu(VView *v)
{
	menu = 0;
	if (hMenu) {
		DestroyMenu(hMenu);
		hMenu = 0;
	}
	if (v) {
		app = (VAppView *)v;
		set();
	}

}

/*@Method
	Remove all items from this VSysMenu object except for the 'Move' and 'Close'
functions.  Return TRUE if successful, otherwise return FALSE.
*/
boolean VSysMenu::minSet(boolean keep_size_too)
{
	if (hMenu == 0) {
		set();
	}
	if (hMenu != 0) {
		if (!keep_size_too) {
			RemoveMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
		}
		RemoveMenu(hMenu, SC_MINIMIZE,   MF_BYCOMMAND);
		RemoveMenu(hMenu, SC_MAXIMIZE,   MF_BYCOMMAND);
		RemoveMenu(hMenu, SC_RESTORE,    MF_BYCOMMAND);
		RemoveMenu(hMenu, SC_NEXTWINDOW, MF_BYCOMMAND);
		RemoveMenu(hMenu, SC_PREVWINDOW, MF_BYCOMMAND);
		RemoveMenu(hMenu, SC_TASKLIST,   MF_BYCOMMAND);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/*@Method
	Reset this VSysMenu object to its original default contents.
*/
void VSysMenu::reset()
{
	if (hMenu && app) { 
		GetSystemMenu(((VView *)app)->hwnd(), TRUE);
		hMenu = 0;
	}
}

/*
*/
boolean VSysMenu::set()
{
	if (app != 0) {
		hMenu = GetSystemMenu(((VView *)app)->hwnd(), FALSE);
		return hMenu != 0;
	}
	else {
		return FALSE;
	}
}
