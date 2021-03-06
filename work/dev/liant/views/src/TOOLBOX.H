// $Header:   Q:/views/common/vcs/TOOLBOX.H_V   1.4   Mar 24 1997 16:13:56   scottt  $

//	toolbox.h
//
//	VToolBox interface [Common]
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
//	10/11/95 dgm	Original.
//	11/01/95 dgm	Updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTOOLBOX_H
#define VTOOLBOX_H

// --------------------------------------------------------------------------
//
#include "utilwin.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VStyle;
CLASS	VUtilityWindow;
CLASS	VWindow;

// --------------------------------------------------------------------------
//
CLASS VToolBox : public VUtilityWindow {
public:
						VToolBox();
						VToolBox(const VFrame&, VWindow *,
								 const VStyle& = StyleDefault);
						VToolBox(VWindow *, const VStyle& = StyleDefault);
					   ~VToolBox();
	VClass			   *iam();

protected:
	void				initialize();
	void				construct();
};

extern VClass *VToolBoxCls;

#endif // VTOOLBOX_H
