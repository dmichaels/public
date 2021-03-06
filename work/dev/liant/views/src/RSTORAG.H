// $Header:   Q:/views/win/vcs/RSTORAG.H_v   1.4   Mar 04 1997 14:13:50   gregm  $

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
//	4/20/94 dss	Initial release.
//	---------------------------------------------------------------------------

#if !defined( _CRSTORAGE_H_ )
#define _CRSTORAGE_H_

// BASIC Views OLE types
#include "oledefs.h"

// Views GUI inlcudes
#include "pathstr.h"

// unknown includes
#include "unknown.h"
#include "olerslt.h"

// Forward Declarations
CLASS CRStorage;
typedef	CRStorage FAR *LPCRSTORAGE;

// STORAGE_STYLE's    
#define deleteOnClose		1
typedef unsigned long STORAGE_STYLE;

#define STD_ISTORAGE_METHODS() \
    virtual STDMETHODIMP CreateStream( LPCOLESTR pwcsName, \
                   DWORD grfMode, \
                   DWORD reserved1, \
                   DWORD reserved2, \
                   IStream FAR *FAR *ppstm) \
					{ return getStorage().CreateStream(pwcsName, \
							grfMode, reserved1, reserved2, ppstm);} \
    virtual STDMETHODIMP OpenStream(LPCOLESTR  pwcsName, \
				 void FAR *reserved1, \
                 DWORD grfMode, \
                 DWORD reserved2, \
                 IStream FAR *FAR *ppstm) \
					{ return getStorage().OpenStream(pwcsName, \
							reserved1, grfMode, reserved2, ppstm);} \
    virtual STDMETHODIMP CreateStorage(LPCOLESTR  pwcsName, \
                DWORD grfMode, \
                DWORD reserved1, \
                DWORD reserved2, \
                IStorage FAR *FAR *ppstg) \
					{ return getStorage().CreateStorage(pwcsName, \
						grfMode, reserved1, reserved2, ppstg);} \
    virtual STDMETHODIMP OpenStorage(LPCOLESTR  pwcsName, \
                  IStorage FAR *pstgPriority, \
                  DWORD grfMode, \
                  SNB snbExclude, \
                  DWORD reserved, \
                  IStorage FAR *FAR *ppstg) \
					{ return getStorage().OpenStorage(pwcsName, \
					pstgPriority, grfMode, snbExclude, reserved, ppstg);} \
    virtual STDMETHODIMP CopyTo(DWORD ciidExclude, \
 		       IID const FAR *rgiidExclude, \
 		       SNB snbExclude, \
 		       IStorage FAR *pstgDest) \
					{ return getStorage().CopyTo(ciidExclude, rgiidExclude, \
									snbExclude, pstgDest);} \
    virtual STDMETHODIMP MoveElementTo(LPCOLESTR  lpszName, \
    			      IStorage FAR *pstgDest, \
                              LPCOLESTR  lpszNewName, \
                              DWORD grfFlags) \
					{ return getStorage().MoveElementTo(lpszName, \
										pstgDest, lpszNewName, grfFlags);} \
    virtual STDMETHODIMP Commit(DWORD grfCommitFlags) \
					{ return getStorage().Commit(grfCommitFlags);} \
    virtual STDMETHODIMP Revert() \
					{ return getStorage().Revert();} \
    virtual STDMETHODIMP EnumElements(DWORD reserved1, \
                 void FAR *reserved2, \
                 DWORD reserved3, \
                 IEnumSTATSTG FAR *FAR *ppenm) \
					{ return getStorage().EnumElements(reserved1, \
							reserved2, reserved3, ppenm);} \
    virtual STDMETHODIMP DestroyElement(LPCOLESTR  pwcsName) \
					{ return getStorage().DestroyElement(pwcsName);} \
    virtual STDMETHODIMP RenameElement(LPCOLESTR  pwcsOldName, \
                LPCOLESTR  pwcsNewName) \
					{ return getStorage().RenameElement(pwcsOldName, \
                								pwcsNewName);} \
    virtual STDMETHODIMP SetElementTimes(LPCOLESTR  lpszName, \
    			        		FILETIME const FAR *pctime, \
                                FILETIME const FAR *patime, \
                                FILETIME const FAR *pmtime) \
					{ return getStorage().SetElementTimes(lpszName, \
    			        		pctime, patime, pmtime);} \
    virtual STDMETHODIMP SetClass(REFCLSID clsid) \
					{ return getStorage().SetClass(clsid);} \
    virtual STDMETHODIMP SetStateBits(DWORD grfStateBits, DWORD grfMask) \
					{ return getStorage().SetStateBits(grfStateBits, grfMask);} \
    virtual STDMETHODIMP Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag) \
					{ return getStorage().Stat(pstatstg, grfStatFlag);}

