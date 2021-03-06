// $Header:   Q:/views/common/vcs/ASSOC.H_V   1.11   Mar 24 1997 16:08:02   scottt  $ 

//	assoc.h
//
//	VAssoc class interface [Common]
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
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VASSOC_H
#define VASSOC_H

// ---------------------------------------------------------------------------
//
#include "object.h"

// ---------------------------------------------------------------------------
//
CLASS VClass;
CLASS VObject;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

// ---------------------------------------------------------------------------
// The VAssoc class provides a mechanism for associating two objects, 
// one known as the key and the other known as the value. VAssoc is used in
// conjunction with the VDictionary class for accessing
// values via keys. The key and value member variables may be accessed directly.
//
CLASS VAssoc : public VObject {

public:
	// Constructors
					VAssoc();
					VAssoc(VObject *, VObject *);
					VAssoc(const VAssoc&);
	VClass		   *iam();

	// Equivalence
	boolean			isEqual(VObject *) const;
	HashKey			hash() const;

	boolean			copyFrom(VObject *);
	VAssoc&			operator=(const VAssoc&);
	VObject			*getKey();
	VObject			*getValue();
	void			setKey(VObject *newKey);
	void			setValue(VObject *newValue);
	void			setKeyValue(VObject *newKey, VObject *newValue);

#ifndef CV_NOARCHIVER
	// Archiving
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
	void			putTo(VArchiver&);
#endif

protected:
	void			init();

public:
	VObject		   *key;		// key of association
	VObject		   *value;		// value of association

};

extern VClass *VAssocCls;

// ---------------------------------------------------------------------------
// Construct a VAssoc object with a null key and value.
//
inline void VAssoc::init()
{
	key = value = 0;
}

// ---------------------------------------------------------------------------
//
inline VObject *VAssoc::getKey()
{
	return key;
}
// ---------------------------------------------------------------------------
//
inline VObject *VAssoc::getValue()
{
	return value;
}
// ---------------------------------------------------------------------------
//
inline void VAssoc::setKey(VObject *newKey)
{
	key = newKey;
}
// ---------------------------------------------------------------------------
//
inline void VAssoc::setValue(VObject *newValue)
{
	value = newValue;
}
// ---------------------------------------------------------------------------
//
inline void VAssoc::setKeyValue(VObject *newKey, VObject *newValue)
{
	key = newKey;
	value = newValue;
}
#endif // VASSOC_H
