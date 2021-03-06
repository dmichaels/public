// $Header:   Q:/views/win/vcs/ARCHBYTE.H_v   1.5   Mar 04 1997 14:11:38   gregm  $

//	achbyte.h
//
//  ILockBytes IMplementation on VArchiver
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
//  Revision History:
//  -----------------
//  06/07/95 dss    Initial release.
//	---------------------------------------------------------------------------

#if !defined( _ARCHBYTE_H_ )
#define _ARCHBYTE_H_

#ifndef CV_NOOLEARCHIVER
CLASS ArchiverBytes : public CLockBytes
{
public: 
    ArchiverBytes(VArchiver&, DWORD, VArchiver::AtMethodGetType, 
                  VArchiver::AtMethodSetType);
    virtual ~ArchiverBytes();
    
    LPTSTR          getClassName() {return _T("ArchiverBytes");}

    // *** ILockBytes methods ***
    STDMETHODIMP ReadAt(ULARGE_INTEGER ulOffset,
             VOID HUGEP *pv,
             ULONG cb,
             ULONG FAR *pcbRead);
    STDMETHODIMP WriteAt(ULARGE_INTEGER ulOffset,
              VOID const HUGEP *pv,
              ULONG cb,
              ULONG FAR *pcbWritten);
    STDMETHODIMP Flush();
    STDMETHODIMP SetSize(ULARGE_INTEGER cb);
    STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 DWORD dwLockType);
    STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset,
                   ULARGE_INTEGER cb,
                 DWORD dwLockType);
    STDMETHODIMP Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag);
    
protected:

    void            init(DWORD, VArchiver::AtMethodGetType, 
                         VArchiver::AtMethodSetType);
    VArchiver::AtMethodGetType  getPos;
    VArchiver::AtMethodSetType  setPos;

    VArchiver&      archiver;
    filepos_t       archiveOffset;
    filepos_t       itemOffset;
    
    DWORD           grfMode;
    
    long            lCount;
};
#endif // CV_NOOLEARCHIVER
#endif // _ARCHBYTE_H_
