// $Header:   Q:/views/win/vcs/iconbttn.cpv   1.8   Mar 04 1997 17:55:12   gregm  $ 

//	iconbttn.cpp
//
//	VIconButton implementation
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
//	05/13/93 pat	added constructor that takes name argument;
//					added getFrom(), putTo(), realize(); 
//					included archiver.h
//	05/20/93 jmd	changed realize to take a VWindow
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 dgm	pass final argument to getChildrenFrom().
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	09/22/93 pat 	added archive tag
//	09/23/93 pat 	added constructor that takes name argument
//  02/02/94 pat	added archive tag check
//  02/09/94 pat	updated defineArchiveTag use
//  03/04/94 pat	Win32 changes:  GetWindowLong().
//	03/26/94 pat	CV_WIN32: cast appropriately for SendMessage().
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#include "iconbttn.h"
#include "notifier.h"
#include "port.h"
#include "rect.h"
#include "shade.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#endif

defineClass(VIconButton, VPushButton)
defineArchiveRevision(VIconButton, 0)

/*@Method
Called when the visual state of this VIconButton 
changes. Upon call, the 'state' instance variable is set to one or more
of the following flags or\'ed together: 
BTTNdraw - draw the entire button; 
BTTNenabled - the button is enabled else disabled; 
BTTNselectDelta - the selection status of the button has changed; 
BTTNselect - ON if button is selected;
BTTNfocusDelta - the keyboard focus of this button has changed; and
BTTNfocus - ON if the button has focus.
*/
boolean VIconButton::paint()
{
	int w, h, wi, hi, offset;

	if (!(state & BTTNselectDelta) && !(state & BTTNdraw)) {
		return(FALSE);
	}
	VPort port(this);
	port.open();
	sizeOfImage(&w, &h);
	VRectangle iR(CornerDim, 0, 0, w, h);

	if (!(state & BTTNselect)) {
		bkShade.panel(Raised, iR, isdefault ? 2: 1, port);
		offset = 0;
	}
	else {
		bkShade.panel(Pressed, iR, isdefault ? 2: 1, port);
		offset = bkShade.getBevelThickness();
	}
	if (bIcon) {
		bIcon->sizeOfIcon(&wi, &hi);
		port.drawIcon(bIcon, (w - wi + offset) /2, (h - hi + offset) /2);
	}
	port.close();
	return(TRUE);
}

/*@Method
	Construct a VIconButton from the resource control-id 'cntrl' of
the resource associated with 'pWin'. This is a non-portable GUI system method.
*/
VIconButton::VIconButton(unsigned cntrl, VWindow *pWin) : bIcon(0)
{
	internal = FALSE;
	getRscWindow(cntrl, pWin);
}

/*@Method
	Construct a VIconButton button with no presentation space.
*/
VIconButton::VIconButton() : bIcon(0)
{
	internal = FALSE;
}

VIconButton::~VIconButton()
{
	if (bIcon && internal) {
		delete bIcon;
	}
}

/*@Method
	Construct an VIconButton object with VFrame 'frame', VIcon object
'ic', parent window 'pWin' and style 'style'.   By default the 'style'
parameter is set to StyleDefault.  Optionally, 'style' may be set to
StyleDefaultButton or  StyleNoAutoDefault. NOTE: the VIconButton does
NOT destroy the VIcon object 'ic' when it is destroyed.
*/
VIconButton::VIconButton(const VFrame& frame, VIcon *ic, VWindow *pWin, 
						 const VStyle& style)
{
	bIcon = ic;
	internal = FALSE;
	(*this)(frame, pWin, style);
}



/*@Method
	If 'b' is TRUE, then set this VIconButton to the default
push button style, otherwise set it to the standard style. 
*/
void VIconButton::setDefault(boolean b)
{
	if (!isdefault || !b) {
		isdefault = b;
		if (b && fromRsc) {
#ifdef CV_WIN32
			SendMessage(GetParent(hWnd), DM_SETDEFID, 
						(WPARAM) GetWindowLong(hWnd, GWL_ID), (LPARAM) 0);
#else
			SendMessage(GetParent(hWnd), DM_SETDEFID, 
						(WPARAM) GetWindowWord(hWnd, GWW_ID), (LPARAM) 0);
#endif
		}
		update();
	}
}


