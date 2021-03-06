// $Header:   Q:/views/win/vcs/owindow.cpv   1.7   Mar 21 1997 08:43:04   gregm  $

//	COleWindow
//
//  OLE IOleWindow Implementation
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
//	12/28/94 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XOleWindow::XOleWindow(COleWindow *pOleWindow)
	: OleWindow( pOleWindow)
{
}

//**********************************************************************
XOleWindow::~XOleWindow()
{
}

//**********************************************************************
//
void COleWindow::init(REFVCOMPONENTWINDOW refCDWindow)
{
	this->lpCDWindow = &refCDWindow;

	exposeInterface(IID_IOleWindow, "COleWindow");
}

//**********************************************************************
//
//	refUnkWindow - the Outer VWindow CUnknown
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
COleWindow::COleWindow(REFVCOMPONENTWINDOW refCDWindow)
	: CUnknown(refCDWindow, new XOleWindow(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In COleWindow's aggregate, sub-class constructor, this = "
			<< this << endl);

	init(refCDWindow);
}

//**********************************************************************
//
//	refUnkWindow - the Outer VWindow CUnknown
//	refUnkBase - the Base Class IUnknown that this object is a super-class of.
//
COleWindow::COleWindow(REFVCOMPONENTWINDOW refCDWindow, LPUNKNOWN lpIUnkBase)
	: CUnknown(refCDWindow, lpIUnkBase)
{
	CDBG(	<< "In COleWindow's aggregate, super-class constructor, this = "
			<< this << endl);

	init(refCDWindow);
}

//**********************************************************************
//
COleWindow::~COleWindow()
{
	CDBG(	<< "In COleWindow's destructor" << endl);
}

//**********************************************************************
//
STDMETHODIMP COleWindow::GetWindow(HWND FAR* lphwnd)
{
	SCODE		sc;

	if (lphwnd == 0) {
		sc = E_INVALIDARG;
	}
	// Start of Sanjay's Code
	//else if (*lphwnd = getVThis()->getTopLevelParent()->hwnd()) {
	// Start of Old Code
	else if (*lphwnd = getVThis()->hwnd()) {
	// End of Old Code
	// End of Sanjay's Code
		sc = S_OK;
	}
	else {
		// Don't have to null *lpwnd; it will be NULL by hwnd(), if we 
		// get here
		sc = E_FAIL;
	}

	CDBG(	<< "In " << getClassName() << "::GetWindow, hwnd = " 
			<< hex << int(*lphwnd) << endl);

	return ResultFromScode(sc);
}

//**********************************************************************
//
STDMETHODIMP COleWindow::ContextSensitiveHelp(BOOL fEnterMode)
{
	CDBG(	<< "In " << getClassName() << "::ContextSensitiveHelp" << endl);

	// we are ignoring context sensitive help: return OK
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
VWindow *COleWindow::getVThis()
{
	return getComponentThis().getVThis();
}

