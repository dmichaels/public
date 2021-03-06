// $Header:   Q:/views/common/vcs/STACK.H_V   1.11   Mar 24 1997 16:13:22   scottt  $ 

//	stack.h
//
//	VStack class interface [Common]
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
//	04/09/93 pmcm	changed id to VObject *
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/06/95 pkt	removed OFI code.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSTACK_H
#define VSTACK_H

// ---------------------------------------------------------------------------
//
#include "ordcllct.h"

// ---------------------------------------------------------------------------
//
CLASS VClass;
CLASS VObject;
CLASS VOrdCollect;

// ---------------------------------------------------------------------------
// A VStack is a first-in-last-out VOrdCollect.
//
CLASS VStack : public VOrdCollect {
public:
	// Constructors & Destructor
					VStack();
					VStack(const VStack&);

	VClass		   *iam();
	VStack&			operator=(const VStack&);

	// Stack Operations
	VObject		   *top();
	VObject		   *pop();

	// Push 'obj' onto ths `VStack`.
	void			push(VObject *obj);

};

extern VClass *VStackCls;

// ---------------------------------------------------------------------------
//
inline VStack& VStack::operator=(const VStack& s)
{
	if (&s != this) {
		reset();
		init();
		copyFrom((VStack *)(&s));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VObject *VStack::top()
{
	return last();
}

// ---------------------------------------------------------------------------
//
inline void VStack::push(VObject *obj)
{
	add(obj);
}

#endif // VSTACK_H
