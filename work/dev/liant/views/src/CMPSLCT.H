// $Header:   Q:/views/win/vcs/CMPSLCT.H_v   1.2   Mar 04 1997 14:12:04   gregm  $ 

//	cmpslct.h
//
//	VComponentSelect interface
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
//	01/31/96 dss	Initial revision.
// ---------------------------------------------------------------------------

#ifndef VCOMPONENTSELECT_H
#define VCOMPONENTSELECT_H

#include "dialog.h"

#include "cmpitem.h"

CLASS VPathString;
CLASS VString;

extern VClass *VComponentSelectCls;

//
// The `VComponentSelect` class is a modal dialog used to obtain VComponentObject's 
// from the user. `VComponentSelect` supports different VComponentObject sub-classes
// such as VComponentControl and VComponentProgrammable.
//

CLASS VComponentSelect : public VDialog {
public:

			VComponentSelect();

			// use this one
			VComponentSelect(const VString& title,
						     VComponentItem *& component,
						     VComponentWindow& parent,		// component's parent
						     VWindow *win,
						     int itemStyle = EmbeddedItem);

			~VComponentSelect();

	VClass *iam();

	// Static methods
	static boolean dialog(const VString& title,
						  VComponentItem *& component,
						  VComponentWindow& parent,		// component's parent
						  VWindow *win,
						  int itemStyle = EmbeddedItem);

	void operator()(const VString& title,
					VComponentItem *& component,
					VComponentWindow& parent,		// component's parent
					VWindow *win,
					int itemStyle);

};

#endif // VCOMPONENTSELECT_H

