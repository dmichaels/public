// $Header:   Q:/views/win/vcs/rect.cpv   1.5   Mar 04 1997 17:55:42   gregm  $ 

//	rect.cpp
//
//	VRectangle implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	10/16/92 jmd	added george's math changes
//	07/18/93 jmd	preened, removed free()
//	07/18/93 jmd	added new port fill interface
//	10/04/93 jmd	added long casts in ::get to prevent overflow
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	02/15/94 dgm	fix in combine() for RgnCopy.
//	03/05/94 pat	STRICT:  cast to HGDIOBJ for DeleteObject().
//	03/26/94 pat	CV_WIN32: updated pointIn(), invert(), combine().
//	07/21/94 jld	fixed NT bug in combine()
//	07/21/94 jld	added error checking to move()
//	07/21/94 jld	sent calls for Ellipse and RndRect to VRegion::pointIn()
//	07/21/94 jld	tempHndl private member variable deleted
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "rect.h"
#include "port.h"
#include "pointrry.h"

defineClass(VRectangle, VRegion)

// ---------------------------------------------------------------------------
// Construct of VRectangle with zero extent at the origin.
//
VRectangle::VRectangle()
{
	top = bottom = left = right = 0;
}

// ---------------------------------------------------------------------------
// Construct a VRectangle as per the enum RectOpts constant which
// may be one of Corners, CenterDim, or CornerDim.  
// If Corners is given, then the upper-left corner is given by ('i1','i2')
// and the lower-right by ('i3','i4').  If CenterDim is given, then
// ('i1','i2') is the center of the rectangle and 'i3' and 'i4' are
// its width and height, respectively.  If CornerDim is given, then
// ('i1','i2') is the upper-left corner of the rectangle and
// 'i3' and 'i4' are its width and height, respectively..
//
VRectangle::VRectangle(RectOpts o, int i1, int i2, int i3, int i4)
{
	set(o, i1, i2, i3, i4);
}

// ---------------------------------------------------------------------------
// Set the position and dimensions of this VRectangle using the
// operation 'o' which may be one of Corners, CenterDim, or CornerDim.  
// If Corners is given, then the upper-left corner is given by ('i1','i2')
// and the lower-right by ('i3','i4').  If CenterDim is given, then
// ('i1','i2') is the center of the rectangle and 'i3' and 'i4' are
// its width and height, respectively.  If CornerDim is given, then
// ('i1','i2') is the upper-left corner of the rectangle and
// 'i3' and 'i4' are its width and height, respectively.
//
void VRectangle::set(RectOpts o, int i1, int i2, int i3, int i4)
{
	switch (o) {
	case Corners:
	default:
		left  = i1;   	
		top   = i2;
		right = i3;
		bottom = i4;
		break;
	case CenterDim:
		left = i1 - (i3 / 2);
		top = i2 - (i4 / 2);
		right = left + i3;
		bottom = top + i4;
		break;
	case CornerDim:
		left  = i1;   	
		top   = i2;
		right = i1 + i3;
		bottom = i2 + i4;
		break;
	}
	ungetHndl();
}

// ---------------------------------------------------------------------------
// Retrieve the coordinates of this VRectangle as specified by RectOpts
// 'o' into '*i1p', '*i2p', '*i3p' and '*i4p'. If 'o' is given as Corners, 
// then ('*i1','*i2') will be the upper-left corner and ('*i3','*i4') the
// lower-right.  If 'o' is given as CenterDim, then ('*i1','*i2') will be the
// center of the rectangle and '*i3' and '*i4' its width and height, respectively.  
// If 'o' is given as CornerDim, then ('*i1','*i2') will be the upper-left
// corner of the rectangle and '*i3' and '*i4' its width and height, respectively.
//
void VRectangle::get(RectOpts o, int *i1p, int *i2p, int *i3p, int *i4p)
{
	int		i1, i2, i3, i4;

	switch (o) {
	case Corners:
	default:
		i1 = left;
		i2 = top;
		i3 = right;
		i4 = bottom;
		break;
	case CenterDim:
		// cast to protect against overflow
		i1 = (int) ((long)(((long)right + (long)left)) / 2);
		i2 = (int) ((long)(((long)bottom + (long)top)) / 2);
		i3 = right - left;
		i4 = bottom - top;
		break;
	case CornerDim:
		i1 = left;
		i2 = top;
		i3 = right - left;
		i4 = bottom - top;
		break;
	}
	if (i1p) {
		*i1p = i1;
	}
	if (i2p) {
		*i2p = i2;
	}
	if (i3p) {
		*i3p = i3;
	}
	if (i4p) {
		*i4p = i4;
	}
}

