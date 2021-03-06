// $Header:   Q:/views/win/vcs/oipuiwin.h_v   1.6   Jun 03 1997 09:51:58   scottt  $

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
//			 		Use Views style #ifndef OIPUIWIN_H.
//			 		Change lpInPlaceActiveObjectName from VString to LPTSTR.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#ifndef COLEINPLACEUIWINDOW_H
#define COLEINPLACEUIWINDOW_H

// basic types before anything but system includes
#include "winrect.h"

// unknowns before OLE includes
#include "unknown.h"

// OLE includes
#include "owindow.h"

// Forward Declarations
CLASS COleInPlaceUIWindow;
typedef	COleInPlaceUIWindow FAR *LPCOLEINPLACEUIWINDOW;
typedef	COleInPlaceUIWindow FAR& REFCOLEINPLACEUIWINDOW;

CLASS COleInPlaceUIWindow : public COleWindow
{
public:

	// Base class constructor (Containment)
	// Use this constructor when this object is a base-object
	// contained in another object.
	COleInPlaceUIWindow(REFVCOMPONENTVIEW refCDView);

	// super-class constructor (Inheritance)
	// use this constructor when this object is a super-class 
	// being inherited by another class.
	COleInPlaceUIWindow(REFVCOMPONENTVIEW refCDView, LPUNKNOWN lpIUnkBase);
	
	virtual ~COleInPlaceUIWindow();

	LPSTR			getClassName() {return _T("COleInPlaceUIWindow");}

public:

	// *** IOleInPlaceUIWindow methods ***
    virtual STDMETHODIMP GetBorder(LPRECT lprectBorder);
    virtual STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS lpborderwidths);
    virtual STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS lpborderwidths);
    virtual STDMETHODIMP SetActiveObject(LPOLEINPLACEACTIVEOBJECT lpActiveObject,
                    LPCOLESTR lpszObjName);

	// *** COleInPlaceUIWindow methods ***
	operator LPOLEINPLACEUIWINDOW() {return (LPOLEINPLACEUIWINDOW) (LPUNKNOWN) getUnknown();}

	LPOLEINPLACEACTIVEOBJECT	getOleInPlaceActiveObject(void);
	void						setBorderSpace(LPCRECT borders);
	void						getBorderSpace(LPRECT borders);

	VComponentView&					getComponentThis() {return *lpCDView;}
	VView						*getVThis();

	boolean					resized(int w, int h);

protected:
	LPVCOMPONENTVIEW		lpCDView;
	LPOLEINPLACEACTIVEOBJECT lpInPlaceActiveObject;
	VString			lpInPlaceActiveObjectName;
	VWinRect			borders;

private:
	void			init(REFVCOMPONENTVIEW refCDView);
};

#define STD_IOLEINPLACEUIWINDOW_METHODS() \
    virtual STDMETHODIMP GetBorder(LPRECT lprectBorder) \
		{return getOleInPlaceUIWindow().GetBorder(lprectBorder);} \
    virtual STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS lpborderwidths) \
		{return getOleInPlaceUIWindow().RequestBorderSpace(lpborderwidths);} \
    virtual STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS lpborderwidths) \
		{return getOleInPlaceUIWindow().SetBorderSpace(lpborderwidths);} \
    virtual STDMETHODIMP SetActiveObject(LPOLEINPLACEACTIVEOBJECT lpActiveObject, \
                    LPCOLESTR lpszObjName) \
		{return getOleInPlaceUIWindow().SetActiveObject(lpActiveObject, \
                    									lpszObjName);}

#define STD_OleInPlaceUIWindow_METHODS() STD_IOLEINPLACEUIWINDOW_METHODS()

#define STD_IOLEINPLACEUIWINDOW_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, OleInPlaceUIWindow, access)

#define INHERIT_IOLEINPLACEUIWINDOW_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleInPlaceUIWindow) \
	STD_IOLEWINDOW_METHODS() \
	STD_IOLEWINDOW_ACCESS(*OleInPlaceUIWindow) \
	STD_IOLEINPLACEUIWINDOW_METHODS() \
	STD_IOLEINPLACEUIWINDOW_ACCESS(*OleInPlaceUIWindow) \
protected: \
	COleInPlaceUIWindow		*OleInPlaceUIWindow;

#define CONTAIN_IOLEINPLACEUIWINDOW_INTERFACE() \
public: \
	STD_IOLEINPLACEUIWINDOW_METHODS() \
	STD_IOLEINPLACEUIWINDOW_ACCESS(*OleInPlaceUIWindow) \
protected: \
	COleInPlaceUIWindow		*OleInPlaceUIWindow;

//**********************************************************************
//
//  IOleInPlaceUIWindow Class
//
CLASS XOleInPlaceUIWindow : public IOleInPlaceUIWindow
{
public:

	XOleInPlaceUIWindow(COleInPlaceUIWindow *);
	~XOleInPlaceUIWindow();

	INHERIT_INTERFACE(IOLEINPLACEUIWINDOW, OleInPlaceUIWindow)
};

//**********************************************************************
// inline Helpers
//

//**********************************************************************
//
inline LPOLEINPLACEACTIVEOBJECT COleInPlaceUIWindow::getOleInPlaceActiveObject(void)
{
	return lpInPlaceActiveObject;
}
	
//**********************************************************************
//
inline void COleInPlaceUIWindow::setBorderSpace(LPCRECT lpBorders)
{
	if (lpBorders) {
		this->borders.setBounds(*lpBorders, VOlePoint::Pixels);
	}
	else {
		this->borders.setBounds(0, 0, 0, 0, VOlePoint::Pixels);
	}
}

//**********************************************************************
//
inline void COleInPlaceUIWindow::getBorderSpace( LPRECT lpBorders)
{
	 borders.getBounds(*lpBorders, VOlePoint::Pixels);
}

#endif
