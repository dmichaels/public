// $Header:   Q:/views/win/vcs/arc.cpv   1.6   Mar 04 1997 17:16:34   gregm  $ 

//	arc.cpp
//
//	VArc implementation
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
//	10/14/92 jmd	created
//	07/18/93 jmd	added new port fill interface
//	08/09/93 jmd	removed free
//	09/02/93 dgm	M8 changes; casts (from double) to float.
//	09/12/93 jmd	preened, removed getAngles, added ArcStyle
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/05/94 pat	STRICT: added HGDIOBJ cast in DeleteObject()
//	08/03/94 jld	reimplemented based on polygon&ellipse intersection
//	08/03/94 jld	added getAngles()
//	08/08/94 jld	added move()
//	09/06/96 glm	removed extern C from include of math.h for VC4.2
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "arc.h"
#include "port.h"

#include <math.h>		// for trig functions

defineClass(VArc, VEllipse)

// ---------------------------------------------------------------------------
// Construct a VArc region of size (0,0) at the origin.
//
VArc::VArc()
{
	_ptCount = 0;
	_startAngle = (float) 0.0;
	_endAngle = (float) 0.0;
	_style   = Pie;
}

// ---------------------------------------------------------------------------
// Construct a VArc region. Set its position and dimensions to
// the specified bounding rectangle (see VRectangle).  Plus an additional
// pair of numbers specifying the starting and ending angles of the arc
// (in radians).
//
VArc::VArc(RectOpts o, int i1, int i2, int i3, int i4, float start, float end, AngleType at)
{
	set(o, i1, i2, i3, i4);
	setAngles(start, end, at);
	makePolygon();
	_style = Pie;
}

// ---------------------------------------------------------------------------
// Specify the starting and ending angles of the arc in radians or
// degrees. Angles will be adjusted so that 0 <= start < 360 degrees
// and 0 <= end - start 360 degrees.
// 
void VArc::setAngles(float start, float end, AngleType at)
{
	const float RadiansPerDegree = (float) 3.14159265F / 180.0F;
	const float twoPi = (float) 2 * 3.14159265F;

	// convert angles to radians if needed
	if (at == Degrees) {
		start = start * RadiansPerDegree;
		end   = end * RadiansPerDegree;
	}

	start = (float) fmod((double) start, (double) twoPi);
	end   = (float) fmod((double) end,   (double) twoPi);

	// shift angles such that 0 < start < end
	if ( start < 0 )
		start += twoPi;
	while ( end < start )
		end += twoPi;

	// make sure polygon is recomputed
	_ptCount = 0;
	ungetHndl();

	_startAngle = start;
	_endAngle = end;
}


// ---------------------------------------------------------------------------
// Returns the starting and ending angles in the units specified (degrees or
// radians).
//
void VArc::getAngles(float* start, float* end, AngleType at)
{
	const float RadiansPerDegree = (float) 3.14159265F / 180.0F;

	if ( at == Degrees ) {
		*start = _startAngle * RadiansPerDegree;
		*end = _endAngle * RadiansPerDegree;
	}
	else {
		*start = _startAngle;
		*end = _endAngle; 
	}
}

// ---------------------------------------------------------------------------
// Finds the coordinates of the intersection of a radial at angle with
// an ellipse of width w and height h. Returns an integer indicating
// the quandrant of the angle.
//
int VArc::getIntersection ( int w, int h, float angle, int* x, int* y)
{
	// find coordinates of point where a radius at the 
	// given angle intersects the arc
	*x = (int) (((float) w) /2 * cos(angle));
	*y = (int) (((float) h) /2 * sin(angle));

	// return the quadrant that contains the angle
	if ( *y > 0 )
		if ( *x > 0 ) 	return 0;
		else 			return 1;
	else
		if ( *x > 0 ) 	return 3;
		else 			return 2;
}

