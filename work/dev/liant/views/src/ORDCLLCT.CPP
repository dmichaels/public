// $Header:   Q:/views/common/vcs/ORDCLLCT.CPV   1.16   Mar 24 1997 16:12:46   scottt  $ 

//	ordcllct.cpp
//
//	VOrdCollect class implementation [Common]
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
//	07/31/93 pkt	eliminated #ifdef macintosh.
//	07/31/93 jmd	preened, added some consts
//	09/17/93 dgm	(jmd) changed idAt() to be const.
//	09/21/93 pmcm	changed idptr to VObject **
//	10/07/93 dgm	added support for archiving VOrdCollects of GUI objects;
//					primarily for proper dialog-editor clipboard support.
//	10/25/93 pkt	changed NIL to NULL.
//	11/12/93 pkt	removed call to free().
//	11/17/93 pkt	fixed idAt bug introduced by 3.0 merge.
//	01/12/94 jmd	fixed return value of replaceAt
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	01/30/94 dgm	simplified ++ expression in removeAt(integert)
//					to avoid apparent Borland C++ 4.0 compiler bug.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	03/06/95 jld	changed freeContents to call VObjArray::freeContents
//	05/19/95 pkt	merged for common source.
//	09/06/95 pkt	removed OFI code.
//	10/06/95 pkt	updated use of 'const'.
//	12/20/95 pkt	fixed 'putTo' to put 'contents' as a temprary object.
//	04/19/96 pkt	updated so that a null pointer cannot be added to a
//					VOrdCollect.
//	04/26/96 pkt	reverted 4/19 change.
//	04/30/96 pkt	added 'allFollowedBy' and 'allPrecededBy'.
//	04/30/96 pkt	changed 'prev' to set the iterator to -1 when done, and
//					'end' to set the iterator to -1 if the collection is
//					empty.
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "ordcllct.h"
#include "iterator.h"
#include "archiver.h"

// ---------------------------------------------------------------------------
//
defineClass(VOrdCollect, VCollection)

// ---------------------------------------------------------------------------
// Construct an VOrdCollect with room for ten objects.
//
VOrdCollect::VOrdCollect()
{
	nextSlot = 0;
}

// ---------------------------------------------------------------------------
// Construct an VOrdCollect with room for n objects.
//
VOrdCollect::VOrdCollect(unsigned n)
{
	nextSlot = 0;
	expandTo(n);
}

// ---------------------------------------------------------------------------
// Construct an VOrdCollect from VOrdCollect c.
//
VOrdCollect::VOrdCollect(const VOrdCollect& c)
{
	init();
	copyFrom((VOrdCollect *)(&c));
}

// ---------------------------------------------------------------------------
// Return the number of objects in this VOrdCollect.
//
integer VOrdCollect::count() const
{
	return nextSlot;
}

// ---------------------------------------------------------------------------
// Replace object old with object newobj in this VOrdCollect.
// Return old if it is in this VOrdCollect, otherwise return a null pointer.
//
VObject *VOrdCollect::replaceSame(VObject *old, VObject *newobj)
{
	integer n = indexOfSame(old);
	old = idAt(n);
	if (n >= 0) {
		replaceAt(n, newobj);
	}
	return old;
}

// ---------------------------------------------------------------------------
// Replace the object at index n in this VOrdCollect with object newobj.
//
VObject *VOrdCollect::replaceAt(integer n, VObject *newobj)
{
	if ((n < 0) || (n >= nextSlot)) {
		return 0;
	}
	VObject *old = idAt(n);
	contents.atPutId(n, newobj);
	return old;
}

// ---------------------------------------------------------------------------
// Add obj to this VOrdCollect at the next available slot
// expanding it if necessary. Return obj.
//
VObject *VOrdCollect::add(VObject *obj)
{
	contents.atPutId(nextSlot, obj);
	nextSlot++;
	return obj;
}

