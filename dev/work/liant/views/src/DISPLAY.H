// $Header:   Q:/views/win/vcs/display.h_v   1.8   Mar 04 1997 16:32:16   gregm  $ 

//	display.h
//
//	VDisplay class interface
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
//	05/14/93 pmcm	added destructor (virtual)
//	05/20/93 jmd	made fontSize() virtual, preened
//	07/23/93 pmcm	removed free
//	07/23/93 pmcm	added public getPalette(), setPalette(), and
//					private hPalette.
//	08/04/93 pmcm	added protected getOldPalette(), setOldPalette(), and
//					private hOldPalette
//	09/22/93 pmcm	added archiver support
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	03/08/94 pmcm	STRICT:included winclass.h before wintypes.h
//					to prevent multiple inclusion of windows.h
// ---------------------------------------------------------------------------

#ifndef VDISPLAY_H
#define VDISPLAY_H

#include "object.h"
#include "winclass.h"	//	for HPALETTE
#include "wintypes.h"

extern VClass *VDisplayCls;

CLASS VBitBltr;
CLASS VBitMap;
CLASS VPort;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	Introduction
//
//	The VDisplay class is an abstract class defining a array
//	of pixels which may be written to by the VBitBltr or VPort classes.
//

CLASS VDisplay : public VObject {

public:

	//	Constructor/Destructor

						VDisplay();
						~VDisplay();

	//	Class identity

	VClass 		   	   *iam();	

	//	Attribute queries

	virtual void	   	sizeOfImage(int *w, int *h);
	virtual int		   	depthOfImage();
	virtual void	   	fontSize(int *fw, int *fh);

	//	GUI Device context handle

	virtual HDC			getHndl();
	virtual void		ungetHndl();

	//	GUI Palette (non-portable, specific to MS-Windows)

	virtual HPALETTE	getPalette();			
	virtual void		setPalette(HPALETTE hPal);

#ifndef CV_NOARCHIVER
	void	   			putTo(VArchiver& a);
	void				getFrom(VArchiver& a);
	virtual void	 	getFrom(VArchiver& a, VObject *data,
								boolean do_realize = TRUE);
#endif

protected:

	friend	VBitBltr;
	friend	VPort;
	friend	VBitMap;

protected:
	HDC				hDC;
	HPALETTE 		hPalette;
	ushort			refCount;

};

#endif // VDISPLAY_H
