// $Header:   Q:/views/common/vcs/docktool.cpv   1.16   25 Feb 1997 10:18:54   tomf  $

//	docktool.cpp
//
//	VDockableToolBar implementation [Common]
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
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	11/20/95 dgm	Changed to switch between row/column depending on whether
//					we're docked in a horizontal/vertical VDockSiteWindow.
//					Added getDockedSize().
//	11/29/95 dgm	Added beginDrag()/endDrag() & docked()/undocked().
//	11/29/95 dgm	Added enableBool().
//	03/21/96 dgm	Archiver support.
//	04/17/96 dgm	Minor changes to suppress conversion warnings.
//	02/04/97 pkt	Made the implicit handling of styles transient.
//	01/12/97 dgm	Disable focus.
// --------------------------------------------------------------------------

#include "docktool.h"
#include "toolbar.h"
#include "georoc.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VDockableToolBar, VDockableWindow)
defineArchiveRevision(VDockableToolBar, 1)

// --------------------------------------------------------------------------
//
extern VClass *VDockSiteWindowCls;

// --------------------------------------------------------------------------
//
VDockableToolBar::VDockableToolBar()
	: VDockableWindow()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VDockableToolBar::VDockableToolBar(VWindow *parentwin, const VStyle& style)
	: VDockableWindow()
{
	initialize();
	(*this)(parentwin, style);
}

