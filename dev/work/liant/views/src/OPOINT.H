/*
$Header:   Q:/views/win/vcs/opoint.h_v   1.5   Jun 03 1997 09:52:00   scottt  $
*/
//	opoint.h
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

#if !defined( _VOLEPOINT_H_ )
#define _VOLEPOINT_H_

// ---------------------------------------------------------------------------
//
#include "location.h"

// ---------------------------------------------------------------------------
// A VOlePoint is an (x,y) pair of short integers used to represent
// a point or a dimension in the presentation space of VDisplay objects.
//
CLASS VOlePoint : public VLocation 
{
public:

	typedef enum {
		Pixels,
		Twips,
		In100ths,
		In1000ths,
		MM10ths,
		MM100ths
	} PtUnits;

	// Constructors
					VOlePoint() {;}
					VOlePoint(const VOlePoint& p)
						: VLocation(p) {;}
					VOlePoint(const int x, const int y, const PtUnits units);

					VOlePoint(const POINTL& p, const PtUnits units);
					VOlePoint(const SIZEL& s, const PtUnits units);
					VOlePoint(const POINT& p, const PtUnits units);

#if defined(CV_WIN16)
					VOlePoint(const SIZE& s, const PtUnits units);
#endif

	VClass		   *iam();

	// Querying and Setting Values

	// Conversions
#if 0
					operator POINTL&();
					operator POINTL*() {return &(POINTL&) *this;}
					operator SIZEL&();
					operator LPSIZEL() {return &(SIZEL&) *this;}

#if defined(CV_WIN16)
					operator POINT&() 
						{point.x = xp; point.y = yp; return point;}
					operator POINT*() {return &(POINT&) *this;}
					operator SIZE&() 
						{size.cx = xp; size.cy = yp; return size;}
					operator LPSIZE() {return &(SIZE&) *this;}
#endif
#endif

void				getPoint(int& x, int& y, const VOlePoint::PtUnits units) const;
void				setPoint(const int x, const int y, const VOlePoint::PtUnits units);

SIZEL&				getPoint(SIZEL& size, const VOlePoint::PtUnits units) const;
POINTL&				getPoint(POINTL& point, const VOlePoint::PtUnits units) const;
POINT&				getPoint(POINT& point, const VOlePoint::PtUnits units) const;

#if defined(CV_WIN16)
SIZE&				getPoint(SIZE& size, const VOlePoint::PtUnits units) const;
#endif

// Start of Sanjay's Code
#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif
// End of Sanjay's Code

protected:

#if 0
	// Used to return references in conversion operators
	POINTL		pointl;
	SIZEL		sizel;

#if defined(CV_WIN16)
	POINT		point;
	SIZE		size;
#endif
#endif
};

extern VClass *VOlePointCls;

inline SIZEL& VOlePoint::getPoint(SIZEL& size, const VOlePoint::PtUnits units) const
{
	int		x, y;
	
	getPoint(x, y, units);
	
	size.cx = x;
	size.cy = y;

	return size;
}

inline POINTL& VOlePoint::getPoint(POINTL& point, const VOlePoint::PtUnits units) const
{
	int		x, y;
	
	getPoint(x, y, units);
	
	point.x = x;
	point.y = y;

	return point;
}

inline POINT& VOlePoint::getPoint(POINT& point, const VOlePoint::PtUnits units) const
{
	int		x, y;
	
	getPoint(x, y, units);
	
	point.x = x;
	point.y = y;

	return point;
}

#if defined(CV_WIN16)
inline SIZE& VOlePoint::getPoint(SIZE& size, const VOlePoint::PtUnits units) const
{
	int		x, y;
	
	getPoint(x, y, units);
	
	size.cx = x;
	size.cy = y;
	
	return size;
}

#endif

#endif // _VOLEPOINT_H_

