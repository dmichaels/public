// $Header:   Q:/views/win/vcs/port.cpv   1.12   Mar 04 1997 17:55:36   gregm  $ 

//	port.cpp
//
//	VPort implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//	09/21/92 jmd	preened
//	10/14/92 jmd	added support for VArc
//	10/21/92 jmd	added fillFrame
//	07/18/93 jmd	preened, removed free()
//	07/18/93 jmd	added new port fill interface
//	07/20/93 jmd	changed rgbColor to VColor
//	08/30/93 jmd	added call to setBackground in clear()
//	09/02/93 dgm	M8 changes; miscellaneous casts.
//	10/04/93 jmd	added rounding to scaleDeviceToLogical
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	01/14/94 pat	preened.
//	03/04/94 pat	Win32 changes (portable to Win 3.x):
//					SetWindow/ViewportOrg/ExtEx(), in useXForm();
//					GetTextExtentPoint(), in wrtText().
//	03/04/94 pat	Win32 changes (portable to Win 3.x):  MoveToEx();
//					replaced NULL with 0.
//	03/26/94 pat	CV_WIN32:  added explicit int cast on POINT and SIZE
//					long members; updated wrtText(); updated cast of style
//					argument for DrawText() calls.
//	04/19/94 jmd	removed extraneous +1s in fillFrame
//	06/08/95 jld	correct rect calculations in writeText()
//	07/18/95 dgm	Miscellaneous const char * changes.
//	10/12/95 pkt	Changed wrtText to use TextOutW in Win32.
//	11/21/95 glm	drawBitMap does not disable viewing transformations
//	09/11/96 dgm	Added support for StipplePat.
//	01/27/97 dgm	Updated drawBitMap() to use new VBitMap implementation.
// ----------------------------------------------------------------------------

#include "bitmap.h"
#include "ellipse.h"
#include "pointrry.h"
#include "port.h"
#include "rndrect.h"
#include "str.h"
#include "winclass.h"
#include "window.h"
#include "icon.h"
#include "arc.h"

extern "C" {
#	include <string.h>
}

defineClass(VPort, VObject)

// ---------------------------------------------------------------------------
// Intialize this `VPort` to its initial settings.
//
void VPort::init()
{
	image = 0;
	clipRgn = 0;
	rastRule = CopyRule;
	mapMode = -1;
	oldPen = oldBrush = oldFont = 0;
	currPen = 0;
	currBrush = 0;
	currFont = 0;
	oldTextClr = 0ul;

	// backGroundClr = GetSysColor(COLOR_WINDOW);
	bkColorEnabled = FALSE;
	hDC = 0;
	usePen(&pen);
	useBrush(&brush);
	useFont(&font);
}

// ---------------------------------------------------------------------------
// Construct a VPort not bound to a VDisplay.
//
VPort::VPort()
{
	init();
}

// ---------------------------------------------------------------------------
// Construct VPort on the VDisplay object 'display'.
//
VPort::VPort(VDisplay *display)
{
	init();
	image = display;
	setArea();
}

// ---------------------------------------------------------------------------
VPort::~VPort()
{
	clean();
	useBrush(0);
	usePen(0);
	useFont(0);
}

// ---------------------------------------------------------------------------
// Set the drawing area to the entire area of the bound VDisplay object.
//
void VPort::setArea()
{
	int w, h;

	image->sizeOfImage(&w, &h);
	area.set(Corners, 0, 0, w, h);
}

// ---------------------------------------------------------------------------
// Close this VPort for output.
//
void VPort::close()
{
	if (hDC) {
		resetPen();
		resetBrush();
		resetFont();
		image->ungetHndl();
	}
	hDC = 0;
}

// ---------------------------------------------------------------------------
// Move to the first point of the VPointArray object 'points', then
// draw lines to the remaining points using the current VPen.
//
void VPort::drawLines(VPointArray *points)
{
	if (hDC != 0) {
		Polyline(hDC, (LPPOINT) points->getData(), (int) points->size());
	}
}

