// $Header:   Q:/views/win/vcs/ROOTSTRG.H_v   1.4   Mar 04 1997 14:13:46   gregm  $

//	CRRootStorage
//
//  Compound Root Storage Reference Class
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

#if !defined( _CRROOTSTORAGE_H_ )
#define _CRROOTSTORAGE_H_

// BASIC Views OLE types
#include "oledefs.h"

// Views GUI inlcudes
#include "pathstr.h"

// unknown includes
#include "unknown.h"

// Forward Declarations
CLASS CRRootStorage;
typedef	CRRootStorage FAR *LPCRROOTSTORAGE;

//**********************************************************************
//
// Root Storage Reference Class
//
CLASS CRRootStorage : public CRStorage
{
public:

	CRRootStorage(VString *lpStorageName = 0);
	~CRRootStorage();
	
	LPTSTR				getClassName() {return _T("CRRootStorage");}
        
	boolean				open();
	boolean				create();
	boolean				exists();
	boolean				remove();
	boolean				rename(const VPathString& dst);
	
	// notifications
	void				renamed(void);
	
private:
	void				init();
};

#endif // _CRROOTSTORAGE_H_
