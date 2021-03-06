// $Header:   Q:/views/win/vcs/rndrect.h_v   1.5   Mar 04 1997 16:33:00   gregm  $ 

//	rndrect.h
//
//	VRoundRect interface
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
//	03/05/94 pat	preened
// ----------------------------------------------------------------------------

#ifndef VRNDRECT_H
#define VRNDRECT_H
#include "rect.h"

extern VClass *VRoundRectCls;

//
// The VRoundRect class defines a region formed by the intersection of
// a VRectangle and an VEllipse.  The amount of rounding is given by a
// roundness 'parameter', which gives the dimensions of the bounding rectangle
// of the ellipse relative to the dimensions of the rectangle.
//

CLASS VRoundRect : public VRectangle {
public:
					VRoundRect();
					VRoundRect(RectOpts, int, int, int, int, float);

	VClass 		   *iam();

	void			roundness(float rnd);
	float			roundness();

protected:

	float			round;		// Roundness factor.

	virtual HRGN	getHndl();
	void			fill(VPort *port);
	void			invert(VPort *port);
};

// ---------------------------------------------------------------------------
// Set the roundness factor of this VRoundRect to 'rnd'.
//
inline void VRoundRect::roundness(float rnd)
{
	round = rnd;
}

// ---------------------------------------------------------------------------
// Return the roundness factor of this VRoundRect.
//
inline float VRoundRect::roundness()
{
	return round;
}

#endif // VRNDRECT_H
