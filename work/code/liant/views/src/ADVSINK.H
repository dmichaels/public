// $Header:   Q:/views/win/vcs/ADVSINK.H_v   1.7   Mar 04 1997 14:11:32   gregm  $

//	CAdviseSink
//
//  OLE IAdviseSink2 Implementation
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
//	01/30/96 dss	Always include unknown.h. Pretty up.
//	---------------------------------------------------------------------------

#ifndef	CADVISESINK_H
#define CADVISESINK_H

#include "unknown.h"

CLASS VComponentItem;

// Note: this is an IAdviseSink2 class
CLASS CAdviseSink : public CUnknown
{
public:

	CAdviseSink(VComponentItem& refCDItem);
	virtual ~CAdviseSink();

	LPTSTR				getClassName() {return _T("CAdviseSink");}

	// *** Inherited methods (no ';' follows these macros) ***
//	INHERIT_INTERFACE(IUNKNOWN, Unknown)

public:

	// *** IAdviseSink methods ***
	virtual STDMETHODIMP_(void) OnDataChange (FORMATETC FAR* pFormatetc, STGMEDIUM FAR* pStgmed);
	virtual STDMETHODIMP_(void) OnViewChange (DWORD dwAspect, LONG lindex);
	virtual STDMETHODIMP_(void) OnRename (LPMONIKER pmk);
	virtual STDMETHODIMP_(void) OnSave ();
	virtual STDMETHODIMP_(void) OnClose ();

    // *** IAdviseSink2 methods ***
    virtual STDMETHODIMP_(void) OnLinkSrcChange(LPMONIKER pmk);

	// cast operators to equate Cxxx class to Ixxx class
	operator 		LPADVISESINK() {return (LPADVISESINK) (LPUNKNOWN) getUnknown();}

protected:
	VComponentItem	FAR *lpCDItem;
};

#define STD_IADVISESINK_METHODS() \
	virtual STDMETHODIMP_(void) OnDataChange(FORMATETC FAR* pFormatetc, STGMEDIUM FAR* pStgmed) \
						{ getAdviseSink().OnDataChange(pFormatetc, pStgmed);} \
	virtual STDMETHODIMP_(void) OnViewChange(DWORD dwAspect, LONG lindex) \
						{ getAdviseSink().OnViewChange(dwAspect, lindex);} \
	virtual STDMETHODIMP_(void) OnRename(LPMONIKER pmk) \
						{ getAdviseSink().OnRename(pmk);} \
	virtual STDMETHODIMP_(void) OnSave() \
						{ getAdviseSink().OnSave();} \
	virtual STDMETHODIMP_(void) OnClose() \
						{ getAdviseSink().OnClose();} \
    virtual STDMETHODIMP_(void) OnLinkSrcChange(LPMONIKER pmk) \
						{ getAdviseSink().OnLinkSrcChange(pmk);}

// Equivalent
#define STD_AdviseSink_METHODS()	STD_IADVISESINK_METHODS()

#define STD_IADVISESINK_ACCESS(access) \
		CONTAIN_ACCESS2( C, I, AdviseSink, *AdviseSink)

#define INHERIT_IADVISESINK_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*AdviseSink) \
	STD_IADVISESINK_METHODS() \
	STD_IADVISESINK_ACCESS(*AdviseSink) \
protected: \
	CAdviseSink		*AdviseSink;

#define CONTAIN_IADVISESINK_INTERFACE() \
public: \
	STD_IADVISESINK_METHODS() \
	STD_IADVISESINK_ACCESS(*AdviseSink) \
protected: \
	CAdviseSink		*AdviseSink;

//**********************************************************************
//
//  IAdviseSink Class
//
CLASS XAdviseSink : public IAdviseSink2
{
public:
	XAdviseSink(CAdviseSink *);
	~XAdviseSink();

	INHERIT_INTERFACE(IADVISESINK, AdviseSink)
};

#endif // CADVISESINK_H
