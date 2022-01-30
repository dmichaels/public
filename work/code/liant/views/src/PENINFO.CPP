// $Header:   Q:/views/common/vcs/PENINFO.CPV   1.7   Mar 24 1997 16:12:52   scottt  $ 

//	peninfo.cpp
//
//	VPenInfo class implementation [Common]
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
//	09/01/93 pmcm	original
//	09/01/93 dgm	M8 changes; cast return value of
//	09/22/93 pmcm	added archive tag
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	02/02/94 pmcm	added archive tag check
//  02/09/94 pmcm	updated defineArchiveTag use
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	11/11/94 dss	Archiver tags macros.
//	05/19/95 pkt	merged for common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "pen.h"
#include "peninfo.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VPenInfo, VObject)
defineArchiveRevision(VPenInfo, 0)

// ---------------------------------------------------------------------------
//
VPenInfo::VPenInfo()
{
	init(0);
}

// ---------------------------------------------------------------------------
//
VPenInfo::VPenInfo(const VPen *pen)
{
	init(pen);
}

// ---------------------------------------------------------------------------
//
void VPenInfo::init(const VPen *pen)
{
	if (pen == 0) {
		color(VColor());
		pattern(SolidLine);
		width(0);
	}
	else {
		color(((VPen *)pen)->color());
		pattern(((VPen *)pen)->pattern());
		width(((VPen *)pen)->width());
	}
}

// ---------------------------------------------------------------------------
//
VPenInfo::~VPenInfo()
{
	;
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

//----------------------------------------------------------------------------
//	Put this VPenInfo object in the given archive.
//
void VPenInfo::putTo(VArchiver& a)
{
	a << VPenInfoArchiveTag;
	a << penColor.getRed();
	a << penColor.getGreen();
	a << penColor.getBlue();
	a << int(pattern());
	a << int(width());
}

//----------------------------------------------------------------------------
//
void VPenInfo::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VPenInfoArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VPenInfoArchiveTag));
		return;
	}

	short RGB[3];

	a >> RGB[0];
	a >> RGB[1];
	a >> RGB[2];

	VColor clr(RGB[0], RGB[1], RGB[2]);
	penColor = clr;

	int pat;
	a >> pat;
	pattern((LineType) pat);

	a >> penSize;
}

// ---------------------------------------------------------------------------
//
void VPenInfo::getFrom(VArchiver& a,
					   VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER
