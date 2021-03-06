// $Header:   Q:/views/win/vcs/rndrect.cpv   1.5   Mar 04 1997 17:55:46   gregm  $ 

//	rndrect.cpp
//
//	VRoundRect implementation
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
//	07/18/93 jmd	preened, removed free()
//	07/18/93 jmd	added new port fill interface
//  09/02/93 dgm	M8 changes.
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/05/94 pat	STRICT:  cast to HGDIOBJ for DeleteObject()
//	07/19/94 jld	fixed getHndl()
//	07/20/94 jld	added invert to overwrite VRectangle::invert
//	08/07/94 dgm	Fixed fill() to work "right".
//	08/25/94 jld	correct interpretation of "roundness" in getHndl()
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "rndrect.h"
#include "port.h"

defineClass(VRoundRect, VRectangle)

// ---------------------------------------------------------------------------
// Construct a VRoundRect of position and dimension with roundness
// 'rnd'.  The bounding rectangle is specified using four int values and
// the enum RectOpts operation 'o' which may be one of Corners, CenterDim, or
// CornerDim.  If Corners is given, then the upper-left corner is given by
// ('i1','i2') and the lower-right by ('i3','i4').  If CenterDim is given, 
// then ('i1','i2') is the center of the rectangle and 'i3' and 'i4' are its
// width and height, respectively.  If CornerDim is given, then ('i1','i2')
// is the upper-left corner of the rectangle and 'i3' and 'i4' are its width
// and height, respectively.
//
VRoundRect::VRoundRect(RectOpts o, int i1, int i2, int i3, int i4, float rnd)
{
	set(o, i1, i2, i3, i4);
	round = rnd;
}

// ---------------------------------------------------------------------------
// Construct a VRoundRect of dimension (0,0) at the origin with
// roundness 1.0.
//
VRoundRect::VRoundRect()
{
	round = 1.0F;
}

// ---------------------------------------------------------------------------
//
HRGN VRoundRect::getHndl()
{
	if (hRgn) {
		return hRgn;
	}
	int w = (int)(abs(right - left) * round);
	int h = (int)(abs(bottom - top) * round);

	if ( h < w )	w = h;
	else			h = w;

	tempHndl++;
	hRgn = CreateRoundRectRgn(left, top, right, bottom, w, h);
	return hRgn;
}

// ---------------------------------------------------------------------------
// Called by VPort.  Fill the region. (Assume port->hDC is valid)
//
void VRoundRect::fill(VPort *port)
{
	int w = (int)(abs(right - left) * round);
	int h = (int)(abs(bottom - top) * round);

	// a better interpretation of round
	if (h < w)	w = h;
	else 		h = w;

	RoundRect(port->getHDC(), left, top, right, bottom, w, h);
}

// ---------------------------------------------------------------------------
// Called by VPort.  Invert the region.
//
void VRoundRect::invert(VPort *port)
{
	VRegion::invert(port);
}
// ---------------------------------------------------------------------------
