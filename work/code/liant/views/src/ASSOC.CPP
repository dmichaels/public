// $Header:   Q:/views/common/vcs/ASSOC.CPV   1.8   Mar 24 1997 16:08:02   scottt  $ 

//	assoc.cpp
//
//	VAssoc class implementation [Common]
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
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	08/09/93 jmd	removed free
//	10/25/93 dgm	3.0 changes.
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "assoc.h"
#include "archiver.h"
#include <stdio.h>

// ---------------------------------------------------------------------------
//
defineClass(VAssoc, VObject)

// ---------------------------------------------------------------------------
// Construct an VAssoc object with a 0 key and value.
//
VAssoc::VAssoc()
{
	VAssoc::init();
}

// ---------------------------------------------------------------------------
// This hash function returns the result of calling the hash function on
// the key object.
//
HashKey VAssoc::hash() const
{
	return key ? key->hash() : 0L;
}

// ---------------------------------------------------------------------------
// Return TRUE if the key of this VAssoc is equivalent to the key of VAssoc
// object a, otherwise return FALSE.   Comparison is performed via the
// isEqual() method. 
//
boolean VAssoc::isEqual(VObject *a) const
{
	VObject *aKey;
	if (a == 0) {
		return FALSE;
	}
	if (a->isA(VAssocCls)) {
		aKey = ((VAssoc *)a)->key;
		return key->isEqual(aKey);
	}
	return FALSE;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Store the key and value of this VAssoc into the VArchiver a.
//
void VAssoc::putTo(VArchiver& a)
{
	a.putObject(key);
	a.putObject(value);
}

// ---------------------------------------------------------------------------
// Read the key and value of this VAssoc from the VArchiver a.
//
void VAssoc::getFrom(VArchiver& a)
{
	key = a.getObject(0); 
	value = a.getObject(0); 
}

// ---------------------------------------------------------------------------
//
void VAssoc::getFrom(VArchiver& a,
					 VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Contruct an VAssoc object with key k and value v.
//
VAssoc::VAssoc(VObject *k, VObject *v) 
{ 
	key = k;
	value = v; 
}

// ---------------------------------------------------------------------------
//
VAssoc::VAssoc(const VAssoc& a)
{ 
	init();
	copyFrom((VAssoc *)(&a));
}

// ---------------------------------------------------------------------------
//
VAssoc& VAssoc::operator=(const VAssoc &a)
{
	if (&a != this) {
		reset();
		init();
		copyFrom((VAssoc *)(&a));
	}
	return *this;
}

// ---------------------------------------------------------------------------
// Copy VAssoc obj to this VAssoc.
//
boolean VAssoc::copyFrom(VObject *obj)
{
	if (!obj->isA(VAssocCls)) {
		return FALSE;
	}
	VAssoc *as = (VAssoc *)obj;
	key = as->key;
	value = as->value;
	return TRUE;
}
