// $Header:   Q:/views/win/vcs/colrslct.cpv   1.6   Mar 04 1997 17:54:48   gregm  $

//	colrslct.cpp
//
//	VColorSelect implementation
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
//	06/09/95 jld	Initial version
//	03/21/96 dgm	Fixed VColorSelect::dialog() to return null on cancel.
// ----------------------------------------------------------------------------

#include "colrslct.h"
#include "notifier.h"
#include "color.h"

extern "C" {
#	include "commdlg.h"
#	include <ctype.h>
#	undef NULL
}

defineClass(VColorSelect, VDialog)

// ---------------------------------------------------------------------------
//
VColorSelect::VColorSelect()
{
	;
}

// ---------------------------------------------------------------------------
//
VColorSelect::VColorSelect(	VColor& color,
							VWindow *parentwin,
							boolean& success)
{
	success = (*this)(color, parentwin);
}

// ---------------------------------------------------------------------------
//
boolean VColorSelect::dialog(VColor& color, VWindow *win)
{
	VColorSelect cs;
	return cs(color, win);
}

// ---------------------------------------------------------------------------
//
boolean VColorSelect::dialog(short& red,
							 short& green,
							 short& blue,
							 VWindow *win)
{
	VColor color;
	VColorSelect cs;
	if (!cs(color, win)) {
		return FALSE;
	}
	else {
		red	= color.getRed();
		green	= color.getGreen();
		blue	= color.getBlue();
		return TRUE;
	}
}

// ---------------------------------------------------------------------------
//
VColor* VColorSelect::dialog(VWindow *win)
{
	VColor* color = new VColor(0, 0, 0);
	VColorSelect cs;
	if (!cs(*color, win)) {
		return 0;
	}
	else {
		return color;
	}
}
	
// ---------------------------------------------------------------------------
// Where it actually happens...
//
boolean VColorSelect::operator()(VColor& color, VWindow *pWin)
{
	boolean ret = FALSE;

	// Initialize custom color array to all white
	const unsigned long init_clr = RGB(255,255,255);
	unsigned long cust_clr[16] = {init_clr, init_clr, init_clr, init_clr,
									init_clr, init_clr, init_clr, init_clr,
									init_clr, init_clr, init_clr, init_clr,
									init_clr, init_clr, init_clr, init_clr
									};

	CHOOSECOLOR colorChoice;

	colorChoice.lStructSize		= sizeof(CHOOSECOLOR);
	colorChoice.hwndOwner		= ((pWin == 0) ? 0 : pWin->hwnd());
	colorChoice.hInstance		= 0;
	colorChoice.rgbResult		= 0;
	colorChoice.lpCustColors	= cust_clr;
	colorChoice.Flags			= 0;
	colorChoice.lCustData		= 0;
	colorChoice.lpfnHook		= 0;
	colorChoice.lpTemplateName	= 0;

	nativeDialog = TRUE;
	notifier->startModal( this );

	ret = (boolean) ChooseColor( (CHOOSECOLOR*) &colorChoice );

	// don't hide because the common dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);

	if (ret) {
		color = (VColor) colorChoice.rgbResult;
	}

	return ret;
}

// ---------------------------------------------------------------------------
