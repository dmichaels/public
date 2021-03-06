// $Header:   Q:/views/win/vcs/cmpsite.cpv   1.7   Jun 03 1997 09:51:42   scottt  $

//	cmpcprg.cpp
//
//	VComponentProgrammable class interface
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

#include "defs.h"
#include "cvassert.h"
#include "archiver.h"
#include "cmpsite.h"

defineClass(VOleControlEvents, VObject)
									 
VOleControlEvents::VOleControlEvents()
	: m_pObject(0)
	
{
}

VOleControlEvents::VOleControlEvents(VComponentControl* pCompCtrl, LPOLEOBJECT pUnk)
{
	// RG: save VComponentControl pointer so events can be farmed out to callback function
	m_pObject = pUnk;
	// RG: save LPOLEOBJECT pointer so dispatch events can be had
	m_xEventSink.m_pCompCtrl = pCompCtrl;
	// RG: grab all events that are supported, then connect incoming sink
	GetEventIID(&m_iidEvents);
	m_dwEventSink = ConnectSink(m_iidEvents, &m_xEventSink);
}

// Uses the connection point and type information from pIDispatch to connect
// the default event sink and populate the eventList callback collection.
VOleControlEvents::VOleControlEvents(LPDISPATCH pIDispatch, VOrdCollect& eventList)
{
	// rhw 5/9/96: Ray... Finish me!!!!!!!!!!!!!!
}
VOleControlEvents::~VOleControlEvents()
{
	DisconnectSink(m_iidEvents, m_dwEventSink);
}

// fill the dictionary with the event names
void VOleControlEvents::getEventNames(LPTYPEINFO pEventInfo)
{
	BSTR eventName;
	UINT idMember;
    VComponentCallback *pEvtCbk; 
	LPTYPEATTR 	pTypeAttr;
	LPFUNCDESC	pEventDesc;
	pEventInfo->GetTypeAttr(&pTypeAttr);	

	for (idMember = 0;idMember < pTypeAttr->cFuncs;idMember++)	{
		if (SUCCEEDED(pEventInfo->GetFuncDesc(idMember,&pEventDesc))) {
			if (pEventDesc->wFuncFlags != FUNCFLAG_FRESTRICTED) {
				if (pEventInfo->GetDocumentation(pEventDesc->memid, &eventName,
										NULL, NULL, NULL) == S_OK) {
		    		pEvtCbk = new VComponentCallback(eventName,pEventDesc->memid,0,0);
    				m_xEventSink.m_pCompCtrl->m_eventList.add(pEvtCbk);
					SysFreeString(eventName);
				}
			}
			pEventInfo->ReleaseFuncDesc(pEventDesc);
		}
	}
	pEventInfo->ReleaseTypeAttr(pTypeAttr);
}

// RG: new begin
// connect to sink using ultra-convoluted Microsoft approach that looks to
// the future, but ignores our grief in the present
DWORD VOleControlEvents::ConnectSink(REFIID iid, LPUNKNOWN punkSink)
{
	cv_assert(m_pObject != NULL);

	LPCONNECTIONPOINTCONTAINER pConnPtCont;

	if (SUCCEEDED(m_pObject->QueryInterface(IID_IConnectionPointContainer,(LPVOID*)&pConnPtCont)))
	{
		cv_assert(pConnPtCont != NULL);
		LPCONNECTIONPOINT pConnPt = NULL;
		DWORD dwCookie;

		if (SUCCEEDED(pConnPtCont->FindConnectionPoint(iid, &pConnPt)))
		{
			cv_assert(pConnPt != NULL);
			pConnPt->Advise(punkSink, &dwCookie);
			pConnPt->Release();
		}

		pConnPtCont->Release();
		return dwCookie;
	}
	
	return 0;
}

