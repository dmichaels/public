// $Header:   Q:/views/common/vcs/INTASSOC.CPV   1.8   Mar 24 1997 16:09:24   scottt  $ 

//	intassoc.cpp
//
//	VIntAssoc class implementation [Common]
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
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened.
//	01/17/94 dgm	cfront C++ changes
//	01/17/94 dgm	cfront C++ changes
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "intassoc.h"
#include "archiver.h"
#include <stdio.h>

// ---------------------------------------------------------------------------
//
defineClass(VIntAssoc, VAssoc)

// ---------------------------------------------------------------------------
// Contruct an VIntAssoc object key k and value v set to 0L.
//
VIntAssoc::VIntAssoc()
{
	key = 0;
	value = 0;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Read the key and value of this VIntAssoc from the VArchiver a.
//
void VIntAssoc::getFrom(VArchiver &a)
{
	long l;

	a >> l;
	key = (VObject *)l;
	a >> l;
	value = (VObject *)l;
}

// ---------------------------------------------------------------------------
//
void VIntAssoc::getFrom(VArchiver& a,
					    VObject * /* data */ , boolean /* do_realize */ )
{
	VIntAssoc::getFrom(a);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// This hash function returns the result of calling the hash function
// on the key integer.
//
HashKey VIntAssoc::hash() const
{
	return (long(key) >> 3) & HASH_KEY_MASK;
}

// ---------------------------------------------------------------------------
// Return TRUE if the key of this VIntAssoc is the same as the key of
// VIntAssoc object a, otherwise return FALSE.   Comparison is by integer
// value.
//
boolean VIntAssoc::isEqual(VObject *a) const
{
	if (a == 0) {
		return FALSE;
	}
	if (a->isA(VIntAssocCls)) {
		if (key == ((VIntAssoc *)a)->key) {
			return TRUE;
		}
	}
	return FALSE;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Store the key and value of this VIntAssoc into the VArchiver a.
//
void VIntAssoc::putTo(VArchiver& a)
{
	a << (long)key;
	a << (long)value;
}
#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Contruct an VIntAssoc object with key k and value v.
//
VIntAssoc::VIntAssoc(long k, long v) 
{ 
	key = (VObject *)k;
	value = (VObject *)v; 
}

// ---------------------------------------------------------------------------
//
VIntAssoc::VIntAssoc(const VIntAssoc &ia)
{	
	init();
	copyFrom((VIntAssoc *)(&ia));
}

