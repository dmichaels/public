// $Header:   Q:/views/common/vcs/STATUSBR.CPV   1.14   Mar 24 1997 16:13:22   scottt  $

//	statusbr.cpp
//
//	VStatusBar implementation [Common]
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
//	07/12/93 dgm	redone.
//	07/20/93 jmd	moved to library
//	07/21/93 dgm	return TRUE from frameChildren; oversight.
//	09/20/93 dgm	removed static members.
//	09/26/93 jmd	changed rgbColor to VColor
//	05/16/94 dgm	Added isStdMdiViewStatusBar.
//	06/08/94 dgm	Changed paint() to ignore hidden and too-small children.
//	07/19/94 dgm	Updated to use setAutoErased().
//	08/09/94 dgm	Added archiver support.
//	08/23/94 dgm	Minor SGI changes.
//	11/11/94 dss	Archiver tags macros.
//	05/17/95 pkt	preened and merged.
//	05/25/95 dgm	added #ifdef MOTIF for VStdMdiAppView::removeStatusBar().
//	11/14/95 dgm	Fixed up frameChildren() for the new geometry management;
//					we never should have been referencing VAttach members
//					directly!
//	11/17/95 dgm	Changed to not delete the font; doing a setFont() hands
//					over deletion responsibility to the base class now.
//	12/05/95 dgm	Moved getSurfaceColor() here.
//	09/17/96 dgm	Fixed up colors a bit; this *really* needs to be rewritten.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "statusbr.h"
#include "clrscm.h"
#include "control.h"
#include "port.h"
#include "brush.h"
#include "font.h"
#ifndef CV_NO_PAINT_EVENTS
#include "shade.h"
#endif
#include "string.h"
#include "attach.h"
#include "ordcllct.h"
#include "iterator.h"
#include "rect.h"
#ifndef CV_NO_MDI
#include "stdmdiap.h"
#endif

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "callback.h"
#endif

#if defined(CV_HAS_SET_AUTO_ERASED)
#define CV_USE_SET_AUTO_ERASED
#endif

// ---------------------------------------------------------------------------
// Class identity definitions
// ---------------------------------------------------------------------------

defineClass(VStatusBar, VControl)
defineArchiveRevision(VStatusBar, 0)

// ---------------------------------------------------------------------------
//
VStatusBar::VStatusBar()
{
	initialize();
}

// ---------------------------------------------------------------------------
//
VStatusBar::VStatusBar(const VFrame& frame, VWindow *parent_window,
					   VStatusBar::Style style)
{
	initialize();
	statusBarStyle = style;
	if (parent_window != 0) {
		(*this)(frame, parent_window);
	}
#if defined(CV_USE_SET_AUTO_ERASED)
	this->setAutoErased(TRUE);
	VControl::setBackground(this->getBackground());
#endif
#ifndef CV_NO_FOCUS_EVENTS
	this->setTabStop(FALSE);
#endif
}

// ---------------------------------------------------------------------------
//
void VStatusBar::initialize()
{
	statusBarStyle = Default;
	shade = new VShade;
	shade->setBevelThickness(1);
	leftMargin = 6;
	rightMargin = 6;
	topMargin = 3;
	bottomMargin = 3;
	defaultPanel = 0;
	defaultPanelBackground = 0;
	isStdMdiViewStatusBar = 0;
	appView = 0;
	explicitlySetBackground = 0;
	explicitlySetShades = 0;
	defaultFont = 0;
}

// ---------------------------------------------------------------------------
//
VStatusBar::~VStatusBar()
{
#ifdef MOTIF
	if (this->isStdMdiViewStatusBar && (appView != 0)) {
		((VStdMdiAppView *)appView)->removeStatusBar();
	}
#endif
	delete shade;
	delete defaultPanel;
	delete defaultPanelBackground;
}

// ---------------------------------------------------------------------------
VColor VStatusBar::getSurfaceColor() const
{
	VColor highlight, shadow, surface;

	shade->getColors(&highlight, &shadow, &surface);
	return surface;
}

// ---------------------------------------------------------------------------
//
boolean VStatusBar::erased()
{
	return VControl::erased();
}

// ---------------------------------------------------------------------------
//
boolean VStatusBar::paint()
{
	VColor	dummy_rgb, surface_rgb;

	shade->getColors(&dummy_rgb, &dummy_rgb, &surface_rgb);

	VBrush	brush(surface_rgb);
	VPort	port(this);
	port.useBrush(&brush);

	if (!port.open()) {
		return TRUE;
	}

	int w, h;
	this->sizeOfImage(&w, &h);

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedForeground));
	VRectangle r(CornerDim, 0, 0, w, h);
#if !defined(CV_USE_SET_AUTO_ERASED)
	port.fillRegion(&r, TRUE);
#else
	port.frameRegion(&r);
