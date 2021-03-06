// $Header:   Q:/views/win/vcs/SUBSTRG.H_v   1.4   Mar 04 1997 14:14:04   gregm  $

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

#if !defined( _CRSUBSTORAGE_H_ )
#define _CRSUBSTORAGE_H_

// BASIC Views OLE types
#include "oledefs.h"

// Views GUI inlcudes
#include "pathstr.h"

// unknown includes
#include "unknown.h"

// Forward Declarations
CLASS CRSubStorage;
typedef	CRSubStorage FAR *LPCRSUBSTORAGE;

CLASS CRSubStorage : public CRStorage
{
public:
	CRSubStorage();
	CRSubStorage(CRStorage &storageParent, VString *lpStorageName = 0);
	~CRSubStorage();
	
	LPTSTR			getClassName() {return _T("CRSubStorage");}

private:
	void				init(void);
};

#endif // _CRSUBSTORAGE_H_
