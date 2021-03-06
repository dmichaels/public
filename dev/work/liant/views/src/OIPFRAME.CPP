// $Header:   Q:/views/win/vcs/oipframe.cpv   1.7   Jun 03 1997 14:15:18   scottt  $

//	COleInPlaceFrame
//
//  OLE IOleInPlaceFrame Implementation
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
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XOleInPlaceFrame::XOleInPlaceFrame(COleInPlaceFrame *pOleInPlaceFrame)
	: OleInPlaceFrame( pOleInPlaceFrame)
{
	CDBG(	<< "Constructing IOleInPlaceFrame (" << this 
			<< ") for COleInPlaceFrame (" << pOleInPlaceFrame << ')' << endl);
}

//**********************************************************************
XOleInPlaceFrame::~XOleInPlaceFrame()
{
}

//**********************************************************************
//
void COleInPlaceFrame::init(REFVCOMPONENTAPPVIEW refCDAppView)
{
	this->lpCDAppView = &refCDAppView;

	menu0 = 0;
	menu2 = 0;
   	menu4 = 0;

	exposeInterface(IID_IOleInPlaceFrame, "COleInPlaceFrame");
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
		// This warning tells us that we are passing this in an initializer,
		// before "this" is through initializing.  This is ok, because
		// we just store the ptr in the other constructors
COleInPlaceFrame::COleInPlaceFrame(REFVCOMPONENTAPPVIEW refCDAppView)
//	: Unknown(IID_IOleInPlaceFrame, "COleInPlaceFrame", refCDAppView, this)
//	, OleWindow(refCDAppView)
	: COleInPlaceUIWindow(refCDAppView, new XOleInPlaceFrame(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In COleInPlaceFrame's aggregate, sub-class constructor, this =" 
			<< this << endl);

	init(refCDAppView);
}

//**********************************************************************
//
COleInPlaceFrame::~COleInPlaceFrame()
{
	CDBG(	<< "In COleInPlaceFrame's destructor" << endl);
}

//**********************************************************************
//
// ***** IOleInPlaceFrame Methods *****
//
//**********************************************************************

//**********************************************************************
//
STDMETHODIMP COleInPlaceFrame::InsertMenus(HMENU hmenuShared, 
										   LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	CDBG(	<< "In COleInPlaceFrame::InsertMenus" << endl);

	// its OK not to have menus, so return OK
	boolean bSuccess = TRUE;

    lpMenuWidths->width[0] = 0;
    lpMenuWidths->width[2] = 0;
    lpMenuWidths->width[4] = 0;

	// File Menu
	menu0 = getVThis()->getMenu()->menuAt(0);

	if (menu0) {
		bSuccess &= AppendMenu(hmenuShared, MF_POPUP, 
							(UINT)menu0->hmenu(),
							menu0->getLabel());

	    lpMenuWidths->width[0] = 1;
    
	}

	return ResultFromScode(bSuccess ? S_OK : E_UNEXPECTED);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceFrame::SetMenu(HMENU hmenuShared, HOLEMENU holemenu, 
									   HWND hwndActiveObject)
{
	CDBG(	<< "In COleInPlaceFrame::SetMenu" << endl);
	boolean		bSuccess = TRUE;

	if (hmenuShared) {

		// Views version of setMenuBar copies vpopups.
		// This requires that we manage the menu bar and delete it 
		// when necessary.
		// We want the entire menu bar set at once.
//		getComponentThis()->setMenuBar(hmenuShared);

		// save the current menu bar
//		getComponentThis().saveMenuBar();

		getComponentThis().setMenuBar(hmenuShared);
	}
	else {
// rhw 4/11/96: Commented this out to get rid of assert.  I am not sure why
// he was checking this but in the Allegris Constructor, the layout mode of
// the painter does not have a menu.  As a result, "holemenu" was NULL but
// it did not appear to cause any problems in the painter when I ran the
// Allegris Constructor.
//		cv_assert(holemenu == 0);
		
		// restore the old menu bar
		getComponentThis().restoreMenuBar();

		// TO DO: MEMORY LEAK! the VMenu created from hmenuShared has not
		// been deleted. We can't delete it because the delete will
		// destroy the native menu, which ole manages. Must find a way
		// to delete the views object without destroying the
		// native elements.
	}

	bSuccess &= (S_OK == OleSetMenuDescriptor(holemenu,
											  getVThis()->hwnd(),
											  hwndActiveObject,
											  NULL,
											  NULL));

	return ResultFromScode(bSuccess ? S_OK : E_UNEXPECTED);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceFrame::RemoveMenus(HMENU hmenuShared)
{
	CDBG(	<< "In COleInPlaceFrame::RemoveMenus" << endl);
	
	// its OK not to have menus, so return OK
	boolean bSuccess = TRUE;

	if (menu0) {
		bSuccess &= removeMenu(hmenuShared, menu0);
		menu0 = 0;
	}
	
	if (menu2) {
		bSuccess &= removeMenu(hmenuShared, menu2);
		menu2 = 0;
	}

	if (menu4) {
		bSuccess &= removeMenu(hmenuShared, menu4);
		menu4 = 0;
	}

	bSuccess &= (S_OK == OleSetMenuDescriptor(NULL,
											  getVThis()->hwnd(),
											  NULL,
											  NULL,
											  NULL));

	return ResultFromScode(bSuccess ? S_OK : E_UNEXPECTED);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceFrame::SetStatusText(LPCOLESTR lpszStatusText)
{
	CDBG(	<< "In COleInPlaceFrame::SetStatusText" << endl);

	if (getVThis()->isA(VStdMdiAppViewCls) && lpszStatusText &&
		*lpszStatusText) {
		VStatusBar *statusBar = ((VStdMdiAppView*)getVThis())->getStatusBar();
		if (statusBar) {
			statusBar->putText(lpszStatusText);
		}
	}

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceFrame::EnableModeless(BOOL bEnable)
{
	CDBG(	<< "In COleInPlaceFrame::EnableModeless" << endl);

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP COleInPlaceFrame::TranslateAccelerator(LPMSG lpmsg, WORD wID)
{
	CDBG(	<< "In COleInPlaceFrame::TranslateAccelerator" << endl);

	// return that the keystroke was not used
	return ResultFromScode(S_FALSE);
}

//**********************************************************************
//
//		***** SUPPORT Functions *****
//
//**********************************************************************

//**********************************************************************
//
boolean COleInPlaceFrame::removeMenu( HMENU hMenu, VMenu *menu)
{
	int		count = GetMenuItemCount(hMenu);
	int		i;

	for (i = 0; i < count; i++) {
		if (menu->hmenu() == GetSubMenu(hMenu, i)) {
			RemoveMenu(hMenu, i, MF_BYPOSITION);
			break;
		}
	}

	// return TRUE if menu removed, else FALSE
	return count != i;
}

//**********************************************************************
//
VAppView *COleInPlaceFrame::getVThis()
{
	return getComponentThis().getVThis();
}

