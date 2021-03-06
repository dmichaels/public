// $Header:   Q:/views/win/vcs/yesno.h_v   1.5   Mar 04 1997 16:33:20   gregm  $ 

//	yesno.h
//
//	VYesNo interface
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
//	07/20/93 jmd	preened, added const VString
//	02/01/94 pat	changed ifndef ARCHIVER to indef ARCHIVER
//	10/11/94 dss	Added operator().
// ----------------------------------------------------------------------------

#ifndef VYESNO_H
#define VYESNO_H
#include "dialog.h"

enum yesNoCancel { CancelButton, YesButton, NoButton };
extern VClass *VYesNoCls;

//
// The `VYesNo` class is a modal dialog that presents a question
// to the user and solicits a YES, NO, or possibly a CANCEL response.
//

CLASS VYesNo : public VDialog {
public:

			VYesNo();
			VYesNo(int which, boolean cancel, enum yesNoCancel *opt,
				   const char *txt, const char *title, VWindow *win);

	VClass *iam();

  	void operator()(int which, boolean cancel, enum yesNoCancel *opt,
		   const char *txt, const char *title, VWindow *win);

	static enum yesNoCancel dialog(VWindow *win, boolean cancel,
								enum yesNoCancel dflt, const char *prompt...);
	static enum yesNoCancel warning(boolean cancel, enum yesNoCancel dflt,
								 const char *title, VWindow *win,
								 const char *prompt...);
	static enum yesNoCancel action(boolean cancel, enum yesNoCancel dflt,
								const char *title, VWindow *win,
								const char *prompt...);
	static enum yesNoCancel question(boolean cancel, enum yesNoCancel dflt,
								  const char *title, VWindow *win,
								  const char *prompt...);

};

#endif // VYESNO_H
