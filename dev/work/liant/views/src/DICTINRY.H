// $Header:   Q:/views/common/vcs/DICTINRY.H_V   1.11   Mar 24 1997 16:08:44   scottt  $ 

//	dictinry.h
//
//	VDictionary class interface [Common]
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
//	01/17/94 pkt	preened.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	04/19/95 jld	useAssoc() deletes assoc before reassigning
//	05/19/95 pkt	selected Windows as common source.
//	10/05/95 pkt	added 'const' to member declarations where appropriate.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VDICTINRY_H
#define VDICTINRY_H

// ---------------------------------------------------------------------------
//
#include "set.h"
#include "assoc.h"

// ---------------------------------------------------------------------------
// A VDictionary is a VSet of VAssoc objects.  A dictionary is
// used to maintain a set of key/value object associations. 
// Inclusion in a dictionary set is given by the isEqual method of
// the key object.
//

CLASS VDictionary : public VSet {

public:
	// Constructors
					VDictionary();
					VDictionary(int);
					VDictionary(const VClass *);
					VDictionary(const VDictionary&);
				   ~VDictionary();

	VClass		   *iam();

	// Adding and Removing Associations
	boolean			atKeyPut(VObject *, VObject *);
	boolean			atKeyPut(VObject *, long);
	VObject		   *removeKey(const VObject *aKey);

	// Association Retrieval
	VObject		   *getValueAtKey(const VObject *) const;
	boolean			includesKey(const VObject *) const;
	VSet		   *setOfKeys() const;

	// Copying
	boolean			copyFrom(VObject *);

	VDictionary&	operator=(const VDictionary &);

	// Initialization
	void			reset();
	inline void		useAssoc(VAssoc *cls);

protected:
	void			init();

	// Association class to use for associations.
	VAssoc		   *assoc;
	VAssoc		   *getAssoc(VObject *key, VObject *value);

};

extern VClass *VDictionaryCls;

// ---------------------------------------------------------------------------
//
inline void VDictionary::useAssoc(VAssoc *cls)
{
	if (assoc != 0) {
		delete assoc;
	}
	assoc = cls;
}

// ---------------------------------------------------------------------------
//
inline void VDictionary::init()
{
	assoc = 0;
}

#endif // VDICTINRY_H
