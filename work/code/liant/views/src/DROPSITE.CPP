//	dropsite.cpp -- [UNDER DEVELOPMENT]
//
//	VDropSite implementation
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

#include "dropsite.h"
#include "objcour.h"
#include "dndimp.h"
#include "window.h"
#include "ordcllct.h"
#include "iterator.h"
#include <stdio.h>

// --------------------------------------------------------------------------
//
defineClass(VDropSite, VObject)

// --------------------------------------------------------------------------
//
#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
VDropSite::VDropSite()
{
	clientObject			= 0;
	dragObjectEnterMethod	= 0;
	dragObjectExitMethod	= 0;
	dragObjectOverMethod	= 0;
	objectDroppedMethod		= 0;
	validTypes				= 0;
	validOperations			= MoveOp;
	dragTracking			= 0;
	dropSiteImp				= 0;
	theWindow				= 0;
	objectCourier			= 0;
}

// --------------------------------------------------------------------------
//
VDropSite::VDropSite(const VDropSite& dropsite)
{
	clientObject			= dropsite.clientObject;
	dragObjectEnterMethod	= dropsite.dragObjectEnterMethod;
	dragObjectExitMethod	= dropsite.dragObjectExitMethod;
	dragObjectOverMethod	= dropsite.dragObjectOverMethod;
	objectDroppedMethod		= dropsite.objectDroppedMethod;
	validTypes				= 0;
	validOperations			= dropsite.validOperations;
	dragTracking			= dropsite.dragTracking;
	dropSiteImp				= 0;
	theWindow				= 0;
	objectCourier			= 0;
}

// --------------------------------------------------------------------------
//
VDropSite::~VDropSite()
{
	if (theWindow != 0) {
		unregisterWindow(theWindow);
	}
	if (dropSiteImp != 0) {
		dropSiteImp = 0;
	}
}

// --------------------------------------------------------------------------
//
void VDropSite::uponDragObjectEnter(VObject *o, method m)
{
	clientObject = o;
	dragObjectEnterMethod = (long (VObject::*)(VDropSite *))m;
}

// --------------------------------------------------------------------------
//
void VDropSite::uponDragObjectExit(VObject *o, method m)
{
	clientObject = o;
	dragObjectExitMethod = (void (VObject::*)(VDropSite *))m;
}

// --------------------------------------------------------------------------
//
void VDropSite::uponDragObjectOver(VObject *o, method m)
{
	clientObject = o;
	dragObjectOverMethod = (long (VObject::*)(VDropSite *, int, int))m;
}

// --------------------------------------------------------------------------
//
void VDropSite::uponObjectDropped(VObject *o, method m)
{
	clientObject = o;
	objectDroppedMethod = (long (VObject::*)(VDropSite *, int, int))m;
}

// --------------------------------------------------------------------------
//
long VDropSite::dragObjectEnter()
{
	if ((clientObject != 0) && (dragObjectEnterMethod != 0)) {
		return (clientObject->*dragObjectEnterMethod)(this);
	}

	// Implement default behavior here ...

	return VDropSite::NoOp;
}

// --------------------------------------------------------------------------
//
void VDropSite::dragObjectExit()
{
	if ((clientObject != 0) && (dragObjectExitMethod != 0)) {
		(clientObject->*dragObjectExitMethod)(this);
	}

	// Implement default behavior here ...

	return;
}

// --------------------------------------------------------------------------
//
long VDropSite::dragObjectOver(int x, int y)
{
	if ((clientObject != 0) && (dragObjectOverMethod != 0)) {
		return (clientObject->*dragObjectOverMethod)(this, x, y);
	}

	// Implement default behavior here ...

	return VDropSite::NoOp;
}

// --------------------------------------------------------------------------
//
long VDropSite::objectDropped(int x, int y)
{
	if ((clientObject != 0) && (objectDroppedMethod != 0)) {
		return (clientObject->*objectDroppedMethod)(this, x, y);
	}

	// Implement default behavior here ...

	return VDropSite::NoOp;
}

// --------------------------------------------------------------------------
//
boolean VDropSite::isValidType(VObjectCourier *c) const
{
	if ((c == 0) || (validTypes == 0)) {
		return FALSE;
	}
	return validTypes->indexOfSame((VObject *)c) >= 0;
}

// --------------------------------------------------------------------------
//
int VDropSite::getNumberOfValidTypes() const
{
	if (validTypes == 0) {
		return 0;
	}
	int count = validTypes->count();
	if (count < 0) {
		return 0;
	}
	return count;
}

// --------------------------------------------------------------------------
//
int VDropSite::getValidOperations() const
{
	return validOperations;
}

// --------------------------------------------------------------------------
// Register the given VWindow as a drop-site referring to this VDropSite
// object.  If it is already registered as a drop-site referring to this
// VDropSite object, then simply return TRUE, otherwise if it is already
// registered as a drop-site referring to another VDropSite object, then
// first unregister the VWindow from that VDropSite object.
//
boolean VDropSite::registerWindow(VWindow *w)
{
	if (w == 0) {
		return FALSE;
	}
	else if ((w->dropSite == this) && (theWindow == w)) {
		return TRUE;
	}
	else if (w->dropSite != 0) {
		w->dropSite->unregisterWindow(w);
	}
	if (dropSiteImp == 0) {
		dropSiteImp = new VDropSiteImp(this);
	}
	theWindow = w;
	w->dropSite = this;
	if (!dropSiteImp->registerDropSite()) {
		theWindow = 0;
		w->dropSite = 0;
		return FALSE;
	}
	return TRUE;
}

// --------------------------------------------------------------------------
// Unregister the given VWindow as a drop-site.
//
void VDropSite::unregisterWindow(VWindow *w)
{
	if ((w == 0) || (w->dropSite != this) || (theWindow != w)) {
		return;
	}
	if (dropSiteImp == 0) {
		dropSiteImp = new VDropSiteImp(this);
	}
	dropSiteImp->unregisterDropSite();
	theWindow = 0;
	w->dropSite = 0;
}

#endif // CV_OLE_DRAG_DROP && !CV_WIN16
