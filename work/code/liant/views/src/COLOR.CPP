// $Header:   Q:/views/win/vcs/color.cpv   1.8   Mar 04 1997 17:54:48   gregm  $ 

//	color.cpp
//
//	VColor implementation
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
//	07/19/93 jmd	created
//	09/29/93 dgm	updated
//	10/09/93 jmd	preened
//	10/13/93 jmd	removed static members
//	02/17/94 pat	made v2.x globals Gray,DarkGray,LightGray CV_NEARDATA.
//	03/08/94 pat	included winclass.h before wintypes.h to prevent
//					re-declaration of windows.h stuff
//	03/25/94 pat	defined CV_NEARDATA for CV_WIN32
//	07/13/94 jld	added dependency on color.h and removed redundancies
//	12/05/95 dgm	Added destructor and call to
//					VColorScheme::checkSystemColorDeletion().
//	01/09/96 dgm	Added assertion for shared system color deletion.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "wintypes.h"
#include "color.h"
#include "clrscm.h"
#include "cvassert.h"

// ---------------------------------------------------------------------------

const long ColorTable[] = {
		RGB(0, 0, 0),			// Black
		RGB(0, 0, 127),			// DarkBlue
		RGB(0, 127, 0),			// DarkGreen
		RGB(0, 127, 127),		// DarkCyan
		RGB(127, 0, 0),			// DarkRed
		RGB(127, 0, 127),		// DarkMagenta
		RGB(127, 127, 0),		// DarkYellow
		RGB(192, 192, 192),		// LightGray
		RGB(127, 127, 127),		// Gray
		RGB(110, 110, 110),		// DarkGray
		RGB(0, 0, 255),			// Blue
		RGB(0, 255, 0),			// Green
		RGB(0, 255, 255),		// Cyan
		RGB(255, 0, 0),			// Red
		RGB(255, 0, 255),		// Magenta
		RGB(255, 255, 0),		// Yellow
		RGB(255, 255, 255)		// White
};

// ---------------------------------------------------------------------------
VColor::VColor(ColorValue cv)
{
	rgbValue = ColorTable[(int)cv];
}

// ---------------------------------------------------------------------------
short VColor::getRed() const
{
	return (short)GetRValue(rgbValue);
}

// ---------------------------------------------------------------------------
short VColor::getGreen() const
{
	return (short)GetGValue(rgbValue);
}

// ---------------------------------------------------------------------------
short VColor::getBlue() const
{
	return (short)GetBValue(rgbValue);
}

// --------------------------------------------------------------------------
//
VColor::~VColor()
{
	boolean deleting_non_shared_resource = !VColorScheme::
										    checkSystemColorDeletion(this);
	cv_assert(deleting_non_shared_resource);
}
