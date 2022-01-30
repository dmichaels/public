// $Header:   Q:/views/common/vcs/SET.CPV   1.15   Mar 24 1997 16:13:18   scottt  $ 

//  set.cpp
//
//  VSet class implementation [Common]
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
//  Revision History:
//  -----------------
//  04/07/93 pmcm   changed id to VObject *
//  04/19/93 pmcm   changed local 'on' from int to integer in ExpandTo().
//  07/31/93 jmd    made count, indexOf const
//  08/07/93 jmd    preened
//  08/12/93 pmcm   changed for loop counter in expandTo() to not use
//                  pointer arithmetic
//  09/21/93 pmcm   changed idptr to VObject **
//  09/26/93 jmd    added test for infinite loop in indexOf
//  01/13/94 dgm    merged and preened
//  01/19/94 pkt    preened and merged with Macintosh.
//  01/17/94 dgm    cfront C++ changes
//  06/06/94 jld    removed VMemory::memory from expandTo()
//  07/11/94 dgm    fixed up assignment-operator and copy-constructor.
//  09/08/94 dgm    Minor coding change to suppress MetaWare C++ warning.
//  05/19/95 pkt    merged for common source.
//	09/06/95 pkt	removed OFI code.
//	10/06/95 pkt	updated use of 'const'.
//	12/22/95 tlf	Fixed bug in ::add.
//	04/30/96 pkt	added 'allFollowedBy' and 'allPrecededBy'.
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "set.h"
#include "iterator.h"
#include "archiver.h"

// ---------------------------------------------------------------------------
//
defineClass(VSet, VCollection)

// ---------------------------------------------------------------------------
// Construct a VSet of size 15.
//
VSet::VSet()
{
    cnt = 0;
    contents.expandTo(15);
}

// ---------------------------------------------------------------------------
// Construct a VSet of size n.
//
VSet::VSet(integer n)
{
    cnt = 0;
    contents.expandTo(n);
}

// ---------------------------------------------------------------------------
// Construct a VSet from VSet object s.
//
VSet::VSet(const VSet& s)
{
    init();
    copyFrom((VSet *)(&s));
}

// ---------------------------------------------------------------------------
// Return the number of objects in this VSet.
//
integer VSet::count() const
{
    return cnt;
}

// ---------------------------------------------------------------------------
// Add object obj to this VSet and return it.
//
VObject *VSet::add(VObject *obj)
{
    if (!obj || find(obj)) {
        return 0;
    }
    HashKey key;
    integer n = contents.size();
    hugeObjPointerPointer p;

    // if ((cnt << 1) > n) {
    // On entry, cnt is the current number in the set.  We want
    // to know if there is room for one more.
    if (((cnt + 1) << 1) > n) {
        expandTo(n = 2 * n + ((!n) ? 15 : 0));
    }
    cnt++;

    if ((p = (hugeObjPointerPointer)contents.getData()) == 0) {
        return 0;
    }
    key = obj->hash() % n;

    if (!p[key]) {
        p[key] = obj;
    }
    else {
        while (++key) {
            if (!p[key % n]) {
                p[key % n] = obj;
                break;
            }
        }
    }
    return obj;
}

// ---------------------------------------------------------------------------
// Set the VIterator i to the first element in this VSet. If there
// is no first element, then set it to a null pointer.
//
void VSet::start(VIterator& i) const
{
    i = -1L;
    next(i);
}

// ---------------------------------------------------------------------------
// Set the VIterator i to the last object in this VSet.
//
void VSet::end(VIterator& i) const
{
    i = contents.size();
    prev(i);
}

// ---------------------------------------------------------------------------
//
void VSet::endIterator(VIterator& i) const
{
	end(i);
}

// ---------------------------------------------------------------------------
// Return the current object referred to by the VIterator i and set i to
// reference the next object.
//
VObject *VSet::next(VIterator& i) const
{
    integer n = contents.size();
    VObject *p = 0;
    VObject *obj;
    integer j, k = i;
    if (k >= n) {
        i = (VObject *)0;
        return 0;
    }
    obj = contents.idAt(i);
    for (j = k + 1; j < n; j++) {
        if ((p = contents.idAt(j)) != 0) {
            break;
        }
    }
    i = j;
    i = p;
    return obj;
}

// ---------------------------------------------------------------------------
// Return the current object referred to by the VIterator i and set i to
// reference the previous object.
//
VObject *VSet::prev(VIterator& i) const
{
    VObject *p = 0;
    VObject *obj = contents.idAt(i);
    integer j, k = i;
    //Fix per kb & sw
    if (k < 0) {
        i = (VObject *)0;
        return 0;
    }
 
    for (j = k - 1; j >= 0; j--) {
        if ((p = contents.idAt(j)) != 0) {
            break;
        }
    }
    i = j;
    i = p;
    return obj;
}

// ---------------------------------------------------------------------------
// Override VCollection::allFollowedBy to tell whether the VIterator i 
// is pointing past the end of the entire collection.
//
boolean VSet::allFollowedBy(VIterator&i) const
{
	integer k = i;
	return k >= contents.size();
}

// ---------------------------------------------------------------------------
// Override VCollection::allPrecededBy to tell whether the VIterator i 
// is pointing before the start of the entire collection.
//
boolean VSet::allPrecededBy(VIterator&i) const
{
	integer k = i;
	return k < 0;
}

