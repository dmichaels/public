// $Header:   Q:/views/common/vcs/FRAME.CPV   1.8   Mar 24 1997 16:08:58   scottt  $ 

//	frame.cpp
//
//	implementation of VFrame class [Common]
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
//	11/08/92 jmd	fixed bug in operator=(VRectangle &)
//	05/05/93 jmd	added units, removed howStr
//	05/20/93 jmd	preened
//	07/19/93 jmd	made operator = use a const arg
//	07/30/93 pkt	eliminated #ifdef macintosh.
//	08/03/93 pkt	added #ifdef ARCHIVER for print(VStream&).
//	08/04/93 pkt	fixed get(const VLocation&, int *, int *, int *, int *).
//	08/05/93 pkt	fixed static translate function not to overflow when
//					translating a ratio-like frame to a value less than 0.0.
//	08/09/93 jmd	removed free
//	08/22/93 dgm	added units to putTo() and getFrom().
//	08/23/93 dgm	fix to Font16 to Pixels conversion in get().
//	09/01/93 dgm	M8 changes.
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	10/12/93 jmd	made ::resize reset to Pixels under certain cases
//	10/15/93 pkt	removed print.
//	11/12/93 pkt	changed id to VObject *.
//	11/13/93 pkt	made VLocation argument to get const.
//	12/27/93 pkt	changed for Symantec compiler.
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	02/15/94 pat	made VLocation& parameters const in resize(), move(),
//					xlate(), get(), convertOpt(), and setAsBefore()
//	06/22/94 dgm	Corrected rounding in get() for Font16().
//	08/31/94 dgm	Minor UnixWare/MetaWare-C++ related changes.
//	05/19/95 pkt	merged for common source.
//	11/20/95 pkt	updated for VRatio's MRATIO macro.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "location.h"
#include "frame.h"
#include "archiver.h"
#include "window.h"

defineClass(VFrame, VObject)

// ---------------------------------------------------------------------------
// Construct a VFrame of zero width and height at the origin (0,0).
//
VFrame::VFrame() 
{ 
	how = CornerDim; 
	xf = yf = wf = hf = 0;
	units = Pixels;
}

// ---------------------------------------------------------------------------
// Construct a VFrame of with width w and height h at the
// origin (0,0).
//
VFrame::VFrame(VRatio w, VRatio h) 
{ 	
	how = CornerDim; 
	xf = yf = 0, 
	wf = w;
	hf = h;
	units = Pixels;
}

// ---------------------------------------------------------------------------
// Construct a VFrame with position and dimensions using the operation
// o which may be one of Corners, CenterDim, or CornerDim.  If
// Corners is given, then the upper-left corner is given by (i1,i2) and
// the lower-right by (i3,i4).  If CenterDim is given, then (i1,i2)
// is the center of the frame and i3 and i4 are its width and height,
// respectively.  If CornerDim is given, then (i1,i2) is the upper-left
// corner of the frame and i3 and i4 are its width and height, respectively.
//
VFrame::VFrame(VRatio i1, VRatio i2, VRatio i3, VRatio i4,
				RectOpts o, Units u)
{
	units = u;

	set(i1, i2, i3, i4, o);
}

// ---------------------------------------------------------------------------
VFrame::VFrame(VRatio i1, VRatio i2, VRatio i3, VRatio i4, Units u)
{
	units = u;

	set(i1, i2, i3, i4, CornerDim);
}

// ---------------------------------------------------------------------------
// Return the VRatios and how-framed of this VFrame into x, y, 
// w, h, and opts, respectively.  The how-framed values are returned as
// one of the enum RectOpts constants: Corners, CenterDim, or CornerDim.
//
void VFrame::get(VRatio& x, VRatio& y, VRatio& w, VRatio& h,
						RectOpts& opts)
{
	x = xf;
	y = yf;
	w = wf;
	h = hf;
	opts = how;
}

