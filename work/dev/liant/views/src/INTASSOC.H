// $Header:   Q:/views/common/vcs/INTASSOC.H_V   1.11   Mar 24 1997 16:09:24   scottt  $ 

//	intassoc.h
//
//	VIntAssoc class interfcae [Common]
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
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VINTASSOC_H
#define VINTASSOC_H

// ---------------------------------------------------------------------------
//
#include "assoc.h"

// ---------------------------------------------------------------------------
// The VIntAssoc class is an association of two long integer values,
// one known as the key and the other known as the value.  VIntAssoc is
// used in conjunction with the VDictionary class for accessing values via
// keys.  Comparison is performed by key integer value, not by object 
// equivalence.
//
CLASS VIntAssoc : public VAssoc {
public:
	// Constructors
				VIntAssoc();
				VIntAssoc(long, long);
				VIntAssoc(const VIntAssoc&);

	VClass	   *iam();

	VIntAssoc&	operator=(const VIntAssoc&);

	// Equivalence
	boolean		isEqual(VObject *) const;
	HashKey		hash() const;

	long		getIntKey();
	long		getIntValue();
	void		setIntKey(long newKey);
	void		setIntValue(long newValue);
	void		setIntKeyValue(long newKey, long newValue);


#ifndef CV_NOARCHIVER
	// Archiving
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
	void			putTo(VArchiver&);
#endif

};

extern VClass *VIntAssocCls;

// ---------------------------------------------------------------------------
//
inline VIntAssoc& VIntAssoc::operator=(const VIntAssoc& ia)
{
	if (&ia != this) {
		reset();
		init();
		copyFrom((VIntAssoc *)(&ia));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
inline long VIntAssoc::getIntKey()
{
	return (long) key;
}
// ---------------------------------------------------------------------------
//
inline long VIntAssoc::getIntValue()
{
	return (long) value;
}
// ---------------------------------------------------------------------------
//
inline void VIntAssoc::setIntKey(long newKey)
{
	key = (VObject *) newKey;
}
// ---------------------------------------------------------------------------
//
inline void VIntAssoc::setIntValue(long newValue)
{
	value = (VObject *) newValue;
}
// ---------------------------------------------------------------------------
//
inline void VIntAssoc::setIntKeyValue(long newKey, long newValue)
{
	key = (VObject *) newKey;
	value = (VObject *) newValue;
}
#endif // VINTASSOC_H
