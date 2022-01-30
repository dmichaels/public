// $Header:   Q:/views/win/vcs/region.cpv   1.5   Mar 04 1997 17:55:44   gregm  $ 

//	region.cpp
//
//	VRegion implementation
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
//	03/05/94 pat	STRICT:  cast to HGDIOBJ for DeleteObject().
//	07/21/94 jld	implemented move() and pointIn()
//	07/21/94 jld	fixed combine() to return correct type objects
//					and potential memory leak on failure.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "region.h"
#include "port.h"

defineClass(VRegion, VObject)

// ---------------------------------------------------------------------------
VRegion::VRegion()
{
	hRgn = 0;
}

// ---------------------------------------------------------------------------
VRegion::~VRegion()
{
	if (hRgn) {
		DeleteObject((HGDIOBJ) hRgn);
	}
	hRgn = 0;
}

// ---------------------------------------------------------------------------
// Move this VRegion by delta x, 'dx', and delta y, 'dy'.
//
boolean VRegion::move(int dx, int dy)
{
	int ntype = OffsetRgn(getHndl(), dx, dy);
	ungetHndl();
	return ntype != ERROR;
}

// ---------------------------------------------------------------------------
// Return TRUE if the point 'x', 'y' is contained within this VRegion,
// otherwise return FALSE.
//
boolean VRegion::pointIn(int x, int y)
{
	boolean r = PtInRegion(getHndl(), x, y);
	ungetHndl();
	return r;
}

// ---------------------------------------------------------------------------
HRGN VRegion::getHndl()
{
	return hRgn;
}

// ---------------------------------------------------------------------------
void VRegion::ungetHndl()
{
	;
}

// ---------------------------------------------------------------------------
// Return a new VRegion that combines this VRegion
// with the region 'rgn' using the combination rule 'rule', where 'rule' is of
// type enum RegionRule: RgnDifference, RgnIntersect, RgnUnion, RgnXor, RgnCopy. 
// Return 0 if their combination is not defined.
//
VRegion *VRegion::combine(VRegion *rgn, RegionRule rule)
{
	HRGN	Rgd;
	VRegion *r = 0;
	int		ntype;

	Rgd = CreateRectRgn(0, 0, 1, 1);

	if (!hRgn)
		hRgn = getHndl();
	
	ntype = CombineRgn(Rgd, hRgn, rgn->getHndl(), rule);

	rgn->ungetHndl();
	ungetHndl();

	if (ntype != ERROR && ntype != NULLREGION) {
		if ( rule == RgnCopy )	// Preserve type on RgnCopy
			r = (VRegion*) copy();
		else 
			r = new VRegion;
		r->hRgn = Rgd;
	}
	else DeleteObject((HGDIOBJ) Rgd);

	return r;
} 

// ---------------------------------------------------------------------------
// Called by VPort.  Fill the region. (Assume port->hDC is valid)
// draws the border... (unless port turns off pen)
//
void VRegion::fill(VPort *port)
{
	HRGN h;
	if (h = getHndl()) {
		PaintRgn(port->getHDC(), h);
		ungetHndl();
	}
}

// ---------------------------------------------------------------------------
// Called by VPort.  Invert the region. (Assume port->hDC is valid)
//
void VRegion::invert(VPort *port)
{
	HRGN h;
	if (h = getHndl()) {
		InvertRgn(port->getHDC(), h);
		ungetHndl();
	}
}

// ---------------------------------------------------------------------------
