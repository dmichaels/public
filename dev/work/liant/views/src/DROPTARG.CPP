// $Header:   Q:/views/win/vcs/droptarg.cpv   1.7   Mar 21 1997 08:42:48   gregm  $

//	droptarg.cpp
//
//	CDropTarget
//
//  OLE IDropTarget Implementation
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
//	12/04/95 dss	Added PVCS header. Drop(): point is in mm not pixels.
//	12/08/95 dss	Drop(): point IS IN pixels. Its in screen coords.
//					Mapped in to parent's coords.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

#include "cmpitem.h"

extern CLIPFORMAT g_cfObjectDescriptor;

//**********************************************************************
//
XDropTarget::XDropTarget(CDropTarget *pDropTarget)
	: DropTarget( pDropTarget)
{
}

//**********************************************************************
XDropTarget::~XDropTarget()
{
}

//**********************************************************************
//
void CDropTarget::init(LPVCOMPONENTWINDOW lpCDWindow)
{
	this->lpCDWindow = lpCDWindow;
	m_fCanDropCopy = FALSE;
	m_fCanDropLink = FALSE;
	m_fDragFeedbackDrawn = FALSE;
	m_dwTimeEnterScrollArea = 0L;
	m_dwNextScrollTime = 0L;
	m_dwLastScrollDir = SCROLLDIR_NULL;

	exposeInterface(IID_IDropTarget, "CDropTarget");
}

//**********************************************************************
//
//
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
CDropTarget::CDropTarget(REFVCOMPONENTWINDOW refCDWindow)
	: CUnknown(refCDWindow, new XDropTarget(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In CDropTarget's aggregate constructor, this = " << this 
			<< endl);

	init(&refCDWindow);
};

//**********************************************************************
//
CDropTarget::~CDropTarget()
{
	CDBG(	<< "In CDropTarget's destructor" << endl);
};

//**********************************************************************
//
// CDropTarget::QueryDrop
//
// Purpose:
//
//      Check if the desired drop operation (identified by the given key
//      state) is possible at the current mouse position (pointl).
//
// Parameters:
//
//      DWORD grfKeyState       - current key state
//      POINTL pointl           - position of mouse
//      BOOL fDragScroll        - TRUE if drag scrolling cursor should
//                                be shown.
//      LPDWORD pdwEffect       - (OUT) drag effect that should occur
//
// Return Value:
//
//      BOOL                    - TRUE if drop could take place,
//                                else FALSE
//
// Comments:
//
//********************************************************************

BOOL CDropTarget::QueryDrop (
	DWORD           grfKeyState,
	POINTL          pointl,
	BOOL            fDragScroll,
	LPDWORD         pdwEffect
)
{
	DWORD      dwScrollEffect = 0L;
	DWORD      dwOKEffects = *pdwEffect;

	/* check if the cursor is in the active scroll area, if so need the
	**    special scroll cursor.
	*/
	if (fDragScroll)
		dwScrollEffect = DROPEFFECT_SCROLL;

	/* if we have already determined that the source does NOT have any
	**    acceptable data for us, the return NO-DROP
	*/
	if (! m_fCanDropCopy && ! m_fCanDropLink)
		goto dropeffect_none;

	/* OLE2NOTE: determine what type of drop should be performed given
	**    the current modifier key state. we rely on the standard
	**    interpretation of the modifier keys:
	**          no modifier -- DROPEFFECT_MOVE or whatever is allowed by src
	**          SHIFT       -- DROPEFFECT_MOVE
	**          CTRL        -- DROPEFFECT_COPY
	**          CTRL-SHIFT  -- DROPEFFECT_LINK
	*/

	*pdwEffect = OleStdGetDropEffect(grfKeyState);
	if (*pdwEffect == 0) {
		// No modifier keys given. Try in order MOVE, COPY, LINK.
		if ((DROPEFFECT_MOVE & dwOKEffects) && m_fCanDropCopy)
			*pdwEffect = DROPEFFECT_MOVE;
		else if ((DROPEFFECT_COPY & dwOKEffects) && m_fCanDropCopy)
			*pdwEffect = DROPEFFECT_COPY;
		else if ((DROPEFFECT_LINK & dwOKEffects) && m_fCanDropLink)
			*pdwEffect = DROPEFFECT_LINK;
		else
			goto dropeffect_none;
	} else {
		/* OLE2NOTE: we should check if the drag source application allows
		**    the desired drop effect.
		*/
		if (!(*pdwEffect & dwOKEffects))
			goto dropeffect_none;

		if ((*pdwEffect == DROPEFFECT_COPY || *pdwEffect == DROPEFFECT_MOVE)
				&& ! m_fCanDropCopy)
			goto dropeffect_none;

		if (*pdwEffect == DROPEFFECT_LINK && ! m_fCanDropLink)
			goto dropeffect_none;
	}

	*pdwEffect |= dwScrollEffect;
	return TRUE;

dropeffect_none:

	*pdwEffect = DROPEFFECT_NONE;
	return FALSE;
}

