// $Header:   Q:/views/win/vcs/STORAG.H_v   1.6   Mar 04 1997 14:14:00   gregm  $

//  CStorage
//
//  OLE IStorage Implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  12/28/94 dss    Initial release.
//	01/09/96 dss	VComponent name change.
//  ---------------------------------------------------------------------------

#ifndef CSTORAGE_H
#define CSTORAGE_H

// BASIC Views OLE types
#include "oledefs.h"

// Views GUI inlcudes
#include "pathstr.h"

// unknown includes
#include "unknown.h"
#include "rstorag.h"

// Forward Declarations
CLASS CStorage;
typedef CStorage FAR *LPCSTORAGE;

//**********************************************************************
//
// CStorage Implementation
//
CLASS CStorage : public CUnknown
{
public:

    // Base class constructor
    // Use this constructor when this object is a base-class object
    CStorage(REFVCOMPONENTOBJECT refCDObject);

    // super-class constructor
    // use this constructor when this object is a super-class object
    CStorage(REFVCOMPONENTOBJECT refCDObject, LPSTORAGE lpIStorage);

    virtual ~CStorage();

    LPTSTR      getClassName() {return _T("CStorage");}

public:

    // *** IStorage methods ***
    virtual STDMETHODIMP CreateStream(LPCOLESTR pwcsName,
                   DWORD grfMode,
                   DWORD reserved1,
                   DWORD reserved2,
                   IStream FAR *FAR *ppstm);
    virtual STDMETHODIMP OpenStream(LPCOLESTR pwcsName,
         void FAR *reserved1,
                 DWORD grfMode,
                 DWORD reserved2,
                 IStream FAR *FAR *ppstm);
    virtual STDMETHODIMP CreateStorage(LPCOLESTR pwcsName,
                DWORD grfMode,
                DWORD reserved1,
                DWORD reserved2,
                IStorage FAR *FAR *ppstg);
    virtual STDMETHODIMP OpenStorage(LPCOLESTR pwcsName,
                  IStorage FAR *pstgPriority,
                  DWORD grfMode,
                  SNB snbExclude,
                  DWORD reserved,
                  IStorage FAR *FAR *ppstg);
    virtual STDMETHODIMP CopyTo(DWORD ciidExclude,
               IID const FAR *rgiidExclude,
               SNB snbExclude,
               IStorage FAR *pstgDest);
    virtual STDMETHODIMP MoveElementTo(LPCOLESTR lpszName,
                      IStorage FAR *pstgDest,
                              LPCOLESTR lpszNewName,
                              DWORD grfFlags);
    virtual STDMETHODIMP Commit(DWORD grfCommitFlags);
    virtual STDMETHODIMP Revert();
    virtual STDMETHODIMP EnumElements(DWORD reserved1,
                 void FAR *reserved2,
                 DWORD reserved3,
                 IEnumSTATSTG FAR *FAR *ppenm);
    virtual STDMETHODIMP DestroyElement(LPCOLESTR pwcsName);
    virtual STDMETHODIMP RenameElement(LPCOLESTR pwcsOldName,
                LPCOLESTR pwcsNewName);
    virtual STDMETHODIMP SetElementTimes(LPCOLESTR lpszName,
                                FILETIME const FAR *pctime,
                                FILETIME const FAR *patime,
                                FILETIME const FAR *pmtime);
    virtual STDMETHODIMP SetClass(REFCLSID clsid);
    virtual STDMETHODIMP SetStateBits(DWORD grfStateBits, DWORD grfMask);
    virtual STDMETHODIMP Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag);

    VComponentObject&      getComponentThis() {return *lpCDObject;}

    operator LPSTORAGE() {return (LPSTORAGE) lpCRStorage;}

    CRStorage&          getStorage() {return *lpCRStorage;}
    boolean             hasStorage() {return &getStorage() != 0;}
    const VPathString   *getStorageName();
    LPMONIKER           getMoniker(DWORD assign,
                                   DWORD whichMoniker = OLEWHICHMK_CONTAINER);
                                   
protected:
    VComponentObject       *lpCDObject;
    LPCRSTORAGE     lpCRStorage;        // IStorage pointer for object

private:
    void            init(REFVCOMPONENTOBJECT refVComponentObject);

};

#define INHERIT_ISTORAGE_INTERFACE() \
public: \
    STD_IUNKNOWN_METHODS() \
    STD_IUNKNOWN_ACCESS(*Storage) \
    STD_ISTORAGE_METHODS() \
    STD_ISTORAGE_ACCESS(*Storage) \
protected: \
    CStorage        *Storage;

#define CONTAIN_ISTORAGE_INTERFACE() \
public: \
    STD_ISTORAGE_METHODS() \
    STD_ISTORAGE_ACCESS(*Storage) \
protected: \
    CStorage        *Storage;
    
//**********************************************************************
//
//  IStorage Class
//
CLASS XStorage : public IStorage
{
public:

    XStorage(CStorage *);
    ~XStorage();

    INHERIT_INTERFACE(ISTORAGE, Storage)
};

#endif

