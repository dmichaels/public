// $Header:   Q:/views/common/vcs/dockwin.cpv   1.38   Mar 20 1997 18:31:20   gregm  $

//	dockwin.cpp
//
//	VDockableWindow implementation [Common]
//
//  Allegris Foundation 1.1.00
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
//	11/01/95 dgm	Original.
//	11/03/95 dgm	Improved tracking of docked/undocked rubberband.
//	11/10/95 dgm	Updated.
//	11/20/95 dgm	Minor update to dragging().
//	11/20/95 dgm	Added getDockedSize().
//	11/21/95 dgm	Added uponUndockedClose().
//	11/29/95 dgm	Added beginDrag()/endDrag().
//	11/30/95 djs	OS/2 changes.
//	11/30/95 dgm	Added uponBeginDrag()/uponEndDrag() & setClientWindow().
//					Crash fix in dock().
//	12/12/95 dgm	Changed changeGeometryXXX() to setGeometryXXX().
//	12/20/95 dgm	Minor fix in undock() for StyleSimpleDock so that
//					a VDockableToolBar doesn't get disabled when undocked.
//  02/06/96 dgm    Added setUndockedSquare() and setUndockedCenter().
//					Added setDockedAttributes(), setUndockedAttributes(),
//					and getBestSquareSize() -- for internal use.
//	02/29/96 dgm	Added DockOnClose.
//	03/04/96 dgm	Added a popup menu, get/setOverDockSitePercentage() and
//					get/setOverDockSitePercentage() and other minor additions.
//	03/21/96 dgm	Archiver support.
//	04/17/96 dgm	Minor changes to suppress conversion warnings.
//	04/29/96 dgm	More archiver changes.
//	05/16/96 dgm	Fixed uponUndockedClose().
//	05/20/96 dgm	Minor fixes.
//	09/13/96 dgm	Fixed memory leak (BoundsChecker).
//	11/22/96 dgm	Fixed focus problem (unset parent's notion of
//					current child with focus when undocked if it's me).
//	01/12/97 dgm	Disable focus.
//	01/31/97 dgm	Clear lastDockSite in unregisterDockSite() if necessary.
//	03/24/97 dgm	Checks for lastDockSite being unregistered during the
//					undocking callback; add data-members doingUndocked and
//					dockSiteUnregisteredWhileDoingUndocked for this purpose.
// --------------------------------------------------------------------------

#include "dockwin.h"
#include "docksite.h"
#include "utilwin.h"
#include "geoform.h"
#include "rband.h"
#include "ordcllct.h"
#include "iterator.h"
#include "notifier.h"
#include "popupmen.h"
#include "menudata.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "cllbckls.h"
#endif

// --------------------------------------------------------------------------
//
#if defined(PM) && !defined(K_Control)
#	define K_Control VK_CTRL
#endif

// --------------------------------------------------------------------------
//
defineClass(VDockableWindow, VWindow)
defineArchiveRevision(VDockableWindow, 1)

// --------------------------------------------------------------------------
//
static unsigned long							numberOfObjects		= 0;
static VDockableWindow::ConfigurationMenu	   *configurationMenu	= 0;

// --------------------------------------------------------------------------
//
#undef  abs
#define abs(a)	(((a) < 0) ? (-(a)) : (a))

// --------------------------------------------------------------------------
//
inline int rectanglesIntersect(int x1, int y1, int w1, int h1,
							   int x2, int y2, int w2, int h2)
{
	if ((x1 > x2 + w2 - 1) || (x1 + w1 - 1 < x2) ||
		(y1 > y2 + h2 - 1) || (y1 + h1 - 1 < y2)) {
		return 0;
	}
	else {
		return 1;
	}
}