//**********************************************************************
//
// CDropTarget::DoDragScroll
//
// Purpose:
//
//     Check to see if Drag scroll operation should be initiated.
//
// Parameters:
//
//      POINTL pointl           - position of mouse
//
// Return Value:
//
//      BOOL                    - TRUE if scroll cursor should be given
//                                else FALSE
//
// Comments:
//     A Drag scroll operation should be initiated when the mouse has
//     remained in the active scroll area (11 pixels frame around border
//     of window) for a specified amount of time (50ms).
//
//********************************************************************

BOOL CDropTarget::DoDragScroll (POINTL pointl)
{
	DWORD dwScrollDir;
	DWORD dwTime = GetCurrentTime();
	VComponentAppView *CDAppView = 
		getVThis()->getAppLevelParent()->getComponentThis();
	int nScrollInset = CDAppView->getScrollInset();
	int nScrollDelay = CDAppView->getScrollDelay();
	int nScrollInterval = CDAppView->getScrollInterval();
	POINT point;
	RECT rect;

	point.x = (int)pointl.x;
	point.y = (int)pointl.y;

	VWindow		*topLevelParent = getVThis()->getTopLevelParent();

	cv_assert(topLevelParent);

	ScreenToClient( topLevelParent->hwnd(), &point);
	GetClientRect ( topLevelParent->hwnd(), (LPRECT) &rect );

	if (rect.top <= point.y && point.y<=(rect.top+nScrollInset)) {
		dwScrollDir = SCROLLDIR_UP;
	} else if ((rect.bottom-nScrollInset) <= point.y && point.y <= rect.bottom) {
		dwScrollDir = SCROLLDIR_DOWN;
	} else if (rect.left <= point.x && point.x <= (rect.left+nScrollInset)) {
		dwScrollDir = SCROLLDIR_LEFT;
	} else if ((rect.right-nScrollInset) <= point.x && point.x <= rect.right) {
		dwScrollDir = SCROLLDIR_RIGHT;
	}

	if (m_dwTimeEnterScrollArea) {

		/* cursor was already in Scroll Area */

		if (! dwScrollDir) {
			/* cusor moved OUT of scroll area.
			**      clear "EnterScrollArea" time.
			*/
			m_dwTimeEnterScrollArea = 0L;
			m_dwNextScrollTime = 0L;
			m_dwLastScrollDir = SCROLLDIR_NULL;

		} else if (dwScrollDir != m_dwLastScrollDir) {
			/* cusor moved into a different direction scroll area.
			**      reset "EnterScrollArea" time to start a new 50ms delay.
			*/
			m_dwTimeEnterScrollArea = dwTime;
			m_dwNextScrollTime = dwTime + (DWORD)nScrollDelay;
			m_dwLastScrollDir = dwScrollDir;

		} else if (dwTime && dwTime >= m_dwNextScrollTime) {
			Scroll ( dwScrollDir );
			m_dwNextScrollTime = dwTime + (DWORD)nScrollInterval;
		}
	} else {
		if (dwScrollDir) {
			/* cusor moved INTO a scroll area.
			**      reset "EnterScrollArea" time to start a new 50ms delay.
			*/
			m_dwTimeEnterScrollArea = dwTime;
			m_dwNextScrollTime = dwTime + (DWORD)nScrollDelay;
			m_dwLastScrollDir = dwScrollDir;
		}
	}

	return (dwScrollDir ? TRUE : FALSE);
}


// Support functions/macros
#define SetTopLeft(rc, pt)      \
	((rc)->top = (pt)->y,(rc)->left = (pt)->x)
