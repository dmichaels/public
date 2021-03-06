// $Header:   Q:/views/win/vcs/storag.cpv   1.6   Mar 25 1997 15:57:16   gregm  $

//	CStorage
//
//  OLE IStorage Implementation
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
//	03/20/95 dss	Initial release.
//	12/04/95 dss	Added PVCS header. Migrated to LPCOLESTR.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
//  XStorage Interface
//
//**********************************************************************

//**********************************************************************
//
XStorage::XStorage(CStorage *pStorage)
	: Storage( pStorage)
{
}

//**********************************************************************
XStorage::~XStorage()
{
}

//**********************************************************************
//
//  CStorage Interface
//
//**********************************************************************

//**********************************************************************
//
void CStorage::init(REFVCOMPONENTOBJECT refCDObject)
{
	lpCRStorage = new CRStorage( (LPSTORAGE)(LPUNKNOWN)getUnknown());

	// override the CRStorage setting
	setDeleteOnRelease(TRUE);
}

//**********************************************************************
//
//	refUnkWindow - the Outer VWindow CUnknown
//
#pragma warning(disable : 4355)  // turn off this warning.
		// This warning tells us that we are passing this in an initializer,
		// before "this" is through initializing.  This is ok, because
		// we just store the ptr in the other constructors
CStorage::CStorage(REFVCOMPONENTOBJECT refCDObject)
	: CUnknown(refCDObject, new XStorage(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
  	CDBG(	<< "In CRStorage's aggregate, sub-class constructor, this = " 
			<< this << endl);

	init(refCDObject);
}

//**********************************************************************
//
//	refCDObject - the Outer CDObject
//	refUnkBase - the Base Class IUnknown that this object is a super-class of.
//
CStorage::CStorage(REFVCOMPONENTOBJECT refCDObject, LPSTORAGE lpIStorage)
	: CUnknown(refCDObject, lpIStorage)
{
  	CDBG(	<< "In CRStorage's aggregate, super-class constructor, this = " 
			<< this << endl);

	init(refCDObject);
}

//**********************************************************************
//
CStorage::~CStorage()
{
  	CDBG(	<< "In CRStorage's destructor" << endl);
}

//**********************************************************************
//
STDMETHODIMP CStorage::CreateStream(LPCOLESTR pwcsName,
                DWORD grfMode,
                DWORD reserved1,
                DWORD reserved2,
                IStream FAR *FAR *ppstm)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::OpenStream(LPCOLESTR pwcsName,
		void FAR *reserved1,
                DWORD grfMode,
                DWORD reserved2,
                IStream FAR *FAR *ppstm)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::CreateStorage(LPCOLESTR pwcsName,
            DWORD grfMode,
            DWORD reserved1,
            DWORD reserved2,
            IStorage FAR *FAR *ppstg)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::OpenStorage(LPCOLESTR pwcsName,
                IStorage FAR *pstgPriority,
                DWORD grfMode,
                SNB snbExclude,
                DWORD reserved,
                IStorage FAR *FAR *ppstg)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::CopyTo(DWORD ciidExclude,
 		    IID const FAR *rgiidExclude,
 		    SNB snbExclude,
 		    IStorage FAR *pstgDest)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::MoveElementTo(LPCOLESTR lpszName,
												IStorage FAR *pstgDest,
												LPCOLESTR lpszNewName,
												DWORD grfFlags)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::Commit(DWORD grfCommitFlags)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::Revert()
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::EnumElements(DWORD reserved1,
                void FAR *reserved2,
                DWORD reserved3,
                IEnumSTATSTG FAR *FAR *ppenm)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::DestroyElement(LPCOLESTR pwcsName)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::RenameElement(LPCOLESTR pwcsOldName,
												 LPCOLESTR pwcsNewName)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::SetElementTimes(LPCOLESTR lpszName,
												FILETIME const FAR *pctime,
												FILETIME const FAR *patime,
												FILETIME const FAR *pmtime)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::SetClass(REFCLSID clsid)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::SetStateBits(DWORD grfStateBits, DWORD grfMask)
{
	return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP CStorage::Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag)
{
	return ResultFromScode(E_FAIL);
}
