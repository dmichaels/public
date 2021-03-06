// $Header:   Q:/views/common/vcs/ITERATOR.H_V   1.14   Mar 24 1997 16:09:32   scottt  $ 

//	iterator.h
//
//	VIterator class interface [Common]
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
//	04/09/93 pmcm	changed id to VObject *, added operator VObject *
//	08/09/93 jmd	removed free
//	08/23/93 pmcm	removed CLASS keyword from friend declaration
//	09/21/93 pmcm	renamed operator id () operator VObject * ()
//	09/26/93 jmd	preened
//	10/10/93 jmd	preened DO macro
//	11/08/93 pkt	fixed init to initialize pColl.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	06/21/94 dgm	Minor changes for the SunoS C++ compiler.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	10/06/95 pkt	updated use of 'const'.
//	04/30/96 pkt	added 'followsAll' and 'precedesAll'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VITERATOR_H
#define VITERATOR_H

// ---------------------------------------------------------------------------
//
#include "object.h"
#include "collectn.h"

// ---------------------------------------------------------------------------
//
#ifdef CVP_NEWINCR
#	define POST_INCR int
#else
#	define POST_INCR 
#endif

// ---------------------------------------------------------------------------
// A VIterator facilitates iteration through the elements of
// any VCollection. Its constructors take a VCollection object as an
// argument. Once constructed, an VIterator may be incremented or decremented
// through the VCollection. It maintains a current index and a current object. 
// It is intended to be used via the DO, REVERSEDO and END macros, for example, 
// DO(collect,class,ident) ... END where collect is class derived from
// VCollection, class is any class name (derived from VObject) and ident
// is an identifier of type class * with scope between the DO/END pair.
// 
CLASS VIterator : public VObject {

public:
	// Constructors & Destructor
					VIterator();
					VIterator(const VCollection&);
					VIterator(const VCollection *);
					VIterator(const VIterator&);
				   ~VIterator();

	VClass		   *iam();

	void			reset();

	VIterator&		operator=(const VIterator&);

	// VIterator Properties
	VObject		   *operator++(POST_INCR);
	VObject		   *operator--(POST_INCR);
	void			start();
	void			end();
	void			endIterator();
	boolean			followsAll();
	boolean			precedesAll();
	operator		VObject *();
	operator		integer();
	VIterator&		operator=(integer n);
	VIterator&		operator=(VObject *obj);
	int				operator==(integer n);
	int				operator==(VObject *obj);
	const VCollection *collection();
	void			collection(const VCollection *collect);

	VObject*		next();
	VObject*		prev();


	// Copying
	boolean			copyFrom(VObject *);

#ifndef CV_NOARCHIVER
	// Archiving
	void			getFrom(VArchiver &);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
	void			putTo(VArchiver &);
#endif

protected:
	void			init();

	integer			nCurr;	// index of current element of this VIterator
	VObject		   *pCurr;	// current element of this VIterator
	const VCollection *pColl;	// the VCollection this VIterator operates on

private:
	friend		VCollection;

};

extern VClass *VIteratorCls;

// ---------------------------------------------------------------------------
//
inline void VIterator::init()
{
	pCurr = 0;
	nCurr = 0;
	pColl = 0;
}

// ---------------------------------------------------------------------------
// Return the current object and bump this VIterator. 
// The POST_INCR flag is used to insure this is a post-increment
// operator overload.
//
inline VObject *VIterator::operator++(POST_INCR)
{
	return pColl->next(*this);
}

// ---------------------------------------------------------------------------
// Return the current object and decrement this VIterator. 
// The POST_INCR flag is used to insure this is a post-increment
// operator overload.
//	
inline VObject *VIterator::operator--(POST_INCR)
{
	return pColl->prev(*this);
}

// ---------------------------------------------------------------------------
// Set this iterator to the first object in its VCollection.
//
inline void VIterator::start()
{
	pColl->start(*this);
}

// ---------------------------------------------------------------------------
// Set this VIterator to the last object in its collection.
//
inline void VIterator::end()
{
	pColl->end(*this);
}

// ---------------------------------------------------------------------------
//
inline void VIterator::endIterator()
{
	pColl->end(*this);
}

// ---------------------------------------------------------------------------
// Return whether this VIterator points after every object in its collection.
//
inline boolean VIterator::followsAll()
{
	return pColl->allFollowedBy(*this);
}

// ---------------------------------------------------------------------------
// Return whether this VIterator points ahead of every object in its collection.
//
inline boolean VIterator::precedesAll()
{
	return pColl->allPrecededBy(*this);
}

// ---------------------------------------------------------------------------
// Return the current object of this VIterator.
//
inline VIterator::operator VObject *()
{
	return pCurr;
}	

// ---------------------------------------------------------------------------
// Return the current index of this VIterator.
//
inline VIterator::operator integer()
{
	return nCurr;
}	

// ---------------------------------------------------------------------------
// Set the current index of this VIterator to the ith object
// in its collection. This method should only be used by VCollection classes.
//
inline VIterator& VIterator::operator=(integer n)
{
	nCurr = n;
	return *this;
}

// ---------------------------------------------------------------------------
// Set the current object of this VIterator to obj. This method is
// to be used only by VCollection classes.
//
inline VIterator& VIterator::operator=(VObject *obj)
{
	pCurr = obj;
	return *this;
}

// ---------------------------------------------------------------------------
// Return TRUE if the current index of this VIterator is n,
// otherwise return FALSE.
//
inline int VIterator::operator==(integer n)
{
	return nCurr == n ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Return TRUE if the current object of this VIterator is obj,
// otherwise return FALSE.
//
inline int VIterator::operator== (VObject *obj)
{
	return pCurr == obj ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Return the VCollection this VIterator is iterating on.
//
inline const VCollection *VIterator::collection()
{
	return pColl;
}

// ---------------------------------------------------------------------------
// Set the VCollection this VIterator is to iterate on to collect.
//
inline void VIterator::collection(const VCollection *collect)
{
	pColl = collect;
}

// ---------------------------------------------------------------------------
//
inline void VIterator::reset()
{
	VIterator::init();
}
// ---------------------------------------------------------------------------
//
inline VObject *VIterator::next()
{
	return (*this)++;
}
// ---------------------------------------------------------------------------
//
inline VObject *VIterator::prev()
{
	return (*this)--;
}

// ---------------------------------------------------------------------------
//
#define DO(collection, type, var) \
		{ \
			type *var; \
			boolean more_iter; \
			VIterator I(collection); \
			while(more_iter = !I.followsAll(), var = (type *)I++, more_iter) {

// ---------------------------------------------------------------------------
//
#define REVERSEDO(collection, type, var) \
		{ \
			type *var; \
			boolean more_iter; \
		 	VIterator I(collection); \
			I.end(); \
			while(more_iter = !I.precedesAll(), var = (type *)I--, more_iter) {

#define END }}

#endif // VITERATOR_H
