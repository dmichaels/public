// $Header:   Q:/views/win/vcs/DROPTARG.H_v   1.5   Mar 04 1997 14:12:36   gregm  $

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
//	01/09/96 dss	VComponent name change.
//	--------------------------------------------------------------------------

#ifndef DROPTARGET_H
#define DROPTARGET_H

// BASIC Views OLE types
#include "oledefs.h"

// unknown includes
#include "unknown.h"

/* Flags to control direction for drag scrolling */
typedef enum tagSCROLLDIR {
	SCROLLDIR_NULL          = 0,
	SCROLLDIR_UP            = 1,
	SCROLLDIR_DOWN          = 2,
	SCROLLDIR_RIGHT         = 3,
	SCROLLDIR_LEFT          = 4
} SCROLLDIR;

CLASS CDropTarget;
typedef CDropTarget FAR *LPCDROPTARGET;
typedef CDropTarget FAR& REFCDROPTARGET;

CLASS CDropTarget : public CUnknown
{
public:
	CDropTarget(REFVCOMPONENTWINDOW refVComponentWindow);
	virtual ~CDropTarget();

	LPTSTR		getClassName() {return _T("CDropTarget");}

public:

	// *** IDropTarget methods ***
	virtual STDMETHODIMP DragEnter (LPDATAOBJECT pDataObj, DWORD grfKeyState,
			POINTL pt, LPDWORD pdwEffect);
	virtual STDMETHODIMP DragOver  (DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	virtual STDMETHODIMP DragLeave ();
	virtual STDMETHODIMP Drop (LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt,
			LPDWORD pdwEffect);

	// cast operators to equate Cxxx class to Ixxx class
	operator 		LPDROPTARGET() {return (LPDROPTARGET)(LPUNKNOWN)getUnknown();}

	BOOL  m_fCanDropCopy;
	BOOL  m_fCanDropLink;
	DWORD m_dwSrcAspect;
	RECT  m_rcDragRect;
	POINT m_ptLast;
	BOOL  m_fDragFeedbackDrawn;
	DWORD m_dwTimeEnterScrollArea;  // time of entering scroll border region
	DWORD m_dwLastScrollDir;        // current dir for drag scroll
	DWORD m_dwNextScrollTime;       // time for next scroll

	void			Scroll(DWORD dwScrollDir);

	VComponentWindow&		getComponentThis() {return *lpCDWindow;}
	VWindow*		getVThis();

protected:
	LPVCOMPONENTWINDOW		lpCDWindow;

	void init(LPVCOMPONENTWINDOW lpCDWindow = 0);

	// Drag/Drop support methods
	BOOL QueryDrop (DWORD grfKeyState, POINTL pointl, BOOL fDragScroll,
			LPDWORD lpdwEffect);
	BOOL DoDragScroll( POINTL pointl );
	void InitDragFeedback(LPDATAOBJECT pDataObj, POINTL pointl);
	void DrawDragFeedback( POINTL pointl );
	void UndrawDragFeedback( void );
};

#define STD_IDROPTARGET_METHODS() \
	STDMETHODIMP DragEnter(LPDATAOBJECT pDataObj, DWORD grfKeyState, \
						   POINTL pt, LPDWORD pdwEffect) \
			{return getDropTarget().DragEnter(pDataObj, grfKeyState, \
											  pt, pdwEffect);} \
	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect) \
			{return getDropTarget().DragOver(grfKeyState, pt, pdwEffect);} \
	STDMETHODIMP DragLeave() \
			{return getDropTarget().DragLeave();} \
	STDMETHODIMP Drop(LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, \
					  LPDWORD pdwEffect) \
			{return getDropTarget().Drop(pDataObj, grfKeyState, pt,  \
										 pdwEffect);}

#define	STD_DropTarget_METHODS() STD_IDROPTARGET_METHODS()

#define STD_IDROPTARGET_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, DropTarget, access)

#define INHERIT_IDROPTARGET_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*DropTarget) \
	STD_IDROPTARGET_METHODS() \
	STD_IDROPTARGET_ACCESS(*DropTarget) \
protected: \
	CDropTarget		*DropTarget;

#define CONTAIN_IDROPTARGET_INTERFACE() \
public: \
	STD_IDROPTARGET_METHODS() \
	STD_IDROPTARGET_ACCESS(*DropTarget) \
protected: \
	CDropTarget		*DropTarget;

//**********************************************************************
//
//  IDropTarget Class
//
CLASS XDropTarget : public IDropTarget
{
public:

	XDropTarget(CDropTarget *);
	~XDropTarget();

	INHERIT_INTERFACE(IDROPTARGET, DropTarget)
};

#endif  // DROPTARGET_H
