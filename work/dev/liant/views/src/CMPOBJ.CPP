// $Header:   Q:/views/win/vcs/cmpobj.cpv   1.4   Jun 03 1997 14:15:14   scottt  $
//
//	cmpobj.cpp
//
//  Compound Document Object
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	02/01/95 dss	Initial release.
//  12/08/95 dss    Added defineClass.
//  12/12/95 dss    added realize, inner VComponentObject.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#include "pre.h"

#include "initguid.h"		// for DEFINE_GUID()

DEFINE_GUID(IID_ViewsObject, 0x49E572C0L, 0xEE61, 0x101B, 0xA3, 0x2D, 0x08, 0x00, 0x17, 0x08, 0x81, 0x74);

defineClass( VComponentObject, VObject)

long VComponentObject::storageCreationCount = 0;

// defineClass(VComponentObject, VObject)

//**********************************************************************
//
// construct an Controlling CUnknown VObject
//
VComponentObject::VComponentObject()
	: CUnknown(0)
{
	init(0);
}

//**********************************************************************
//
// construct an Controlling CUnknown VObject
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
VComponentObject::VComponentObject(LPVOBJECT lpVObject)
	: CUnknown(new XUnknown(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	init(lpVObject);
}

//**********************************************************************
//
// construct an inner (aggregate child) Unknown VObject
//
// lpVObject - the parent VObject
// refCUnkOuter - the controlling unknown
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
VComponentObject::VComponentObject(LPVOBJECT lpVObject, REFCUNKNOWN refCUnkOuter)
	: CUnknown(refCUnkOuter, *this)
#pragma warning (default : 4355)  // Turn the warning back on
{
	init(lpVObject);
}

//**********************************************************************
//
VComponentObject::~VComponentObject()
{
	// Release the Storage
	if (Storage) {
		delete Storage;
		Storage = 0;
	} 
	
}

//********************************************************************
//
void VComponentObject::init(LPVOBJECT lpVObject)
{
	this->lpVObject = lpVObject;
	Storage = 0;

	exposeInterface(IID_ViewsObject, "VComponentObject");
}


//**********************************************************************
//
// realize a CUnknown VObject
// lpUnkOuter - the controlling IUnknown. Equal to 0, if this VComponentObject is the
// controlling unknown.
//
//!!!dss 12/12/95 - HACK: the arg should be a base unknown type common to 
// CRUknown and CUnknown. Should be an abstraction of IUnknown.
//
void VComponentObject::realize(LPCUNKNOWN lpCUnkOuter)
{
    //!!!dss 12/12/95: NOTE this realizes a controlling unknown.
    // Also, need to realize an aggregate child but CRUnkown and CUnkown
    // must share a common ancestry, first.
	CUnknown::realize(lpCUnkOuter, new XUnknown(this));
}

//**********************************************************************
//
// This function creates root storage or sub-storage.
// If no parent is given, then root storage is created. IF no parent 
// and no name is given, then a temporary file in root storage is
// created.
//
// If a parent is given, and no name is given, then a uniquely
// named sub-storage is created, based on the object's class name.
//
boolean VComponentObject::createStorage(VComponentObject *storageParent, VString *lpStorageName)
{
	// if parent has storage, use that	
	if (storageParent && storageParent->hasStorage()) {
		VString		tempString;
	
		// if no storage name, create class based unique names for sub-storages.
		if (lpStorageName == 0) {
			lpStorageName = &tempString;

			lpStorageName->prints("%s%d", getClassName(), 
								  storageCreationCount++);
		}

		Storage = new VComponentSubStorage(storageParent->getStorage(), lpStorageName);
		
	}

	// else create root storage
	else {
		Storage = new VComponentRootStorage(lpStorageName);
	}
	  
	// TO DO: do I need this?
	// Storage->uponRename( this, methodOf(VComponentObject, storageRenamed));

	// TO DO: should this be Storage->realize()? Should open errors be 
	// reported?
	return Storage->create() == YesButton;
}
 
