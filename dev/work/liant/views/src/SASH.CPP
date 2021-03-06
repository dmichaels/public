// $Header:   Q:/views/common/vcs/SASH.CPV   1.17   Mar 24 1997 16:13:16   scottt  $ 

//	sash.cpp
//
//	VSash implementation [Common]
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
//	07/02/93 dgm	original.
//	07/19/93 dgm	make sure we're dragging before handling mouseMv.
//	08/09/93 dgm	call update() on the parent after the sash move.
//	08/09/93 jmd	added erased.
//	08/18/93 dgm	fix in VSash::setupConstaints.
//	08/23/93 dgm	added archiver support.
//	08/25/93 dgm	removed static members.
//	09/20/93 dgm	call setStyle() to set the style in contstructor.
//	09/22/93 pat	added archive tag
//
//	01/24/94 dgm	minor fix in key()
//  02/02/94 pat	added archive tag check
//  02/09/94 pat	updated defineArchiveTag use
//	02/27/94 jmd	changed port to thePort, pen to thePen, deleted them
//	07/27/94 dgm	fixed memory leak in destructor.
//	07/29/94 dgm	Removed focus support.
//  08/19/94 jmd	updated shade::line call because of new shade interface
//	08/23/94 dgm	Changed disableFocus() call to setTabStop().
//	09/29/94 jld	delete hCursor and vCursor in class destructor
//	11/11/94 dss	Archiver tags macros.
//
//	01/29/95 dgm	Changed the XOR color to be the XOR of the default
//					foreground and background.
//	05/16/95 pkt	preened and merged.
//	05/25/95 dgm	added #ifdef MOTIF for mouseExit().
//	06/26/95 dgm	Merged in Intersolv changes.
//	10/23/95 dss	Added getInfo() to remove WIN95 native 3D client edge.
//	12/05/95 pkt	disabled keyboard focus for Macintosh.
//	06/12/96 pkt	updated to support port of library to internet server.
//	09/16/96 pkt	updated for WebShop.
//	09/17/96 pkt	updated some more for WebShop.
//	09/17/96 dgm	Fixed up colors a bit; this *really* needs to be rewritten.
//	01/21/97 pkt	fixed so that sash does not move when clicked, unless
//					explicitly dragged to a new position.
//	02/04/97 pkt	added comments.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "sash.h"
#include "notifier.h"
#include "appview.h"
#include "pushbttn.h"
#include "control.h"
#include "port.h"
#include "pen.h"
#include "brush.h"
#ifndef CV_NO_MOUSE_EVENTS
#include "mouscrsr.h"
#endif
#include "attach.h"
#include "ordcllct.h"
#include "iterator.h"
#include "shade.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// ---------------------------------------------------------------------------
// Static data-member definitions
// ---------------------------------------------------------------------------

defineClass(VSash, VControl)
defineArchiveRevision(VSash, 0)

// ---------------------------------------------------------------------------
//
#if defined(MOTIF)
#	define XOR_COLOR	BLACK
#else
#	define XOR_COLOR	WHITE
#endif

// ---------------------------------------------------------------------------
//
VSash::VSash()
{
	initialize();
}

// ---------------------------------------------------------------------------
//
VSash::VSash(const VFrame& frame, VWindow *parent_window, VSash::Style style)
{
	initialize();
	setStyle(style);
	if (parent_window == 0) {
		return;
	}
	VStyle	this_style = StyleDefault;
	// pkt 2/4/97 notes that this has no effect.
	if (style & Hidden) {
		this_style += StyleHidden;
	}
	if (style & Border) {
		this_style += StyleBorder;
	}
	(*this)(frame, parent_window);
#ifndef CV_NO_FOCUS_EVENTS
	setTabStop(FALSE);
#endif
}

// ---------------------------------------------------------------------------
//
VSash::VSash(const VString& name,
			 const VFrame& frame, VWindow *parent_window, VSash::Style style)
{
	initialize();
	setStyle(style);
	if (parent_window == 0) {
		return;
	}
	VStyle	this_style = StyleDefault;
	// pkt 2/4/97 notes that this has no effect.
	if (style & Hidden) {
		this_style += StyleHidden;
	}
	if (style & Border) {
		this_style += StyleBorder;
	}
	(*this)(frame, parent_window, StyleDefault, name);
#ifndef CV_NO_FOCUS_EVENTS
	setTabStop(FALSE);
#endif
}

