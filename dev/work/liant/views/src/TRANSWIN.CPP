// $Header:   Q:/views/win/vcs/transwin.cpv   1.8   Mar 04 1997 17:55:58   gregm  $

//	transwin.cpp
//
//	VTransparentWindow implementation
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
//	08/05/93 dgm	original; also added support in VNotifier.
//	08/18/93 dgm	updated name parameter.
//	07/10/95 dss	Added getInfo().
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Include files
// --------------------------------------------------------------------------

#include "transwin.h"
#include "window.h"
#include "notifier.h"

// --------------------------------------------------------------------------
// Class identity definitions
// --------------------------------------------------------------------------

defineClass(VTransparentWindow, VWindow)

// --------------------------------------------------------------------------
// VTransparentWindow::VTransparentWindow
// --------------------------------------------------------------------------

VTransparentWindow::VTransparentWindow()
{
}

// --------------------------------------------------------------------------
// VTransparentWindow::VTransparentWindow
//
// The real work of constructing a transparent-window is done in the notifier
// which calls CreateWindowEx with a style of WS_EX_TRANSPARENT.
// --------------------------------------------------------------------------

VTransparentWindow::VTransparentWindow(const VFrame& frame,
									   VWindow *parent, const VStyle& style)
{
	(*this)(frame, parent, style);
}

// --------------------------------------------------------------------------
// VTransparentWindow::VTransparentWindow
// --------------------------------------------------------------------------

VTransparentWindow::VTransparentWindow(const VString& name,
									   const VFrame& frame,
									   VWindow *parent, const VStyle& style)
{
	(*this)(frame, parent, style, name);
}

// --------------------------------------------------------------------------
// VTransparentWindow::~VTransparentWindow
// --------------------------------------------------------------------------

VTransparentWindow::~VTransparentWindow()
{
}

// --------------------------------------------------------------------------
// VTransparentWindow::erased
// --------------------------------------------------------------------------

boolean VTransparentWindow::erased()
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
WinType VTransparentWindow::winType()
{
	return WinControl;
}

// --------------------------------------------------------------------------
//
void VTransparentWindow::getInfo(VWinInfo& info)
{
	VWindow::getInfo(info);
	info.exStyle |= WS_EX_TRANSPARENT;
}
