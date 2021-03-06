// $Header:   Q:/views/win/vcs/owindow.h_v   1.5   Jun 03 1997 09:52:02   scottt  $

//	COleWindow
//
//  OLE IOleWindow Implementation
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
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#ifndef COLEWINDOW_H
#define COLEWINDOW_H

// BASIC Views OLE types
#include "oledefs.h"

// unknown includes
#include "unknown.h"

// Forward Declarations
CLASS COleWindow;
typedef	COleWindow FAR *LPCOLEWINDOW;

CLASS COleWindow : public CUnknown
{
public:

	// Base class constructor
	// Use this constructor when this object is a base-class object
	COleWindow(REFVCOMPONENTWINDOW refCDWindow);

	// super-class constructor
	// use this constructor when this object is a super-class object
	COleWindow(REFVCOMPONENTWINDOW refCDWindow, LPUNKNOWN lpIUnkBase);

	virtual ~COleWindow();

	LPTSTR		getClassName() {return _T("COleWindow");}

	// *** Inherited methods (no ';' follows these macros) ***
//	INHERIT_INTERFACE(IUNKNOWN, Unknown)

public:

	// *** IOleWindow methods ***
    virtual STDMETHODIMP GetWindow(HWND FAR* lphwnd);
    virtual STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

	operator LPOLEWINDOW() {return (LPOLEWINDOW) (LPUNKNOWN) getUnknown();}

	VComponentWindow&		getComponentThis() {return *lpCDWindow;}
	VWindow			*getVThis();

protected:
	VComponentWindow		*lpCDWindow;

private:
	void			init(REFVCOMPONENTWINDOW refCDWindow);

};

#define STD_IOLEWINDOW_METHODS() \
    virtual STDMETHODIMP GetWindow (HWND FAR* lphwnd) \
					{ return getOleWindow().GetWindow(lphwnd);} \
    virtual STDMETHODIMP ContextSensitiveHelp(BOOL bEnterMode) \
					{ return getOleWindow().ContextSensitiveHelp(bEnterMode);}

#define STD_OleWindow_METHODS() STD_IOLEWINDOW_METHODS()

#define STD_IOLEWINDOW_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, OleWindow, access)

#define INHERIT_IOLEWINDOW_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleWindow) \
	STD_IOLEWINDOW_METHODS() \
	STD_IOLEWINDOW_ACCESS(*OleWindow) \
protected: \
	COleWindow		*OleWindow;

#define CONTAIN_IOLEWINDOW_INTERFACE() \
public: \
	STD_IOLEWINDOW_METHODS() \
	STD_IOLEWINDOW_ACCESS(*OleWindow) \
protected: \
	COleWindow		*OleWindow;

//**********************************************************************
//
//  IOleWindow Class
//
CLASS XOleWindow : public IOleWindow
{
public:

	XOleWindow(COleWindow *);
	~XOleWindow();

	INHERIT_INTERFACE(IOLEWINDOW, OleWindow)
};

#endif