// ---------------------------------------------------------------------------
// Draw the VBitMap 'bm' in this VPort at the position ('x','y')
// using the current rastor-op mode (see rule()).
//
void VPort::drawBitMap(VBitMap *bitmap, int x, int y)
{
	if (bitmap != 0) {
		bitmap->bitblt(image, x, y, rastRule);
	}
}

// ---------------------------------------------------------------------------
// Erase VRegion 'rgn' by painting it with the background color of
// the current VBrush object.
//
void VPort::eraseRegion(VRegion *rgn)
{
	HRGN h;

	if (!hDC) {
		return;
	}
	VColor phue = currPen->penColor;
	VColor bfore = currBrush->fgColor;
	VColor back = currBrush->bkColor;

	currPen->color(back);
	if (currBrush->pattern() != BackgroundPat) {
		currBrush->foreground(back);
	}
	if (h = rgn->hRgn) {
		PaintRgn(hDC, h);
	}
	else {
		fillRegion(rgn, FALSE);
	}
	currPen->color(phue);
	if (currBrush->pattern() != BackgroundPat) {
		currBrush->foreground(bfore);
	}
}

// ---------------------------------------------------------------------------
// Fill in 'rgn' with the current VBrush and draw its border
// with the current VPen if 'border' is TRUE.
//
void VPort::fillRegion(VRegion *rgn, boolean border)
{
	if (!hDC) {
		return;
	}

	int bgMode = SetBkMode(hDC, OPAQUE);

	COLORREF save_text_color;
	COLORREF save_bk_color;

	if (currBrush != 0) {
		save_text_color = GetTextColor(hDC);
		save_bk_color = GetBkColor(hDC);
		SetTextColor(hDC, COLORREF(currBrush->foreground()));
		SetBkColor(hDC, COLORREF(currBrush->background()));
	}

	if (rgn->hRgn || !border) {
		// optimize for objects that have created a region...
		HRGN h;
		h = rgn->getHndl();

		if (h) {	// jmd 02/26/94
			PaintRgn(hDC, h);
		}

		if (border) {
			frameRegion(rgn);
		}
		rgn->ungetHndl();
	}
	else {
		rgn->fill(this);
	}

	SetBkMode(hDC, bgMode);

	if (currBrush != 0) {
		SetTextColor(hDC, save_text_color);
		SetBkColor(hDC, save_bk_color);
	}
}

// ---------------------------------------------------------------------------
// Paint the border of 'rgn' with the current VPen.
//
void VPort::frameRegion(VRegion *rgn)
{
	HANDLE hbr, oldBr;

	if (!hDC) {
		return;
	}

	if (rgn->hRgn) {
		// optimize for objects that have created a region...
		// !!!jmd-what about arc...
		// !!!dgm-this is causing non-solid ellipse frames to be solid.
		int	w, h;
		VBrush frameBrush(ForegroundPat, currPen->penColor, 0ul);
		w = h = currPen->penSize;
		if (mapMode) {
			scaleDeviceToLogical(&w, &h);
		}
		FrameRgn(hDC, rgn->hRgn, frameBrush.hbrush, w, h);
	}
	else {
		hbr = GetStockObject(NULL_BRUSH);
		oldBr = SelectObject(hDC, hbr);

		rgn->fill(this);

		SelectObject(hDC, oldBr);
	}

}

// ---------------------------------------------------------------------------
// Free any auxiliary strage associated with this VPort.
//
void VPort::clean()
{
	if (hDC) {
		close();
	}
	pen.reset();
	brush.reset();
}

// ---------------------------------------------------------------------------
// Return the relative width/height of a pixel.
// NOTE: this VPort must be opened, otherwise zero is returned.
//
float VPort::aspectRatio()
{
	int x, y;
	if (hDC) {
		x = GetDeviceCaps(hDC, ASPECTX);
		y = GetDeviceCaps(hDC, ASPECTY);
		return float ((x * 100.0) / (y * 100.0));
	}
	return float (0.0);
}

// ---------------------------------------------------------------------------
// Invert the pixels within the region 'rgn' in a device specific fashion.
//
void VPort::invertRegion(VRegion *rgn)
{
	if (hDC) {
		rgn->invert(this);
	}
}

