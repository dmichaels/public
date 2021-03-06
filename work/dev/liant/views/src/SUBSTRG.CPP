// $Header:   Q:/views/win/vcs/SUBSTRG.CPv   1.3   Mar 04 1997 14:14:02   gregm  $

//	CRSubStorage
//
//  Compound Sub-Storage Reference Class
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
//	4/20/94 dss	Initial release.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
//  CRSubStorage Interface
//
//**********************************************************************

//**********************************************************************
//
void CRSubStorage::init(void)
{
}

//**********************************************************************
//
CRSubStorage::CRSubStorage()
{
  	CDBG(	<< "In CRSubStorage::CRSubStorage(), this = " << this 
			<< endl);

	init();
}

//**********************************************************************
//
CRSubStorage::CRSubStorage(CRStorage &storageParent, VString *lpStorageName)
{
  	CDBG(	<< "In CRSubStorage::CRSubStorage(name = " << lpStorageName << ", parent = "
  			<< &storageParent << "), this = " << this << endl);

	init();

	setStorageParent(&storageParent);
	
	if (lpStorageName) {
		setStorageName(*lpStorageName);
	}
}

//**********************************************************************
//
CRSubStorage::~CRSubStorage()
{
}

