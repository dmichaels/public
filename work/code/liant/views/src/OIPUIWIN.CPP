// $Header:   Q:/views/win/vcs/oipuiwin.cpv   1.6   Jun 03 1997 14:15:20   scottt  $

//	COleInPlaceUIWindow
//
//  OLE IOleInPlaceUIWindow Implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//	12/28/94 dss	Initial release.
//	12/04/95 dss	Added PVCS header. Migrated to LPCOLESTR.
//					Removed lpInPlaceActiveObjectName.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XOleInPlaceUIWindow::XOleInPlaceUIWindow(COleInPlaceUIWindow *pOleInPlaceUIWindow)
	: OleInPlaceUIWindow( pOleInPlaceUIWindow)
{
	CDBG(	<< "Constructing IOleInPlaceUIWindow (" << this 
			<< ") for COleInPlaceUIWindow (" << pOleInPlaceUIWindow << ')'
			<< endl);
}

//**********************************************************************
XOleInPlaceUIWindow::~XOleInPlaceUIWindow()
{
}

//**********************************************************************
//
void COleInPlaceUIWindow::init(REFVCOMPONENTVIEW refCDView)
{
	this->lpCDView = &refCDView;
	lpInPlaceActiveObject = 0;

	exposeInterface(IID_IOleInPlaceUIWindow, "COleInPlaceUIWindow");
}

//**********************************************************************
//
//	refView - the VView of the Outer CUnknown
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
COleInPlaceUIWindow::COleInPlaceUIWindow(REFVCOMPONENTVIEW refCDView)
	: COleWindow(refCDView, new XOleInPlaceUIWindow(this) )
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In COleInPlaceUIWindow's aggregate, sub-class constructor, this = "
			<< this << endl);

	init(refCDView);
}

//**********************************************************************
//
//	refView - the VView of the Outer CUnknown
//	refUnkBase - the Base Class IUnknown that this object is a super-class of.
//
COleInPlaceUIWindow::COleInPlaceUIWindow(REFVCOMPONENTVIEW refCDView, 
										 LPUNKNOWN lpIUnkBase)
	: COleWindow(refCDView, lpIUnkBase)
{
	CDBG(	<< "In COleInPlaceUIWindow's aggregate, super-class constructor, this = "
			<< this << endl);

	init(refCDView);
}

//**********************************************************************
//
COleInPlaceUIWindow::~COleInPlaceUIWindow()
{
	CDBG(	<< "In COleInPlaceUIWindow's destructor" << endl);
}

//**********************************************************************
//
// ***** IOleInPlaceUIWindow Methods *****
//
//**********************************************************************

//**********************************************************************
//
STDMETHODIMP COleInPlaceUIWindow::GetBorder(LPRECT lprectBorder)
{
//	SCODE sc = INPLACE_E_NOTOOLSPACE;
	SCODE sc = S_OK;

	int	x, y, w, h;

	cv_assert(lprectBorder);
	VWindow	*win = lpCDView->getVThis();
	win->getClientRel(&x, &y, &w, &h);
	
	lprectBorder->top = y;
	lprectBorder->left = x;
	lprectBorder->bottom = y + h;
	lprectBorder->right = x + w;
          
    // account for the status bar, if there's one
    // To DO: should this be part of border negotiation? It seems that
    // GetBorder is not called on resize, but request/set border is.
	if (win->isA(VStdMdiAppViewCls)) {
		VStatusBar *statusBar = ((VStdMdiAppView*)win)->getStatusBar();
		if (statusBar && !statusBar->isHidden()) {
			statusBar->getClientRel(&x, &y, &w, &h);
			lprectBorder->bottom -= h;
		}
	}
	
	CDBG(	<< "In " << getClassName() << "::GetBorder, border = "
			<< *lprectBorder << endl);

	return ResultFromScode(sc);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceUIWindow::RequestBorderSpace(LPCBORDERWIDTHS lpborderwidths)
{
//	SCODE sc = INPLACE_E_NOTOOLSPACE;
	SCODE sc = S_OK;
	                          
	CDBG(	<< "In " << getClassName() << "::RequestBorderSpace, "
			   "border widths = " << *lpborderwidths << endl);

	return ResultFromScode(sc);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceUIWindow::SetBorderSpace(LPCBORDERWIDTHS lpBorderWidths)
{
	SCODE sc = S_OK;

	CDBG(	<< "In " << getClassName() 
			<< "::SetBorderSpace, border widths = "); // NO endl!
             
    if (lpBorderWidths) {
		CDBG(  << *lpBorderWidths << endl);
	} else {
		CDBG(  << "<null>" << endl);
	}

	setBorderSpace(lpBorderWidths);
	
	return ResultFromScode(sc);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceUIWindow::SetActiveObject(
					LPOLEINPLACEACTIVEOBJECT lpInPlaceActiveObject,
					LPCOLESTR lpszInPlaceObjName)
{
	CDBG(	<< "In " << getClassName() << "::SetActiveObject" << endl);

	// release the old one
	if (this->lpInPlaceActiveObject) {
		this->lpInPlaceActiveObject->Release();
	}

	// add ref the new one
	if (this->lpInPlaceActiveObject = lpInPlaceActiveObject) {
		this->lpInPlaceActiveObject->AddRef();
	}

	this->lpInPlaceActiveObjectName = lpszInPlaceObjName;

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// ***** COleInPlaceUIWindow methods *****
//
//**********************************************************************

//**********************************************************************
//
boolean COleInPlaceUIWindow::resized(int w, int h)
{
	LPOLEINPLACEACTIVEOBJECT pOleInPlaceActiveObject;

	if (pOleInPlaceActiveObject = getOleInPlaceActiveObject()) {
		RECT	rectBorder = {0, 0, w, h};
         
		CDBG(	<< "In " << getClassName() << "::resized, rect = "
				<< rectBorder << endl);

		HRESULT hResult = pOleInPlaceActiveObject->ResizeBorder(&rectBorder,
									  *this,
									  getComponentThis().getVThis()->isA(VAppViewCls));
		cv_assert(SUCCEEDED(hResult));
	}

	return TRUE;
}

//**********************************************************************
//
VView *COleInPlaceUIWindow::getVThis()
{
	return getComponentThis().getVThis();
}

