// $Header:   Q:/views/win/vcs/oipsite.cpv   1.14   Mar 25 1997 15:57:16   gregm  $

//	COleInPlaceSite
//
//  OLE IOleInPlaceSite Implementation
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
//	12/08/95 dss	canInPlaceActivate(): ask container, also.
//	01/09/96 dss	VComponent name change.
//  05/21/96 evc    removed cv_assert(lpFrameInfo->cb == 
//					sizeof(OLEINPLACEFRAMEINFO));
//  06/07/96 evc	Added call to
//					VComponentItem::uponInplaceObjectMoveOrResize()
//					in OleInPlaceSite::OnPosRectChange
//	---------------------------------------------------------------------------

#include "pre.h"

#define WIDTH(r) ((r).right - (r).left)
#define HEIGHT(r) ((r).bottom - (r).top)

//**********************************************************************
//
XOleInPlaceSite::XOleInPlaceSite(COleInPlaceSite *pOleInPlaceSite)
	: OleInPlaceSite( pOleInPlaceSite)
{
	CDBG(	<< "Constructing IOleInPlaceSite (" << this 
			<< ") for COleInPlaceSite (" << pOleInPlaceSite << ')' << endl);
}

//**********************************************************************
XOleInPlaceSite::~XOleInPlaceSite()
{
}

