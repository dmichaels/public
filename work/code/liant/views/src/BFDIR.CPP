// $Header:   Q:/views/common/vcs/BFDIR.CPV   1.8   Mar 24 1997 16:08:06   scottt  $

//	bfdir.cpp
//
//	VBlockFileDir class implementation [Common]
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
//	06/30/93 pmcm	created.
//	07/29/93 pmcm	removed typecheck arg from contains()
//	08/03/93 jmd	made contains() const
//	09/08/93 pmcm	freed contents in destructor.
//	01/13/94 dgm	merged and preened
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "bfdir.h"
#include "bfdiritm.h"
#include "ordcllct.h"
#include "iterator.h"

// ---------------------------------------------------------------------------
//	Construct a VBlockFileDir object.
//
VBlockFileDir::VBlockFileDir()
{
	;
}

// ---------------------------------------------------------------------------
//	Destroy this VBlockFileDir object and its contents.
//
VBlockFileDir::~VBlockFileDir()
{
	items.freeContents();
}

// ---------------------------------------------------------------------------
//
void VBlockFileDir::sortBy(method m)
{
	DO(items, VBlockFileDirItem, item)
		item->perform(m);
	END
	items.sort(TRUE, methodOf(VBlockFileDirItem, compare));
}

// ---------------------------------------------------------------------------
VOrdCollect *VBlockFileDir::getSortedList()
{
	VString *listitem;
	VOrdCollect *list = new VOrdCollect(unsigned(items.count()));

	DO(items, VBlockFileDirItem, item)
		listitem = new VString(item->getName());
		list->add(listitem);
	END

	return list;
}

// ---------------------------------------------------------------------------
boolean	VBlockFileDir::contains(const VBlockFileDirItem& item,
								integer& index) const
{
	VBlockFileDirItem i(item);
	return (index = items.indexOf(&i)) == -1L ? FALSE : TRUE;
}