#define SetBottomRight(rc, pt)      \
	((rc)->bottom = (pt)->y,(rc)->right = (pt)->x)
#define OffsetPoint(pt, dx, dy)     \
	((pt)->x += dx, (pt)->y += dy)


/* HighlightRect
** -------------
**    Invert rectangle on screen. used for drop target feedback.
*/

static int HighlightRect(HWND hwnd, HDC hdc, LPRECT rc)
{
	POINT pt1, pt2;
	int old = SetROP2(hdc, R2_NOT);
	HPEN hpen;
	HGDIOBJ hold;

	pt1.x = rc->left;
	pt1.y = rc->top;
	pt2.x = rc->right;
	pt2.y = rc->bottom;

	ScreenToClient(hwnd, &pt1);
	ScreenToClient(hwnd, &pt2);

	hold = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	hpen = (HPEN) SelectObject(hdc, CreatePen(PS_SOLID, 2,
						  GetSysColor(COLOR_ACTIVEBORDER)));

	Rectangle(hdc, pt1.x, pt1.y, pt2.x, pt2.y);

	SetROP2(hdc, old);

	hold = SelectObject(hdc, hold);
	hpen = (HPEN) SelectObject(hdc, hpen);

	DeleteObject(hpen);

  return 0;
}


//**********************************************************************
//
// CDropTarget::InitDragFeedback
//
// Purpose:
//
//      Initialize data used to draw drop target feedback.
//      As feedback we draw a rectangle the size of the object.
//
// Parameters:
//
//      LPDATAOBJECT pDataObj   - IDataObject from Drop source
//      POINTL pointl           - position of mouse
//
// Return Value:
//
//      none.
//
// Function Calls:
//      Function                    Location
//
//      IDataObject::GetData        Object
//      XformSizeInHimetricToPixels OLE2UI Library
//      GlobalLock                  WINDOWS API
//      GlobalUnlock                WINDOWS API
//      ReleaseStgMedium            OLE2 API
//      OffsetPoint                 IDT.CPP
//      SetTopLeft                  IDT.CPP
//      SetBottomRight              IDT.CPP
//
// Comments:
//      In order to known the size of the object before the object
//      is actually dropped, we render CF_OBJECTDESCRIPTOR format.
//      this data format tells us both the size of the object as
//      well as which aspect is the object is displayed as in the
//      source. if the object is currently displayed as DVASPECT_ICON
//      then we want to create the object also as DVASPECT_ICON.
//
//********************************************************************

void CDropTarget::InitDragFeedback(LPDATAOBJECT pDataObj, POINTL pointl)
{
	FORMATETC fmtetc;
	STGMEDIUM stgmed;
	POINT pt;
	int height, width;
	HRESULT hrErr;

	height = width = 100; // some default values
	pt.x = (int)pointl.x;
	pt.y = (int)pointl.y;

	// do a GetData for CF_OBJECTDESCRIPTOR format to get the size of the
	// object as displayed in the source. using this size, initialize the
	// size for the drag feedback rectangle.
	fmtetc.cfFormat = g_cfObjectDescriptor;
	fmtetc.ptd = NULL;
	fmtetc.lindex = -1;
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.tymed = TYMED_HGLOBAL;

	hrErr = pDataObj->GetData(&fmtetc, &stgmed);
	if (hrErr == NOERROR) {
		LPOBJECTDESCRIPTOR pOD=(LPOBJECTDESCRIPTOR)GlobalLock(stgmed.hGlobal);
		if (pOD != NULL) {
			XformSizeInHimetricToPixels(NULL, &pOD->sizel, &pOD->sizel);

			width = (int)pOD->sizel.cx;
			height = (int)pOD->sizel.cy;
			m_dwSrcAspect = pOD->dwDrawAspect;
		}

		GlobalUnlock(stgmed.hGlobal);
		ReleaseStgMedium(&stgmed);
	}

	m_ptLast = pt;
	m_fDragFeedbackDrawn = FALSE;

	OffsetPoint(&pt, -(width/2), -(height/2));
	SetTopLeft(&m_rcDragRect, &pt);

	OffsetPoint(&pt, width, height);
	SetBottomRight(&m_rcDragRect, &pt);
}