// --------------------------------------------------------------------------
//
VDockableToolBar::VDockableToolBar(const VString& name,
								   VWindow *parentwin, const VStyle& style)
	: VDockableWindow()
{
	initialize();
	(*this)(parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VDockableToolBar::VDockableToolBar(const VFrame& frame,
								   VWindow *parentwin, const VStyle& style)
	: VDockableWindow()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VDockableToolBar::VDockableToolBar(const VString& name,
								   const VFrame& frame, VWindow *parentwin,
								   const VStyle& style)
	: VDockableWindow()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::operator()(VWindow *parentwin)
{
	(*this)(VFrame(), parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::operator()(VWindow *parentwin, const VStyle& style)
{
	(*this)(VFrame(), parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::operator()(VWindow *parentwin, const VStyle& style,
								  const VString& name)
{
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::operator()(const VFrame& frame, VWindow *parentwin,
								  const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::operator()(const VFrame& frame, VWindow *parentwin,
								  const VStyle& style, const VString& name)
{
	VDockableWindow::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VDockableToolBar::~VDockableToolBar()
{
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::initialize()
{
	toolBar = 0;
	explicitWrapping = 0;
	explicitShading = 0;
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::construct()
{
	// The tool-bar is created in newClientWindow() which
	// is called from the base class (i.e. VDockableWindow).

	if (!style.contains(StyleVertical) && !style.contains(StyleHorizontal)) {
#ifdef CVS_USE_TRANSIENT
		style += setTransient + StyleHorizontal;
#else
		style += StyleHorizontal;
#endif
	}

	disableFocus();
	clipToChildren(TRUE);
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::setWrapping(boolean b)
{
	explicitWrapping = b;
	if (toolBar != 0) {
		toolBar->setWrapping(b);
	}
	VGeoManager *parent_manager = getGeoFrame()->getParent();
	if ((parent_manager != 0) && parent_manager->isRowOrColumnManager()) {
		if (b) {
			((VGeoRowOrColumn *)parent_manager)->
				setChildShrinkability(getGeoIndex(), 1.0F);
		}
		else {
			((VGeoRowOrColumn *)parent_manager)->
				setChildShrinkability(getGeoIndex(), 0.0F);
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::beginDrag()
{
	VGeoRowOrColumn *roc = toolBar->getGeoRowOrColumn();
	if (roc != 0) {
		roc->setAssumeWrappingOnGetConstrainedSize(TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::endDrag()
{
	VGeoRowOrColumn *roc = toolBar->getGeoRowOrColumn();
	if (roc != 0) {
		roc->setAssumeWrappingOnGetConstrainedSize(FALSE);
	}
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::dock(VWindow *docksite)
{
	setDockedAttributes(docksite);
	return VDockableWindow::dock(docksite);
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::undock(int x, int y)
{
	setUndockedAttributes();
	return VDockableWindow::undock(x, y);
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::undocked(VWindow *docksite)
{
	return VDockableWindow::undocked(docksite);
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::docked(VWindow *docksite)
{
	if ((toolBar != 0) && (toolBar->getWrapping() == TRUE)) {
		VGeoManager *parent_manager = getGeoFrame()->getParent();
		if ((parent_manager != 0) &&
			 parent_manager->isRowOrColumnManager()) {
			((VGeoRowOrColumn *)parent_manager)->
				setChildShrinkability(getGeoIndex(), 1.0F);
		}
	}
	return VDockableWindow::docked(docksite);
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::getDockedSize(int *w, int *h,
									 int docksite_w, int docksite_h,
									 VWindow *docksite) const
{
	if (docksite != 0) {
		if (docksite->isA(VDockSiteWindowCls)) {
			if (docksite->getStyle().contains(StyleVertical)) {
				if (toolBar->isRow()) {
					toolBar->setColumn();
					determineConstrainedSize(w, h, 1, 0);
					toolBar->setRow();
				}
				else {
					determineConstrainedSize(w, h, 1, 0);
				}
			}
			else if (toolBar->isColumn()) {
				toolBar->setRow();
				determineConstrainedSize(w, h, 0, 1);
				toolBar->setColumn();
			}
			else {
				determineConstrainedSize(w, h, 0, 1);
			}
			return;
		}
		else if (((VWindow *)this)->getStyle().contains(StyleSimpleDock)) {
			VWindow *dockable_window = getDockableWindow();
			if (((VWindow *)this)->getStyle().contains(StyleVertical)) {
				if (toolBar->isRow()) {
					toolBar->setColumn();
					dockable_window->determineConstrainedSize(w, h, 1, 0);
					toolBar->setRow();
				}
				else {
					dockable_window->determineConstrainedSize(w, h, 1, 0);
				}
			}
			else if (toolBar->isColumn()) {
				toolBar->setRow();
				dockable_window->determineConstrainedSize(w, h, 0, 1);
				toolBar->setColumn();
			}
			else {
				dockable_window->determineConstrainedSize(w, h, 0, 1);
			}
			return;
		}
	}

	VDockableWindow::getDockedSize(w, h, docksite_w, docksite_h, docksite);
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::setDockedAttributes(VWindow *docksite)
{
	if (toolBar == 0) {
		return;
	}
	if (docksite == 0) {
		docksite = getLastDockSite();
	}

	if ((docksite != 0) && docksite->isA(VDockSiteWindowCls)) {
		if (toolBar->isRow()) {
			if (docksite->getStyle().contains(StyleVertical)) {
				toolBar->setColumn();
			}
		}
		else if (docksite->getStyle().contains(StyleHorizontal)) {
			toolBar->setRow();
		}
	}

	if (!explicitWrapping) {
		toolBar->setWrapping(FALSE);
	}

	if (!explicitShading) {
		if ((docksite != 0) && docksite->isA(VDockSiteWindowCls)) {
			toolBar->setShadeStyle(VToolBar::NoShade);
		}
		else if (!getStyle().contains(StyleSimpleDock)) {
			toolBar->setShadeStyle(VToolBar::RaisedShade);
		}
	}
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::setUndockedAttributes()
{
	if (toolBar == 0) {
		return;
	}
	toolBar->setRow();
	toolBar->setWrapping(TRUE);
	if (!explicitShading) {
		toolBar->setShadeStyle(VToolBar::NoShade);
	}
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::mouseDn(int mx, int my)
{
	return VDockableWindow::mouseDn(mx, my);
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::mouseDbl(int mx, int my)
{
	return VDockableWindow::mouseDbl(mx, my);
}

// --------------------------------------------------------------------------
//
boolean VDockableToolBar::rmouseDn(int mx, int my)
{
	return VDockableWindow::rmouseDn(mx, my);
}

// --------------------------------------------------------------------------
//
void VDockableToolBar::enableBool(boolean b)
{
	if (toolBar != 0) {
		toolBar->enableBool(b);
	}
}

// --------------------------------------------------------------------------
//
VToolBar *VDockableToolBar::newToolBar(VWindow *parentwin)
{
	VToolBar *toolbar = new VToolBar(parentwin);
	return toolbar;
}

// --------------------------------------------------------------------------
//
VWindow *VDockableToolBar::newClientWindow(VWindow *parentwin)
{
	toolBar = newToolBar(parentwin);
	if (toolBar == 0) {
		toolBar = new VToolBar(parentwin);
	}
	setDockedAttributes(getDockSite());
	if (getStyle().contains(StyleSimpleDock)) {
		toolBar->setShadeStyle(VToolBar::RaisedShade);
	}
	return toolBar;
}

// --------------------------------------------------------------------------
//
VUtilityWindow *VDockableToolBar::newUndockedWindow(VWindow *parentwin)

{
	VUtilityWindow *undocked_window =
		VDockableWindow::newUndockedWindow(parentwin);
	undocked_window->setResizingStyle(VUtilityWindow::PreferredResizing);
	return undocked_window;
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VDockableToolBar::putTo(VArchiver& a)
{
	VDockableToolBar::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VDockableToolBar::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VDockableWindow::putTo(a, FALSE);

	// Write the object version tag.

	a << VDockableToolBarArchiveTag;

	// Write any object specific data.

	if (explicitWrapping) { a << char(1); } else { a << char(0); }

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VDockableToolBar::getFrom(VArchiver& a)
{
	// Read the base object data.

	VDockableWindow::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VDockableToolBarArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VDockableToolBarArchiveTag));
		return;
	}

	// Read any object specific data.

	char c;
	a >> c;
	if (c) { explicitWrapping = 1; } else { explicitWrapping = 0; }
}

// ---------------------------------------------------------------------------
//
void VDockableToolBar::getFrom(VArchiver& a,
							   VObject *parentwin, boolean do_realize)
{
	VDockableToolBar::getFrom(a);

	if (do_realize) {
		VDockableToolBar::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VDockableToolBar::realize(VWindow *parentwin)
{
	VDockableWindow::realize(parentwin);

	VOrdCollect *children = childrenOf();
	if (children != 0) {
		DO (*children, VWindow, child)
			if (child->isA(VToolBarCls)) {
				toolBar = (VToolBar *)child;
				break;
			}
		END
	}
	clipToChildren(TRUE);
}

#endif // !CV_NOARCHIVER
