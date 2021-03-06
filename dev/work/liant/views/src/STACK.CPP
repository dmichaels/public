// $Header:   Q:/views/common/vcs/STACK.CPV   1.10   Mar 24 1997 16:13:20   scottt  $ 

//	stack.cpp
//
//	VStack class implementation [Common]
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
//	10/12/92 pmcm	rewrote VStack::pop to fix
//					"multiple-copies-of-same-object" bug
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	05/19/95 pkt	selected Motif as common source.
//	09/06/95 pkt	removed OFI code.
//	06/07/96 pkt	fixed 'pop' to act when top item is null.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "stack.h"

// ---------------------------------------------------------------------------
//
defineClass(VStack, VOrdCollect)

// ---------------------------------------------------------------------------
// Construct an empty VStack.
//
VStack::VStack()
{
}

// ---------------------------------------------------------------------------
// Pop and return the top of this VStack. Return a null pointer if the
// stack is empty.    
//
VObject *VStack::pop()
{
	VObject *topObj = 0;

	if (count() > 0) {
		topObj = idAt(count() - 1);
		removeAt(count() - 1);
	}

	return topObj;
}

// ---------------------------------------------------------------------------
// Construct a VStack object from VStack t.
//
VStack::VStack(const VStack& s)
{
	init();
	copyFrom((VStack *)(&s));
}