#endif

	VOrdCollect *children_windows = this->childrenOf();

	if (children_windows != 0) {
		int bevel_thickness = shade->getBevelThickness();
		int double_bevel_thickness = bevel_thickness * 2;
		DO (children_windows, VControl, child)
			if (child->isHidden()) {
				continue;
			}
			int x, y, w, h;
			child->getRel(&x, &y, &w, &h);
			if (w <= 1) {
				continue;
			}
			x -= bevel_thickness;
			y -= bevel_thickness;
			w += double_bevel_thickness;
			h += double_bevel_thickness;
			VRectangle r(CornerDim, x, y, w, h);
			shade->panel(Indented, r, FALSE, port, FALSE);
		END
	}

#if 0  // This shouldn't happen.

	else {
			int w, h;
			this->sizeOfImage(&w, &h);
			w -= leftMargin + rightMargin;
			h -= topMargin + bottomMargin;
			VRectangle r(CornerDim, leftMargin, topMargin, w, h);
			shade->panel(Indented, r, FALSE, port, FALSE);
	}
#endif

	port.close();
	port.useBrush(0);

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VStatusBar::resized(int w, int h)
{
	frameChildren(w, h);
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VStatusBar::frameChildren(int new_w, int new_h, boolean /* test */)
{
	VAttach *child_attach;
	VOrdCollect *children_windows = this->childrenOf();

	if (children_windows == 0) {
		//
		// Here, this status-bar has no children; give it a default one
		// which is simply a text-box which fills the entire status-bar.
		//
		createDefaultPanel();
		children_windows = this->childrenOf();
	}
	else if ((children_windows->count() > 1) && (defaultPanel != 0)) {
		//
		// Here, this status-bar has children, but it previously
		// didn't and had a default one assigned to it (see above);
		// now we can delete it.
		//
		delete defaultPanel;
		defaultPanel = 0;
	}

	// Squish (via top/bottom attachments) the height of each child
	// of this status-bar so that it fits nicely (vertically) into
	// the status-bar, allowing for the appropriate shading.
	//
	// Also, for any child whose left and/or right sides are attached,
	// temporarily mung the attachment offsets so as to shrink the
	// width of the child so as to allow for the appropriate shading.

	int bevel_thickness = shade->getBevelThickness();
	int double_bevel_thickness = bevel_thickness * 2;

	DO (children_windows, VControl, child)

		this->attachWin(child, VAttach::Top, VAttach::Parent,
						bevel_thickness + topMargin);
		this->attachWin(child, VAttach::Bottom, VAttach::Parent,
						bevel_thickness + bottomMargin);

		child_attach = child->getAttach();

		switch (child_attach->attach[VAttach::Left]) {
		case VAttach::Parent:
			child_attach->offset[VAttach::Left] += bevel_thickness +
													leftMargin;
			break;
		case VAttach::Sibling:
			child_attach->offset[VAttach::Left] += double_bevel_thickness;
			break;
		case VAttach::Frame:
		case VAttach::SiblingSame:
			child_attach->offset[VAttach::Left] += bevel_thickness;
			break;
#if 0
		case VAttach::UnAttached:
			this->attachWin(child, VAttach::Left, VAttach::Frame,
							bevel_thickness);
			break;
#endif
		default:
			break;
		}

		child->setAttach(VAttach::Left,
						 child_attach->attach[VAttach::Left],
						 child_attach->offset[VAttach::Left],
						 child_attach->sibling[VAttach::Left]);

		switch (child_attach->attach[VAttach::Right]) {
		case VAttach::Parent:
			child_attach->offset[VAttach::Right] += bevel_thickness +
													rightMargin;
			break;
		case VAttach::Sibling:
			child_attach->offset[VAttach::Right] += double_bevel_thickness;
			break;
		case VAttach::Frame:
		case VAttach::SiblingSame:
			child_attach->offset[VAttach::Right] += bevel_thickness;
			break;
#if 0
		case VAttach::UnAttached:
			this->attachWin(child, VAttach::Right, VAttach::Frame,
							bevel_thickness);
			break;
#endif
		default:
			break;
		}

		child->setAttach(VAttach::Right,
						 child_attach->attach[VAttach::Right],
						 child_attach->offset[VAttach::Right],
						 child_attach->sibling[VAttach::Right]);

	END

	VWindow::frameChildren(new_w, new_h);

	DO (children_windows, VControl, child)

		child_attach = child->getAttach();

		switch (child_attach->attach[VAttach::Left]) {
		case VAttach::Parent:
			child_attach->offset[VAttach::Left] -= bevel_thickness +
												   leftMargin;
			break;
		case VAttach::Sibling:
			child_attach->offset[VAttach::Left] -= double_bevel_thickness;
			break;
		case VAttach::Frame:
		case VAttach::SiblingSame:
			child_attach->offset[VAttach::Left] -= bevel_thickness;
			break;
		case VAttach::UnAttached:
		case VAttach::Child:
			break;
		}

		child->setAttach(VAttach::Left,
						 child_attach->attach[VAttach::Left],
						 child_attach->offset[VAttach::Left],
						 child_attach->sibling[VAttach::Left]);

		switch (child_attach->attach[VAttach::Right]) {
		case VAttach::Parent:
			child_attach->offset[VAttach::Right] -= bevel_thickness +
													rightMargin;
			break;
		case VAttach::Sibling:
			child_attach->offset[VAttach::Right] -= double_bevel_thickness;
			break;
		case VAttach::Frame:
		case VAttach::SiblingSame:
			child_attach->offset[VAttach::Right] -= bevel_thickness;
			break;
		case VAttach::UnAttached:
		case VAttach::Child:
			break;
		}

		child->setAttach(VAttach::Right,
						 child_attach->attach[VAttach::Right],
						 child_attach->offset[VAttach::Right],
						 child_attach->sibling[VAttach::Right]);

	END
	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VStatusBar::createDefaultPanel(boolean do_realize)
{
	if (defaultPanel != 0) {
		if (defaultPanel->hwnd() != 0) {
			return;
		}
		if (!do_realize) {
			return;
		}
		(*defaultPanel)(VFrame(0, 0, 1.0F, 0), this);
	}
	else if (!do_realize) {
		defaultPanel = new VTextBox;
		return;
	}
	else {
		defaultPanel = new VTextBox(VFrame(0, 0, 1.0F, 0), this);
	}
	this->attachWin(defaultPanel, VAttach::Left, VAttach::Parent);
	this->attachWin(defaultPanel, VAttach::Right, VAttach::Parent);
	defaultPanel->setBackground(&VColorScheme::
								 systemBrush(VColorScheme::ShadedBackground));
	defaultPanel->setForeground(VColorScheme::
								systemColor(VColorScheme::ShadedForeground));
	VFont *font = this->getFont();
	if (font != 0) {
		defaultPanel->setFont(font);
	}
}

// ---------------------------------------------------------------------------
//
void VStatusBar::getMinSize(short &w, short &h)
{
	w = h = 1;
}

// ----------------------------------------------------------------------------
//
void VStatusBar::systemColorChanged()
{
	if (!explicitlySetShades && (shade != 0)) {
		shade->setDefaultColors();
		if (defaultPanel != 0) {
			defaultPanel->setBackground
				(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));
			defaultPanel->setForeground
				(VColorScheme::systemColor(VColorScheme::ShadedForeground));
		}
		if (!explicitlySetBackground && (defaultPanelBackground != 0)) {
			defaultPanelBackground->background
				(VColorScheme::systemColor(VColorScheme::ShadedBackground));
		}
	}
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VStatusBar::putTo(VArchiver& a)
{
	VStatusBar::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VStatusBar::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VControl::putTo(a, FALSE);

	// Write the object version tag.

	a << VStatusBarArchiveTag;

	// Write any object specific data.

	a << int(statusBarStyle);
	a << leftMargin;
	a << rightMargin;
	a << topMargin;
	a << bottomMargin;

	if (defaultPanel != 0) {
		a << 1;
	}
	else {
		a << 0;
	}

	if (shade != 0) {
		a << int(shade->getBevelThickness());
	}
	else {
		a << -1;
	}

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VStatusBar::getFrom(VArchiver& a)
{
	// Read the base object data.

	VControl::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;

	if (!archiveTagOk(tag, VStatusBarArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VStatusBarArchiveTag));
		return;
	}

	// Read any object specific data.

	int style;
	a >> style;
	statusBarStyle = VStatusBar::Style(style);

	a >> leftMargin;
	a >> rightMargin;
	a >> topMargin;
	a >> bottomMargin;

	int use_default_panel;
	a >> use_default_panel;

	if (use_default_panel) {
		defaultPanel = new VTextBox;	// flag only! (see realize())
	}

	int bevel_thickness;
	a >> bevel_thickness;

	if (shade != 0) {
		shade->setBevelThickness(bevel_thickness);
	}
}

// ---------------------------------------------------------------------------
//
void VStatusBar::getFrom(VArchiver& a, VObject *parent_window, boolean do_realize)
{
	VStatusBar::getFrom(a);

	if (do_realize) {
		VStatusBar::realize((VWindow *)parent_window);
	}

	if (parent_window != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VStatusBar::realize(VWindow *parentw)
{
	VControl::realize(parentw);

	if (defaultPanel != 0) {
		delete defaultPanel;
		defaultPanel = 0;
		VOrdCollect *childrenw = this->childrenOf();
		if ((childrenw != 0) && (childrenw->count() == 1)) {
			VWindow *child = (VWindow *)(childrenw->idAt(0));
			if ((child != 0) && child->isA(VTextBoxCls)) {
				defaultPanel = (VTextBox *)child;
				defaultPanel->setBackground(this->getBackground());
				VFont *font = this->getFont();
				if (font != 0) {
					defaultPanel->setFont(font);
				}
			}
		}
	}

	frameChildren();
}

#endif // ARCHIVER
