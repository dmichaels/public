// $Header:   Q:/views/win/vcs/ARCHBYTE.CPv   1.5   Mar 04 1997 14:11:36   gregm  $

//  archbyte.cpp
//
//  Compound Document Item
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
//  02/23/96 dss    Borland conversion.
//	--------------------------------------------------------------------------

#ifndef CV_NOOLEARCHIVER

#include "pre.h"

// OLE Includes
#include "archbyte.h"

void ArchiverBytes::init(DWORD grfMode,
                         VArchiver::AtMethodGetType getPos, 
                         VArchiver::AtMethodSetType setPos)
{
    cv_assert(archiver.isOpen());

    archiveOffset = (archiver.*getPos)();
    
    this->getPos = getPos;
    this->setPos = setPos;
    
    this->grfMode = grfMode;
    
    lCount = 0;
}

//**********************************************************************
//
ArchiverBytes::ArchiverBytes(VArchiver& a, DWORD grfMode,
                             VArchiver::AtMethodGetType getPos, 
                             VArchiver::AtMethodSetType setPos)
    : archiver(a)
{
    CDBG(   << "In ArchiverBytes ctor" << endl);

    init(grfMode, getPos, setPos);
}

//**********************************************************************
//
ArchiverBytes::~ArchiverBytes()
{
    CDBG(   << "In ArchiverBytes dtor" << endl);
}

// *** ILockBytes methods *** 

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::ReadAt(ULARGE_INTEGER ulOffset,
                    VOID HUGEP *pv,
                    ULONG cb,
                    ULONG FAR *pcbRead)
{
    CDBG(   << "In ArchiverBytes::ReadAt, offset = " << ulOffset
            << ", count = " << cb << endl);
    
    // setPos method only handles longs. Assert that we are getting this.
	cv_assert(ULIGet32Hi(ulOffset) == 0);

    if ((archiver.*setPos)(ULIGet32Lo(ulOffset)) == -1) {
        return ResultFromScode(E_FAIL);
    }
    
    filepos_t       numToRead,
                    numRead;
                        
    if (cb > 0xfffe) {
        numToRead = filepos_t(0xfffe);
    }
    else {
        numToRead = filepos_t(cb);
    }
    
    *pcbRead = 0;
    while (1) {
        numRead = archiver.read((char *)pv, filepos_t(numToRead));
        
        // if error, leave loop
        if (numRead < 0) {
            return ResultFromScode(E_FAIL);
        }
        
        // update the read count
        *pcbRead += numRead;
        
        // if read less than asked, leave loop, but update the read count (above).
        if (numRead < numToRead) {
            break;
        }
        
        pv = (char HUGEP *) pv + numRead;

        cb -= numToRead;
        
        // if we've read the last buffer, exit.
        if (cb == 0) {
            break;
        }

        // if last buffer, then set read buffer size
        if ( cb < ULONG(numToRead)) {
            numToRead = filepos_t(cb);
        }
    }
    
    CoFileTimeNow(&accessTime);

    return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::WriteAt(ULARGE_INTEGER ulOffset,
                     VOID const HUGEP *pv,
                     ULONG cb,
                     ULONG FAR *pcbWritten)
{
    CDBG(   << "In ArchiverBytes::WriteAt, offset = " << ulOffset
            << ", count = " << cb << endl);
    
    // setPos method only handles longs. Assert that we are getting this.
    cv_assert(ULIGet32Hi(ulOffset) == 0);
    
    if ((archiver.*setPos)(ULIGet32Lo(ulOffset)) == -1) {
        return ResultFromScode(E_FAIL);
    }
    
    filepos_t       numToWrite,
                    numWritten;
                        
    if (cb > 0xfffe) {
        numToWrite = filepos_t(0xfffe);
    }
    else {
        numToWrite = filepos_t(cb);
    }
    
    *pcbWritten = 0;
    while (1) {
        numWritten = archiver.write((char *)pv, filepos_t(numToWrite));
        
        // if error, leave loop
        if (numWritten < 0) {
            return ResultFromScode(E_FAIL);
        }
        
        // update the read count
        *pcbWritten += numWritten;
        
        // if written less than asked, leave loop, but update the written count (above).
        if (numWritten < numToWrite) {
            break;
        }
        
        pv = (char HUGEP *) pv + numWritten;
        
        cb -= numToWrite;

        // if we've written the last buffer, exit.
        if (cb == 0) {
            break;
        }

        // if last buffer, then set read buffer size
        if ( cb < ULONG(numToWrite)) {
            numToWrite = filepos_t(cb);
        }
        
    }

    CoFileTimeNow(&accessTime);
    // all mod time == accessTime: do NOT call CoFileTimeNow() again!
    modificationTime = accessTime;  

    return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::Flush()
{
    CDBG(   << "In ArchiverBytes::Flush" << endl);
    
    return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::SetSize(ULARGE_INTEGER cb)
{
    CDBG(   << "In ArchiverBytes::SetSize, size = " << cb << endl);
    
    // TO DO: this method does nothing since VFile and VBlockFile are auto 
    // expanding. However, there could be issues where truncation is
    // desired (cb < current file size)?
         
    return ResultFromScode(S_OK);
}

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::LockRegion(ULARGE_INTEGER libOffset,
                        ULARGE_INTEGER cb,
                        DWORD dwLockType)
{
    CDBG(   << "In ArchiverBytes::LockRegion, offset = " << libOffset
            << ", count = " << cb << endl);
    
    return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::UnlockRegion(ULARGE_INTEGER libOffset,
                          ULARGE_INTEGER cb,
                          DWORD dwLockType)
{
    CDBG(   << "In ArchiverBytes::UnlockRegion" << endl);
    
    return ResultFromScode(E_FAIL);
}

//**********************************************************************
//
STDMETHODIMP ArchiverBytes::Stat(STATSTG FAR *pstatstg, DWORD grfStatFlag)
{
    CDBG(   << "In ArchiverBytes::Stat" << endl);
    
    memset(pstatstg, '\0', sizeof(STATSTG));
    
    pstatstg->type = STGTY_LOCKBYTES;
    
    // To DO: replace the seek position difference with a real (object in
    // storage) size.
    ULISet32(pstatstg->cbSize, (archiver.*getPos)() - archiveOffset);
    
    pstatstg->mtime = modificationTime;
    pstatstg->ctime = creationTime;
    pstatstg->atime = accessTime;
    
    pstatstg->grfMode = grfMode;

    return ResultFromScode(S_OK);
}
#endif //CV_NOOLEARCHIVER
