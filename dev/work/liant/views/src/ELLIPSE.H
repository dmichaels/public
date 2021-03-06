// $Header:   Q:/views/win/vcs/ellipse.h_v   1.5   Mar 04 1997 16:32:20   gregm  $ 

//	ellipse.h
//
//	VEllipse class interface
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
//	03/05/94 pmcm	preened
//	07/20/94 jld	added invert to overwrite VRectangle::invert
// ---------------------------------------------------------------------------

#ifndef VELLIPSE_H
#define VELLIPSE_H
#include "rect.h"

extern VClass *VEllipseCls;

//
// The VEllipse class defines an elliptical region which is specified
// through the coordinates of a bounding rectangle.
//

CLASS VEllipse : public VRectangle {

public:
					VEllipse();
					VEllipse(RectOpts, int, int, int, int);

	VClass 		   *iam();

protected:

	virtual HRGN 	getHndl();
	void			fill(VPort *port);
	void			invert(VPort *port);
};

#endif // VELLIPSE_H
