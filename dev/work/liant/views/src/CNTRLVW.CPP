// $Header:   Q:/views/common/vcs/cntrlvw.cpv   1.6   May 30 1997 17:14:50   scottt  $ 

//	cntrlvw.cpp
//
//	VControlView implementation [Common]
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
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	05/16/95 pkt	preened.
//	09/01/95 dss	VStyle changes.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "cntrlvw.h"
#include "control.h"
#include "cvassert.h"

defineClass(VControlView, VView)

// ---------------------------------------------------------------------------
// Construct a `VControlView` with no presentation space.
//
VControlView::VControlView()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a `VView` with frame 'frame', style 'style' for `VControl`
// class 'cntrl'. The style for the `VControl` object is optionally specified
// by 'cntrlStyle'.
//
VControlView::VControlView(const VFrame& frame, VWindow *win, VClass *cntrl, const VStyle& style, const VStyle& cntrlStyle) 
{
	(*this)(frame, win, style);
	control = (VControl *)cntrl->construct();
	cv_assert(cntrl && cntrl->isa(VControlCls));
	(*control)(VFrame(1.0F, 1.0F), this, cntrlStyle);
	show();
}

// ---------------------------------------------------------------------------
//
boolean VControlView::prevChildFocus(VWindow *)
{
	return parent->prevChildFocus(this);
}

// ---------------------------------------------------------------------------
//
boolean VControlView::nextChildFocus(VWindow *)
{
	return parent->nextChildFocus(this);
}

// ---------------------------------------------------------------------------
//
VControlView::~VControlView()
{
	delete control;
}
