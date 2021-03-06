// $Header:   Q:/views/win/vcs/transwin.h_v   1.7   Mar 04 1997 16:33:12   gregm  $

//	transwin.h
//
//	VTransparentWindow interface
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
//	08/05/93 dgm	Original; also added support in VNotifier.
//	08/18/93 dgm	Updated name parameter.
//	10/31/94 dgm	Added winType().
//	07/10/95 dss	Added getInfo().
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#ifndef TRANSWIN_H
#define TRANSWIN_H

// --------------------------------------------------------------------------
// Include files
// --------------------------------------------------------------------------

#include "window.h"

// --------------------------------------------------------------------------
// External class declarations
// --------------------------------------------------------------------------

CLASS VClass;
CLASS VFrame;
CLASS VWindow;
CLASS VWinInfo;

// --------------------------------------------------------------------------
// Class definitions
// --------------------------------------------------------------------------

CLASS VTransparentWindow : public VWindow {

public:
					 VTransparentWindow();
					 VTransparentWindow(const VFrame& frame,
										VWindow *parent,
										const VStyle& style = StyleDefault);
					 VTransparentWindow(const VString&,
										const VFrame& frame,
										VWindow *parent,
										const VStyle& style = StyleDefault);
					~VTransparentWindow();
	VClass			*iam();

protected:
	void 			getInfo(VWinInfo& info);

	boolean			erased();
	WinType			winType();
};

extern VClass *VTransparentWindowCls;

#endif // TRANSWIN_H