// ---------------------------------------------------------------------------
// Constructs a polygon which when intersected with an ellipse will
// produce the region for the arc. The points of the polygon comprise
// the intersection of the start radial with the ellipse, corner points
// from the enclosing rectangle in counterclockwise order, the end radial
// intersection, and the center point. The first corner point is in the
// same quadrant as the start angle, and the last corner in the same
// quadrant as the end angle. 
//
void VArc::makePolygon()
{
	// find corners of bounding rectangle
	int	cx, cy, w, h;
	get(CenterDim, &cx, &cy, &w, &h);

	int quad, lastQuad, x, y;

	// put point in for start angle
	quad = getIntersection( w, h, _startAngle, &x, &y );
	_poly[0].x = cx + x;
	_poly[0].y = cy - y;

	// find quadrant for end angle and coords of last point
	lastQuad = getIntersection( w, h, _endAngle, &x, &y );

	// add corner points to the polygon as needed
	_ptCount = 1;
	while (1) {
		switch (quad) {
			case 0:
				_poly[_ptCount].x = right;
				_poly[_ptCount++].y = top;
				break;
			case 1:
				_poly[_ptCount].x = left;
				_poly[_ptCount++].y = top;
				break;
			case 2:
				_poly[_ptCount].x = left;
				_poly[_ptCount++].y = bottom;
				break;
			case 3:
				_poly[_ptCount].x = right;
				_poly[_ptCount++].y = bottom;
				break;
		}
		if ( quad == lastQuad )	break;
		else 					quad = (quad + 1) % 4;
	}

	// add end intersection point to polygon
	_poly[_ptCount].x = cx + x;
	_poly[_ptCount++].y = cy - y;

	// add center point to polygon
	_poly[_ptCount].x = cx;
	_poly[_ptCount++].y = cy;
}

// ---------------------------------------------------------------------------
HRGN VArc::getHndl()
{
	if (hRgn) {
		return hRgn;
	}

	// Windows does not provide an "arc" region, we must build one
	// by combining an ellipse with a polygon.
	HRGN  rgnDest, rgnEll, rgnPoly;
	int   ntype;
	
	if ( _ptCount == 0 ) makePolygon();

	rgnDest = CreateRectRgn(0, 0, 1, 1);
	rgnEll = CreateEllipticRgn(left, top, right, bottom);

	// Don't use the last point of the polygon (center of ellipse)
	// when the style is chord.
	if ( _style == Chord )
		rgnPoly = CreatePolygonRgn(_poly, _ptCount-1, ALTERNATE);
	else
		rgnPoly = CreatePolygonRgn(_poly, _ptCount, ALTERNATE);

	ntype = CombineRgn(rgnDest, rgnPoly, rgnEll, RGN_AND);
	DeleteObject((HGDIOBJ) rgnPoly);
	DeleteObject((HGDIOBJ) rgnEll);

	if (ntype != ERROR && ntype != NULLREGION) {
		tempHndl++;
		hRgn = rgnDest;
	}

	return hRgn;
}

// ---------------------------------------------------------------------------
// Called by VPort.  Fill the region. (Assume port->hDC is valid)
//
void VArc::fill(VPort *port)
{

	if( _ptCount == 0 ) makePolygon();

	switch (_style) {
	case Chord:
		::Chord(port->getHDC(), left, top, right, bottom, _poly[0].x, _poly[0].y, _poly[_ptCount-2].x, _poly[_ptCount-2].y);
		break;
	case Pie:
		::Pie(port->getHDC(), left, top, right, bottom, _poly[0].x, _poly[0].y, _poly[_ptCount-2].x, _poly[_ptCount-2].y);
		break;
	case Arc:
		::Arc(port->getHDC(), left, top, right, bottom, _poly[0].x, _poly[0].y, _poly[_ptCount-2].x, _poly[_ptCount-2].y);
		break;
	}
}

// ---------------------------------------------------------------------------
// Move the arc by the specified amount.
//
boolean VArc::move(int dx, int dy)
{
	if ( ! 	VRectangle::move( dx, dy ) ) return FALSE;

	for ( int i = 0; i < _ptCount; i++ ) {
		_poly[i].x += dx;
		_poly[i].y += dy;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Change the bounding rectangle of the arc by the specified amounts.
//
boolean VArc::inset(int dx, int dy)
{
	if ( VRectangle::inset( dx, dy ) ) {
		// make sure the intersecting polygon will be recomputed.
		_ptCount = 0;
		ungetHndl();
		return TRUE;
	}
	return FALSE;
}
// ---------------------------------------------------------------------------

