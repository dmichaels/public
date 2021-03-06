// $Header:   Q:/views/win/vcs/colrslct.h_v   1.4   Mar 04 1997 16:32:04   gregm  $

//	colrslct.h
//
//	VColorSelect class interface
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
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
//	06/09/95 jld	Initial version
//	03/21/96 dgm	Tidy up.
// ---------------------------------------------------------------------------

#ifndef VCOLRSLCT_H
#define VCOLRSLCT_H

// ---------------------------------------------------------------------------
//
#include "dialog.h"

// ---------------------------------------------------------------------------
// The VColorSelect class is a modal dialog used to obtain color 
// specifications from the user. 
//
CLASS VColorSelect : public VDialog {
public:

					VColorSelect();
					VColorSelect(VColor& color, VWindow *parentwin,
								 boolean& success);
					VColorSelect(short& red, short& green, short& blue,
								 VWindow *parentwin, boolean& success);
	VClass		   *iam();
	boolean			operator()(VColor&, VWindow *parentwin);

	static boolean	dialog(VColor& color, VWindow *parentwin);
	static boolean	dialog(short& red, short& green, short& blue,
						   VWindow *parentwin);
	static VColor  *dialog(VWindow *parentwin);
};

extern VClass *VColorSelectCls;

#endif // VCOLRSLCT_H
