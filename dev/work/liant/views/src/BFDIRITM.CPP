// $Header:   Q:/views/common/vcs/BFDIRITM.CPV   1.9   Mar 24 1997 16:08:08   scottt  $

//	bfdiritm.cpp
//
//	VBlockFileDirItem class implementation [Common]
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
//	07/28/93 pmcm	added time member
//	07/31/93 jmd	made compare, count, indexOf, hash, etc. const
//	08/07/93 jmd	made copy constructor set time
//	08/10/93 pmcm	changed type of start block to long
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	Gnu C++ changes
//	01/30/94 pmcm   added integer cast in compare() to quiet warning.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "bfdiritm.h"

defineClass(VBlockFileDirItem, VObject)

// ---------------------------------------------------------------------------
//
VBlockFileDirItem::VBlockFileDirItem() 
					: 	keyon(Name), 
						itemTime(0L),
						itemSequence(-1L), 
						itemStartBlock(-1L),
						wildtype(FALSE)
{
 	;
}

// ---------------------------------------------------------------------------
//	Create a Block File Directory Item with the given sequence number, name,
//	and typename.
//
VBlockFileDirItem::VBlockFileDirItem(	const VString& name, 
										const VString& type,
										integer startBlock) 
					: 	keyon(Name), 
						itemTime(0L),
						itemSequence(-1L), 
						itemStartBlock(startBlock),
						wildtype(FALSE)
{
 	setName(name);
	setType(type);
}

// ---------------------------------------------------------------------------
//	Create a Block File Directory Item with contents equal to the given item.
//
VBlockFileDirItem::VBlockFileDirItem(const VBlockFileDirItem& i)
{
 	setName(i.getName());
	setType(i.getType());
	setSequence(i.getSequence());
	setStartBlock(i.getStartBlock());
	keyon = i.keyon;
	wildtype = i.wildtype;
	itemTime = i.itemTime;	
}

// ---------------------------------------------------------------------------
//	Destroy this Block File Directory Item.
//
VBlockFileDirItem::~VBlockFileDirItem() 
{
	;
}

// ---------------------------------------------------------------------------
//
boolean VBlockFileDirItem::isEqual(VObject *obj) const
{
	if (obj->iam()->isa(VBlockFileDirItemCls)) {
		VBlockFileDirItem *i = (VBlockFileDirItem *) obj;

		if (itemName.isEqual(&(i->itemName))) {

			if (!(wildtype || i->wildtype) 			&& 
				!itemType.isEqual(&(i->itemType))		) {

				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
int VBlockFileDirItem::compare(VObject *obj) const
{
	if (obj->iam()->isa(VBlockFileDirItemCls)) {
		VBlockFileDirItem *i = (VBlockFileDirItem *) obj;

		if (i->keyon == keyon) {
			if (keyon == Sequence) {
				return (int) (itemSequence - i->itemSequence);
			}
			else {
		 		VString us(keyon == Name ? itemName : itemType);
				if (keyon == Type) {
					us += itemName;
				}
				else {
					us += itemType;
				}

		 		VString them(keyon == Name ? i->itemName : i->itemType);
		 		them += keyon == Type ? i->itemName : i->itemType;

				return us.compare(&them);
			}
		}
	}

	return 1;
}
