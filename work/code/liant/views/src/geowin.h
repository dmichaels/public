//	geowin.h
//
//	VGeoWindow interface [Common]
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
//	07/29/97 dgm	Original.
// ---------------------------------------------------------------------------

#ifndef VGEOWIN_H
#define VGEOWIN_H

// --------------------------------------------------------------------------
//
#include "geomgr.h"
#include "window.h"

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VFrame;
CLASS VGeoFrame;
CLASS VObject;
CLASS VWindow;

// --------------------------------------------------------------------------
//
CLASS VGeoWindow : public VGeoManager {
public:
						VGeoWindow();
						VGeoWindow(const VFrame&, VGeoManager *parent);
						VGeoWindow(int x, int y, int w, int h,
								   VGeoManager *parent);
						VGeoWindow(int w, int h, VGeoManager *parent);
						VGeoWindow(VGeoManager *parent);
						VGeoWindow(const VFrame&, VWindow *parent_window);
						VGeoWindow(int x, int y, int w, int h,
								   VWindow *parent_window);
						VGeoWindow(int w, int h, VWindow *parent_window);
						VGeoWindow(VWindow *parent_window);
					   ~VGeoWindow();
	virtual VClass	   *iam();

public:
	virtual void		getPreferredSize(int *, int *);
	virtual boolean		getConstrainedSize(int *, int *, int, int);
	virtual void		getMinimumSize(int *, int *);
	virtual void		getMaximumSize(int *, int *);
	virtual void		frameChildren(int = 0, int = 0);
	virtual boolean		frameChildrenOk(int = 0, int = 0);
};

extern VClass *VGeoWindowCls;

#endif // VGEOWIN_H
