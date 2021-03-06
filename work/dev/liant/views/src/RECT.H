// $Header:   Q:/views/win/vcs/rect.h_v   1.7   Mar 04 1997 16:32:58   gregm  $ 

//	rect.h
//
//	VRectangle interface
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
//	10/14/92 jmd	preened
//	07/18/93 jmd	preened, removed free()
//	07/18/93 jmd	added new port fill interface
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	02/09/94 jmd	added getWidth, getHeight
//	03/05/94 pmcm	preened
//	07/20/94 jld	made invert() virtual for Ellipse & RndRectangle
//	07/21/94 jld	tempHndl private member variable deleted
// ----------------------------------------------------------------------------

#ifndef VRECT_H
#define VRECT_H
#include "region.h"

CLASS VPointArray;
CLASS VWindow;

extern VClass *VRectangleCls;

//
// The VRectangle class represents a rectangular area.
// An instance of VRectangle can be described by an origin
// (the top left corner) and a corner (the bottom right corner) point; 
// or by an origin and an extent point (the width and height of the
// rectangle).
//
typedef enum {
	Corners,
	CenterDim,
	CornerDim
} RectOpts;

CLASS VRectangle : public VRegion {
public:
			VRectangle();
			VRectangle(RectOpts, int, int, int, int);

	VClass *iam();

	VPointArray *asPoints();
	void	set(RectOpts, int, int, int, int);
	void	get(RectOpts, int *, int *, int *, int *);
	boolean	inset(int dx, int dy);
	boolean move(int dx, int dy); 
	boolean pointIn(int x, int y);

	int		getWidth() const;
	int		getHeight() const;

	VRegion *combine(VRegion *rgn, RegionRule rule); 

protected:
	int		left;		// X coordinate of the upper-left corner.
	int		top;		// Y coordinate of the upper-left corner.
	int		right;		// X coordinate of the lower-right corner.
	int		bottom;		// Y coordinate of the lower-right corner.

	HRGN	tempHndl;

	virtual HRGN getHndl();
	virtual void ungetHndl();

	// VPort interface routines
	virtual void fill(VPort *port);
	virtual void invert(VPort *port);

	friend	VPort;
	friend	VWindow;

};

// ---------------------------------------------------------------------------
inline int VRectangle::getWidth() const
{
	return right - left;
}

// ---------------------------------------------------------------------------
inline int VRectangle::getHeight() const
{
	return bottom - top;
}

// ---------------------------------------------------------------------------


#endif // VRECT_H