// RG: critical to call on object destruction
void VOleControlEvents::DisconnectSink(REFIID iid, DWORD dwCookie)
{
	if (dwCookie == 0)
		return;

	cv_assert(m_pObject != NULL);

	LPCONNECTIONPOINTCONTAINER pConnPtCont;

	if ((m_pObject != NULL) &&
		SUCCEEDED(m_pObject->QueryInterface(IID_IConnectionPointContainer,(LPVOID*)&pConnPtCont)))
	{
		cv_assert(pConnPtCont != NULL);
		LPCONNECTIONPOINT pConnPt = NULL;

		if (SUCCEEDED(pConnPtCont->FindConnectionPoint(iid, &pConnPt)))
		{
			cv_assert(pConnPt != NULL);
			pConnPt->Unadvise(dwCookie);
			pConnPt->Release();
		}

		pConnPtCont->Release();
	}
}

BOOL VOleControlEvents::GetEventIID(IID* piid)
{
	*piid = GUID_NULL;

	//cv_assert(m_pObject->getOleObject() != NULL);
	cv_assert(m_pObject != NULL);

	// Use IProvideClassInfo2, if control supports it.
// Start of Sanjay's Code
/*
	LPPROVIDECLASSINFO2 pPCI2 = NULL;
	
	if (SUCCEEDED(m_pObject->QueryInterface(IID_IProvideClassInfo2,
		(LPVOID*)&pPCI2)))
	
	{
		cv_assert(pPCI2 != NULL);

		if (SUCCEEDED(pPCI2->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, piid)))
			cv_assert(!IsEqualIID(*piid, GUID_NULL));
		else
			cv_assert(IsEqualIID(*piid, GUID_NULL));

		pPCI2->Release();
	}
*/
// Start of old code
/*
	LPPROVIDECLASSINFO2 pPCI2 = NULL;
	
	if (SUCCEEDED(getOleObject().QueryInterface(IID_IProvideClassInfo2,
		(LPVOID*)&pPCI2)))
	
	{
		cv_assert(pPCI2 != NULL);

		if (SUCCEEDED(pPCI2->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, piid)))
			cv_assert(!IsEqualIID(*piid, GUID_NULL));
		else
			cv_assert(IsEqualIID(*piid, GUID_NULL));

		pPCI2->Release();
	}
*/
// End of Old code
// End of Sanjay's code	
	// Fall back on IProvideClassInfo, if IProvideClassInfo2 not supported.
	LPPROVIDECLASSINFO pPCI = NULL;

	if (IsEqualIID(*piid, GUID_NULL) &&
		//SUCCEEDED(m_pObject->getOleObject().QueryInterface(IID_IProvideClassInfo,
		SUCCEEDED(m_pObject->QueryInterface(IID_IProvideClassInfo,
		(LPVOID*)&pPCI)))
	{
		cv_assert(pPCI != NULL);

		LPTYPEINFO pClassInfo = NULL;

		if (SUCCEEDED(pPCI->GetClassInfo(&pClassInfo)))
		{
			cv_assert(pClassInfo != NULL);

			LPTYPEATTR pClassAttr;
			if (SUCCEEDED(pClassInfo->GetTypeAttr(&pClassAttr)))
			{
				cv_assert(pClassAttr != NULL);
				cv_assert(pClassAttr->typekind == TKIND_COCLASS);

				// Search for typeinfo of the default events interface.

				int nFlags;
				HREFTYPE hRefType;

				for (unsigned int i = 0; i < pClassAttr->cImplTypes; i++)
				{
					if (SUCCEEDED(pClassInfo->GetImplTypeFlags(i, &nFlags)) &&
						((nFlags & IMPLTYPE_MASK) == IMPLTYPE_DEFAULTSOURCE))
					{
						// Found it.  Now look at its attributes to get IID.

						LPTYPEINFO pEventInfo = NULL;

						if (SUCCEEDED(pClassInfo->GetRefTypeOfImplType(i,
								&hRefType)) &&
							SUCCEEDED(pClassInfo->GetRefTypeInfo(hRefType,
								&pEventInfo)))
						{
							cv_assert(pEventInfo != NULL);

							LPTYPEATTR pEventAttr;

							if (SUCCEEDED(pEventInfo->GetTypeAttr(&pEventAttr)))
							{
								cv_assert(pEventAttr != NULL);
								*piid = pEventAttr->guid;
								pEventInfo->ReleaseTypeAttr(pEventAttr);
							}

							pEventInfo->Release();
						}
						getEventNames(pEventInfo);

						break;
					}
				}

				pClassInfo->ReleaseTypeAttr(pClassAttr);
			}

			pClassInfo->Release();
		}

		pPCI->Release();
	}

	return (!IsEqualIID(*piid, GUID_NULL));
}

