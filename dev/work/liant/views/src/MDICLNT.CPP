// $Header:   Q:/views/win/vcs/mdiclnt.cpv   1.7   Mar 04 1997 17:55:20   gregm  $ 

//	mdiclnt.cpp
//
//	VMdiClient implementation
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
//	09/19/92 jmd	preened
//	06/25/93 jmd	preened
//  08/09/93 jmd	removed free
//  09/17/93 dgm	changed quit() to not assume that all the
//					children of the VMdiClient are VMdiViews.
//	03/26/94 pat	CV_WIN32: cast appropriately for SendMessage().
//	08/11/94 dgm	Set the client for the MDI "Window" pulldown menu
//					to this VMdiClient object; used by VMenu::~VMenu()
//					to prevent this speciall pulldown menu from being
//					deleted, as it is done in VMdiClient::~VMdiClient().
//	09/05/94 dss	getInfo(): exStyle initialization.
//	11/03/94 dss	Added close(). Removed quit.
// ----------------------------------------------------------------------------

#include "mdiclnt.h"
#include "mdiview.h"
#include "notifier.h"
#include "popupmen.h"
#include "winclass.h"
#include "iterator.h"

defineClass(VMdiClient, VWindow)

char   *mdiNames[] =  { "&Cascade\tShift+F5",
						"&Tile\tShift+F4",
						"&Arrange Icons",
						 0 };

method	mdiMthds[] =  { methodOf(VMdiClient, cascade),
						methodOf(VMdiClient, tile),
						methodOf(VMdiClient, arrange) };

int		mdiAccels[] = { K_Mod_Shift + K_F5,
						K_Mod_Shift + K_F4,
						K_NULL,
						K_NULL };

// ---------------------------------------------------------------------------
// Construct a `VMdiClient` with no presentation window.
//
VMdiClient::VMdiClient() :
	popup(0)
{
	;
}

// ---------------------------------------------------------------------------
void VMdiClient::getInfo(VWinInfo& info)
{
	info.style = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	info.exStyle = 0;

	if (style.contains(StyleBorder)) {
		info.style |= WS_BORDER;
	}
	if (style.contains(StyleHorizontal)) {
		info.style |= WS_HSCROLL;
	}
	if (style.contains(StyleVertical)) {
		info.style |= WS_VSCROLL;
	}
	if (!popup) {
		popup = new VPopupMenu("&Window");
		popup->addItems(mdiNames, mdiMthds, mdiAccels, this);
		popup->setClient(this);
	}
	notifier->popupMenu(popup);
}

// ---------------------------------------------------------------------------
VMdiClient::~VMdiClient()
{
	if (popup) {
		delete popup;		// jmd (was free)
		notifier->popupMenu(0);
	}
}

// ---------------------------------------------------------------------------
// Callback from the standard 'VWindow' menu to cascade all of the
// MDI child window icons.  This method may be called directly with a NIL
// argument.
//
boolean VMdiClient::cascade(VMenuItem *m)
{
	return (boolean) SendMessage(hWnd, WM_MDICASCADE, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Callback from the standard 'VWindow' menu to arrange all of the
// MDI child window icons.  This method may be called directly with a NIL
// argument.
//
boolean VMdiClient::arrange(VMenuItem *m)
{
	return (boolean) SendMessage(hWnd, WM_MDIICONARRANGE, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Callback from the standard 'VWindow' menu to tile all of the
// MDI child window icons.  This method may be called directly with a NIL
// argument.
//
boolean VMdiClient::tile(VMenuItem *m)
{
	return (boolean)SendMessage(hWnd, WM_MDITILE, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Return the current active `VMdiView` child window.
//
VMdiView  *VMdiClient::activeChild()
{
	HWND hChildWin = (HWND) SendMessage(hWnd, WM_MDIGETACTIVE, (WPARAM) 0, 
																(LPARAM) 0);

	if (hChildWin) {
		return (VMdiView *)notifier->getThis(hChildWin);
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
// Close all MDI child windows.
//
void VMdiClient::closeAll()
{
	if (children) {
		REVERSEDO(children, VMdiView, child)
			delete child;	// jmd (was free)
		END 
		delete children;	// jmd (was free)
		children = 0;
	}
}

// ---------------------------------------------------------------------------
// Poll all the kids to see if is all right to close. If a kid returns
// TRUE, it means abort the kid close so return TRUE to abort the
// parent (VMdiAppView) close.
//
boolean VMdiClient::quit()
{

	if (children) {
		// poll the kiddies
		DO(children, VMdiView, child)
			if (child->isA(VMdiViewCls) && child->quit()) {
				return TRUE;
			}
		END 
	}

	return FALSE;
}

// ---------------------------------------------------------------------------
// Return the `WinMdiClient` window type attribute.
//
enum WinType VMdiClient::winType() 
{ 
	return WinMdiClient;
}

// ---------------------------------------------------------------------------
// Set the Mdi `VWindow` popup menu to 'winMenu'.  This new popup menu 
// will replace the default Mdi `VWindow` popup menu produced by this `VMdiClient`
// object.  This method MUST be called before any Mdi Child windows are created
// and before any menus are added to the `VMdiAppView`\'s menu bar.
//
void VMdiClient::setMdiWinMenu(VPopupMenu *winMenu)
{
	if (popup) {
		delete popup;		// jmd (was free)
	}
	popup = winMenu;

	if (hWnd) {  // rebuild Mdi Client with new menu.
		VWindow::clean();
		build(style);
	}
}
