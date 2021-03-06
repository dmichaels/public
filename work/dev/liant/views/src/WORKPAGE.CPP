// $Header:   Q:/views/common/vcs/workpage.cpv   1.9   May 30 1997 17:17:26   scottt  $ 

//	workpage.cpp
//
//	VWorkPage implementation [Common]
//
//	Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	06/23/95 dgm	Original check-in; original code from kerstinb.
//	08/21/95 dgm	Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "workpage.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#endif

defineClass(VWorkPage, VObject)
defineArchiveRevision(VWorkPage, 0)

// --------------------------------------------------------------------------
VWorkPage::VWorkPage()
	:	isEnabled(TRUE)
	,	isVrfWindow(FALSE)
	,	theWindow(0)
	,	theBitMap(0)
	,	width(0)
{
	;
}

// --------------------------------------------------------------------------
VWorkPage::VWorkPage(const VString& caption, 
	const VString& winname, VWindow *win, boolean vrfwin)
	:	isEnabled(TRUE)
	,	theCaption(caption)
	,	theWindowName(winname)
	,	theWindow(win)
	,	isVrfWindow(vrfwin)
	,	theBitMap(0)
	,	width(0)
{
	;
}

// --------------------------------------------------------------------------
VWorkPage::~VWorkPage()
{
	// n.b. WorkBook is responsible for deleting window and bitmap
	;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
void VWorkPage::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VWorkPageArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VWorkPageArchiveTag));
		return;
	}

	a >> isEnabled;
	a >> isVrfWindow;

	a.getObject(&theCaption);
	a.getObject(&theWindowName);
	a.getObject(&theBitMapName);

	theWindow = 0;
}

// ---------------------------------------------------------------------------
void VWorkPage::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VWorkPage::getFrom(a);
}

// ---------------------------------------------------------------------------
void VWorkPage::putTo(VArchiver& a)
{
	a << VWorkPageArchiveTag;

	a << isEnabled;
	a << isVrfWindow;

	a.putObject(&theCaption);
	a.putObject(&theWindowName);
	a.putObject(&theBitMapName);
}

#endif // CV_NOARCHIVER
