// $Header:   Q:/views/win/vcs/LOCKBYTE.H_v   1.4   Mar 04 1997 14:12:54   gregm  $

//	lochbyte.h
//
//  CLockBytes
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
//	06/07/95 dss	Initial release.
//	---------------------------------------------------------------------------

#if !defined( _LOCKBYTE_H_ )
#define _LOCKBYTE_H_

CLASS CLockBytes : public CUnknown
{
public: 
	CLockBytes();
	virtual ~CLockBytes();
	
	LPTSTR			getClassName() {return _T("CLockBytes");}

public:
    // *** ILockBytes methods ***
    virtual STDMETHODIMP ReadAt(ULARGE_INTEGER ulOffset,
             VOID HUGEP *pv,
             ULONG cb,
             ULONG FAR *pcbRead) = 0;
    virtual STDMETHODIMP WriteAt(ULARGE_INTEGER ulOffset,
              VOID const HUGEP *pv,
              ULONG cb,
              ULONG FAR *pcbWritten) = 0;
    virtual STDMETHODIMP Flush() = 0;
    virtual STDMETHODIMP SetSize(ULARGE_INTEGER cb) = 0;
    virtual STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 DWORD dwLockType) = 0;
    virtual STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset,
                   ULARGE_INTEGER cb,
                 DWORD dwLockType) = 0;
    virtual STDMETHODIMP Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag) = 0;
    
	// cast operators to equate Cxxx class to Ixxx class
	operator 		LPLOCKBYTES() {return (LPLOCKBYTES)(LPUNKNOWN)getUnknown();}
	
protected:
	FILETIME		accessTime;
	FILETIME		modificationTime;
	FILETIME		creationTime;

private:
	void			init(void);
};

#define STD_ILOCKBYTES_METHODS() \
    virtual STDMETHODIMP ReadAt(ULARGE_INTEGER ulOffset, \
             VOID HUGEP *pv, \
             ULONG cb, \
             ULONG FAR *pcbRead) \
				{ return getLockBytes().ReadAt(ulOffset, pv, cb, pcbRead);} \
    virtual STDMETHODIMP WriteAt(ULARGE_INTEGER ulOffset, \
              VOID const HUGEP *pv, \
              ULONG cb, \
              ULONG FAR *pcbWritten) \
				{ return getLockBytes().WriteAt(ulOffset, pv, cb, pcbWritten);} \
    virtual STDMETHODIMP Flush() \
				{ return getLockBytes().Flush();} \
    virtual STDMETHODIMP SetSize(ULARGE_INTEGER cb) \
				{ return getLockBytes().SetSize(cb);} \
    virtual STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset, \
                 ULARGE_INTEGER cb, \
                 DWORD dwLockType) \
				{ return getLockBytes().LockRegion(libOffset, \
							cb, dwLockType);} \
    virtual STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, \
                   ULARGE_INTEGER cb, \
                 DWORD dwLockType) \
				{ return getLockBytes().UnlockRegion(libOffset, \
							cb, dwLockType);} \
    virtual STDMETHODIMP Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag) \
				{ return getLockBytes().Stat(pstatstg, grfStatFlag);}

#define STD_LockBytes_METHODS() STD_ILOCKBYTES_METHODS()

#define STD_ILOCKBYTES_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, LockBytes, access)

#define INHERIT_ILOCKBYTES_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*LockBytes) \
	STD_ILOCKBYTES_METHODS() \
	STD_ILOCKBYTES_ACCESS(*LockBytes) \
protected: \
	CLockBytes		*LockBytes;

#define CONTAIN_ILOCKBYTES_INTERFACE() \
public: \
	STD_ILOCKBYTES_METHODS() \
	STD_ILOCKBYTES_ACCESS(*LockBytes) \
protected: \
	CLockBytes		*LockBytes;

//**********************************************************************
//
//  ILockBytes Class
//
CLASS XLockBytes : public ILockBytes
{
public:

	XLockBytes(CLockBytes *);
	~XLockBytes();

	INHERIT_INTERFACE(ILOCKBYTES, LockBytes)
};

#endif // _LOCKBYTE_H_
