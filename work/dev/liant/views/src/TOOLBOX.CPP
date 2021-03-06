// $Header:   Q:/views/common/vcs/TOOLBOX.CPV   1.4   Mar 24 1997 16:13:56   scottt  $

//	toolbox.cpp
//
//	VToolBox implementation [Common]
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
//	10/10/95 dgm	Original.
//	11/01/95 dgm	Updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "toolbox.h"
#include "georow.h"
#include "geocol.h"
#include "clrscm.h"

// --------------------------------------------------------------------------
//
defineClass(VToolBox, VUtilityWindow)

// --------------------------------------------------------------------------
//
VToolBox::VToolBox()
	: VUtilityWindow()
{
}

// --------------------------------------------------------------------------
//
VToolBox::VToolBox(const VFrame& frame, VWindow *parentwin, const VStyle& s)
	: VUtilityWindow(frame, parentwin, s)
{
	initialize();
	construct();
}

// --------------------------------------------------------------------------
//
VToolBox::VToolBox(VWindow *parentwin, const VStyle& s)
	: VUtilityWindow(parentwin, s)
{
	initialize();
	construct();
}

// --------------------------------------------------------------------------
//
VToolBox::~VToolBox()
{
}

// --------------------------------------------------------------------------
//
void VToolBox::initialize()
{
	setMovingStyle(VToolBox::TitleBarMoving | VToolBox::ClientAreaMoving);
	setResizingStyle(VToolBox::PreferredResizing);
	setCloseBehavior(VToolBox::DeleteOnClose);
}

// --------------------------------------------------------------------------
//
void VToolBox::construct()
{
	VWindow *client = getClientAreaWindow();

	VGeoRow *geoRow = new VGeoRow;
	client->setGeoManager(geoRow);

	geoRow->setMargins(5, 5, 5, 5);
	geoRow->setDefaultChildMargins(5, 5, 5, 5);
	geoRow->setWrapping(TRUE);
	geoRow->setSpacing(VGeoRow::NoSpacing);
	geoRow->setDefaultChildJustify(VGeoRow::CenterJustify);
}
