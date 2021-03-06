// $Header:   Q:/views/common/vcs/BFDIRITM.H_V   1.9   Mar 24 1997 16:08:10   scottt  $ 

//	bfdiritm.h
//
//	VBlockFileDirItem class interface [Common]
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
//	06/30/93 pmcm	created
//  07/19/93 pmcm	added forward declaration of VBlockFile.
//	07/28/93 pmcm	added time member
//	07/31/93 jmd	made compare, count, indexOf, hash, etc. const
//	08/10/93 pmcm	changed type of start block to long
//	11/30/93 pkt	documented setTime and getTime.
//	01/13/94 dgm	merged and preened
//	11/08/95 dss	setWildType() returns boolean.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VBFDIRITM_H
#define VBFDIRITM_H

// ---------------------------------------------------------------------------
//
#include "object.h"
#include "str.h"

// ---------------------------------------------------------------------------
//
CLASS VBlockFile;
CLASS VBlockFileDir;

// ---------------------------------------------------------------------------
//	Block File Directory
//
CLASS VBlockFileDirItem : public VObject {

public:
					VBlockFileDirItem();

					VBlockFileDirItem(	const VBlockFileDirItem& item);

					VBlockFileDirItem(	const VString& name,
										const VString& type,
										integer startBlock = -1L);

				   ~VBlockFileDirItem();

	VClass 		   *iam();

	const VString& 	getName() const;
	const VString& 	getType() const;
	const long		getTime() const;
	integer 		getSequence() const;

	boolean			getWildType() const;
	boolean			setWildType(boolean tf);

	virtual int 	compare(VObject *obj) const;
	virtual boolean	isEqual(VObject *obj) const;

protected:
	enum KeyOn		{ Type, Name, Sequence };

protected:
	KeyOn			keyon;

	void 			keyOnName();
	void 			keyOnType();
	void 			keyOnSequence();

	void			setName(const VString& s);
	void			setType(const VString& s);
	void			setTime(const long t);
	void			setSequence(integer n);

	long			getStartBlock() const;
	void			setStartBlock(long n);

private:
	integer			itemSequence;
	long			itemStartBlock;
	VString			itemName;
	VString			itemType;
	long			itemTime;
	boolean			wildtype;

private:
	friend			VBlockFile;
	friend			VBlockFileDir;
};

extern VClass *VBlockFileDirItemCls;

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::keyOnName()
{
	keyon = Name;
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::keyOnType()
{
	keyon = Type;
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::keyOnSequence()
{
	keyon = Sequence;
}

// ---------------------------------------------------------------------------
//
inline const VString& VBlockFileDirItem::getName() const
{
	return itemName;
}

// ---------------------------------------------------------------------------
//
inline const VString& VBlockFileDirItem::getType() const
{
	return itemType;
}

// ---------------------------------------------------------------------------
// Returns the timestamp of this directory item, measured in seconds since
// the beginning of the year 1970.
//
inline const long VBlockFileDirItem::getTime() const
{
	return itemTime;
}

// ---------------------------------------------------------------------------
//
inline integer VBlockFileDirItem::getSequence() const
{
	return itemSequence;
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::setName(const VString& s) 
{
	itemName.puts(s.gets());
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::setType(const VString& s) 
{
	itemType.puts(s.gets());
}

// ---------------------------------------------------------------------------
// Sets the timestamp of this directory item to 't' seconds after the 
// beginning of the year 1970.
//
inline void VBlockFileDirItem::setTime(const long t) 
{
	itemTime = t;
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::setSequence(integer n) 
{
	itemSequence = n;
}

// ---------------------------------------------------------------------------
//
inline long VBlockFileDirItem::getStartBlock() const
{
	return itemStartBlock;
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDirItem::setStartBlock(long n) 
{
	itemStartBlock = n;
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFileDirItem::getWildType() const
{
	return wildtype;
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFileDirItem::setWildType(boolean tf) 
{
	boolean		oldType = wildtype;
	wildtype = tf;
	return oldType;
}

#endif // VBFDIRITM_H
