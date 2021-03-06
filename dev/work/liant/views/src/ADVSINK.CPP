// $Header:   Q:/views/win/vcs/advsink.cpv   1.7   Mar 21 1997 08:42:40   gregm  $

//	CAdviseSink
//
//  OLE IAdviseSink Implementation
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
//  06/05/96 evc	IAdvise::OnViewChange now calls
//  				VComponentItem::uponViewAdvise  which is virtual
//					in ComponentItem.  Allows custom behavior to be delegated
//					to the site. Similarly,
//					IAdvise::OnDataChange calls ponDataAdvise();
//  
//	--------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XAdviseSink::XAdviseSink(CAdviseSink *pAdviseSink)
	: AdviseSink( pAdviseSink)
{
}

//**********************************************************************
XAdviseSink::~XAdviseSink()
{
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
CAdviseSink::CAdviseSink(REFVCOMPONENTITEM refCDItem)
	: CUnknown(refCDItem, new XAdviseSink(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In CAdviseSink's aggregate constructor, this = " << this
			<< endl);

	this->lpCDItem = &refCDItem;
	lCount = 0;

	exposeInterface(IID_IAdviseSink, "CAdviseSink");

	// There's really no CAdviseSink2 class. This is just for
	// differentiating the interfaces on debug output. The same Interface
	// Pointer is returned (getUnknown()), regardless.
	exposeInterface(IID_IAdviseSink2, "CAdviseSink2");
};

//**********************************************************************
//
CAdviseSink::~CAdviseSink() {
	CDBG(	<< "In CAdviseSink's destructor" << endl);
	cv_assert(lCount == 0);
};

//**********************************************************************
//
// CAdviseSink::OnDataChange
//
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//
// Function Calls:
//      Function                    Location
//
// Comments:
//
//      Not Implemented (needs to be stubbed out)
//
//********************************************************************

STDMETHODIMP_(void) CAdviseSink::OnDataChange (FORMATETC FAR* pFormatetc, STGMEDIUM FAR* pStgmed)
{
	CDBG(	<< "In CAdviseSink::OnDataChange" << endl);
	lpCDItem->uponDataAdvise();
}

//**********************************************************************
//
// CAdviseSink::OnViewChange
//
// Purpose:
//
//      Notifies us that the view has changed and needs to be updated.
//
// Parameters:
//
//      DWORD dwAspect  - Aspect that has changed
//      LONG lindex     - Index that has changed
//
// Return Value:
//
//      None
//
//********************************************************************

STDMETHODIMP_(void) CAdviseSink::OnViewChange (DWORD dwAspect, LONG lindex)
{
	LPVIEWOBJECT2 lpViewObject2;
	CDBG(	<< "In CAdviseSink::OnViewChange" << endl);

	// get a pointer to IViewObject2
	HRESULT hResult = lpCDItem->getOleObject().QueryInterface(
			IID_IViewObject2,(LPVOID FAR *)&lpViewObject2);

	if (SUCCEEDED(hResult)) {
		SIZEL		sizel;
		// get extent of the object
		// NOTE: this method will never be remoted; it can be called w/i 
		// this async method, contrary to what the documentation says.
		// Got this from Microsoft Simpdnd example.
		lpViewObject2->GetExtent(DVASPECT_CONTENT, -1 , NULL, &sizel);
		lpViewObject2->Release();

		lpCDItem->size(VOleSize(sizel,VOlePoint::MM100ths));
	}

	// TO DO: substitute with lpCDItem->update() when items get real hwnds.
	// EC 6/5  - invented VComponentItem::uponViewAdvise instead

	lpCDItem->uponViewAdvise();

}

//**********************************************************************
//
// CAdviseSink::OnRename
//
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//
//********************************************************************

STDMETHODIMP_(void) CAdviseSink::OnRename (LPMONIKER pmk)
{
	CDBG(	<< "In CAdviseSink::OnRename" << endl);
}

//**********************************************************************
//
// CAdviseSink::OnSave
//
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//
//********************************************************************

STDMETHODIMP_(void) CAdviseSink::OnSave ()
{
	CDBG(	<< "In CAdviseSink::OnSave" << endl);
}

//**********************************************************************
//
// CAdviseSink::OnClose
//
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//
//********************************************************************

STDMETHODIMP_(void) CAdviseSink::OnClose()
{
	CDBG(	<< "In CAdviseSink::OnClose" << endl);
}

//********************************************************************
//
STDMETHODIMP_(void) CAdviseSink::OnLinkSrcChange(LPMONIKER pmk)
{
	CDBG(	<< "In CAdviseSink::OnLinkSrcChange" << endl);
}

