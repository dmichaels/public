// $Header:   Q:/views/win/vcs/dropsrc.cpv   1.5   Mar 25 1997 15:57:14   gregm  $

//	dropsrc.cpp
//
//  Object Model Drop Source
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
//	12/09/94 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XDropSource::XDropSource(CDropSource *pDropSource)
	: DropSource( pDropSource)
{
}

//**********************************************************************
XDropSource::~XDropSource()
{
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
		// This warning tells us that we are passing this in an initializer,
		// before "this" is through initializing.  This is ok, because
		// we just store the ptr in the other constructors
CDropSource::CDropSource(REFVCOMPONENTWINDOW refCDWindow)
	: CUnknown(refCDWindow, new XDropSource(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In CDropSource's aggregate constructor, this = " << this 
			<< endl);

	this->lpCDWindow = &refCDWindow;

	exposeInterface(IID_IDropSource, "CDropSource");
}

//**********************************************************************
//
CDropSource::~CDropSource()
{
	CDBG(	<< "In CDropSource's destructor" << endl);
}

//**********************************************************************
//
// CDropSource::QueryContinueDrag
//
// Purpose:
//
//      Called to determine if a drop should take place or be canceled.
//
// Parameters:
//
//      BOOL fEscapePressed - TRUE if ESCAPE key has been pressed
//      DWORD grfKeyState   - key state
//
// Return Value:
//
//      DRAGDROP_S_CANCEL   - drag operation should be canceled
//      DRAGDROP_S_DROP     - drop operation should be performed
//      S_OK                - dragging should continue
//
// Comments:
//
//********************************************************************

STDMETHODIMP CDropSource::QueryContinueDrag (
		BOOL    fEscapePressed,
		DWORD   grfKeyState)
{
	if (fEscapePressed)
		return ResultFromScode(DRAGDROP_S_CANCEL);
	else if (!(grfKeyState & MK_LBUTTON))
		return ResultFromScode(DRAGDROP_S_DROP);
	else
		return NOERROR;
}

//**********************************************************************
//
// CDropSource::GiveFeedback
//
// Purpose:
//
//      Called to set cursor feedback
//
// Parameters:
//
//      DWORD dwEffect      - drop operation to give feedback for
//
// Return Value:
//
//      DRAGDROP_S_USEDEFAULTCURSORS  - tells OLE to use standard cursors
//
// Comments:
//
//********************************************************************

STDMETHODIMP CDropSource::GiveFeedback (DWORD dwEffect)
{
	return ResultFromScode(DRAGDROP_S_USEDEFAULTCURSORS);
}

//********************************************************************
//
VWindow *CDropSource::getVThis()
{
	return getComponentThis().getVThis();
}

