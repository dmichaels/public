// $Header:   Q:/views/win/vcs/STATUS.CPv   1.3   Mar 04 1997 14:13:54   gregm  $

//	status.cpp
//
//	VStatus class interface
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
#include "archiver.h"

#include "status.h"

defineClass(VStatus, VObject)

VStatus::VStatus()
	: m_statusCode(0)
{
}

VStatus::VStatus(const VStatus& status)
{
	copyFrom((VObject*)&status);
}

VStatus::VStatus(const long errorCode)
	: m_statusCode(errorCode)
{
}

VString VStatus::getErrorMsg()
{
	VString	result;
	LPVOID	lpMessageBuffer;

#if defined(CV_WIN32)
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,
				  m_statusCode,
				  0x409,	// Later, replace with VLocale
				  (LPTSTR) &lpMessageBuffer,
				  0,
				  NULL);
	
	result = (LPTSTR) lpMessageBuffer;

	LocalFree(lpMessageBuffer);
#else
	result = "The operation failed. Details are unavailable.\r\n";
#endif

	return result;
}

VStatus& VStatus::operator=(const VStatus& rhs)
{
	copyFrom((VObject*) &rhs);

	return *this;
}

boolean VStatus::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(VStatusCls));
	VStatus*	source = (VStatus*) pObject;

	// Don't bother if an attempt is made to copy from yourself.
	// If there were pointer in this class, it could even be dangerous.
	if (this == source)
		return TRUE;

	m_statusCode	= source->m_statusCode;

	return TRUE;
}

#ifndef CV_NOARCHIVER
void VStatus::getFrom(VArchiver& a)
{
	VObject::getFrom(a);

	a >> m_statusCode;
}

void VStatus::putTo(VArchiver& a)
{
	VObject::putTo(a);

	a << m_statusCode;
}
#endif
