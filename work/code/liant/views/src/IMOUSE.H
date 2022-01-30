//	imouse.h -- [UNDER DEVELOPMENT]
//
//	VIntelliMouse interface
//
//  Allegris Foundation 1.1.00
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
//	12/06/96 dgm	Original.  For a complete description and SDK, see:
//					http://www.microsoft.com/products/hardware/
//											 intellimouse/sdkhome.htm.
// --------------------------------------------------------------------------

#ifndef IMOUSE_H
#define IMOUSE_H

// --------------------------------------------------------------------------
//
#include "defs.h"

// --------------------------------------------------------------------------
//
class VIntelliMouse {
public:
	static boolean		isPresent();
	static int			getScrollLinesPerWheelRotation();

public:
	static void			initialize();
	static void			systemSettingsChanged();
	static boolean		handleWheelMessage(WPARAM, LPARAM);

public:
	static unsigned int	wheelMessageId;
};

#endif // IMOUSE_H
