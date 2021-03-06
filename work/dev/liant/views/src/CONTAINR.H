// $Header:   Q:/views/common/vcs/CONTAINR.H_V   1.10   Mar 24 1997 16:08:36   scottt  $ 

//	containr.h
//
//	VContainer class interface [Common]
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
//	09/26/93 jmd	preened
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened NULL.
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	05/19/95 pkt	selected Windows as common source.
//	08/09/96 pkt	added Win16 support for containers larger than 64K.
//	08/20/96 pkt	updated 8/9 change for archiving support.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VCONTAINR_H
#define VCONTAINR_H

// ---------------------------------------------------------------------------
//
#include "object.h"

// ---------------------------------------------------------------------------
// A VContainer is an array of nEle elements of eleSize
// bytes.  Its contents are allocated from the memory() heap 
// The contents of a VContainer may expand or shrink as needed.
//
CLASS VContainer : public VObject {
public:
	// Constructors & Destructor
					VContainer();
					VContainer(const VContainer&);
					VContainer(unsigned);
					VContainer(integer, unsigned);
				   ~VContainer();

	VClass		   *iam();

	VContainer&		operator=(const VContainer&);

	// VContainer Properties
	virtual integer	size() const;

	handle			getDataHndl();

	unsigned		sizeOfEle();

	// Altering Contents
	boolean			atGet(integer, char*);
	integer			atGet(integer, char *, integer);
	virtual integer	atGet(integer, VContainer *, integer);
	void			atPut(integer, char *);
	integer			atPut(integer, char *, integer);
	virtual integer atPut(integer, VContainer *, integer);

	// Resizing
	void			expand();
	void			expand(integer);
	virtual void	expandTo(integer);

	// Equivalence
	boolean			isEqual(VObject *) const;
	HashKey			hash() const;

	// Duplication
	boolean			copyFrom(VObject *);

	// Direct Access of Contents
	void			clearData();
	pointer			getData() const;
	pointer			setData(pointer);

	// Initialization
	void			reset();

#ifndef CV_NOARCHIVER
	// Archiving
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
	void			putTo(VArchiver&);
#endif

	boolean			isHuge() const;

protected:
	void			init();

protected:
	char	   *data; 		// The contents of this VContainer
	integer		nEle;		// Number of elements in this VContainer
	unsigned	eleSize;	// Size of each element of this VContainer
};

extern VClass *VContainerCls;

#if defined(CV_WIN16)
typedef char huge  *hugePointer;
#else
typedef pointer 	hugePointer;
#endif	


// ---------------------------------------------------------------------------
// Return the number of bytes comprising each element of this VContainer.
//
inline unsigned VContainer::sizeOfEle()
{
	return eleSize;
}

// ---------------------------------------------------------------------------
// Break the association between this VContainer and its data memory.
// Set this VContainer to empty, but do not free the data memory.
//
inline void VContainer::clearData()
{
	data = 0;
	nEle = 0;
}

// ---------------------------------------------------------------------------
// Return a pointer to the contents of this VContainer.
//
inline pointer VContainer::getData() const
{
	return data;
}

// ---------------------------------------------------------------------------
//
inline void VContainer::init()
{
	data = 0;
	nEle = 0;
	eleSize = 0;
}

#endif // VCONTAINR_H
