// $Header:   Q:/views/common/vcs/shdctrl.cpv   1.12   May 30 1997 17:16:26   scottt  $ 

//	shdctrl.cpp
//
//	VShadeControl implementation [Common]
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
//	04/14/93 jmd	created
//	08/09/93 jmd	made VFrame const
//  08/09/93 jmd	removed free
//	09/21/93 dgm	added archiver support.
//	09/23/93 dgm	added archive tag.
//	12/18/93 jmd	added beville save/load, upped archive tag #.
//  02/02/94 pat	added archive tag check
//  02/09/94 pat	updated defineArchiveTag use
//  02/27/94 jmd	updated shade::line call because of new shade interface
//	05/20/94 dgm	Added virtSetupBackground().
//	06/02/94 dgm	Simplified ?: expressions in function calls to
//					work-around SPARCworks C++ compiler (4.0) bug.
//	06/09/94 dgm	Disabled focus.
//	08/23/94 dgm	Changed disableFocus() call to setTabStop().
//	11/11/94 dss	Archiver tags macros.
//  01/11/95 evc    Added GetInfo so shade has static native Windows class
//	05/17/95 pkt	preened and merged.
//	06/26/95 dgm	Added Intersolv change to fill respect the bevel thickness
//					for the shaded lines which are thicker than twice the
//					bevel thickness.
//	10/18/95 dgm	Added getInfo() to avoid duplicate 3D.
//	11/15/95 dgm	Added getInternalBorderGeometry().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "shdctrl.h"
#include "port.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VShadeControl, VControl)
//
// revision 0
// revision 1	added bevelThickness
//
defineArchiveRevision(VShadeControl, 1)

// ---------------------------------------------------------------------------
//
VShadeControl::VShadeControl(const VFrame& f, VWindow *parent_window,
							 ShadeStyle ss) :
	style(ss)
{
	(*this)(f, parent_window);
	setTabStop(FALSE);
}

// ---------------------------------------------------------------------------
//
VShadeControl::VShadeControl() :
	style(VShadeControl::Raised)
{
	setTabStop(FALSE);
}

// ---------------------------------------------------------------------------
//
VShadeControl::~VShadeControl()
{
}

// ---------------------------------------------------------------------------
// return TRUE if the current style is a panel, FALSE if it's a line
//
boolean VShadeControl::isPanel()
{
	return (style == VShadeControl::Raised ||
			style == VShadeControl::Indented ||
			style == VShadeControl::Flat ||
			style == VShadeControl::Pressed);
}

// ---------------------------------------------------------------------------
//
boolean VShadeControl::erased()
{
	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VShadeControl::getMinSize(short &w, short &h)
{
	w = 2;
	h = 2;
}

// ---------------------------------------------------------------------------
//
boolean VShadeControl::paint()
{
	VPort port(this);

	if (!port.open()) {
		return FALSE;
	}

	int w, h;
	sizeOfImage(&w, &h);

	switch (style) {
	case HDip:
		{
			if (theShade.getBevelThickness() * 2  < h) {
				VRectangle r(CornerDim, 0, 0, w, h);
				theShade.panel(::Flat, r, 0, port);
			}
			theShade.line(::HDip, 0, h / 2, w, port);
			break;
		}
	case HBump:
		{
			if (theShade.getBevelThickness() * 2  < h) {
				VRectangle r(CornerDim, 0, 0, w, h);
				theShade.panel(::Flat, r, 0, port);
			}
			theShade.line(::HBump, 0, h / 2, w, port);
			break;
		}
	case VDip:
		{
			if (theShade.getBevelThickness() * 2  < w) {
				VRectangle r(CornerDim, 0, 0, w, h);
				theShade.panel(::Flat, r, 0, port);
			}
			theShade.line(::VDip, w / 2, 0, h, port);
			break;
		}
	case VBump:
		{
			if (theShade.getBevelThickness() * 2  < w) {
				VRectangle r(CornerDim, 0, 0, w, h);
				theShade.panel(::Flat, r, 0, port);
			}
			theShade.line(::VBump, w / 2, 0, h, port);
			break;
		}
	case Raised:
		{
			VRectangle r(CornerDim, 0, 0, w, h);
			theShade.panel(::Raised, r, 0, port);
			break;
		}
	case Indented:
		{
			VRectangle r(CornerDim, 0, 0, w, h);
			theShade.panel(::Indented, r, 0, port);
			break;
		}
	case Flat:
		{
			VRectangle r(CornerDim, 0, 0, w, h);
			theShade.panel(::Flat, r, 0, port);
			break;
		}
	case Pressed:
		{
			VRectangle r(CornerDim, 0, 0, w, h);
			theShade.panel(::Pressed, r, 0, port);
			break;
		}
	}

	port.close();

	return TRUE;
}

#if defined(MOTIF)
// ---------------------------------------------------------------------------
//
void VShadeControl::virtSetupBackground(VBrush *brush)
{
	if (brush == 0) {
		theShade.setDefaultColors();
	}
	else {
		VControl::virtSetupBackground(brush);
		if (this->backgroundIsPixel) {
			theShade.setColors(VColor(this->backgroundPixel));
		}
	}
}

// ---------------------------------------------------------------------------
//
void VShadeControl::addChild(VWindow *child)
{
	enableFocus();
	VControl::addChild(child);
}

// ---------------------------------------------------------------------------
//
boolean VShadeControl::removeChild(VWindow *child)
{
	boolean return_value = VControl::removeChild(child);
	if ((children == 0) || (children->count() <= 0)) {
		setTabStop(FALSE);
	}
	return return_value;
}
#endif /* MOTIF */

void VShadeControl::getInfo(VWinInfo &wi)
{
	VControl::getInfo(wi);
#if defined(CV_WIN32)
	wi.exStyle &= ~WS_EX_CLIENTEDGE;
#endif
}

// ----------------------------------------------------------------------------
//
void VShadeControl::getInternalBorderGeometry(int *left, int *right,
											  int *top, int *bottom) const
{
	int bevel_thickness = ((VShadeControl *)this)->
						  theShade.getBevelThickness();

	if (left   != 0) { *left   = bevel_thickness; }
	if (right  != 0) { *right  = bevel_thickness; }
	if (top    != 0) { *top    = bevel_thickness; }
	if (bottom != 0) { *bottom = bevel_thickness; }
}

// ----------------------------------------------------------------------------
//
void VShadeControl::systemColorChanged()
{
	theShade.setDefaultColors();
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VShadeControl::putTo(VArchiver& a)
{
	VShadeControl::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VShadeControl::putTo(VArchiver& a, boolean most_derived)
{
	VControl::putTo(a, FALSE);
	a << VShadeControlArchiveTag;
	a << (int)style;

	// jmd 12/18/93
	a << (int)theShade.getBevelThickness();

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VShadeControl::getFrom(VArchiver& a)
{
	VControl::getFrom(a);
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VShadeControlArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VShadeControlArchiveTag));
		return;
	}

	int i;
	a >> i;
	style = (VShadeControl::ShadeStyle)i;

	// jmd 12/18/93
	if (getArchiveTagRev(tag) > 0) {
		// read bevel thickness (not in version 0)
		int bt;
		a >> bt;
		theShade.setBevelThickness(bt);
	}
}

// ---------------------------------------------------------------------------
//
void VShadeControl::getFrom(VArchiver& a, VObject *parent_window, boolean do_realize)
{
	VShadeControl::getFrom(a);
	if (do_realize) {
		VShadeControl::realize((VWindow *)parent_window);
	}
	if (parent_window != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VShadeControl::realize(VWindow *parent_window)
{
	VControl::realize(parent_window);
}

#endif

