// $Header:   Q:/views/common/vcs/ATTACH.CPV   1.6   Mar 24 1997 16:08:04   scottt  $ 

//	attach.cpp
//
//	VAttach implementation [Common]
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
//	04/29/93 jmd	created
//	05/20/93 jmd	renamed None to UnAttached to avoid Motif problems
//	10/21/93 dgm	3.0 changes; re-ported from MS/Windows.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "attach.h"

// ---------------------------------------------------------------------------
//
VAttach::VAttach()
{
	int i;

	for (i = 0; i < 4; i++) {
		attach[i] = UnAttached;
		offset[i] = 0;
		sibling[i] = 0;
	}

	priority = 0;
	ptest = 0;
}

// ---------------------------------------------------------------------------
//
VAttach::~VAttach()
{
	;
}

// ---------------------------------------------------------------------------
//
void VAttach::setAttach(Side s, Type t, int off, VWindow *sib)
{
	attach[s] = t;
	offset[s] = off;
	sibling[s] = sib;
}
