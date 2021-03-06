// $Header:   Q:/views/common/vcs/docksite.cpv   1.14   25 Feb 1997 10:18:52   tomf  $

//	docksite.cpp
//
//	VDockSiteWindow implementation [Common]
//
//	Allegris Foundation 1.1.00
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
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	11/20/95 dgm	Removed setMinimizePreferredChildMinorDimension() calls.
//	11/28/95 dgm	Fix in construct() so that we get size 0 if no children.
//	11/30/95 dgm	Added undocked()/docked().
//					Changed uponDocked()/uponUndocked().
//	12/05/95 dgm	Changed to use VColorScheme::systemXXX().
//	03/05/96 dgm	Changed getInternalBorderGeometry() to support the
//					collapsing of the dock-site when all contained
//					dockables are hidden.
//	04/02/96 dgm	Archiver support.
//	09/13/96 dgm	Fix in getInternalBorderGeometry() ... we really
//					need to well-define the notion of isHidden().
//	01/12/97 dgm	Disable focus.
// --------------------------------------------------------------------------

#include "docksite.h"
#include "dockwin.h"
#include "brush.h"
#include "pen.h"
#include "color.h"
#include "clrscm.h"
#include "port.h"
#include "rect.h"
#include "georow.h"
#include "geocol.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "cllbckls.h"
#include "notifier.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VDockSiteWindow, VWindow)
defineArchiveRevision(VDockSiteWindow, 1)

// --------------------------------------------------------------------------
//
static long			objectCount		= 0;
static VPort	   *thePort			= 0;
static VRectangle  *theRectangle	= 0;

