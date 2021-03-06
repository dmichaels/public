// $Header:   Q:/views/win/vcs/mdiappvw.cpv   1.15   Mar 04 1997 17:55:18   gregm  $ 

//	mdiappvw.cpp
//
//	VMdiAppView implementation
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
//	04/07/93 pat	changed id to VObject *
//	06/25/93 jmd	preened, removed free()
//	08/02/93 pat 	updated call to VMenu() re:build flag
//	08/08/93 jmd	made VFrame const
//	09/01/93 dgm	added "Main".
//	09/14/93 dgm	added isClosing().
//	09/17/93 dgm	hide the main window in close() so we
//					don't get so much flashing on the way out.
//	02/09/94 dgm	override showBool() to make sure all VPopupWindow
//					children get hidden when hiding the VMdiAppView.
//	03/26/94 pat	CV_WIN32: updated WM_MDISETMENU handling
//	03/26/94 pat	CV_WIN32: cast appropriately for SendMessage().
//	08/09/94 dgm	Added archiver support; added operator();
//					add do_realize to VMdiAppView(); added initialize().
//	10/25/94 dgm	Fixes in getFrom().
//	11/04/94 dss	close(): removed PostQuit().
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
//	10/17/95 dss	Added client style archiving.
//	11/01/95 dgm	Changed activate() signature.
//					Added #ifndef CV_NOOLE's.
//	11/17/95 dss	Added null check in activate() (found by dgm).
//	11/20/95 dss	activate(): check if view isA VMdiViewCls before
//					SendMessage.
//	01/09/96 dss	VComponent name change.
//	12/13/96 dgm	Added vectorEvent() call to close().
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "mdiappvw.h"
#include "notifier.h"
#include "popupmen.h"
#include "mdiview.h"
#include "ordcllct.h"
#include "iterator.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "callback.h"
#endif

// Compound Document Objects
#ifndef CV_NOOLE
#include "cmpappvw.h"
#endif

// ---------------------------------------------------------------------------
//
defineClass(VMdiAppView, VAppView)
defineArchiveRevision(VMdiAppView, 0)

// ---------------------------------------------------------------------------
extern VClass *VPopupWindowCls;

// ---------------------------------------------------------------------------
// Construct an VMdiAppView with default size.
//
VMdiAppView::VMdiAppView(int do_realize)
	: VAppView(0)
{ 
	initialize();
	if (do_realize) {
		(*this)(VFrame(0.05F, 0.05F, 0.85F, 0.85F),
				StyleHorizontal + StyleVertical, "Main");
	}
}

// ---------------------------------------------------------------------------
// Construct an VMdiAppView with the VFrame 'frame'.
//
VMdiAppView::VMdiAppView(const VFrame& frame)
	: VAppView(0)
{
	initialize();
	(*this)(frame, StyleHorizontal + StyleVertical, "Main");
}

// ---------------------------------------------------------------------------
// Construct an VMdiAppView with the VFrame 'myFrame', whose
// MDI client window has the VFrame 'clientFrame' and optional client
// style 'clientStyle'.  The 'clientStyle' may be set to StyleBorder, 
// StyleHorizontal, or StyleVertical.  By default, the 'clientStyle' argument
// is set to StyleHorizontal|StyleVertical which will cause scroll bars
// to appear when the MDI frame window is resized so that all of the MDI child
// views are not visible.  To override this style, set 'clientStyle' to
// StyleDefault or StyleBorder.
//
VMdiAppView::VMdiAppView(const VFrame& frame, const VFrame& client_frame,
						 const VStyle& client_style)
	: VAppView(0)
{
	initialize();
	(*this)(frame, client_frame, client_style, "Main");
}

// ---------------------------------------------------------------------------
//
VMdiAppView::VMdiAppView(const VString& name,
						 const VFrame& frame, const VFrame& client_frame,
						 const VStyle& client_style)
	: VAppView(0)
{
	initialize();
	(*this)(frame, client_frame, client_style, name);
}

