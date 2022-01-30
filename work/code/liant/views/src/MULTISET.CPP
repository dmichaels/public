// $Header:   Q:/views/common/vcs/MULTISET.CPV   1.7   Mar 24 1997 16:12:38   scottt  $

//	multiset.cpp
//
//	VMultiSet class implementation [Common]
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
//	10/04/95 pkt	created.
//	10/12/95 pkt	added archiver tag.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "multiset.h"
#if !defined(CV_NOARCHIVER)
# include "archiver.h"
# include "archvrtg.h"
#endif
#include <stdio.h>

defineClass(VMultiSet, VDictionary)
defineArchiveRevision(VMultiSet, 0)

// ---------------------------------------------------------------------------
// Construct an empty multiset.
//
VMultiSet::VMultiSet() :
	VDictionary(MultiAssocCls)
{
}

// ---------------------------------------------------------------------------
// Construct a multiset which is a copy of another.
//
VMultiSet::VMultiSet(const VMultiSet& ms) :
	VDictionary(ms)
{
}

// ---------------------------------------------------------------------------
//
VMultiSet::~VMultiSet()
{
}

// ---------------------------------------------------------------------------
//
long VMultiSet::getValueAtKey(const VObject *key) const
{
	MultiAssoc *old;
	assoc->key = (VObject *)key;
	old = (MultiAssoc *)find(assoc);
	assoc->key = 0;
	if (old != 0) {
		return long(old->value);
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Add the association (key, value) to this VMultiSet.
// Return TRUE if this association was successfully added.
// Fails if value < 0, or if an equivalent key in the multiset already
// had an association. 
//
boolean VMultiSet::atKeyPut(VObject *key, long value)
{
	if (value > 0) {
		MultiAssoc *a;
		if ((a = (MultiAssoc *)getAssoc(key, (VObject *)value)) != 0) {
			if (add(a) != 0) {
				return TRUE;
			}
			else {
				delete a;
			}
		}
	}
	else if (value == 0) {
		removeKey(key);
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
VMultiSet& VMultiSet::operator=(const VMultiSet& object)
{
	if (&object != this) {
		copyFrom((VMultiSet *)&object);
	}
	return *this;
}

#if 0
// Removed these functions, because they cause this VMultiSet to capture
// the parameter collection's objects in a totally unmanageable way.
//
// ---------------------------------------------------------------------------
//
VMultiSet& VMultiSet::operator+=(const VMultiSet& ms)
{
	DO (ms, MultiAssoc, a)
		VObject *key = a->key;
		atKeyPut(key, getValueAtKey(key) + long(a->value));
	END
}

// ---------------------------------------------------------------------------
//
VMultiSet& VMultiSet::operator+=(const VCollection& coll)
{
	DO (coll, VObject, key)
		atKeyPut(key, getValueAtKey(key) + 1);
	END
}

// ---------------------------------------------------------------------------
//
VMultiSet& VMultiSet::operator-=(const VMultiSet&)
{
	DO (ms, MultiAssoc, a)
		VObject *key = a->key;
		long old_value = getValueAtKey(key);
		if (old_value > 0) {
			long new_value = old_value - long(a->value);
			if (new_value >= 0) {
				atKeyPut(key, new_value);
			}
			else {
				removeKey(key);
			}
		}
	END
}

// ---------------------------------------------------------------------------
//
VMultiSet& VMultiSet::operator-=(const VCollection&)
{
	DO (coll, VObject, key)
		long old_value = getValueAtKey(key);
		if (old_value > 0) {
			atKeyPut(key, old_value - 1);
		}
	END
}
#endif

// ---------------------------------------------------------------------------
// Add object 'object' to this set and return it. If a matching object is
// already in the set (matching by way of the isEqual member function) then
// return that object.  Adding increases the object's replication value by 1.
//
VObject *VMultiSet::addKey(VObject *object)
{
	MultiAssoc *old;
	assoc->key = object;
	old = (MultiAssoc *)find(assoc);
	if (old != 0) {
		old->value = (VObject *)(long(old->value) + 1);
		return old->key;
	}
	else {
		atKeyPut(object, 1);
		return object;
	}
}

// ---------------------------------------------------------------------------
// Return the object in this multiset that has the same value as 'object'
// (matches object 'object' by way of the isEqual member function) otherwise
// return a null pointer.
//
VObject *VMultiSet::findKey(const VObject *object) const
{
	MultiAssoc *old;
	assoc->key = (VObject *)object;
	old = (MultiAssoc *)find(assoc);
	assoc->key = 0;
	if (old != 0) {
		return old->key;
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VMultiSet contains an object equivalent
// to 'object' by isEqual, otherwise return FALSE.
//
boolean VMultiSet::containsKey(const VObject *object) const
{
	return getValueAtKey(object) > 0;
}

// ---------------------------------------------------------------------------
// Remove and return the object equivalent to 'object' (that
// is, this isEqual() member function returns TRUE) if 'object'
// is in this multiset, otherwise return a null pointer.
// Removal consists of reducing the object's value by 1.
//
VObject *VMultiSet::removeKey(const VObject *object)
{
	MultiAssoc *old;
	assoc->key = (VObject *)object;
	old = (MultiAssoc *)find(assoc);
	assoc->key = 0;
	if (old != 0) {
		VObject *old_key = old->key;
		old->value = (VObject *)(long(old->value) - 1);
		if (long(old->value) <= 0) {
			VDictionary::removeKey(old_key);
		}
		return old_key;
	}
	// not found
	return 0;
}

// ===========================================================================
// VMultiSet::MultiAssoc
//
// ---------------------------------------------------------------------------
//
defineNestedClass(VMultiSet, MultiAssoc, VAssoc)

// ---------------------------------------------------------------------------
// Contruct an MultiAssoc object key k and value v set to 0L.
//
VMultiSet::MultiAssoc::MultiAssoc()
{
	key = 0;
	value = (VObject *)0L;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Read the key and value of this MultiAssoc from the VArchiver a.
//
void VMultiSet::MultiAssoc::getFrom(VArchiver &a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VMultiSetArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VMultiSetArchiveTag));
		return;
	}

	key = a.getObject(0); 

	long l;
	a >> l;
	value = (VObject *)l;
}

// ---------------------------------------------------------------------------
//
void VMultiSet::MultiAssoc::getFrom(VArchiver& a,
					    VObject * /* data */ , boolean /* do_realize */ )
{
#ifndef __SC__
	MultiAssoc::getFrom(a);
#else
	getFrom(a);
#endif
}

// ---------------------------------------------------------------------------
// Store the key and value of this MultiAssoc into the VArchiver a.
//
void VMultiSet::MultiAssoc::putTo(VArchiver& a)
{
	a << VMultiSetArchiveTag;

	a.putObject(key);
	a << (long)value;
}
#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Contruct an MultiAssoc object with key k and value v.
//
VMultiSet::MultiAssoc::MultiAssoc(VObject *k, long v) 
{ 
	key = k;
	value = (VObject *)v; 
}

// ---------------------------------------------------------------------------
//
VMultiSet::MultiAssoc::MultiAssoc(const MultiAssoc &ia)
{	
	init();
	copyFrom((MultiAssoc *)(&ia));
}
