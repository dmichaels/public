// $Header:   Q:/views/common/vcs/INTSET.H_V   1.10   Mar 24 1997 16:09:30   scottt  $ 

//	intset.h
//
//	VIntegerSet class interface [Common]
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
//	01/18/94 pkt	preened.
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VINTSET_H
#define VINTSET_H

// ---------------------------------------------------------------------------
//
#include "containr.h"

// ---------------------------------------------------------------------------
// A VIntegerSet is a hash table of long integer values.
//
CLASS VIntegerSet : public VContainer {
public:
	// Constructors
					VIntegerSet();
					VIntegerSet(integer);
					VIntegerSet(const VIntegerSet&);
	VClass		   *iam();

	VIntegerSet&	operator=(const VIntegerSet&);

	// Querying, Adding and Removing Values
	integer			count();
	boolean			add(long);
	boolean			find(long);
	integer			indexOf(long);
	long			remove(long);

	// Initialization
	void			expandTo(integer);
	void			reset();
	boolean			copyFrom(VObject *);

#ifndef CV_NOARCHIVER
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
	void			putTo(VArchiver&);
#endif

protected:
	void			init();

	integer			cnt;	// number of objects in this VIntegerSet

};

extern VClass *VIntegerSetCls;

// ---------------------------------------------------------------------------
//
inline void VIntegerSet::init()
{
	cnt = 0;
}

#endif // VINTSET_H
