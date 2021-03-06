// $Header:   Q:/views/common/vcs/MULTISET.H_V   1.4   Mar 24 1997 16:12:40   scottt  $

//	multiset.h
//
//	VMultiSet class interface [Common]
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
//	10/04/95 pkt	created.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VMULTISET_H
#define VMULTISET_H

#include "object.h"
#include "assoc.h"
#include "dictinry.h"

// A VMultiSet associates a positive count with each of its set members.
// As with a VDictionary, the key objects do not belong to this container
// object.

CLASS VMultiSet : public VDictionary {

public:
	// constructors/destructor

					VMultiSet();
					VMultiSet(const VMultiSet&);
					~VMultiSet();

	long			getValueAtKey(const VObject *key) const;
	boolean			atKeyPut(VObject *key, long value);
	VMultiSet&		operator=(const VMultiSet& object);

	VObject		   *addKey(VObject *object);
	VObject		   *findKey(const VObject *object) const;
	boolean			containsKey(const VObject *object) const;
	VObject		   *removeKey(const VObject *object);

#if 0
	VMultiSet&		operator+=(const VMultiSet&);
	VMultiSet&		operator+=(const VCollection&);
	VMultiSet&		operator-=(const VMultiSet&);
	VMultiSet&		operator-=(const VCollection&);
#endif

	VClass		   *iam();

	CLASS MultiAssoc : public VAssoc {
	public:
		// Constructors
						MultiAssoc();
						MultiAssoc(VObject *, long);
						MultiAssoc(const MultiAssoc&);
	
		VClass		   *iam();
	
		MultiAssoc&		operator=(const MultiAssoc&);

#ifndef CV_NOARCHIVER
		// Archiving
		void			getFrom(VArchiver&);
		virtual void 	getFrom(VArchiver& a, VObject *data,
								boolean do_realize = TRUE);
		void			putTo(VArchiver&);
#endif
	};
	declareClassNested(MultiAssoc)
	
protected:
};

extern VClass *VMultiSetCls;

// ---------------------------------------------------------------------------
//
inline VMultiSet::MultiAssoc& VMultiSet::MultiAssoc::operator=(
														const MultiAssoc& ia)
{
	if (&ia != this) {
		reset();
		init();
		copyFrom((MultiAssoc *)(&ia));
	}
	return *this;
}

#endif	//	VMULTISET_H
