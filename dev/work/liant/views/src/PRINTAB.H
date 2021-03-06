// $Header:   Q:/views/win/vcs/printab.h_v   1.5   Mar 04 1997 16:32:50   gregm  $ 

//	printab.h
//
//	VPrintAbort interface
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
//	03/09/94 pmcm	preened; made VTextBox and VPushButton members protected;
//					removed typedef of FARPROC
//	03/09/94 pmcm	STRICT:	abort proc is now ABORTPROC, not FARPROC
//	10/11/94 dss	Added operator().
// ----------------------------------------------------------------------------

#ifndef VPRINTAB_H
#define VPRINTAB_H

#include "dialog.h"

CLASS VTextBox;
CLASS VPushButton;
CLASS VPrinter;
CLASS VButton;

//	The VPrintAbort class is a modeless dialog used to provide a
//	mechanism for the user to cancel and abort a print job. This dialog
//	works with a specific VPrinter object to invoke its abort function.
//	To work properly, the VPrintAbort dialog must be constructed through
//	a VPrinter object by calling its doAbortDialog() member function.  
//	The implementation of this dialog is dependant on the underlying 
//	GUI system.

CLASS VPrintAbort : public VDialog {

public:
					VPrintAbort();
					VPrintAbort(char 	   *title, 
								char 	   *prompt, 
								VPrinter   *prntr, 
								VWindow    *pwin);
					~VPrintAbort();
	VClass 		   *iam();

	void operator()(char *title, char *prompt, VPrinter *prntr, VWindow *pwin);

protected:

	ABORTPROC  		lpAbortProc;

	VTextBox 	   *pText100;
	VPushButton    *pButton101;

	boolean 		abortJob(VButton *bttn);
	boolean 		close();

};

extern VClass *VPrintAbortCls;

#endif	//	VPRINTAB_H
