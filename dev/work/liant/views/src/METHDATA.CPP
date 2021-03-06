// $Header:   Q:/views/win/vcs/methdata.cpv   1.3   Jul 30 1996 16:18:58   GREGM  $

//	methdata.cpp
//
//	VMethodData class interface
//
//	Allegris Foundation 1.0.00 for Microsoft Windows
//	Copyright (c) 1996 by INTERSOLV, Inc.
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
//	02/06/96 dss	use quotes and not angle brackets.
//
// ----------------------------------------------------------------------------

#include "defs.h"
#include "cvassert.h"
#include "archiver.h"
#include "iterator.h"

#include "methdata.h"

defineClass(VMethodData, VObject)

VMethodData::VMethodData()
	: m_pReturnVariant(0)
	, m_pDisp(0)
	, m_pVParms(0)
	, m_pNamedArgs(0)
{
	initializeObjectSystem();
}

VMethodData::VMethodData(const VString& methodName)
	: m_pReturnVariant(0)
	, m_pDisp(0)
	, m_pVParms(0)
	, m_pNamedArgs(0)
	, m_methodName(methodName)
{
	initializeObjectSystem();
}

VMethodData::~VMethodData()
{
	releaseEverything();

	uninitializeObjectSystem();
}

VMethodData& VMethodData::operator<<(VVariable param)
{
	VVariable* pVariable = new VVariable;

	pVariable->copyFrom(&param);

	m_parms.add(pVariable);

	return *this;
}

VMethodData& VMethodData::operator=(const VMethodData& rhs)
{
	copyFrom((VObject*) &rhs);

	return *this;
}

void VMethodData::setMethodName(const VString& methodName)
{
	m_methodName = methodName;
}

void VMethodData::setReturnVariable(VVariable returnVariable)
{
	m_returnVariable = returnVariable;
}

boolean VMethodData::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(VMethodDataCls));
	VMethodData*	source = (VMethodData*) pObject;

	// Don't bother if an attempt is made to copy from yourself.
	// If there were pointer in this class, it could even be dangerous.
	if (source == this)
		return TRUE;
	
	VObject::copyFrom(source);

	releaseEverything();

	// Copy source members
	m_parms				= source->m_parms;
	m_methodName		= source->m_methodName;
	m_returnVariable	= source->m_returnVariable;
	// m_isInitialized is set in the constructor so don't worry about it here

	return TRUE;
}

#ifndef CV_NOARCHIVER
void VMethodData::getFrom(VArchiver& a)
{
	VObject::getFrom(a);

	a.getObject(&m_parms);
	a.getObject(&m_methodName);
	a.getObject(&m_returnVariable);
	m_pReturnVariant	= 0;
	m_pDisp				= 0;
	m_pVParms			= 0;
	m_pNamedArgs		= 0;
}

void VMethodData::putTo(VArchiver& a)
{
	VObject::putTo(a);

	a.putObject(&m_parms);
	a.putObject(&m_methodName);
	a.putObject(&m_returnVariable);
}
#endif

VString& VMethodData::getMethodName()
{
	return m_methodName;
}

VARIANTARG* VMethodData::getReturnVariantArg()
{
	cv_assert(m_isInitialized);

	releaseReturnVariantArg();

	m_pReturnVariant = new VARIANTARG;

	// OLE Initialize the VARIANTARG
	VariantInit(m_pReturnVariant);

	// Have the client's return variable supply the appropriate setting
	// for VARIANTARG
	m_returnVariable.getVariantArg(m_pReturnVariant);

	return m_pReturnVariant;
}

