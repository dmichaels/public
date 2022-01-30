// $Header:   Q:/views/common/vcs/LOCATION.CPV   1.8   Mar 24 1997 16:09:34   scottt  $ 

//	location.cpp
//
//	VLocation class implementation [Common]
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
//	07/24/93 jmd	preened
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "location.h"
#include "archiver.h"

// ---------------------------------------------------------------------------
//
defineClass(VLocation, VObject)

// ---------------------------------------------------------------------------
// Construct a VLocation at the origin (0,0).
//
VLocation::VLocation()
{
	xp = yp = 0;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store this VLocation into the VArchiver a.
//
void VLocation::putTo(VArchiver& a)
{
	a << xp << yp;
}

// ---------------------------------------------------------------------------
// Read this VLocation from VArchiver a.
//
void VLocation::getFrom(VArchiver& a)
{
	a >> xp >> yp;
}

// ----------------------------------------------------------------------------
//
void VLocation::getFrom(VArchiver& a,
					    VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER
