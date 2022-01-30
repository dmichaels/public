// $Header:   Q:/views/common/vcs/BOOL.H_V   1.8   Mar 24 1997 16:08:16   scottt  $ 

//	bool.h
//
//	VBool class interface [Common]
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
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VBOOL_H
#define VBOOL_H

// ---------------------------------------------------------------------------
//
#include "object.h"

// ---------------------------------------------------------------------------
// Boolean represents a boolean value: TRUE or FALSE.
//
CLASS VBool : public VObject {

public:
	// Constructors
					VBool();
					VBool(boolean);
					VBool(const VBool&);

	VClass		   *iam();
	int				operator|| (int i);
	int				operator&&(int i);
	int				operator!();
	operator		int();
	VBool&			operator=(long i);
	VBool&			operator=(const VBool&);

	// Copying
	boolean			copyTo(VObject *);
	boolean			copyFrom(VObject *);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:
	ushort			isTrue;				// truth value
};

extern VClass *VBoolCls;

// ---------------------------------------------------------------------------
// Logical OR operation.
//
inline int VBool::operator|| (int i) 
{
	return isTrue || i;
}

// ---------------------------------------------------------------------------
// Logical AND operation.
//
inline int VBool::operator&&(int i)
{
	return isTrue && i;
}

// ---------------------------------------------------------------------------
// Logical NOT operation.
//
inline int VBool::operator!()
{
	return !isTrue;
}

// ---------------------------------------------------------------------------
// As an 'int' value.
//
inline VBool::operator int()
{
	return isTrue;
}

// ---------------------------------------------------------------------------
// Assignment from a 'long' value.
//
inline VBool& VBool::operator=(long i)
{
	isTrue = i ? TRUE : FALSE;
	return *this;
}

#endif // VBOOL_H