/////////////////////////////////////////////////////////////////////////////
// VOleControlEvents::XEventSink

STDMETHODIMP_(ULONG) VOleControlEvents::XEventSink::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) VOleControlEvents::XEventSink::Release()
{
	return 0;
}

STDMETHODIMP VOleControlEvents::XEventSink::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	//if (IID_IUnknown==iid || IID_IDispatch==iid)  //IsEqualIID(iid, *m_iidEvents)	)
	
	//{
		*ppvObj = this;
		AddRef();
		return S_OK;
	//} /
	//else
	//{
		//return E_NOINTERFACE;
	//}
}

STDMETHODIMP VOleControlEvents::XEventSink::GetTypeInfoCount(
	unsigned int*)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP VOleControlEvents::XEventSink::GetTypeInfo(
	unsigned int, LCID, ITypeInfo**)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP VOleControlEvents::XEventSink::GetIDsOfNames(
	REFIID, LPOLESTR*, unsigned int, LCID, DISPID*)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP VOleControlEvents::XEventSink::Invoke(
	DISPID dispid, REFIID, LCID, unsigned short wFlags,
	DISPPARAMS* pDispParams, VARIANT* pvarResult,
	EXCEPINFO* pExcepInfo, unsigned int* puArgError)
{
	//UNUSED(wFlags);

	//METHOD_PROLOGUE_EX(VOleControlEvents, EventSink)
	//cv_assert(pThis->m_pCtrlCont != NULL);
	//cv_assert(pThis->m_pCtrlCont->m_pWnd != NULL);
	//cv_assert(m_pCtrlCont != NULL);
	//cv_assert(m_pCtrlCont->m_pWnd != NULL);
	cv_assert(wFlags == DISPATCH_METHOD);

	// RG: package up any relevant information from the dispatch connection
	OCX_EVENT event(OCX_EVENT::event, dispid, pDispParams, pExcepInfo, puArgError);
	//m_pevent = event;
	// RG: similar to notify, but intended for events only
	UINT nID = 1;
	ProcessVEvent(nID, &event);
	
	if (pvarResult != NULL)
		VariantClear(pvarResult);

	//return event.m_hResult;
	return 0;
}

void VOleControlEvents::XEventSink::ProcessVEvent(UINT nID, OCX_EVENT* pVEvent)
{
	//IDC_SMILECTRL1                  1000
	//const AFX_EVENTSINKMAP_ENTRY* pEntry = GetEventSinkEntry(idCtrl, pEvent);
	
	// RG: run the callback pointer to this object!
	//m_pevent

	// BB Need to marshall parms into a standard object,
	// maybe use VMethodData

    DO(m_pCompCtrl->m_eventList,VComponentCallback,pCbk)
        if (pCbk->m_dispid == pVEvent->m_dispid) {
			if (pCbk->m_client != 0 && pCbk->m_callmeth !=0) {
            	pCbk->m_client->perform(pCbk->m_callmeth,(long)pVEvent);
            	return;
			}
        }
    END
    return;
    /*
    if (m_pCompCtrl->client && m_pCompCtrl->eventMthd != NIL_METHOD) {
        //m_pCompCtrl->client->perform(m_pCompCtrl->eventMthd, (long)this);
		m_pCompCtrl->client->perform(m_pCompCtrl->eventMthd, (long)pVEvent);
	}
    */
}
