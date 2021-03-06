// $Header:   Q:/views/win/vcs/wininfo.h_v   1.5   Mar 04 1997 16:33:18   gregm  $ 

//	wininfo.h
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
// ----------------------------------------------------------------------------

#ifndef wininfo_h
#define wininfo_h

#include "defs.h"

//	The `VWinInfo` class is used to encapsulate platform dependent
//	information. It is used by C++/Views for the creation of 'windows' in the
//	underlying GUI environment, for example, MS Windows. Setting values in
//	`VWinInfo` objects is often required when subclassing the various `VWindow`
//	classes within C++/Views (see the various `getInfo()` methods in the
//	`VWindow` hierarchy).

CLASS VWinInfo {

	public:
		char *wClass;

		unsigned long style;
  
		unsigned long exStyle;

		VWinInfo() { };
		~VWinInfo() { };
};

#endif /* wininfo_h */
