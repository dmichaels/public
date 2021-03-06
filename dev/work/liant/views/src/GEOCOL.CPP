// $Header:   Q:/views/common/vcs/GEOCOL.CPV   1.5   Mar 24 1997 16:09:00   scottt  $

//	geocol.cpp
//
//	VGeoColumn implementation [Common]
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

#include "geocol.h"

// --------------------------------------------------------------------------
//
#if !defined(CV_NOARCHIVER)
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VGeoColumn, VGeoRowOrColumn)
defineArchiveRevision(VGeoColumn, 0)

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn(const VFrame& frame, VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Column, frame, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn(int x, int y, int w, int h, VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Column, x, y, w, h, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn(int w, int h, VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Column, w, h, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn(VGeoManager *parent)
	: VGeoRowOrColumn(VGeoRowOrColumn::Column, parent)
{
}

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn(const VFrame& frame, VWindow *parent_window)
	: VGeoRowOrColumn(VGeoRowOrColumn::Column, frame, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn(VWindow *parent_window)
	: VGeoRowOrColumn(VGeoRowOrColumn::Column, parent_window)
{
}

// --------------------------------------------------------------------------
//
VGeoColumn::VGeoColumn()
	: VGeoRowOrColumn(VGeoRowOrColumn::Column)
{
}

// ==========================================================================

#if !defined(CV_NOARCHIVER)

// --------------------------------------------------------------------------
//
void VGeoColumn::putTo(VArchiver& a)
{
	putTo(a, TRUE);
	putToDone(a);
}

// --------------------------------------------------------------------------
//
void VGeoColumn::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base class data.

	VGeoRowOrColumn::putTo(a, FALSE);

	// Write the revision tag.

	a << VGeoColumnArchiveTag;

	// Write the class specific data.

	;
}

// --------------------------------------------------------------------------
//
void VGeoColumn::getFrom(VArchiver& a, VObject *object, boolean)
{
	getFrom(a);
	getFromDone(a, object);
}

// --------------------------------------------------------------------------
//
void VGeoColumn::getFrom(VArchiver& a)
{
	// Read the base class data.

	VGeoRowOrColumn::getFrom(a);

	// Read and verify the revision tag.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VGeoColumnArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VGeoColumnArchiveTag));
		return;
	}

	// Read the class specific data.

	;
}

#endif
