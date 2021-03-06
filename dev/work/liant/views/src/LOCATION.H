// $Header:   Q:/views/common/vcs/LOCATION.H_V   1.10   Mar 24 1997 16:09:34   scottt  $ 

//	location.h
//
//	VLocation class interface [Common]
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
//	07/24/93 jmd	preened, added const
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened.
//	01/17/94 dgm	cfront C++ changes
//	11/03/94 jmd	fixed operator -=
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VLOCATION_H
#define VLOCATION_H

// ---------------------------------------------------------------------------
//
#include "object.h"

// ---------------------------------------------------------------------------
// A VLocation is an (x,y) pair of short integers used to represent
// a point or a dimension in the presentation space of VDisplay objects.
//
CLASS VLocation : public VObject {
public:
	// Constructors
					VLocation();
					VLocation(const VLocation& p);
					VLocation(int x, int y);

	VClass		   *iam();

	// Querying and Setting Values
	int				x() const;
	int				y() const;
	void			x(int x);
	void			y(int y);

	void			operator()(int x, int y);

	// Operations on VLocation
	VLocation		operator+(const VLocation& p);
	VLocation		operator-(const VLocation& p);
	VLocation		operator/(int n);
	VLocation		operator*(int n);
	VLocation&		operator+=(const VLocation& p);
	VLocation&		operator-=(const VLocation& p);
	VLocation&		operator*=(int n);
	VLocation&		operator/=(int n);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:
	short			xp;	// The X coordinate of this VLocation.
	short			yp;	// The Y coordinate of this VLocation.

};

extern VClass *VLocationCls;

// ---------------------------------------------------------------------------
// Construct a VLocation from VLocation p.
//
inline VLocation::VLocation(const VLocation& p)
{
	xp = p.xp;
	yp = p.yp;
}

// ---------------------------------------------------------------------------
// Construct a VLocation from (x, y).
//
inline VLocation::VLocation(int x, int y)
{
	xp = x;
	yp = y;
}

// ---------------------------------------------------------------------------
// Return the X value of this VLocation.
//
inline int VLocation::x() const
{
	return xp;
}

// ---------------------------------------------------------------------------
// Return the Y value of this VLocation.
//
inline int VLocation::y() const
{
	return yp;
}

// ---------------------------------------------------------------------------
// Set the X value of this VLocation.
//
inline void VLocation::x(int x)
{
	xp = x;
}

// ---------------------------------------------------------------------------
// Set the Y value of this VLocation.
//
inline void VLocation::y(int y)
{
	yp = y;
}

// ---------------------------------------------------------------------------
// Set the values of this Locaation to (x,y).
//
inline void VLocation::operator()(int x, int y)
{
	xp = x;
	yp = y;
}

// ---------------------------------------------------------------------------
// Return  p + this VLocation as a VLocation.
//
inline VLocation VLocation::operator+(const VLocation& p)
{
	return VLocation(xp + p.xp, yp + p.yp);
}

// ---------------------------------------------------------------------------
// Return this VLocation minus p as a VLocation.
//
inline VLocation VLocation::operator-(const VLocation& p)
{
	return VLocation(xp - p.xp, yp - p.yp);
}

// ---------------------------------------------------------------------------
// Return this VLocation divided by n as a VLocation (scalar divide).
//
inline VLocation VLocation::operator/(int n)
{
	return VLocation(xp / n, yp / n);
}

// ---------------------------------------------------------------------------
// Return this VLocation times n as a VLocation (scalar multiply).
//
inline VLocation VLocation::operator*(int n)
{
	return VLocation(xp * n, yp * n);
}

// ---------------------------------------------------------------------------
// Add p to this VLocation.
//
inline VLocation& VLocation::operator+=(const VLocation& p)
{
	xp += p.xp;
	yp += p.yp;
	return *this;
}

// ---------------------------------------------------------------------------
// Subtract p from this VLocation.
//
inline VLocation& VLocation::operator-=(const VLocation& p)
{
	xp -= p.xp;
	yp -= p.yp;
	return *this;
}

// ---------------------------------------------------------------------------
// Scalar multiply n times this VLocation. 
//
inline VLocation& VLocation::operator*=(int n)
{
	xp *= n;
	yp *= n;
	return *this;
}

// ---------------------------------------------------------------------------
// Scalar divide this VLocation by n. 
//
inline VLocation& VLocation::operator/=(int n)
{
	xp /= n;
	yp /= n;
	return *this;
}

#endif // VLOCATION_H