// ---------------------------------------------------------------------------
// Delete each object in this VOrdCollect.
//
void VOrdCollect::freeContents()
{
	contents.freeContents();
	nextSlot = 0;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Load an VOrdCollect from the VArchiver a.
//
void VOrdCollect::getFrom(VArchiver& a)
{
	a >> nextSlot;
	a.getObject(&contents);
}

void VOrdCollect::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	a >> nextSlot;
	a.getObject(&contents, parent, do_realize);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Return obj if obj is in this VOrdCollect.
//
VObject *VOrdCollect::findSame(const VObject *obj) const
{
	return contents.findSame(obj);
}

// ---------------------------------------------------------------------------
// Return an object in this VOrdCollect equivalent to obj via the
// isEqual() method, otherwise return a null pointer if none is found.
//
VObject *VOrdCollect::find(const VObject *obj) const
{
	return contents.find(obj);
}

// ---------------------------------------------------------------------------
// Return the index of the object equivalent to obj in this
// VOrdCollect if present, otherwise return -1.
//
integer VOrdCollect::indexOf(VObject *obj) const
{
	return contents.indexOf(obj);
}

// ---------------------------------------------------------------------------
// Return the index of obj in this VOrdCollect if present, 
// otherwise return -1.
//
integer VOrdCollect::indexOfSame(VObject *obj) const
{
	return contents.indexOfSame(obj);
}

// ---------------------------------------------------------------------------
// Set the VIterator i to the first object in this VOrdCollect.  
// 
void VOrdCollect::start(VIterator& i) const
{
	i = 0L;
	if (nextSlot == 0) {
		i = (VObject *)0;
	}
	else {
		i = contents.idAt(0);
	}
}

// ---------------------------------------------------------------------------
// Set VIterator i to the end of this VOrdCollect.
//
void VOrdCollect::end(VIterator& i) const
{
	i = nextSlot - 1;
	if (nextSlot != 0) {
		i = contents.idAt(nextSlot - 1);
	}
	else {
		i = (VObject *)0;
	}
}

// ---------------------------------------------------------------------------
// Set VIterator i to the end of this VOrdCollect.
//
void VOrdCollect::endIterator(VIterator& i) const
{
	end(i);
}

// ---------------------------------------------------------------------------
// Return the current object refered to by the VIterator i and
// increment VIterator i to reference the next object.
//
VObject *VOrdCollect::next(VIterator& i) const
{
	integer n = i;
	VObject *o = i;
	if (n < nextSlot) {
		if (++n == nextSlot) {
			i = (VObject *)0;
		}
		else {
			i = contents.idAt(n); 
		}
		i = n;
	}
	else {
		i = (VObject *)0;
	}
	return o;
}

// ---------------------------------------------------------------------------
// Return the current object referred to by the VIterator i and
// decrement VIterator i to reference the previous object.
//
VObject *VOrdCollect::prev(VIterator& i) const
{
	integer n = i;
	VObject *o = i;
	if (n >= 0) {
		if (-- n >= 0) {
			i = contents.idAt(n); 
		}
		else {
			i = (VObject *)0;
		}
		i = n;
	}
	else {
		i = (VObject *)0;
	}
	return o;
}

// ---------------------------------------------------------------------------
// Override VCollection::allFollowedBy to tell whether the VIterator i 
// is pointing past the end of the entire collection.
//
boolean VOrdCollect::allFollowedBy(VIterator&i) const
{
	integer n = i;
	return n >= nextSlot;
}

// ---------------------------------------------------------------------------
// Override VCollection::allPrecededBy to tell whether the VIterator i 
// is pointing before the start of the entire collection.
//
boolean VOrdCollect::allPrecededBy(VIterator&i) const
{
	integer n = i;
	return n < 0;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store this VOrdCollect into the VArchiver a.
//
void VOrdCollect::putTo(VArchiver& a)
{
	a << nextSlot;
	contents.expandTo(nextSlot);
	a.putObject(&contents, TRUE);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Remove obj from this VOrdCollect shifting elements to fill
// the gap and return TRUE.  Return FALSE if obj is not found.
//  
VObject *VOrdCollect::removeSame(VObject *obj)
{
	integer i;

	if ((i = indexOfSame(obj)) < 0) {
		return 0;
	}
	return removeAt(i);
}

// ---------------------------------------------------------------------------
// Remove and return the object at index i shifting the objects
// with equal or greater indexes to the left. A null referent is returned if
// index i is out of bounds.
//
VObject *VOrdCollect::removeAt(integer i)
{
	hugeObjPointerPointer p;
	VObject *obj = 0;

	if ((i >= count()) || (i < 0)) {
		return 0;
	}
	if ((p = (hugeObjPointerPointer)contents.getData()) != 0) {
		// i is one past found object
		for (obj = p[i], i++; i < nextSlot; i++) {
			p[i - 1] = p[i];
		}
		nextSlot--;
		p[nextSlot] = 0;
	}
	return obj;
} 

// ---------------------------------------------------------------------------
// Sort the objects in this VOrdCollect in the increasing
// order given by compare method.  The compare method is called
// with an object in the VOrdCollect as a single argument and
// must return an integer value <0 for less, 0 for equal, and >0
// for greater than the called object respectively.
//
void VOrdCollect::sort(boolean increasing, method compare)
{
	integer i, j;
	int k;
	hugeObjPointerPointer x;
	hugeObjPointerPointer y; 
	VObject *z;

	if (compare == 0) {
		compare = methodOf(VObject, compare);
	}
	if (nextSlot <= 1) {
		return;
	}

	if ((x = (hugeObjPointerPointer)contents.getData()) != 0) {
		for (i = 0; i < nextSlot - 1; i++ , x += 1) {
			for (j = i + 1, y = x + 1; j < nextSlot; j++ , y += 1) {
				z = *x;
				k = z->perform(compare, (long) *y);
				if (increasing && k > 0 || !increasing && k < 0) {
					*x = *y, *y = z;
				}
			} 
		}
	}
} 

// ---------------------------------------------------------------------------
// Make this VOrdCollect empty.  Its contents are not freed.
//
void VOrdCollect::reset() 
{ 
	contents.reset();
	nextSlot = 0; 
}

// ---------------------------------------------------------------------------
// Return TRUE if obj is an VOrdCollect and its contents are equal
// to the contents of this VOrdCollect.
//
boolean VOrdCollect::isEqual(VObject *obj) const
{
	if (!obj->isA(VOrdCollectCls)) {
		return FALSE;
	}
	VOrdCollect *coll = (VOrdCollect *)obj;
	return contents.isEqual((VObject *)&coll->contents);
}

// ---------------------------------------------------------------------------
// Return a hash value computed from the hash values of each of the
// objects contained in this VOrdCollect.
//
HashKey VOrdCollect::hash() const
{
	HashKey key = 0;
	DO ((VOrdCollect *)this, VObject, anId)
		key = (key + anId->hash()) & HASH_KEY_MASK;	
	END 
	return key;
}

// ---------------------------------------------------------------------------
// Insert object obj into this VOrdCollect at slot i, shifting
// the objects with greater or equal indexes to the right.
//
VObject *VOrdCollect::insertAt(integer i, VObject *obj)
{
	hugeObjPointerPointer p;
	integer j;
	if (i < 0 || i > nextSlot) {
		i = nextSlot;
	}
	if (contents.size() == nextSlot) {
		contents.expand();
	}
	p = (hugeObjPointerPointer)contents.getData();
	for (j = nextSlot; j > i; j--) {
		p[j] = p[j -1];
	}
	nextSlot++;
	p[i] = obj;
	return obj;
}

// ---------------------------------------------------------------------------
//
void VOrdCollect::expandTo(integer n) 
{ 
	contents.expandTo(n); 
}

// ---------------------------------------------------------------------------
// Return the object at index i in this VOrdCollect.
//
VObject *VOrdCollect::idAt(integer i) const
{
	if (i < nextSlot) {
		return (((VOrdCollect *)this)->contents)[i];
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
//
boolean VOrdCollect::copyFrom(VObject *obj)
{
	VOrdCollect::reset();
	return VCollection::copyFrom(obj);
}
