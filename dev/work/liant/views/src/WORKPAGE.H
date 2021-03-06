// $Header:   Q:/views/common/vcs/workpage.h_v   1.9   May 30 1997 17:17:28   scottt  $ 

//	workpage.h
//
//	VWorkPage interface [Common]
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

#ifndef workpage_h
#define workpage_h
#include "object.h"
#include "str.h"
#include "window.h"

CLASS VWorkBook;
CLASS VBitMap;

extern VClass *VWorkPageCls;

CLASS VWorkPage : public VObject {
public:
	VWorkPage();
	VWorkPage(const VString& caption, 
		const VString& winname, VWindow *win, boolean vrfwin);
	~VWorkPage();

	VClass *iam();

	friend	VWorkBook;

#ifndef CV_NOARCHIVER
	void	getFrom(VArchiver& a);
	void	getFrom(VArchiver& a, VObject *parent, boolean do_realize);
	void	putTo(VArchiver& a);
#endif

private:
	VString		theCaption;
	VString		theWindowName;			// name of page in .vrf file
	VString		theBitMapName;			// name of tab bmap in .vrf file

	boolean		isEnabled;
	boolean		isVrfWindow;			// window loaded from .vrf file

	int			width;					// width of tab
	VWindow	   *theWindow;				// pointer to page window
	VBitMap	   *theBitMap;				// pointer to tab bitmap
};

#endif // workpage_h
