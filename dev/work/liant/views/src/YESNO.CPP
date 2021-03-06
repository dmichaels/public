// $Header:   Q:/views/win/vcs/yesno.cpv   1.6   Mar 04 1997 15:36:20   gregm  $ 

//	yesno.cpp
//
//	VYesNo implementation
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
//	04/16/93 pat	added matching va_end for va_start
//	07/20/93 jmd	preened, added const VString
//	10/11/94 dss	Added operator(). Constructor calls operator().
//	01/03/96 pkt	bulletproofed calls to vsprintf.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "yesno.h"
#include "notifier.h"

extern "C" {
#	include <stdarg.h>
#	include <stdio.h>
}

defineClass(VYesNo, VDialog)

// ---------------------------------------------------------------------------
//
VYesNo::VYesNo()
{
	;
}

// ---------------------------------------------------------------------------
// Put up a dialog which displays 'txt' and presents a YES and
// NO button, when this constructor returns the response is returned
// in '*opt' as one of enum yesNoCancel constants YesButton or NoButton. 
// If 'cancel' is TRUE, then add a CANCEL button which will cause
// CancelButton to be returned if hit.  If the user hits the return key, 
// then the default button given as '*opt' will be returned.  The VYesNo object
// returned by this constructor should be freed.  Called by the static functions
// of VYesNo.
//
VYesNo::VYesNo(int which, boolean cancel, enum yesNoCancel *opt,
			   const char *txt, const char *title, VWindow *win)
{
	(*this)(which, cancel, opt, txt, title, win);
}

// ---------------------------------------------------------------------------
//
void VYesNo::operator()(int which, boolean cancel, enum yesNoCancel *opt,
			   const char *txt, const char *title, VWindow *win)
{
	which |= cancel ? MB_YESNOCANCEL: MB_YESNO;
	HWND hWnd = win ? win->hwnd() : NIL;

	if (!opt) {
		return;
	}

	switch (*opt) {
	case CancelButton:
		which |= MB_DEFBUTTON3;	
		break;
	case YesButton:
		break;
	case NoButton:
		which |= MB_DEFBUTTON2;	
		break;
	}

	nativeDialog = TRUE;
	notifier->startModal( this);

	switch (MessageBox(hWnd, (LPSTR)txt, (LPSTR)title, which)) {
	case IDYES:
		*opt = YesButton;
		break;
	case IDNO:
		*opt = NoButton;
		break;
	default:
		*opt = CancelButton;
		break;
	}

	// don't hide because the dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);
}

// ---------------------------------------------------------------------------
// Perform a yes/no style dialog within window 'win' with a 'printf'
// style prompt.  If 'cancel' is TRUE, then provide a CANCEL button, otherwise,
// provide YES and NO buttons only.  Set the default button to 'dlft' which is
// one of the enum yesNoCancel constants: YesButton, NoButton, or
// CancelButton.  Return the button selected by the user as one of the
// enum yesNoCancel constants.
//
enum yesNoCancel VYesNo::dialog(VWindow *win, boolean cancel,
								enum yesNoCancel dflt, const char *prompt...)
{
	enum yesNoCancel e = dflt;
	char b[100];
	VString sb;
	va_list arg;
	va_start(arg, prompt);
	char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
	va_end(arg);
	if (bufp != 0) {
		va_start(arg, prompt);
		vsprintf(bufp, (char *) prompt, arg);
		va_end(arg);
		VYesNo yesno(0, cancel, &e, bufp, 0, win);
	}
	return e;
}

// ---------------------------------------------------------------------------
// Perform a yes/no user-warning style dialog with title 'title', 
// within window 'win' with a 'printf' style prompt.  Show the 'exclamation'
// icon in the dialog box.  If 'cancel' is TRUE, then provide a CANCEL button, 
// otherwise, provide YES and NO buttons only.  Set the default button to
// 'dlft' which is one of the enum yesNoCancel constants: YesButton, 
// NoButton, or CancelButton.  Return the button selected by the user as
// one of the enum yesNoCancel constants.
//
enum yesNoCancel VYesNo::warning(boolean cancel, enum yesNoCancel dflt,
								 const char *title, VWindow *win,
								 const char *prompt...)
{
	enum yesNoCancel e = dflt;
	char b[100];
	VString sb;
	va_list arg;
	va_start(arg, prompt);
	char *bufp = VString::selectFormatBuffer(b, 100, &sb, prompt, arg);
	va_end(arg);
	if (bufp != 0) {
		va_start(arg, prompt);
		vsprintf(bufp, (char *) prompt, arg);
		va_end(arg);
		VYesNo yesno(MB_ICONEXCLAMATION, cancel, &e, bufp, title, win);
	}
	return e;
}

// ---------------------------------------------------------------------------
// Perform a yes/no user-action style dialog with title 'title', within
// window 'win' with a 'printf' style prompt.  Show the 'stop-sign' icon in the
// dialog box. If 'cancel' is TRUE, then provide a CANCEL button, otherwise, 
// provide YES and NO buttons only.  Set the default button to 'dlft' which is
// one of the enum yesNoCancel constants: YesButton, NoButton, or
// CancelButton.  Return the button selected by the user as one of the
// enum yesNoCancel constants.
//
enum yesNoCancel VYesNo::action(boolean cancel, enum yesNoCancel dflt,
								const char *title, VWindow *win,
								const char *prompt...)
{
	enum yesNoCancel e = dflt;
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
		VYesNo yesno(MB_ICONSTOP, cancel, &e, bufp, title, win);
	}
	return e;
}

// ---------------------------------------------------------------------------
// Perform a yes/no user-question style dialog with title
// 'title', within window 'win' with a 'printf' style prompt.  Show the
// 'question mark' icon in the dialog box. If 'cancel' is TRUE, then provide
// a CANCEL button, otherwise, provide YES and NO buttons only.  Set the
// default button to 'dlft' which is one of the enum yesNoCancel constants: 
// YesButton, NoButton, or CancelButton.  Return the button selected by
// the user as one of the enum yesNoCancel constants.
//
enum yesNoCancel VYesNo::question(boolean cancel, enum yesNoCancel dflt,
								  const char *title, VWindow *win,
								  const char *prompt...)
{
	enum yesNoCancel e = dflt;
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
		VYesNo yesno(MB_ICONQUESTION, cancel, &e, bufp, title, win);
	}
	return e;
}

// ---------------------------------------------------------------------------
