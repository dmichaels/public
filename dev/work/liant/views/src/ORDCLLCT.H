// $Header:   Q:/views/common/vcs/ORDCLLCT.H_V   1.15   Mar 24 1997 16:12:48   scottt  $ 

//	ordcllct.h
//
//	VOrdCollect class interface [Common]
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
//	07/31/93 jmd	preened, added some consts
//	09/17/93 dgm	(jmd) changed idAt() to be const.
//	10/07/93 dgm	added support for archiving VOrdCollects of GUI objects;
//					primarily for proper dialog-editor clipboard support.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	12/29/94 pkt	updated for 'VListBox::putTo' fix.
//	05/19/95 pkt	merged for common source.
//	06/07/95 pkt	fixed #ifdef ARCHIVER to use CV_NOARCHIVER.
//	09/06/95 pkt	removed OFI code.
//	10/06/95 pkt	updated use of 'const'.
//	04/30/96 pkt	added 'allFollowedBy' and 'allPrecededBy'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VORDCLLCT_H
#define VORDCLLCT_H

// ---------------------------------------------------------------------------
//
#include "collectn.h"
#include "objarray.h"

CLASS VListBox;

// ---------------------------------------------------------------------------
// A VOrdCollect is a VCollection whose objects are ordered
// from 0 to count() - 1. The contents of a VOrdCollect object are expanded
// to accomodate new objects.
//
CLASS VOrdCollect : public VCollection {

public:
	// Contructors & Destructor
						VOrdCollect();
						VOrdCollect(unsigned n);
						VOrdCollect(const VOrdCollect&);

	VClass			   *iam();

	// Adding and Removing Objects
	VObject			   *add(VObject *obj);
	virtual VObject    *insertAt(integer i, VObject *obj);
	virtual VObject    *removeAt(integer i);

	VObject 		   *removeSame(VObject *obj);		// by referent
	VObject 		   *replaceSame(VObject *obj,		// by referent
									VObject *newobj);

	virtual VObject	   *replaceAt(integer i, VObject *newobj);

	// Iterating
	void				start(VIterator& i) const;
	void				end(VIterator& i) const;
	void				endIterator(VIterator& i) const;
	VObject			   *next(VIterator& i) const;
	VObject			   *prev(VIterator& i) const;
	boolean				allFollowedBy(VIterator&i) const;
	boolean				allPrecededBy(VIterator&i) const;

	// Accessing Contents
	virtual integer		indexOf(VObject *obj) const;
	virtual integer		indexOfSame(VObject *obj) const;

	VObject			   *find(const VObject *obj) const;		// by isEqual
	VObject			   *findSame(const VObject *obj) const;	// by referent

	virtual VObject	   *idAt(integer i) const;

	// Sorting Collection
	void				sort(boolean increasing, method m = 0);

#ifndef CV_NOARCHIVER
	// Archival
	void				getFrom(VArchiver& a);
	void				getFrom(VArchiver& a, VObject *parent,
								boolean do_realize = TRUE);
	void				putTo(VArchiver& a);
#endif

	// Querying VOrdCollect
	integer				count() const;

	// Initialization
	void				freeContents();
	void				reset();

	// Expand this VOrdCollect.
	virtual void		expandTo(integer n);
	void				expand();

	// Comparison and Copying
	HashKey				hash() const;
	boolean				isEqual(VObject *obj) const;

	VOrdCollect&		operator=(const VOrdCollect&);

	boolean				copyFrom(VObject *obj);

protected:
	void				init();

	VObjArray	contents; // Array of Objects
	integer		nextSlot; // Index of the next available slot in the collection
	
	friend		VListBox;

};

extern VClass *VOrdCollectCls;

// ---------------------------------------------------------------------------
//
inline void VOrdCollect::init()
{
	nextSlot = 0;
}

// ---------------------------------------------------------------------------
//
inline VOrdCollect& VOrdCollect::operator=(const VOrdCollect& oc)
{
	if (&oc != this) {
		reset();
		init();
		copyFrom((VOrdCollect *)(&oc));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
inline void VOrdCollect::expand()
{
	expandTo(contents.size() + (contents.size() >> 1)); 
}

#endif // VORDCLLCT_H
