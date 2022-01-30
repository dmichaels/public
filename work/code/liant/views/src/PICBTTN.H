// $Header:   Q:/views/win/vcs/picbttn.h_v   1.6   Mar 04 1997 16:32:46   gregm  $ 

//	picbttn.h
//
//	VPicButton interface
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
//	09/10/92 jmd	added erased()
//	09/17/92 jmd	preened
//	09/19/92 jmd	changed constructor to take VRatio, not VRatio&
//	07/08/93 jmd	changed constructor to take const char *
//	08/09/93 jmd	made VFrame const, removed free
//	09/23/93 pat	added constructor that takes window name
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#ifndef picbttn_h
#define picbttn_h
#include "pushbttn.h"
#include "bitmap.h"

// ----------------------------------------------------------------------------
//	The VPicButton class is designed to support graphical, picture based
//	buttons in an application. As a subclass of VPushButton it behaves in every
//	way like a VPushButton object. Using a VPicButton object requires
//	an array of up to four VBitMap objects to represent the various visual
//	states of the button.  The first two VBitMap objects are required and they
//	represent the 'unpressed' and 'pressed' states of the button respectively.
//	The optional third VBitMap object represents the disabled button image and
//	the optional fourth VBitMap object represents the image of the button when
//	it has focus.  If the disabled or focus button VBitMaps are not used,
//	then NULL pointers must be provided. The four VBitMap objects can be
//	provided directly to a VPicButton object or they can be provided indirectly
//	by simply making their resource names available. In the first case, the
//	user of the VPicButton is responsible for destroying the VBitMap objects,
//	while in the later case the VPicButton will destroy the VBitMap objects
//	it creates from the resource names.


CLASS VPicButton : public VPushButton {
protected:
	void cleanUp();

	VBitMap *bdn;

	VBitMap *bup;

	VBitMap *bdisable;

	VBitMap *bfocus;

	boolean internal;
	void	init(int*, int*, VBitMap **);
	void	getInfo(VWinInfo &);
	void	drawDefFrame();

public:

	VPicButton(		const VFrame& frame, 
					VWindow *pWin, 
					const VStyle& style = StyleDefault);

	VPicButton(		const VString& wName, 
					const VFrame& frame,
					VWindow *pWin, 
					const VStyle& style = StyleDefault);

	VPicButton(VRatio, VRatio, const char**, VWindow *, const VStyle& style = StyleDefault);
	VPicButton(VRatio, VRatio, VBitMap **, VWindow *, const VStyle& style = StyleDefault);
	VPicButton(unsigned, VWindow *);
	VPicButton();
	~VPicButton();

	void	setBitMaps(char**);
	void	setBitMaps(VBitMap **);
	void	setDefault(boolean);

	boolean paint();
	boolean erased();
	VClass *iam();
};

extern VClass *VPicButtonCls;
#endif /* picbttn_h */
