// $Header:   Q:/views/win/vcs/RSTORAG.CPv   1.5   Mar 04 1997 14:13:48   gregm  $

//	CRStorage
//
//  Compound Storage Reference Class
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
//	04/20/94 dss	Initial release.
//	11/17/95 dss	Account for WIN16/WIN32 differences in declaration of 
//					CreateItemMoniker().
//
// TO DO: what about moving storages from one storage parent to 
// another. This is what rename does but is more invlolved?
// Currently move is aliased to reanme, THis is wrong as it implies
// moving the storage to a new name in the same parent. However,
// in the RStorage::move(RStorage& dst) case the dst could be the
// same parent of different one.
//
//**********************************************************************

#include "pre.h"

#include "iterator.h"
#include "ordcllct.h"

//**********************************************************************
//
//  CRStorage Interface
//
//**********************************************************************

//**********************************************************************
//
void CRStorage::init(void)
{
	lpStorageParent = 0;
	lpMoniker = 0;
	lpContainerMoniker = 0;
	
	// set CRUnknown ref count for stability
	lCount = 1;
	
	bTemporary = TRUE;
	bDirty = TRUE;
}

//**********************************************************************
//
CRStorage::CRStorage()
	: CRUnknown(0)
	, theResult(*(new OLEResult))
{
  	CDBG(	<< "In CRStorage::CStorage(), this = " << this << endl);

	init();
}

//**********************************************************************
//
CRStorage::CRStorage(LPSTORAGE lpStorage)
	: CRUnknown(lpStorage)
	, theResult(*(new OLEResult))
{
  	CDBG(	<< "In CRStorage::CStorage(IStorage = " << lpStorage
  			<< "), this = " << this << endl);
	init();
}

//**********************************************************************
//
// TO DO: an optimization: if my parent isTemp(), then don't save
// regardless of if i'm temp. Is this dangerous? Should there be a flag
// to set this choice?
// CON: If parent is temp, but this storage is being released, programmer
// may expected this storage to save, the rename the parent, thereby making
// it persistent. Parent is saved, but the child changes are lost because they
// were never saved due to above algorithm. Perhaps we need an inherit temp
// style that so that the programmer can make a choice like this.
//
CRStorage::~CRStorage()
{
	// was close or unrealize done before destruction?
		CDBG(	<< "cv_assert(!isRealized());"<<endl);

	if (lpMoniker) {
		lpMoniker->Release();
		lpMoniker = 0;
	}
	
	delete &theResult;
}

//**********************************************************************
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::open()
{
  	CDBG(	<< "In CRStorage:open, opening " << getStorageName() << endl);
		
	LPSTORAGE	lpStorage;
	                         
	// don't try to open more than once
	cv_assert(!isRealized());
              
    // parent must be realized becasue we open storage from it.
    // TO DO: what about the case where the parent is not open and
    // the child does an open. Should we re-open the parent? What 
    // if the parent is not configured yet (ie name?). Do we
    // open the parent and use RStorage::move().
	cv_assert(getStorageParent().hasStorage() &&
			  getStorageParent().isRealized());

	// create a sub-storage for the object
	theResult = getStorageParent().getStorage().OpenStorage(
				getStorageName().size() > 0 ? (LPCOLESTR) getStorageName() : NULL,
				0, 
				STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE, 
				0, 
				0, 
				&lpStorage);

	if (processOpen(lpStorage)) {
		getStorageParent().openChildrenOf().add(this);
	}
	
	return isRealized();
}

//**********************************************************************
//
// TO DO: what happens if the storage exists?
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::create()
{
  	CDBG(	<< "In CRStorage:create, creating " << getStorageName() << endl);
		
	LPSTORAGE	lpStorage;
	                         
	// don't try to open more than once
	cv_assert(!isRealized());
              
    // parent must be realized becasue we create storage from it.
    // TO DO: what about the case where the parent is not open and
    // the child does a create. Should we re-open the parent? What 
    // if the parent is not configured yet (ie name?). Do we
    // open the parent and use RStorage::move().
	cv_assert(getStorageParent().hasStorage() &&
			  getStorageParent().isRealized());

	// create a sub-storage for the object
	theResult = getStorageParent().getStorage().CreateStorage( 
				getStorageName().size() > 0 ? (LPCOLESTR) getStorageName() : NULL,
				STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE,
				0,
				0,
				&lpStorage);

	if (processOpen(lpStorage)) {
		getStorageParent().openChildrenOf().add(this);
	}
	
	return isRealized();
}

