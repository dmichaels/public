// $Header:   Q:/views/win/vcs/OOBJECT.H_v   1.7   Mar 04 1997 14:13:26   gregm  $

//	COleObject
//
//  OLE IOleObject Implementation
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
//	12/04/95 dss	Added PVCS header. Use Views style #ifndef OOBJECT_H.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#ifndef COLEOBJECT_H
#define COLEOBJECT_H

#include "owindow.h"

// Forward Declarations
CLASS VComponentItem;
typedef VComponentItem FAR& REFVCOMPONENTITEM;
typedef VComponentItem FAR *LPVCOMPONENTITEM;

CLASS COleObject;
typedef	COleObject FAR *LPCOLEOBJECT;
typedef IOleObject FAR& REFOLEOBJECT;

CLASS CROleObject;
typedef	CROleObject FAR *LPCROLEOBJECT;
typedef CROleObject FAR& REFCOLEOBJECT;

//**********************************************************************
//
// COleObject Implementation
//
CLASS COleObject : public CUnknown
{
public:

	COleObject(REFCUNKNOWN refCUnknown, LPOLEOBJECT pOleObject);
	COleObject(REFCUNKNOWN refCUnknown);
	virtual ~COleObject();

	TCHAR			*getClassName() {return _T("COleObject");}

public:

	// *** IOleObject methods ***
	// to be filled in later. We have no OleObject implementation, yet
    virtual STDMETHODIMP SetClientSite(LPOLECLIENTSITE pClientSite)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP GetClientSite (LPOLECLIENTSITE FAR* ppClientSite)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP SetHostNames (LPCOLESTR szContainerApp, LPCOLESTR szContainerObj)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP Close (DWORD dwSaveOption)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP SetMoniker (DWORD dwWhichMoniker, LPMONIKER pmk)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP GetMoniker (DWORD dwAssign, DWORD dwWhichMoniker,
									 LPMONIKER FAR* ppmk)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP InitFromData (LPDATAOBJECT pDataObject,
                BOOL fCreation,
                DWORD dwReserved)
				{ return ResultFromScode(E_NOTIMPL);}
	virtual STDMETHODIMP GetClipboardData (DWORD dwReserved,
										LPDATAOBJECT FAR* ppDataObject)
				{ return ResultFromScode(E_NOTIMPL);}
	virtual STDMETHODIMP DoVerb (LONG iVerb, LPMSG lpmsg,
	            			    LPOLECLIENTSITE pActiveSite, LONG lindex,
	        			        HWND hwndParent, LPCRECT lprcPosRect)
				{ return ResultFromScode(E_NOTIMPL);}
	virtual STDMETHODIMP EnumVerbs (LPENUMOLEVERB FAR* ppenumOleVerb)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP Update () { return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP IsUpToDate () { return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP GetUserClassID (CLSID FAR* pClsid)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP GetUserType (DWORD dwFormOfType,
									  LPOLESTR FAR* pszUserType)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP SetExtent (DWORD dwDrawAspect, LPSIZEL lpsizel)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP GetExtent (DWORD dwDrawAspect, LPSIZEL lpsizel)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP Advise(LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP Unadvise(DWORD dwConnection)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP EnumAdvise (LPENUMSTATDATA FAR* ppenumAdvise)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP GetMiscStatus (DWORD dwAspect, DWORD FAR* pdwStatus)
				{ return ResultFromScode(E_NOTIMPL);}
    virtual STDMETHODIMP SetColorScheme (LPLOGPALETTE lpLogpal)
				{ return ResultFromScode(E_NOTIMPL);}
    
	operator LPOLEOBJECT() {return (LPOLEOBJECT) (LPUNKNOWN) getUnknown();}

protected:
	LPVCOMPONENTITEM	lpCDItem;
	
private:
	void		init(void);
};

// To DO: some of these methods should zero out their argument(s) in addition to
/// returning not implemented.
#define STD_IOLEOBJECT_METHODS() \
    virtual STDMETHODIMP SetClientSite(LPOLECLIENTSITE pClientSite) \
			{ return getOleObject().SetClientSite(pClientSite);} \
    virtual STDMETHODIMP GetClientSite (LPOLECLIENTSITE FAR* ppClientSite) \
			{ return getOleObject().GetClientSite (ppClientSite);} \
    virtual STDMETHODIMP SetHostNames (LPCOLESTR szContainerApp, LPCOLESTR szContainerObj) \
			{ return getOleObject().SetHostNames (szContainerApp, szContainerObj);} \
    virtual STDMETHODIMP Close (DWORD dwSaveOption) \
			{ return getOleObject().Close (dwSaveOption);} \
    virtual STDMETHODIMP SetMoniker (DWORD dwWhichMoniker, LPMONIKER pmk) \
			{ return getOleObject().SetMoniker (dwWhichMoniker, pmk);} \
    virtual STDMETHODIMP GetMoniker (DWORD dwAssign, DWORD dwWhichMoniker, \
									 LPMONIKER FAR* ppmk) \
			{ return getOleObject().GetMoniker (dwAssign, dwWhichMoniker, ppmk);} \
    virtual STDMETHODIMP InitFromData (LPDATAOBJECT pDataObject, \
                BOOL fCreation, \
                DWORD dwReserved) \
			{ return getOleObject().InitFromData (pDataObject, fCreation, dwReserved);} \
	virtual STDMETHODIMP GetClipboardData (DWORD dwReserved, \
										LPDATAOBJECT FAR* ppDataObject) \
			{ return getOleObject().GetClipboardData (dwReserved, ppDataObject);} \
	virtual STDMETHODIMP DoVerb (LONG iVerb, LPMSG lpmsg, \
	            			    LPOLECLIENTSITE pActiveSite, LONG lindex, \
	        			        HWND hwndParent, LPCRECT lprcPosRect) \
			{ return getOleObject().DoVerb (iVerb, lpmsg, \
                pActiveSite, lindex, hwndParent, lprcPosRect);} \
	virtual STDMETHODIMP EnumVerbs (LPENUMOLEVERB FAR* ppenumOleVerb) \
			{ return getOleObject().EnumVerbs (ppenumOleVerb);} \
    virtual STDMETHODIMP Update () { return getOleObject().Update ();} \
    virtual STDMETHODIMP IsUpToDate () { return getOleObject().IsUpToDate ();} \
    virtual STDMETHODIMP GetUserClassID (CLSID FAR* pClsid) \
			{ return getOleObject().GetUserClassID (pClsid);} \
    virtual STDMETHODIMP GetUserType (DWORD dwFormOfType, \
									  LPOLESTR FAR* pszUserType) \
			{ return getOleObject().GetUserType (dwFormOfType, pszUserType);} \
    virtual STDMETHODIMP SetExtent (DWORD dwDrawAspect, LPSIZEL lpsizel) \
			{ return getOleObject().SetExtent (dwDrawAspect, lpsizel);} \
    virtual STDMETHODIMP GetExtent (DWORD dwDrawAspect, LPSIZEL lpsizel) \
			{ return getOleObject().GetExtent (dwDrawAspect, lpsizel);} \
    virtual STDMETHODIMP Advise(LPADVISESINK pAdvSink, DWORD FAR* pdwConnection) \
			{ return getOleObject().Advise(pAdvSink, pdwConnection);} \
    virtual STDMETHODIMP Unadvise(DWORD dwConnection) \
			{ return getOleObject().Unadvise(dwConnection);} \
    virtual STDMETHODIMP EnumAdvise (LPENUMSTATDATA FAR* ppenumAdvise) \
			{ return getOleObject().EnumAdvise (ppenumAdvise);} \
    virtual STDMETHODIMP GetMiscStatus (DWORD dwAspect, DWORD FAR* pdwStatus) \
			{ return getOleObject().GetMiscStatus (dwAspect, pdwStatus);} \
    virtual STDMETHODIMP SetColorScheme (LPLOGPALETTE lpLogpal) \
			{ return getOleObject().SetColorScheme (lpLogpal);}

#define	STD_OleObject_METHODS() STD_IOLEOBJECT_METHODS()

#define STD_IOLEOBJECT_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, OleObject, access)

// These macros are embedded in my aggregate parent's class.
// It gives my aggregate parent the C++ API that would occur if inheritance 
// was used to access this interface.
//
// NOTE: This macro leaves the class inheritance in "protected" member
//		 access mode.
//		 The programmer is advised to follow this macro with a different 
//		 access mode if protected is not desired.
//

#define CONTAIN_IOLEOBJECT_INTERFACE() \
public: \
	STD_IOLEOBJECT_METHODS() \
	STD_IOLEOBJECT_ACCESS(*OleObject) \
protected: \
	COleObject		*OleObject;

#define INHERIT_IOLEOBJECT_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleObject) \
	STD_IOLEOBJECT_METHODS() \
	STD_IOLEOBJECT_ACCESS(*OleObject) \
protected: \
	COleObject		*OleObject;
                               
//**********************************************************************
//
// IOleObject Reference Class
//
// This class is
//		- a sub-object of an IOleObject object
//
// This is a convenience class that refers to an IOleObject Interface Pointer 
// (IP) that is given to you by another process. It adds minimal helpful
// functionality not found in OLE (inheritance, among others) to use an
// Interface Pointer.
//
// This class is NOT an IOleObject implementation, and hence cannot be used
// to override IOleObject methods. You must reference the IP contain within 
// to access IOleObject methods.
//
CLASS CROleObject : public CRUnknown
{
public:

	CROleObject(LPOLEOBJECT lpIOleObject);
	virtual ~CROleObject();
	
	CRUNKNOWN_INTERFACE(OleObject)
	
public:	
	LPTSTR			getClassName()		{return _T("CROleObject");}

	HRESULT			Cache(LPDATAOBJECT lpDataObj, CLIPFORMAT cfClipFormat, 
						  DWORD FAR *lpdwConnection);
	HRESULT			Cache(LPDATAOBJECT lpDataObj, CLIPFORMAT cfClipFormat);
						  
private:
	void			init(void);
};

//**********************************************************************
//
//  IOleObject Class
//
CLASS XOleObject : public IOleObject
{
public:

	XOleObject(COleObject *);
	~XOleObject();

	INHERIT_INTERFACE(IOLEOBJECT, OleObject)
};

//**********************************************************************
inline HRESULT	CROleObject::Cache(LPDATAOBJECT lpDataObj, CLIPFORMAT cfClipFormat)
{
	DWORD dwConnection;
	return Cache(lpDataObj, cfClipFormat, &dwConnection);
}

#endif
