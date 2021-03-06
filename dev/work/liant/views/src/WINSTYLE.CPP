// $Header:   Q:/views/common/vcs/WINSTYLE.CPV   1.9   Mar 24 1997 16:14:34   scottt  $

//	winstyle.cpp
//
//	miscellaneous window style definitions [Common]
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
//	09/26/95 dss	Replace ctor w/VImplictStyle w/ctor w/VStyle. Set persist
//					on all operator(const VStyle&) methods.
//	10/02/95 dss	StyleOperation now enum and moved in class VStyle.
//	10/12/95 dss	don't allow set/add on ourselves.
//	12/06/95 dss	name change: VStyleExpression.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "archiver.h"

#include "bitset.h"
#include "style.h"
#include "exclstyl.h"
#include "implstyl.h"
#include "stylexpr.h"
#include "winstyle.h"

defineClass(VWinStyle, VImplicitStyle)

void VWinStyle::add(const VStyleExpression& a)
{
	persistent.add(a.getPersistent(VStyle::addOp));

	// NOTE: do not call VImplicitStyle::add(VStyleExpression&) as this will
	// call VWinStyle::add(VImplicitStyle&) which is the VImplicitStyle form of the
	// add method, which is recursive. Since persistence is taken care of,
	// already, just call into the VImplicitStyle class to do the right thing.
	VImplicitStyle::add(a.getResult());

}

void VWinStyle::remove(const VStyleExpression& a)
{
	persistent.remove(a.getPersistent(VStyle::removeOp));

	// NOTE: do not call VImplicitStyle::remove(VStyleExpression&) as this will
	// call VWinStyle::remove(VImplicitStyle&) which is the VImplicitStyle form of the
	// remove method, which is recursive. Since persistence is taken care of, 
	// already, just call into the VImplicitStyle class to do the right thing.
	VImplicitStyle::remove(a.getResult());
}

void VWinStyle::set(const VStyleExpression& a)
{
	persistent.set(a.getPersistent(VStyle::addOp)); // use addOp, not setOp!

	// NOTE: do not call VImplicitStyle::set(VStyleExpression&) as this will
	// call VWinStyle::set(VImplicitStyle&) which is the VImplicitStyle form of the
	// remove method, which is recursive. Since persistence is taken care of, 
	// already, just call into the VImplicitStyle class to do the right thing.
	VImplicitStyle::set(a.getResult());
}

void VWinStyle::add(const VWinStyle& a)
{
	if (this == &a) {
		return;
	}

	persistent.add(a);
	VImplicitStyle::add(a);
}

void VWinStyle::remove(const VWinStyle& a)
{
	persistent.remove(a);
	VImplicitStyle::remove(a);
}

void VWinStyle::set(const VWinStyle& a)
{
	if (this == &a) {
		return;
	}

	persistent.set(a);
	VImplicitStyle::set(a);
}

void VWinStyle::set(const VStyle& a)
{
	if (a.isA(VWinStyleCls)) {
		VWinStyle::set((VWinStyle&)a);
	}
	else if (a.isA(VImplicitStyleCls)) {
		persistent.set(a);
		VImplicitStyle::set((VImplicitStyle&)a);
	}
	else if (a.isA(VExclusiveStyleCls)) {
		persistent.set(a);
		VExclusiveStyle::set((VExclusiveStyle&)a);
	}
	else {
		persistent.set(a);
		VStyle::set(a);
	}
}

void VWinStyle::add(const VStyle& a)
{
	if (a.isA(VWinStyleCls)) {
		VWinStyle::add((VWinStyle&)a);
	}
	else if (a.isA(VImplicitStyleCls)) {
		persistent.add(a);
		VImplicitStyle::add((VImplicitStyle&)a);
	}
	else if (a.isA(VExclusiveStyleCls)) {
		persistent.add(a);
		VExclusiveStyle::add((VExclusiveStyle&)a);
	}
	else {
		persistent.add(a);
		VStyle::add(a);
	}
}

void VWinStyle::remove(const VStyle& a)
{
	if (a.isA(VWinStyleCls)) {
		VWinStyle::remove((VWinStyle&)a);
	}
	else if (a.isA(VImplicitStyleCls)) {
		persistent.remove(a);
		VImplicitStyle::remove((VImplicitStyle&)a);
	}
	else if (a.isA(VExclusiveStyleCls)) {
		persistent.remove(a);
		VExclusiveStyle::remove((VExclusiveStyle&)a);
	}
	else {
		persistent.remove(a);
		VStyle::remove(a);
	}
}

#if defined(V_UNIT_TEST)
void VWinStyle::dump(ostream& s) const
{
	VImplicitStyle::dump(s);

	if (!persistent.isEmpty()) {
		s << ", ";
		// Use VStyle::dump to prevent dumping the extraneous
		// stuff we don't care about.
		persistent.VStyle::dump(s);
	}
	
}
#endif

#ifndef CV_NOARCHIVER
void VWinStyle::getFrom(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "retrieving VWinStyle " << name << endl;
#endif

	// don't call VImplicitStyle::getFrom() !!! since we only archived the
	// persistent styles.
	a.getObject(&persistent);

	VStyle::add(persistent);
}

void VWinStyle::putTo(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "archiving VWinStyle " << name << endl;
#endif

	// don't call VImplicitStyle::putTo() !!! We only want to archive the
	// persistent styles. This objects style was set up to a default
	// when it was constructued and we don't want to erase the defaults.
	// We want the persistent styles to add to the current values.
	a.putObject(&persistent);
}

#endif // CV_NOARCHIVER