// ---------------------------------------------------------------------------
// Return the object in this VSet that matches object obj via the
// isEqual() method, otherwise return a null object pointer.
//
VObject *VSet::find(const VObject *obj) const
{
    integer i = indexOf(obj);
    if (i == -1L) {
        return 0;
    }
    return contents.idAt(i);
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Load this VSet from VArchiver a.
//
void VSet::getFrom(VArchiver& a)
{
    a.getObject(&contents);
    a >> cnt;
}

// ----------------------------------------------------------------------------
//
void VSet::getFrom(VArchiver& a,
                   VObject * /* data */ , boolean /* do_realize */ )
{
    getFrom(a);
}

// ---------------------------------------------------------------------------
// Store this VSet into VArchiver a.
//
void VSet::putTo(VArchiver& a)
{
    a.putObject(&contents);
    a << cnt;
}
#endif

// ---------------------------------------------------------------------------
// Return the index an object equivalent to obj (via the isEqual()
// method) in this VSet. Otherwise, return -1 if an equivalent object is not
// found.
//
integer VSet::indexOf(const VObject *obj) const
{
    hugeObjPointerPointer p;
    if (obj == 0) {
        return -1L;
    }
    integer i, n = contents.size();
    if (n == 0) {
        return -1L;
    }
    integer m = -1L;
    HashKey key = obj->hash() % n;
    if ((p = (hugeObjPointerPointer)contents.getData()) == 0) {
        return -1L;
    }

    // crude infinite loop test
    int test = 0;

    while (p[i = key % n]) {
        if (p[i]->isEqual((VObject *)obj)) {
            m = i;
            break;
        }
        else {
            if (i == 0) {
                if (test > 0) {
                    break;
                }
                else {
                    test++;
                }
            }

            key++;
        }
    }
    return m;
}

// ---------------------------------------------------------------------------
// Replace object old with object new in this VSet.
// Return old if present, otherwise return a null pointer.
//
VObject *VSet::replaceSame(VObject *old, VObject *New)
{
    VObject *obj;
    if ((obj = removeSame(old)) != 0) {
        add(New);
    }
    return obj;
}

// ---------------------------------------------------------------------------
// Return the index of obj in this VSet, otherwise return -1
// if it is not in this VSet.
//
integer VSet::indexOfSame(const VObject *obj) const
{
    return contents.indexOfSame(obj);
}

// ---------------------------------------------------------------------------
// Return obj if it is in this VSet, otherwise return a null pointer.
//
VObject *VSet::findSame(const VObject *obj) const
{
    return contents.findSame(obj);
}

// ---------------------------------------------------------------------------
// Remove and return the object obj in this VSet,
// otherwise return a null pointer if it is not present.
//
VObject *VSet::removeSame(VObject *obj)
{
    VObject *old;
    integer j, m, i;
    integer k = indexOfSame(obj);
    integer n = contents.size();
    if (k < 0) {
        return 0;
    }
    hugeObjPointerPointer p;
    if ((p = (hugeObjPointerPointer)contents.getData()) == 0) {
        return 0;
    }
    cnt--;
    old = p[k];
    p[k] = 0;
    m = k++;
    while (p[j = k % n]) {
        i = p[j]->hash() % n;
        if ((j < i || i <= m) && (i <= m || m < j) && (m < j || j < i)) {
            p[m] = p[j];
            p[j] = 0;
            m = j;
        }
        k++;
    }
    return old;
}

// ---------------------------------------------------------------------------
// Delete each object in this VSet.
//
void VSet::freeContents()
{
    contents.freeContents();
    VSet::reset();
}

// ---------------------------------------------------------------------------
// Expand this VSet to size n.
//
void VSet::expandTo(integer n)
{
    if (n == 0) {
        return;
    }
    hugeObjPointerPointer q;
    hugeObjPointerPointer p;

    if ((cnt == 0) || ((p = (hugeObjPointerPointer)contents.getData()) == 0)) {
        contents.expandTo(n);
        return;
    }
    integer on = contents.size();
    contents.clearData();
    cnt = 0;
    contents.expandTo(n);
    for (q = p; on > 0L; q++, on--) {
        if (*q) {
            add(*q);
        }
    }
    delete [](char *)p;
}

// ---------------------------------------------------------------------------
// Make the contents of this VSet empty.  Does not free its contents.
//
void VSet::reset()
{
    contents.reset();
    cnt = 0;
}

// ---------------------------------------------------------------------------
// Return the hash key of this VSet computed from the hash keys
// returned by all of the objects in it.
//
HashKey VSet::hash() const
{
    HashKey key = 0;
    DO ((VSet *)this, VObject, anId)
        key += anId->hash() & HASH_KEY_MASK;
    END

    return key & HASH_KEY_MASK;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VSet has all, and only, the elements matching
// those of the set obj, otherwise return FALSE.
//
boolean VSet::isEqual(VObject *obj) const
{
    if (!obj->isA(VSetCls)) {
        return FALSE;
    }
    VSet *o = (VSet *)obj;
    if (cnt != o->cnt) {
        return FALSE;
    }
    DO ((VSet *)this, VObject, a)
        if (!(o->find(a))) {
            return FALSE;
        }
    END

    return TRUE;
}

// ---------------------------------------------------------------------------
// Copy the contents of VSet object obj to this VSet.
//
boolean VSet::copyFrom(VObject *obj)
{
    VSet::reset();
    return VCollection::copyFrom(obj);
}
