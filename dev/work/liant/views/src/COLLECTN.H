// $Header:   Q:/views/common/vcs/COLLECTN.H_V   1.13   Mar 24 1997 16:08:34   scottt  $ 

//	collectn.h
//
//	VCollection class interface [Common]
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
//	07/31/93 jmd	made count const
//	08/09/93 jmd	removed free
//	09/26/93 jmd	preened
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	10/06/95 pkt	updated use of 'const'.
//	04/30/96 pkt	added 'allFollowedBy' and 'allPrecededBy'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VCOLLECTN_H
#define VCOLLECTN_H

// ---------------------------------------------------------------------------
//
#include "object.h"

// ---------------------------------------------------------------------------
//
CLASS VIterator;

// ---------------------------------------------------------------------------
// A VCollection is an abstract class of object referents .  Initially
// the collection is empty.
//
CLASS VCollection : public VObject {

public:
						VCollection(const VCollection&);
						VCollection();
	VClass			   *iam();

	VCollection&		operator=(const VCollection&);

	// Adding, Removing and Accessing Objects

	// Pure virtual function to add object obj to this VCollection.
	virtual VObject	   *add(VObject *obj) = 0;

	VObject			   *remove(VObject *);

	// Pure virtual function that removes and returns the object obj 
	// if obj (by referent) is in this VCollection.
	virtual VObject	   *removeSame(VObject *obj) = 0; 

	VObject			   *replace(VObject *, VObject *); 

	// Pure virtual function that replaces and returns the object obj 
	// if obj is in this VCollection.
	virtual VObject	   *replaceSame(VObject *, VObject *) = 0;

	VObject			   *filter(VObject *);

	// Pure virtual function that returns the object equivalent to obj 
	// if there is one in this VCollection.
	virtual VObject	   *find(const VObject *obj) const = 0;

	// Pure virtual function that returns obj  if obj is in this VCollection.
	virtual VObject	   *findSame(const VObject *obj) const = 0;

	VObject			   *first() const;
	VObject			   *last() const;

	boolean				empty() const;

	// Pure virtual function that returns the number of objects
	// in this VCollection.
	virtual integer		count() const = 0;

	boolean				contains(const VObject *obj) const;
	void				addTo(const VCollection *);

	// Iterating

	// Pure virtual function that sets the VIterator i to the first
	// object in this VCollection or to a null pointer if this VCollection
	// is empty.
	virtual void		start(VIterator&i) const = 0;

	// Pure virtual function to set VIterator i to the last object in 
	// this VCollection or to null pointer if it is empty.
	virtual void		end(VIterator&i) const = 0;
	virtual void		endIterator(VIterator&i) const = 0;

	// Pure virtual function that returns the current object refered to by
	// the VIterator i and increments i to reference the next object.
	virtual VObject	   *next(VIterator&i) const = 0;

	// Pure virtual function that returns the current object referred to
	// by the VIterator i and decrements i to reference the previous object.
	virtual VObject	   *prev(VIterator&) const = 0;

	// Pure virtual function that tells whether the VIterator i is pointing
	// past the end of the entire collection.
	virtual boolean		allFollowedBy(VIterator&i) const = 0;

	// Pure virtual function that tells whether the VIterator i is pointing
	// before the start of the entire collection.
	virtual boolean		allPrecededBy(VIterator&i) const = 0;

	// Performing
	void				doPerform(method) const;
	void				doPerform(method, long) const;
	void				doPerform(method, long, long) const;
	void				doPerform(method, long, long, long) const;
	void				reverseDo(method) const;
	void				reverseDo(method, long) const;

	// Pure virtual function that frees all of the objects in this
	// VCollection and does not free this VCollection.
	virtual void		freeContents() = 0;

	virtual void		freeAll();

	// Copying
	boolean				copyFrom(VObject *);

protected:
	void				init();

};

extern VClass *VCollectionCls;

// ---------------------------------------------------------------------------
//
inline void VCollection::init()
{
}

// ---------------------------------------------------------------------------
// Return TRUE if this VCollection does not contain any objects,
// otherwise return FALSE.
//
inline boolean VCollection::empty() const
{
	return count() ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VCollection contains an object equivalent to
// obj, otherwise return FALSE.
//
inline boolean VCollection::contains(const VObject *obj) const
{
	return find(obj) ? TRUE : FALSE;
}

#endif // VCOLLECTN_H