// ---------------------------------------------------------------------------
// Combine this VRectangle with VRegion 'r' with rule 'rule', where
// 'rule' is of type enum RegionRule:  RgnDifference, RgnIntersect, 
// RgnUnion, RgnXor, or RgnCopy.
//
VRegion *VRectangle::combine(VRegion *r, RegionRule rule)
{
	if (!(getIsA() == VRectangleCls && r->getIsA() == VRectangleCls)) {
		return VRegion::combine(r, rule);
	}

	RECT		wRectDst;
	VRectangle *rp 		= (VRectangle *)r;

	switch (rule) {
	case RgnDifference:
	case RgnXor:
		return VRegion::combine(r, rule);
 
#if defined(CV_WIN32)
	case RgnUnion:
	case RgnIntersect:
		//	Win32 RECT contains long coordinate members

		RECT	wRectSrc1;
		wRectSrc1.left 		= left;
		wRectSrc1.top 		= top;
		wRectSrc1.right 	= right;
		wRectSrc1.bottom 	= bottom;

		RECT	wRectSrc2;
		wRectSrc2.left 		= rp->left;
		wRectSrc2.top 		= rp->top;
		wRectSrc2.right 	= rp->right;
		wRectSrc2.bottom 	= rp->bottom;

		if (rule == RgnIntersect) {
			if (!IntersectRect(&wRectDst, &wRectSrc1, &wRectSrc2)) {
				return 0;
			}
		}
		else {
			if (!UnionRect(&wRectDst, &wRectSrc1, &wRectSrc2)) {
				return 0;
			}
		}
		break;
#else
	case RgnIntersect:
		if (!IntersectRect
			 ((LPRECT)&wRectDst, (LPRECT)&left, (LPRECT)&rp->left) ) {
			return 0;
		}
		break;

	case RgnUnion:
		if (!UnionRect
			 ((LPRECT)&wRectDst, (LPRECT)&left, (LPRECT)&rp->left) ) {
			return 0;
		}
		break;

#endif	//	!CV_WIN32

	case RgnCopy:
		return new VRectangle(Corners, left, top, right, bottom);
	}

	return new VRectangle(	Corners, 
							(int) wRectDst.left, 
							(int) wRectDst.top,
							(int) wRectDst.right, 
							(int) wRectDst.bottom);
}

// ---------------------------------------------------------------------------
// Shrink or expand the size and/or shape of this VRectangle while
// keeping the center position at the same point.   'Dx' specifies how
// much to move the left and right sides toward the center (a negative 'dx'
// moves away from the center) and 'dy' specifies how much to move the top and
// bottom toward the center (or away if negative). Returns true if the inset
// was successful. Otherwise leaves the dimensions of the rectangle
// unchanged.
//
boolean VRectangle::inset(int dx, int dy)
{
	ungetHndl();
	left   += dx;
	right  -= dx;
	top    += dy;
	bottom -= dy;
	if ( ! getHndl() ) {
		left   += dx;
		right  -= dx;
		top    += dy;
		bottom -= dy;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Move this VRectangle along the x and y axis by an amount 'dx' in
// the x direction and 'dy' in the y direction. Returns true if the
// move was successful. Otherwise the dimensions of the rectangle are
// unchanged.
//
boolean VRectangle::move(int dx, int dy)
{
	int ntype = OffsetRgn(getHndl(), dx, dy);
	ungetHndl();

	if ( ERROR != ntype ) {
		left += dx;
		right  += dx;
		top  += dy;
		bottom += dy;
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Return TRUE if the point specified by ('x', 'y') is in this
// VRectangle, otherwise return FALSE.
//
boolean VRectangle::pointIn(int x, int y)
{ 
	if (!(getIsA() == VRectangleCls))
		return VRegion::pointIn(x, y);
	
	POINT  pt;
	pt.x = x;
	pt.y = y;

#if defined(CV_WIN32)  
	RECT wRect;
	wRect.left 		= left;
	wRect.top 		= top;
	wRect.right 	= right;
	wRect.bottom 	= bottom;

	return PtInRect(&wRect, pt);
#else
	return PtInRect((LPRECT)&left, pt);
#endif
}

// ---------------------------------------------------------------------------
// Return this VRectangle as a VPointArray of four points. The caller
// is responsible for disposal of the returned VPointArray object.
//
VPointArray *VRectangle::asPoints()
{
	VPointArray *pa;

	pa = new VPointArray();
	pa->reset();
	pa->add(left, top);
	pa->add(right, top);
	pa->add(right, bottom);
	pa->add(left, bottom);

	return pa;
}

// ---------------------------------------------------------------------------
void VRectangle::ungetHndl()
{
	if (hRgn) {
		DeleteObject((HGDIOBJ) hRgn);
		hRgn = 0;
	}
}

// ---------------------------------------------------------------------------
HRGN VRectangle::getHndl()
{
	if (hRgn) {
		return hRgn;
	}
	return hRgn = CreateRectRgn(left, top, right, bottom);
}

// ---------------------------------------------------------------------------
// Called by VPort.  Fill the region. (Assume port->hDC is valid)
//
void VRectangle::fill(VPort *port)
{
	Rectangle(port->getHDC(), left, top, right, bottom);
}

// ---------------------------------------------------------------------------
// Called by VPort.  Invert the region. (Assume port->hDC is valid)
//
void VRectangle::invert(VPort *port)
{
#if defined(CV_WIN32)  
	RECT wRect;
	wRect.left 		= left;
	wRect.top 		= top;
	wRect.right 	= right;
	wRect.bottom 	= bottom;

	InvertRect(port->getHDC(), &wRect);
#else
	InvertRect(port->getHDC(), (LPRECT)&left);
#endif
}

// ---------------------------------------------------------------------------


