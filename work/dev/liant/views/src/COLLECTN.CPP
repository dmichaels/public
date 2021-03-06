// $Header:   Q:/views/common/vcs/COLLECTN.CPV   1.9   Mar 24 1997 16:08:34   scottt  $ 

//	collectn.cpp
//
//	VCollection implementation [Common]
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
//	08/09/93 jmd	removed free
//	09/26/93 jmd	preened
//	01/26/93 jmd	preened
//	01/13/94 dgm	merged and preened
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	05/19/95 pkt	selected Motif as common source.
//	10/06/95 pkt	updated use of 'const'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "collectn.h"
#include "archiver.h"
#include "iterator.h"

// ---------------------------------------------------------------------------
//
defineAbstractClass(VCollection, VObject)

// ---------------------------------------------------------------------------
// Returns the first object in this VCollection or 0 if it is empty.
//
VObject *VCollection::first() const
{
	VIterator i(this);
	return i++;
}

// ---------------------------------------------------------------------------
// Return the last object in this VCollection or 0 if this
// VCollection is empty.
//
VObject *VCollection::last() const
{
	if (empty()) {
		return 0;
	}
	VIterator i(this);
	i.end();
	return i--;
}

// ---------------------------------------------------------------------------
// Replace and return the object equivalent to old with the object
// New. Otherwise, if there is no equivalent object, return 0.
//
VObject *VCollection::replace(VObject *old, VObject *New)
{
	if ((old = find(old)) != 0) {
		return replaceSame(old, New);
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Remove and return the object equivalent to obj (i.e., the IsEqual()
// method returns TRUE) if obj is in this VCollection, otherwise return 0.
//
VObject *VCollection::remove(VObject *obj)
{
	if ((obj = find(obj)) != 0) {
		return removeSame(obj);
	}
	return 0;
}

// ---------------------------------------------------------------------------
// For each object in this VCollection perform method m, in
// ascending order, (see methodOf() in VObject).
//
void VCollection::doPerform(method m) const
{
	DO (this, VObject, anId)
		anId->perform(m);
	END
}

// ---------------------------------------------------------------------------
// For each object in this VCollection perform method m, in
// ascending order, with the long argument param.
//
void VCollection::doPerform(method m, long param) const
{
	DO (this, VObject, anId)
		anId->perform(m, param);
	END
}

// ---------------------------------------------------------------------------
// For each object in this VCollection perform method m, in
// ascending order, with long arguments param1 and param2.
//
void VCollection::doPerform(method m, long param1, long param2) const
{
	DO (this, VObject, anId)
		anId->perform(m, param1, param2);
	END
}

// ---------------------------------------------------------------------------
// For each object in this VCollection perform method m, in
// ascending order, with long arguments param1, param2, and param3.
//
void VCollection::doPerform(method m, long param1, long param2, long param3) const
{
	DO (this, VObject, anId)
		anId->perform(m, param1, param2, param3);
	END
}

// ---------------------------------------------------------------------------
// Delete each object in this VCollection, then delete this VCollection.
//
void VCollection::freeAll()
{
	freeContents();
	delete this;		// jmd (was free)
}

// ---------------------------------------------------------------------------
// For each object in this VCollection perform method m in descending order.
//
void VCollection::reverseDo(method m) const
{
	REVERSEDO (this, VObject, anId)
		anId->perform(m);
	END
}

// ---------------------------------------------------------------------------
// For each object in this VCollection perform method m, in
// descending order, with the long argument param.
//
void VCollection::reverseDo(method m, long param) const
{
	REVERSEDO(this, VObject, anId)
		anId->perform(m, param);
	END
}

// ---------------------------------------------------------------------------
// Add the contents of pColl to this VCollection
//
void VCollection::addTo(const VCollection *pColl)
{
	DO (pColl, VObject, anId)
		add(anId);
	END
}

// ---------------------------------------------------------------------------
// If obj is equivalent to an object in this VCollection,
// then replace that object with obj and
// return that object, otherwise add obj and return it;
//
VObject *VCollection::filter(VObject *obj)
{
	VObject *old = obj;

	if (find(obj)) {
		old = remove(obj);
	}
	add(obj);
	return old;
}

// ---------------------------------------------------------------------------
// A VCollection can copy another VCollection to itself.
//
boolean VCollection::copyFrom(VObject *obj)
{
	if (!obj->isA(VCollectionCls)) {
		return FALSE;
	}
	reset();
	DO ((VCollection *)obj, VObject, o)
		add(o);
	END
	return TRUE;
}

// ---------------------------------------------------------------------------
//
VCollection::VCollection()
{
	init();
}

// ---------------------------------------------------------------------------
//
VCollection::VCollection(const VCollection &c)
{
	init();
	copyFrom((VCollection *)(&c));
}

// ---------------------------------------------------------------------------
//
VCollection& VCollection::operator=(const VCollection &c)
{
	if (&c != this) {
		reset();
		init();
		copyFrom((VCollection *)(&c));
	}
	return *this;
}
