// $Header:   Q:/views/win/vcs/atom.cpv   1.6   Mar 04 1997 17:16:36   gregm  $ 

//	atom.cpp
//
//	VAtom implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	10/04/93 jmd	preened
//	07/18/95 dgm	Miscellaneous const char * changes.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "memry.h"
#include "atom.h"

extern "C" {
#	include <string.h>
}

// ---------------------------------------------------------------------------
// Construct an VAtom with the handle 'h'.
//
VAtom::VAtom(unsigned int h)
{
	hAtom = h;
	pAtom = 0;
}

// ---------------------------------------------------------------------------
// Construct an VAtom with the name 's' and increment its reference count.
//
VAtom::VAtom(const char *s)
{
	pAtom = 0;
	if (s != 0) {
		hAtom = GlobalAddAtom((char *)s);
	}
	else {
		hAtom = 0;
	}
}

// ---------------------------------------------------------------------------
// Destruct this VAtom and release its memory.
//
VAtom::~VAtom()
{
	if (pAtom != 0) {
		delete [] pAtom;
		pAtom = 0;
//jld	VMemory::memory(pAtom, 0);
	}
}

// ---------------------------------------------------------------------------
// Decrement the reference count for the atom associated with this VAtom.
//
void VAtom::operator !()
{
	if (hAtom) {
		GlobalDeleteAtom(hAtom);
		hAtom = 0;
	}
}

// ---------------------------------------------------------------------------
// Return the name of this VAtom.
//
VAtom::operator char*()
{
	int i = 1, n, sz;
	char *buf;

	if (hAtom != 0) {
		while (i) {
			sz = i *ATOMmaxLength;
			buf = new char[sz +1];
			n = GlobalGetAtomName(hAtom, buf, sz);
			if (n >= sz -1) {
				i++;
				delete buf;
			}
			else {
				i = 0;
			}
		}
		buf[n] = 0;
		if (pAtom != 0) {
			delete [] pAtom;
		}
		pAtom = new char [n + 1];
//jld	pAtom = VMemory::memory(pAtom, n +1);
		strcpy(pAtom, buf);
		delete buf;
		return pAtom;
	}
	return 0;
}