// ---------------------------------------------------------------------------
//
VSash::~VSash()
{
	delete theShade;
#if defined(CV_USE_STATIC_MEMBERS)
	freeStaticResources();
#else
#ifndef CV_NO_MOUSE_EVENTS
	delete horizontalCursor;
	delete verticalCursor;
#endif
	delete thePen;
	delete thePort;
#endif
}

// ---------------------------------------------------------------------------
//
void VSash::initialize()
{
	sashStyle = Default;
	constrainedBySiblingSashes = TRUE;
	disabledSash = FALSE;
	theShade = 0;

#if defined(CV_USE_STATIC_MEMBERS)

	allocateStaticResources();

#else

	numberOfSashes			= 0;
	horizontalCursor		= 0;
	verticalCursor			= 0;
	thePort					= 0;
	thePen					= 0;
	sashParent				= 0;
	minSashPosition			= 0;
	maxSashPosition			= 0;
	startX					= 0;
	startY					= 0;
	endX					= 0;
	endY					= 0;
	sashX					= 0;
	sashY					= 0;
	sashWidth				= 0;
	sashHeight				= 0;
	mousePosition			= 0;

#ifndef CV_NO_MOUSE_EVENTS
	horizontalCursor = new VMouseCursor(SizeNS);
	verticalCursor = new VMouseCursor(SizeWE);
#endif
	thePort = new VPort;
#if defined(MOTIF)
	thePen = new VPen(VColor(notifier->defaultForeground() ^
							 notifier->defaultBackground()));
#else
	thePen = new VPen(XOR_COLOR);
#endif
	thePort->usePen(thePen);
	thePort->rule(XorRule);

#endif

#if defined(MOTIF)
	this->setAutoErased(TRUE);
#endif
}

// ---------------------------------------------------------------------------
//
void VSash::constrainBySiblingSashes(boolean b)
{
	constrainedBySiblingSashes = b;
}

// ---------------------------------------------------------------------------
//
void VSash::setStyle(VSash::Style style)
{
	int s = sashStyle;

	if (style & Horizontal) {
		s &= ~Vertical;
		s |= Horizontal;
	}
	else if (style & Vertical) {
		s &= ~Horizontal;
		s |= Vertical;
	}
	if (style & NoShade) {
		s &= ~(DipShade | BumpShade);
		s |= NoShade;
		delete theShade;
		theShade = 0;
	}
	else if (style & DipShade) {
		s &= ~(NoShade | BumpShade);
		s |= DipShade;
		if (theShade == 0) {
			theShade = new VShade;
			theShade->setBevelThickness(1);
		}
	}
	else if (style & BumpShade) {
		s &= ~(NoShade | DipShade);
		s |= BumpShade;
		if (theShade == 0) {
			theShade = new VShade;
			theShade->setBevelThickness(1);
		}
	}
	sashStyle = (VSash::Style)s;
}

// ---------------------------------------------------------------------------
//
boolean	VSash::mouseEnter(int /* button_down */ )
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (!disabledSash && !thePort->isOpen()) {
#ifndef CV_NO_MOUSE_EVENTS
		if (sashStyle & Horizontal) {
			this->setCursor(horizontalCursor);
		}
		else {
			this->setCursor(verticalCursor);
		}
#endif
	}
	return TRUE;
}

#ifdef MOTIF

// ---------------------------------------------------------------------------
//
boolean	VSash::mouseExit(int /* button_down */ )
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (!disabledSash && !thePort->isOpen()) {
		this->setCursor(0);
	}
	return TRUE;
}

#endif

// ---------------------------------------------------------------------------
//
boolean VSash::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (disabledSash) {
		return TRUE;
	}
	if (thePort->isOpen()) {
		return TRUE;
	}
	if ((sashParent = this->getParent()) == 0) {
		return TRUE;
	}
	this->getRel(&sashX, &sashY, &sashWidth, &sashHeight);
	setupConstraints();
	disableClipping();
	if (!thePort->openOn(sashParent)) {
		return FALSE;
	}
	if (sashStyle & Horizontal) {
#ifndef CV_NO_MOUSE_EVENTS
		this->setCursor(horizontalCursor);
#endif
		thePen->width(sashHeight);
		startX			= sashX;
		endX			= startX + sashWidth;
#if 1
		startY			= sashY + (sashHeight / 2);
#else
		startY			= sashY;
#endif
		endY			= startY;
		mousePosition	= my;
	}
	else {
		this->setCursor(verticalCursor);
		thePen->width(sashWidth);
#if 1
		startX			= sashX + (sashWidth / 2);
#else
		startX			= sashX;
#endif
		endX			= startX;
		startY			= sashY;
		endY			= startY + sashHeight;
		mousePosition	= mx;
	}
	thePort->moveTo(startX, startY);
	thePort->lineTo(endX, endY);

