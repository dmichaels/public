// $Header:   Q:/views/win/vcs/MONEY.CPv   1.3   Mar 04 1997 14:13:10   gregm  $

//	VMoney.cpp
//
//	VMoney class interface
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

#include "money.h"

defineClass(VMoney, VObject)

VMoney::VMoney()
{
}

VMoney& VMoney::operator=(const VMoney& rhs)
{
	copyFrom((VObject*) &rhs);

	return *this;
}

boolean	VMoney::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(VMoneyCls));
	VMoney*	source = (VMoney*) pObject;

	// Don't bother if an attempt is made to copy from yourself.
	// If there were pointer in this class, it could even be dangerous.
	if (this == source)
		return TRUE;

	// Copy source member values to this object here

	return TRUE;
}

#ifndef CV_NOARCHIVER

void VMoney::getFrom(VArchiver& a)
{
	VObject::getFrom(a);

	// Restore member variables here
}

void VMoney::putTo(VArchiver& a)
{
	VObject::putTo(a);

	// Persist member variables here
}

#endif