// ---------------------------------------------------------------------------
// Set the position and dimensions of this VFrame using the operation
// o which may be one of Corners, CenterDim, or CornerDim.  If
// Corners is given, then the upper-left corner is given by (i1,i2) and
// the lower-right by (i3,i4).  If CenterDim is given, then (i1,i2)
// is the center of the frame and i3 and i4 are its width and height,
// respectively.  If CornerDim is given, then (i1,i2) is the upper-left
// corner of the frame and i3 and i4 are its width and height, respectively.
//
void VFrame::set(VRatio i1, VRatio i2, VRatio i3, VRatio i4, RectOpts o)
{
	how = o;
	xf = i1;
	yf = i2;
	wf = i3;
	hf = i4;
}

// ---------------------------------------------------------------------------
// Retrieve the coordinates of this VFrame, relative to the dimensions
// of an enclosing VFrame given in VLocation p, into VRectangle object r.
//
void VFrame::get(const VLocation& p, VRectangle& r)
{
	VRatio txf(xf);
	VRatio tyf(yf);
	VRatio twf(wf);
	VRatio thf(hf);
	r.set(how, txf(p.x()), tyf(p.y()), twf(p.x()), thf(p.y()));
}

// ---------------------------------------------------------------------------
// A VFrame can copy to another VFrame.
//
boolean VFrame::copyTo(VObject *obj)
{
	if (obj->isA(VFrameCls)) {
		VFrame *f = (VFrame *)obj;
		f->xf = xf;
		f->yf = yf;
		f->wf = wf;
		f->hf = hf;
		f->how = how;
		f->units = units;
	}
	else {
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// A VFrame can copy from another VFrame.
//
boolean VFrame::copyFrom(VObject *obj)
{
	if (obj->isA(VFrameCls)) {
		VFrame *f = (VFrame *)obj;
		xf = f->xf;
		yf = f->yf;
		wf = f->wf;
		hf = f->hf;
		how = f->how;
		units = f->units;
	}
	else if (obj->isA(VRectangleCls)) {
		*this = *(VRectangle *)obj;
	}
	else {
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the contents of this VFrame from VRectangle r.
//
VFrame& VFrame::operator= (VRectangle& r)
{
	r.get(how = CornerDim, &xf, &yf, &wf, &hf);
	return *this;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Store this VFrame into VArchiver a.
//
void VFrame::putTo(VArchiver& a)
{
	a << (int)how;
	a << xf;
	a << yf;
	a << wf;
	a << hf;
	a << (int)units;
}

// ---------------------------------------------------------------------------
// Read this VFrame from VArchiver a.
//
void VFrame::getFrom(VArchiver& a)
{
	int i;

	a >> i;
	how = (RectOpts)i;
	a >> xf;
	a >> yf;
	a >> wf;
	a >> hf;
	a >> i;
	units = (Units)i;
}

// --------------------------------------------------------------------------
//
void VFrame::getFrom(VArchiver& a,
					 VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif

// ---------------------------------------------------------------------------
// Return TRUE if this VFrame contains no ratios, otherwise return FALSE.
//
boolean VFrame::noRatio()
{
	return (xf > MRATIO && wf > MRATIO && yf > MRATIO && hf > MRATIO) ? TRUE : FALSE;
}

#define sgn(i) (i > MRATIO)

// ---------------------------------------------------------------------------
// Where xf is an int object representing an integer or fraction just
// as in a VRatio's internals, translate xf while attempting to 
// preserve its ratio-ness.  That is, whether it started out representing
// an integer or a fraction, try to retain that.
//
// The amount of the translation is given by the VRatio x, scaled if
// necessary (because the ratio-ness of x and xf don't match) by dim.
//
static int translate(VRatio &x, int &xf, int dim)
{ 
	if (sgn(xf)) {
		xf += x(dim);
		// not much chance of overflow here, as long as we're
		// managing windows
	}
	else {
		VRatio xr = x[dim]; // Try to make x into a ratio.
		if (xr.isRatio()) {
			xf += xr - IRATIO;
		}
		else {
			// Handle x being a non-ratio which puts xf out of ratio range.
			float fxf = float(int(xf)- IRATIO) / RATIO 
			+ float(int(x)) / float(dim);
			if (0.0F <= fxf && fxf <= 2.0) {
				// The result can be a ratio.
				xf = VRatio(fxf);
			}
			else {
				// Can't preserve ratio-ness of result.
				VRatio ixf(xf);
				xf = ixf(dim) + int(x);
			}
		}
	}
	return xf;
}

// ---------------------------------------------------------------------------
// Translate this VFrame by (x,y) within the dimensions of the
// enclosing frame given by the VLocation dim.
//
void VFrame::xlate(const VLocation& dim, VRatio x, VRatio y)
{
	translate(x, xf, dim.x());
	translate(y, yf, dim.y());
	if (how != Corners) {
		return;
	}
	translate(x, wf, dim.x());
	translate(y, hf, dim.y());
}

// ---------------------------------------------------------------------------
// Move this VFrame to (x,y) within the dimensions of the
// enclosing frame given by VLocation dim.
//
void VFrame::move(const VLocation& /* dim */ , VRatio x, VRatio y)
{
//	VRatio txf(xf);
//	VRatio tyf(yf);
//	xlate(dim,x(dim.x())-txf(dim.x()),y(dim.y())-tyf(dim.y()));
	xf = x;
	yf = y;
}

// ---------------------------------------------------------------------------
// Resize this VFrame to (w,h) within the dimensions of the
// enclosing frame given by the VLocation  dim.
//
void VFrame::resize(const VLocation& dim, VRatio w, VRatio h)
{
	if (how != Corners) {
		if (sgn(w)) {
			// reset to Pixel coords
			units = Pixels;
		}
		wf = w;
		hf = h;
		return;
	}

	VRatio txf(xf);
	VRatio tyf(yf);
	if (sgn(wf)) {
		wf = txf(dim.x()) + w(dim.x());
	}
	else {
		wf = txf[dim.x()] + w[dim.x()] - IRATIO;
	}
	if (sgn(hf)) {
		hf = tyf(dim.y()) + h(dim.y());
	}
	else {
		hf = tyf[dim.y()] + h[dim.y()] - IRATIO;
	}
}

// ---------------------------------------------------------------------------
// Copy this VFrame from VFrame f.
//
VFrame& VFrame::operator=(const VFrame& f)
{
	xf = f.xf;
	yf = f.yf;
	wf = f.wf;
	hf = f.hf;
	how = f.how;

	units = f.units;
 
	return *this;
}

// ---------------------------------------------------------------------------
// For each coordinate of this VFrame return a TRUE or FALSE indicating
// whether that coordinate is a framing ratio (percent) or a fixed value, respectively.
//
void VFrame::isRatio(boolean& x, boolean& y, boolean& w, boolean& h)
{
	x = xf <= MRATIO ? TRUE : FALSE;
	y = yf <= MRATIO ? TRUE : FALSE;
	w = wf <= MRATIO ? TRUE : FALSE;
	h = hf <= MRATIO ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
void VFrame::isFont16(boolean& x, boolean& y, boolean& w, boolean& h)
{
	if (units == Font16) {
		x = xf <= MRATIO ? FALSE : TRUE;
		y = yf <= MRATIO ? FALSE : TRUE;
		w = wf <= MRATIO ? FALSE : TRUE;
		h = hf <= MRATIO ? FALSE : TRUE;
	}
	else {
		x = y = w = h = FALSE;
	}
}

// ---------------------------------------------------------------------------
// Convert the way this VFrame's position and size values are
// interpreted.  The newOpt parameter can be one of the enum RectOpts
// constants: Corners, CenterDim, or CornerDim
//
void VFrame::convertOpt(const VLocation& pnt, RectOpts newOpt)
{
	if (how == newOpt) {
		return;
	}
	VRectangle rect;
	int x, y, w, h;
	get(pnt, rect);
	rect.get(newOpt, &x, &y, &w, &h);
	how = newOpt;
	setAsBefore(pnt, x, y, w, h);
}

// ---------------------------------------------------------------------------
void VFrame::setAsBefore(const VLocation& pnt, int x, int y, int w, int h)
{
	boolean		bX, bY, bW, bH;
	RectOpts	opts;
	VRatio		rx, ry, rw, rh;

	isRatio(bX, bY, bW, bH);
	get(rx, ry, rw, rh, opts);
	if (bX) {
		rx = int((x + 0.5) * RATIO / pnt.x() + RATIO);
	}
	else {
		rx = x;
	}
	if (bY) {
		ry = int((y + 0.5) * RATIO / pnt.y() + RATIO);
	}
	else {
		ry = y;
	}
	if (bW) {
		rw = int((w + 0.5) * RATIO / pnt.x() + RATIO);
	}
	else {
		rw = w;
	}
	if (bH) {
		rh = int((h + 0.5) * RATIO / pnt.y() + RATIO);
	}
	else {
		rh = h;
	}
	set(rx, ry, rw, rh, opts);
}

// ---------------------------------------------------------------------------
// Retrieve the coordinates of this VFrame, relative to the dimensions
// of an enclosing VFrame given in VLocation p, into *x, *y, *w,
// and *h.
//
void VFrame::get(const VLocation& p, int *x, int *y, int *w, int *h)
{
	VRectangle r;

	VRatio txf(xf);
	VRatio tyf(yf);
	VRatio twf(wf);
	VRatio thf(hf);
	r.set(how, txf(p.x()), tyf(p.y()), twf(p.x()), thf(p.y()));
	r.get(CornerDim, x, y, w, h);
}

// ---------------------------------------------------------------------------
//
#define round_to_int(f)	(int(f) + (((f - float(int(f))) >= 0.5F) ? 1 : 0))

// ---------------------------------------------------------------------------
//
void VFrame::get(VWindow *win, const VLocation& p,
				 int *x, int *y, int *w, int *h)
{
	VRectangle r;

	VRatio txf(xf);
	VRatio tyf(yf);
	VRatio twf(wf);
	VRatio thf(hf);

	if (win && units == Font16) {

		int		font_width, font_height, tx, ty, tw, th;
		float	f;
		boolean	do_font16_adjustment = FALSE;

		win->fontSize(&font_width, &font_height);

		tx = txf(p.x());
		ty = tyf(p.y());
		tw = twf(p.x());
		th = thf(p.y());

		if (xf > MRATIO) {
			f = (float(tx) * float(font_width)) / 16.0F;
			tx = round_to_int(f);
		}
		if (yf > MRATIO) {
			f = (float(ty) * float(font_height)) / 16.0F;
			ty = round_to_int(f);
		}
		if (wf > MRATIO) {
			f = (float(tw) * float(font_width)) / 16.0F;
			tw = round_to_int(f);
			do_font16_adjustment = TRUE;
		}
		if (hf > MRATIO) {
			f = (float(th) * float(font_height)) / 16.0F;
			th = round_to_int(f);
			do_font16_adjustment = TRUE;
		}

		if (do_font16_adjustment) {
#ifdef MOTIF
			int *wp, *hp;
			if (wf > MRATIO) {
				wp = &tw;
			}
			else {
				wp = 0;
			}
			if (hf > MRATIO) {
				hp = &th;
			}
			else {
				hp = 0;
			}
			win->adjustDimensionForFont16(wp, hp);
#endif
		}

#if 0
		tx = (xf <= MRATIO) ? tx : int((float(tx) * float(font_width)) / 16.0);
		ty = (yf <= MRATIO) ? ty : int((float(ty) * float(font_height)) / 16.0);
		tw = (wf <= MRATIO) ? tw : int((float(tw) * float(font_width)) / 16.0);
		th = (hf <= MRATIO) ? th : int((float(th) * float(font_height)) / 16.0);
		win->adjustDimensionForFont16(&tw, &th);
#endif


		r.set(how, tx, ty, tw, th);
	}
	else {
		r.set(how, txf(p.x()), tyf(p.y()), twf(p.x()), thf(p.y()));
	}

	r.get(CornerDim, x, y, w, h);
}
