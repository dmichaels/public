// $Header:   Q:/views/win/vcs/CMPSTRG.CPv   1.2   Mar 04 1997 14:12:06   gregm  $

//	cdstorag.cpp
//
//  Compound Document Storage
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
//  02/22/95 dss    Initial release.
//	01/09/96 dss	VComponent name change.
//	--------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
void VComponentStorage::init(void)
{
}

//**********************************************************************
//
VComponentStorage::VComponentStorage()
{
    Storage = 0;
}

//**********************************************************************
//
VComponentStorage::~VComponentStorage()
{
    // ensure close before destruction: save and delete temps
    close(TRUE, isTemporary());

    if (hasStorage()) {
        Storage->Release();
        Storage = 0;
    }
}

//**********************************************************************
//
enum yesNoCancel VComponentStorage::rename(const VPathString& newName)
{
    CDBG(   << "In VComponentStorage:rename, renaming " << getStorageName() 
            << " to " << newName << endl);

    // can't have open storage when doing the rename(remove/move).
    cv_assert(!isRealized());

    if (newName.exists()) {
        // return cancel on remove failure.
        if (!newName.remove()) {
            // move the result into this storage. cast away const.
            getStorage().getResult() = ((VPathString&)newName).getResult();
            CDBG(   << "    ...rename failed: " 
                    << ((VPathString&)newName).getResult().asSystemMessage() 
                    << endl);
        return CancelButton;
        }
    }
        
    return getStorage().rename(newName) ? YesButton : CancelButton;
    
}

//**********************************************************************
//
VComponentRootStorage::VComponentRootStorage(VString *lpStorageName)
{
    Storage = new CRRootStorage(lpStorageName);
}

//**********************************************************************
//
VComponentSubStorage::VComponentSubStorage(VComponentStorage &storageParent, VString *lpStorageName)
{
    Storage = new CRSubStorage(storageParent.getStorage(), lpStorageName);
}
