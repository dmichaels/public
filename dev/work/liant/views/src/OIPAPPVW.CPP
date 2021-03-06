// $Header:   Q:/views/win/vcs/oipappvw.cpv   1.4   Jun 03 1997 09:51:54   scottt  $

//	VOleInPlaceAppView
//
//  Ole In Place AppView Interface
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
//	02/17/95 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"
#include "statusbr.h"

//**********************************************************************
//
VOleInPlaceAppView::VOleInPlaceAppView(REFVCOMPONENTAPPVIEW refCDAppView)
	: COleInPlaceFrame(refCDAppView)
{
}

//**********************************************************************
//
VOleInPlaceAppView::~VOleInPlaceAppView()
{
}

//**********************************************************************
//
STDMETHODIMP VOleInPlaceAppView::SetBorderSpace(LPCBORDERWIDTHS lpBorderWidths)
{
	CDBG(	<< "In VOleInPlaceAppView::SetBorderSpace, border widths = "); // NO endl!
            
    if (lpBorderWidths) {
		CDBG(  << *lpBorderWidths << endl);
	} else {
		CDBG(  << "<null>" << endl);
	}

	// super-class (inheritance)
	HRESULT hResult = COleInPlaceFrame::SetBorderSpace(lpBorderWidths);

	// super-object (containment)
	hResult = getComponentThis().setToolSpace(lpBorderWidths);

	return hResult;
}

//**********************************************************************
//
STDMETHODIMP VOleInPlaceAppView::GetWindow(HWND FAR* lphwnd)
{
	HRESULT hResult;

#if 0	// just testing
	SCODE		sc;

	if (lphwnd == 0) {
		sc = E_INVALIDARG;
	}
	else if (getComponentThis()->isA(VStdMdiAppViewCls)) {
		if (*lphwnd = ((VStdMdiAppView*)getComponentThis())->clientOf()->hwnd()) {
			sc = S_OK;
		}
		else {
			sc = E_FAIL;
		}
	}
	else if (*lphwnd = getComponentThis()->hwnd()) {
		sc = S_OK;
	}
	else {
		// Don't have to null *lpwnd; it will be NULL by hwnd(), if we 
		// get here
		sc = E_FAIL;
	}

	hResult = ResultFromScode(sc);
#else
	hResult = COleInPlaceFrame::GetWindow(lphwnd);
#endif

	CDBG(	<< "In " << getClassName() << "::GetWindow, hwnd = " 
			<< hex << int(*lphwnd) << endl);

	return hResult;
}
