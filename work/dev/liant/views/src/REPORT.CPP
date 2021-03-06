// $Header:   Q:/views/win/vcs/report.cpv   1.6   Mar 04 1997 17:55:44   gregm  $ 

//	report.cpp
//
//	VReport implementation
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
//	04/16/93 pmcm	added matching va_end for va_start
//	07/20/93 jmd	preened, added warning
//	07/25/93 jmd	added const
//	09/01/93 dgm	M8 changes.
//	10/11/94 dss	Added operator(). Constructor calls operator().
//	01/03/95 pkt	bulletproofed calls to vsprintf.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "report.h"
#include "notifier.h"

extern "C" {
#	include <stdio.h>
#	include <stdarg.h>
}

defineClass(VReport, VDialog)

// ---------------------------------------------------------------------------
VReport::VReport()
{
	;
}

// ---------------------------------------------------------------------------
// Perform a titled warning dialog box within 'win' with the message 'prompt'.
//
VReport::VReport(int type, const char *title, VWindow *win, const char *prompt)
{
	(*this)(type, title, win, prompt);
}

// ---------------------------------------------------------------------------
//
//
void VReport::operator()(int type, const char *title, VWindow *win, const char *prompt)
{
	HWND hWnd = win ? win->hwnd() : NIL;

	nativeDialog = TRUE;
	notifier->startModal( this);

	MessageBox((HWND)hWnd, (LPSTR)prompt, (LPSTR)title, type);

	// don't hide because the dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);
}

// ---------------------------------------------------------------------------
// Compatability with 1.0 only.
//
VReport::VReport(char *txt, VWindow *win)
{
	HWND hWnd = win ? win->hwnd() : NIL;

	nativeDialog = TRUE;
	notifier->startModal( this);

	MessageBox((HWND)hWnd, (LPSTR)txt, (LPSTR)"ALERT!!", MB_OK | MB_ICONEXCLAMATION);

	// don't hide because the dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);

}

// ---------------------------------------------------------------------------
// Compatability with 1.0 only.
//
VReport::VReport(char *txt, char *title, VWindow *win)
{
	HWND hWnd = win ? win->hwnd() : NIL;

	nativeDialog = TRUE;
	notifier->startModal( this);

	MessageBox((HWND)hWnd, (LPSTR)txt, (LPSTR)title, MB_OK);
	
	// don't hide because the dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);
}

// ---------------------------------------------------------------------------
VReport::~VReport()
{
	;
}

// ---------------------------------------------------------------------------
// Perform an alert dialog within window 'win' prompting the
// user with the result of a 'printf' style format.
//
void VReport::dialog(VWindow *win, const char *prompt...)
{
	char b[100];
	VString sb;
	va_list arg;
	va_start(arg, prompt);
	char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
	va_end(arg);
	if (bufp != 0) {
		va_start(arg, prompt);
		vsprintf(bufp, (char *)prompt, arg);
		va_end(arg);
		VReport r(MB_OK, 0, win, bufp);
	}
}

// ---------------------------------------------------------------------------
// Perform an non-alert dialog with title 'title' within window 'win', 
// prompting the user with the result of a 'printf' style format.
//
void VReport::dialog(const char *title, VWindow *win, const char *prompt...)
{
	char b[100];
	VString sb;
	va_list arg;
	va_start(arg, prompt);
	char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
	va_end(arg);
	if (bufp != 0) {
		va_start(arg, prompt);
		vsprintf(bufp, (char *)prompt, arg);
		va_end(arg);
		VReport r(MB_OK, title, win, bufp);
	}
}

// ---------------------------------------------------------------------------
// Display a titled dialog box within 'win' with the message 'txt'.  
// Label the box with 'title'.  The user must press ENTER or click the OK
// button to continue. Called by the dialog() static function.
//
void VReport::warning(const char *title, VWindow *win, const char *prompt...)
{
	char b[100];
	VString sb;
	va_list arg;
	va_start(arg, prompt);
	char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
	va_end(arg);
	if (bufp != 0) {
		va_start(arg, prompt);
		vsprintf(bufp, (char *)prompt, arg);
		va_end(arg);
		VReport r(MB_OK | MB_ICONEXCLAMATION, title, win, bufp);
	}
}

// ---------------------------------------------------------------------------
