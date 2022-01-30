// $Header:   Q:/views/win/vcs/fontslct.h_v   1.5   Mar 04 1997 16:32:24   gregm  $ 

//	fontslct.h
//
//	VFontSelect class interface
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
//	05/28/93 jmd	preened
//	07/10/93 jmd	added new members and features
//	07/20/93 jmd	changed rgbColor to VColor
//	08/09/93 jmd	removed free
//	10/11/94 dss	Added operator().
// ---------------------------------------------------------------------------

#ifndef VFONTSLCT_H
#define VFONTSLCT_H
#include "dialog.h"

CLASS VFont;
CLASS VPrinter;

//
// The VFontSelect class is a modal dialog object designed for user
// font selection.  A VFontSelect dialog can display for selection screen
// fonts, printer fonts, or both.  The printer fonts to be displayed can
// be controlled by providing a VPrinter object representing the printer
// of interest. Otherwise, if no VPrinter object is specified, then
// the VFontSelect dialog looks at the default installed printer
// for the available printer fonts.
//

extern VClass *VFontSelectCls;

CLASS VFontSelect : public VDialog {
public:

	// styles can be |'ed together
	enum DlgStyle { ScreenFonts = 0x01, PrinterFonts = 0x02, Effects = 0x04 };

			// use this one
			VFontSelect(VFont **font,
						VWindow *pWin,
						boolean& success,
						VColor& color,
						VPrinter *print = 0,
						DlgStyle dstyle = VFontSelect::ScreenFonts);

			// for v2.0 compatability
			VFontSelect(boolean all,
						VFont **font,
						VPrinter *print,
						VWindow *pwin);

			VFontSelect();
			~VFontSelect();

	VClass *iam();

	static VFont *dialog(boolean all, VPrinter *print, VWindow *pwin);
	static VFont *dialog(boolean all, VWindow *pwin);
	static boolean dialog(VFont **font, boolean all, VWindow *pWin);

	boolean operator()(VFont **font, VWindow *pWin, VColor& color,
							VPrinter *print, DlgStyle dstyle);
};

#endif // VFONTSLCT_H
