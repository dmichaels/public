// $Header:   Q:/views/win/vcs/printab.cpv   1.5   Mar 04 1997 17:55:38   gregm  $ 

//	printab.cpp
//
//	VPrintAbort dialog implemtentation
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//  08/09/93 jmd	removed free
//	03/09/94 pat	preened.
//	03/09/94 pat	STRICT:
//					added ABORTPROC casts;
//					updated abort procedure definition.
//	03/24/94 pat	CV_WIn32: auto-defined CV_WINDOWS.
//	07/14/94 jld	added defs.h dependency for CV_WINDOWS
//	10/11/94 dss	Added operator(). Constructor calls operator().
// ----------------------------------------------------------------------------

#include "defs.h"
#include "textbox.h"
#include "pushbttn.h"
#include "printer.h"
#include "printab.h"
#include "notifier.h"

defineClass(VPrintAbort, VDialog)

boolean bAbort = 0;

// ---------------------------------------------------------------------------
#if defined(DLL)
BOOL CALLBACK CV_DLL_MODIFIER CTalkAbortProc(HDC hPr, int Code)
#else
BOOL CALLBACK CTalkAbortProc(HDC hPr, int Code)
#endif
{
	MSG msg;

	/* Process messages intended for the abort dialog box */

	while (!bAbort && PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {       
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/* bAbort is TRUE (return is FALSE) if the user has aborted */

	return(!bAbort);
}

// ---------------------------------------------------------------------------
VPrintAbort::VPrintAbort() 
{ 
	; 
}

// ---------------------------------------------------------------------------
//	Construct a printer abort dialog with caption title, message prompt
//	prompt, for VPrinter object print within parent VWindow pWin. Upon
//	selection of the Cancel button, escape key, or closure of this dialog, the
//	abort() function of VPrinter object print is called. 
//
VPrintAbort::VPrintAbort(	char 		*title, 
							char 		*prompt, 
							VPrinter 	*print, 			
							VWindow 	*pWin) 
	: 	VDialog(64, 51, 147, 66, pWin)
{
	(*this)(title, prompt, print, pWin);
}

//---------------------------------------------------------------------------
//
void VPrintAbort::operator()(char *title, char *prompt, VPrinter *print, VWindow *pWin)
{
	HDC hDC;
	HANDLE devmode;

	pButton101 = new VPushButton(VFrame(0.380952F, 0.696970F, 0.238095F, 0.212121F), (VWindow *)this, StyleDefaultButton, "Cancel");
	pText100 = new VTextBox(VFrame(0.040816F, 0.106061F, 0.904762F, 0.515152F), (VWindow *)this, StyleCenter);

	if (title != 0) {
		setTitle(title);
	}
	if (prompt != 0) {
		pText100->putText(prompt);
	}

	pButton101->uponClick(this, methodOf(VPrintAbort, abortJob));

	bAbort = FALSE;
	print->getDevice(&hDC, &devmode);

	show();
	pButton101->takeFocus();

	if (hDC) {
		lpAbortProc = (ABORTPROC) MakeProcInstance(
									(FARPROC) CTalkAbortProc, notifier->inst());
#ifdef CV_WIN16
		SetAbortProc(hDC, lpAbortProc);
#else
		Escape(hDC, SETABORTPROC, 0, (LPCSTR) lpAbortProc, (LPVOID) 0);
#endif
	}
	else {
		lpAbortProc  = 0;
	}
}

// ---------------------------------------------------------------------------
//
VPrintAbort::~VPrintAbort()
{
	bAbort = TRUE;
	if (lpAbortProc) {
		FreeProcInstance((FARPROC) lpAbortProc);
	}
}

// ---------------------------------------------------------------------------
//
boolean VPrintAbort::abortJob(VButton *b)
{
	setTitle("Abort Print Job!");
	bAbort = TRUE;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VPrintAbort::close()
{
	abortJob(NIL);
	return TRUE;
}

// ---------------------------------------------------------------------------
