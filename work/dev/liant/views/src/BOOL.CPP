// $Header:   Q:/views/common/vcs/BOOL.CPV   1.8   Mar 24 1997 16:08:16   scottt  $ 

//	bool.cpp
//
//	VBool class implementation [Common]
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
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "bool.h"
#include "archiver.h"
#include "str.h"

// ---------------------------------------------------------------------------
//
defineClass(VBool, VObject)

// ---------------------------------------------------------------------------
// Construct a VBool with a FALSE value.
//
VBool::VBool()
{
	isTrue = 0;
}

// ---------------------------------------------------------------------------
// Construct a VBool with the value b.
//
VBool::VBool(boolean b)
{
	isTrue = b ? TRUE: FALSE;
}

// ---------------------------------------------------------------------------
//
VBool::VBool(const VBool& b)
{
	isTrue = b.isTrue;
}

// ---------------------------------------------------------------------------
// A VBool can copy to a VBool.
//
boolean VBool::copyTo(VObject *o)
{
	if (o->isA(VBoolCls)) {
		*(VBool *)o = isTrue;
	}
	else {
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// A VBool can copy from a VBool, or a VString.
// A VString is TRUE if it is non-null.
//
boolean VBool::copyFrom(VObject *o)
{
	if (o->isA(VStringCls)) {
		isTrue = ((VString *)o)->size() ? 1 : 0;
	}
	else if (o->isA(VBoolCls)) {
		isTrue = int(*((VBool *)o));
	}
	else {
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
VBool& VBool::operator=(const VBool& b)
{
	isTrue = b.isTrue;
	return *this;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Put this VBool object to VArchiver a.
//
void VBool::putTo(VArchiver& a)
{
	a << isTrue;
}

// ---------------------------------------------------------------------------
// Get this VBool object to VArchiver a.
//
void VBool::getFrom(VArchiver& a)
{
	a >> isTrue;
}

// ---------------------------------------------------------------------------
//
void VBool::getFrom(VArchiver &a,
					VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER
