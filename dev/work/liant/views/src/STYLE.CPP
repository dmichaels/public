// $Header:   Q:/views/common/vcs/STYLE.CPV   1.4   Mar 24 1997 16:13:34   scottt  $ 

//	style.h
//
//	Style Class [Common]
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
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "object.h"
#include "archiver.h"

#include "bitset.h"
#include "style.h"
#include "exclstyl.h"
#include "implstyl.h"
#include "stylexpr.h"

defineClass(VStyle, VObject)

void VStyle::add(const VStyle& a) 
{ 
	bitSet.add(a.bitSet);
}

void VStyle::remove(const VStyle& a) 
{ 
	bitSet.remove(a.bitSet);
}

void VStyle::set(const VStyle& a) 
{
	bitSet.set(a.bitSet);
}

boolean VStyle::contains(const VStyle& a) const
{ 
	return bitSet.contains(a.bitSet);
}

#if defined(V_UNIT_TEST)
void VStyle::dump(ostream& s) const
{
	s << name << " = ";
	bitSet.dump(s);
}
#endif

#ifndef CV_NOARCHIVER
void VStyle::getFrom(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "retrieving ExclStyle " << name << endl;
#endif

	a.getObject(&bitSet);
}

void VStyle::putTo(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "archiving ExclStyle " << name << endl;
#endif
	a.putObject(&bitSet);
}
#endif

