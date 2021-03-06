// $Header:   Q:/views/win/vcs/ROOTSTRG.CPv   1.4   Mar 04 1997 14:13:46   gregm  $

//	CRRootStorage
//
//	Compound Root Storage Reference Class
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
//	04/20/94	dss	Initial release.
//	12/04/95	dss	Added PVCS header. Migrated to LPCOLESTR.
//	---------------------------------------------------------------------------

#include "pre.h"

#include <errno.h>

//**********************************************************************
//
//  CRRootStorage Interface
//
//**********************************************************************

//**********************************************************************
//
void CRRootStorage::init(void)
{
}

//**********************************************************************
//
CRRootStorage::CRRootStorage(VString *lpStorageName)
{
  	CDBG(	<< "In CRRootStorage::CRRootStorage(name = " << lpStorageName
  			<< "), this = " << this << endl);

	init();

	if (lpStorageName) {
		setStorageName(*lpStorageName);
	}
}

//**********************************************************************
//
CRRootStorage::~CRRootStorage()
{
}

//**********************************************************************
//
// TO DO: what happens if the storage exists?
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRRootStorage::open()
{
  	CDBG(	<< "In CRRootStorage::open, opening " << getStorageName() << endl);
		
	LPSTORAGE	lpStorage;
	                         
	// don't try to open more than once
	cv_assert(!isRealized());

	// open root storage.
	theResult = StgOpenStorage (
				getStorageName().size() > 0 ? (LPCOLESTR) getStorageName() : NULL,
				0, 
				STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE, 
				0, 
				0, 
				&lpStorage);

	processOpen(lpStorage);
	
	return isRealized();
}

//**********************************************************************
//
// TO DO: what happens if the storage exists?
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRRootStorage::create()
{
  	CDBG(	<< "In CRRootStorage::create, creating " << getStorageName()
  			<< endl);
		
	LPSTORAGE	lpStorage;
	                         
	// don't try to open more than once
	cv_assert(!isRealized());

	// create root storage.
	theResult = StgCreateDocfile (
		getStorageName().size() > 0 ? (LPCOLESTR) getStorageName() : NULL,
		STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE,
		0,
		&lpStorage);

	processOpen(lpStorage);
	
	return isRealized();
}

//**********************************************************************
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRRootStorage::remove()
{
  	CDBG(	<< "In CRRootStorage::remove, removing " << getStorageName() << endl);

	boolean result;
	
	cv_assert(getStorageName().size() > 0);
	
	// can't do the remove on open storage.
	cv_assert(!isRealized());

	cv_assert(isRoot());

	result = getStorageName().remove();
		
	if (!result) {
	  	theResult = getStorageName().getResult();
	  	CDBG(	<< "    ...remove failed: " 
	  			<< theResult.asSystemMessage() << endl);
	}	
	
	return result;
}
	
//**********************************************************************
//
// Returns:
//		TRUE if successful, else FALSE
//
// TO DO: what about the moniker? Should it be re-realized? What if
// it been returned to an object (has a ref count > 0)?
//
// TO DO: implement dst name != this->getStorageName() name
//
boolean CRRootStorage::rename(const VPathString& dst)
{
  	CDBG(	<< "In CRRootStorage::rename, renaming " << getStorageName()
			<< " to " << dst << endl);

	boolean result;
	
	cv_assert(dst.size() > 0);
	
	cv_assert(isRoot());
	
	// can't be realized.
	cv_assert(!isRealized());
       
    // rename the source. If it fails, report the error to debug,
    // but continue and try copy/delete.
	// TO DO: let the rename fail because some OS's allow this.
	// However, there is no facility independent way to analyze a 
	// VResult for a specific result. This needs to be addressed.
	result = getStorageName().move(dst);
	if (!result) {
		theResult = getStorageName().getResult();
  		CDBG(	<< "    ...move to " << dst << " failed: "
  				<< getStorageName().getResult().asSystemMessage()
  				<< endl);
  		CDBG(	<< "    ...trying duplicate/remove" << endl);
	}

	// if rename fails try copy/delete
	if (!result) {
		// copy the source
		if (!getStorageName().duplicate(dst)) {
			theResult = getStorageName().getResult();
		  	CDBG(	<< "    ...duplicate to " << dst << " failed: "
	  				<< getStorageName().getResult().asSystemMessage()
	  				<< endl);
	  		return FALSE;
	  	}
	  	
		// Delete the source. However, don't return error, because the file
		// copy succeeded.
		if (!getStorageName().remove()) {
			theResult = getStorageName().getResult();
		  	CDBG(	<< "    ...removal of " << getStorageName() << " failed: "
	  				<< getStorageName().getResult().asSystemMessage()
	  				<< endl);
	  	}
	}		

	// set the new name in the storage
	setStorageName(dst);
		
	// Temporary storages are storages created without names.
	// Once a storage is (re)named, it can no longer be temporary,
	// so set the storage to NOT temporary.
	bTemporary = FALSE;

	return TRUE;
}

//**********************************************************************
//
void CRRootStorage::renamed(void)
{
	
}
	
//**********************************************************************
//
// Returns:
//		TRUE if storage exists, else FALSE
//
boolean CRRootStorage::exists()
{
	cv_assert(getStorageName().size() > 0);
	
	cv_assert (isRoot());
	
	return getStorageName().exists();
}

