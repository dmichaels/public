// $Header:   Q:/views/common/vcs/TOOLBAR.CPV   1.38   Mar 24 1997 16:13:54   scottt  $

//	toolbar.cpp
//
//	VToolBar implementation [Common]
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
//	10/19/95 dgm	Original.
//	11/01/95 dgm	Updated.
//	11/03/95 dgm	Changed initialize() to take one parameter as before.
//	11/15/95 dgm	Updated.
//	11/20/95 dgm	Use set/getMinorSpacing() instead of setting margins
//					on the children (which didn't work out very well).
//	11/20/95 dgm	Minor fix in beginGroup().
//	11/20/95 dgm	Changed setSpacing(), setGroupSpacing(), and beginGroup()
//					to take an optional minor spacing parameter.
//					Added set/getMinor[Group]Spacing()
//					Fixes in setRow()/setColumn().
//					Interpret as StyleHorizontal/Vertical as row/column
//					Removed setMinimizePreferredChildMinorDimension() calls.
//	11/29/95 dgm	Added enableBool().
//	12/01/95 dgm	Added newSeparator() for backward compatibility.
//	12/05/95 dgm	Changed to use VColorScheme::systemXXX().
//	12/06/95 dgm	Set background in realize().
//	12/06/95 dgm	Call setMinimizeConstrainedMajorDimension() for groups.
//	12/07/95 dss	putTo(): short cast on majorSpacing.
//	12/08/95 dgm	Motif additions.
//  02/19/96 evc    Fixed defaultStyle so its CV_NEARDATA
//	02/20/96 dgm	Added missing constructors.
//	02/22/96 dgm	Overrode getPreferredSize().
//	04/17/96 dgm	Minor changes to suppress conversion warnings.
//	05/14/96 dgm	Minor fix (update oversight) in setColumn().
//	06/12/96 pkt	updated for port of library to internet server.
//	02/04/97 pkt	removed handling of style defaults from 'init'.
//	12/13/96 dgm	Set shrinkability according to wrapping in beginGroup().
// --------------------------------------------------------------------------

#include "toolbar.h"
#include "geoframe.h"
#include "geomgr.h"
#include "georoc.h"
#include "georow.h"
#include "geocol.h"
#include "geoform.h"
#include "ordcllct.h"
#include "iterator.h"
#ifndef CV_NO_PAINT_EVENTS
#include "port.h"
#endif
#include "brush.h"
#include "pen.h"
#include "color.h"
#include "clrscm.h"
#include "rect.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

#if defined(CV_MOTIF)
#include "stdmdiap.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VToolBar, VControl)
defineArchiveRevision(VToolBar, 1)

// --------------------------------------------------------------------------
//
#define CheckUnsignedCharRange(c)	{if ((c) < 0) (c) = 0; \
									 else if ((c) > 255) (c) = 255; }
typedef unsigned char UnsignedChar;

// --------------------------------------------------------------------------
//
static long			objectCount				= 0;
static VPort	   *thePort					= 0;
static VRectangle  *theRectangle			= 0;

// --------------------------------------------------------------------------
//
VToolBar::VToolBar()
	: VControl()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VToolBar::VToolBar(const VFrame& frame, VWindow *parentwin,
				   const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style);
	construct();
}

// --------------------------------------------------------------------------
//
VToolBar::VToolBar(VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
	construct();
}

