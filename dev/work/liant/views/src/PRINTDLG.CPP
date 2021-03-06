// $Header:   Q:/views/win/vcs/printdlg.cpv   1.10   Mar 04 1997 17:55:38   gregm  $ 

//	printdlg.cpp
//
//	VPrintDialog implementation
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
//	10/11/94 dss	Added operator(). Constructor calls operator().
//  08/14/95 evc    inited 'pd' struct to 0 before its assigned fixes NT 3.51
//					problem (dialog wouldn't come up in demogfx)
//  08/15/95 evc	added memory.h as include to build 16 bit version
//	01/17/96 glm	modified PRINTDLG struct for Win95 API
// ----------------------------------------------------------------------------

#include "printdlg.h"
#include "printer.h"
#include "notifier.h"
#include <memory.h>

defineClass(VPrintDialog, VDialog)

VPrintDialog::VPrintDialog()
{
	;
}

VPrintDialog::~VPrintDialog()
{
	;
}

// Construct a standard printer 'setup' or 'print' dialog within
// VWindow 'pWin'.  If 'setup' is TRUE, then present the standard printer
// setup dialog, otherwise present the standard 'print' dialog on the default
// installed printer.  Return TRUE if the user selected the OK button, otherwise, 
// return FALSE.  If a VPrinter object is provided in '*printer', then use the
// settings of that VPrinter to initialize the controls of this dialog.  If
// TRUE is returned, then the device modes of the VPrinter passed in '*printer'
// are updated.  If TRUE is returned and a NIL pointer to VPrinter was passed
// in '*printer', then a new VPrinter object is constructed and returned in
// '*printer'.

VPrintDialog::VPrintDialog(boolean setup, char *docName, VPrinter **printer, VWindow *pWin)
{
	(*this)(setup, docName, printer, pWin);
}

//---------------------------------------------------------------------------
//

void VPrintDialog::operator()(boolean setup, char *docName, VPrinter **printer, VWindow *pWin)
{
	HDC hdc = 0;
	HGLOBAL hdevmode = 0;

	if (*printer) {
		(*printer)->getDevice(&hdc, &hdevmode);
	}
	// initialize pd to 0 first
	memset(&pd,0, sizeof(pd));

	pd.lStructSize  = sizeof(PRINTDLG);
	pd.hwndOwner    = ((pWin == 0) ? 0 : pWin->hwnd());
	pd.hDevMode     = hdevmode;
	pd.hDevNames    = 0;
	pd.Flags        = PD_RETURNDC | PD_PAGENUMS; 
	if (setup) {
		pd.Flags |= PD_PRINTSETUP;
	}
	pd.nCopies = 1;
	pd.nFromPage = 1;
	pd.nToPage = 1;
	pd.nMinPage = 1;
	pd.nMaxPage = 1;


	nativeDialog = TRUE;
	notifier->startModal( this);

	if (PrintDlg((LPPRINTDLG)&pd) && pd.hDC) {
		if (!*printer) {
			*printer = new VPrinter(docName, TRUE);
		}
		(*printer)->setDevice(pd.hDC, pd.hDevMode);
		if (pd.hDevNames) {
			GlobalFree(pd.hDevNames);
			pd.hDevNames = 0;
		}
		result = TRUE;
	}
	else {
		result = FALSE;
	}

	// don't hide because the dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);
}

// Perform a standard printer 'print' dialog within VWindow 'pWin'.  
// Return TRUE if the user selected the OK button, otherwise, return FALSE.  If
// a VPrinter object is provided in '*printer', then use the settings of that
// VPrinter to initialize the controls of this dialog.  If TRUE is returned, 
// then the device modes of the VPrinter passed in '*printer' are updated.  
// If TRUE is returned and a NIL pointer to VPrinter was passed in '*printer',
// then a new VPrinter object is constructed and returned in '*printer'. This
// is useful for preserving the device modes and settings of a printer and then
// passing them on to the VPrintDialog\'s static setup() function or to
// subsequent print() calls. 

boolean VPrintDialog::print(char *docName, VPrinter **printer, VWindow *pWin)
{
	VPrintDialog pd(FALSE, docName, printer, pWin);
	return(pd.result);
}

// Perform a printer set up dialog within VWindow 'pWin'.  Return
// TRUE if the user selected the OK button, otherwise, return FALSE.  If a
// VPrinter object is provided in '*printer', then use the settings of that
// VPrinter to initialize the controls of this dialog.  If TRUE is returned, 
// then the device modes of the VPrinter passed in '*printer' are updated.  
// If TRUE is returned and a NIL pointer to VPrinter was passed in '*printer',
// then a new VPrinter object is constructed and returned in '*printer'. This
// is useful for preserving the device modes and settings of a printer and then
// passing them on to the VPrintDialog\'s static print() function. 

boolean VPrintDialog::setup(VPrinter **printer, VWindow *pWin)
{
	VPrintDialog pd(TRUE, NIL, printer, pWin);
	return(pd.result);
}
