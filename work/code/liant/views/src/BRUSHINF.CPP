// $Header:   Q:/views/common/vcs/BRUSHINF.CPV   1.9   Mar 24 1997 16:08:18   scottt  $ 

//	brushinf.cpp
//
//	VBrushInfo class implementation [Common]
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
//  09/01/93 dgm	M8 changes; no temps in ?:.
//	09/22/93 pmcm	added archive tag
//	09/23/93 pmcm	added isDefaultBrush to archive data
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	01/17/94 dgm	cfront C++ changes
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	02/02/94 pmcm 	added archive tag check
//  02/09/94 pat 	updated defineArchiveTag use
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	11/11/94 dss	Archiver tags macros.
//	05/19/95 pkt	merged for common source.
//	05/25/95 dgm	fixed VBrushinfoTagRev typo.
//	01/25/96 djs	fixed ::getFrom - fgColor was specified instead of bgColor
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "brush.h"
#include "brushinf.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#endif

defineClass(VBrushInfo, VObject)
defineArchiveRevision(VBrushInfo, 0)

// ---------------------------------------------------------------------------
//
VBrushInfo::VBrushInfo()
{
	init(0);
}

// ---------------------------------------------------------------------------
//
VBrushInfo::VBrushInfo(const VBrush *brush)
{
	init(brush);
}

// ---------------------------------------------------------------------------
//
void VBrushInfo::init(const VBrush *brush)
{
	if (brush == 0) {
		pattern(BackgroundPat);
		foreground(VColor());
		background(VColor());
		isDefault(TRUE);
	}
	else {
		pattern(((VBrush *)brush)->pattern());
		foreground(((VBrush *)brush)->foreground());
		background(((VBrush *)brush)->background());
		isDefault(FALSE);
	}
}

// ---------------------------------------------------------------------------
//
VBrushInfo::~VBrushInfo()
{
	;
}

// ---------------------------------------------------------------------------

// ===========================================================================
#ifndef CV_NOARCHIVER	

//----------------------------------------------------------------------------
//	Put this VBrushInfo object in the given archive.
//
void VBrushInfo::putTo(VArchiver& a)
{
	a << VBrushInfoArchiveTag;

	a << fgColor.getRed();
	a << fgColor.getGreen();
	a << fgColor.getBlue();
	a << bgColor.getRed();
	a << bgColor.getGreen();
	a << bgColor.getBlue();
	a << int(pattern());
	a << isDefaultBrush;
}

//----------------------------------------------------------------------------
//
void VBrushInfo::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VBrushInfoArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VBrushInfoArchiveTag));
		return;
	}

	short RGB[3];

	a >> RGB[0];
	a >> RGB[1];
	a >> RGB[2];
	VColor fore(RGB[0], RGB[1], RGB[2]);
	fgColor = fore;

	a >> RGB[0];
	a >> RGB[1];
	a >> RGB[2];
	VColor back(RGB[0], RGB[1], RGB[2]);
	bgColor = back;

	int fpat;
	a >> fpat;
	pattern((FillPattern) fpat);

	a >> isDefaultBrush;
}

// --------------------------------------------------------------------------
//
void VBrushInfo::getFrom(VArchiver& a,
						 VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER
