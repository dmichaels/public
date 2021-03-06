// $Header:   Q:/views/win/vcs/fileslct.h_v   1.7   Mar 04 1997 16:32:24   gregm  $ 

//	fileslct.h
//
//	VFileSelect interface
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
//	05/27/93 jmd	preened
//	06/28/93 jmd	added new members and features
//	10/11/94 dss	Added operator().
//	05/16/96 dgm	Added setRememberInitialPath().
// ---------------------------------------------------------------------------

#ifndef VFILESLCT_H
#define VFILESLCT_H

// ---------------------------------------------------------------------------
//
#include "dialog.h"
#include "strm.h"

// ---------------------------------------------------------------------------
//
CLASS VClass;
CLASS VDialog;
CLASS VPathString;
CLASS VString;
CLASS VWindow;

// ---------------------------------------------------------------------------
// The VFileSelect class is a modal dialog used to obtain file pathname 
// specifications from the user.  VFileSelect supports changing drives
// and directories as well as the filtering of file name listings.
//
CLASS VFileSelect : public VDialog {
public:

	// Styles can be OR'd (|) together.

	enum DlgStyle {		FileOpen		= 0x0001,
						SaveAs			= 0x0002,
						FileMustExist	= 0x0004,
						ConfirmReplace	= 0x0008 };

						VFileSelect(DlgStyle, const VString& title,
									VPathString& initial_path,
									VString *filter, boolean& success,
									int& filter_index, VWindow *parentwin);
						VFileSelect();
					   ~VFileSelect();
	VClass			   *iam();

	boolean				operator()(DlgStyle, const VString& title,
								   VPathString& path, VString *filter,
								   int& filter_index, VWindow *parentwin);

	static boolean		dialog(DlgStyle, const VString& title,
							   VPathString&, VWindow *parentwin,
							   VString *filter = 0, int *filter_index = 0);

	// Old/obsolete (v2.0).

						VFileSelect(boolean saveas, char *title, char *prompt,
									VString **filename,	VString *filter,
									VWindow *parentwin);
	static VString	   *dialog(VString *, VString *, char *,
							   VWindow *, char *...);
	static VString	   *dialog(boolean, VString *, char *,
							   VWindow *, char *...);
	static VString	   *dialog(VWindow *, char *, VString *, VString *);
	static VString	   *dialog(boolean, VWindow *, char *, VString *);
	static boolean		dialog(boolean, VPathString&, char *,
							   VWindow *, char *...);

	static void			setRememberInitialPath(boolean);
	static boolean		isRememberInitialPath();

private:
	void				processFilter(VString *filter);
private:
	VStream				wcFilter;
};

extern VClass *VFileSelectCls;

#endif // VFILESLCT_H