//**********************************************************************
//
// CDropTarget::UndrawDragFeedback
//
// Purpose:
//
//      Erase any drop target feedback.
//      As feedback we draw a rectangle the size of the object.
//
// Comments:
//      In order to known the size of the object before the object
//      is actually dropped, we render CF_OBJECTDESCRIPTOR format.
//      this data format tells us both the size of the object as
//      well as which aspect is the object is displayed as in the
//      source. if the object is currently displayed as DVASPECT_ICON
//      then we want to create the object also as DVASPECT_ICON.
//
//********************************************************************

void CDropTarget::UndrawDragFeedback( void )
{
	if (m_fDragFeedbackDrawn) {
		m_fDragFeedbackDrawn = FALSE;
		HDC hDC = getVThis()->getHndl();

		// to do: use VPort/VRectangle to draw the highlite rect
		HighlightRect(getVThis()->hwnd(), hDC, &m_rcDragRect);
		getVThis()->ungetHndl();
	}
}

//**********************************************************************
//
// CDropTarget::DrawDragFeedback
//
// Purpose:
//
//      Compute new position of drop target feedback rectangle and
//      erase old rectangle and draw new rectangle.
//      As feedback we draw a rectangle the size of the object.
//
// Parameters:
//
//      POINTL pointl           - position of mouse
//
// Comments:
//
//********************************************************************

void CDropTarget::DrawDragFeedback( POINTL pointl )
{
	POINT ptDiff;

	ptDiff.x = (int)pointl.x - m_ptLast.x;
	ptDiff.y = (int)pointl.y - m_ptLast.y;

	if (m_fDragFeedbackDrawn && (ptDiff.x == 0 && ptDiff.y == 0))
		return;     // mouse did not move; leave rectangle as drawn

	HDC hDC = getVThis()->getHndl();

	if (m_fDragFeedbackDrawn) {
		m_fDragFeedbackDrawn = FALSE;
		// to do: use VPort/VRectangle to draw the highlite rect
		HighlightRect(getVThis()->hwnd(), hDC, &m_rcDragRect);
	}

	OffsetRect(&m_rcDragRect, ptDiff.x, ptDiff.y);
	// to do: use VPort/VRectangle to draw the highlite rect
	HighlightRect(getVThis()->hwnd(), hDC, &m_rcDragRect);

	m_fDragFeedbackDrawn = TRUE;
	m_ptLast.x = (int)pointl.x;
	m_ptLast.y = (int)pointl.y;
	
	getVThis()->ungetHndl();
}

//**********************************************************************
//
// CDropTarget::DragEnter
//
// Purpose:
//
//      Called when the mouse first enters our DropTarget window
//
// Parameters:
//
//      LPDATAOBJECT pDataObj   - IDataObject from Drop source
//      DWORD grfKeyState       - current key state
//      POINTL pointl           - position of mouse
//      LPDWORD pdwEffect       - (IN-OUT) drag effect that should occur
//                                ON INPUT, this is dwOKEffects that source
//                                passed to DoDragDrop API.
//                                ON OUTPUT, this is the effect that we
//                                want to take effect (used to determine
//                                cursor feedback).
//
// Return Value:
//
//      S_OK
//
// Comments:
//      Callee should honor the dwEffects as passed in to determine
//      if the caller allows DROPEFFECT_MOVE.
//
//********************************************************************

STDMETHODIMP CDropTarget::DragEnter (LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pointl, LPDWORD pdwEffect)
{
	CDBG(	<< "In CDropTarget::DragEnter, mouse = " << pointl << endl);

	/* Determine if the drag source data object offers a data format
	**  that we understand. we accept only creating embedded objects.
	*/
	m_fCanDropCopy = (SUCCEEDED(OleQueryCreateFromData(pDataObj)) ?
			TRUE : FALSE);
	m_fCanDropLink = (SUCCEEDED(OleQueryLinkFromData(pDataObj)) ?
			TRUE : FALSE);

	if (m_fCanDropCopy || m_fCanDropLink)
		InitDragFeedback(pDataObj, pointl);

	BOOL fDragScroll = DoDragScroll ( pointl );

	if (QueryDrop(grfKeyState,pointl,fDragScroll,pdwEffect))
		DrawDragFeedback( pointl );

	return NOERROR;
}