// ---------------------------------------------------------------------------
// Return TRUE if this VPort is currently open on a VDisplay object, otherwise
// return FALSE.
//
boolean VPort::isOpen()
{
	return (hDC) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Draw a line from the current drawing position to ('x','y')
// using the current VPen.
//
void VPort::lineTo(int x, int y)
{
	if (!hDC) {
		return;
	}
	LineTo(hDC, x, y);
}

// ---------------------------------------------------------------------------
// Perform the current viewing transformation on 'rect' and return the
// results in 'rect'.  Return TRUE if the transformation is successful, otherwise
// return FALSE.  NOTE: a viewing transformation must have been established and
// enabled prior to using this method.
//
boolean VPort::xForm(VRectangle *rect)
{
	boolean    mMode, b;
	if (!hDC) {
		return FALSE;
	}
	if (!(mMode = mapMode)) {
		useXform(TRUE);
	}
	b = LPtoDP(hDC, (LPPOINT)&rect->left, 2) ? TRUE: FALSE;
	if (!mMode) {
		useXform(FALSE);
	}
	return b;
}

// ---------------------------------------------------------------------------
// Perform the inverse of the current viewing transformation on the
// point ('*x','*y') and return the results in '*x' and '*y', respectively.
// Return TRUE if the transformation is successful, otherwise return FALSE.
// NOTE: a viewing transformation must have been established and enabled prior
// to using this method.
//
boolean VPort::unXform(int *x, int *y)
{
	boolean    mMode, b;

	if (!hDC) {
		return FALSE;
	}
	if (!(mMode = mapMode)) {
		useXform(TRUE);
	}

	POINT  p;
	p.x = *x;  
	p.y = *y;
	b = DPtoLP(hDC, (LPPOINT)&p, 1) ? TRUE: FALSE;

	//	pat--under CV_WIN32 POINT coordinates are longs, we are truncating!	
	
	*x = (int) p.x;   
	*y = (int) p.y;

	if (!mMode) {
		useXform(FALSE);
	}
	return b;
}

// ---------------------------------------------------------------------------
// Move the current drawing position to ('x','y').
//
void VPort::moveTo(int x, int y)
{
	if (!hDC) {
		return;
	}
	MoveToEx(hDC, x, y, (LPPOINT) 0);
}

// ---------------------------------------------------------------------------
// Open this VPort for output on the current VDisplay object. Return
// TRUE if this VPort is successfully opened, otherwise return FALSE;
//
boolean VPort::open()
{
	if (!image)  {
		return FALSE;
	}
	if (hDC) {
		return TRUE;
	}

	setArea();
	if (hDC = image->getHndl()) {
		if (mapMode < 0) {
			int w, h;
			image->sizeOfImage(&w, &h);
			vRect.set(CornerDim, 0, 0, w, h);
			vpRect.set(CornerDim, 0, 0, w, h);
			mapMode = FALSE;
		}
		usePen(currPen);
		useBrush(currBrush);
		useFont(currFont);
		SetROP2(hDC, rastRule);
		SetBkMode(hDC, bkColorEnabled ? OPAQUE: TRANSPARENT);

		// SetBkColor(hDC, backGroundClr);
		if (clipRgn) {
			setClipRgn(clipRgn);
		}
		if (mapMode) {
			useXform(TRUE);
		}
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Open this VPort for output to 'display'. Return TRUE if open was
// successful, otherwise return FALSE.
//
boolean VPort::openOn(VDisplay *display)
{
	image = display;
	return open();
}

// ---------------------------------------------------------------------------
// Reset this VPort back to its initial conditions.
//
void VPort::reset()
{
	boolean opened = FALSE;

	if (hDC) {
		opened = TRUE, close();
	}
	if (clipRgn) {
		clipRgn = 0;
	}

	pen.reset();
	brush.reset();
	usePen(0);
	useBrush(0);
	useFont(0);
	rule(CopyRule);

	if (opened) {
		open();
	}
}

// ---------------------------------------------------------------------------
// Set the clipping region to 'rgn' and return TRUE.  Return FALSE if
// the operation failed.
//
boolean VPort::setClipRgn(VRegion *rgn)
{
	HRGN   h = 0;

	if (!hDC) {
		return FALSE;
	}
	if (rgn)  {
		h = rgn->getHndl();
	}
	else {
		h = area.getHndl();
	}
	if (h) {
		SelectClipRgn(hDC, h);
	}
	if (rgn) {
		rgn->ungetHndl();
	}
	else {
		area.ungetHndl();
	}
	clipRgn = rgn;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the coordinates of the viewing transformation\'s 'view-port' to
// the rectangle defined by (x1,y1) and (x2,y2).  The how is defined
// as per the VRectangle() constructor of the VRectangle class to be
// one of Corners, CenterDim or, CornerDim.
//
void VPort::viewPort(RectOpts how, int x1, int y1, int x2, int y2)
{
	vpRect.set(how, x1, y1, x2, y2);
	if (mapMode) {
		useXform(TRUE);
	}
}

// ---------------------------------------------------------------------------
// Set the coordinates of the viewing transformation\'s 'view-port' to
// the VRectangle object 'rect'.
//
void VPort::viewPort(VRectangle *rect)
{
	vpRect.set(Corners, rect->left, rect->top, rect->right, rect->bottom);
	vRect.hRgn = 0;
	if (mapMode) {
		useXform(TRUE);
	}
}

// ---------------------------------------------------------------------------
// Set the coordinates of the viewing transformation\'s 'view-window'
// to the rectangle defined by (x1,y1) and (x2,y2).  The how is
// defined as per the VRectangle() constructor of the VRectangle class
// to be one of Corners, CenterDim, or CornerDim.
//
void VPort::viewWindow(RectOpts how, int x1, int y1, int x2, int y2)
{
	vRect.set(how, x1, y1, x2, y2);
	if (mapMode) {
		useXform(TRUE);
	}
}

// ---------------------------------------------------------------------------
// Set the coordinates of the viewing transformation\'s 'view-window' to
// the VRectangle object 'rect'.
//
void VPort::viewWindow(VRectangle *rect)
{
	vRect.set(Corners, rect->left, rect->top, rect->right, rect->bottom);
	vRect.hRgn = 0;
	if (mapMode) {
		useXform(TRUE);
	}
}

// ---------------------------------------------------------------------------
// Perform the viewing transformation if 'enable' is TRUE, otherwise no
// transformation is performed.
//
boolean VPort::useXform(boolean enable)
{
	if (!hDC)  {
		return FALSE;
	}
	if (enable) {
		int  x1, x2, y1, y2, w, h;
		SetMapMode(hDC, MM_ANISOTROPIC);
		vRect.get(Corners, &x1, &y1, &x2, &y2);

		SetWindowExtEx(hDC, 	x2 - x1, 	y2 - y1, 	0);
		SetWindowOrgEx(hDC, 	x1, 		y1, 		0);

		vpRect.get(Corners, &x1, &y1, &x2, &y2);
		w = x2 - x1;  h = y2 - y1;

		SetViewportExtEx(hDC, 	w, 	h, 	0);
		SetViewportOrgEx(hDC, 	x1, y1,	0);
	}
	else {
		SetMapMode(hDC, MM_TEXT);

		SetWindowExtEx(hDC, 	1, 1, 0);
		SetWindowOrgEx(hDC, 	0, 0, 0);
		SetViewportExtEx(hDC, 	1, 1, 0);
		SetViewportOrgEx(hDC, 	0, 0, 0);
	}
	mapMode = enable;
	useFont(currFont);
	return TRUE;
}

// ---------------------------------------------------------------------------
// Perform the inverse of the current viewing transformation on 'rect'.
// Return TRUE if the transformation is successful, otherwise return FALSE.
// NOTE: a viewing transformation must have been established and enabled prior
// to using this method.
//
boolean VPort::unXform(VRectangle *rect)
{
	boolean    mMode, b;

	if (!hDC) {
		return FALSE;
	}
	if (!(mMode = mapMode)) {
		useXform(TRUE);
	}

	b = DPtoLP(hDC, (LPPOINT)&rect->left, 2);

	if (!mMode) {
		useXform(FALSE);
	}
	return b;
}

// ---------------------------------------------------------------------------
// Perform the current viewing transformation on the point ('*x','*y')
// and return the results in '*x' and '*y', respectively.  Return TRUE if the
// transformation is successful, otherwise return FALSE.  NOTE: a viewing
// transformation must have been established and enabled prior to using
// this method.
//
boolean VPort::xForm(int *x, int *y)
{
	boolean    mMode, b = FALSE;

	if (!hDC) {
		return FALSE;
	}
	if (!(mMode = mapMode)) {
		useXform(TRUE);
	}

	POINT  p;
	p.x = *x;  
	p.y = *y;
	b = LPtoDP(hDC, (LPPOINT)&p, 1);

	//	pat--under CV_WIN32 POINT coordinates are longs, we are truncating!	

	*x = (int) p.x;   
	*y = (int) p.y;

	if (!mMode) {
		useXform(FALSE);
	}
	return b;
}

// ---------------------------------------------------------------------------
// Write the contents of the VString object 's' at the specified ('x','y')
// position using the current VFont object. The upper left corner of the first
// character is placed at ('x','y').  Return the length of the written string
// in pixels.
//
int VPort::wrtText(const VString *s, int x, int y)
{
	const char *t;

	if (s) {
		t =  s->gets();
		return wrtText(t, x, y);
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Write the C string 's' at the specified ('x','y') position using the
// current VFont object.  The upper left corner of the first character is
// placed at ('x','y').  Return the length of the written string in pixels.
//
int VPort::wrtText(const char *s, int x, int y)
{
	int n;

	if (s && hDC) {
#if defined(CV_WIN32)
		// Check for Unicode font.  Japanese text was not displayed
		// correctly with Unicode font, absent this change.
	    if (currFont != 0) {
	    	if (!currFont->actualLog) {
	        	currFont->getActual();
			}
	    	// if (currFont->lf.lfCharSet == UNICODE_CHARSET) {
	    	if (TRUE) {
				VString text = s;
				n = text.getCharSize();
				TextOutW(hDC, x, y, text.getwcs(), n);

				SIZE textExtent;
				if (GetTextExtentPointW(hDC, text.getwcs(), (int) n, (LPSIZE) &textExtent)) {
					return int(textExtent.cx);
				}
			}
		}
#endif
		n = strlen(s);
		TextOut(hDC, x, y, (LPSTR)s, n);

		SIZE textExtent;
		if (GetTextExtentPoint(hDC, (LPCSTR) s, (int) n, (LPSIZE) &textExtent)) {
			return int(textExtent.cx);
		}
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Set the current VBrush of this VPort to 'b'. If 'b' is 0, then
// use the default VBrush.
//
boolean VPort::useBrush(VBrush *b)
{
	HANDLE ob = 0;

	if (!b) {
		return useBrush(&brush);
	}

	if (currBrush != b) {
		b->bind(this, TRUE);
		b->pReset = methodOf(VPort, resetBrush);
		b->pSet = methodOf(VPort, useBrush);
		if (currBrush) {
			currBrush->bind(this, FALSE);
		}
		currBrush = b;
	}

	if (hDC) {
		ob = SelectObject(hDC, b->hbrush);
		backGroundClr = b->bkColor;
		SetBkColor(hDC, b->bkColor);
		if (!oldBrush) {
			oldBrush = ob;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the current VPen of this VPort to 'p'.  If 'p' is 0, then
// use the default VPen.
//
boolean VPort::usePen(VPen *p)
{
	HANDLE op = 0;
	VColor otc = 0ul;

	if (!p) {
		return usePen(&pen);
	}
	if (currPen != p) {
		p->bind(this, TRUE);
		p->pReset = methodOf(VPort, resetPen);
		p->pSet = methodOf(VPort, usePen);
		if (currPen) {
			currPen->bind(this, FALSE);
		}
		currPen = p;
	}

	if (hDC) {
		op = SelectObject(hDC, p->hpen);
		otc = SetTextColor(hDC, p->penColor);
		if (!oldPen) {
			oldPen = op;
		}
		if (!oldTextClr) {
			oldTextClr = otc;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the current VFont of this VPort to 'f'.  If 'f' is 0,
// then use the default VFont.
//
boolean VPort::useFont(VFont *f)
{
	//	Use the default font if the given font is null

	if (f == 0) {
		return useFont(&font);
	}

	//	Initialize binding of a new font
	
	if (currFont != f) {
		//	bind the given font to this port
		f->bind(this, TRUE);

		//	unbind the current font
		if (currFont) {
			currFont->bind(this, FALSE);
		}
	
		//	when the font changes or is destroyed this port must be
		//	notified. attach the methods by which the font can set 
		//	and reset the font of this port
			
		f->pSet 	= methodOf(VPort, useFont);
		f->pReset 	= methodOf(VPort, resetFont);

		//	make the given font the current font
		currFont 	= f;
	}

	//	Select the font into the current device context

	if (hDC) {
		resetFont();			//	select old font, if present
		
		//	attach method by which font can scale its device
		//	coordinates to logical coordinates in accord with the 
		//	viewing transformation associated with this port

		f->scale 	= methodOf(VPort, scaleDeviceToLogical);

		//	ask the font to select itself into our device context

		HANDLE of 	= f->select(hDC, this);

		if (oldFont == 0) {		//	keep old font for reselection
			oldFont = of;
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the drawing mode of this VPort to the rastor-op mode 'rop',
// where 'rop' is one of: enum RasterRule: CopyRule, OrRule, XorRule,
// NoCopyRule, NotOrRule, NotXorRule, EraseRule, AndRule.
//
void VPort::rule(RasterRule rop)
{
	rastRule = rop;
	if (hDC) {
		SetROP2(hDC, rop);
	}
}

// ---------------------------------------------------------------------------
// Clear the current VDisplay object to the background color
// of the current VBrush.
//
void VPort::clear()
{
	if (hDC) {
		VBrush b, *bp = currBrush;
		b.foreground(bp->bkColor);
		b.background(bp->bkColor);		// jmd 8-30-93
		useBrush(&b);
		eraseRegion(&area);
		useBrush(bp);
	}
}

// ---------------------------------------------------------------------------
// Return the current VPen object used by this VPort.
//
VPen *VPort::currentPen()
{
	return currPen;
}

// ---------------------------------------------------------------------------
// Return the current VBrush object used by this VPort.
//
VBrush *VPort::currentBrush()
{
	return currBrush;
}

// ---------------------------------------------------------------------------
// Return the current VFont object used by this VPort.
//
VFont *VPort::currentFont()
{
	return currFont;
}

// ---------------------------------------------------------------------------
// Place the number of pixels per inch along the device width and
// height in '*pixX' and '*pixY', respectively and return TRUE.
// NOTE: this VPort must be opened, otherwise FALSE is returned.
//
boolean VPort::pixelsPerInch(int *pixX, int *pixY)
{
	if (!hDC)  {
		return FALSE;
	}
	*pixX = GetDeviceCaps(hDC, LOGPIXELSX);
	*pixY = GetDeviceCaps(hDC, LOGPIXELSY);
	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VPort::resetPen()
{
	if (oldPen && hDC) {
		SelectObject(hDC, oldPen);
	}
	if (oldTextClr && hDC) {
		SetTextColor(hDC, oldTextClr);
	}
	oldPen = 0;
	oldTextClr = 0ul;
	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VPort::resetBrush()
{
	if (oldBrush && hDC) {
		SelectObject(hDC, oldBrush);
	}
	oldBrush = 0;
	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VPort::resetFont()
{
	if (oldFont && hDC) {
		SelectObject(hDC, oldFont);
	}
	oldFont = 0;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return the amount of space required between lines of text being
// written with the current VFont object.  The returned value can be used as
// a simple increment of the y coordinate when writing multiple lines of text
// using wrtText().  The value returned is in pixels and it includes the text
// height plus the external leading.  NOTE: this VPort must be opened,
// otherwise a value of zero is returned.
//
int VPort::textLineSpace()
{
	TEXTMETRIC  tm;

	if (hDC) {
		GetTextMetrics(hDC, &tm);
		return tm.tmHeight + tm.tmExternalLeading;
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
// Scale the point ('*x','*y') into logical units using the inverse
// of the current viewing transformation and return the results in '*x' and
// '*y', respectively.  Return TRUE if the transformation is successful, otherwise
// return FALSE.  NOTE: a viewing transformation must have been established
// and enabled prior to using this method.
//
boolean VPort::scaleDeviceToLogical(int *x, int *y)
{
	int wXo, wYo, wXe, wYe;
	int vXo, vYo, vXe, vYe;

	if (!mapMode) {
		return FALSE;
	}
	vRect.get(CornerDim, &wXo, &wYo, &wXe, &wYe);
	vpRect.get(CornerDim, &vXo, &vYo, &vXe, &vYe);

	// scale both values from device coords to logical coords.
	*x = (int)((float) *x * ((float)wXe /(float)vXe) + 0.5);
	*y = (int)((float) *y * ((float)wYe /(float)vYe) + 0.5);
	return TRUE;
}

// ---------------------------------------------------------------------------
// Scale the logical point ('*x','*y') into device units using the
// current viewing transformation and return the results in '*x' and '*y',
// respectively.  Return TRUE if the transformation is successful, otherwise
// return FALSE.  NOTE: a viewing transformation must have been established
// and enabled prior to using this method.
//
boolean VPort::scaleLogicalToDevice(int *x, int *y)
{
	int wXo, wYo, wXe, wYe;
	int vXo, vYo, vXe, vYe;

	if (!mapMode) {
		return FALSE;
	}
	vRect.get(CornerDim, &wXo, &wYo, &wXe, &wYe);
	vpRect.get(CornerDim, &vXo, &vYo, &vXe, &vYe);

	// scale both values from logical coords to device coords.
	*x = (int)((float) *x * ((float)vXe /(float)wXe));
	*y = (int)((float) *y * ((float)vYe /(float)wYe));
	return TRUE;
}

// ---------------------------------------------------------------------------
// If 'enable' is TRUE, then enable the use of this VPort\'s background
// color for drawing styled lines and writing text.  Otherwise, if 'enable'
// is FALSE, then drawing styled lines and writing text will not remove
// any background color of the VDisplay object.  Lines are drawn using
// the current VPen object\'s color and pattern and, if enabled, the
// background color is used to fill the gaps of patterned lines.  Also, if
// enabled, the background color is used to fill the background of character
// cells of text written with the current VFont object. The default for a VPort
// object is to have the use of the background color disabled. NOTE: the
// VPort object\'s background color is always the background color of the
// current VBrush object.
//
void VPort::enableBkColor(boolean enable)
{
	bkColorEnabled = enable;
	if (hDC) {
		SetBkMode(hDC, enable ? OPAQUE: TRANSPARENT);
	}
}

// ---------------------------------------------------------------------------
// Write formatted text, given in the C string 's', in VRectangle
// 'rect' at the specified ('x','y') position using the current VFont object.
// Return the length of the written string in pixels.  Text formatting is
// specified with one or more of the following defined values or\'ed together:
// JustifyTop		Top justified text.
// JustifyLeft		Left jutified text.
// JustifyCenter	Center justified text.
// JustifyRight 	Right justified text.
// JustifyVcenter	Vertically centered text.
// JustifyBottom	Bottom justified text.
// WordBreak		Auto break of lines extending past 'rect'.
// SingleLine		Single line only.
// ExpandTabs		Expand tab characters.
// NoClipping		Don't clip to 'rect'.
// CalcRect  		Modify 'rect' to fit 's' - don\'t write text.
// NoPrefix  		Disable use of prefix characters (e.g., '&' on MS Windows)
//
int VPort::wrtText(const char *s, VRectangle *rect, short style)
{
	int n;

	if (s && hDC) {
		n = strlen(s);

#if defined(CV_WIN32)

		RECT wRect;
		wRect.left 		= rect->left;
		wRect.top 		= rect->top;
		wRect.right 	= rect->right;
		wRect.bottom 	= rect->bottom;

		DrawText(hDC, (LPCSTR)s, n, &wRect, (UINT) style);
     
		if (style & CalcRect) {
			rect->left   = wRect.left;
			rect->top    = wRect.top;
			rect->right  = wRect.right;
			rect->bottom = wRect.bottom;
		}
#else
		DrawText(hDC, (LPCSTR)s, n, (LPRECT)&rect->left, (UINT) style);
#endif
		SIZE textExtent;
		if (GetTextExtentPoint(hDC, (LPCSTR) s, (int) n, (LPSIZE) &textExtent)) {

			//	(pat--truncatating long to int under Win32)
			return int(textExtent.cx);
		}
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Return the width and height of the line of text pointed to by 's'
// into '*w' and '*h', respectively.  Return a width and height of zero if this
// VPort is not open.  Use the current VFont object to compute
// the dimensions of 's' and return the dimensions in logicial units.
//
void VPort::textSize(const char *s, int *w, int *h)
{
	if (hDC) {
		SIZE sz;
#if defined(CV_WIN32)
		//	(GetTextExtentPoint32 lacks subtle off-by-one bug
		//	in certain circumstances that GetTextExtentPoint has)

		if (GetTextExtentPoint32(hDC, (LPCSTR) s, strlen(s), &sz)) 
#else
		if (GetTextExtentPoint(hDC, (LPCSTR) s, strlen(s), &sz)) 
#endif
		{
			*w = (int) sz.cx;
			*h = (int) sz.cy;
			return;
		}
	}
	*w = *h = 0;
}

// ---------------------------------------------------------------------------
// Draw the VIcon 'icon' in this VPort at the position ('x','y') using
// the current the current viewing transformation (if specified).
//
void VPort::drawIcon(VIcon *icon, int x, int y)
{
	if (hDC && icon) {
		HICON hi = icon->handle();
		if (hi) {
			DrawIcon(hDC, x, y, hi);
		}
	}
}

// ---------------------------------------------------------------------------
// Fill the region between the outer and inner rectangle
// Assumes that port is open.
//
//	111111
//	2 in 3
//	244443
//
void VPort::fillFrame(VRectangle *outer, VRectangle *inner)
{
	int oxmin, oymin, oxmax, oymax;
	int ixmin, iymin, ixmax, iymax;
	VRectangle	rect;

	if (!isOpen()) {
		return;
	}

	outer->get(Corners, &oxmin, &oymin, &oxmax, &oymax);
	inner->get(Corners, &ixmin, &iymin, &ixmax, &iymax);

	// region 1
	if (oymin < iymin) {
		if (oymax < iymin) {
			// inner is completely beneath outer
			fillRegion(outer, FALSE);
			return;
		}
		else {
			rect.set(Corners, oxmin, oymin, oxmax, iymin);
			fillRegion(&rect, FALSE);
			oymin = iymin;
		}
	}

	// region 2
	if (oxmin < ixmin) {
		if (oxmax < ixmin) {
			// inner is completely to the right of outer
			rect.set(Corners, oxmin, oymin, oxmax, oymax);
			fillRegion(&rect, FALSE);
			return;
		}
		else {
			rect.set(Corners, oxmin, oymin, ixmin, oymax);
			fillRegion(&rect, FALSE);
		}
	}

	// region 3
	if (oxmax > ixmax) {
		if (oxmin > ixmax) {
			// inner is completely to the left of outer
			rect.set(Corners, oxmin, oymin, oxmax, oymax);
			fillRegion(&rect, FALSE);
			return;
		}
		else {
			rect.set(Corners, ixmax, oymin, oxmax, oymax);
			fillRegion(&rect, FALSE);
		}
	}

	// region 4
	if (oymax > iymax) {
		ixmin = (ixmin > oxmin) ? ixmin : oxmin;
		ixmax = (ixmax < oxmax) ? ixmax : oxmax;
		rect.set(Corners, ixmin, iymax, ixmax, oymax);
		fillRegion(&rect, FALSE);
	}
}

// ---------------------------------------------------------------------------


