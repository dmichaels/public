// $Header:   Q:/views/common/vcs/ARCHVRTG.CPV   1.5   Mar 24 1997 16:07:58   scottt  $ 

//	archvrtg.cpp
//
//	VArchiver Tag Helpers implementation [Common]
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
//	09/22/93 pat	original.
//	09/23/93 pat	added VSashTag
//	01/13/94 dgm	merged and preened
//	02/02/94 pat	inlined get*() stuff; added archiveTagAbortCheck()
//	02/09/94 pat	removed class specific tag values in favor of magic number
//	03/06/95 jld	added #ifndef CV_NOARCHIVER wrapper
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef	CV_NOARCHIVER

#include "archiver.h"
#include "archvrtg.h"

// ---------------------------------------------------------------------------
//
VArchiver::AbortReason archiveTagAbortCheck(	const long archTag, 
												const long objTag)
{
	if ((archTag & 0x0F0000) == 0) {
		;	//	pat--3.0 tags, ignore.
	}
	else if ((archTag & 0x0FFF00) != 0x0ABC00) {
		return VArchiver::BadTag;
	}

	if (getArchiveTagRev(archTag) > getArchiveTagRev(objTag)) {
		return VArchiver::NewArchiveObject;
	}

	return VArchiver::NoAbort;
}

#endif	// CV_NOARCHIVER
