// $Header:   Q:/views/common/vcs/BFDIR.H_V   1.8   Mar 24 1997 16:08:06   scottt  $ 

//	bfdir.h
//
//	VBlockFile class interface [Common]
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
//	07/29/93 pmcm	removed typecheck arg from contains()
//	08/03/93 jmd	made contains() const
//  08/09/93 jmd	removed free
//	11/01/93 pkt	changed ordcllctn.h to ordcllct.h.
//	01/13/94 dgm	merged and preened
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VBFDIR_H
#define VBFDIR_H

// ---------------------------------------------------------------------------
//
#include "bfdiritm.h"	//	for inline sortBy() methods
#include "ordcllct.h"

// ---------------------------------------------------------------------------
//
CLASS VBlockFile;

// ---------------------------------------------------------------------------
//	Block File Directory
//
CLASS VBlockFileDir {

public:
						VBlockFileDir();
					   ~VBlockFileDir();

	//	Querying contents

	integer				count() const;

	VOrdCollect 	   *getItems();			//	VBlockFileDirItem collection
	VOrdCollect 	   *getSortedList();	//	VString collection

	boolean				contains(	const VString& 	name, 
									integer& 		index) const;
  
	boolean				contains(	const VBlockFileDirItem& item, 
									integer& 		index) const;

	VBlockFileDirItem *getItemAt(integer i);

	integer				indexOf(const VBlockFileDirItem& item) const;
	integer				indexOf(const VString& name, const VString& type);

	//	Sorting

	void 				sortByName();
	void 				sortByType();
	void 				sortBySequence();

protected:
	void				addItem(VBlockFileDirItem *item);
	void				addItem(const VBlockFileDirItem& item);
	void				clear();
	void				clearContents();

private:
	void				sortBy(method m);

private:
	VOrdCollect 		items;

private:
	friend				VBlockFile;
};


// ---------------------------------------------------------------------------
//
inline VBlockFileDirItem *VBlockFileDir::getItemAt(integer i)
{
	return (VBlockFileDirItem *) items.idAt(i);
}

// ---------------------------------------------------------------------------
//
inline integer VBlockFileDir::indexOf(const VBlockFileDirItem& item) const
{
	VBlockFileDirItem i(item);

	return items.indexOf(&i);
}

// ---------------------------------------------------------------------------
//
inline integer VBlockFileDir::indexOf(const VString& name,const VString& type)
{
	VBlockFileDirItem i(name, type);

	return items.indexOf(&i);
}

// ---------------------------------------------------------------------------
//
inline integer VBlockFileDir::count() const
{
 	return items.count();
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::sortByName()
{
	sortBy(methodOf(VBlockFileDirItem, keyOnName));
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::sortByType()
{
	sortBy(methodOf(VBlockFileDirItem, keyOnType));
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::sortBySequence()
{
	sortBy(methodOf(VBlockFileDirItem, keyOnSequence));
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::addItem(const VBlockFileDirItem& item)
{
	VBlockFileDirItem *i = new VBlockFileDirItem(item);

	(void) items.add(i);
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::addItem(VBlockFileDirItem *item)
{
	(void) items.add(item);
}
// ---------------------------------------------------------------------------
//
inline VOrdCollect *VBlockFileDir::getItems()
{
	return &items;
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::clear()
{
	items.free();		// !!!jmd what is this supposed to do?
}

// ---------------------------------------------------------------------------
//
inline void VBlockFileDir::clearContents()
{
	items.freeContents();
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFileDir::contains(const VString& name,
									   integer& index) const
{
	VBlockFileDirItem item(name, VString());
	item.setWildType(TRUE);
		
	return contains(item, index);
}

#endif // VBFDIR_H
