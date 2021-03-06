// $Header:   Q:/views/win/vcs/display.cpv   1.7   Mar 04 1997 17:54:58   gregm  $ 

//	display.cpp
//
//	Implementation of VDisplay class
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	05/14/93 pat	added destructor (virtual)
//	07/23/93 pat	removed free; preened.
//	07/23/93 pat	added public getPalette(), setPalette().
//	08/04/93 pat	initialized hOldPalette
//	09/22/93 pat	added archiver support
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	02/02/94 pat	added archive tag check
//	03/15/94 pat	CV_WIN32: updated casts in CreateIC()
//	11/11/94 dss	Archiver tags macros.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "display.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#endif

defineClass(VDisplay, VObject)
defineArchiveRevision(VDisplay, 0)

// ---------------------------------------------------------------------------
//
VDisplay::VDisplay() :	hPalette(0), hDC(0), refCount(0)
{
	;
}

// ---------------------------------------------------------------------------
//	Destroy this VDisplay
//
VDisplay::~VDisplay()
{
	;
}

// ---------------------------------------------------------------------------
//	Place the width and height of this `VDisplay` into '*w' and '*h'
//	respectively. Derived classes must reimplement this method. 
//
void VDisplay::sizeOfImage(int *w, int *h)
{
	*w = *h = 0;
}

// ---------------------------------------------------------------------------
//	Return the number of bits per pixel of this `VDisplay`. This method
//	returns zero. Derived classes must reimplement this method. 
//
int VDisplay::depthOfImage()
{
	return 0;
}

// ---------------------------------------------------------------------------
//	Return the GUI system display context handle for this `VDisplay`.
//	Implemented by derived classes.
//
HDC VDisplay::getHndl()
{
	return 0;
}

// ---------------------------------------------------------------------------
//	Release and possibly destroy the GUI system display context handle
//	for this `VDisplay` object.  Implemented by derived classes. 
//
void VDisplay::ungetHndl()
{
	;
}

// ---------------------------------------------------------------------------
//	Return the width and height of the standard system font
//	int '*w' and '*h' respectively.  
//
void VDisplay::fontSize(int *w, int *h)
{
	HDC hIC;
	TEXTMETRIC tm;

#if defined(CV_WIN32)
	hIC = CreateIC("DISPLAY", (LPCSTR) 0, (LPSTR) 0, (CONST DEVMODE *) 0);
#else
	hIC = CreateIC("DISPLAY", (LPCSTR) 0, (LPCSTR) 0, (const void FAR *) 0);
#endif
	GetTextMetrics(hIC, &tm);
	DeleteDC(hIC);
	*w = tm.tmAveCharWidth;
	*h = tm.tmHeight;
}    

// ---------------------------------------------------------------------------
//	
HPALETTE VDisplay::getPalette()
{
	return hPalette;
}

// ---------------------------------------------------------------------------
//	
void VDisplay::setPalette(HPALETTE hPal)
{
	hPalette = hPal;
}


// ===========================================================================
#ifndef CV_NOARCHIVER	

//----------------------------------------------------------------------------
//
void VDisplay::putTo(VArchiver& a)
{
	a << VDisplayArchiveTag;
}

//----------------------------------------------------------------------------
//
void VDisplay::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VDisplayArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VDisplayArchiveTag));
		return;
	}
}

//----------------------------------------------------------------------------
//
void VDisplay::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------

#endif	//	ARCHIVER
// ===========================================================================
