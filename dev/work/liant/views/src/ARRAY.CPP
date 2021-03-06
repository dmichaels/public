// $Header:   Q:/views/win/vcs/ARRAY.CPv   1.6   Mar 04 1997 14:11:40   gregm  $

//	array.cpp
//
//	VArray class interface
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
//	02/06/96 dss	use quotes and not angle brackets.
//
// ----------------------------------------------------------------------------

#include "defs.h"
#include "cvassert.h"

#include "array.h"

defineClass(VArray, VObject)

VArray::VArray()
	: m_varType(VT_EMPTY)
	, m_pSafeArray(NULL)
{
}

VArray::VArray(VARTYPE varType, SAFEARRAY* pSafeArray)
	: m_varType(varType)
	, m_pSafeArray(pSafeArray)
{
}

VArray::~VArray()
{
	if(m_pSafeArray)
		SafeArrayDestroy(m_pSafeArray);
}

VArray& VArray::operator=(const VArray& rhs)
{
	copyFrom((VObject*) &rhs);

	return *this;
}

boolean VArray::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(VArrayCls));
	VArray*	source = (VArray*) pObject;

	// Don't bother if an attempt is made to copy from yourself.
	// If there were pointer in this class, it could even be dangerous.
	if (this == source)
		return TRUE;

	m_varType		= source->m_varType;
	if(source->m_pSafeArray)
	{
		if(m_pSafeArray)
			SafeArrayDestroy(m_pSafeArray);
		SafeArrayCopy(source->m_pSafeArray, &m_pSafeArray);
	}

	return TRUE;
}

void VArray::convertToSafeArray(VARTYPE* type, SAFEARRAY* pSafeArray)
{
	*type = m_varType;
	if(m_pSafeArray)
		SafeArrayCopy(m_pSafeArray, &pSafeArray);
}

void VArray::operator=(VARIANTARG* pVArg)
{
	m_varType = pVArg->vt;
	
	// Copy SafeArray
	if(m_varType & VT_BYREF)
	{
		if(pVArg->pparray)
			SafeArrayCopy(*(pVArg->pparray), &m_pSafeArray); 
	}
	else
	{
		if(pVArg->parray)
			SafeArrayCopy(pVArg->parray, &m_pSafeArray);
	} 
}
