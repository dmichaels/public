// $Header:   Q:/views/common/vcs/FONTINFO.CPV   1.11   Mar 24 1997 16:08:56   scottt  $ 

// 	fontinfo.cpp
//
//	VFontInfo class [Common]
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
// 	Revision History:
// 	-----------------
//	09/01/93 pat	original
//  09/01/93 dgm	M8 changes.
//	09/22/93 pat	added archive tag
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//  02/02/94 pat	added archive tag check
//  02/09/94 pat	updated defineArchiveTag use
//	11/11/94 dss	Archiver tags macros.
//	05/16/95 pkt	preened.
//	09/14/95 dss	Iterating Platform type fix.
//	10/04/95 evc	Added stockobject member:
//	12/22/95 tlf	Updated FONTINFO tag.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "font.h"
#include "fontinfo.h"
#include <stdio.h>

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VFontInfo, VObject)
//
// revision 0 	original
// revision 1	fontinfo archiving moved to VWindow.cpp
//
defineArchiveRevision(VFontInfo, 1)

// ---------------------------------------------------------------------------
//
#if defined(CV_WIN16) || defined (CV_WIN32)
VFontInfo::Platform VFontInfo::this_platform =  VFontInfo::mswin;
#elif defined (PM)
VFontInfo::Platform VFontInfo::this_platform =  VFontInfo::os2;
#else
VFontInfo::Platform VFontInfo::this_platform =  VFontInfo::motif;
#endif

// ---------------------------------------------------------------------------
//
VFontInfo::VFontInfo()
{
	init(0);
}

// ---------------------------------------------------------------------------
//
VFontInfo::VFontInfo(const VFont *font) 
{
	init(font);
}

//-----------------------------------------------------------------------------
VFontInfo::VFontInfo(const VFontInfo &fi)
{
	for (int i= (int) first_platform; i<= (int) last_platform; i++){
		Platform		j = (Platform) i;

		orientation(fi.orientation(j),j);
		pointSize(fi.pointSize(j),j);
		bold(fi.bold(j),j);
		italic(fi.italic(j),j);
		underline(fi.underline(j),j);
		strikeOut(fi.strikeOut(j),j);
		face(fi.face(j),j);
		stockObject(fi.stockObject(j),j);
	}
}
//-----------------------------------------------------------------------------
//		assignment
//
VFontInfo& VFontInfo::operator =(const VFontInfo &fi){
	for (int i= (int) first_platform; i<= (int) last_platform; i++){
		Platform		j = (Platform) i;

		orientation(fi.orientation(j),j);
		pointSize(fi.pointSize(j),j);
		bold(fi.bold(j),j);
		italic(fi.italic(j),j);
		underline(fi.underline(j),j);
		strikeOut(fi.strikeOut(j),j);
		face(fi.face(j),j);
		stockObject(fi.stockObject(j),j);
	}
	return *this;
}
//-----------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void VFontInfo::init(const VFont *font)
{
	if (font == 0) {
		for (int i= (int) first_platform; i<= (int) last_platform; i++){
			Platform		j = (Platform) i;

			//orientation(VFont::Normal);
			pointSize(0, j);
			bold(FALSE, j);
			italic(FALSE, j);
			underline(FALSE, j);
			strikeOut(FALSE, j);
			face("", j);
			stockObject(0,j);
		}
	}
	else {
	// This should require a simple assignment of the VFontInfo structure
	// that hangs off the VFont

		*this = *font->fontinfo;

	}
}

// ---------------------------------------------------------------------------
//
VFontInfo::~VFontInfo()
{
	;
}

// ===========================================================================
#ifndef CV_NOARCHIVER	

// ---------------------------------------------------------------------------
// Put this VFontInfo object in the given archive.
//
void VFontInfo::putTo(VArchiver& a)
{
	a << VFontInfoArchiveTag;

	for (int i= (int) first_platform; i<= (int) last_platform; i++){
		Platform		j = (Platform) i;

		a.putObject(&typeface[j], TRUE);

		a << pts[j];
		a << int(((const VFontInfo *)this)->orientation(j));
		a << isBold[j];
		a << isItalic[j];
		a << isUnderLine[j];
		a << isStrikeOut[j];
		a << stockObjectId[j];
	}
}

// ---------------------------------------------------------------------------
//
void VFontInfo::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;


	if (!archiveTagOk(tag, VFontInfoArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VFontInfoArchiveTag));
		return;
	}
	int rev =getArchiveTagRev(tag);
	if (rev == 0){

		(void)a.getObject(&typeface[this_platform]);
						   	
		int 	ornttn;
		a >> pts[this_platform];
		a >> ornttn;
		a >> isBold[this_platform];
		a >> isItalic[this_platform];
		a >> isUnderLine[this_platform];
		a >> isStrikeOut[this_platform];

		orientation((Orient) ornttn);

	} else {
	// READ the NEW format
		int 	ornttn;

		for (int i= (int) first_platform; i<= (int) last_platform; i++){
			Platform		j = (Platform) i;

			(void)a.getObject(&typeface[j]);
			a >> pts[j];
			a >> ornttn;
			a >> isBold[j];
			a >> isItalic[j];
			a >> isUnderLine[j];
			a >> isStrikeOut[j];
			a >> stockObjectId[j];
			orientation((Orient) ornttn, j);
		}
	}
}

// ---------------------------------------------------------------------------
//
void VFontInfo::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------

#endif	// ARCHIVER
// ===========================================================================

