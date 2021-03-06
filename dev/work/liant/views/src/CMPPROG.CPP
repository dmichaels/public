// $Header:   Q:/views/win/vcs/cmpprog.cpv   1.11   21 May 1997 10:39:08   ALEXS  $

//	cmpcprg.cpp
//
//	VComponentClientProgrammable class interface
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
//	01/23/96 rhw	Initial Revision.
//	01/26/96 dss	cmpcprg.h changed to cmpprog.h.
//					Name change: VComponentProgrammable.
//	01/31/96 dss	added ctor that takes IDispatch.
//	02/06/96 dss	use quotes and not angle brackets.
//	05/09/96 rhw	Added event support
//
// ----------------------------------------------------------------------------

#include "defs.h"
#include "cvassert.h"
#include "archiver.h"

#include "cmpprog.h"
#include "cmpcntl.h"	// Has required VComponentCallback definition
#include "cmpsite.h"

defineClass(VComponentProgrammable, VObject)

#define MAX_PROGID 256
#define MAX_METHNAME 256

VComponentProgrammable::VComponentProgrammable()
	: m_pIDispatch(0)
	, m_isSinkConnected(FALSE)
{
	initializeObjectSystem();
}

VComponentProgrammable::VComponentProgrammable(const VString& programID,
							 const VLocale& locale,
							 boolean postponeServerCreation)
	: m_programID(programID)
	, m_locale(locale)
	, m_pIDispatch(0)
	, m_isSinkConnected(FALSE)
{
	initializeObjectSystem();
	
	if (m_isInitialized)
		initializeServer(postponeServerCreation);
}

VComponentProgrammable::VComponentProgrammable(IDispatch FAR *pDispatch,
							 const VLocale& locale)
	: m_programID(VString())
	, m_locale(locale)
	, m_pIDispatch(0)
	, m_isSinkConnected(FALSE)
{
	initializeObjectSystem();

	setIDispatch(pDispatch);
}

// Sanjay Start
VComponentProgrammable::VComponentProgrammable(const VComponentProgrammable& source)
	: m_programID(VString())
	, m_pIDispatch(0)
	, m_isSinkConnected(FALSE)
{
	initializeObjectSystem();
	copyFrom((VObject*)&source);
} 

VComponentProgrammable& VComponentProgrammable::operator=  (const VComponentProgrammable& source)
{	
	if (source == *this)
		return *this;
	if (m_pIDispatch && m_isInitialized)
	{
		m_eventList.freeContents();
		m_pIDispatch->Release();
	}
	copyFrom((VObject*)&source);

	return *this;
}

VComponentProgrammable* VComponentProgrammable::operator=  (const VComponentProgrammable* source)
{	
	if (source == this)
		return this;
	if (m_pIDispatch && m_isInitialized)
	{
		m_eventList.freeContents();
		m_pIDispatch->Release();
	}
	copyFrom((VObject*)source);

	return this;
}
// Sanjay End

VComponentProgrammable::~VComponentProgrammable()
{
	// Don't bother the OLE runtime if it never got initialized for whatever
	// reason.
	if (m_pIDispatch && m_isInitialized)
	{
		m_eventList.freeContents();
		m_pIDispatch->Release();
	}
	
	uninitializeObjectSystem();
}

boolean VComponentProgrammable::setProperty(VString propertyName, VVariable value)
{
	// If OLE could not initialize, bail
	if (!m_isInitialized)
		return FALSE;

	VMethodData methodData(propertyName);

	methodData.setProperty(value);
	
	if (value.isReference())
		return invoke(methodData, INVOKE_PROPERTYPUTREF);
	else
		return invoke(methodData, INVOKE_PROPERTYPUT);
}

boolean VComponentProgrammable::getProperty(VString propertyName, VVariable value)
{
	// If OLE could not initialize, bail
	if (!m_isInitialized)
		return FALSE;

	VMethodData methodData(propertyName);

	methodData.getProperty(value);
	
	return invoke(methodData, INVOKE_PROPERTYGET);
}

boolean VComponentProgrammable::setProperty(VMethodData& methodData)
{
	// If OLE could not initialize, bail
	if (!m_isInitialized)
		return FALSE;

	return invoke(methodData, INVOKE_PROPERTYPUT);
}

boolean VComponentProgrammable::getProperty(VMethodData& methodData)
{
	// If OLE could not initialize, bail
	if (!m_isInitialized)
		return FALSE;

	return invoke(methodData, INVOKE_PROPERTYGET);
}

