// $Header:   Q:/views/common/vcs/OBJARRAY.CPV   1.10   Mar 24 1997 16:12:42   scottt  $ 

//	objarray.cpp
//
//	VObjArray class implementation [Common]
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
//	09/21/93 pmcm	changed idptr to VObject **
//	10/07/93 dgm	added support for archiving VObjArrays of GUI objects;
//					primarily for proper dialog-editor clipboard support.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	03/06/95 jld	freeContents() now deletes contents in reverse order.
//	05/19/95 pkt	selected Windows as common source.
//	10/06/95 pkt	updated use of 'const'.
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "objarray.h"
#include "archiver.h"

// ---------------------------------------------------------------------------
//
defineClass(VObjArray, VContainer)

// ---------------------------------------------------------------------------
// Free all of the objects in this VObjArray.
//
void VObjArray::freeContents()
{
	integer i;
	hugeObjPointerPointer op = (hugeObjPointerPointer)getData();

	if (op == 0) {
		return;
	}
	for (i = nEle-1; i >= 0; i--) {
		if (op[i] != 0) {
			delete op[i];
		}
		op[i] = 0;
	}
}

// ---------------------------------------------------------------------------
// Set the contents of this VObjArray to all null pointers.
//
void VObjArray::reset()
{
	integer i;
	hugeObjPointerPointer op = (hugeObjPointerPointer)getData();

	if (op == 0) {
		return;
	}
	for (i = 0; i < nEle; i++) {
		*op++ = 0;
	}
}

// ---------------------------------------------------------------------------
// Construct a new VObjArray with room for ten objects.
//
VObjArray::VObjArray()
{ 	
	VObjArray::init();
	expandTo(10);
}

// ---------------------------------------------------------------------------
// Construct a new VObjArray with room for size objects.  
//
VObjArray::VObjArray(integer size)
{
	VObjArray::init();
	expandTo(size);
}

