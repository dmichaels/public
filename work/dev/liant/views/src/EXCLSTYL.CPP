// $Header:   Q:/views/common/vcs/EXCLSTYL.CPV   1.6   Mar 24 1997 16:08:50   scottt  $ 

//	exclstyl.cpp
//
//	Exclusive Style Class [Common]
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
//	09/01/95 dss	created.
//	10/12/95 dss	don't allow set/add on ourselves.
//
//	02/04/97 pkt	Removed 'creation'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "archiver.h"

#include "bitset.h"
#include "style.h"
#include "exclstyl.h"

defineClass(VExclusiveStyle, VStyle)

// -----------------------------------------------------------------------
void VExclusiveStyle::setExclusive(class VExclusiveStyle *a[]) 
{ 
	VStyle	x;
	class VExclusiveStyle **p = a;
	
	while (*p) {
		x += **p++;
	}
	
	p = a;
	while (*p) {
		(*p++)->exclude = x;
	}
}

// -----------------------------------------------------------------------
void VExclusiveStyle::set(const VExclusiveStyle& a) 
{
	if (this == &a) {
		return;
	}

	VStyle::set(a);

	exclude = a.exclude;
#ifndef CVS_SMALL
	creation = a.creation;
#endif
}

// -----------------------------------------------------------------------
void VExclusiveStyle::add(const VExclusiveStyle& a) 
{ 
	if (this == &a) {
		return;
	}

	VStyle::remove(a.exclude);

	bitSet.add(a.bitSet);
	
	exclude += a.exclude;
#ifndef CVS_SMALL
	creation += a.creation;
#endif
}

// -----------------------------------------------------------------------
void VExclusiveStyle::remove(const VExclusiveStyle& a) 
{ 
	if (contains(a)) {
		exclude -= a.exclude;
#ifndef CVS_SMALL
		creation -= a.creation;
#endif
	}

	VStyle::remove(a);
}

// -----------------------------------------------------------------------
void VExclusiveStyle::set(const VStyle& a)
{
	if (a.isA(VExclusiveStyleCls)) {
		VExclusiveStyle::set((VExclusiveStyle&)a);
	}
	else {
		VStyle::set(a);
	}
}

// -----------------------------------------------------------------------
void VExclusiveStyle::add(const VStyle& a)
{
	if (a.isA(VExclusiveStyleCls)) {
		VExclusiveStyle::add((VExclusiveStyle&)a);
	}
	else {
		VStyle::add(a);
	}
}

// -----------------------------------------------------------------------
void VExclusiveStyle::remove(const VStyle& a)
{
	if (a.isA(VExclusiveStyleCls)) {
		VExclusiveStyle::remove((VExclusiveStyle&)a);
	}
	else {
		VStyle::remove(a);
	}
}

#if defined(V_UNIT_TEST)
// -----------------------------------------------------------------------
void VExclusiveStyle::dump(ostream& s) const
{	
	VStyle::dump(s);

	if (!exclude.isEmpty()) {
		s << ", ";
		exclude.dump(s);
	}

#ifndef CVS_SMALL
	if (!creation.isEmpty()) {
		s << ", ";
		creation.dump(s);
	}
#endif
}
#endif

#ifndef CV_NOARCHIVER
// -----------------------------------------------------------------------
void VExclusiveStyle::getFrom(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "retrieving VExclusiveStyle " << name << endl;
#endif
#ifdef CVS_SMALL
	VStyle creation("creation");
#endif
	
	VStyle::getFrom(a);
	a.getObject(&exclude);
	a.getObject(&creation);
}

// -----------------------------------------------------------------------
void VExclusiveStyle::putTo(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "archiving VExclusiveStyle " << name << endl;
#endif
#ifdef CVS_SMALL
	VStyle creation("creation");
	creation += *this;
#endif

	VStyle::putTo(a);
	a.putObject(&exclude);
	a.putObject(&creation);
}
#endif
