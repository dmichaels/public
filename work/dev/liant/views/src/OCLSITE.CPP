// $Header:   Q:/views/win/vcs/oclsite.cpv   1.8   Mar 21 1997 08:42:50   gregm  $

//	COleClientSite
//
//  OLE IOleClientSite Implementation
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
//  05/21/96 evc    re-enabled saveObject using m_lpStorage via
//					VComponentItem::getStg()
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XOleClientSite::XOleClientSite(COleClientSite *pOleClientSite)
	: OleClientSite( pOleClientSite)
{
	CDBG(	<< "Constructing IOleClientSite (" << this 
			<< ") for COleClientSite (" << pOleClientSite << ')' << endl);
}

//**********************************************************************
XOleClientSite::~XOleClientSite()
{
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
COleClientSite::COleClientSite(REFVCOMPONENTITEM refCDItem)
	: CUnknown(refCDItem, new XOleClientSite(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In COleClientSite's aggregate constructor, this = " << this
			<< endl);
	this->lpCDItem = &refCDItem;

	exposeInterface(IID_IOleClientSite, "COleClientSite");
}

//**********************************************************************
//
COleClientSite::~COleClientSite()
{
	CDBG(	<< "In COleClientSite's destructor" << endl);
}

//**********************************************************************
//
// COleClientSite::SaveObject
//
// Purpose:
//
//      Called by the object when it wants to be saved to persistant
//      storage
//
// Parameters:
//
//      None
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      IOleObject::QueryInterface          Object
//      IPersistStorage::SaveCompleted      Object
//      IPersistStorage::Release            Object
//      OleSave                             OLE API
//      ResultFromScode                     OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP COleClientSite::SaveObject()
{
	LPPERSISTSTORAGE lpPS;
	SCODE sc = E_FAIL;

	CDBG(	<< "In COleClientSite::SaveObject" << endl);

	// get a pointer to IPersistStorage
	HRESULT hErr = lpCDItem->getOleObject().QueryInterface(IID_IPersistStorage, (LPVOID FAR *)&lpPS);

	// save the object
	if (hErr == NOERROR)
		{
		sc = GetScode( OleSave(lpPS, 
		//lpCDItem->getStorage(), 
		 lpCDItem->getStg(), TRUE) );

		lpPS->SaveCompleted(NULL);
		lpPS->Release();
		}

	return ResultFromScode(sc);
}

//**********************************************************************
//
// COleClientSite::GetMoniker
//
//********************************************************************

STDMETHODIMP COleClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker,
										LPMONIKER FAR* ppmk)
{
	CDBG(	<< "In COleClientSite::GetMoniker" << endl);
//	SCODE		sc;

	// 1st getStorage isA VComponentStorage, 2nd isA CRStorage().
	// Monikers are only known in CRStorage, not VComponentStorage
// rhw 4/15/96: I commented all the rest of this code out since the final storage
// implementation is not here yet.  When my temporary hack is removed, uncomment
// and test.
//	*ppmk = lpCDItem->getStorage().getStorage().getMoniker(dwAssign, dwWhichMoniker);

//	if (*ppmk) {
//		(*ppmk)->AddRef();
//		sc = S_OK;
//	}
//	else  {
//		sc = E_FAIL;
//	}

// rhw 4/15/96: Just return fail until storage implementation is ready.
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
// COleClientSite::GetContainer
//
// Purpose:
//
//      Not Implemented
//
//********************************************************************

STDMETHODIMP COleClientSite::GetContainer(LPOLECONTAINER FAR* ppContainer)
{
	CDBG(	<< "In COleClientSite::GetContainer" << endl);

	// NULL the out pointer
	*ppContainer = NULL;

	return ResultFromScode(E_NOTIMPL);
}

//**********************************************************************
//
// COleClientSite::ShowObject
//
// Comments:
//
//      This function is not implemented because we don't support
//      linking.
//
//********************************************************************

STDMETHODIMP COleClientSite::ShowObject()
{
	CDBG(	<< "In COleClientSite::ShowObject" << endl);
	return NOERROR;
}

//**********************************************************************
//
// COleClientSite::OnShowWindow
//
// Purpose:
//
//      Object calls this method when it is opening/closing non-InPlace
//      Window
//
// Parameters:
//
//      BOOL fShow  - TRUE if Window is opening, FALSE if closing
//
// Return Value:
//
//      S_OK
//
//********************************************************************

STDMETHODIMP COleClientSite::OnShowWindow(BOOL bShow)
{
	CDBG(	<< "In COleClientSite::OnShowObject" << endl);
	
//	lpCDItem->getVThis()->update(lpCDItem->bounds(), FALSE);
	
	RECT		bounds;

	InvalidateRect(lpCDItem->getVThis()->hwnd(), 
				   &lpCDItem->bounds().getBounds(bounds, VOlePoint::Pixels), 
				   TRUE);

	lpCDItem->setActivationState(bShow ? VComponentItem::remotelyActive : VComponentItem::inactive);

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// COleClientSite::RequestNewObjectLayout
//
// Comments:
//
//      Not Implemented
//
//********************************************************************

STDMETHODIMP COleClientSite::RequestNewObjectLayout()
{
	CDBG(	<< "In COleClientSite::RequestNewObjectLayout" << endl);
	return ResultFromScode(E_NOTIMPL);
}