//**********************************************************************
//
// CDropTarget::DragOver
//
// Purpose:
//
//      Called when the mouse moves, key state changes, or a time
//      interval passes while the mouse is still within our DropTarget
//      window.
//
// Parameters:
//
//      DWORD grfKeyState       - current key state
//      POINTL pointl           - position of mouse
//      LPDWORD pdwEffect       - (IN-OUT) drag effect that should occur
//                                ON INPUT, this is dwOKEffects that source
//                                passed to DoDragDrop API.
//                                ON OUTPUT, this is the effect that we
//                                want to take effect (used to determine
//                                cursor feedback).
//
// Return Value:
//
//      S_OK
//
// Comments:
//      Callee should honor the dwEffects as passed in to determine
//      if the caller allows DROPEFFECT_MOVE. OLE pulses the DragOver
//      calls in order that the DropTarget can implement drag scrolling
//
//********************************************************************

STDMETHODIMP CDropTarget::DragOver  (DWORD grfKeyState, POINTL pointl, LPDWORD pdwEffect)
{
	CDBG(	<< "In CDropTarget::DragOver, mouse = " << pointl << endl);

	BOOL fDragScroll = DoDragScroll ( pointl );

	if (QueryDrop(grfKeyState,pointl,fDragScroll,pdwEffect))
		DrawDragFeedback( pointl );
	else
		UndrawDragFeedback();

	return NOERROR;
}


//**********************************************************************
//
// CDropTarget::DragLeave
//
// Purpose:
//
//      Called when the mouse leaves our DropTarget window
//
// Parameters:
//
//      none.
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      UndrawDragFeedback          IDT.CPP
//      ResultFromScode             OLE2 API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CDropTarget::DragLeave ()
{
	CDBG(	<< "In CDropTarget::DragLeave" << endl);

	UndrawDragFeedback();

	return ResultFromScode(S_OK);
}

//**********************************************************************
//
// CDropTarget::Drop
//
// Purpose:
//
//      Called when a Drop operation should take place.
//
// Parameters:
//
//      LPDATAOBJECT pDataObj   - IDataObject from Drop source
//      DWORD grfKeyState       - current key state
//      POINTL pointl           - position of mouse
//      LPDWORD pdwEffect       - (IN-OUT) drag effect that should occur
//                                ON INPUT, this is dwOKEffects that source
//                                passed to DoDragDrop API.
//                                ON OUTPUT, this is the effect that we
//                                want to take effect (used to determine
//                                cursor feedback).
//
// Return Value:
//
//      S_OK
//
// Comments:
//      Callee should honor the dwEffects as passed in to determine
//      if the caller allows DROPEFFECT_MOVE.
//
//********************************************************************

STDMETHODIMP CDropTarget::Drop (LPDATAOBJECT pDataObj, DWORD grfKeyState, 
								POINTL pointl, LPDWORD pdwEffect)

{
	CDBG(	<< "In CDropTarget::Drop, mouse = " << pointl << endl);

	UndrawDragFeedback();

	if (pDataObj && QueryDrop(grfKeyState, pointl, FALSE, pdwEffect)) { 
		VComponentWindow&	cdWin = getComponentThis();

		//!!!dss - 12/09/95: views doesn't support 32 bit coords, yet.
		cv_assert( pointl.x < (1 << 16));
		cv_assert( pointl.y < (1 << 16));

		int		x = (int) pointl.x, 
				y = (int) pointl.y;
		
		cdWin.getVThis()->mapToWin(&x, &y);

		cdWin.objDropped( CRDataObject(pDataObj), 
						  VOlePosition(x, y, VOlePoint::Pixels),
						  *pdwEffect);
	}

	return ResultFromScode(S_OK);
}

//********************************************************************
//
void CDropTarget::Scroll(DWORD dwScrollDir)
{
	int		dx, dy;

	switch (dwScrollDir) {
	case SCROLLDIR_UP:
		dy = -10;	// scroll up
		dx = 0;
		break;

	case SCROLLDIR_DOWN:
		dy = 10;	// scroll down
		dx = 0;
		break;

	case SCROLLDIR_LEFT:
		dx = -10;	// scroll left
		dy = 0;
		break;

	case SCROLLDIR_RIGHT:
		dx = 10;	// scroll right
		dy = 0;
		break;

	default:
		dx = 0;
		dy = 0;
	} // switch

	getVThis()->scroll(dx, dy);
}

//**********************************************************************
//
VWindow *CDropTarget::getVThis()
{
	return getComponentThis().getVThis();
}