//**********************************************************************
//
void COleInPlaceSite::init(REFVCOMPONENTITEM refCDItem)
{
	this->lpCDItem = &refCDItem;
	lpOleInPlaceObject = 0;

	exposeInterface(IID_IOleInPlaceSite, "COleInPlaceSite");
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
		// This warning tells us that we are passing this in an initializer,
		// before "this" is through initializing.  This is ok, because
		// we just store the ptr in the other constructors
COleInPlaceSite::COleInPlaceSite(REFVCOMPONENTITEM refCDItem)
//	: Unknown(IID_IOleInPlaceSite, "COleInPlaceSite", refCDItem, this)
	: COleWindow(refCDItem, new XOleInPlaceSite(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In COleInPlaceSite's aggregate, sub-class constructor, this = "
			<< this << endl);

	init(refCDItem);
}

//**********************************************************************
//
COleInPlaceSite::COleInPlaceSite(REFVCOMPONENTITEM refCDItem, 
								 LPUNKNOWN lpIUnkBase)
//	: Unknown(IID_IOleInPlaceSite, "COleInPlaceSite", refCDItem, this)
	: COleWindow(refCDItem, lpIUnkBase)
{
	CDBG(	<< "In COleInPlaceSite's aggregate, super-class constructor, this = "
			<< this << endl);

	init(refCDItem);
}

//**********************************************************************
//
COleInPlaceSite::~COleInPlaceSite()
{
	CDBG(	<< "In COleInPlaceSite's destructor" << endl);
}

//**********************************************************************
//
// ***** IOleInPlaceSite Methods *****
//
//**********************************************************************

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::CanInPlaceActivate()
{
	CDBG(	<< "In COleInPlaceSite::CanInPlaceActivate = ");

	SCODE	sc;

	VWindow	*topLevelWindow = lpCDItem->getVThis()->getTopLevelParent();
	cv_assert(topLevelWindow);

	if ((topLevelWindow->getComponentThis() != 0) &&
// Start of Sanjay's Code
// Old Code
	//	topLevelWindow->getComponentThis()->isA(VComponentViewCls) &&
// End of old code
// End of Sanjay's code
		((VComponentView*)topLevelWindow->getComponentThis())->canLocallyActivate() &&
		lpCDItem->canLocallyActivate()) {
		sc = S_OK;
		CDBG(	<< "TRUE" << endl);
	}
	else {
		sc = S_FALSE;
		CDBG(	<< "FALSE" << endl);
	}

	return ResultFromScode(sc);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::OnInPlaceActivate()
{
	CDBG(	<< "In COleInPlaceSite::OnInPlaceActivate" << endl);

	// get the InPlaceObject interface if we don't have one.
	if (lpOleInPlaceObject == 0) {
		lpCDItem->getOleObject().QueryInterface(IID_IOleInPlaceObject,
			(LPVOID FAR*) &lpOleInPlaceObject);

		cv_assert(lpOleInPlaceObject);
		}
		
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::OnUIActivate()
{
	CDBG(	<< "In COleInPlaceSite::OnUIActivate" << endl);

	lpCDItem->setActivationState(VComponentItem::locallyActive);
	// Start of Sanjay's Code
	// Hide client site window
	if(!lpCDItem->isActiveXControl())
		lpCDItem->getVThis()->hide();
	// End of Sanjay's Code
     
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame,
                    LPOLEINPLACEUIWINDOW FAR* lplpDoc,
                    LPRECT lprcPosRect,
                    LPRECT lprcClipRect,
                    LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	CDBG(	<< "In COleInPlaceSite::GetWindowContext" << endl);

	cv_assert(lplpFrame);
	cv_assert(lplpDoc);
	cv_assert(lprcPosRect);
	cv_assert(lprcClipRect);
	cv_assert(lpFrameInfo);

	LPVCOMPONENTAPPVIEW lpCDAppView = 
		lpCDItem->getVThis()->getAppLevelParent()->getComponentThis();

	// Sanjay Start
	// Return if the object is not initialized
	if(!lpCDAppView)
		return ResultFromScode(E_UNEXPECTED);
	// Sanjay End
	cv_assert(lpCDAppView);

	// get the InPlaceFrame from the AppView
	*lplpFrame = lpCDAppView->getOleInPlaceFrame();

	// Always return an interface pointer with an incremented ref count
	(*lplpFrame)->AddRef();

#if 1
	LPVCOMPONENTWINDOW lpCDWindow = 
		lpCDItem->getVThis()->getTopLevelParent()->getComponentThis();
//		lpCDItem->getVThis()->getParent()->getComponentThis();

	cv_assert(lpCDWindow);
	cv_assert(lpCDWindow->getVThis()->isA(VViewCls));

	// we can cast to a sub class after verifying with isA().
	LPVCOMPONENTVIEW lpCDTopLevelView = (LPVCOMPONENTVIEW) lpCDWindow;

  	// if the top level view is the app view, then there's no document view, 
	// so return null.
	if (lpCDTopLevelView == lpCDAppView) {
		*lplpDoc = 0;
	}

	// else return the UI Window of the document view.
	else {
		*lplpDoc = lpCDTopLevelView->getOleInPlaceUIWindow();

		// Always return an interface pointer with an incremented ref count
		(*lplpDoc)->AddRef();
	}
#else
	LPVCOMPONENTWINDOW lpCDWindow = lpCDItem;

  	// if the top level view is the app view, then there's no document view, 
	/// so return null.
	if (lpCDItem->getComponentThis() == lpCDAppView->getComponentThis()) {
		*lplpDoc = 0;
	}

	// else return the UI Window of the document view.
	else {
		*lplpDoc = lpCDTopLevelView->getOleInPlaceUIWindow();

		// Always return an interface pointer with an incremented ref count
		(*lplpDoc)->AddRef();
	}
#endif

	#if 0
	// I'm taking this out because I've read nothing that 
	// says this is an issue  -- Ed C.

	// TO DO: take evasive action if lpFrameInfo->cb != 
	// sizeof(OLEINPLACEFRAMEINFO). See OLE documentation.
	cv_assert(lpFrameInfo->cb == sizeof(OLEINPLACEFRAMEINFO));
	#endif


	// TO DO: need to dynamically tell whether SDI or MDI app.
	lpFrameInfo->fMDIApp		= lpCDAppView->getVThis()->isA(VMdiAppViewCls);
	
	lpFrameInfo->hwndFrame		= lpCDAppView->getVThis()->hwnd();
	lpFrameInfo->haccel			= NULL;
	lpFrameInfo->cAccelEntries	= 0;

	// return the answer
	lpCDItem->bounds().getBounds(*lprcPosRect, VOlePoint::Pixels);
	// Sanjay Start
 	if(lpCDItem->isActiveXControl())
 	{
		lprcPosRect->right -= lprcPosRect->left;
		lprcPosRect->bottom -= lprcPosRect->top;
		lprcPosRect->top = 0;
		lprcPosRect->left = 0;
 	} 
 	// Sanjay End
#if 0
// this method executes BEFORE COleInPlaceUIWindow::SetBorder() so
// we cannot adjust for the border, here.
	LPCRECT	lpBorders;
	lpCDItem->getDocBase()->getOleInPlaceUIWindow().getBorderSpace(lpBorders);
	lprcPosRect->left += lpBorders->left;
	lprcPosRect->top += lpBorders->top;
#endif

	// TO DO: get rid of NATIVE!
//	GetClientRect(lpCDTopLevelView->getVThis()->hwnd(), lprcClipRect);
// Start of Sanjay's Code
	GetClientRect(lpCDItem->getVThis()->getTopLevelParent()->hwnd(), lprcClipRect);
// Old Code
	// Let the cditem clip its CD object
	//GetClientRect(lpCDItem->getVThis()->hwnd(), lprcClipRect);
// End of old code
// End of Sanjay's code

	CDBG(	<< "    item rect = " << *lprcPosRect << ", clip rect = "
			<< *lprcClipRect << endl);

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::Scroll(SIZE scrollExtent)
{
	CDBG(	<< "In COleInPlaceSite::Scroll" << endl);
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::OnUIDeactivate(BOOL fUndoable)
{
	CDBG(	<< "In COleInPlaceSite::OnUIDeactivate" << endl);

	VComponentAppView *lpCDApp = getVThis()->getAppLevelParent()->getComponentThis();
	HRESULT hResult = lpCDApp->adjustMdiClient(NULL);

	// TO DO: an optimization, to prevent menu bar flashing, etc.,
	// is to to handle the case where there's another item that's 
	// already UI active.

	// Although the ole object is still in place active, at this point,
	// the user interface is not. CDItem's state reflect the user
	// interface, so set the cditem state to inactive..
	// Sanjay Start
	DWORD miscStatus;
	hResult = lpCDItem->getOleObject().GetMiscStatus(DVASPECT_CONTENT, &miscStatus);
	if(SUCCEEDED(hResult))
	{
		if(!(miscStatus & OLEMISC_ACTIVATEWHENVISIBLE))
			lpCDItem->setActivationState(VComponentItem::inactive);
	}
	// Start old Code
	//lpCDItem->setActivationState(VComponentItem::inactive);
	// End Old Code
	// Sanjay End

	// Start of Sanjay's Code
	// Show client site window
	if(!lpCDItem->isActiveXControl())
		lpCDItem->getVThis()->show();
	// End of Sanjay's Code

	return ResultFromScode(hResult);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::OnInPlaceDeactivate()
{
	CDBG(	<< "In COleInPlaceSite::OnInPlaceDeactivate" << endl);

	cv_assert(lpOleInPlaceObject);
	lpOleInPlaceObject = 0;

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::DiscardUndoState()
{
	CDBG(	<< "In COleInPlaceSite::DiscardUndoState" << endl);
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::DeactivateAndUndo()
{
	CDBG(	<< "In COleInPlaceSite::DeactivateAndUndo" << endl);
	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::OnPosRectChange(LPCRECT lprcPosRect)
{
	CDBG(	<< "In COleInPlaceSite::OnPosRectChange, PosRect = "
			<< *lprcPosRect << endl);

	RECT	rcClipRect,
			rcPosRect;		// a read/write PosRect (lprcPosRect is const)
	HRESULT hResult = NOERROR;
	int		width = WIDTH(*lprcPosRect),
			height = HEIGHT(*lprcPosRect);
// Start of Sanjay's Code
	SIZEL sizel;
	sizel.cx = width;
	sizel.cy = height;
    lpCDItem->size(VOleSize(sizel,VOlePoint::Pixels));
    lpCDItem->move(VOlePosition(lprcPosRect->left, lprcPosRect->top,VOlePoint::Pixels));
// Start of Old Code
	//lpCDItem->bounds().setBounds(*lprcPosRect, VOlePoint::Pixels);
// End of old code
// End of Sanjay's Code
	// initialize a writable copy for adjustments.
	rcPosRect = *lprcPosRect;

// Start of Sanjay's Code
	GetClientRect(lpCDItem->getVThis()->getTopLevelParent()->hwnd(), &rcClipRect);
// Start of Old Code
	// TO DO: get rid of native!
	//GetClientRect(lpCDItem->getVThis()->hwnd(), &rcClipRect);
// End of old code
// End of Sanjay's Code

	// check the pos rect for out of bounds:
	// Note: server bug: some servers, notabely MS WinWord, doesn't clip
	// the IP rect when it is dragged about. Consequently. the IP can be
	// moved to a position where it is 100% clipped and the user can never
	// access it again (unless there is some sort non-mouse method like
	// casacade or tile.
	// the top left and bottom right 10x10 pixels must remain within view, 
	// so that the whole IP object doesn't get clipped by the view.

// Start of Sanjay's Code
// Commented out
/*
	if (rcPosRect.left > rcClipRect.right - 10) {
		rcPosRect.left = rcClipRect.right - 10;
		// preserve the original size.
		rcPosRect.right = rcPosRect.left + width;
	}
	
	if (rcPosRect.top > rcClipRect.bottom - 10) {
		rcPosRect.top = rcClipRect.bottom - 10;
		// preserve the original size.
		rcPosRect.bottom = rcPosRect.top + height;
	}

	if (rcPosRect.right < rcClipRect.left + 10) {
		rcPosRect.right = rcClipRect.left + 10;
		// preserve the original size.
		rcPosRect.left = rcPosRect.right - width;
	}

	if (rcPosRect.bottom < rcClipRect.top + 10) {
		rcPosRect.bottom = rcClipRect.top + 10;
		// preserve the original size.
		rcPosRect.top = rcPosRect.bottom - height;
	}
*/
// End of Sanjay's Code
#if 0
	if (rcPosRect.left < 0) {
		rcPosRect.left = 0;
	}

	if (rcPosRect.top < 0) {
		rcPosRect.top = 0;
	}
#endif

	getOleInPlaceObject().SetObjectRects(&rcPosRect, &rcClipRect);
	lpCDItem-> uponInplaceObjectMoveOrResize();

	return ResultFromScode(hResult);
}

// Start of Sanjay's Code
//**********************************************************************
//
STDMETHODIMP COleInPlaceSite::GetWindow(HWND FAR* lphwnd)
{
	SCODE		sc;

	if (lphwnd == 0) {
		sc = E_INVALIDARG;
	}
	else if (!lpCDItem->isActiveXControl() &&
			(*lphwnd = lpCDItem->getVThis()->getTopLevelParent()->hwnd())) {
		sc = S_OK;
	}
	else if (lpCDItem->isActiveXControl() && 
			(*lphwnd = lpCDItem->getVThis()->hwnd())) {
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
// End of Sanjay's Code

