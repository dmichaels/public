//	dragsite.cpp -- [UNDER DEVELOPMENT]
//
//	VDragSite implementation
//
//	Allegris Foundation 1.0.03
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
//	06/28/95 dgm	Original
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
//
#include "dragsite.h"
#include "objcour.h"
#include "dndimp.h"
#include "window.h"

// --------------------------------------------------------------------------
//
defineClass(VDragSite, VObject)

// --------------------------------------------------------------------------
//
#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
VDragSite::VDragSite()
{
	theWindow				= 0;
	clientObject			= 0;
	dragStartMethod			= 0;
	dropSiteFeedbackMethod	= 0;
	copyMouseCursor			= 0;
	moveMouseCursor			= 0;
	linkMouseCursor			= 0;
	invalidMouseCursor		= 0;
	noMouseCursor			= 0;
	dragSiteImp				= 0;
}

// --------------------------------------------------------------------------
//
VDragSite::VDragSite(const VDragSite& dragsite)
{
	theWindow				= 0;
	clientObject			= dragsite.clientObject;
	dragStartMethod			= dragsite.dragStartMethod;
	dropSiteFeedbackMethod	= dragsite.dropSiteFeedbackMethod;
	copyMouseCursor			= dragsite.copyMouseCursor;
	moveMouseCursor			= dragsite.moveMouseCursor;
	linkMouseCursor			= dragsite.linkMouseCursor;
	invalidMouseCursor		= dragsite.invalidMouseCursor;
	noMouseCursor			= dragsite.noMouseCursor;
	dragSiteImp				= 0;
}

// --------------------------------------------------------------------------
//
VDragSite::~VDragSite()
{
	if (theWindow != 0) {
		unregisterWindow(theWindow);
	}
	if (dragSiteImp != 0) {
		delete dragSiteImp;
	}
}

// --------------------------------------------------------------------------
//
void VDragSite::uponDragStart(VObject *o, method m)
{
	clientObject = o;
	dragStartMethod = (VObjectCourier *(VObject::*)(VWindow *, int, int))m;
}

// --------------------------------------------------------------------------
//
void VDragSite::uponDropSiteFeedback(VObject *o, method m)
{
	clientObject = o;
	dropSiteFeedbackMethod = (boolean (VObject::*)(long))m;
}

// --------------------------------------------------------------------------
// This virtual function is called when the end-user initiates a drag-and-
// drop operation (e.g. with a click of the mouse) on the given VWindow at
// the given location.  If FALSE is returned then no action is taken,
// otherwise a drag will be started by the drag-manager.
//
VObjectCourier *VDragSite::dragStart(VWindow *w, int x, int y)
{
	if ((w != 0) &&
		(clientObject != 0) && (dragStartMethod != 0)) {
		return (clientObject->*dragStartMethod)(w, x, y);
	}

	// Implement default behavior here ...

	return 0;
}

// --------------------------------------------------------------------------
//
boolean VDragSite::dropSiteFeedback(long code)
{
	if ((clientObject != 0) && (dropSiteFeedbackMethod != 0)) {
		return (clientObject->*dropSiteFeedbackMethod)(code);
	}

	// Implement default behavior here ...

	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VDragSite::transferStart(VObjectCourier *)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VDragSite::transferEnd(VObjectCourier *)
{
	return;
}

// --------------------------------------------------------------------------
//
boolean VDragSite::registerWindow(VWindow *w)
{
	if (w == 0) {
		return FALSE;
	}
	else if ((w->dragSite == this) && (theWindow == w)) {
		return TRUE;
	}
	else if (w->dragSite != 0) {
		w->dragSite->unregisterWindow(w);
	}
	if (dragSiteImp == 0) {
		dragSiteImp = new VDragSiteImp(this);
	}
	theWindow = w;
	w->dragSite = this;
	if (!dragSiteImp->registerDragSite()) {
		theWindow = 0;
		w->dragSite = 0;
		return FALSE;
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VDragSite::unregisterWindow(VWindow *w)
{
	if ((w == 0) || (w->dragSite != this)) {
		return;
	}
	if (dragSiteImp == 0) {
		dragSiteImp = new VDragSiteImp(this);
	}
	dragSiteImp->unregisterDragSite();
	theWindow = 0;
	w->dragSite = 0;
}

// --------------------------------------------------------------------------
//
boolean VDragSite::dragLoop(VObjectCourier *object_courier)
{
	if (dragSiteImp == 0) {
		return FALSE;
	}
	return dragSiteImp->dragLoop(object_courier);
}

#endif // CV_OLE_DRAG_DROP && !CV_WIN16