/* 
	Called by the VNotifier to obtain info on the underlying GUI system
window represented by this VIconButton object.
*/
void VIconButton::getInfo(VWinInfo &iconinfo)
{
	VButton::getInfo(iconinfo);
	if (style.contains(StyleDefaultButton))  {
		isdefault = TRUE;
	}
}

//	--------------------------------------------------------------------------
//	Construct an VIconButton object with VFrame frame, parent window
//	pWin and style style.  The style parameter is optional and it may be
//	set to StyleDefaultButton or StyleNoAutoDefault.  The VIcon object for
//	this button can be provided via the setIcon(VIcon&) method.
//	
VIconButton::VIconButton(const VFrame& frame, VWindow *pWin, const VStyle& style) 
			:	bIcon(0)
{
	internal = FALSE;
	(*this)(frame, pWin, style);
}

//	--------------------------------------------------------------------------
//	Construct an VIconButton object with VFrame frame, parent window
//	pWin and style style.  The style parameter is optional and it may be
//	set to StyleDefaultButton or StyleNoAutoDefault.  The VIcon object for
//	this button can be provided via the setIcon(VIcon&) method.
//	
VIconButton::VIconButton(const VString& wName, const VFrame& frame, 
							VWindow *pWin, const VStyle& style) 
			:	bIcon(0)
{
	internal = FALSE;
	(*this)(frame, pWin, style, wName);
}

//	--------------------------------------------------------------------------

/*@Method
	Construct the VIcon object that is to be used by this VIconButton
from the resource name 'iconName'.  NOTE: When the VIconButton is destroyed,
it will destroy the VIcon object created from the passed resource name.
*/
void VIconButton::setIcon(char *iconName)
{
	if (bIcon && internal) {
		delete bIcon;
	}
	bIcon = new VIcon(iconName);
	internal = TRUE;
}


/*@Method
	Set the VIcon object that is to be used by this VIconButton 
to 'ic'.  NOTE: the VIconButton does NOT destroy the VIcon object 'ic'
when it is destroyed.
*/
void VIconButton::setIcon(VIcon *ic)
{
	if (bIcon && internal) {
		delete bIcon;
	}
	bIcon = ic;
	internal = FALSE;
}

/*@Method
	Construct an VIconButton object with VFrame 'frame', icon resource
name 'iconName', parent window 'pWin' and style 'style'.  By default the
'style' parameter is set to StyleDefault.  Optionally, 'style' may be set
to StyleDefaultButton or  StyleNoAutoDefault. NOTE: the VIconButton
will destroy the VIcon object created from the resource name when it is
destroyed.
*/
VIconButton::VIconButton(const VFrame& frame, char *iconName, VWindow *pWin, 
						 const VStyle& style)
{
	if (iconName) {
		bIcon = new VIcon(iconName);
		internal = TRUE;
	}
	else {
		bIcon = 0;
		internal = FALSE;
	}
	(*this)(frame, pWin, style);
}

// ----------------------------------------------------------------------------
//
void VIconButton::systemColorChanged()
{
	bkShade.setDefaultColors();
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// --------------------------------------------------------------------------
// VIconButton::putTo
//
void VIconButton::putTo(VArchiver& a)
{
	VIconButton::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
// VIconButton::putTo
//
void VIconButton::putTo(VArchiver& a, boolean most_derived)
{
	VPushButton::putTo(a, FALSE);  					//	base object data

	a << VIconButtonArchiveTag;

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
// VIconButton::getFrom
//
void VIconButton::getFrom(VArchiver& a)
{
	VPushButton::getFrom(a);	   						//	base object data
	
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VIconButtonArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VIconButtonArchiveTag));
		return;
	}
}

// --------------------------------------------------------------------------
// VIconButton::getFrom
//
void VIconButton::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VIconButton::getFrom(a);				

	if (do_realize) {
		VIconButton::realize((VWindow *)data);
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// --------------------------------------------------------------------------
// VIconButton::realize
//
void VIconButton::realize(VWindow *pwin)
{
	VPushButton::realize(pwin);
}

// --------------------------------------------------------------------------

#endif //	ARCHIVER	
// ===========================================================================


