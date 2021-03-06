// $Header:   Q:/views/common/vcs/SET.H_V   1.15   Mar 24 1997 16:13:20   scottt  $ 

//	set.h
//
//	VSet class interface [Common]
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
//	05/17/93 pmcm	merged MOTIF and MS-Windows versions
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	08/07/93 jmd	preened
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	01/19/94 pkt	preened and merged with Macintosh.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	merged for common source.
//	05/25/95 dgm	inverted #if check from ARCHIVER to CV_NOARCHIVER.
//	09/06/95 pkt	removed OFI code.
//	10/06/95 pkt	updated use of 'const'.
//	04/30/96 pkt	added 'allFollowedBy' and 'allPrecededBy'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSET_H
#define VSET_H

// ---------------------------------------------------------------------------
//
#include "collectn.h"
#include "objarray.h"

// ---------------------------------------------------------------------------
// A VSet is a VCollection of non-matching objects.
// Two objects match if they are equal via the isEqual method.
// A VSet object is implemented as a hash table.  A hash key is determined
// for an object by calling its hash() method.
//
CLASS VSet : public VCollection {
public:
	// Constructors & Destructor
				VSet();
				VSet(integer);
				VSet(const VSet& set);

	VClass	   *iam();

	// Adding and Removing Elements
	VObject	   *add(VObject *);
	VObject	   *removeSame(VObject *);
	VObject	   *replaceSame(VObject *, VObject *);

	// Querying
	VObject	   *find(const VObject *) const;
	VObject	   *findSame(const VObject *) const;
	integer		indexOf(const VObject *) const;
	integer		indexOfSame(const VObject *) const;

	// Iterating
	void		start(VIterator& i) const;
	void		end(VIterator& i) const;
	void		endIterator(VIterator& i) const;
	VObject	   *next(VIterator& i) const;
	VObject	   *prev(VIterator& i) const;
	boolean		allFollowedBy(VIterator&i) const;
	boolean		allPrecededBy(VIterator&i) const;

	// Set Properties
	integer		count() const;
	boolean		isEqual(VObject *) const;
	HashKey		hash() const;

	// Initialization
	void		reset();
	void		freeContents();

#ifndef CV_NOARCHIVER
	// Archival
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

	// Resizing
	void		expandTo(integer);

	// Copying
	VSet&		operator=(const VSet&);
	boolean		copyFrom(VObject *);

protected:
	void		init();

	integer		cnt;		// number of objects in this VSet
	VObjArray	contents;	// contents of this VSet

};

extern VClass *VSetCls;

// ---------------------------------------------------------------------------
//
inline void VSet::init()
{
	cnt = integer(0);
}

// ---------------------------------------------------------------------------
// Copy VCollection c and its contents to this VSet.
//
inline VSet& VSet::operator=(const VSet& s)
{
	if (&s != this) {
		reset();
		init();
		copyFrom((VSet *)(&s));
	}
	return *this; 
}

#endif // VSET_H
