// $Header:   Q:/views/win/vcs/report.h_v   1.5   Mar 04 1997 16:33:00   gregm  $ 

//	report.h
//
//	VReport interface
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
//	07/20/93 jmd	preened, added warning
//	07/25/93 jmd	added const
//	10/11/94 dss	Added operator().
// ----------------------------------------------------------------------------

#ifndef VREPORT_H
#define VREPORT_H
#include "dialog.h"

//
// The `VReport` class is a modal dialog that is used to simply pass
// informational statements to the user.  The user acknowledges the
// statement and terminates the dialog by pressing the ENTER key or
// by clicking on the OK button.
//

extern VClass  *VReportCls;

CLASS VReport : public VDialog {
public:
			VReport();
			VReport(int type, const char *title, VWindow *win, const char *prompt);
			~VReport();

			// v1.x
			VReport(char *txt, VWindow *win);
			VReport(char *txt, char *title, VWindow *win);

	VClass *iam();

 	void operator()(int type, const char *title, VWindow *win, const char *prompt);

	static void dialog(VWindow *win, const char *prompt...);
	static void dialog(const char *title, VWindow *win, const char *prompt...);
	static void warning(const char *title, VWindow *win, const char *prompt...);

};

#endif // VREPORT_H