DISPPARAMS* VMethodData::getDispparms(INVOKEKIND invocationType)
{
	boolean isPropertyPut = invocationType == INVOKE_PROPERTYPUT || 
							invocationType == INVOKE_PROPERTYPUTREF;
	int	maxParms = m_parms.count();

	cv_assert(m_isInitialized);

	// Remove anything leftover from a previous call to this method
	if (m_pDisp)
		delete m_pDisp;
	if (m_pNamedArgs)
	{
		delete m_pNamedArgs;
		m_pNamedArgs = 0;
	}
	releaseVParms();

	// Reallocate
	m_pDisp = new DISPPARAMS;
	m_pVParms = new VARIANTARG[maxParms];
	if (isPropertyPut)
		m_pNamedArgs = new DISPID[maxParms];

	// Load up all the parameters
	for (int i = 0; i < maxParms; i++)
	{
		VariantInit(&m_pVParms[i]);

		// OLE needs to see the paramters in reverse order so use the last 
		// parameter to load the first vparms
		((VVariable*)m_parms.idAt(maxParms - i - 1))->getVariantArg(&m_pVParms[i]);

		if (isPropertyPut)
			m_pNamedArgs[i] = DISPID_PROPERTYPUT;
	}

	// Initialize the disp parms structure
	m_pDisp->rgvarg				= m_pVParms;
	m_pDisp->cArgs				= maxParms;

	if (isPropertyPut)
	{
		// For property puts, each argument has to be named by DISPID_PROPERTYPUT
		// This is the standard OLE DISPID for property puts
		m_pDisp->rgdispidNamedArgs	= m_pNamedArgs;
		m_pDisp->cNamedArgs			= maxParms;
	}
	else
	{
		m_pDisp->rgdispidNamedArgs	= NULL;
		m_pDisp->cNamedArgs			= 0;
	}

	return m_pDisp;
}

void VMethodData::releaseVParms()
{
	cv_assert(m_isInitialized);

	if (m_pVParms && m_isInitialized)
	{
		int maxParms = m_parms.count();

		// Have to allow VARIANTARGs to release their resources
		for (int i = 0; i < maxParms; i++)
			VariantClear(&m_pVParms[i]);

		delete m_pVParms;

		m_pVParms = 0;
	}
}

void VMethodData::releaseReturnVariantArg()
{
	cv_assert(m_isInitialized);

	if (m_pReturnVariant && m_isInitialized)
	{
		VariantClear(m_pReturnVariant);

		delete m_pReturnVariant;

		m_pReturnVariant = 0;
	}
}

void VMethodData::releaseEverything()
{
	releaseVParms();
	releaseReturnVariantArg();

	m_parms.freeContents();

	if (m_pDisp)
	{
		delete m_pDisp;
		m_pDisp = 0;
	}

	if (m_pNamedArgs)
	{
		delete m_pNamedArgs;
		m_pNamedArgs = 0;
	}
}

void VMethodData::setProperty(VVariable& param)
{
	VVariable* pVariable = new VVariable;

	pVariable->copyFrom(&param);

	m_parms.add(pVariable);
}

void VMethodData::getProperty(VVariable& param)
{
	m_returnVariable = param;
}

// Update return variable and any non-native by-reference variables
void VMethodData::updateVariables()
{
	VIterator	parmItr(m_parms);
	VVariable*	pVVariable;
	int			i;
	int			end = m_parms.count() - 1;		// Index of last param

	// Update any client type that are non-native to OLE like VString and
	// VDateTime.
	// Note:  (rhw unfinished:) Currently, this code is not optimal
	// in that it does not discriminate between native and non-native types
	// when it updates them.  Later, we should add code to discriminate and
	// only update the non-native types.  The native variables have already
	// been updated (if appropriate) by the automation method called so
	// updating them here amounts to assigning the same value over itself.	
	for (i = 0, parmItr.start(); pVVariable = (VVariable*) parmItr++; i++)
	{
		if (pVVariable->isReference())
			*pVVariable = &m_pVParms[end - i];
	}

	// Update the return variable
	m_returnVariable = m_pReturnVariant;
}

void VMethodData::initializeObjectSystem()
{
	// Multiple calls to this API cause no harm and simply need to be balanced
	// with a call to OleUninitialize().  This approach assures that OLE will
	// still be active so that this object can safely release its OLE resources.
	m_isInitialized = SUCCEEDED(OleInitialize(NULL));

	cv_assert(m_isInitialized);
}

void VMethodData::uninitializeObjectSystem()
{
	// If OLE was successfully initialized, then balance out by uninitialize
	if (m_isInitialized)
	{
		OleUninitialize();
		m_isInitialized = FALSE;
	}
}
