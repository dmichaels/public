// $Header:   Q:/views/win/vcs/poppwndw.h_v   1.14   Mar 04 1997 16:32:48   gregm  $ 

//	poppwndw.h
//
//	VPopupWindow interface
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
//	07/15/93 pat	added support for VArchiver; preened.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name VString const
//	08/12/94 dss	Ctl3d.
//	08/31/94 dss	Removed inline hasCtl3dEffects() body. Added isTopLevel().
//	08/23/95 dss	Removed hasCtl3dEffects() as it is inherited from VWindow.
//	09/01/95 dss	VStyle changes.
//	09/29/95 dgm	Added constrainMinimumSize() and resizeClientArea().
//	10/18/95 dgm	Removed constrainMinimumSize() and resizeClientArea().
//					Added getNonClientAreaSize().
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	01/29/96 dgm	Changed isTopLevel() to be a const function.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VPOPUPWINDOW_H
#define VPOPUPWINDOW_H

#include "view.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VPopupWindowCls;

//	Introduction
//
//	The VPopupWindow class is a subclass of VView which, when
//	displayed, appears in front of and on top of all other windows in an
//	application.  VPopupWindow objects must have a parent window, but 
//	they are not clipped by their parent window. 
 
CLASS VPopupWindow : public VView {

public:
				VPopupWindow();
				VPopupWindow( const VString& wname,
								const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = StyleDefault);
				VPopupWindow( const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = StyleDefault);
				~VPopupWindow();

	VClass 	   *iam();

	static void stdSize(const VStyle& style, int *w, int *h);
	void 		getRel(int *x, int *y, int *w, int *h);

	// Window Attributes
 	virtual boolean	isTopLevel() const;

	// Geometry management.

    virtual void	getSystemBorderSize(int *, int *) const;

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	void 		getInfo(VWinInfo& popinfo);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

};

#endif	//	VPOPUPWINDOW_H
