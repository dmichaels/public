//	geowin.cpp
//
//	VGeoWindow implementation [Common]
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

#include "geowin.h"

// --------------------------------------------------------------------------
//
defineClass(VGeoWindow, VGeoManager)

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(const VFrame& frame, VGeoManager *parent)
	: VGeoManager(frame, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(int x, int y, int w, int h,
							  VGeoManager *parent)
	: VGeoManager(x, y, w, h, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(int w, int h, VGeoManager *parent)
	: VGeoManager(w, h, parent)
{
}

VGeoWindow::VGeoWindow(VGeoManager *parent)
	: VGeoManager(parent)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(const VFrame& frame, VWindow *parent_window)
	: VGeoManager(frame, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(int x, int y, int w, int h,
							  VWindow *parent_window)
	: VGeoManager(x, y, w, h, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(int w, int h, VWindow *parent_window)
	: VGeoManager(w, h, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow(VWindow *parent_window)
	: VGeoManager(parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::VGeoWindow()
	: VGeoManager()
{
}

// --------------------------------------------------------------------------
//
VGeoWindow::~VGeoWindow()
{
}

// --------------------------------------------------------------------------
//
void VGeoWindow::getPreferredSize(int *w, int *h)
{
	VGeoManager::getPreferredSize(w, h);
}

// --------------------------------------------------------------------------
//
boolean VGeoWindow::getConstrainedSize(int *w, int *h, int cw, int ch)
{
	return VGeoFrame::getConstrainedSize(w, h, cw, ch);
}

// --------------------------------------------------------------------------
//
void VGeoWindow::getMinimumSize(int *w, int *h)
{
	VGeoFrame::getMinimumSize(w, h);
}

// --------------------------------------------------------------------------
//
void VGeoWindow::getMaximumSize(int *w, int *h)
{
	VGeoFrame::getMaximumSize(w, h);
}

// --------------------------------------------------------------------------
//
void VGeoWindow::frameChildren(int w, int h)
{
	VWindow *window = getWindow();
	if (window != 0) {
		window->frameChildren(w, h);
	}
	else {
		VGeoManager::frameChildren(w, h);
	}
}

// --------------------------------------------------------------------------
//
boolean VGeoWindow::frameChildrenOk(int w, int h)
{
	VWindow *window = getWindow();
	if (window != 0) {
		return window->frameChildren(w, h, TRUE);
	}
	else {
		return VGeoManager::frameChildrenOk(w, h);
	}
}
