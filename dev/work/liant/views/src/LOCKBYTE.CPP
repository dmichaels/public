// $Header:   Q:/views/win/vcs/lockbyte.cpv   1.4   Mar 21 1997 08:42:50   gregm  $

//	CLockBytes
//
//  OLE ILockBytes Implementation
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
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
XLockBytes::XLockBytes(CLockBytes *pLockBytes)
	: LockBytes( pLockBytes)
{
	CDBG(	<< "Constructing ILockBytes (" << this 
			<< ") for CLockBytes (" << pLockBytes << ')' << endl);
}

//**********************************************************************
XLockBytes::~XLockBytes()
{
}

//**********************************************************************
//
void CLockBytes::init(void)
{
	CoFileTimeNow(&accessTime);
	
	// all three times the same: do NOT call CoFileTimeNow() 3 times!
	modificationTime = creationTime = accessTime;

	exposeInterface(IID_ILockBytes, "CLockBytes");
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
CLockBytes::CLockBytes()
	: CUnknown(new XLockBytes(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In CLockBytes default ctor, this = " << this
			<< endl);
			
	init();
}

//**********************************************************************
//
CLockBytes::~CLockBytes()
{
	CDBG(	<< "In CLockBytes dtor" << endl);
}