// --------------------------------------------------------------------------
//
inline int getRectangleIntersectionXY(int xy1, int wh1, int xy2, int wh2)
{
	if ((wh1 > 0) && (wh2 > 0) && (xy1 + wh1 > xy2) && (xy2 + wh2 > xy1)) {
		if (xy1 + wh1 <= xy2 + wh2) {
			if (xy1 <= xy2) {
				return xy1 + wh1 - xy2;
			}
			else {
				return wh1;
			}
		}
		else if (xy1 <= xy2) {
			return wh2;
		}
		else {
			return xy2 + wh2 - xy1;
		}
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
inline int getRectangleIntersectionArea(int x1, int y1, int w1, int h1,
										int x2, int y2, int w2, int h2)
{
	return getRectangleIntersectionXY(x1, w1, x2, w2) *
		   getRectangleIntersectionXY(y1, h1, y2, h2);
}

// --------------------------------------------------------------------------
//
static void getCenterPosition(VWindow *undocked_window, int *x, int *y)
{
	if (undocked_window != 0) {
		VWindow *parent = undocked_window->getParent();
		if (parent != 0) {
			VWindow *top_parent = parent->getTopLevelParent();
			if (top_parent != 0) {
				int uw, uh, px, py, pw, ph;
				undocked_window->sizeOfWin(&uw, &uh);
				top_parent->getAbs(&px, &py, &pw, &ph);
				if (x != 0) {
					if ((*x = (px + pw / 2) - uw / 2) < 0) { *x = 0; }
				}
				if (y != 0) {
					if ((*y = (py + ph / 2) - uh / 2) < 0) { *y = 0; }
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
VDockableWindow::VDockableWindow()
	: VWindow()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VDockableWindow::VDockableWindow(VWindow *parentwin, const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(parentwin, style);
}

// --------------------------------------------------------------------------
//
VDockableWindow::VDockableWindow(const VString& name,
								 VWindow *parentwin, const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VDockableWindow::VDockableWindow(const VFrame& frame, VWindow *parentwin,
								 const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VDockableWindow::VDockableWindow(const VString& name,
								 const VFrame& frame, VWindow *parentwin,
								 const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VDockableWindow::operator()(VWindow *parentwin)
{
	(*this)(VFrame(), parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VDockableWindow::operator()(VWindow *parentwin, const VStyle& style)
{
	(*this)(VFrame(), parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VDockableWindow::operator()(VWindow *parentwin, const VStyle& style,
								 const VString& name)
{
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VDockableWindow::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VDockableWindow::operator()(const VFrame& frame, VWindow *parentwin,
								 const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VDockableWindow::operator()(const VFrame& frame, VWindow *parentwin,
								 const VStyle& style, const VString& name)
{
	VWindow::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VDockableWindow::~VDockableWindow()
{
	if ((clientWindow != 0) && (clientWindow->dockableWindowOwner == this)) {
		clientWindow->dockableWindowOwner = 0;
	}
	if (undockedTitle != 0) {
		delete undockedTitle;
	}
	if (dockSites != 0) {
		dockSites->freeContents();
		delete dockSites;
	}
	if (--numberOfObjects == 0) {
		if (configurationMenu != 0) {
			delete configurationMenu;
			configurationMenu = 0;
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::uponBeginDrag(VObject *o, method m)
{
	clientObject = o;
	beginDragMethod = (void (VObject::*)(VDockableWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::uponEndDrag(VObject *o, method m)
{
	clientObject = o;
	endDragMethod = (void (VObject::*)(VDockableWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::uponDocked(VObject *o, method m)
{
	clientObject = o;
	dockedMethod = (boolean (VObject::*)(VDockableWindow *, VWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::uponUndocked(VObject *o, method m)
{
	clientObject = o;
	undockedMethod = (boolean (VObject::*)(VDockableWindow *, VWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::uponUndockedClose(VObject *o, method m)
{
	clientObject = o;
	undockedCloseMethod = (void (VObject::*)(VWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::undockedClosed(VWindow *)
{
	if ((clientObject != 0) && (undockedCloseMethod != 0)) {
		(clientObject->*undockedCloseMethod)(this);
	}
	else if (undockedDockOnClose && !dockingCurrentlyDisabled &&
									!currentlyDocked) {
		dock();
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::initialize()
{
	numberOfObjects++;
	clientWindow							= 0;
	undockedWindow							= 0;
	dockSites								= 0;
	currentlyDocked							= 1;
	undockedTitle							= 0;
	lastDockSite							= 0;
	lastUndockedX							= 32767;
	lastUndockedY							= 32767;
	lastUndockedWidth						= 0;
	lastUndockedHeight						= 0;
    lastUndockedSizeIsValid					= 0;
    lastUndockedStyle						= 0;
	actingAsDockSite						= 0;
	undockedLeftMargin						= 0;
	undockedRightMargin						= 0;
	undockedTopMargin						= 0;
	undockedBottomMargin					= 0;
	undockedCloseBehavior					= HideOnClose;
	undockedDockOnClose						= 0;
	undockedSquare							= 0;
	undockedCenter							= 1;
	beginDragMethod							= 0;
	endDragMethod							= 0;
	dockedMethod							= 0;
	undockedMethod							= 0;
	undockedCloseMethod						= 0;
	clientObject							= 0;
	explicitShow							= 0;
	explicitHide							= 0;
    undockedNotYetShown						= 1;
	overDockSitePercentage					= 0.0F;
	overDockSitePointer						= 0;
	configurationMenuDisabled				= 0;
	dockingCurrentlyDisabled				= 0;
    undockedNoCloseBox						= 0;
	undockedDockedStyle						= 0;
	doingUndocked							= 0;
	dockSiteUnregisteredWhileDoingUndocked	= 0;

#ifndef CV_NOARCHIVER
	clientObjectIndex			= -1;
	beginDragMethodIndex		= -1;
	endDragMethodIndex			= -1;
	dockedMethodIndex			= -1;
	undockedMethodIndex			= -1;
	undockedCloseMethodIndex	= -1;
	clientWindowIndex			= -1;
#endif

}

// --------------------------------------------------------------------------
//
void VDockableWindow::construct()
{
	if (style.contains(StyleSimpleDock)) {
		actingAsDockSite = 1;
	}

	if (style.contains(StyleNoCloseBox)) {
		undockedNoCloseBox = 1;
	}

	clientWindow = newClientWindow(this);

	if (clientWindow != 0) {
		clientWindow->dockableWindowOwner = this;
		//
		// I think this makes sense whether or not we are
		// ourselves acting as a dock-it (actingAsDockSite).
		//
		attachWin(clientWindow, VAttach::Left,   VAttach::Parent);
		attachWin(clientWindow, VAttach::Right,  VAttach::Parent);
		attachWin(clientWindow, VAttach::Top,    VAttach::Parent);
		attachWin(clientWindow, VAttach::Bottom, VAttach::Parent);
		frameChildren();
	}

	if (actingAsDockSite && (clientWindow != 0)) {
		//
		// In this case, the VDockableWindow is itself acting
		// as a dock-site VWindow for its client VWindow child.
		//
		lastDockSite = this;
		//
		// Implicitly register ourselves as a dock-site.
		//
		registerDockSite(this);
	}
	else {
		actingAsDockSite = 0;
		//
		// Implicitly register our parent as a dock-site,
		// but only if it is a VDockSiteWindow type.
		//
		VWindow *parent = getParent();
		if ((parent != 0) && parent->isA(VDockSiteWindowCls)) {
			registerDockSite(parent);
			lastDockSite = parent;
		}
	}

	if (style.contains(StyleUndocked)) {
		int x, y, w, h;
		this->getAbs(&x, &y, &w, &h);
		undock(x, y);
	}

	disableFocus();
	clipToChildren(TRUE);
}

// --------------------------------------------------------------------------
//
void VDockableWindow::constructUndockedWindow()
{
	if (undockedWindow != 0) {
		return;
	}
	undockedWindow = newUndockedWindow(this);
	if (undockedTitle != 0) {
		undockedWindow->setTitle(*undockedTitle);
	}
	undockedWindow->uponClose
		(this, methodOf(VDockableWindow, undockedClosed));
	undockedWindow->setCloseBehavior(undockedCloseBehavior);
}
// --------------------------------------------------------------------------
//
void VDockableWindow::constructConfigurationMenu()
{
	if (configurationMenu == 0) {
		configurationMenu = new ConfigurationMenu;
	}
}

// --------------------------------------------------------------------------
//
VWindow *VDockableWindow::getDockSite() const
{
	if (!currentlyDocked) {
		return 0;
	}
	else if (actingAsDockSite) {
		return (VWindow *)this;
	}
	else {
		return ((VWindow *)this)->getParent();
	}
}

// --------------------------------------------------------------------------
//
VWindow *VDockableWindow::getDockableWindow() const
{
	if (actingAsDockSite) {
		return clientWindow;
	}
	else {
		return (VWindow *)this;
	}
}
// --------------------------------------------------------------------------
//
void VDockableWindow::setDockedAttributes(VWindow *)
{
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedAttributes()
{
}

// --------------------------------------------------------------------------
//
void VDockableWindow::beginDrag()
{
	if ((clientObject != 0) && (beginDragMethod != 0)) {
		(clientObject->*beginDragMethod)(this);
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::endDrag()
{
	if ((clientObject != 0) && (endDragMethod != 0)) {
		(clientObject->*endDragMethod)(this);
	}
}

// --------------------------------------------------------------------------
// This virtual method is called as a notification that this VDockableWindow
// has been docked to the given dock-site VWindow.  A return of TRUE
// indicates that any reframing of the given dock-site VWindow has been
// performed; a return of FALSE will cause the VDockable to perform the
// appropriate reframe; this is the default.
//
boolean VDockableWindow::docked(VWindow *docksite)
{
	if ((clientObject != 0) && (dockedMethod != 0)) {
		return (clientObject->*dockedMethod)(this, docksite);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
// This virtual method is called as a notification that this VDockableWindow
// has been undocked from the given dock-site VWindow.  A return of TRUE
// indicates that any reframing of the given dock-site VWindow has
// been performed.  A return of FALSE will cause the VDockableWindow
// to perform the appropriate reframing; this is the default.
//
boolean VDockableWindow::undocked(VWindow *docksite)
{
	if ((clientObject != 0) && (undockedMethod != 0)) {
		return (clientObject->*undockedMethod)(this, docksite);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::dock(VWindow *docksite)
{
	if (docksite == 0) {
		if (lastDockSite == 0) {
			return FALSE;
		}
		else {
			docksite = lastDockSite;
		}
	}
	else if ((docksite == lastDockSite) && currentlyDocked) {
		return TRUE;
	}

	VWindow *dockable_window = getDockableWindow();

	if (!currentlyDocked) {
		if (!lastUndockedSizeIsValid) {
			int w, h;
			dockable_window->sizeOfWin(&w, &h);
			lastUndockedWidth = w;
			lastUndockedHeight = h;
			lastUndockedSizeIsValid = 1;
		}
		undockedWindow->hide();
	}


	dockable_window->reparent(docksite);

	if ((docksite->childrenOf() != 0) &&
		(docksite->childrenOf()->count() > 0)) {
		docksite->VWindow::enableBool(TRUE);
	}

	if (actingAsDockSite && (docksite == this)) {
		docksite->attachWin(dockable_window, VAttach::Left,
											 VAttach::Parent);
		docksite->attachWin(dockable_window, VAttach::Right,
											 VAttach::Parent);
		docksite->attachWin(dockable_window, VAttach::Top,
											 VAttach::Parent);
		docksite->attachWin(dockable_window, VAttach::Bottom,
											 VAttach::Parent);
	}

	VWindow *old_docksite = lastDockSite;
	int was_already_docked = currentlyDocked;

	currentlyDocked = 1;
	lastDockSite = docksite;

	boolean reframe_old_docksite, reframe_new_docksite;

	if (was_already_docked && (docksite != old_docksite)) {
		reframe_old_docksite = !undocked(old_docksite);
		if (old_docksite->isA(VDockSiteWindowCls)) {
			((VDockSiteWindow *)old_docksite)->undocked(this);
		}
	}
	else {
		reframe_old_docksite = FALSE;
	}

	reframe_new_docksite = !docked(docksite);

	VWindow *new_docksite_parent = docksite->getParent();

	if (reframe_old_docksite) {
		VWindow *old_docksite_parent = old_docksite->getParent();
		if (reframe_new_docksite) {
			if (old_docksite_parent != 0) {
				if (old_docksite_parent == new_docksite_parent) {
					old_docksite_parent->frameChildren();
				}
				else {
					old_docksite_parent->frameChildren();
					if (new_docksite_parent != 0) {
						new_docksite_parent->frameChildren();
					}
					else {
						docksite->frameWindow();
					}
				}
			}
			else {
				old_docksite->frameWindow();
				if (new_docksite_parent != 0) {
					new_docksite_parent->frameChildren();
				}
				else {
					docksite->frameWindow();
				}
			}
		}
		else if (old_docksite_parent != 0) {
			old_docksite_parent->frameChildren();
		}
		else {
			old_docksite->frameWindow();
		}
	}
	else if (reframe_new_docksite) {
		if (new_docksite_parent != 0) {
			new_docksite_parent->frameChildren();
		}
		else {
			docksite->frameWindow();
		}
	}

	if (docksite->isA(VDockSiteWindowCls)) {
		((VDockSiteWindow *)docksite)->docked(this);
	}

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::undock(int x, int y)
{
	int center_x = 0, center_y = 0;

	if (x == 32767) {
		if (lastUndockedX == 32767) {
			center_x = 1;
		}
		else {
			x = lastUndockedX;
		}
	}
	if (y == 32767) {
		if (lastUndockedY == 32767) {
			center_y = 1;
		}
		else {
			y = lastUndockedY;
		}
	}

	if (!currentlyDocked) {
		if (center_x) {
			if (center_y) {
				getCenterPosition(undockedWindow, &x, &y);
			}
			else {
				getCenterPosition(undockedWindow, &x, 0);
			}
		}
		else if (center_y) {
			getCenterPosition(undockedWindow, 0, &y);
		}
		undockedWindow->setGeometryPosition(x, y);
		return TRUE;
	}

	doingUndocked = 1;

	if (undockedWindow == 0) {
		constructUndockedWindow();
	}

	VWindow *undocked_docksite = undockedWindow->getClientAreaWindow();
	VWindow *dockable_window = getDockableWindow();
	VWindow *docksite = getDockSite();

	VWindow *dockable_window_parent = dockable_window->getParent();
	if ((dockable_window_parent != 0) &&
		(dockable_window_parent->getChildFocus() == dockable_window)) {
		dockable_window_parent->setChildFocus(0);
	}

	dockable_window->reparent(undocked_docksite);

	if ((docksite->childrenOf() == 0) ||
		(docksite->childrenOf()->count() <= 0)) {
		docksite->VWindow::enableBool(FALSE);
	}

	undocked_docksite->attachWin(dockable_window, VAttach::Left,
												  VAttach::Parent,
												  undockedLeftMargin);
	undocked_docksite->attachWin(dockable_window, VAttach::Right,
												  VAttach::Parent,
												  undockedRightMargin);
	undocked_docksite->attachWin(dockable_window, VAttach::Top,
												  VAttach::Parent,
												  undockedTopMargin);
	undocked_docksite->attachWin(dockable_window, VAttach::Bottom,
												  VAttach::Parent,
												  undockedBottomMargin);

	if ((lastUndockedWidth > 0) && (lastUndockedHeight > 0)) {
		if (center_x) {
			if (center_y) {
				getCenterPosition(undockedWindow, &x, &y);
			}
			else {
				getCenterPosition(undockedWindow, &x, 0);
			}
		}
		else if (center_y) {
			getCenterPosition(undockedWindow, 0, &y);
		}
		int border_w, border_h;
		undockedWindow->getSystemBorderSize(&border_w, &border_h);
		undockedWindow->setGeometry
			(x, y, lastUndockedWidth + border_w,
				   lastUndockedHeight + border_h);
	}
	else {
		int preferred_w, preferred_h;
		if (undockedSquare) {
			getBestSquareSize(dockable_window, &preferred_w, &preferred_h);
		}
		else {
			undockedWindow->getClientAreaWindow()->frameChildren();
			undockedWindow->frameChildren();
			undockedWindow->frameWindow();
			dockable_window->determinePreferredSize
				(&preferred_w, &preferred_h);
		}
		undockedWindow->resizeClientArea
			(preferred_w + undockedLeftMargin + undockedRightMargin,
			 preferred_h + undockedTopMargin + undockedBottomMargin);
		if (center_x) {
			if (center_y) {
				getCenterPosition(undockedWindow, &x, &y);
			}
			else {
				getCenterPosition(undockedWindow, &x, 0);
			}
		}
		else if (center_y) {
			getCenterPosition(undockedWindow, 0, &y);
		}
		undockedWindow->setGeometryPosition(x, y);
//		int w, h;
//		if (!lastUndockedSizeIsValid) {
//			undockedWindow->sizeOfWin(&w, &h);
//			lastUndockedWidth = w;
//			lastUndockedHeight = h;
//		}
	}
	undocked_docksite->frameChildren();

	if (explicitShow || (!explicitHide && !style.contains(StyleHidden))) {
		undockedNotYetShown = 0;
		undockedWindow->show();
	}

	currentlyDocked = 0;

	if (!undocked(docksite)) {
    	if (docksite != dockSiteUnregisteredWhileDoingUndocked) {
			VWindow *docksite_parent = docksite->getParent();
			if (docksite_parent != 0) {
				docksite_parent->frameChildren();
			}
			else {
				docksite->frameWindow();
			}
		}
	}

    if (docksite != dockSiteUnregisteredWhileDoingUndocked) {
		if (docksite->isA(VDockSiteWindowCls)) {
			((VDockSiteWindow *)docksite)->undocked(this);
		}
	}

    dockSiteUnregisteredWhileDoingUndocked = 0;
	doingUndocked = 0;

	return TRUE;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::toggleDock()
{
	if (!currentlyDocked) {
		dock();
	}
	else {
		undock();
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::registerDockSite(VWindow *window)
{
	if (window == 0) {
		return;
	}
	if (dockSites == 0) {
		dockSites = new VOrdCollect;
	}
	else {
		DO (dockSites, DockSiteData, d)
			if (d->dockSiteWindow == window) {
				return;
			}
		END
	}
	DockSiteData *d = new DockSiteData(window);
	dockSites->add(d);
}

// --------------------------------------------------------------------------
//
void VDockableWindow::unregisterDockSite(VWindow *window)
{
	if ((window == 0) || (dockSites == 0)) {
		return;
	}
	DO (dockSites, DockSiteData, d)
		if (d->dockSiteWindow == window) {
			dockSites->removeSame(d);
		}
	END
	if (lastDockSite == window) {
		lastDockSite = 0;
	}
	if (doingUndocked) {
    	dockSiteUnregisteredWhileDoingUndocked = window;
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedCloseBehavior(int cb)
{
	if (cb & DockOnClose) {
		undockedDockOnClose = 1;
		cb &= ~DockOnClose;
	}
	else {
		undockedDockOnClose = 0;
	}
	if (undockedWindow != 0) {
		undockedWindow->setCloseBehavior(cb);
	}
	else {
		undockedCloseBehavior = char(cb);
	}
}

// --------------------------------------------------------------------------
//
int VDockableWindow::getUndockedCloseBehavior() const
{
	int cb;
	if (undockedWindow != 0) {
		cb = undockedWindow->getCloseBehavior();
	}
	else {
		cb = undockedCloseBehavior;
	}
	if (undockedDockOnClose) {
		cb |= DockOnClose;
	}
	return cb;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedTitle(const VString& title)
{
	if (undockedWindow != 0) {
		undockedWindow->setTitle(title);
	}
	else if (undockedTitle != 0) {
		*undockedTitle = title;
	}
	else {
		undockedTitle = new VString(title);
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::getUndockedTitle(VString& title) const
{
	if (undockedWindow != 0) {
		undockedWindow->getTitle(title);
	}
	else if (undockedTitle != 0) {
		title = *undockedTitle;
	}
	else {
		title = "";
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedSquare(boolean b)
{
	if ((undockedSquare = b) && !currentlyDocked && (undockedWindow != 0)) {
		VWindow *dockable_window = getDockableWindow();
		if (dockable_window != 0) {
			int preferred_w, preferred_h;
			getBestSquareSize(dockable_window, &preferred_w, &preferred_h);
			undockedWindow->resizeClientArea
				(preferred_w + undockedLeftMargin + undockedRightMargin,
				 preferred_h + undockedTopMargin + undockedBottomMargin);
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedCenter(boolean b)
{
	if ((undockedCenter = b) && !currentlyDocked && (undockedWindow != 0)) {
		VWindow *parent = undockedWindow->getParent();
		if (parent != 0) {
			VWindow *top_parent = parent->getTopLevelParent();
			if (top_parent != 0) {
				int x, y, uw, uh, px, py, pw, ph;
				undockedWindow->sizeOfWin(&uw, &uh);
				top_parent->getAbs(&px, &py, &pw, &ph);
				if ((x = (px + pw / 2) - uw / 2) < 0) { x = 0; }
				if ((y = (py + ph / 2) - uh / 2) < 0) { y = 0; }
				undockedWindow->setGeometryPosition(x, y);
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedPosition(int x, int y)
{
	lastUndockedX = x;
	lastUndockedY = y;
	if (!currentlyDocked && (undockedWindow != 0)) {
		undockedWindow->setGeometryPosition(x, y);
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::getUndockedPosition(int *x, int *y) const
{
	if (undockedWindow != 0) {
		int dummy;
		undockedWindow->getAbs(x, y ,&dummy, &dummy);
	}
	else {
		if (x != 0) { *x = lastUndockedX; }
		if (y != 0) { *y = lastUndockedY; }
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::getDockedSize(int *w, int *h,
									int docksite_w, int docksite_h,
									VWindow *) const
{
	determineConstrainedSize(w, h, docksite_w, docksite_h);
}

// --------------------------------------------------------------------------
//
void VDockableWindow::getBestSquareSize(VWindow *window, int *w, int *h)
{
	int preferred_w, preferred_h;
	int best_preferred_w, best_preferred_h;
	int last_preferred_w, last_preferred_h;
	window->determineConstrainedSize(&preferred_w, &preferred_h, 0, 1);
	int best_diff = preferred_w - preferred_h;
	if (best_diff < 0) { best_diff = -best_diff; }
	best_preferred_w = preferred_w;
	best_preferred_h = preferred_h;
	while (1) {
		window->determineConstrainedSize(&preferred_w, &preferred_h,
										 last_preferred_w - 1, 0);
		int diff = preferred_w - preferred_h;
		if (diff < 0) { diff = -diff; }
		if (diff < best_diff)  {
			best_diff = diff;
			best_preferred_w = preferred_w;
			best_preferred_h = preferred_h;
		}
		if ((last_preferred_w == preferred_w) ||
			(last_preferred_h == preferred_h) ||
			((last_preferred_w = preferred_w) <= 0) &&
			((last_preferred_h = preferred_h) <= 0)) {
			break;
		}
	}
	if (w != 0) { *w = best_preferred_w; }
	if (h != 0) { *h = best_preferred_h; }
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedNoClose(boolean b)
{
    undockedNoCloseBox = b;
	if (undockedWindow != 0) {
		if (undockedNoCloseBox) {
			undockedWindow->setCloseBehavior
				(undockedWindow->getCloseBehavior() | NoClose);
		}
		else {
			undockedWindow->setCloseBehavior
				(undockedWindow->getCloseBehavior() & ~NoClose);
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedWindowMargins(int left, int right,
                                               int top, int bottom)
{
	return;
    if      (left < 0)      { undockedLeftMargin   = 0;            }
    else if (left > 127)    { undockedLeftMargin   = char(127);    }
    else                    { undockedLeftMargin   = char(left);   }
    if      (right < 0)     { undockedRightMargin  = 0;            }
    else if (right > 127)   { undockedRightMargin  = char(127);    }
    else                    { undockedRightMargin  = char(right);  }
    if      (top < 0)       { undockedTopMargin    = 0;            }
    else if (top > 127)     { undockedTopMargin    = char(127);    }
    else                    { undockedTopMargin    = char(top);    }
    if      (bottom < 0)    { undockedBottomMargin = 0;            }
    else if (bottom > 127)  { undockedBottomMargin = char(127);    }
    else                    { undockedBottomMargin = char(bottom); }
}

// --------------------------------------------------------------------------
//
static int over_dock_site				= 0;
static int dock_site_x					= 0;
static int dock_site_y					= 0;
static int dock_site_mx					= 0;
static int dock_site_my					= 0;
static int opaque_moving				= 0;
static int am_undocking					= 0;
static int undocked_border_width		= 0;
static int undocked_border_height		= 0;

// --------------------------------------------------------------------------
//
void VDockableWindow::setUndockedDockedStyle(boolean b)
{
	if (b) {
		if (!undockedDockedStyle) {
			undockedDockedStyle = 1;
			VWindow *dockable_window = getDockableWindow();
			VWindow *undocked_clientwindow =
				undockedWindow->getClientAreaWindow();
			undocked_clientwindow->attachWin
				(dockable_window, VAttach::Left,   VAttach::Parent, 0);
			undocked_clientwindow->attachWin
				(dockable_window, VAttach::Right,  VAttach::Parent, 0);
			undocked_clientwindow->attachWin
				(dockable_window, VAttach::Top,    VAttach::Parent, 0);
			undocked_clientwindow->attachWin
				(dockable_window, VAttach::Bottom, VAttach::Parent, 0);
			undockedWindow->setDisableDecorations(TRUE);
		}
	}
	else if (undockedDockedStyle) {
		undockedDockedStyle = 0;
		VWindow *dockable_window = getDockableWindow();
		VWindow *undocked_clientwindow =
			undockedWindow->getClientAreaWindow();
		undocked_clientwindow->attachWin
			(dockable_window, VAttach::Left,   VAttach::Parent,
											   undockedLeftMargin);
		undocked_clientwindow->attachWin
			(dockable_window, VAttach::Right,  VAttach::Parent,
											   undockedRightMargin);
		undocked_clientwindow->attachWin
			(dockable_window, VAttach::Top,    VAttach::Parent,
											   undockedTopMargin);
		undocked_clientwindow->attachWin
			(dockable_window, VAttach::Bottom, VAttach::Parent,
											   undockedBottomMargin);
		undockedWindow->setDisableDecorations(FALSE);
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::beginDragging(int mx, int my)
{
	beginDrag();

	// Start the actual dragging; the methods dragging() and
	// endDragging() will be called automatically via callbacks.

	int x, y, w, h, dummy;
	mapToScreen(&mx, &my);

	if (!currentlyDocked) {
		undockedWindow->getAbs(&x, &y ,&dummy, &dummy);
		lastUndockedX = x;
		lastUndockedY = y;
		if (!lastUndockedSizeIsValid) {
			VWindow *dockable_window = getDockableWindow();
			dockable_window->sizeOfWin(&w, &h);
			lastUndockedWidth  = w;
			lastUndockedHeight = h;
		}
		undockedWindow->getSystemBorderSize(&undocked_border_width,
											&undocked_border_height);
		undockedWindow->moveWithRubberBand(x, y, w, h, mx, my);
	}
	else {
		getAbs(&x, &y, &w, &h);
		VRubberBand rubberband;
		rubberband.setHysteresis(2, 2);
		rubberband.setThickness(1);
		rubberband.uponBeginMove(this, methodOf(VDockableWindow,
												prepareForDragging));
		rubberband.uponMove(this, methodOf(VDockableWindow, dragging));
		int status = rubberband.move(x, y, w, h, mx, my, this);
		endDragging(x, y, w, h, mx, my, status);
	}
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::prepareForDragging()
{
	am_undocking = currentlyDocked;

	VWindow *dockable_window = getDockableWindow();

	// Determine the preferred size of the undocked window.

	if (!currentlyDocked) {
		int x, y, dummy;
		undockedWindow->getAbs(&x, &y ,&dummy, &dummy);
		lastUndockedX = x;
		lastUndockedY = y;
	}
	if ((lastUndockedWidth == 0) || (lastUndockedHeight == 0)) {
		int preferred_w, preferred_h;
		if (undockedSquare) {
			setUndockedAttributes();
			getBestSquareSize(dockable_window, &preferred_w, &preferred_h);
			setDockedAttributes(getDockSite());
		}
		else {
			dockable_window->determinePreferredSize(&preferred_w,
													&preferred_h);
		}
		if (undockedWindow == 0) {
			constructUndockedWindow();
		}
		lastUndockedWidth = preferred_w;
		lastUndockedHeight = preferred_h;
		lastUndockedSizeIsValid = 1;
	}
	else if (!lastUndockedSizeIsValid) {
		int w, h;
		dockable_window->sizeOfWin(&w, &h);
		lastUndockedWidth = w;
		lastUndockedHeight = h;
		lastUndockedSizeIsValid = 1;
	}
	undockedWindow->getSystemBorderSize(&undocked_border_width,
										&undocked_border_height);

	// Go through the possible dock-sites and determine for each,
	// the preferred size which we would like to be if we had to
	// be docked to that dock-site.

	if (dockSites != 0) {
		DO (dockSites, DockSiteData, d)
			VWindow *window = d->dockSiteWindow;
			d->dockSiteX = 0;
			d->dockSiteY = 0;
			d->dockSiteWidth = 0;
			d->dockSiteHeight = 0;
			d->dockableWindowWidth = 0;
			d->dockableWindowHeight = 0;
			if (window->isHidden()) {
				continue;
			}
			int dx, dy, dw, dh;
			window->getAbs(&dx, &dy, &dw, &dh);
			d->dockSiteX = dx;
			d->dockSiteY = dy;
			d->dockSiteWidth = dw;
			d->dockSiteHeight = dh;
			if (dw == 0) { dw = 1; }
			if (dh == 0) { dh = 1; }
			int docked_w, docked_h;
			getDockedSize(&docked_w, &docked_h, dw, dh, window);
			d->dockableWindowWidth = docked_w;
			d->dockableWindowHeight = docked_h;
		END
	}

	if (currentlyDocked || (undockedWindow == 0)) {
		opaque_moving = 0;
	}
	else {
		opaque_moving = undockedWindow->isOpaqueMoving();
	}

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::dragging(int& x, int& y, int& w, int& h,
								  int mx, int my)
{
	int was_over_dock_site = over_dock_site;

	VRubberBand *rubberband = VRubberBand::getCurrent();
	if (notifier->keyStatus(K_Control) ||
		overDockSite(mx, my, x, y, w, h) == 0) {
		//
		// Here, we are *not* over a dock-site.
		//
		if ((lastUndockedWidth > 0) && (lastUndockedHeight > 0)) {
			w = lastUndockedWidth + undocked_border_width;
			h = lastUndockedHeight + undocked_border_height;
		}
		if (rubberband != 0) {
			rubberband->setThickness(3);
		}
		if (was_over_dock_site) {
			//
			// Here, we *were* previously over a dock-site.
			//
			over_dock_site = 0;
			if (mx <= dock_site_x) {
				x = dock_site_x - w + (dock_site_mx - mx);
			}
			if (my <= dock_site_y) {
				y = dock_site_y - h + (dock_site_my - my);
			}
			if (opaque_moving) {
				setUndockedDockedStyle(FALSE);
			}
		}
	}
	else {
		//
		// Here, we *are* over a dock-site.
		//
		if (rubberband != 0) {
			rubberband->setThickness(1);
		}
		if (opaque_moving) {
			if (!was_over_dock_site) {
				setUndockedDockedStyle(TRUE);
			}
			int border_w, border_h;
			undockedWindow->getSystemBorderSize(&border_w, &border_h);
			w += border_w, h += border_h;
		}
	}

	if (mx > x + w) {
		x = mx - w + 2;
	}
	else if (mx < x) {
		x = mx;
	}
	if (my > y + h) {
		y = my - h + 2;
	}
	else if (my < y) {
		y = my;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::endDragging(int x, int y, int w, int h,
									 int mx, int my, int& status)
{
	endDrag();
	if (opaque_moving) {
		setUndockedDockedStyle(FALSE);
	}
	if (status > 0) {
		VWindow *docksite = notifier->keyStatus(K_Control) ? 0 :
							overDockSite(mx, my, x, y, w, h);
		if (docksite != 0) {
			dock(docksite);
		}
		else {
			setUndockedDockedStyle(FALSE);
			undock(x, y);
		}
	}
	over_dock_site = 0;
	return TRUE;
}

// --------------------------------------------------------------------------
//
VWindow *VDockableWindow::overDockSite(int mx, int my,
									   int& x, int& y, int& w, int& h)
{	
	if ((dockSites != 0) && !dockingCurrentlyDisabled) {
		DO (dockSites, DockSiteData, d)
			int ww = d->dockableWindowWidth;
			int hh = d->dockableWindowHeight;
			int xx = (mx > x + ww) ? mx - ww + 2 : x;
			int yy = (my > y + hh) ? my - hh + 2 : y;
			if (rectanglesIntersect
				(xx, yy, ww, hh,
				 d->dockSiteX, d->dockSiteY,
				 d->dockSiteWidth, d->dockSiteHeight)) {
				if (!over_dock_site) {
			 		if (overDockSitePercentage > 0.0) {
						int area = getRectangleIntersectionArea
								   (x, y, w, h,
								    d->dockSiteX, d->dockSiteY,
									d->dockSiteWidth, d->dockSiteHeight);
						int sw = w < d->dockSiteWidth ?
								 w : d->dockSiteWidth;
						int sh = h < d->dockSiteHeight ?
								 h : d->dockSiteHeight;
						if (area < sw * sh * overDockSitePercentage) {
							continue;
						}
					}
					if (overDockSitePointer) {
						if ((mx + 2 < d->dockSiteX) ||
							(my + 2 < d->dockSiteY) ||
							(mx - 2 > d->dockSiteX + d->dockSiteWidth) ||
							(my - 2 > d->dockSiteY + d->dockSiteHeight)) {
							continue;
						}
					}
				}
				w = ww;
				h = hh;
				over_dock_site = 1;
				dock_site_x = d->dockSiteX;
				dock_site_y = d->dockSiteY;
				dock_site_mx = mx;
				dock_site_my = my;
				return d->dockSiteWindow;
			}
		END
	}
	return 0;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::endResizing(int, int, int w, int h, int, int&)
{
	lastUndockedWidth = w - undocked_border_width;
	lastUndockedHeight = h - undocked_border_height;
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VDockableWindow::frameWindow(boolean minimum)
{
	if (currentlyDocked) {
		if (actingAsDockSite) {
			if (clientWindow != 0) {
				clientWindow->frameWindow(minimum);
			}
		}
		else {
			VWindow::frameWindow(minimum);
		}
	}
	else if (undockedWindow != 0) {
		getDockableWindow()->getGeoFrame()->update();
		undockedWindow->getGeoFrame()->update();
		undockedWindow->getClientAreaWindow()->getGeoFrame()->update();
		undockedWindow->frameWindow(minimum);
		lastUndockedSizeIsValid = 0;
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::showBool(boolean show, boolean update)
{
	if (currentlyDocked) {
		if (actingAsDockSite) {
			if (clientWindow != 0) {
				clientWindow->showBool(show, update);
			}
		}
		else {
			VWindow::showBool(show, update);
		}
	}
	else if (undockedWindow != 0) {
		if (show) {
			if (undockedNotYetShown) {
				undockedNotYetShown = 0;
				frameWindow();
			}
		}
		undockedWindow->showBool(show, update);
	}
	explicitShow = show;
	explicitHide = !show;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	beginDragging(mx, my);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::mouseDbl(int, int)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (!dockingCurrentlyDisabled) {
		toggleDock();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::rmouseDn(int x, int y)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (!configurationMenuDisabled && !currentlyDocked) {
		constructConfigurationMenu();
		configurationMenu->popup(x, y, this);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockableWindow::rmouseDnUndockedWindow(int x, int y)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (undockedWindow != 0) {
		undockedWindow->mapToScreen(&x, &y);
		mapToWin(&x, &y);
		return rmouseDn(x, y);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
VUtilityWindow *VDockableWindow::newUndockedWindow(VWindow *parentwin)
{
	if (parentwin != 0) {
		parentwin = parentwin->getTopLevelParent();
	}
	VWinStyle style = StyleHidden;
	if (undockedNoCloseBox) {
		style += StyleNoCloseBox;
	}
	else {
		style += StyleCloseBox;
	}
	VUtilityWindow *undocked_window = new VUtilityWindow(parentwin, style);
	undocked_window->uponBeginMoving
		(this, methodOf(VDockableWindow, prepareForDragging));
	undocked_window->uponMoving
		(this, methodOf(VDockableWindow, dragging));
	undocked_window->uponEndMoving
		(this, methodOf(VDockableWindow, endDragging));
	undocked_window->uponEndResizing
		(this, methodOf(VDockableWindow, endResizing));
	undocked_window->uponMouseDbl
		(this, methodOf(VDockableWindow, mouseDbl));
	undocked_window->uponRmouseDn
		(this, methodOf(VDockableWindow, rmouseDnUndockedWindow));
	return undocked_window;
}

// --------------------------------------------------------------------------
//
VWindow *VDockableWindow::newClientWindow(VWindow *dockable_window)
{
	if (actingAsDockSite) {
		return new VWindow(VFrame(), dockable_window);
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::setClientWindow(VWindow *client_window)
{
	if (!actingAsDockSite &&
		(clientWindow == 0) && (client_window->getParent() == this) &&
		(client_window != 0)) {
		clientWindow = client_window;
		clientWindow->dockableWindowOwner = this;
	}
}

// ===========================================================================
// VDockableWindow::ConfigurationMenu
// ===========================================================================

// --------------------------------------------------------------------------
//
VDockableWindow::ConfigurationMenu::ConfigurationMenu()
	: VPopupMenu(VMenuData("", "", methodOf(ConfigurationMenu, poppedUp), this))
{
	dockableWindow = 0;
	addMenuItem(VMenuData
		("&Move", "",
		 methodOf(ConfigurationMenu, setMove), this, 1));
	addMenuItem(VMenuData
		("&Size", "",
		 methodOf(ConfigurationMenu, setSize), this, 2));
	addMenuItem(VMenuData
		("&Dock", "",
		 methodOf(ConfigurationMenu, setDock), this, 3));
	addMenuItem(VMenuData());
	addMenuItem(VMenuData
		("&Close", "",
		 methodOf(ConfigurationMenu, setClose), this, 9));
	addMenuItem(VMenuData());

	VPopupMenu *popup1 = new VPopupMenu
		(VMenuData("Options", "",
				   methodOf(ConfigurationMenu, poppedUpOptions), this));
	popup1->addMenuItem(VMenuData
		("&Dockable", "",
		 methodOf(ConfigurationMenu, setSensitivity), this, 9));

	addMenuItem(VMenuData(popup1));

	VPopupMenu *popup2 = new VPopupMenu
		(VMenuData("Docking Sensitivity", "",
				   methodOf(ConfigurationMenu, poppedUpSensitivity), this));
	popup2->addMenuItem(VMenuData
		("Low", "",
		 methodOf(ConfigurationMenu, setSensitivity), this, 1));
	popup2->addMenuItem(VMenuData
		("Medium", "",
		 methodOf(ConfigurationMenu, setSensitivity), this, 2));
	popup2->addMenuItem(VMenuData
		("High", "",
		 methodOf(ConfigurationMenu, setSensitivity), this, 3));
	popup2->addMenuItem(VMenuData());
	popup2->addMenuItem(VMenuData
		("Pointer", "",
		 methodOf(ConfigurationMenu, setSensitivity), this, 0));
	popup1->addMenuItem(VMenuData(popup2));

	popup2 = new VPopupMenu
		(VMenuData("Style", "",
		 methodOf(ConfigurationMenu, poppedUpStyle), this));
	popup2->addMenuItem(VMenuData
		("MS/Windows &95", "",
		 methodOf(ConfigurationMenu, setStyle), this, 1));
	popup2->addMenuItem(VMenuData
		("MS/Windows &3.1", "",
		 methodOf(ConfigurationMenu, setStyle), this, 2));
	popup2->addMenuItem(VMenuData
		("&OS/2", "",
		 methodOf(ConfigurationMenu, setStyle), this, 3));
	popup2->addMenuItem(VMenuData
		("OSF/&Motif", "",
		 methodOf(ConfigurationMenu, setStyle), this, 4));
	popup1->addMenuItem(VMenuData(popup2));

	VPopupMenu *popup3 = new VPopupMenu
		(VMenuData("Opaque", "",
		 methodOf(ConfigurationMenu, poppedUpOpaque), this));
	popup3->addMenuItem(VMenuData
		("Mo&ve/Size", "",
		 methodOf(ConfigurationMenu, setOpaque), this, 1));
	popup3->addMenuItem(VMenuData
		("&Move", "",
		 methodOf(ConfigurationMenu, setOpaque), this, 2));
	popup3->addMenuItem(VMenuData
		("&Size", "",
		 methodOf(ConfigurationMenu, setOpaque), this, 3));
	popup2->addMenuItem(VMenuData());
	popup2->addMenuItem(VMenuData(popup3));
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::popup(int x, int y, VDockableWindow *dockable)
{
	if (dockable != 0) {
		dockableWindow = dockable;
		dockableWindow->mapToScreen(&x, &y);
		popupAt(x, y);
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::poppedUp(VPopupMenu *pm)
{
	if ((pm != 0) && (dockableWindow != 0)) {
		pm->enableMenuItem(1, FALSE);
		pm->enableMenuItem(2, FALSE);
		pm->enableMenuItem(3, dockableWindow->isDockingEnabled());
		pm->enableMenuItem(9, !(dockableWindow->getUndockedCloseBehavior() &
								NoClose));
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::poppedUpOptions(VPopupMenu *pm)
{
	if ((pm != 0) && (dockableWindow != 0)) {
		pm->checkMenuItem(9, dockableWindow->isDockingEnabled());
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::poppedUpOpaque(VPopupMenu *pm)
{
	if ((pm != 0) && (dockableWindow != 0)) {
		VUtilityWindow *utility_window = dockableWindow->getUndockedWindow();
		if (utility_window != 0) {
			pm->checkMenuItem(1, utility_window->isOpaqueMoving() &&
								 utility_window->isOpaqueResizing());
			pm->checkMenuItem(2, utility_window->isOpaqueMoving());
			pm->checkMenuItem(3, utility_window->isOpaqueResizing());
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::poppedUpSensitivity(VPopupMenu *pm)
{
	if ((pm != 0) && (dockableWindow != 0)) {
		float f = dockableWindow->getOverDockSitePercentage();
		pm->checkMenuItem(3, f == 0.0);
		pm->checkMenuItem(2, f > 0.0 && f < 0.6666);
		pm->checkMenuItem(1, f >= 0.6666 && f <= 1.0);
		pm->checkMenuItem(0, dockableWindow->getOverDockSitePointer());
		VUtilityWindow *utility_window = dockableWindow->getUndockedWindow();
		if (utility_window != 0) {
			pm->checkMenuItem(4, utility_window->isOpaqueResizing());
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::poppedUpStyle(VPopupMenu *pm)
{
	if (pm != 0) {
		pm->enableMenuItem(1, VUtilityWindow::supportsStyleWindows95());
		pm->enableMenuItem(2, VUtilityWindow::supportsStyleWindows31());
		pm->enableMenuItem(3, VUtilityWindow::supportsStyleOs2());
		pm->enableMenuItem(4, VUtilityWindow::supportsStyleMotif());
		pm->checkMenuItem(1, VUtilityWindow::isStyleWindows95());
		pm->checkMenuItem(2, VUtilityWindow::isStyleWindows31());
		pm->checkMenuItem(3, VUtilityWindow::isStyleOs2());
		pm->checkMenuItem(4, VUtilityWindow::isStyleMotif());
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setMove(VMenuItem *)
{
	// Not yet implemented.
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setSize(VMenuItem *)
{
	// Not yet implemented.
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setDock(VMenuItem *)
{
	if (dockableWindow != 0) {
		dockableWindow->dock();
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setClose(VMenuItem *)
{
	if (dockableWindow != 0) {
		VUtilityWindow *utility_window = dockableWindow->getUndockedWindow();
		if (utility_window != 0) {
			utility_window->activateCloseBox();
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setOpaque(VMenuItem *mi)
{
	if ((mi != 0) && (dockableWindow != 0)) {
		VUtilityWindow *utility_window = dockableWindow->getUndockedWindow();
		if (utility_window != 0) {
			switch (mi->getIdNo()) {
			case 1:
				if (utility_window->isOpaqueMoving() &&
					utility_window->isOpaqueResizing()) {
					utility_window->setOpaqueMoving(FALSE);
					utility_window->setOpaqueResizing(FALSE);
				}
				else {
					utility_window->setOpaqueMoving(TRUE);
					utility_window->setOpaqueResizing(TRUE);
				}
				break;
			case 2:
				utility_window->setOpaqueMoving
					(!utility_window->isOpaqueMoving());
				break;
			case 3:
				utility_window->setOpaqueResizing
					(!utility_window->isOpaqueResizing());
				break;
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setStyle(VMenuItem *mi)
{
	if ((mi != 0) && (dockableWindow != 0)) {
		VUtilityWindow *utility_window = dockableWindow->getUndockedWindow();
		switch (mi->getIdNo()) {
		case 1:
			if (!VUtilityWindow::isStyleWindows95()) {
    			dockableWindow->lastUndockedSizeIsValid = 0;
				VUtilityWindow::setStyleWindows95();
			}
			break;
		case 2:
			if (!VUtilityWindow::isStyleWindows31()) {
    			dockableWindow->lastUndockedSizeIsValid = 0;
				VUtilityWindow::setStyleWindows31();
			}
			break;
		case 3:
			VUtilityWindow::setStyleOs2();
			break;
		case 4:
			VUtilityWindow::setStyleMotif();
			break;
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableWindow::ConfigurationMenu::setSensitivity(VMenuItem *mi)
{
	if ((mi != 0) && (dockableWindow != 0)) {
		switch (mi->getIdNo()) {
		case 3:
			dockableWindow->setOverDockSitePercentage(0.0F);
			break;
		case 2:
			dockableWindow->setOverDockSitePercentage(0.5F);
			break;
		case 1:
			dockableWindow->setOverDockSitePercentage(1.0F);
			break;
		case 0:
			dockableWindow->setOverDockSitePointer
				(!dockableWindow->getOverDockSitePointer());
			break;
		case 9:
			dockableWindow->enableDocking
				(!dockableWindow->isDockingEnabled());
			break;
		}
	}
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VDockableWindow::putTo(VArchiver& a)
{
	VDockableWindow::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VDockableWindow::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VWindow::putTo(a, FALSE);

	// Write the object version tag.

	a << VDockableWindowArchiveTag;

	// Write any object specific data.

    a.putObject(notifier->
				getClientName(clientObject, clientObjectIndex), TRUE);

    const char *class_name = 0, *method_name = 0;
    VCallbackList::findCallback(method(beginDragMethod),
								class_name, method_name,
								beginDragMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(endDragMethod),
								class_name, method_name,
								endDragMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(dockedMethod),
								class_name, method_name,
								dockedMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(undockedMethod),
								class_name, method_name,
								undockedMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(undockedCloseMethod),
								class_name, method_name,
								undockedCloseMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

	if (clientWindow != 0) {
		VString *name = clientWindow->getName();
		if (name == 0) {
			static unsigned long k = 0; VString s;
			s.prints("VDockableWindow_ClientWindow_%d", ++k);
			clientWindow->setName(s);
		    a.putObject(&s, TRUE);
		}
		else {
		    a.putObject(notifier->getClientName
						(clientWindow, clientWindowIndex), TRUE);
		}
	}
	else {
		VString s("");
		a.putObject(&s, TRUE);
	}

	if (dockSites != 0) {
		a << short(dockSites->count());
		DO (*dockSites, DockSiteData, docksite)
			VString *name = docksite->dockSiteWindow->getName();
			if (name == 0) {
				static unsigned long k = 0; VString s;
				s.prints("%s_%d", iam()->nameOf(), ++k);
				docksite->dockSiteWindow->setName(s);
			}
			VString s = *docksite->dockSiteWindow->getName();
	    	a.putObject(&s, TRUE);
		END
	}
	else {
		a << short(0);
	}

	VString title;
	getUndockedTitle(title);
	a.putObject(&title, TRUE);

	a << char(actingAsDockSite);
	a << char(currentlyDocked);
    a << char(undockedLeftMargin);
    a << char(undockedRightMargin);
    a << char(undockedTopMargin);
    a << char(undockedBottomMargin);
    a << char(undockedCloseBehavior);
    a << char(undockedSquare);
    a << char(undockedCenter);
    a << char(configurationMenuDisabled);
    a << char(dockingCurrentlyDisabled);
    a << char(overDockSitePointer);
	a << float(overDockSitePercentage);

	// Write any children.

	if (most_derived) {
		if (undockedWindow != 0) { undockedWindow->setArchiveEnable(FALSE); }
		VWindow::putChildrenTo(a);
		if (undockedWindow != 0) { undockedWindow->setArchiveEnable(TRUE); }
	}
}

// ---------------------------------------------------------------------------
//
void VDockableWindow::getFrom(VArchiver& a)
{
	// Read the base object data.

	VWindow::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VDockableWindowArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VDockableWindowArchiveTag));
		return;
	}

	// Read any object specific data.

    VString client_name;
    a.getObject(&client_name);
    clientObject = notifier->getClient(client_name, &clientObjectIndex);

    char method_name[256], *method_name_ptr = method_name;
    char class_name[256], *class_name_ptr = class_name;
	method m;

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&beginDragMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponBeginDrag(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&endDragMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponEndDrag(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&dockedMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponDocked(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&undockedMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponUndocked(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&undockedCloseMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponUndockedClose(clientObject, m);
    }

    a.getObject(&client_name);
    clientWindow = (VWindow *)notifier->
							  getClient(client_name, &clientWindowIndex);
	if (clientWindow != 0) {
    	clientWindow->dockableWindowOwner = this;
	}


	short ndocksites;
	a >> ndocksites;
	while (ndocksites-- > 0) {
		VString *docksite_name = (VString *)a.getObject(0);
		if (docksite_name != 0) { 
			if (dockSites == 0) { dockSites = new VOrdCollect; }
			dockSites->add(docksite_name);
		}
	}

	VString *title = (VString *)a.getObject(0);
	if (title != 0) {
		setUndockedTitle(*title);
	}

	char c;
	a >> c; actingAsDockSite = c;
	a >> c; currentlyDocked = c;
    a >> c; undockedLeftMargin = c;
    a >> c; undockedRightMargin = c;
    a >> c; undockedTopMargin = c;
    a >> c; undockedBottomMargin = c;
    a >> c; undockedCloseBehavior = c;
    a >> c; undockedSquare = c;
    a >> c; undockedCenter = c;
    a >> c; configurationMenuDisabled = c;
    a >> c; dockingCurrentlyDisabled = c;
    a >> c; overDockSitePointer = c;
	a >> overDockSitePercentage;
}

// ---------------------------------------------------------------------------
//
void VDockableWindow::getFrom(VArchiver& a,
							   VObject *parentwin, boolean do_realize)
{
	VDockableWindow::getFrom(a);

	if (do_realize) {
		VDockableWindow::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VDockableWindow::realize(VWindow *parentwin)
{
	VWindow::realize(parentwin);
	if (clientWindow == 0) {
		VOrdCollect *children = childrenOf();
		if (children != 0) {
			clientWindow = (VWindow *)children->idAt(0);
		}
	}
	if (clientWindow != 0) {
		clientWindow->dockableWindowOwner = this;
	}
	if (dockSites != 0) {
		VOrdCollect *docksites = dockSites;
		dockSites = 0;
		DO (*docksites, VString, docksite_name)
			if (docksite_name->isA(VStringCls)) {
				registerDockSite
					(notifier->findWindow(*docksite_name, VWindowCls));
			}
		END
		docksites->freeContents(); delete docksites;
	}
	clipToChildren(TRUE);
}

#endif // !CV_NOARCHIVER
