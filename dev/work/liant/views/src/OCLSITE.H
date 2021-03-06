// $Header:   Q:/views/win/vcs/OCLSITE.H_v   1.6   Mar 04 1997 14:13:16   gregm  $

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
//	---------------------------------------------------------------------------

#ifndef COLECLIENTSITE_H
#define COLECLIENTSITE_H

#include "unknown.h"

// Forward Declarations
CLASS VComponentItem;
typedef VComponentItem FAR& REFVCOMPONENTITEM;
typedef VComponentItem FAR *LPVCOMPONENTITEM;

CLASS COleClientSite;
typedef	COleClientSite FAR *LPCOLECLIENTSITE;

CLASS COleClientSite : public CUnknown
{
public:

	COleClientSite(REFVCOMPONENTITEM refCDItem);
	virtual ~COleClientSite();

	LPTSTR			getClassName() {return _T("COleClientSite");}

public:

	// *** IOleClientSite methods ***
	virtual STDMETHODIMP SaveObject();
	virtual STDMETHODIMP GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppmk);
	virtual STDMETHODIMP GetContainer(LPOLECONTAINER FAR* ppContainer);
	virtual STDMETHODIMP ShowObject();
	virtual STDMETHODIMP OnShowWindow(BOOL bShow);
	virtual STDMETHODIMP RequestNewObjectLayout();

	operator 		LPOLECLIENTSITE() {return (LPOLECLIENTSITE)(LPUNKNOWN)getUnknown();}

protected:
	LPVCOMPONENTITEM	lpCDItem;
};

#define STD_IOLECLIENTSITE_METHODS() \
	virtual STDMETHODIMP SaveObject() \
				{ return getOleClientSite().SaveObject();} \
	virtual STDMETHODIMP GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppmk) \
				{ return getOleClientSite().GetMoniker(dwAssign, dwWhichMoniker, ppmk);} \
	virtual STDMETHODIMP GetContainer(LPOLECONTAINER FAR* ppContainer) \
				{ return getOleClientSite().GetContainer(ppContainer);} \
	virtual STDMETHODIMP ShowObject() \
				{ return getOleClientSite().ShowObject();} \
	virtual STDMETHODIMP OnShowWindow(BOOL bShow) \
				{ return getOleClientSite().OnShowWindow(bShow);} \
	virtual STDMETHODIMP RequestNewObjectLayout() \
				{ return getOleClientSite().RequestNewObjectLayout();}

#define STD_OleClientSite_METHODS() STD_IOLECLIENTSITE_METHODS()

#define STD_IOLECLIENTSITE_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, OleClientSite, access)

#define INHERIT_IOLECLIENTSITE_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleClientSite) \
	STD_IOLECLIENTSITE_METHODS() \
	STD_IOLECLIENTSITE_ACCESS(*OleClientSite) \
protected: \
	COleClientSite		*OleClientSite;

#define CONTAIN_IOLECLIENTSITE_INTERFACE() \
public: \
	STD_IOLECLIENTSITE_METHODS() \
	STD_IOLECLIENTSITE_ACCESS(*OleClientSite) \
protected: \
	COleClientSite		*OleClientSite;

//**********************************************************************
//
//  IOleClientSite Class
//
CLASS XOleClientSite : public IOleClientSite
{
public:

	XOleClientSite(COleClientSite *);
	~XOleClientSite();

	INHERIT_INTERFACE(IOLECLIENTSITE, OleClientSite)
};

#endif // COLECLIENTSITE_H
