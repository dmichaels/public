// $Header:   Q:/views/common/vcs/INTSET.CPV   1.12   Mar 24 1997 16:09:30   scottt  $ 

//	intset.cpp
//
//	VIntegerSet class implementation [Common]
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
//	07/30/93 pkt	eliminated #ifdef macintosh.
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	06/06/94 jld	removed VMemory::memory from expandTo
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/05/94 pkt	fixed 'add' to not lose track of count when data expanded.
//	05/19/95 pkt	selected Windows as common source.
//	03/05/96 dgm	fix in copyFrom().
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	08/20/96 pkt	fixed 8/9 change to delete data pointer correctly.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "intset.h"
#include "archiver.h"

#ifdef CV_WIN16
typedef long huge  *hugeLongptr;
#else
typedef longptr		hugeLongptr;
#endif

// ---------------------------------------------------------------------------
//
defineClass(VIntegerSet, VContainer)

// ---------------------------------------------------------------------------
// Construct a VIntegerSet of size 15.
//
VIntegerSet::VIntegerSet() 
{ 
	cnt = 0;
	eleSize = sizeof(long);
	expandTo(15);
}

// ---------------------------------------------------------------------------
// Construct an VIntegerSet of size n.
//
VIntegerSet::VIntegerSet(integer n) 
{ 
	cnt = 0;
	eleSize = sizeof(long);
	expandTo(n);
}

// ---------------------------------------------------------------------------
// Return the number of integers in this VIntegerSet.
//
integer VIntegerSet::count() 
{ 
	return cnt;
}

// ---------------------------------------------------------------------------
// Add the value n to this VIntegerSet.  If necesary, expand this
// VIntegerSet to hold n.  Return TRUE if successful, otherwise return FALSE.
//
boolean VIntegerSet::add(long n)
{
	if ((n <= 0) || find(n)) {
		return FALSE;
	}
	integer sz = size(); 
	hugeLongptr p;
	
	if (sz == 0) {
		expandTo(15);
		sz = size();
	}
	// cnt++; pkt 09/05/94: can't call expandTo while cnt doesn't match data.
	integer new_cnt = cnt + 1;
	if ((new_cnt << 1) > sz) {
		expandTo(sz *= 2);
	}
	cnt = new_cnt;
	if ((p = (hugeLongptr)getData()) == 0) {
		return FALSE;
	}
	integer key = n % sz;
	if (p[key] == 0) {
		p[key] = n;
	}
	else {
		while (++ key) {
			if (p[key % sz] == 0) {
				p[key % sz] = n;
				break;
			}
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return TRUE if n is in this VIntegerSet, otherwise return FALSE.
//
boolean VIntegerSet::find(long n)
{
	integer i = indexOf(n);
	if (i == -1L) {
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return the index of n in this VIntegerSet.
//
integer VIntegerSet::indexOf(long n)
{
	hugeLongptr p;
	if (n <= 0) {
		return -1L;
	}
	integer i, sz = size();
	if (sz == 0) {
		return -1L;
	}
	integer m = -1L;
	unsigned key = (unsigned)(n % sz);
	if ((p = (hugeLongptr)getData()) == 0) {
		return -1L;
	}
	while (p[i = key % sz]) {
		if (p[i] == n) {
			m = i;
			break;
		}
		else {
			key++;
		}
	}
	return m;
}

// ---------------------------------------------------------------------------
// Remove and return n from this VIntegerSet,
// otherwise return -1L if it is not present.
//
long VIntegerSet::remove(long n)
{
	long old;
	integer j, m, i;
	integer k = indexOf(n);
	integer sz = size();
	if (k < 0) {
		return -1L;
	}
	hugeLongptr p;
	if ((p = (hugeLongptr)getData()) == 0) {
		return -1L;
	}
	cnt--;
	old = p[k];
	p[k] = 0;
	m = k++;
	while (p[j = k % sz]) {
		i = p[j]% sz;

	//
	// The following test is due to Mark Nichols of OFI
	// Define a distance function that measures the distance
	// from a to b clockwise around a circle of circumference c:
	//
#	define dc(c,a,b) (((a) <= (b)) ? (b) - (a) : (b) + (c) - (a))

		if (dc(sz, i, m) < dc(sz, i, j)) {
			p[m] = p[j];
			p[j] = 0;
			m = j;
		}
		k++;
	}
	return old;
}

// ---------------------------------------------------------------------------
// Expand this VIntegerSet table to size n.
//
void VIntegerSet::expandTo(integer n) 
{ 
	if (n <= 0) {
		return;
	}
	hugeLongptr q;
	hugeLongptr p;

	if ((cnt == 0) || ((p = hugeLongptr(getData())) == 0)) {
		VContainer::expandTo(n);
		return;
	}
	integer on = size();
	boolean p_is_huge = isHuge();
	clearData();
	// We are now responsible for deleting p.
	
	cnt = 0;
	VContainer::expandTo(n);
	for (q = p; q - p < on; q++) {
		if (*q != 0) {
			add(*q);
		}
	}
	
#ifdef CV_WIN16
	if (p_is_huge) {
	 	GlobalFreePtr((hugePointer)p);
	}
	else {
		delete [] (hugePointer)p;
	}
#else 
	delete [] (hugePointer)p;
#endif 
}

// ---------------------------------------------------------------------------
// Make the contents of this VIntegerSet empty.  
//
void VIntegerSet::reset() 
{ 
	VContainer::reset();
	cnt = 0; 
}

// ---------------------------------------------------------------------------
// Make this VIntegerSet have exactly the same contents as
// VIntegerSet o. Return TRUE if successful, otherwise return FALSE.
//
boolean VIntegerSet::copyFrom(VObject *o)
{
	if (!VContainer::copyFrom(o)) {
		return FALSE;
	}
	if (!o->isA(VIntegerSetCls)) {
		return TRUE;
	}
	cnt = ((VIntegerSet *)o)->cnt;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Construct an VIntegerSet from VIntegerSet is.
//
VIntegerSet::VIntegerSet(const VIntegerSet& is)
{
	init();
	copyFrom((VIntegerSet *)(&is));
}

// ---------------------------------------------------------------------------
//
VIntegerSet& VIntegerSet::operator=(const VIntegerSet& is)
{
	if (&is != this) {
		reset();
		init();
		copyFrom((VIntegerSet *)(&is));
	}
	return *this;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Read this VIntegerSet and its contents from VArchiver a.
//
void VIntegerSet::getFrom(VArchiver& a)
{
	VContainer::getFrom(a);
	a >> cnt;
}

// ---------------------------------------------------------------------------
//
void VIntegerSet::getFrom(VArchiver& a,
						  VObject * /* data */ , boolean /* do_realize */ )
{
	VIntegerSet::getFrom(a);
}

// ---------------------------------------------------------------------------
// Store this VIntegerSet and its contents into VArchiver a.
//
void VIntegerSet::putTo(VArchiver& a)
{
	VContainer::putTo(a);
	a << cnt;
}
#endif // ARCHIVER
