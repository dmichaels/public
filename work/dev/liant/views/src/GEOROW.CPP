// $Header:   Q:/views/common/vcs/GEOROW.CPV   1.5   Mar 24 1997 16:09:14   scottt  $

//	georow.cpp
//
//	VGeoRow implementation [Common]
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
//	09/29/95 dgm	Original.
//	10/02/95 dgm	Updated.
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "georow.h"

// --------------------------------------------------------------------------
//
#if !defined(CV_NOARCHIVER)
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VGeoRow, VGeoRowOrColumn)
defineArchiveRevision(VGeoRow, 0)

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow(const VFrame& frame, VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Row, frame, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow(int x, int y, int w, int h, VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Row, x, y, w, h, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow(int w, int h, VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Row, w, h, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow(VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Row, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow(const VFrame& frame, VWindow *parent_window)
	: VGeoRowOrColumn(VGeoRowOrColumn::Row, frame, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow(VWindow *parent_window)
	: VGeoRowOrColumn(VGeoRowOrColumn::Row, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoRow::VGeoRow()
	: VGeoRowOrColumn(VGeoRowOrColumn::Row)
{
}

// ==========================================================================

#if !defined(CV_NOARCHIVER)

// --------------------------------------------------------------------------
//
void VGeoRow::putTo(VArchiver& a)
{
	putTo(a, TRUE);
	putToDone(a);
}

// --------------------------------------------------------------------------
//
void VGeoRow::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base class data.

	VGeoRowOrColumn::putTo(a, FALSE);

	// Write the revision tag.

	a << VGeoRowArchiveTag;

	// Write the class specific data.

	;
}

// --------------------------------------------------------------------------
//
void VGeoRow::getFrom(VArchiver& a, VObject *object, boolean)
{
	getFrom(a);
	getFromDone(a, object);
}

// --------------------------------------------------------------------------
//
void VGeoRow::getFrom(VArchiver& a)
{
	// Read the base class data.

	VGeoRowOrColumn::getFrom(a);

	// Read and verify the revision tag.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VGeoRowArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VGeoRowArchiveTag));
		return;
	}

	// Read the class specific data.

	;
}

#endif
