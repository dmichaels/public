// $Header:   Q:/views/win/vcs/cmpcntl.cpv   1.10   Jun 03 1997 09:51:32   scottt  $

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
//	01/29/96 dss	Initial Revision.
//  06/04/96 evc    Added a new constructor
//  06/13/96 evc    When getting an LPOLEOBJECT from getOleObject()
//					we need to do a QueryInterface(IID_IOLEOBJECT
//					in init
//
// ----------------------------------------------------------------------------

#include "cmpcntl.h"
#include "cmpsite.h"

defineClass(VComponentCallback, VObject)

VComponentCallback::VComponentCallback()
{
    m_eventName = "";
    m_client = 0;
    m_dispid = 0;
    m_callmeth = 0;

}

VComponentCallback::VComponentCallback(VString name, DISPID dispId, VObject* client, method callback)
{
    m_eventName = name;
    m_client = client;
    m_dispid = dispId;
    m_callmeth = callback;
}

defineClass(VComponentControl, VComponentItem)

// ----------------------------------------------------------------------------
//
//
VComponentControl::VComponentControl()
{
	init();
}

// ----------------------------------------------------------------------------
//  Sanjay - Added new parameter to Constructors to differentiate between
//           ActiveX and non-ActiveX components for Archiving purposes.
//
VComponentControl::VComponentControl(const VString& objectType,
									 VComponentWindow& CDParent,
									 VWindow& win)
	: VComponentItem(objectType, CDParent, win, TRUE)
{
	init();
}
//----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
VComponentControl::VComponentControl(const VString& objectType,
									 VWindow& win)
	: VComponentItem(objectType, win, TRUE)
{
	init();
}


// ----------------------------------------------------------------------------
//
//
VComponentControl::VComponentControl(const VString& objectType,
									 const VFrame& frame,
									 VComponentWindow& CDParent,
									 const VStyle& style)
	: VComponentItem(objectType, frame, CDParent, style, TRUE)
{
	init();
}

// ----------------------------------------------------------------------------
//
//
VComponentControl::VComponentControl(const VPathString& storage,
									 VComponentWindow& CDParent,
									 VWindow& win,
									 int itemType)
	: VComponentItem(storage, CDParent, win, itemType, TRUE)
{
	init();
}

// ----------------------------------------------------------------------------
//
//
VComponentControl::VComponentControl(const VPathString& storage,
								     const VFrame& frame,
									 VComponentWindow& CDParent,
									 const VStyle& style,
									 int itemType)
	: VComponentItem(storage, frame, CDParent, style, itemType, TRUE)
{
	init();
}

// ----------------------------------------------------------------------------
//
//
VComponentControl::~VComponentControl()
{
	m_eventList.freeContents();
	if (m_programmable) {
		delete m_programmable;
	}
}

// ----------------------------------------------------------------------------
//
//
void VComponentControl::init(void)
{
	LPDISPATCH	lpDispatch = 0;

	if (hasOleObject()) {
		getOleObject().QueryInterface(IID_IDispatch, (LPVOID FAR*)&lpDispatch);
	}

	if (lpDispatch) {
		m_programmable = new VComponentProgrammable(lpDispatch);
		CROleObject& ref =  getOleObject();
		LPOLEOBJECT lpoleobject ;
		ref.QueryInterface(IID_IOleObject, (LPVOID FAR*)&lpoleobject);
		lpDispatch->Release();
		// RG: new begin
		VOleControlEvents* pSite = NULL;
		pSite = new VOleControlEvents(this, lpoleobject);
		//pSite = new VOleControlEvents(this);
		// RG: new end
	}
	else {
		m_programmable = 0;
	}


}

// Expose VProgrammable methods
boolean VComponentControl::setProperty(VString propertyName, VVariable value)
{
   return m_programmable->setProperty(propertyName,value);
}

boolean VComponentControl::getProperty(VString propertyName, VVariable value)
{
   return m_programmable->getProperty(propertyName,value);
}


boolean VComponentControl::setProperty(VMethodData& methodData)
{
   return m_programmable->setProperty(methodData);
}


boolean VComponentControl::getProperty(VMethodData& methodData)
{
   return m_programmable->getProperty(methodData);
}

boolean VComponentControl::invokeMethod(VMethodData& methodData)
{
   return m_programmable->invokeMethod(methodData);
}

VString VComponentControl::getErrorMsg()
{
   return m_programmable->getErrorMsg();
}

VStatus VComponentControl::getStatus()
{
   return m_programmable->getStatus();
}

// Return a collection of Event names as new VStrings
VOrdCollect	VComponentControl::getEvents()
{
	VOrdCollect list;
	DO(m_eventList,VComponentCallback,pEvent)
    	list.add(new VString(pEvent->m_eventName));
	END
	return list;
}

boolean VComponentControl::onEvent(VString eventName, VObject* client, method mth)
{
   DO(m_eventList,VComponentCallback,pEvent)
      if (pEvent->m_eventName == eventName) {
        pEvent->m_client = client;
        pEvent->m_callmeth = mth;
        return TRUE;
      }
   END
   return FALSE;
}


// RG: this is called to register the callback	
void VComponentControl::uponEvent(VObject *clnt, method mthd) 
{ 
    if (clnt) {
        client = clnt; 
    }
    if (mthd) {
        eventMthd = mthd; 
    }
    if (!clnt && mthd == NIL_METHOD) {
        client = NIL, eventMthd = NIL;
    }
}