//**********************************************************************
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::processOpen(LPSTORAGE lpStorage)
{
	if (FAILED(theResult.asResult())) {
	  	CDBG(	<< "    ...open failed with result " << hex << setw(8)
	  			<< long(theResult.asResult()) << endl);
	  	return FALSE;
	}
	     
    // Call super-class to set realized object.
    // NOTE: CRUnknown::realize() is used because CRRootStorage::open()
    // may, one day, be CRRootStorage::realize() which is an alternative
    // to CRRootStorage::CRRootStorage(lpStorage).
	CRUnknown::realize(lpStorage);

	// if no storage name was given, OLE created a temp name. Save the
	// temp name as the storage name.
	if (getStorageName().size() == 0) {
		STATSTG			StatStg;
		HRESULT hResult = lpStorage->Stat(&StatStg, STATFLAG_DEFAULT);
		cv_assert(SUCCEEDED(hResult));

		cv_assert(StatStg.pwcsName);

		// save the temp storage name
		storageName = StatStg.pwcsName;
		OleStdFree(StatStg.pwcsName);
	}
		
	// else storage created with a name is NEVER temporary. 
	// TO DO: BUG! if the temp storage is closed and then reopened,
	// the temp storage name exists and the storage loses its temp
	// status.
	else {
		bTemporary = FALSE;
	}
		
	DebugOutputEnumStatStg(&getStorage());

	return TRUE;
}

//**********************************************************************
//
// Inputs
//	deepSave - save all the storage children BEFORE saving self.
//	ignoreErrors - Applies to recursive saves. Keep saving regardless
//		of if a child had a save error.
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::save(boolean	deep,
						boolean	ignoreErrors,
						int		recursionLevel)
{
  	CDBG(	<< ws(4 * recursionLevel) 
  			<< "In CRStorage:save, saving " << getStorageName() << endl);

	theResult = NOERROR;
	
	if (isRealized()) {
		// if deep save, then save kids, first.
		if (deep && openChildrenOf().count() > 0) {
			DO(openChildrenOf(), CRStorage, kid)
			
				// if the kid had an error
				if (!kid->save(deep, ignoreErrors, recursionLevel+1) && 
					!ignoreErrors) {
					theResult = kid->getResult();
					return FALSE;
				}
			END
		}
		
		theResult = Commit(STGC_DEFAULT);
		
		if (FAILED(theResult.asResult())) {
		  	CDBG(	<< ws(4 * recursionLevel) 
  					<< "    ...save failed with result " << hex << setw(8)
		  			<< long(theResult.asResult()) << endl);
		}
	}

	return SUCCEEDED(theResult.asResult());
}

//**********************************************************************
//
// This method closes the storage and optionally saves it before closing
// and optionally deletes it after it closes.
//
// The Storage is unrealized after this method returns leaving it in the
// same state as before it was opened. In the closed (unrealized) state, 
// operating system level operations are allowed: rename, remove, etc.
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::close(boolean bSave, boolean bDelete)
{
  	CDBG(	<< "In CRStorage:close("
  			<< (bSave ? "SAVE, " : "NOSAVE, ")
  			<< (bDelete ? "DELETE" : "KEEP")
  			<< "), closing " << getStorageName() << endl);

	boolean 	result = TRUE;
	
	if (isRealized()) {
		if (bSave) {
			result = save();
		}
		 
		// if save was ok, then unrealize and optionally remove.
		// NOTE: don't unrealize on error. This gives the user the option to
		// take evasive action, such as saving to another storage. However,
		// ALL storages MUST BE CLOSED successfully or a file system lock and
		// and interface pointer will be held, so after evasive action, the 
		// storage must be closed without save. If this fails then close without 
		// save and without delete.
		if (result) {
			// Unrealize the native object. If we don't do this after a close,
			// the native OS still has a lock on the storage and other methods
			// that depend on closed storages (like rename, remove, etc.) will
			// fail due to lock errors.

			// Unrealize gets rid of the native object but still keeps the C++
			// object around (as opposed to CRUnknown::Release, which could delete
			// "this"), which is what we want so that we can still perform methods
			// on it when it is in the closed state.
			unrealize();

			// remove the storage if we're not keeping it.	
			if (bDelete) {
				result = remove();
			}
	
			if (hasStorageParent()) {
				getStorageParent().openChildrenOf().remove(this);
			}
		
		} // if saved
	
	} // if realized
	
	return result;
}

