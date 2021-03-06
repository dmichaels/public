// $Header:   Q:/views/win/vcs/POSTPAIN.H_v   1.5   Mar 04 1997 14:13:36   gregm  $ 

//	postpain.h
//
//	VPostPainter interface (private)
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
//	07/12/93 dgm	original;
// ----------------------------------------------------------------------------

#ifndef VPOSTPAIN_H
#define VPOSTPAIN_H

// --------------------------------------------------------------------------
// Include files
// --------------------------------------------------------------------------

#include "window.h"
#include "control.h"

// --------------------------------------------------------------------------
// Class definition
//
// This is an MS-Windows private class to help implement the
// VWindow::enablePostPaint() and VWindow::postPaint() methods
// which are used to allow a window to specify that it is to receive
// a VWIndow::postPaint() message *after* all its (immediate) children,
// which have been specified as such using VWindow::enablePostPaint(), have
// received all of there paint() messages in any series of paint() messages.
// --------------------------------------------------------------------------

CLASS VPostPainter : public VControl {
private:
			 VPostPainter(VWindow *);
			~VPostPainter();
	boolean		 paint();
	void		 queuePostPaint();
	VWindow		*owner;
	long		 pendingPostPaints;
	friend		 VWindow;
	friend		 VNotifier;
};

#endif // VPOSTPAIN_H