// --------------------------------------------------------------------------
//
VDockSiteWindow::VDockSiteWindow()
	: VWindow()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VDockSiteWindow::VDockSiteWindow(const VFrame& frame, VWindow *parentwin,
								 const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VDockSiteWindow::VDockSiteWindow(VWindow *parentwin, const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VDockSiteWindow::VDockSiteWindow(const VString& name,
								 const VFrame& frame, VWindow *parentwin,
								 const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VDockSiteWindow::VDockSiteWindow(const VString& name,
								 VWindow *parentwin, const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::operator()(const VFrame& frame, VWindow *parentwin,
								 const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::operator()(const VFrame& frame, VWindow *parentwin,
								 const VStyle& style, const VString& name)
{
	VWindow::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VDockSiteWindow::~VDockSiteWindow()
{
	deleteStaticData();
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::initialize()
{
	clientObject			= 0;
	dockedMethod			= 0;
	undockedMethod			= 0;
	shadeStyle				= RaisedShade;

#ifndef CV_NOARCHIVER
	clientObjectIndex		= -1;
	dockedMethodIndex		= -1;
	undockedMethodIndex		= -1;
#endif
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::construct()
{
	constructStaticData();
	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));
	disableFocus();
	clipToChildren(TRUE);
	setup();
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::setup()
{
	if (style.contains(StyleHorizontal)) {
		VGeoRow *row = new VGeoRow;
		row->setSpacing(VGeoRow::NoSpacing);
		row->setDefaultChildJustify(VGeoRow::CenterJustify);
		row->setWrapping(TRUE);
		setGeoManager(row);
	}
	else if (style.contains(StyleVertical)) {
		VGeoColumn *column = new VGeoColumn;
		column->setSpacing(VGeoColumn::NoSpacing);
		column->setDefaultChildJustify(VGeoColumn::CenterJustify);
		column->setWrapping(TRUE);
		setGeoManager(column);
	}
	else {
		setGeoManager();
	}
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::constructStaticData()
{
	if (objectCount++ > 0) {
		return;
	}
	thePort = new VPort;
	theRectangle = new VRectangle;
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::deleteStaticData()
{
	if (--objectCount > 0) {
		return;
	}
	delete thePort; thePort = 0;
	delete theRectangle; theRectangle = 0;
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::docked(VDockableWindow *dockable_window)
{
	if ((clientObject != 0) && (dockedMethod != 0)) {
		(clientObject->*dockedMethod)(this, dockable_window);
	}
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::undocked(VDockableWindow *dockable_window)
{
	if ((clientObject != 0) && (undockedMethod != 0)) {
		(clientObject->*undockedMethod)(this, dockable_window);
	}
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::uponDocked(VObject *o, method m)
{
	clientObject = o;
	dockedMethod = (void (VObject::*)(VDockSiteWindow *,
									  VDockableWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::uponUndocked(VObject *o, method m)
{
	clientObject = o;
	undockedMethod = (void (VObject::*)(VDockSiteWindow *,
										VDockableWindow *))m;
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::setShadeStyle(int s)
{
	shadeStyle = s;
}

// --------------------------------------------------------------------------
//
void VDockSiteWindow::getInternalBorderGeometry(int *left, int *right,
												int *top, int *bottom) const
{
	int internal_border_thickness;

	if (shadeStyle != NoShade) {
		internal_border_thickness = 1;
	}
	else {
		internal_border_thickness = 0;
	}

	VOrdCollect *children = ((VDockSiteWindow *)this)->childrenOf();
	if (children == 0) {
		internal_border_thickness = 0;
	}
	else {
		int visible_docked_count = 0;
		DO (*children, VWindow, child)
			if (child->isA(VDockableWindowCls)) {
				VDockableWindow *dockable = (VDockableWindow *)child;
#if 0
				if (!dockable->isHidden()) {
					visible_docked_count++;
				}
#else
				if ((!dockable->explicitHide &&
					 !dockable->getStyle().contains(StyleHidden)) ||
					dockable->explicitShow) {
					visible_docked_count++;
				}
#endif
			}
		END
		if (visible_docked_count == 0) {
			internal_border_thickness = 0;
		}
	}

	if (left   != 0) { *left   = internal_border_thickness; }
	if (right  != 0) { *right  = internal_border_thickness; }
	if (top    != 0) { *top    = internal_border_thickness; }
	if (bottom != 0) { *bottom = internal_border_thickness; }
}

// --------------------------------------------------------------------------
//
boolean VDockSiteWindow::paint()
{
	if (shadeStyle == NoShade) {
		return TRUE;
	}

	if ((thePort == 0) || !thePort->openOn(this)) {
		return FALSE;
	}

	int w, h;
	sizeOfImage(&w, &h);

	VPen *shaded_highlight_pen = &VColorScheme::
								 systemPen(VColorScheme::ShadedHighlight);
	VPen *shaded_shadow_pen	   = &VColorScheme::
								 systemPen(VColorScheme::ShadedShadow);

	if (shadeStyle == RaisedShade) {

		thePort->usePen(shaded_highlight_pen);
		thePort->moveTo(0, 0);
		thePort->lineTo(w - 1, 0);
		thePort->moveTo(0, 1);
		thePort->lineTo(0, h - 1);

		thePort->usePen(shaded_shadow_pen);
		thePort->moveTo(w - 1, 0);
		thePort->lineTo(w - 1, h);
		thePort->moveTo(0, h - 1);
		thePort->lineTo(w - 1, h - 1);
	}
	else if (shadeStyle == SunkenShade) {

		thePort->usePen(shaded_shadow_pen);
		thePort->moveTo(0, 0);
		thePort->lineTo(w - 1, 0);
		thePort->moveTo(0, 1);
		thePort->lineTo(0, h - 1);

		thePort->usePen(shaded_highlight_pen);
		thePort->moveTo(w - 1, 0);
		thePort->lineTo(w - 1, h);
		thePort->moveTo(0, h - 1);
		thePort->lineTo(w - 1, h - 1);
	}

	thePort->close();

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockSiteWindow::mouseDn(int, int)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockSiteWindow::mouseDbl(int, int)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VDockSiteWindow::rmouseDn(int, int)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
int VDockSiteWindow::getDockedCount() const
{
	VOrdCollect *children = ((VDockSiteWindow *)this)->childrenOf();
	if (children == 0) {
		return 0;
	}
	int count = children->count();
	return count < 0 ? 0 : count;
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VDockSiteWindow::putTo(VArchiver& a)
{
	VDockSiteWindow::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VDockSiteWindow::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VWindow::putTo(a, FALSE);

	// Write the object version tag.

	a << VDockSiteWindowArchiveTag;

	// Write any object specific data.

    a.putObject(notifier->
				getClientName(clientObject, clientObjectIndex), TRUE);

    const char *class_name = 0, *method_name = 0;
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

	a << char(shadeStyle);

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VDockSiteWindow::getFrom(VArchiver& a)
{
	// Read the base object data.

	VWindow::getFrom(a);

	// Read object version tag and verify.

	long tag; a >> tag;
	if (!archiveTagOk(tag, VDockSiteWindowArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VDockSiteWindowArchiveTag)); return;
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
}

// ---------------------------------------------------------------------------
//
void VDockSiteWindow::getFrom(VArchiver& a,
							  VObject *parentwin, boolean do_realize)
{
	VDockSiteWindow::getFrom(a);

	if (do_realize) {
		VDockSiteWindow::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VDockSiteWindow::realize(VWindow *parentwin)
{
	VWindow::realize(parentwin);
	constructStaticData();
	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));
	clipToChildren(TRUE);
}

#endif // !CV_NOARCHIVER