//**********************************************************************
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::remove()
{
  	CDBG(	<< "In CRStorage:remove, removing " << getStorageName() << endl);

	boolean result;
	
	cv_assert(getStorageName().size() > 0);
	
	// can't do the remove on open storage.
	cv_assert(!isRealized());

	cv_assert(!isRoot());
	cv_assert(hasStorageParent());
	cv_assert(getStorageParent().isRealized());
		
	theResult = getStorageParent().DestroyElement(getStorageName());
	result = SUCCEEDED(theResult.asResult());
		
	if (!result) {
	  	CDBG(	<< "    ...remove failed with result " << hex << setw(8)
	  			<< long(theResult.asResult()) << endl);
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
boolean CRStorage::rename(const VPathString& dst)
{
  	CDBG(	<< "In CRStorage:rename, renaming " << getStorageName()
			<< " to " << dst << endl);

	boolean result;
	
	cv_assert(dst.size() > 0);
	
	cv_assert(!isRoot());

	cv_assert(hasStorageParent());
	cv_assert(getStorageParent().isRealized());
		
	theResult = getStorageParent().RenameElement(getStorageName(), dst);
	result = SUCCEEDED(theResult.asResult());
		
	if (!result) {
	  	CDBG(	<< "    ...rename failed with result " << hex << setw(8)
	  			<< long(theResult.asResult()) << endl);
	}	

	// if storage was renamed without error then do post-rename processing.
	if (result == TRUE) {
	
		// set the new name in the storage
		setStorageName(dst);
		
		// Temporary storages are storages created without names.
		// Once a storage is (re)named, it can no longer be temporary,
		// so set the storage to NOT temporary.
		bTemporary = FALSE;
	}

	return result;
}
	
//**********************************************************************
//
// Returns:
//		TRUE if successful, else FALSE
//
boolean CRStorage::undo(void)
{
  	CDBG(	<< "In CRStorage:undo, undoing " << getStorageName() << endl);

	cv_assert(isRealized());
	theResult = Revert();

	if (FAILED(theResult.asResult())) {
	  	CDBG(	<< "    ...undo failed with result " << hex << setw(8)
	  			<< long(theResult.asResult()) << endl);
	}	
	
	return SUCCEEDED(theResult.asResult());
}

//**********************************************************************
//
LPMONIKER CRStorage::createMoniker(void)
{
  	CDBG( << "In CRStorage::createMoniker, "); // NO endl!

	LPMONIKER	lpMoniker = 0;
    HRESULT		hResult;
                          
	cv_assert(getStorageName().size() > 0);
                          
	// if I have parent storage, then create an item moniker
	if (hasStorageParent()) {
#if defined(CV_WIN16)
		// On WIN16 the 2nd param is not const so we need the cast.
		hResult = CreateItemMoniker(_OT("!"), (LPSTR) getStorageName().gets(), &lpMoniker);
#else
		hResult = CreateItemMoniker(_OT("!"), getStorageName(), &lpMoniker);
#endif
	}

	// else I'm the container, so create the file moniker
	else {
#if defined(CV_WIN16)
		// On WIN16 the 1st param is not const so we need the cast.
		hResult = CreateFileMoniker((LPSTR) getStorageName().gets(), &lpMoniker);
#else
		hResult = CreateFileMoniker(getStorageName(), &lpMoniker);
#endif
	}

	cv_assert(SUCCEEDED(hResult));

	//!!!dss Do I have to AddRef this or was this unknown given to me
	// with an incremented refCount, like QueryInterface?
	// lpMoniker->AddRef();

  	CDBG(	<< "moniker(" << getStorageName() << ") = "
			<< lpMoniker << endl);

	return lpMoniker;
}

//**********************************************************************
//
// TO DO: Implement moniker assignment and unassignment:
//
//		OLEGETMONIKER_ONLYIFTHERE
//		OLEGETMONIKER_FORCEASSIGN
//		OLEGETMONIKER_UNASSIGN
//		OLEGETMONIKER_TEMPFORUSER	return moniker but don't cache
//
LPMONIKER CRStorage::getMoniker(DWORD assign, DWORD whichMoniker)
{
  	CDBG(	<< "In CRStorage::getMoniker(" << assign2Name(assign) << ", "
			<< which2Name(whichMoniker) << ')' << endl);

	if (whichMoniker ==  OLEWHICHMK_CONTAINER ) {
		// if we're the container, return our moniker
		if (lpStorageParent == 0) {
			// if we don't have a moniker, and we're asked to create one,
			// do so.
			if (lpMoniker == 0 /* && assign == OLEGETMONIKER_FORCEASSIGN */) {
				lpMoniker = createMoniker();
			}
		  	CDBG(	<< "    moniker = " << lpMoniker << endl);
			return lpMoniker;
		}

		// else if we cached the container moniker, return that.
		else if (lpContainerMoniker) {
		  	CDBG(	<< "    moniker = " << lpContainerMoniker << endl);
			return lpContainerMoniker;
		}
		else {
			lpContainerMoniker = lpStorageParent->getMoniker( assign, whichMoniker);
		  	CDBG(	<< "    moniker = " << lpContainerMoniker << endl);
			return lpContainerMoniker;
		}
	}
	else if (whichMoniker ==  OLEWHICHMK_OBJREL) {
	}
	else if (whichMoniker ==  OLEWHICHMK_OBJFULL) {
	}

	return NULL;
}

//**********************************************************************
//
char *CRStorage::assign2Name(DWORD assign)
{
	if (assign == OLEGETMONIKER_ONLYIFTHERE) {
		return "ONLYIFTHERE";
	}
	else if (assign == OLEGETMONIKER_FORCEASSIGN) {
		return "FORCEASSIGN";
	}
	else if (assign == OLEGETMONIKER_UNASSIGN) {
		return "UNASSIGN";
	}
	else if (assign == OLEGETMONIKER_TEMPFORUSER) {
		return "TEMPFORUSER";
	}

	return "ASSIGN_UNKNOWN";
}
	

//**********************************************************************
//
char *CRStorage::which2Name(DWORD which)
{
	if (which == OLEWHICHMK_CONTAINER) {
		return "CONTAINER";
	}
	else if (which == OLEWHICHMK_OBJREL) {
		return "OBJREL";
	}
	else if (which == OLEWHICHMK_OBJFULL) {
		return "OBJFULL";
	}

	return "WHICH_UNKNOWN";
}

//**********************************************************************
//
boolean CRStorage::isTemporary(void) const
{
	return bTemporary;
}

//**********************************************************************
//
boolean CRStorage::isDirty(void) const
{
	// TO DO: fill this in: see IPersistStorage::Save() doc.
	return TRUE;
}

//**********************************************************************
//
// Returns:
//		TRUE if storage exists, else FALSE
//
boolean CRStorage::exists()
{
	boolean result;
	
	cv_assert(getStorageName().size() > 0);
	
	cv_assert (!isRoot());

	cv_assert(hasStorageParent());
	cv_assert(getStorageParent().isRealized());
		            
	// To DO: there must be a better way than opening the storage to
	// see if it exists.                                              
	LPSTORAGE		lpStorage = 0;		                                                          
	HRESULT hResult = getStorageParent().OpenStorage(getStorageName(),
													 NULL,
													 STGM_FAILIFTHERE |
													 STGM_READ |
													 STGM_SHARE_EXCLUSIVE,
													 NULL,
													 0,
													 &lpStorage);
														 
	if (lpStorage) {
		HRESULT hResult = getStorageParent().DestroyElement(getStorageName());
		cv_assert(SUCCEEDED(hResult));
		lpStorage->Release();
	}
																 
	result = (lpStorage != 0);
	
	return result;
}

//**********************************************************************
//
CRStorage& CRStorage::getRootStorageParent(void) const
{
	const CRStorage	*storage;
	
	for (storage = this;
		 storage && !storage->isRoot();
		 storage = storage->lpStorageParent)
		; // NO CODE GOES HERE!
	
	//!!!dss: this is to protect the reference. Why is it a reference?
	cv_assert(storage);

	return (CRStorage&) *storage;
}