boolean VComponentProgrammable::invokeMethod(VMethodData& methodData)
{
	// If OLE could not initialize, bail
	if (!m_isInitialized)
		return FALSE;

	return invoke(methodData, INVOKE_FUNC);
}

boolean VComponentProgrammable::invoke(VMethodData& methodData, INVOKEKIND invocationType)
{
	DISPID methodID;
	OLECHAR* pwMethodName = (OLECHAR*) methodData.getMethodName().getwcs();

	// Only create dispatch once and only if server creation was postponed
	// or a dispatch was not set using a call to setDispatch()
	if (!m_pIDispatch && !createServer())
		return FALSE;		// Bail since we can't get the disp interface

	// rhw unfinished: use VLocale later...
	m_status = m_pIDispatch->GetIDsOfNames(IID_NULL, &pwMethodName, 1, 0x409, &methodID);

	if (m_status.isSuccess())
		// This is it!  This actually invokes the method specified by methodData
		m_status = m_pIDispatch->Invoke(methodID,
										IID_NULL,
										0x409,		// Later, replace with VLocale
										invocationType,
										methodData.getDispparms(invocationType),
										methodData.getReturnVariantArg(),
										NULL /* exception Info struct */,
										NULL /* index of parameter type mismatch */);

	// Update return variable and any non-native by-reference variables
	methodData.updateVariables();

	return m_status.isSuccess();
}

boolean VComponentProgrammable::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(VComponentProgrammableCls));
	VComponentProgrammable*	source = (VComponentProgrammable*) pObject;

	// Don't bother if an attempt is made to copy from yourself.
	// If there were pointer in this class, it could even be dangerous.
	if (source == this)
		return TRUE;
	
	VObject::copyFrom(source);

	m_programID		= source->m_programID;
	m_locale		= source->m_locale;
	m_clsID			= source->m_clsID;
	m_pIDispatch	= source->m_pIDispatch;
	// Account for this new reference to the source's IDispatch interface
	if (m_pIDispatch)
		m_pIDispatch->AddRef();
	m_status		= source->m_status;
	// m_isInitialized is set in the constructor so don't worry about it here

	return TRUE;
}

#ifndef CV_NOARCHIVER
void VComponentProgrammable::getFrom(VArchiver& a)
{
	VObject::getFrom(a);

	a.getObject(&m_programID);
	a.getObject(&m_locale);	

	initializeServer(TRUE /* Postpones server creation */);
}

void VComponentProgrammable::putTo(VArchiver& a)
{
	VObject::putTo(a);

	// If this assert fails, then there is no hope of being able to perform
	// a valid restore later (using getFrom).  The problem is that for this
	// object to work correctly, we have to have a valid m_IDispatch.  We can't
	// persist IDispatch since its a pointer to something that must always be
	// created dynamically (using the OLE runtime).  The only way for this
	// class to create an IDispatch is by using its persisted program ID
	// (m_proramID).  If this assert fails, then m_programID has no value and
	// we must protest.
	// One way for this situation to occur is if this object was the return
	// value of some VComponentProgrammable method invokation (or getProperty), but
	// the IDispatch in question does not return an ITypeInfo interface
	// from GetTypeInfo which means that there is no way to get its progID.
	cv_assert(m_programID != "");

	a.putObject(&m_programID);
	a.putObject(&m_locale);	
}
#endif

boolean VComponentProgrammable::createServer()
{
	m_pIDispatch	= 0;

	CLSIDFromProgID(m_programID, &m_clsID);

	// Create the instance of the server
	m_status = CoCreateInstance(m_clsID,
								0 /* No aggregation */,
								CLSCTX_INPROC_SERVER |
//								CLSCTX_INPROC_HANDLER |
								CLSCTX_LOCAL_SERVER,
								IID_IDispatch,
								(LPVOID*)&m_pIDispatch);

	return m_status.isSuccess();
}

void VComponentProgrammable::initializeServer(boolean postponeServerCreation)
{
	// Set these in case server creation is being postponed
	m_pIDispatch	= 0;
	m_status		= NOERROR;

	if (!postponeServerCreation)
		createServer();
}

void VComponentProgrammable::initializeObjectSystem()
{
	// Multiple calls to this API cause no harm and simply need to be balanced
	// with a call to OleUninitialize().  This approach assures that OLE will
	// still be active so that this object can safely release its OLE resources.
	m_status = OleInitialize(NULL);

	m_isInitialized = SUCCEEDED(m_status);

	cv_assert(m_isInitialized);
}

