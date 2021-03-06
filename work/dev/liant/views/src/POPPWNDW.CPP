// $Header:   Q:/views/win/vcs/poppwndw.cpv   1.13   Mar 04 1997 17:55:34   gregm  $ 

//	poppwndw.cpp
//
//	VPopupWindow implementation
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
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	07/27/93 pat	removed free()
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name VString const
//	09/22/93 pat	added archive tag
//	02/02/94 pat	added archive tag check
//	02/09/94 pat	updated defineArchiveTag use
//	08/31/94 dss	Ctl3d non-dialog support.
//	11/11/94 dss	Archiver tags macros.
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

#include "winclass.h"
#include "poppwndw.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VPopupWindow, VView)
defineArchiveRevision(VPopupWindow, 0)

// ---------------------------------------------------------------------------
//	Construct a VPopupWindow with no presentation window.
//
VPopupWindow::VPopupWindow() 
{  	
	;
}

// ---------------------------------------------------------------------------
//
VPopupWindow::~VPopupWindow() 
{  	
	;
}

// ---------------------------------------------------------------------------
//	Construct a VPopupWindow with frame 'frame', parent 'win' and 
//	style 'style'.  The 'style' can be one or more of: StyleCloseBox, 
//	StyleSizable, StyleTitle, StyleHorizontal, or StyleVertical.
//
VPopupWindow::VPopupWindow(const VFrame& frame, VWindow *win, 
								const VStyle&  style) 
{
	(*this)(frame, win, style);
}

// ---------------------------------------------------------------------------
//	Construct a VPopupWindow with frame 'frame', parent 'win' and 
//	style 'style'.  The 'style' can be one or more of: StyleCloseBox, 
//	StyleSizable, StyleTitle, StyleHorizontal, or StyleVertical.
//
VPopupWindow::VPopupWindow(const VString& wname, const VFrame& frame, 
								VWindow *win, const VStyle& style) 
{
	(*this)(frame, win, style, wname);
}

// ---------------------------------------------------------------------------
//	Add the width and height of a standard VPopupWindow border to
//	'*w' and '*h' respectively.
//	
void VPopupWindow::stdSize(const VStyle& style, int *w, int *h)
{
	VView::stdSize(style, w, h);
}

// ---------------------------------------------------------------------------
//
void VPopupWindow::getInfo(VWinInfo &popinfo)
{
	VView::getInfo(popinfo);
	popinfo.style |= WS_POPUP;
	popinfo.style &= (~WS_CHILD);
}

// ---------------------------------------------------------------------------
//	Return the position and size of this VPopupWindow in '*x', '*y',
//	'*w' and '*h' respectively.  The position is relative to upper left
//	hand corner of screen and not the upper left hand corner of the
//	parent window. 
void VPopupWindow::getRel(int *x, int *y, int *w, int *h)
{
	getAbs(x, y, w, h);
}

//--------------------------------------------------------------------------
//
boolean VPopupWindow::isTopLevel() const
{
	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VPopupWindow::getSystemBorderSize(int *width, int *height) const
{
	VView::getSystemBorderSize(width, height);
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
//	Put this object to the archive.  This method is called from 
//	VArchiver::putObject().
//
void VPopupWindow::putTo(VArchiver& a)
{
	VPopupWindow::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//	Put this object to the archive by archiving the base class layer, 
//	then the object data of this class layer.
//
//	If 'most_derived' is FALSE, then this method has been called from a derived
//	class.  That is, this class is a base to some derived class and we should
//	not archive our children, if any.
//
//	If 'most_derived' is TRUE, then this method is being called directly from a 
//	VArchiver.  That is, this class is the most derived class layer for this 
//	object and we archive our children, if any.
//
void VPopupWindow::putTo(VArchiver& a, boolean most_derived)
{
	VView::putTo(a, FALSE); 			//	base object data

	a << VPopupWindowArchiveTag;

	//	(no object data specific to this class)

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
//	Get this object from the archive by retrieving the object 
//	data of the base class and then the object data of this class.
//
//	This method is invoked by the getFrom() of this most-derived class or by
//	the getFrom() of a derived class.
//
void VPopupWindow::getFrom(VArchiver& a)
{
	VView::getFrom(a);		 			//	base object data
	
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VPopupWindowArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VPopupWindowArchiveTag));
		return;
	}

	//	(no object data specific to this class)
}

// --------------------------------------------------------------------------
//	Get this object from the archive.  The 'data' parameter is a VWindow *
//	that is our parent window.  In response, retrieve our object data,
//	realize the presentation space under the parent, and finally, retrieve
//	our children.
//
//	This method is invoked by VArchiver::getObject().
//
void VPopupWindow::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VPopupWindow::getFrom(a);

	if (do_realize) {
		VPopupWindow::realize((VWindow *)data);
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// --------------------------------------------------------------------------
//	Realize the presentation space of this object.  Upon entry, the
//	Views object exists, but no GUI object.  First, the realize() method of 
//	our base class is invoked to build the GUI object, then we set any 
//	additional object data that affects the GUI.
//
//	This method is invoked by the getFrom() method of this most-derived 
//	class or by the getFrom() of a derived class.
//
void VPopupWindow::realize(VWindow *pwin)
{
	VView::realize(pwin);
}

// --------------------------------------------------------------------------

#endif //	CV_NOARCHIVER	
// ===========================================================================


