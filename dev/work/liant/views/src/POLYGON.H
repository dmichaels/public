// $Header:   Q:/views/win/vcs/polygon.h_v   1.5   Mar 04 1997 16:32:48   gregm  $ 

//	polygon.h
//
//	VPolygon interface
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
//	07/21/93 pmcm	changed class to CLASS in forward declarations.
// ----------------------------------------------------------------------------

#ifndef VPOLYGON_H
#define VPOLYGON_H
#include "region.h"

CLASS VPointArray;

extern VClass *VPolygonCls;

//
// The VPolygon class defines a polygonal region via a set of points.
//

CLASS VPolygon : public VRegion {
public:
			VPolygon();
			VPolygon(VPointArray *pa);

	VClass *iam();

	void	set(VPointArray *pa);
	boolean move(int dx, int dy);
	boolean pointIn(int x, int y);
};

#endif // VPOLYGON_H