// --------------------------------------------------------------------------
//
VToolBar::VToolBar(const VString& name,
				   const VFrame& frame, VWindow *parentwin,
				   const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VToolBar::VToolBar(const VString& name,
				   VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VToolBar::~VToolBar()
{
#if defined(CV_MOTIF)
	if (this->isStdMdiViewToolBar && (appView != 0)) {
		((VStdMdiAppView *)appView)->removeToolBar();
	}
#endif
	deleteStaticData();
}

// --------------------------------------------------------------------------
//
void VToolBar::setRow()
{
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if ((roc != 0) && roc->isColumnManager()) {
		roc->setDimension(VGeoRowOrColumn::Row);
		roc->update();
		//
		// Make sure each of our groups are
		// also changed from column to row.
		//
		VOrdCollect *geo_children = roc->getChildren();
		if (geo_children != 0) {
			DO (*geo_children, VGeoFrame, geo_child)
				if (geo_child->isColumnManager() &&
					(geo_child->getWindow() == 0)) {
					((VGeoRowOrColumn *)geo_child)->
						setDimension(VGeoRowOrColumn::Row);
					geo_child->update();
				}
			END
		}
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::setColumn()
{
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if ((roc != 0) && roc->isRowManager()) {
		roc->setDimension(VGeoRowOrColumn::Column);
		roc->update();
		//
		// Make sure each of our groups are
		// also changed from column to row.
		//
		VOrdCollect *geo_children = roc->getChildren();
		if (geo_children != 0) {
			DO (*geo_children, VGeoFrame, geo_child)
				if (geo_child->isRowManager() &&
					(geo_child->getWindow() == 0)) {
					((VGeoRowOrColumn *)geo_child)->
						setDimension(VGeoRowOrColumn::Column);
					geo_child->update();
				}
			END
		}
	}
}

// --------------------------------------------------------------------------
//
boolean VToolBar::isRow() const
{
	return getGeoRow() != 0;
}

// --------------------------------------------------------------------------
//
boolean VToolBar::isColumn() const
{
	return getGeoColumn() != 0;
}

// --------------------------------------------------------------------------
//
void VToolBar::setWrapping(boolean b)
{
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->setWrapping(b);
		//
		// Make sure each of our groups, if any, are shrinkable so
		// that they in turn will wrap their children when appropriate.
		//
		float shrinkability = b ? 1.0F : 0.0F;
		VOrdCollect *geo_children = roc->getChildren();
		if (geo_children != 0) {
			DO (*geo_children, VGeoFrame, geo_child)
				if (geo_child->isRowOrColumnManager() &&
					(geo_child->getWindow() == 0)) {
					roc->setChildShrinkability
						(geo_child->getIndex(), shrinkability);
				}
			END
		}
		roc->update();
	}
}

// --------------------------------------------------------------------------
//
boolean VToolBar::getWrapping() const
{
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	return (roc != 0) && roc->getWrapping();
}

// --------------------------------------------------------------------------
//
void VToolBar::setSpacing(int major_spacing, int minor_spacing)
{
	CheckUnsignedCharRange(major_spacing);
	majorSpacing = UnsignedChar(major_spacing);

	if (minor_spacing >= 0) {
		CheckUnsignedCharRange(minor_spacing);
		minorSpacing = UnsignedChar(minor_spacing);
	}

	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->setPreferredSpacing(major_spacing);
		if (minor_spacing >= 0) {
			roc->setPreferredMinorSpacing(minor_spacing);
		}
	}
}

// --------------------------------------------------------------------------
//
int VToolBar::getSpacing() const
{
	return majorSpacing;
}

// --------------------------------------------------------------------------
//
void VToolBar::setMinorSpacing(int minor_spacing)
{
	CheckUnsignedCharRange(minor_spacing);
	minorSpacing = UnsignedChar(minor_spacing);

	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->setPreferredMinorSpacing(minor_spacing);
	}
}

// --------------------------------------------------------------------------
//
int VToolBar::getMinorSpacing() const
{
	return minorSpacing;
}

// --------------------------------------------------------------------------
//
void VToolBar::setShadeStyle(int shadestyle)
{
	shadeStyle = shadestyle;
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn *VToolBar::beginGroup(int major_spacing, int minor_spacing)
{
	if (currentGroup != 0) {
		//
		// No nested groups.
		//
		if (major_spacing >= 0) {
			CheckUnsignedCharRange(major_spacing);
			majorGroupSpacing = UnsignedChar(major_spacing);
			currentGroup->setPreferredSpacing(major_spacing);
		}
		if (minor_spacing >= 0) {
			CheckUnsignedCharRange(minor_spacing);
			minorGroupSpacing = UnsignedChar(minor_spacing);
			currentGroup->setPreferredSpacing(minor_spacing);
		}
		return currentGroup;
	}
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc == 0) {
		return 0;
	}
	if (major_spacing >= 0) {
		CheckUnsignedCharRange(major_spacing);
		majorGroupSpacing = UnsignedChar(major_spacing);
	}
	if (minor_spacing >= 0) {
		CheckUnsignedCharRange(minor_spacing);
		minorGroupSpacing = UnsignedChar(minor_spacing);
	}
	currentGroup = new VGeoRowOrColumn(roc->getDimension(), this);
	currentGroup->setSpacing(VGeoRowOrColumn::NoSpacing);
	currentGroup->setPreferredSpacing(majorGroupSpacing);
	currentGroup->setPreferredMinorSpacing(minorGroupSpacing);
	currentGroup->setWrapping(TRUE);
	currentGroup->setMinimizeConstrainedMajorDimension(FALSE);
	float shrinkability = getWrapping() ? 1.0F : 0.0F;
	roc->setChildShrinkability(currentGroup->getIndex(), shrinkability);
	return roc;
}

// --------------------------------------------------------------------------
//
void VToolBar::addChild(VWindow *child)
{
	VWindow::addChild(child);
	if (currentGroup != 0) {
		currentGroup->addChild(child);
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::endGroup()
{
	if (currentGroup != 0) {
		if (currentGroup->getNumberOfChildren() == 0) {
			delete currentGroup;
		}
		currentGroup = 0;
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::setGroupSpacing(int major_spacing, int minor_spacing)
{
	if (major_spacing >= 0) {
		CheckUnsignedCharRange(major_spacing);
		majorGroupSpacing = UnsignedChar(major_spacing);
		if (currentGroup != 0) {
			currentGroup->setPreferredSpacing(major_spacing);
		}
	}
	if (minor_spacing >= 0) {
		CheckUnsignedCharRange(minor_spacing);
		minorGroupSpacing = UnsignedChar(minor_spacing);
		if (currentGroup != 0) {
			currentGroup->setPreferredMinorSpacing(minor_spacing);
		}
	}
}

// --------------------------------------------------------------------------
//
int VToolBar::getGroupSpacing() const
{
	return majorGroupSpacing;
}

// --------------------------------------------------------------------------
//
void VToolBar::setMinorGroupSpacing(int minor_spacing)
{
	CheckUnsignedCharRange(minor_spacing);

	minorGroupSpacing = UnsignedChar(minor_spacing);

	if (currentGroup != 0) {
		currentGroup->setPreferredMinorSpacing(minor_spacing);
	}
}

// --------------------------------------------------------------------------
//
int VToolBar::getMinorGroupSpacing() const
{
	return minorGroupSpacing;
}

// --------------------------------------------------------------------------
//
void VToolBar::addSeparator(int thickness)
{
	addSpace(thickness);
}

// --------------------------------------------------------------------------
//
void VToolBar::addSpace(int space)
{
	CheckUnsignedCharRange(space);

	int w, h;
	if (isRow()) { w = space; h = 0; } else { w = 0; h = space; }

	if (currentGroup != 0) {
		(void) new VGeoFrame(w, h, currentGroup);
	}
	else {
		(void) new VGeoFrame(w, h, this);
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::setMargins(int all)
{
	CheckUnsignedCharRange(all);

	majorNearMargin = UnsignedChar(all);
	majorFarMargin  = UnsignedChar(all);
	minorNearMargin = UnsignedChar(all);
	minorFarMargin  = UnsignedChar(all);

	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->setMargins(all);
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::setMargins(int major, int minor)
{
	CheckUnsignedCharRange(major);
	CheckUnsignedCharRange(minor);

	majorNearMargin = UnsignedChar(major);
	majorFarMargin  = UnsignedChar(major);
	minorNearMargin = UnsignedChar(minor);
	minorFarMargin  = UnsignedChar(minor);

	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->setMargins(major, minor);
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::setMargins(int major_near, int major_far,
						  int minor_near, int minor_far)
{
	CheckUnsignedCharRange(major_near);
	CheckUnsignedCharRange(major_far);
	CheckUnsignedCharRange(minor_near);
	CheckUnsignedCharRange(minor_far);

	majorNearMargin = UnsignedChar(major_near);
	majorFarMargin  = UnsignedChar(major_far);
	minorNearMargin = UnsignedChar(minor_near);
	minorFarMargin  = UnsignedChar(minor_far);

	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->setMargins(major_near, major_far, minor_near, minor_far);
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::getMargins(int *major_near, int *major_far,
						  int *minor_near, int *minor_far)
{
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->getMargins(major_near, major_far, minor_near, minor_far);
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::initialize(boolean /* child_indent */ )
{
#ifndef CVS_LATER_DEFAULT
    style					= setTransient + getDefaultStyle();
#endif
	majorSpacing			= 0;
	minorSpacing			= 0;
	majorGroupSpacing		= 0;
	minorGroupSpacing		= 0;
	shadeStyle				= RaisedShade;
	majorNearMargin			= 0;
	majorFarMargin			= 0;
	minorNearMargin			= 0;
	minorFarMargin			= 0;
	currentGroup			= 0;
	currentlyDisabled		= 0;
#if defined(CV_MOTIF)
	appView					= 0;
	isStdMdiViewToolBar		= 0;
#endif
}

// --------------------------------------------------------------------------
//
void VToolBar::construct()
{
	constructStaticData();
	disableFocus();
#ifndef CV_NO_FOCUS_EVENTS
	setTabStop(FALSE);
#endif
	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));

	if (style.contains(StyleVertical)) {
		setupManager(VGeoColumnCls);
	}
	else {
		setupManager(VGeoRowCls);
	}

#ifndef CV_NO_PAINT_EVENTS
	clipToChildren(TRUE);
#endif
}

// --------------------------------------------------------------------------
//
void VToolBar::setupManager(VClass *c)
{
	if (c == VGeoFormCls) {
		setGeoForm();
	}
	else {
		VGeoRowOrColumn *roc = setGeoRowOrColumn();
		roc->setSpacing(VGeoRowOrColumn::NoSpacing);
		roc->setDefaultChildJustify(VGeoRowOrColumn::CenterJustify);
		roc->setPreferredSpacing(majorSpacing);
		roc->setPreferredMinorSpacing(minorSpacing);
		roc->setMargins(majorNearMargin, majorFarMargin,
						minorNearMargin, minorFarMargin);
		roc->setWrapping(FALSE);
		if (c == VGeoColumnCls) {
			roc->setDimension(VGeoRowOrColumn::Column);
		}
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::constructStaticData()
{
	if (objectCount++ > 0) {
		return;
	}
#ifndef CV_NO_PAINT_EVENTS
	thePort = new VPort;
#endif
	theRectangle = new VRectangle;
}

// --------------------------------------------------------------------------
//
void VToolBar::deleteStaticData()
{
	if (--objectCount > 0) {
		return;
	}
#ifndef CV_NO_PAINT_EVENTS
	delete thePort;			thePort			= 0;
#endif
	delete theRectangle;	theRectangle	= 0;
}

// --------------------------------------------------------------------------
//
void VToolBar::getInternalBorderGeometry(int *left, int *right,
										 int *top, int *bottom) const
{
	int internal_border_thickness;

	if (shadeStyle != NoShade) {
		internal_border_thickness = 1;
	}
	else {
		internal_border_thickness = 0;
	}

	if (left   != 0) { *left   = internal_border_thickness; }
	if (right  != 0) { *right  = internal_border_thickness; }
	if (top    != 0) { *top    = internal_border_thickness; }
	if (bottom != 0) { *bottom = internal_border_thickness; }
}

// --------------------------------------------------------------------------
//
boolean VToolBar::paint()
{
#ifndef CV_NO_PAINT_EVENTS
	if (shadeStyle == NoShade) {
		return TRUE;
	}

	if ((thePort == 0) || !thePort->openOn(this)) {
		return FALSE;
	}

	int w, h;
	sizeOfImage(&w, &h);

	VPen *shaded_highlight_pen	= &VColorScheme::
								  systemPen(VColorScheme::ShadedHighlight);
	VPen *shaded_shadow_pen		= &VColorScheme::
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
#endif

	return TRUE;
}

// --------------------------------------------------------------------------
//
void VToolBar::enableBool(boolean b)
{
	currentlyDisabled = !b;
	VOrdCollect *children = childrenOf();
	if (children != 0) {
		DO (*children, VWindow, child)
			child->enableBool(b);
		END
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::updateGeometry(int decide_what_this_means_later)
{
	VGeoRowOrColumn *roc = getGeoRowOrColumn();
	if (roc != 0) {
		roc->update(decide_what_this_means_later);
		//
		// Make sure each of our groups gets updated.
		//
		VOrdCollect *geo_children = roc->getChildren();
		if (geo_children != 0) {
			DO (*geo_children, VGeoFrame, geo_child)
				if (geo_child->isRowOrColumnManager() &&
					(geo_child->getWindow() == 0)) {
					geo_child->update(decide_what_this_means_later);
				}
			END
		}
	}
}

// --------------------------------------------------------------------------
//
void VToolBar::getPreferredSize(int *w, int *h) const
{
	if (w != 0) { *w = 0; }
	if (h != 0) { *h = 0; }
}

// --------------------------------------------------------------------------
//
VStyle& VToolBar::getDefaultStyle() const
{
	return getClassDefaultStyle();
}

// --------------------------------------------------------------------------
//
VStyle& VToolBar::getClassDefaultStyle()
{
	static VWinStyle CV_NEARDATA default_style(VControl::getClassDefaultStyle() +
								   setTransient + StyleNoBorder);
	return default_style;
}

// ==========================================================================
// This following is for backward compatibility with old VToolBar only!
// ==========================================================================

// --------------------------------------------------------------------------
// For backward compatibility with old VToolBar.
//
VToolBar::VToolBar(const VFrame& frame, VWindow *parentwin, boolean autoplace)
{
	initialize();
	(*this)(frame, parentwin, StyleDefault);
	construct();
	setAutoPlace(autoplace);
}

// --------------------------------------------------------------------------
// For backward compatibility with old VToolBar.
//
VToolBar::VToolBar(const VString& name,
				   const VFrame& frame, VWindow *parentwin, boolean autoplace)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, StyleDefault, name);
	construct();
	setAutoPlace(autoplace);
}

// --------------------------------------------------------------------------
// For backward compatibility with old VToolBar.
//
void VToolBar::setAutoPlace(boolean auto_place, short child_gap)
{
	if (!auto_place) {
		setupManager(VGeoFormCls);
	}
	else if (getGeoRowOrColumn() == 0) {
		setupManager();
	}
	setSpacing(child_gap);
}

// --------------------------------------------------------------------------
// For backward compatibility with old VToolBar.
//
VSeparator *VToolBar::newSeparator(int width)
{
	addSeparator(width);
	return 0;
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VToolBar::putTo(VArchiver& a)
{
	VToolBar::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VToolBar::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VControl::putTo(a, FALSE);

	// Write the object version tag.

	a << VToolBarArchiveTag;

	// Write any object specific data.

    if (VToolBarTagRev < 1) {
		a << short(majorSpacing);
		boolean child_indent = FALSE;
		a << child_indent;
		boolean auto_place = TRUE;
		a << auto_place;
	}
	else {
		a << short(majorSpacing);
		a << short(minorSpacing);
		a << short(majorGroupSpacing);
		a << short(minorGroupSpacing);
		a << short(shadeStyle);
		a << short(majorNearMargin);
		a << short(majorFarMargin);
		a << short(minorNearMargin);
		a << short(minorFarMargin);
	}

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VToolBar::getFrom(VArchiver& a)
{
	// Read the base object data.

	VControl::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VToolBarArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VToolBarArchiveTag));
		return;
	}

	// Read any object specific data.

    if (getArchiveTagRev(tag) < 1) {

		short child_gap;
		a >> child_gap;

		boolean child_indent;
		a >> child_indent;

		boolean auto_place;
		a >> auto_place;

		setAutoPlace(auto_place, child_gap);
	}
	else {
		short s;
		a >> s; majorSpacing	  = (unsigned char)s;
		a >> s; minorSpacing	  = (unsigned char)s;
		a >> s; majorGroupSpacing = (unsigned char)s;
		a >> s; minorGroupSpacing = (unsigned char)s;
		a >> s; shadeStyle		  = (unsigned char)s;
		a >> s; majorNearMargin	  = (unsigned char)s;
		a >> s; majorFarMargin	  = (unsigned char)s;
		a >> s; minorNearMargin	  = (unsigned char)s;
		a >> s; minorFarMargin	  = (unsigned char)s;
	}
}

// ---------------------------------------------------------------------------
//
void VToolBar::getFrom(VArchiver& a, VObject *parentwin, boolean do_realize)
{
	VToolBar::getFrom(a);

	if (do_realize) {
		VToolBar::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VToolBar::realize(VWindow *parentwin)
{
	VControl::realize(parentwin);

	constructStaticData();

	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));

	frameChildren();
}

#endif // !CV_NOARCHIVER
