// $Header:   Q:/views/win/vcs/polygon.cpv   1.5   Mar 04 1997 17:55:32   gregm  $ 

//	polygon.cpp
//
//	VPolygon implementation
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
//	03/05/94 pat	STRICT: cast to HGDIOBJ for DeleteObject()
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "pointrry.h"
#include "polygon.h"

defineClass(VPolygon, VRegion)

// ---------------------------------------------------------------------------
// Construct a NULL VPolygon.
//
VPolygon::VPolygon()
{
	hRgn = 0;
}

// ---------------------------------------------------------------------------
// Construct a polygonal VPolygon from VPointArray  'pa'.
//
VPolygon::VPolygon(VPointArray *pa)
{
	hRgn = 0;
	set(pa);
}

// ---------------------------------------------------------------------------
// Set the vertices of this VPolygon to the points in VPointArray 'pa'.
//
void VPolygon::set(VPointArray *pa)
{
	int x, x1, y, y1;
	if (hRgn) {
		DeleteObject((HGDIOBJ) hRgn);
	}
	pa->getpnt(0, &x, &y);
	pa->getpnt(pa->size() - 1, &x1, &y1);
	if (x != x1 || y != y1) {
		pa->add(x, y);
	}
	hRgn = CreatePolygonRgn((LPPOINT)pa->getData(), (int)pa->size(), ALTERNATE);
}

// ---------------------------------------------------------------------------
// Move this VPolygon along the x and y axis by an amount 'dx' in
// the x direction and 'dy' in the y direction. 
//
boolean VPolygon::move(int dx, int dy)
{ 
	int     ntype;
	ntype = OffsetRgn(hRgn, dx, dy);
	return (ntype != ERROR || ntype != NULLREGION) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Return TRUE if the point ('x','y') is in this VPolygon, otherwise
// return FALSE.
//
boolean VPolygon::pointIn(int x, int y)
{
	return (PtInRegion(hRgn, x, y) ? TRUE : FALSE);
}

// ---------------------------------------------------------------------------
