/*
$Headr: $
*/
//	opoint.cpp
//
//	VOlePoint interface
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
//	01/18/95 dss	Initial Release.
// ---------------------------------------------------------------------------

#include "pre.h"

#include "location.h"

defineClass(VOlePoint, VLocation)

// ---------------------------------------------------------------------------
//
// Construct a VOlePoint from an x,y pair.
//
VOlePoint::VOlePoint(const int x, const int y, const PtUnits units)
{
	switch (units)
	{
	case Pixels:
		xp = x;
		yp = y;
		break;

	case MM100ths:
		xp = XformWidthInHimetricToPixels(NULL, (int) x);
		yp = XformHeightInHimetricToPixels(NULL, (int) y);
		break;

	default:
		cv_assert(0);
	}
}

// ---------------------------------------------------------------------------
//
// Construct a VOlePoint from POINTL.
//
// TO DO: VLocation store corrdinates as int's. 
//		WIN32: sizeof(int) == sizeof(long), but not true for WIN16.
//		Could be a problem on WIN16.
//
VOlePoint::VOlePoint(const POINTL& p, const PtUnits units)
{
	switch (units)
	{
	case Pixels:
		xp = int(p.x);
		yp = int(p.y);
		break;

	case MM100ths:
		xp = XformWidthInHimetricToPixels(NULL, (int) p.x);
		yp = XformHeightInHimetricToPixels(NULL, (int) p.y);
		break;

	default:
		cv_assert(0);
	}
}

// ---------------------------------------------------------------------------
//
// Construct a VOlePoint from POINT.
// Assumes POINT is in pixels.
//
VOlePoint::VOlePoint(const POINT& p, const PtUnits units)	
{
	switch (units)
	{
	case Pixels:
		// TO DO: losing precision because of Views.
		xp = short(p.x);
		yp = short(p.y);
		break;

	case MM100ths:
		xp = XformWidthInHimetricToPixels(NULL, p.x);
		yp = XformHeightInHimetricToPixels(NULL, p.y);
		break;

	default:
		cv_assert(0);
	}
}

#if 0
// ---------------------------------------------------------------------------
//
// Construct a VOlePoint from LPPOINT.
// Assumes POINT is in pixels.
//
VOlePoint::VOlePoint(LPPOINT p, const PtUnits units)
{
	xp = p->x;
	yp = p->y;
}
#endif

// ---------------------------------------------------------------------------
//
// Construct a VOlePoint from SIZEL.
// Assumes SIZEL is in Himetric units.
//
VOlePoint::VOlePoint(const SIZEL& s, const PtUnits units)
{
	switch (units)
	{
	case Pixels:
		xp = int(s.cx);
		yp = int(s.cy);
		break;

	case MM100ths:
		xp = XformWidthInHimetricToPixels(NULL, (int) s.cx);
		yp = XformHeightInHimetricToPixels(NULL, (int) s.cy);
		break;

	default:
		cv_assert(0);
	}
}

#if defined(CV_WIN16)
// ---------------------------------------------------------------------------
//
// Construct a VOlePoint from SIZE.
// Assumes SIZE is in pixels.
//
VOlePoint::VOlePoint(const SIZE& s, const PtUnits units)
{
	switch (units)
	{
	case Pixels:
		xp = s.cx;
		yp = s.cy;
		break;

	case MM100ths:
		xp = XformWidthInHimetricToPixels(NULL, s.cx);
		yp = XformHeightInHimetricToPixels(NULL, s.cy);
		break;

	default:
		cv_assert(0);
	}
}

#endif

#if 0
// ---------------------------------------------------------------------------
//
// Convert a VOlePoint to a POINTL reference.
// Assumes POINTL is in Himetric units.
//
VOlePoint::operator POINTL&()
{
	pointl.x = XformWidthInPixelsToHimetric(NULL, (int) xp);
	pointl.y = XformHeightInPixelsToHimetric(NULL, (int) yp);

	return pointl;
}

// ---------------------------------------------------------------------------
//
// Convert a VOlePoint to a SIZEL reference.
// Assumes POINTL is in Himetric units.
//
VOlePoint::operator SIZEL&()
{
	sizel.cx = XformWidthInPixelsToHimetric(NULL, (int) xp);
	sizel.cy = XformHeightInPixelsToHimetric(NULL, (int) yp);

	return sizel;
}
#endif

// ---------------------------------------------------------------------------
//
// Return the position, as x and y.
//
void VOlePoint::getPoint(int& x, int& y, const VOlePoint::PtUnits units) const
{
	switch (units)
	{
	case Pixels:
		x = xp;
		y = yp;
		break;

	case MM100ths:
		x = XformWidthInPixelsToHimetric(NULL, xp);
		y = XformHeightInPixelsToHimetric(NULL, yp);
		break;

	default:
		cv_assert(0);
	}
}

// ---------------------------------------------------------------------------
//
// Return the position, as x and y.
//
void VOlePoint::setPoint(const int x, const int y, const VOlePoint::PtUnits units)
{
	switch (units)
	{
	case Pixels:
		xp = x;
		yp = y;
		break;

	case MM100ths:
		xp = XformWidthInPixelsToHimetric(NULL, x);
		yp = XformHeightInPixelsToHimetric(NULL, y);
		break;

	default:
		cv_assert(0);
	}
}

// Start of Sanjay's Code
#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
// Store this VWinRect into the VArchiver a.
//
void VOlePoint::putTo(VArchiver& a)
{
	VLocation::putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VWinRect from VArchiver a.
//
void VOlePoint::getFrom(VArchiver& a)
{
	VLocation::getFrom(a);
}

// ----------------------------------------------------------------------------
//
void VOlePoint::getFrom(VArchiver& a,
                        VObject * /* data */ , boolean /* do_realize */ )
{
    getFrom(a);
}

#endif // ARCHIVER
// End of Sanjay's Code
