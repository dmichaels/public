// $Header:   Q:/views/win/vcs/cmpsite.h_v   1.6   Jun 03 1997 09:51:42   scottt  $

//	cmpprog.h
//
//	VOleControlEvents class interface
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
//	4/23/96 REG	Initial Version.
//
// ----------------------------------------------------------------------------

#ifndef VOleControlEvents_H
#define VOleControlEvents_H

#include "cmpcntl.h"
#include <stdarg.h>		// This include needs to be made portable

#include "object.h"
#include "str.h"
#include "vlocale.h"
#include "methdata.h"
#include "variable.h"
#include "status.h"

// Start of Sanjay's Code
//#include <olectlid.h>
//#include <ocidl.h>
// End of Sanjay's code

typedef IProvideClassInfo FAR* LPPROVIDECLASSINFO;
// Start of Sanjay's Code
//typedef IProvideClassInfo2 FAR* LPPROVIDECLASSINFO2;
// Old code
//typedef IProvideClassInfo2 FAR* LPPROVIDECLASSINFO2;
// End of Old Code
// End of Sanjay's code
#define IMPLTYPE_MASK \
	(IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE | IMPLTYPEFLAG_FRESTRICTED)

#define IMPLTYPE_DEFAULTSOURCE \
	(IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE)

struct OCX_EVENT
{
        enum { event, propRequest, propChanged };

        OCX_EVENT(int eventKind, DISPID dispid, DISPPARAMS* pDispParams = NULL,
                EXCEPINFO* pExcepInfo = NULL, UINT* puArgError = NULL);

        int m_eventKind;
        DISPID m_dispid;
        DISPPARAMS* m_pDispParams;
        EXCEPINFO* m_pExcepInfo;
        UINT* m_puArgError;
        BOOL m_bPropChanged;
        HRESULT m_hResult;
};

inline OCX_EVENT::OCX_EVENT(int eventKind, DISPID dispid,
        DISPPARAMS* pDispParams, EXCEPINFO* pExcepInfo, UINT* puArgError)
{
        m_eventKind = eventKind;
        m_dispid = dispid;
        m_pDispParams = pDispParams;
        m_pExcepInfo = pExcepInfo;
        m_puArgError = puArgError;
        m_hResult = NOERROR;
}
enum OCX_DISPMAP_FLAGS
{
        afxDispCustom = 0,
        afxDispStock = 1
};

struct OCX_DISPMAP_ENTRY
{
        LPCTSTR lpszName;       // member/property name
        long lDispID;           // DISPID (may be DISPID_UNKNOWN)
        LPCSTR lpszParams;      // member parameter description
        WORD vt;                // return value type / or type of property
        //OCX_PMSG pfn;           // normal member On<membercall> or, OnGet<pro»
        //OCX_PMSG pfnSet;        // special member for OnSet<property>
		DWORD* pfn;           // normal member On<membercall> or, OnGet<pro»
        DWORD* pfnSet;        // special member for OnSet<property>
        size_t nPropOffset;     // property offset
        OCX_DISPMAP_FLAGS flags;// flags (e.g. stock/custom)
};

struct OCX_EVENTSINKMAP_ENTRY
{
        OCX_DISPMAP_ENTRY dispEntry;
        UINT nCtrlIDFirst;
        UINT nCtrlIDLast;
};

CLASS VOleControlEvents : public VObject
{	  
public:

					VOleControlEvents();
					VOleControlEvents(VComponentControl* pCompCtrl, LPOLEOBJECT pUnk);
					VOleControlEvents(LPDISPATCH pIDispatch, VOrdCollect& eventList);
	virtual 		~VOleControlEvents();
	virtual VClass*	iam();

	// RG: member functions
	DWORD 			ConnectSink(REFIID iid, LPUNKNOWN punkSink); 
	void 			DisconnectSink(REFIID iid, DWORD dwCookie);
	void 			uponEvent(VObject *clnt, method mthd); 
	void			getEventNames(LPTYPEINFO pEventInfo);
	BOOL 			GetEventIID(IID* piid); // RG: new

	// RG: class variables
	DWORD			m_dwEventSink;
	IID 			m_iidEvents;
	LPOLEOBJECT 	m_pObject;

	//VComponentControl* 	m_pCompCtrl;
	
	class XEventSink : public IDispatch
    {
    public:
		// RG: class variables
        VOleControlEvents* 				m_pParent;
		VComponentControl* 				m_pCompCtrl;
		
		// RG: member functions
		virtual STDMETHODIMP 			QueryInterface(REFIID riid, LPVOID FAR* ppvObj);
		virtual STDMETHODIMP_(ULONG) 	AddRef();
		virtual STDMETHODIMP_(ULONG) 	Release();
		STDMETHODIMP 					GetTypeInfoCount(unsigned int*);
		STDMETHODIMP 					GetTypeInfo(unsigned int, LCID, ITypeInfo**);
		STDMETHODIMP 					GetIDsOfNames(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		STDMETHODIMP 					Invoke(DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);
		void 							ProcessVEvent(UINT nID, OCX_EVENT* pVEvent);
    } m_xEventSink;

// methods
protected:

// data
protected:

// methods
private:

};

#endif	// VOleControlEvents_H
