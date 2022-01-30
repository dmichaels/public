// $Header:   Q:/views/win/vcs/icon.cpv   1.6   Mar 04 1997 17:55:12   gregm  $ 

//	icon.cpp
//
//	VIcon class
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
//	08/09/93 jmd	removed free
//	02/16/96 pkt	changed constructor to take a const char*.
// ----------------------------------------------------------------------------

#include "icon.h"
#include "notifier.h"

defineClass(VIcon, VObject)

VIcon::VIcon() : hpIcon(0)
{
	;
}

// ----------------------------------------------------------------------------
//
VIcon::~VIcon()
{
	;
}

// ----------------------------------------------------------------------------
//	Construct an VIcon object from the resource (or file) specified by
//	the C string 'iconName'.
//
VIcon::VIcon(const char *iconName)
{
	hpIcon = LoadIcon(notifier->inst(), (char *)iconName);
}

// ----------------------------------------------------------------------------
//	Place the width and height of this VIcon into '*w' and '*h'
//	respectfully. 
//
void VIcon::sizeOfIcon(int *w, int *h)
{
	*w = GetSystemMetrics(SM_CXICON);
	*h = GetSystemMetrics(SM_CYICON);
}