#ifndef CV_NO_MOUSE_EVENTS
	notifier->captureMouseFor(this);
	notifier->mouseTracking(TRUE);
#endif

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VSash::mouseDbl(int mx, int my)
{
	return this->mouseDn(mx, my);
}

// ---------------------------------------------------------------------------
//
boolean VSash::mouseUp(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (disabledSash) {
		return TRUE;
	}
	if (!thePort->isOpen()) {
		return TRUE;
	}

#ifndef CV_NO_MOUSE_EVENTS
	notifier->mouseTracking(FALSE);
	notifier->releaseCapture();
#endif

	setupForNextPosition(mx, my);

	thePort->close();
	restoreClipping();

	int sash_X = startX;
	int sash_Y = startY;
	if (sashStyle & Horizontal) {
		sash_Y -= sashHeight / 2;
	}
	else {
		sash_X -= sashWidth / 2;
	}

	this->move(sash_X, sash_Y, TRUE);
	sashParent->frameChildren();

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VSash::mouseMv(int mx, int my, int /* button_down */ )
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (disabledSash) {
		return TRUE;
	}
	if (!thePort->isOpen()) {
		return TRUE;
	}
	setupForNextPosition(mx, my);
	thePort->moveTo(startX, startY);
	thePort->lineTo(endX, endY);
	return TRUE;
}

// ---------------------------------------------------------------------------
// VSash::setupForNextPosition
//
// Erases the current rubber line and sets up (startX, endX, startY, endY)
// for the next one; enforces appropriate constraints.
//
void VSash::setupForNextPosition(int mx, int my)
{
	int new_start_x, new_end_x, new_start_y, new_end_y, dummy = 0;

	if (sashStyle & Horizontal) {
		new_start_y = startY + (my - mousePosition);
		if (new_start_y <= minSashPosition) {
			new_start_y = minSashPosition;
			mousePosition = minSashPosition;
			sashParent->mapToScreen(&dummy, &mousePosition);
			this->mapToWin(&dummy, &mousePosition);
		}
		else if (new_start_y >= maxSashPosition) {
			new_start_y = maxSashPosition;
			mousePosition = maxSashPosition;
			sashParent->mapToScreen(&dummy, &mousePosition);
			this->mapToWin(&dummy, &mousePosition);
		}
		else {
			 mousePosition = my;
		}
		new_end_y = new_start_y;
		new_start_x = startX;
		new_end_x = endX;
	}
	else {
		new_start_x = startX + (mx - mousePosition);
		if (new_start_x <= minSashPosition) {
			new_start_x = minSashPosition;
			mousePosition = minSashPosition;
			sashParent->mapToScreen(&mousePosition, &dummy);
			this->mapToWin(&mousePosition, &dummy);
		}
		else if (new_start_x >= maxSashPosition) {
			new_start_x = maxSashPosition;
			mousePosition = maxSashPosition;
			sashParent->mapToScreen(&mousePosition, &dummy);
			this->mapToWin(&mousePosition, &dummy);
		}
		else {
			mousePosition = mx;
		}
		new_end_x = new_start_x;
		new_start_y = startY;
		new_end_y = endY;
	}
	thePort->moveTo(startX, startY);
	thePort->lineTo(endX, endY);
	startY	= new_start_y;
	endY	= new_end_y;
	startX	= new_start_x;
	endX	= new_end_x;
}

