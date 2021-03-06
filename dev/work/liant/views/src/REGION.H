// $Header:   Q:/views/win/vcs/region.h_v   1.5   Mar 04 1997 16:32:58   gregm  $ 

//	region.h
//
//	VRegion interface
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
// ----------------------------------------------------------------------------

#ifndef VREGION_H
#define VREGION_H
#include "wintypes.h"
#include "object.h"

CLASS VPort;

typedef enum {
	RgnDifference = RGN_DIFF,
	RgnIntersect = RGN_AND,
	RgnUnion = RGN_OR,
	RgnXor = RGN_XOR,
	RgnCopy = RGN_COPY
} RegionRule;

// A VRegion is an abstract super class which
// represents a closed area.  Classes derived from VRegion implement
// different types of closed regions.

CLASS VRegion : public VObject {
public:
			VRegion();
			~VRegion();
	VClass *iam();

	VRegion *combine(VRegion *rgn, RegionRule rule);
	virtual boolean move(int dx, int dy);
	virtual boolean pointIn(int x, int y);

protected:

	HRGN	hRgn;

	virtual HRGN getHndl();
	virtual void ungetHndl();

	// VPort interface routines
	virtual	void fill(VPort *port);
	virtual void invert(VPort *port);

	friend	VPort;
};

extern VClass *VRegionCls;

#endif // VREGION_H
