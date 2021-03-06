// $Header:   Q:/views/common/vcs/CONTAINR.CPV   1.11   Mar 24 1997 16:08:36   scottt  $

//	containr.cpp
//
//	VContainer class implementation [Common]
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
//	09/26/93 jmd	preened
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	06/06/94 jld	removed VMemory::memory in expandTo, setData, and getFrom.
//  07/11/94 dgm	merged.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	08/30/94 pkt	updated for VNOMALLOC version of VMemory.
//	05/19/95 pkt	selected Windows as common source
//	09/06/95 pkt	updated for revised C++ standard for loops.
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	08/20/96 pkt	updated 8/9 change for archiving support.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "containr.h"
#include "archiver.h"
#if defined(CV_WIN16)
#	include <windowsx.h>
#endif

// ---------------------------------------------------------------------------
//
defineClass(VContainer, VObject)

#ifdef CV_WIN16
static const long LargeNum = 65536L;
#endif 

// ---------------------------------------------------------------------------
// Construct a VContainer with no contents.
// 
VContainer::VContainer() 
{ 
	VContainer::init();
}

// ---------------------------------------------------------------------------
// Construct a VContainer of elements of size size
// with room with one element.
//
VContainer::VContainer(unsigned size)
{
	VContainer::init();
	eleSize = size;
	expandTo(1);
}

// ---------------------------------------------------------------------------
// Construct a VContainer to contain n elements, each of 
// size bytes. 
//
VContainer::VContainer(integer n, unsigned size)
{
	VContainer::init();
	eleSize = size;
	expandTo(n);
}

// ---------------------------------------------------------------------------
// Construct a VContainer from VContainer c.
//
VContainer::VContainer(const VContainer& c)
{
	init();
	copyFrom((VContainer *)(&c));
}