// ---------------------------------------------------------------------------
// Return the hash value (a HashKey) of this VObjArray. The hash value
// is a function of the hash values of the objects it contains.
//
HashKey VObjArray::hash() const
{
	HashKey key = 0;
	hugeObjPointerPointer q;
	hugeObjPointerPointer p = (hugeObjPointerPointer)getData();

	if (p == 0) {
		return 0;
	}
	for (q = p; q - p < nEle; q++) {
		if (*q != 0) {
			key = (key + q[0]->hash()) & HASH_KEY_MASK;
		}
	}
	return key;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VObjArray has matching objects in the
// same slots as VObjArray obj, otherwise return FALSE.  Two objects
// match if they are equal via the isEqual() method.  If equivalence by
// object address is needed use VContainer::isEqual.  
//
boolean VObjArray::isEqual(VObject *obj) const
{
	VObject *a;
	VObject *b;
	integer i;

	if (!obj->isA(VObjArrayCls)) {
		return FALSE;
	}
	VObjArray *o = (VObjArray *)obj;
	if (o->nEle != nEle) {
		return FALSE;
	}
	for (i = 0; i < nEle; i++) {
		a = idAt(i);
		b = o->idAt(i);
		if ((a == 0) && (b == 0)) {
			continue;
		}
		if ((a == 0) || (b == 0)) {
			return FALSE;
		}
		if (!a->isEqual(b)) {
			return FALSE;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Put obj into this VObjArray at position idx. If the size of
// this VObjArray is not large enough, then expand it to fit obj. 
//
void VObjArray::atPutId(integer idx, VObject *obj)
{
	if (idx == nEle) {
		expand();
	}
	atPut((unsigned)idx, (char *)&obj);
}

// ---------------------------------------------------------------------------
// If this VObjArray contains an object equivalent (via the isEqual()
// method) to obj, return TRUE, otherwise return FALSE.
//
boolean VObjArray::contains(VObject *anObj)
{
	return indexOf(anObj) >= 0;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Read an VObjArray and its contents from the VArchiver a.
//
void VObjArray::getFrom(VArchiver &a)
{
	VObject *anId;
	integer i, n;
	a >> n;
	eleSize = (int)sizeof(VObject *);
	expandTo(n);
	for (i = 0; i < n; i++) {
		anId = a.getObject(0);
		atPut((unsigned)i, (char *)&anId);
	}
}

// ---------------------------------------------------------------------------
//
void VObjArray::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VObject *anId;
	integer i, n;
	a >> n;
	eleSize = (int)sizeof(VObject *);
	expandTo(n);
	for (i = 0; i < n; i++) {
		anId = a.getObject(0, parent, do_realize);
		atPut((unsigned)i, (char *)&anId);
	}
}
#endif

// ---------------------------------------------------------------------------
// Return the object referent at the index idx in this VObjArray.
//
VObject *VObjArray::idAt(integer idx) const
{
	VObject *anId; 
	hugeObjPointerPointer c;

	if ((idx < 0) || (idx >= nEle) || ((c = (hugeObjPointerPointer)getData()) == 0))  {
		anId = 0;
	}
	else {
		anId = c[idx];
	}
	return anId;
}

// ---------------------------------------------------------------------------
// Return the index of the object equivalent (via the isEqual() method)
// to obj in this VObjArray, otherwise return -1 if it is not present. 
//
integer VObjArray::indexOf(const VObject *obj) const
{
	hugeObjPointerPointer c; 
	integer i;

	if ((obj == 0) || ((c = (hugeObjPointerPointer)getData()) == 0)) {
		i = nEle;
	}
	else {
		for (i = 0; i < nEle; i++ , c++) {
			if (*c && obj->isEqual(*c)) {
				break;
			}
		}
	}
	return (integer)(i == nEle ? -1L : i);
}

// ---------------------------------------------------------------------------
// Return the index of obj in this VObjArray, otherwise return -1
// if not present. 
//
integer VObjArray::indexOfSame(const VObject *obj) const
{
	hugeObjPointerPointer c; 
	integer i;

	if ((obj == 0) || ((c = (hugeObjPointerPointer)getData()) == 0)) {
		i = nEle;
	}
	else {
		for (i = 0; i < nEle; i++ , c++) {
			if (*c == obj) {
				break;
			}
		}
	}
	return (integer)(i == nEle ? -1L : i);
}

// ---------------------------------------------------------------------------
// Return obj if it is in this VObjArray, otherwise return a null pointer.
//
VObject *VObjArray::findSame(const VObject *obj) const
{
	hugeObjPointerPointer c; 
	VObject *p = 0;
	integer i;

	if ((obj == 0) || ((c = (hugeObjPointerPointer)getData()) == 0)) {
		i = nEle;
	}
	else {
		for (i = 0; i < nEle; i++ , c++) {
			if (*c == obj) {
				p = *c;
				break;
			}
		}
	}
	return p;
}

// ---------------------------------------------------------------------------
// Return the object in this VObjArray that is equivalent (via the
// isEqual() method) to obj, otherwise return a null pointer.
//
VObject *VObjArray::find(const VObject *obj) const
{
	hugeObjPointerPointer c; 
	VObject *p = 0;
	integer i;

	if ((obj == 0) || ((c = (hugeObjPointerPointer)getData()) == 0)) {
		i = nEle;
	}
	else {
		for (i = 0; i < nEle; i++ , c++) {
			if ((*c != 0) && obj->isEqual(*c)) {
				p = *c;
				break;
			}
		}
	}
	return p;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Store this VObjArray and its contents into the VArchiver a.
//
void VObjArray::putTo(VArchiver& a)
{
	integer i;
	a << nEle;
	for (i = 0; i < nEle; i++) {
		a.putObject(idAt(i));
	}
}
#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Construct an VObjArray from VObjArray oa.
//
VObjArray::VObjArray(const VObjArray &oa)
{
	init();
	copyFrom((VObjArray *)(&oa));
}

// ---------------------------------------------------------------------------
//
VObjArray& VObjArray::operator=(const VObjArray& oa)
{
	if (&oa != this) {
		reset();
		init();
		copyFrom((VObjArray *)(&oa));
	}
	return *this;
}

// ---------------------------------------------------------------------------
// Add obj to the next empty slot of this VObjArray. If the size of
// this VObjArray is not large enough, then expand it to fit obj. 
//
void VObjArray::add(VObject *obj)
{
	integer i;
	hugeObjPointerPointer p = (hugeObjPointerPointer)getData();
	for (i = 0; i < nEle; i++) {
		if (*p++ == 0) {
			break;
		}
	}
	if (i == nEle) {
		expand();
	}
	p = ((hugeObjPointerPointer)getData()) + i;
	*p = obj;
}

// ---------------------------------------------------------------------------
// Return the element of this VObjArray at index i.
//
VObject *&VObjArray::operator[](integer i)
{
	if (i < 0) {
		i = 0;
	}
	if (i >= nEle) {
		expandTo(i + 1);
	}
	hugeObjPointerPointer p = (hugeObjPointerPointer)getData();
	return p[i];
}