void VComponentProgrammable::uninitializeObjectSystem()
{
	// If OLE was successfully initialized, then uninitialize it
	if (m_isInitialized)
	{
		OleUninitialize();
		m_isInitialized = FALSE;
	}
}

VString VComponentProgrammable::getErrorMsg()
{
	VString errorMsg("The operation completed successfully.\r\n");

	// If the state of this object is failure, return a text message
	// describing error.
	if (m_status.isFailure())
	{
#if defined(CV_WIN32)
		LPVOID lpMessageBuffer;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					  NULL,
					  m_status,
					  0x409,	// Use VLocale later
					  (LPTSTR) &lpMessageBuffer,
					  0,
					  NULL);

		errorMsg.puts((LPTSTR) lpMessageBuffer);

		LocalFree(lpMessageBuffer);
#else
	errorMsg = "The operation failed. Details are unavailable.\r\n";
#endif
	}

	return errorMsg;
}

// Return a collection of VStrings representing event names.  Caller is
// responsible for calling VOrdCollect::freeContents() to free list.
VOrdCollect	VComponentProgrammable::getEvents() 
{
	VOrdCollect list;
	DO(m_eventList,VComponentCallback,pEvent)
		list.add(new VString(pEvent->m_eventName));
	END

	return list;
}

// Register callback method, mth of instance client, for event, eventName
boolean VComponentProgrammable::onEvent(VString eventName, VObject* client, method mth)
{
	// We only want to establish a sink connection if someone is interested in
	// establishing a callback for some event and we only want to do it once.
	if (!m_isSinkConnected)
	{
		// Only create dispatch once and only if server creation was postponed
		// or a dispatch was not set using a call to setDispatch()
		if (!m_pIDispatch && !createServer())
			return FALSE;		// Bail since we can't get the disp interface

		// Connect the event sink in preparation to register callback later
		VOleControlEvents(m_pIDispatch, m_eventList);

		m_isSinkConnected = TRUE;
	}

	DO(m_eventList,VComponentCallback,pEvent)
		if (pEvent->m_eventName == eventName)
		{
			pEvent->m_client = client;
			pEvent->m_callmeth = mth;

			return TRUE;
		}
	END

	return FALSE;
}

boolean VComponentProgrammable::setIDispatch(LPDISPATCH dispatch)
{
	LPTYPEINFO 	pITypeInfo	= 0;
	LPTYPEATTR	pTypeAttr	= 0;
	LPOLESTR	pProgID		= 0;
	LPMALLOC	pIMalloc	= 0;

	m_programID = "";
//	m_locale = default;  Figure out what a good default would be

	// If we already had a dispatch, let it go
	if (m_pIDispatch)
		m_pIDispatch->Release();

	m_pIDispatch = dispatch;

	if(!m_pIDispatch)
		return TRUE;

	// Have to account for extra reference created by previous statement
	dispatch->AddRef();

	// Prepare to set this objects prog ID to that of dispatch's
	m_status = dispatch->GetTypeInfo(0, 	// reserved by MS for later use
						  			 0x409, // replace later with VLocale
						  			 &pITypeInfo);
		
	// If this failed, bail
	if (m_status.isFailure())
		return FALSE;
	
	m_status = pITypeInfo->GetTypeAttr(&pTypeAttr);

	// If this failed, bail
	if (m_status.isFailure())
	{
		pITypeInfo->Release();
		return FALSE;
	}
	
	// rhw unfinished: m_locale = pTypeAttr->lcid;  (0x409) Fix this later!  
	// Need to figure out how to convert lcid to VLocale setting
	m_status = ProgIDFromCLSID(pTypeAttr->guid, &pProgID);

	if (m_status.isFailure())
	{
		pITypeInfo->ReleaseTypeAttr(pTypeAttr);
		pITypeInfo->Release();
		return FALSE;
	}
		
	if (pProgID)
		m_programID.puts((VWideChar *)pProgID);

	// Free prog ID
	m_status = CoGetMalloc(MEMCTX_TASK, &pIMalloc);
	if (m_status.isSuccess())
		pIMalloc->Free((LPVOID)pProgID);

	pITypeInfo->ReleaseTypeAttr(pTypeAttr);
	pITypeInfo->Release();
	
	return TRUE;
}

LPDISPATCH VComponentProgrammable::getIDispatch()
{
	if (m_pIDispatch)
		m_pIDispatch->AddRef();

	return m_pIDispatch;
}
