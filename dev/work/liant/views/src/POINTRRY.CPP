// $Header:   Q:/views/common/vcs/POINTRRY.CPV   1.10   Mar 24 1997 16:12:54   scottt  $ 

//	pointrry.cpp
//
//	VPointArray class implementation [Common]
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
//	07/31/93 jmd	made size const
//	08/09/93 jmd	removed free
//	09/28/93 pmcm	fixed getFrom() and putTo()--backwardly incompatible,
//					but now portable.
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened.
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	03/27/95 jld	removed nonportable #ifdef CPPV21_PUTTO code
//	05/19/95 pkt	selected Windows as common source.
//	08/15/95 evc    getFrom no longer reverses the order of archived pt. array
//	10/09/95 tomf   corrected count maintained in howMany in putpnt.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "pointrry.h"
#include "archiver.h"

// ---------------------------------------------------------------------------
//
defineClass(VPointArray, VContainer)

// ---------------------------------------------------------------------------
// Contstruct a VPointArray with room for one point.
//
VPointArray::VPointArray()
{
	VPointArray::init();
	expandTo(1);
}

// ---------------------------------------------------------------------------
// Construct a VPointArray with room for n points.
//
VPointArray::VPointArray(integer n)
{
	VPointArray::init();
	expandTo(n);
}

// ---------------------------------------------------------------------------
// Add the pair (x,y) as the next next Point in this VPointArray.
//
void VPointArray::add(int x, int y)
{
	point p;

	p.x = x;
	p.y = y;
	VContainer::atPut(howMany++ , (char *)&p);
}

// ---------------------------------------------------------------------------
// Retrieve the point at index idx from this VPointArray into
// *x and *y.  Return TRUE if successful, otherwise return FALSE.
//
boolean VPointArray::getpnt(integer idx, int *x, int *y)
{
	point p;

	if (!(VContainer::atGet(idx, (char *)&p))) {
		return FALSE;
	}

	//	pmcm--under CV_WIN32 POINT coordinates are longs, we are truncating!	
	*x = (int) p.x;
	*y = (int) p.y;

	return TRUE;
}

// ---------------------------------------------------------------------------
// Put the pair (x,y) at index idx of this VPointArray, 
// expanding it if necessary.
//
void VPointArray::putpnt(integer idx, int x, int y)
{
	point p;

	p.x = x;
	p.y = y;
	VContainer::atPut(idx, (char *)&p);
	if (idx >= howMany) {
		howMany = idx + 1;
	}
}

// ---------------------------------------------------------------------------
// Set the number of points in this VPointArray to zero.
//
void VPointArray::reset() 
{ 
	howMany = 0;
}

// ---------------------------------------------------------------------------
// Return the number of (x,y) pairs in this VPointArray.
//
integer VPointArray::size() const
{
	return howMany;
}

// ---------------------------------------------------------------------------
// A VPointArray can be copied from another VPointArray.
//
boolean VPointArray::copyFrom(VObject *obj)
{
	if (!VContainer::copyFrom(obj)) {
		return FALSE;
	}
	if (!obj->isA(VPointArrayCls)) {
		return TRUE;
	}
	VPointArray *p = (VPointArray *) obj;
	howMany = p->howMany;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Construct an VPointArray from VPointArray pa.
//
VPointArray::VPointArray(const VPointArray &pa)
{
	 init();
	 copyFrom((VPointArray *)(&pa));
}

// ---------------------------------------------------------------------------
//
VPointArray& VPointArray::operator=(const VPointArray &pa)
{
	if (&pa != this) {
		reset();
		init();
		copyFrom((VPointArray *)(&pa));
	}
	return *this;
}

// ===========================================================================
//
#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Load a VPointArray from the VArchiver 'a'.
//
void VPointArray::getFrom(VArchiver& a)
{
	integer i;
	a >> i;
	expandTo(i);

	int x;
	int y;
	while (i-- > 0L) {
		a >> x;
		a >> y;
		//add(x, y);
		putpnt(i, x, y);
	}
}

// ----------------------------------------------------------------------------
//
void VPointArray::getFrom(VArchiver& a,
						  VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Store this VPointArray to the VArchiver 'a'.
//
void VPointArray::putTo(VArchiver& a)
{
	a << howMany;

	int x;
	int y;
	for (integer i = howMany; i > 0L; i--) {
		if (!getpnt(i - 1, &x, &y)) {
			x = y = 0;
		}
		a << x;
		a << y;
	}
}

#endif // ARCHIVER