// ---------------------------------------------------------------------------
//
boolean VSash::key(int virtual_key, char ascii_key)
{
	if (disabledSash) {
		return TRUE;
	}
	if (!thePort->isOpen()) {
		if (VControl::key(virtual_key, ascii_key)) {
			return TRUE;
		}
		else {
			return drag();
		}
	}

	int mx, my, increment;

	// This keyStatus() stuff is a work-around
	// to an apparent C++/Views bug on MS-Windows.

	if (notifier->keyStatus(K_Left)) {
		virtual_key = K_Left;
	}
	else if (notifier->keyStatus(K_Right)) {
		virtual_key = K_Right;
	}
	else if (notifier->keyStatus(K_Up)) {
		virtual_key = K_Up;
	}
	else if (notifier->keyStatus(K_Down)) {
		virtual_key = K_Down;
	}

	if (notifier->keyStatus(K_Shift) ||
		notifier->keyStatus(K_Meta1)) {
		increment = 5;
	}
	else {
		increment = 1;
	}

	switch (virtual_key) {
	case K_Return:
		notifier->getCursorPos(&mx, &my);
		this->mapToWin(&mx, &my);
		return this->mouseUp(mx, my);
	case K_Escape:
		notifier->mouseTracking(FALSE);
		notifier->releaseCapture();
		thePort->moveTo(startX, startY);
		thePort->lineTo(endX, endY);
		thePort->close();
		break;
	case K_Left:
		if (sashStyle & Vertical) {
			notifier->getCursorPos(&mx, &my);
			notifier->setCursorPos(mx -= increment, my);
			this->mapToWin(&mx, &my);
		}
		break;
	case K_Right:
		if (sashStyle & Vertical) {
			notifier->getCursorPos(&mx, &my);
			notifier->setCursorPos(mx += increment, my);
			this->mapToWin(&mx, &my);
		}
		break;
	case K_Up:
		if (sashStyle & Horizontal) {
			notifier->getCursorPos(&mx, &my);
			notifier->setCursorPos(mx, my -= increment);
			this->mapToWin(&mx, &my);
		}
		break;
	case K_Down:
		if (sashStyle & Horizontal) {
			notifier->getCursorPos(&mx, &my);
			notifier->setCursorPos(mx, my += increment);
			this->mapToWin(&mx, &my);
		}
		break;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VSash::drag()
{
	if (thePort->isOpen()) {
		return TRUE;
	}
	int mx, my;
	notifier->getCursorPos(&mx, &my);
	this->mapToWin(&mx, &my);
	if (sashStyle & Horizontal) {
		my = 0;
	}
	else {
		mx = 0;
	}
	this->mapToScreen(&mx, &my);
	notifier->setCursorPos(mx, my);
	this->mapToWin(&mx, &my);
	this->takeFocus();
	this->mouseDn(mx, my);
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VSash::paint()
{
	VPort port(this);

	if (!port.open()) {
		return TRUE;
	}

	int w, h;
	this->sizeOfImage(&w, &h);

	if (sashStyle & NoShade) {
		if (sashStyle & Horizontal) {
			port.moveTo(0, 0);
			port.lineTo(w, 0);
			port.moveTo(0, h - 1);
			port.lineTo(w, h - 1);
		}
		else {
			port.moveTo(0, 0);
			port.lineTo(0, h);
			port.moveTo(w - 1, 0);
			port.lineTo(w - 1, h);
		}
	}
	else {
		int thickness;
		lineType style;
		int		 sx = 0, sy = 0;
		if (sashStyle & DipShade) {
			if (sashStyle & Horizontal) {
				style = HDip;
				thickness = w;
				sy = h / 2;
			}
			else {
				style = VDip;
				thickness = h;
				sx = w / 2;
			}
		}
		else {
			if (sashStyle & Horizontal) {
				style = HBump;
				thickness = w;
				sy = h / 2;
			}
			else {
				style = VBump;
				thickness = h;
				sx = w / 2;
			}
		}
		if (theShade == 0) {
			theShade = new VShade;
			theShade->setBevelThickness(1);
		}
		theShade->line(style, sx, sy, thickness, port);
	}
	port.close();
	return TRUE;
}

// ---------------------------------------------------------------------------
// for VSash::disableClipping and VSash::restoreClipping
//
static boolean parent_was_clipping;

// ---------------------------------------------------------------------------
//
void VSash::disableClipping()
{
	parent_was_clipping = sashParent->clipToChildren();
	if (parent_was_clipping) {
		sashParent->clipToChildren(FALSE);
	}
}

// ---------------------------------------------------------------------------
//
void VSash::restoreClipping()
{
	if (parent_was_clipping) {
		sashParent->clipToChildren(TRUE);
	}
}

// ---------------------------------------------------------------------------
//
void VSash::getMinSize(short& w, short& h)
{
	w = h = 1;
}

// ---------------------------------------------------------------------------
// VSash::setupConstraints
//
// Set up the (minSashPosition, maxSashPosition) constraints.
// Note that (sashParent, sashX, sashY, sashWidth, sashHeight)
// are assumed to be setup correctly.
//
void VSash::setupConstraints()
{
	VWindow *parent_window = sashParent;

	if (parent_window == 0) {
		return;
	}

	VOrdCollect *children_windows = parent_window->childrenOf();

	if (children_windows == 0) {
		return;
	}

	minSashPosition = -32768;
	maxSashPosition =  32767;

	if (constrainedBySiblingSashes) {
		DO (children_windows, VWindow, child)
			if ((child == this) ||
				!child->isA(VSashCls) ||
			    ((((VSash *)child)->getStyle() & Horizontal) &&
			     (sashStyle & Vertical)) ||
			    ((((VSash *)child)->getStyle() & Vertical) &&
			     (sashStyle & Horizontal))) {
				continue;
			}
			int x, y, w, h;
			child->getRel(&x, &y, &w, &h);
			if (sashStyle & Horizontal) {
				if (((sashX >= x) &&
					 (sashX <= (x + w - 1))) ||
				    (((sashX + sashWidth - 1) >= x) &&
					 ((sashX + sashWidth - 1) <= (x + w - 1)))) {
					if (sashY > y) {
						if (y > minSashPosition) {
							minSashPosition = y + h + 1;
						}
					}
					else if (sashY < y) {
						if (y < maxSashPosition) {
							maxSashPosition = y - sashHeight - 1;
						}
					}
				}
			}
			else if (sashStyle & Vertical) {
				if (((sashY >= y) &&
					 (sashY <= (y + h - 1))) ||
				    (((sashY + sashHeight - 1) >= y) &&
					 ((sashY + sashHeight - 1) <= (y + h - 1)))) {
					if (sashX > x) {
						if (x > minSashPosition) {
							minSashPosition = x + w + 1;
						}
					}
					else if (sashX < x) {
						if (x < maxSashPosition) {
							maxSashPosition = x - sashWidth - 1;
						}
					}
				}
			}
		END
	}

	if (minSashPosition == -32768) {
		minSashPosition = 1;
	}
	if (maxSashPosition == 32767) {
		int w, h;
		parent_window->sizeOfImage(&w, &h);
		if (sashStyle & Horizontal) {
			maxSashPosition = h - sashHeight - 1;
		}
		else {
			maxSashPosition = w - sashWidth - 1;
		}
	}
}

// ---------------------------------------------------------------------------
// Make sure that background gets erased using window brush
//
boolean VSash::erased()
{
	return VWindow::erased();
}

// ---------------------------------------------------------------------------
//
void VSash::getInfo(VWinInfo& info)
{
	VControl::getInfo(info);

#if defined(CV_WIN32) && !defined(CV_REMOTE_GUI)
	// No native 3D effects. 3D effects are rendered by the class.
	info.exStyle &= ~WS_EX_CLIENTEDGE;
#elif defined(V_MACINTOSH)
	info.shunsFocus = TRUE;
#endif
}

// ----------------------------------------------------------------------------
//
void VSash::systemColorChanged()
{
	if (theShade != 0) {
		theShade->setDefaultColors();
	}
}

#if defined(CV_USE_STATIC_MEMBERS)

// ---------------------------------------------------------------------------
//
void VSash::allocateStaticResources()
{
	if (++numberOfSashes != 1) {
		return;	
	}
#ifndef CV_NO_MOUSE_EVENTS
	horizontalCursor = new VMouseCursor(SizeNS);
	verticalCursor = new VMouseCursor(SizeWE);
#endif
	thePort = new VPort;
#if defined(MOTIF)
	thePen = new VPen(VColor(notifier->defaultForeground() ^
							 notifier->defaultBackground()));
#else
	thePen = new VPen(XOR_COLOR);
#endif
	thePort->usePen(thePen);
	thePort->rule(XorRule);
}

// ---------------------------------------------------------------------------
//
void VSash::freeStaticResources()
{
	if (numberOfSashes-- != 1) {
		return;
	}
#ifndef CV_NO_MOUSE_EVENTS
	delete horizontalCursor;
	delete verticalCursor;
#endif
	delete thePort;
	delete thePen;
}

#endif

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VSash::putTo(VArchiver& a)
{
	VSash::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VSash::putTo(VArchiver& a, boolean most_derived)
{
	VControl::putTo(a, FALSE);
	a << VSashArchiveTag;
	a << (int)sashStyle;
	a << (int)disabledSash;
	a << (int)constrainedBySiblingSashes;
	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VSash::getFrom(VArchiver& a)
{
	VControl::getFrom(a);

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VSashArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VSashArchiveTag));
		return;
	}

	int i;
	a >> i; sashStyle = (VSash::Style)i;
	a >> i; disabledSash = (boolean)i;
	a >> i; constrainedBySiblingSashes = (boolean)i;
}

// ---------------------------------------------------------------------------
//
void VSash::getFrom(VArchiver& a, VObject *parent_window, boolean do_realize)
{
	VSash::getFrom(a);
	if (do_realize) {
		VSash::realize((VWindow *)parent_window);
	}
	if (parent_window != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VSash::realize(VWindow *parent_window)
{
	VControl::realize(parent_window);
}

#endif
