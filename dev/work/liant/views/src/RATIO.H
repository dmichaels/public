// $Header:   Q:/views/common/vcs/RATIO.H_V   1.10   Mar 24 1997 16:13:08   scottt  $ 

//	ratio.h
//
//	VRatio class interface [Common]
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
//	10/31/92 jmd	preened
//	10/02/93 dgm	added asFloat().
//	11/02/94 dgm	moved operator() to ratio.cpp.
//	05/16/95 pkt	preened.
//	08/21/95 pkt	added constructor that takes a double.
//	09/06/95 pkt	removed OFI code; fixed bug in non-Macintosh case of
//					operator[].
//	11/20/95 pkt	added MRATIO macro.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef RATIO_H
#define RATIO_H

#include "defs.h"

// ---------------------------------------------------------------------------
// Parameters used in representing framing ratios:
//		RATIO:  scaling factor, floating point to fixed point representation
//		IRATIO: representation of 0.0F framing ratio
//		MRATIO: cutoff between ratios and straight integers
//
#define RATIO (-10000.0F)
#define IRATIO (-10000)
#define MRATIO (-5000)

// VRatio is a class used with VFrame to implement framing
// ratios between a child VWindow's coordinates and its parent or to give
// the child VWindow's actual coordinates relative to its parent. 
// If constructed with a float, then the VRatio object represents a ratio
// of the parent VWindow's client region or the VDisplay region
// (if the child has no parent).

CLASS VString;

CLASS VRatio {
public:
	// Constructors
				VRatio(int i);
				VRatio(float f);
				VRatio(double d);
				VRatio();

	// Ratio Operations
				operator int();
	VRatio&		operator=(int i);

	boolean		isRatio();
	void		asString(VString &);
	void		fromString(VString &);
	void		asFloat(float &);

	int			operator()(int i);
	VRatio		operator[](int i);
	
protected:
	int			n;	// coordinate if >MRATIO, framing ratio if <=MRATIO

};

// ---------------------------------------------------------------------------
// Construct a VRatio as the coordinate 'i'. 
//
inline VRatio::VRatio(int i)
{
	n = i;
}

// ---------------------------------------------------------------------------
// Construct a VRatio as the ratio 'f' where 0 <= 'f' <= 1.0.
//
inline VRatio::VRatio(float f)
{
	n = (int)(RATIO * f + IRATIO - .5F);
}

// ---------------------------------------------------------------------------
// Construct a VRatio as the ratio 'd' where 0 <= 'd' <= 1.0.
//
inline VRatio::VRatio(double d)
{
	n = (int)(RATIO * d + IRATIO - .5);
}

// ---------------------------------------------------------------------------
// Construct a VRatio with the coordinate zero. 
//
inline VRatio::VRatio()
{
	n = 0;
}

// ---------------------------------------------------------------------------
// Return this VRatio as an int.
//
inline VRatio::operator int()
{
	return n;
}

// ---------------------------------------------------------------------------
// Copy the value of this VRatio from int 'i'.
//
inline VRatio &VRatio::operator= (int i)
{
	n = i;
	return *this;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VRatio represents a framing ratio (a percent), 
// otherwise return FALSE.
//
inline boolean VRatio::isRatio()
{
	return n > MRATIO ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
// If this VRatio represents a framing ratio (that is, a percent), 
// then return the ratio of the int i.  If this VRatio represents
// a coordinate, then return that coordinate value.
//
inline int VRatio::operator()(int i)
{
	if (n > MRATIO) {
		return n;
	}
	else {
		return int(((i * (n - RATIO)) / RATIO) + 0.5F); 
	}
}

// ---------------------------------------------------------------------------
// If the value of this VRatio is a ratio, return it. Otherwise
// return it as a ratio (if applicable) of i.
//
inline VRatio VRatio::operator[](int i)
{ 
	if (n <= MRATIO) {
		return VRatio((float)n);
	}
	return VRatio(float(n)/i);
}

#endif // RATIO_H

