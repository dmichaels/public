// $Header:   Q:/views/win/vcs/printdlg.h_v   1.5   Mar 04 1997 16:32:52   gregm  $ 

//	printdlg.h
//
//	VPrintDialog interface
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
//	10/11/94 dss	Added operator().
// ----------------------------------------------------------------------------
   
#ifndef printdlg_h
#define printdlg_h
#include "dialog.h"

extern "C" {
#	include "commdlg.h"
}

//	The VPrinterDialog class is a modal dialog object designed for 
//	performing standard printer setup and print job startup dialogs.  Two 
//	static member functions, setup() and print(), are provided to execute the
//	standard setup and print dialogs, respectively.  These methods take an
//	optional pointer to an exisitng VPrinter object as a parameter to initialize the
//	various controls of the setup and/or print dialogs.  If the user selects Ok
//	from either of these dialogs, then the passed VPrinter object is updated to
//	reflect the new settings.  If a pointer to an existing VPrinter object is
//	not provided and the user selects Ok, then a new VPrinter object is
//	constructed and returned through the pointer to VPrinter object parameter.

CLASS VPrinter;

CLASS VPrintDialog : public VDialog {

public:

	static boolean print(char*, VPrinter **, VWindow *);
	static boolean setup(VPrinter **, VWindow *);

	VPrintDialog(boolean, char*, VPrinter **, VWindow *);
	VClass *iam();

	VPrintDialog();
	~VPrintDialog();

	void operator()(boolean setup, char *docName, VPrinter **printer, VWindow *pWin);

protected:

	PRINTDLG pd;
};

extern VClass *VPrintDialogCls;

#endif /* printdlg_h */