// ---------------------------------------------------------------------------
//
VContainer& VContainer::operator=(const VContainer& c)
{
	if (&c != this) {
		reset();
		init();
		copyFrom((VContainer *)(&c));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
VContainer::~VContainer()
{  
	VContainer::reset();
}

// ---------------------------------------------------------------------------
// Return the number of elements deemed to be in this VContainer. 
// This method returns the number of elements actually allocated.  
//
integer VContainer::size() const
{ 
	return nEle; 
}

// ---------------------------------------------------------------------------
// Copy n elements of VContainer c into the contents of
// this VContainer starting with element s.  Return the number of elements
// copied.
//
integer VContainer::atPut(integer s, VContainer *c, integer n) 
{ 
	hugePointer p = 0, q = 0;
	integer i;
	integer m = n * eleSize;
	if (c->nEle + s > nEle) {
		expandTo(c->nEle + s);
	}
	p = hugePointer(getData()) + s * eleSize;
	q = hugePointer(c->getData());
	for (i = 0; i < m; i++) {
		*p++ = *q++;
	}
	return n;
}

// ---------------------------------------------------------------------------
// Copy n elements from the contents of this VContainer, 
// starting with element s, into VContainer c.  Return the number of 
// characters copied.
//
integer VContainer::atGet(integer s, VContainer *c, integer n) 
{ 
	hugePointer p = 0, q = 0;
	integer i, m;
	if (s + n > nEle) {
		n = nEle - s;
	}
	if (n > c->nEle) {
		c->expandTo(n);
	}
	if ((p = hugePointer(getData())) == 0) {
		return 0;
	}
	p += s * eleSize;
	q = c->getData();
	if (q != 0) {
		m  = n * eleSize;
		for (i = 0; i < m; i++) {
			*q++ = *p++;
		}
		return n;
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
// Return the hash value of this VContainer, which is a byte-wise
// function on its contents.
//
HashKey VContainer::hash() const
{
	hugePointer p;
	long sz = size();
	integer i, n = sz * eleSize;
	HashKey key = n;
	if (n > 8) {
		n = 8;
	}
	p = hugePointer(getData());
	if (p == 0) {
		return 0;
	}
	for (i = 0; i < n; i++) {
		key = (key + (*p++)) & HASH_KEY_MASK;
	}
	return key;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VContainer are the
// same as VContainer o.  This is a byte-wise comparison of contents.
//
boolean VContainer::isEqual(VObject *o) const
{
	hugePointer p, q;
	boolean b = TRUE;
	integer sz = size();
	if (!o->isA(VContainerCls)) {
		return FALSE; 
	}
	if (sz != ((VContainer *)o)->size()) {
		return FALSE;
	}
	if (eleSize != ((VContainer *)o)->eleSize) {
		return FALSE;
	}
	p = hugePointer(getData());
	q = hugePointer(((VContainer *)o)->getData());
	sz *= eleSize;
	if ((p == 0) && (q == 0)) {
		return TRUE;
	}
	if (((p != 0) && (q == 0)) || ((p == 0) && (q != 0))) {
		b = FALSE;
	}
	else {
		while (b && (sz-- != 0)) {
			if (*p++ != *q++) {
				b = FALSE;
			}
		}
	}
	return b;
}

// ---------------------------------------------------------------------------
// Copy one element at index idx from this VContainer
// to ele and return TRUE.  If the index is out of range, then return FALSE. 
//
boolean VContainer::atGet(integer idx, char *ele)
{
	return atGet(idx, ele, 1) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Copy n elements, starting at index idx, from this VContainer
// to the C string ele.  Return the number of elements copied.  The number
// of elements copied may be less than n if the end of the container was 
// reached first. If index idx is out of range, then return a value of 0.
//
integer VContainer::atGet(integer idx, char *ele, integer n)
{       
	hugePointer p;
	hugePointer e = hugePointer(ele);
	integer i;

	if (idx >= nEle) {
		return 0;
	}
	if (idx + n > nEle) {
		n = nEle - idx;
	}
	p = hugePointer(getData());
	p += (idx * eleSize);
	for (i = n * eleSize; i--;) {
		*e++ = *p++;
	}
	return n;
}

// ---------------------------------------------------------------------------
// Copy one element from ele into this VContainer at
// index idx.  If the index is out of range, then expand its contents.
//
void VContainer::atPut(integer idx, char *ele)
{
	atPut(idx, ele, 1);
}

// ---------------------------------------------------------------------------
// Copy n elements from ele into the contents of this
// VContainer starting with index idx.  The contents of the VContainer are
// expanded as needed.  Return the number of elements copied.
//
integer VContainer::atPut(integer idx, char *ele, integer n)
{
	hugePointer p;
	hugePointer e = hugePointer(ele);
	integer i;

	if (idx + n > nEle) {
		expandTo(idx + n);
	}
	if ((p = hugePointer(getData())) == 0) {
		return 0;
	}
	p += idx * eleSize;
	for (i = n * eleSize; i--;) {
		*p++ = *e++;
	}
	return n;
}

// ---------------------------------------------------------------------------
// Make this VContainer have exactly the same contents as
// VContainer o.  Return TRUE if successful, otherwise return FALSE.
//
boolean VContainer::copyFrom(VObject *o)
{
	if (!o->isA(VContainerCls)) {
		return FALSE;
	}
	VContainer *c = (VContainer *)o;
	hugePointer p = 0, q = 0;
	eleSize = c->eleSize;		
	integer i, bytes = c->nEle *eleSize;
	reset();
	expandTo(c->nEle);
	p = hugePointer(c->getData());
	q = hugePointer(getData());
	for (i = 0; i < bytes; i++) {
		q[i] = p[i];
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Expand the capacity of this VContainer by halving again its size.  
// Expand it to ten elements if it is empty.
//
void VContainer::expand()
{
	integer n = nEle / 2;
	if (n == 0) {
		n = 10;
	}
	expand(n);
}

// ---------------------------------------------------------------------------
// Expand or shrink this VContainer to n elements.
// 
void VContainer::expandTo(integer n)
{
	if (data && n == nEle) {	// container size ok
		return;
	}
	if ( n == 0 ) {		// empty the container
		reset();
		return;
	}
	integer nsz = eleSize * n;	// number of bytes wanted
	integer cpy = (n > nEle) ? eleSize * nEle : nsz;
#ifdef CV_WIN16
	pointer newData = 0;
	if (nsz >= LargeNum) {
		newData = (pointer)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_ZEROINIT, nsz);
	}
	else {
		newData = new char [nsz];
	}
#else
	pointer newData = new char [nsz];
#endif
	hugePointer ndp = hugePointer(newData);
	hugePointer odp = hugePointer(data);

    // copy elements into new container
	// if new container is smaller, elements may be lost.
	integer i;
    for (i=0; i < cpy; i++) {
		*ndp++ = *odp++;
	}
	// initialize any new elements
	for ( ; i < nsz; i++) {
		*ndp++ = 0;
	}
	setData(newData);
	nEle = n;
}

// ---------------------------------------------------------------------------
// Expand this VContainer by n elements.
//
void VContainer::expand(integer n)
{
	expandTo(nEle + n);
}

// ---------------------------------------------------------------------------
// Free-up the contents of this VContainer.
//
void VContainer::reset() 
{
	setData(0);
	nEle = 0;
}              

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Read this VContainer and its contents from VArchiver a.
// 
void VContainer::getFrom(VArchiver &a)
{
	setData(0);
	a >> nEle;
	a >> eleSize;
	if (nEle) {
#ifdef CV_WIN16
		long nsz = nEle * eleSize;
		if (nsz >= LargeNum) {
			data = (pointer)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_ZEROINIT, nsz);
		}
		else {
			data = new char [nsz];
		}
#else
		data = new char [nEle * eleSize];
#endif
		a.get(this, nEle);
	}
}

// --------------------------------------------------------------------------
// 
void VContainer::getFrom(VArchiver &a,
						 VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Store this VContainer and its contents into VArchiver a.
// 
void VContainer::putTo(VArchiver &a)
{
	a << nEle;
	a << eleSize;
	if (nEle) {
		a.put(this, nEle);
	}
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
//
handle VContainer::getDataHndl() 
{ 
	handle *hp = (handle *)data; 
	return *--hp;
}

// ---------------------------------------------------------------------------
// Set the contents of this VContainer to p and free its
// current contents if necessary.
//
pointer VContainer::setData(pointer p) 
{ 
	if (data != 0)  {
#ifdef CV_WIN16
		if (isHuge()) {
		 	GlobalFreePtr(data);
		}
		else {
			delete [] data;
		}
#else 
		delete [] data;
#endif 
	}
	return data = p;
}

// ---------------------------------------------------------------------------
// Returns TRUE if the container's data is allocated so as to span segments
// (Win 16).
//
boolean VContainer::isHuge() const
{
#ifdef CV_WIN16
	return data != 0 && (nEle * eleSize >= LargeNum);
#else
	return FALSE;
#endif
}
