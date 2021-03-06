// $Header:   Q:/views/win/vcs/listslct.h_v   1.6   Mar 04 1997 16:32:34   gregm  $ 

//	listslct.h
//
//	VListSelect interface
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
//	10/11/94 dss	Added operator().
// ----------------------------------------------------------------------------

// The `VListSelect` class is a modal dialog used to either view a list
// of strings, or to obtain a selection from a list of strings.

#include "dialog.h"

#ifndef listslct_h
#define listslct_h

CLASS VListBox;
CLASS VPushButton;
CLASS VButton;

CLASS VListSelect : public VDialog {
protected:
	int selection;
public:
	VClass *iam();

	static int dialog(boolean, char *, VString **, VWindow *);
	static int dialog(boolean, char *, VOrdCollect *, VWindow *);

	VListSelect();
	VListSelect(boolean, char *, VOrdCollect *, int *, VWindow *);
	~VListSelect();
	boolean dblclick(int);

	void operator()(boolean select, char *title,
				  VOrdCollect *lst, int *n, VWindow *pWin);

private:
    void centerWindow(VWindow*);


};

extern VClass *VListSelectCls;

#endif