#define STD_Storage_METHODS() STD_ISTORAGE_METHODS()

#define STD_ISTORAGE_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, Storage, access)

//**********************************************************************
//
// Storage Reference Class
//
// This class is
//		- a sub-object of an IStorage object
//
// This is a convenience class that refers to an IStorage Interface Pointer 
// (IP) that is given to you by another process. It adds minimal helpful
// functionality not found in OLE (inheritance, among others) to use an
// Interface Pointer.
//
// This class is NOT an IStorage implementation, and hence cannot be used
// to override IStorage methods. You must reference the IP contain within 
// to access IStorage methods.
//
CLASS CRStorage : public CRUnknown
{
public:

	CRStorage();
	CRStorage(LPSTORAGE lpIStorage);
	virtual ~CRStorage();
	
	CRUNKNOWN_INTERFACE(Storage)
public:	

	LPTSTR				getClassName() {return _T("CRStorage");}
        
	VPathString&		getStorageName() {return storageName;}
						// Storage Name is not a pointer, so return TRUE;
	boolean				hasStorageName() const {return TRUE;}
	void				setStorageName(const VString& storageName)
							{this->storageName = storageName;}
							
	CRStorage&			getStorageParent() const {return *lpStorageParent;}
	boolean				hasStorageParent() const {return lpStorageParent != 0;}
	void				setStorageParent(CRStorage *lpStorageParent)
							{this->lpStorageParent = lpStorageParent;}
	CRStorage&			getRootStorageParent(void) const;
							
	LPMONIKER			createMoniker(void);
	LPMONIKER			getMoniker(DWORD assign,
								   DWORD whichMoniker = OLEWHICHMK_CONTAINER);
	boolean				hasMoniker(DWORD whichMoniker = OLEWHICHMK_CONTAINER) const 
								{return lpMoniker != 0;}

	static LPTSTR		assign2Name(DWORD assign);
	static LPTSTR		which2Name(DWORD which);
	
	boolean				isRoot(void) const {return lpStorageParent == 0;}
	boolean				isTemporary(void) const;
	boolean				isDirty(void) const;
	
	virtual boolean		open();
	virtual boolean		create();
	virtual boolean		close(boolean bSave, boolean bDelete);
	virtual	boolean		save(boolean	deep = FALSE,
							 boolean	ignoreErrors = FALSE, 
							 int		recursionLevel = 0);
	virtual boolean		exists();
	virtual boolean		remove();
	boolean				move(const VPathString& dst) {return rename(dst);}	// rename alias
	virtual boolean		rename(const VPathString& dst);
	virtual boolean		undo(void);
	
	virtual OLEResult&	getResult(void) {return theResult;}
	boolean				processOpen(LPSTORAGE lpStorage);
	
	VOrdCollect&		openChildrenOf() {return openChildren;}
	
protected:
	VPathString			storageName;
	CRStorage			*lpStorageParent; // parent CRStorage pointer for object
	LPMONIKER			lpMoniker;		// IMoniker pointer for storage
	LPMONIKER			lpContainerMoniker;	// cache IMoniker pointer for container

	boolean				bTemporary;
	boolean				bDirty;
	
	OLEResult&			theResult;

	VOrdCollect			openChildren;
private:                    
	void			init();
};

#endif // _CRSTORAGE_H_
