/*
$Header:   Q:/views/win/vcs/INPUT.H_V   1.9   Mar 24 1997 16:18:02   scottt  $ 
*/
//	input.h
//
//	VInput class interface
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1996 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	05/24/93 jmd	preened, made protected data private
//	10/11/94 dss	Added operator().
//  08/27/96 dgm    Reworked layout.
//  09/05/96 dgm    Added getEditLine().
// ----------------------------------------------------------------------------

#ifndef VINPUT_H
#define VINPUT_H

// ----------------------------------------------------------------------------
//
#include "dialog.h"

// ----------------------------------------------------------------------------
//
CLASS VClass;
CLASS VDialog;
CLASS VEditLine;
CLASS VString;
CLASS VWindow;

// ----------------------------------------------------------------------------
// The VInput class is a modal dialog object designed for user text
// entry.  An VInput object takes a C string for use as a prompt message to
// the user, and optionally, an instance of a VString object to be used as
// the default text entry. 

CLASS VInput : public VDialog {
public:
					VInput();
					VInput(const char *, const char *, VString **, VWindow *);
				   ~VInput();
	VClass		   *iam();

	void			operator()(const char *title, const char *prompt,
							   VString **s, VWindow *);

	static VString *dialog(VString *, const char *,
						   VWindow *, const char *...);
	static VString *dialog(const char *, VString *, VWindow *);

	VEditLine	   *getEditLine() const;

protected:
	boolean			givenFocus();

private:
	VEditLine	   *editLine;
};

extern VClass *VInputCls;

// ----------------------------------------------------------------------------
//
inline VEditLine *VInput::getEditLine() const
{
	return editLine;
}

#endif
