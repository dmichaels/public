// $Header:   Q:/views/common/vcs/DICTINRY.CPV   1.9   Mar 24 1997 16:08:44   scottt  $ 

//	dictinry.cpp
//
//	VDictionary class implementation [Common]
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
//	07/14/92 jmd	fixed logic bug in atKeyPut()
//	04/07/93 pmcm	changed id to VObject *
//	07/30/93 pkt	eliminated #ifdef macintosh.
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	04/19/95 jld	copyFrom() deletes assoc before reassigning it
//	05/19/95 pkt	merged for common source.
//	10/06/95 pkt	updated use of 'const'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "dictinry.h"
#include "iterator.h"
#include "str.h"

extern "C" {
#include <stdio.h>
}

// ---------------------------------------------------------------------------
//
defineClass(VDictionary, VSet)

// ---------------------------------------------------------------------------
// Construct a VAssoc derived object with key and value.
//
VAssoc *VDictionary::getAssoc(VObject *key, VObject *value)
{
	VAssoc *a = (VAssoc *)assoc->getIsA()->construct();
	a->key = key;
	a->value = value;
	return a;
}

// ---------------------------------------------------------------------------
// Add the association (key,value) to this VDictionary. Return 
// TRUE if the association was successfully added. Otherwise, return FALSE
// indicating that the association was already in this VDictionary. 
//
boolean VDictionary::atKeyPut(VObject *key, VObject *value)
{
	VAssoc *a;
	if ((a = getAssoc(key, value)) != 0) {
		if (add(a)) {
			return TRUE;
		}
		else {
			delete a;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Add the association (key,value) to this VDictionary where
// value is not an object referent. Return TRUE if the association was 
// successfully added.  Otherwise, return FALSE indicating that the
// association was already in this VDictionary.
//
boolean VDictionary::atKeyPut(VObject *key, long value)
{
	VAssoc *a;
	if ((a = getAssoc(key, (VObject *)value)) != 0) {
		if (add(a)) {
			return TRUE;
		}
		else {
			delete a;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Copy the contents of VDictionary object obj to this VDictionary.
//
boolean VDictionary::copyFrom(VObject *o)
{
	if (!o->isA(VDictionaryCls)) {
		return FALSE;
	}
	reset();
	VDictionary *d = (VDictionary *)o;
	DO (d, VAssoc, a)
		add(a->copy());
	END
	if (assoc != 0) {
		delete assoc;
	} 
	assoc = (VAssoc *)d->assoc->copy();
	return TRUE;
}

// ---------------------------------------------------------------------------
// Construct a dictionary which may hold 25 associations.
//
VDictionary::VDictionary()
{
	assoc = new VAssoc;
	expandTo(50);
}

// ---------------------------------------------------------------------------
// Construct a dictionary which may hold n associations.
//
VDictionary::VDictionary(int n)
{
	assoc = new VAssoc;
	expandTo(n << 1);
}

// ---------------------------------------------------------------------------
// Construct a dictionary which uses objects of VClass type cls to
// hold its associations.
//
VDictionary::VDictionary(const VClass *cls)
{
	assoc = (VAssoc *)cls->construct();
}

// ---------------------------------------------------------------------------
// Delete this VDictionary as well as all of the associations
// contained. Do not free the key and value objects.
//
VDictionary::~VDictionary()
{
	reset();
	if (assoc != 0) {
		delete assoc;		// jmd (was free)
	}
}

// ---------------------------------------------------------------------------
// Return the value at the specified key in this VDictionary. 
// If an entry matching key is not found, then return a null pointer. 
//
VObject *VDictionary::getValueAtKey(const VObject *key) const
{
	VAssoc *old;
	assoc->key = (VObject *)key;
	old = (VAssoc *)find(assoc);
	assoc->key = 0;
	if (old != 0) {
		return old->value;
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VDictionary contains association with key, 
// otherwise return FALSE. 
//
boolean VDictionary::includesKey(const VObject *key) const
{
	assoc->key = (VObject *)key;
	boolean b = find(assoc) != 0;
	assoc->key = 0;
	return b;
}

// ---------------------------------------------------------------------------
// Remove the association with the given key from this VDictionary
// and return its associated value, otherwise return a null pointer.
//
VObject *VDictionary::removeKey(const VObject *aKey)
{
	VAssoc  *old; 
	VObject *aValue = 0;
	assoc->key = (VObject *)aKey;
	old = (VAssoc *)find(assoc);
	assoc->key = 0;
	if (old != 0) {
		aValue = old->value;
		remove(old);
		delete old;		// jmd (was free)
	}
	return aValue;
}

// ---------------------------------------------------------------------------
// Return a new VSet containing all the key objects of the associations in
// this VDictionary.  The caller is responsible for freeing the returned set. 
//
VSet *VDictionary::setOfKeys() const
{
	VSet *s = new VSet;

	DO (this, VAssoc, a)
		s->add(a->key);
	END 
	return s;
}

// ---------------------------------------------------------------------------
// Remove and free all of the associations from this VDictionary. 
// Do not free the key and value objects.
//
void VDictionary::reset()
{
	freeContents();
}

// ---------------------------------------------------------------------------
//
VDictionary::VDictionary(const VDictionary &d) 
{ 
	init();
	copyFrom((VDictionary *)(&d));
}

// ---------------------------------------------------------------------------
//
VDictionary& VDictionary::operator=(const VDictionary &d) 
{ 
	if (&d != this) {
		reset();
		init();
		copyFrom((VDictionary *)(&d));
	}
	return *this;
}
