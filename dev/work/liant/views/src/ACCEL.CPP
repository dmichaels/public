// $Header:   Q:/views/common/vcs/ACCEL.CPV   1.8   Mar 24 1997 16:07:54   scottt  $ 

//	accel.cpp
//
//	VAccelerator implementation [Common]
//
//	Allegris Foundation 1.1.00
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
//	04/07/93 pmcm	changed id to VObject *
//	07/19/93 pmcm	added VArchiver support; preened.
//	07/23/93 pmcm	updated name arg for new VNotifier::findWindow()
//	07/27/93 pmcm	removed free()
//					added client and method indices; 
//					updated getFrom() and putTo() to use client list
//					updated VClass::findCallback() calls
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	09/02/93 pmcm	updated findCallback() calls
//	09/22/93 pmcm	added archive tag
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//  02/02/94 pmcm 	added archive tag check
//  02/09/94 pmcm 	added ClientCallbackMismatch check
//  02/09/94 pmcm 	updated defineArchiveTag use
//	11/11/94 dss	Archiver tags macros.
//	05/16/95 pkt	preened.
//	01/12/96 tlf	Fixed coding error in call().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "accel.h"
#include "menuitem.h"
#include "control.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "notifier.h"
#include "cllbckls.h"	// for callbackList class
#endif

defineClass(VAccelerator, VObject)

#ifndef CV_NOARCHIVER
defineArchiveRevision(VAccelerator, 0)
#endif

// ---------------------------------------------------------------------------
//
VAccelerator::VAccelerator()
#ifndef CV_NOARCHIVER
			:	clientIndex(-1L),
				methodIndex(-1L)
#endif	
{
	pClient = 0;
	pMethod = 0;
	wKey = 0;
}

// ---------------------------------------------------------------------------
// Construct an `VAccelerator` object for virtual key code 'key'.
//
VAccelerator::VAccelerator(int key)
#ifndef CV_NOARCHIVER
			:	clientIndex(-1L),
				methodIndex(-1L)
#endif	
{
	pClient = 0;
	pMethod = 0;
	wKey = key;
}

// ---------------------------------------------------------------------------
// Arrange to call method 'mthd' on object 'obj' when the virtual key
// code 'key' is pressed.
//
VAccelerator::VAccelerator(int key, VObject *obj, method mthd)
#ifndef CV_NOARCHIVER
			:	clientIndex(-1L),
				methodIndex(-1L)
#endif	
{
	pClient = obj;
	pMethod = mthd;
	wKey = key;
}

// ---------------------------------------------------------------------------
// Perform the callback associated with this `VAccelerator`.
//
void VAccelerator::call()
{
	if (!pClient) {
		return;
	}
	if (pMethod == NIL_METHOD) {
		if (pClient->isA(VMenuItemCls)) {
                    if (((VMenuItem*)pClient)->enabled()) {
  			((VMenuItem *)pClient)->selected();
  		    }
		}
		else if (pClient->isA(VControlCls)) {
		    VControl* control = (VControl*)pClient;
                    if (!control->isHidden() && control->isEnabled()) {
			((VControl *)pClient)->notify(ACCELcode);
                    }
		}
	}
	else {
		pClient->perform(pMethod);
	}
}


// ---------------------------------------------------------------------------
// Compare `VAccelerator` objects by virtual key. That is, return TRUE
// if the `VAccelerator` object 'obj' and this `VAccelerator` object 
// contain the same virtual key code.  Otherwise, return FALSE if they 
// don't have the same virtual key code or if 'obj' is not 
// an `VAccelerator` object.
//
boolean VAccelerator::isEqual(VObject *obj) const
{
	if (!obj->isA(VAcceleratorCls)) {
		return FALSE;
	}
	return (((VAccelerator *)obj)->wKey == wKey) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Return a hash value for this `VObject` based on its virtual key code.
//
HashKey VAccelerator::hash() const
{
	return wKey & HASH_KEY_MASK;
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// ---------------------------------------------------------------------------
// Put this object to the archive.  This method is called from 
// VArchiver::putObject().
//
void VAccelerator::putTo(VArchiver& a)
{
	a << VAcceleratorArchiveTag;

	// accelerator key

	a << wKey;

	// client

	a.putObject(notifier->getClientName(pClient, clientIndex), TRUE);

	// callback method

	const char *className 	= 0;			
	const char *methodName	= 0;			
	VCallbackList::findCallback(pMethod, className,  methodName, methodIndex);
	a << (char *) className;	
	a << (char *) methodName;	
}

// ---------------------------------------------------------------------------
// Get this object from the archive by retrieving the object 
// data of the base class and then the object data of this class.
//
// This method is invoked by the getFrom() of this most-derived class or by
// the getFrom() of a derived class.
//
void VAccelerator::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VAcceleratorArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VAcceleratorArchiveTag));
		return;
	}

	// key

	a >> wKey;

	// client

	VString client_name;
	a.getObject(&client_name);
	pClient = notifier->getClient(client_name, &clientIndex);

	// callback method

	char		methodName[256];							
	char *		methodNamePtr = methodName;
	char		className[256];
	char *		classNamePtr = className;

	a >> classNamePtr;
	a >> methodNamePtr;

	pMethod = VCallbackList::findCallback(classNamePtr, methodNamePtr, 
															&methodIndex);

	if (pClient != 0 && pMethod != 0) {
		if (!pClient->isA(VClass::of(classNamePtr))) {
			a.abort(VArchiver::ClientCallbackMismatch);
			return;
		}
	}
}

// ---------------------------------------------------------------------------
//
void VAccelerator::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------

#endif // ARCHIVER	
// ===========================================================================