// ---------------------------------------------------------------------------
//
VMdiAppView::~VMdiAppView()
{
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::operator()(const VFrame& frame, const VStyle& client_style)
{
	VAppView::operator()(frame, StyleDefault);
	client(VFrame(1.0F, 1.0F), this, client_style);
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::operator()(const VFrame& frame, const VFrame& client_frame,
							 const VStyle& client_style)
{
	VAppView::operator()(frame, StyleDefault);
	client(client_frame, this, client_style);
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::operator()(const VFrame& frame, const VStyle& client_style,
							 const VString& name)
{
	VAppView::operator()(frame, StyleDefault, name);
	client(VFrame(1.0F, 1.0F), this, client_style);
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::operator()(const VFrame& frame, const VFrame& client_frame,
							 const VStyle& client_style, const VString& name)
{
	VAppView::operator()(frame, StyleDefault, name);
	client(client_frame, this, client_style);
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::initialize()
{
	closing = FALSE;
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::getInfo(VWinInfo& info)
{
	VAppView::getInfo(info);
	info.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
}

// ---------------------------------------------------------------------------
// Add 'm' to the menubar of this VMdiAppView where 'm' is of class
// VMenuItem or VPopupMenu.  Construct the VMenu object if it is not already
// constructed.
//
void VMdiAppView::addPopup(VObject *m)
{
	HMENU hm;

	if (!menu) {
		menu = new VMenu(TRUE);
		menu->setApp(this);
		menu->addItem(m);
		if ((VPopupMenu *)m == client.getMdiWinMenu()) {
			hm = ((VPopupMenu *)m)->hmenu();
		}
		else {
			hm = 0;
		}
#if defined(CV_WIN32)
		SendMessage(client.hwnd(), WM_MDISETMENU,
			(WPARAM) menu->hmenu(), (LPARAM) hm);
#else
		SendMessage(client.hwnd(), WM_MDISETMENU, 0, 
			MAKELONG(menu->hmenu(), hm));
#endif
	}
	else {
		menu->addItem(m);
		if ((VPopupMenu *)m == client.getMdiWinMenu()) {
#if defined(CV_WIN32)
			SendMessage(client.hwnd(), WM_MDISETMENU, 
				(WPARAM) menu->hmenu(), (LPARAM) ((VPopupMenu *)m)->hmenu());
				
#else
			SendMessage(client.hwnd(), WM_MDISETMENU, 0, 
				MAKELONG(menu->hmenu(),  ((VPopupMenu *)m)->hmenu()));
#endif
		}
	}

	if (menuUpdate) {
		drawMenu();  
	}
}

// ---------------------------------------------------------------------------
// Activate the VMdiView child window object mdichild.
//
boolean VMdiAppView::activate(VView *mdichild)
{
	if (mdichild != 0 && mdichild->isA(VMdiViewCls)) {
		SendMessage(client.hwnd(), WM_MDIACTIVATE,
					WPARAM(mdichild->hwnd()), LPARAM(0));
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Called by the VNotifier when the application window is to be
// closed. An VMdiAppView object will typically check with the user, by
// having its close() method poll all of the Mdi Child Views, to see
// if it is OK to close.  When an VMdiAppView object
// is closed, the application is terminated. 
//
boolean VMdiAppView::close()
{
	closing = TRUE;
	
#ifndef CV_NOOLE
	if (getComponentThis()) {
		// if CDThis overrides close, then return abort close.
		if (getComponentThis()->close()) {
			return TRUE;
		}
	}
#endif
	if (vectorEvent(VWindow::Quit)) {
		return TRUE;
	}

	return quit();
}

// ---------------------------------------------------------------------------
boolean VMdiAppView::quit()
{
	closing = TRUE;
	
#ifndef CV_NOOLE
	if (getComponentThis()) {
		// if CDThis overrides quit, then return abort close.
		if (getComponentThis()->quit()) {
			return TRUE;
		}
	}
#endif

	return client.quit();
}
// ---------------------------------------------------------------------------
//
void VMdiAppView::showBool(boolean do_show, boolean do_update)
{
	VAppView::showBool(do_show, do_update);

	// When an VMdiAppView is hidden/shown, all of its
	// VMdiView children are automatically also hidden/shown,
	// but VPopupWindow children are not, so we do it here.

	VOrdCollect *children = this->childrenOf();
	if (children != 0) {
		DO (*children, VWindow, child)
			if ((child != 0) && child->isA(VPopupWindowCls)) {
				child->showBool(do_show, do_update);
			}
		END
	}
}

// ---------------------------------------------------------------------------
// Return the WinMdiApp window type attribute.
//
enum WinType VMdiAppView::winType()
{
	return WinMdiApp;
}

// ---------------------------------------------------------------------------
//
boolean VMdiAppView::nextChildFocus(VWindow *child)
{
//	return clientOf()->nextChildFocus(child);
	return VAppView::nextChildFocus(child);
}

// ---------------------------------------------------------------------------
//
boolean VMdiAppView::prevChildFocus(VWindow *child)
{
//	return clientOf()->prevChildFocus(child);
	return VAppView::prevChildFocus(child);
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::setChildFocus(VWindow *child)
{
//	clientOf()->setChildFocus(child);
	VAppView::setChildFocus(child);
}

// ---------------------------------------------------------------------------
//
VWindow *VMdiAppView::getChildFocus()
{
//	return clientOf()->getChildFocus();
	return VAppView::getChildFocus();
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VMdiAppView::putTo(VArchiver& a)
{
	VMdiAppView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::putTo(VArchiver& a, boolean most_derived)
{
	client.setArchiveEnable(FALSE);

	// Write the base object data.

	VAppView::putTo(a, FALSE);

	// Write the object version tag.

	a << VMdiAppViewArchiveTag;

	// Write any object specific data.
	
	VFrame client_frame;
	client.getFrame(client_frame);
	a.putObject(&client_frame);

	a.putObject(&client.getStyle());

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::getFrom(VArchiver& a)
{
	// Read the base object data.

	VAppView::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VMdiAppViewArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VMdiAppViewArchiveTag));
		return;
	}

	// Read any object specific data.

	VFrame client_frame;
	a.getObject(&client_frame);
	client.setFrame(client_frame);

	a.getObject(&client.getStyle());
}

// ---------------------------------------------------------------------------
//
void VMdiAppView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VMdiAppView::getFrom(a);

#if 1

	VWindow::getChildrenFrom(a, FALSE);

	if (do_realize) {
		VMdiAppView::realize((VWindow *)parent);
//		VWindow::realizeChildren();
	}

#else
	if (do_realize) {
		VMdiAppView::realize((VWindow *)parent);
	}
	VWindow::getChildrenFrom(a, do_realize);
#endif

}

// ---------------------------------------------------------------------------
//
void VMdiAppView::realize(VWindow *parentw)
{
	VAppView::realize(parentw);
	VFrame client_frame;
	client.getFrame(client_frame);
	client(client_frame, this, client.style);
}

#endif // ARCHIVER
