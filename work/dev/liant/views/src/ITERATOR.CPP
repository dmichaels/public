// $Header:   Q:/views/common/vcs/ITERATOR.CPV   1.9   Mar 24 1997 16:09:32   scottt  $ 

//	iterator.cpp
//
//	VIterator class implementation [Common]
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
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	10/06/95 pkt	updated use of 'const'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "object.h"
#include "iterator.h"
#include "archiver.h"

defineClass(VIterator, VObject)

// ---------------------------------------------------------------------------
//
VIterator::VIterator()
{
	;
}

// ---------------------------------------------------------------------------
// Construct an VIterator on VCollection aColl.
//
VIterator::VIterator(const VCollection &aColl)
{
	pColl = &aColl;
	start();
}

// ---------------------------------------------------------------------------
// Construct an VIterator on VCollection pColl.
//
VIterator::VIterator(const VCollection *pcoll)
{
	this->pColl = pcoll;
	start();
}

// ---------------------------------------------------------------------------
//
VIterator::VIterator(const VIterator &i)
{
	init();
	copyFrom((VIterator *)(&i));
}

// ---------------------------------------------------------------------------
//
VIterator& VIterator::operator=(const VIterator &i)
{
	if (&i != this) {
		reset();
		init();
		copyFrom((VIterator *)(&i));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
VIterator::~VIterator()
{
	;
}

// ---------------------------------------------------------------------------
// An VIterator can copy from another VIterator.
//
boolean VIterator::copyFrom(VObject *obj)
{
	if (!obj->isA(VIteratorCls)) {
		return FALSE;
	}
	VIterator *pIter = (VIterator *)obj;
	pCurr = pIter->pCurr;
	nCurr = pIter->nCurr;
	pColl = pIter->pColl;
	return TRUE;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Read this VIterator from VArchiver a.
//
void VIterator::getFrom(VArchiver& a) 
{
	a >> pCurr;
	a >> nCurr;
}

// ---------------------------------------------------------------------------
// Read this VIterator from VArchiver a.
//
void VIterator::getFrom(VArchiver& a,
						VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Store this VIterator into the VArchiver a.
//
void VIterator::putTo(VArchiver& a)
{
	a << pCurr;
	a << nCurr;
}
#endif // ARCHIVER
