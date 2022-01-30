// $Header:   Q:/views/win/vcs/oipframe.h_v   1.6   Jun 03 1997 09:51:56   scottt  $

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
//			 		Use Views style #ifndef CUNKNOWN_H.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#ifndef COLEINPLACEFRAME_H
#define COLEINPLACEFRAME_H

// BASIC Views OLE types
#include "oledefs.h"

// Views Ole Classes
//#include "owindow.h"
#include "oipuiwin.h"

// Views Classes
#include "menu.h"

// Forward Declarations
CLASS COleInPlaceFrame;
typedef	COleInPlaceFrame FAR *LPCOLEINPLACEFRAME;

CLASS COleInPlaceFrame : public COleInPlaceUIWindow
{
public:

	COleInPlaceFrame(REFVCOMPONENTAPPVIEW refCDAppView);
	virtual ~COleInPlaceFrame();

	LPTSTR			getClassName() {return _T("COleInPlaceFrame");}

public:

	// *** IOleInPlaceFrame methods ***
    STDMETHODIMP InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
    STDMETHODIMP SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
    STDMETHODIMP RemoveMenus(HMENU hmenuShared);
    STDMETHODIMP SetStatusText(LPCOLESTR lpszStatusText);
    STDMETHODIMP EnableModeless(BOOL fEnable);
    STDMETHODIMP TranslateAccelerator(LPMSG lpmsg, WORD wID);

	operator LPOLEINPLACEFRAME() {return (LPOLEINPLACEFRAME)(LPUNKNOWN)getUnknown();}

	VPopupMenu		*menu0;
	VPopupMenu		*menu2;
	VPopupMenu		*menu4;

	VComponentAppView&		getComponentThis() {return *lpCDAppView;}
	VAppView		*getVThis();

protected:
	VComponentAppView			*lpCDAppView;

	boolean				removeMenu( HMENU hMenu, VMenu *menu);

private:
	void				init(REFVCOMPONENTAPPVIEW refCDAppView);
};

#define STD_IOLEINPLACEFRAME_METHODS() \
    virtual STDMETHODIMP InsertMenus(HMENU hmenuShared,  \
							 LPOLEMENUGROUPWIDTHS lpMenuWidths) \
			{return getOleInPlaceFrame().InsertMenus(hmenuShared,  \
							 						 lpMenuWidths);} \
    virtual STDMETHODIMP SetMenu(HMENU hmenuShared, HOLEMENU holemenu,  \
						 HWND hwndActiveObject) \
			{return getOleInPlaceFrame().SetMenu(hmenuShared, holemenu,  \
						 						 hwndActiveObject);} \
    virtual STDMETHODIMP RemoveMenus(HMENU hmenuShared) \
			{return getOleInPlaceFrame().RemoveMenus(hmenuShared);} \
    virtual STDMETHODIMP SetStatusText(LPCOLESTR lpszStatusText) \
			{return getOleInPlaceFrame().SetStatusText(lpszStatusText);} \
    virtual STDMETHODIMP EnableModeless(BOOL bEnable) \
			{return getOleInPlaceFrame().EnableModeless(bEnable);} \
    virtual STDMETHODIMP TranslateAccelerator(LPMSG lpmsg, WORD wID) \
			{return getOleInPlaceFrame().TranslateAccelerator(lpmsg, wID);}

#define STD_OleInPlaceFrame_METHODS() STD_IOLEINPLACEFRAME_METHODS() 

#define  STD_IOLEINPLACEFRAME_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, OleInPlaceFrame, access)

#define INHERIT_IOLEINPLACEFRAME_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleInPlaceFrame) \
	STD_IOLEWINDOW_METHODS() \
	STD_IOLEWINDOW_ACCESS(*OleInPlaceFrame) \
	STD_IOLEINPLACEUIWINDOW_METHODS() \
	STD_IOLEINPLACEUIWINDOW_ACCESS(*OleInPlaceFrame) \
	STD_IOLEINPLACEFRAME_METHODS() \
	STD_IOLEINPLACEFRAME_ACCESS(*OleInPlaceFrame) \
protected: \
	COleInPlaceFrame	*OleInPlaceFrame;

#define CONTAIN_IOLEINPLACEFRAME_INTERFACE() \
public: \
	STD_IOLEINPLACEFRAME_METHODS() \
	STD_IOLEINPLACEFRAME_ACCESS(*OleInPlaceFrame) \
protected: \
	COleInPlaceFrame	*OleInPlaceFrame;

//**********************************************************************
//
//  IOleInPlaceFrame Class
//
CLASS XOleInPlaceFrame : public IOleInPlaceFrame
{
public:

	XOleInPlaceFrame(COleInPlaceFrame *);
	~XOleInPlaceFrame();

	INHERIT_INTERFACE(IOLEINPLACEFRAME, OleInPlaceFrame)
};

#endif
