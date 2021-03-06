// $Header:   Q:/views/win/vcs/ellipse.cpv   1.5   Mar 04 1997 17:55:02   gregm  $ 

//	ellipse.cpp
//
//	VEllipse implementation
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
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	07/20/94 jld	added invert to overwrite VRectangle::invert
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "ellipse.h"
#include "port.h"

defineClass(VEllipse, VRectangle)

// ---------------------------------------------------------------------------
// Construct an VEllipse region of size (0,0) at the origin.
//
VEllipse::VEllipse()
{
	;
}

// ---------------------------------------------------------------------------
// Construct an VEllipse region and set its position and dimensions to
// the bounding rectangle specified using the enum RectOpts operation 'o' which
// may be one of Corners, CenterDim, or CornerDim.  If Corners is given, 
// then the upper-left corner is given by ('i1','i2') and the lower-right by
// ('i3','i4').  If CenterDim is given, then ('i1','i2') is the center of the
// rectangle and 'i3' and 'i4' are its width and height, respectively.  If
// CornerDim is given, then ('i1','i2') is the upper-left corner of the
// rectangle and 'i3' and 'i4' are its width and height, respectively.
//
VEllipse::VEllipse(RectOpts o, int i1, int i2, int i3, int i4)
{
	set(o, i1, i2, i3, i4);
}

// ---------------------------------------------------------------------------
HRGN VEllipse::getHndl()
{
	if (hRgn) {
		return hRgn;
	}

	tempHndl++;
	return hRgn = CreateEllipticRgn(left, top, right, bottom);
}

// ---------------------------------------------------------------------------
// Called by VPort.  Fill the region. (Assume port->hDC is valid)
//
void VEllipse::fill(VPort *port)
{
	Ellipse(port->getHDC(), left, top, right, bottom);
}

// ---------------------------------------------------------------------------
// Called by VPort.  Invert the region.
//
void VEllipse::invert(VPort *port)
{
	VRegion::invert(port);
}

// ---------------------------------------------------------------------------
