// $Header:   Q:/views/common/vcs/OBJARRAY.H_V   1.12   Mar 24 1997 16:12:44   scottt  $ 

//	objarray.h
//
//	VObjArray class interface [Common]
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
//	10/07/93 dgm	added support for archiving VObjArrays of GUI objects;
//					primarily for proper dialog-editor clipboard support.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	10/06/95 pkt	updated use of 'const'.
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VOBJARRAY_H
#define VOBJARRAY_H

// ---------------------------------------------------------------------------
//
#include "containr.h"

// ---------------------------------------------------------------------------
//
CLASS VOrdCollect;
CLASS VCollection;

// ---------------------------------------------------------------------------
// A VObjArray is a VContainer of object referents.
// Initially it is empty.
//
CLASS VObjArray : public VContainer {
public:
	// Constructors
				VObjArray();
				VObjArray(integer);
				VObjArray(const VObjArray&);

	VClass	   *iam();

	VObjArray&	operator=(const VObjArray&);

	// Adding, Removing and Accessing Objects

	void		add(VObject *);

	VObject	  *&operator[](integer i);		// synonym for idAt(i)
	VObject	   *idAt(integer) const;

	void		atPutId(integer, VObject *);
	integer		indexOf(const VObject *) const;
	integer		indexOfSame(const VObject *) const;
	boolean		contains(VObject *);
	VObject	   *find(const VObject *) const;
	VObject	   *findSame(const VObject *) const;

	void		freeContents();
	void		freeAll();
	void		reset();

	// Equivalence

	boolean		isEqual(VObject *) const;
	HashKey		hash() const;

#ifndef CV_NOARCHIVER
	// Archiving
	void		getFrom(VArchiver &);
	void		getFrom(VArchiver& a, VObject *parent,
						boolean do_realize = TRUE);
	void		putTo(VArchiver &);
#endif

protected:
	void		init();

private:
	friend	VCollection;

};

extern VClass *VObjArrayCls;

// ---------------------------------------------------------------------------
//
typedef VObject			   *objPointer;
#ifdef CV_WIN16
typedef objPointer huge    *hugeObjPointerPointer;
#else
typedef objPointer		   *hugeObjPointerPointer;
#endif

// ---------------------------------------------------------------------------
//
inline void VObjArray::init()
{
	eleSize = sizeof(VObject *);
}

// ---------------------------------------------------------------------------
// Free this VObjArray and all of its contents.
//
inline void VObjArray::freeAll()
{
	freeContents();
	delete this;
}

#endif // VOBJARRAY_H
