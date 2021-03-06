// $Header:   Q:/views/win/vcs/picbttn.cpv   1.6   Mar 04 1997 17:55:30   gregm  $ 

//	picbttn.cpp
//
//	VPicButton implementation
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
//	09/10/92 jmd	added erased(), made the def frame box thinner
//	09/19/92 jmd	changed constructor to take VRatio, not VRatio&
//	07/08/93 jmd	changed constructor to take const char *
//	08/09/93 jmd	made VFrame const, removed free
//	08/11/93 dgm	return VPushButton::erased() from erased()
//					rather than TRUE if there are no bitmaps.
//	09/23/93 pat	added constructor that takes window name
//  03/04/94 pat	CV_WIN32: updated GetWindowLong(); added MoveToEx().
//	03/08/94 pat	STRICT: changed NULL to 0.
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#include "picbttn.h"
#include "winclass.h"
#include "wintypes.h"

defineClass(VPicButton, VPushButton)

// ----------------------------------------------------------------------------
//	Construct a VPicButton button with no presentation space.
//
VPicButton::VPicButton()
{
	internal = FALSE;
	init(0, 0, 0);
}
VPicButton::~VPicButton()
{
	cleanUp();
}

// ----------------------------------------------------------------------------
//	Called by the VNotifier to obtain info on the underlying GUI system
//	window represented by this VPicButton object.
//
void VPicButton::getInfo(VWinInfo &picinfo)
{
	VButton::getInfo(picinfo);
	if (style.contains(StyleDefaultButton))  {
		isdefault = TRUE;
	}
}

// ----------------------------------------------------------------------------
//	Called when the visual state of this VPicButton 
//	changes. Upon call, the 'state' instance variable is set to one or more
//	of the following flags or\'ed together: 
//		BTTNdraw - draw the entire button; 
//		BTTNenabled - the button is enabled else disabled; 
//		BTTNselectDelta - the selection status of the button has changed; 
//		BTTNselect - ON if button is selected;
//		BTTNfocusDelta - the keyboard focus of this button has changed; and
//		BTTNfocus - ON if the button has focus.
//
boolean VPicButton::paint()
{
	HDC srcDC;
	VBitMap *bm;
	int w, h;

	if (!bup || !bdn) {
		return(FALSE);
	}
	sizeOfImage(&w, &h);

	if (!(state & BTTNenabled) && bdisable) {
		srcDC = bdisable->getHndl();
		BitBlt(hDC, 0, 0, w, h, srcDC, 0, 0, SRCCOPY);
		bdisable->ungetHndl();
		return(TRUE);
	}

	if (((state & BTTNdraw) && (state & BTTNselect)) || ((state & BTTNselect) && (state & BTTNselectDelta))) {
		bm = bdn;
	}
	else {
		if (state & BTTNfocus && bfocus) {
			bm = bfocus;
		}
		else {
			bm = bup;
		}
	}

	srcDC = bm->getHndl();
	BitBlt(hDC, 0, 0, w, h, srcDC, 0, 0, SRCCOPY);
	bm->ungetHndl();
	if (!bfocus && isdefault) {
		drawDefFrame();
	}
	return(TRUE);
}

boolean VPicButton::erased()
{
	// Eliminate flash by not erasing background
	if ((bup == 0) || (bdn == 0)) {
		return VPushButton::erased();
	}
	else {
		return TRUE;
	}
}

// ----------------------------------------------------------------------------
//	Initialize the internal VBitMap objects of this VPicButton object
//	and return the actual width and height of this button.
//
void VPicButton::init(int *w, int *h, VBitMap **bitmaps)
{
	if (bitmaps) {
		bup = bitmaps[0];
		bup->sizeOfImage(w, h);
		bdn = bitmaps[1];
		bdisable = bitmaps[2];
		bfocus = bitmaps[3];
	}
	else {
		bup = bdn = bdisable = bfocus = 0;
	}
}

// ----------------------------------------------------------------------------
//	Construct a VPicButton at location ('x','y') with parent 'win', style 
//	'style' and an array of upto four VBitMap objects.  The 'style' may be set 
//	to StyleDefaultButton or StyleNoAutoDefault. The first two VBitMap
//	objects, 'bitmaps[0]' and 'bitmaps[1]', are required and they represent the
//	'unpressed' and 'pressed' states of the button, respectively. The optional
//	third VBitMap object, 'bitmaps[2]', represents the disabled button image
//	and the optional fourth VBitMap object, 'bitmaps[3]', represents the image
//	of the button when it has focus.  If the disabled or focus button VBitMaps
//	are not used, then NULL pointers must be provided.  NOTE: the VPicButton
//	does NOT destroy these VBitMap objects when it is destroyed.
//
VPicButton::VPicButton(VRatio x, VRatio y, VBitMap **bitmaps, VWindow *win, 
					   const VStyle& style) 
{
	int w, h;

	internal = FALSE;
	init(&w, &h, bitmaps);
	(*this)(VFrame(x, y, w, h), win, style);
}

// ----------------------------------------------------------------------------
//	If 'b' is TRUE, then set this VPicButton to the default
//	push button style, otherwise set it to the standard style. 
//
void VPicButton::setDefault(boolean b)
{
	if (!isdefault || !b) {
		isdefault = b;
		if (b && fromRsc) {
#ifdef CV_WIN32
			SendMessage(GetParent(hWnd), DM_SETDEFID, 
						(WPARAM) GetWindowLong(hWnd, GWL_ID), (LPARAM) 0);
#else
			SendMessage(GetParent(hWnd), DM_SETDEFID, 
						(WPARAM) GetWindowWord(hWnd, GWW_ID), (LPARAM) 0);
#endif
		}
		update();
	}
}

// ----------------------------------------------------------------------------
//	Draw the 'default' frame for this VPicButton object.
//
void VPicButton::drawDefFrame()
{
	int x, y, w, h;

	HDC hdc = getHndl();
	sizeOfImage(&w, &h);
	w--; h--;
	for (x = y = 0; x < 1; x++ , y++ , w-- , h--) {
		MoveToEx(hdc, x, y, 0);
		LineTo(hdc, w, y);
		LineTo(hdc, w, h);
		LineTo(hdc, x, h);
		LineTo(hdc, x, y);
	}
	ungetHndl();
}

// ----------------------------------------------------------------------------
//	Construct a VPicButton at location ('x','y') with parent 'win', style 
//	'style' and an array of upto four C strings representing the names of VBitmap
//	resources.  The 'style' may be set to StyleDefaultButton or StyleNoAutoDefault.
//	The first two names, 'bmNames[0]' and 'bmNames[1]', are required and they are
//	the names of the bitmap resources that represent the 'unpressed' and 'pressed'
//	states of the button, respectively. The optional third string, 'bmNames[2]'
//	is the name of the bitmap resource that represents the disabled button image
//	and the optional fourth string, 'bmNames[3]', is the name of the bitmap
//	resource that represents the image of the button when it has focus.  If the
//	disabled or focus images are not used, then NULL string pointers must be
//	provided for those entries.  NOTE: When the VPicButton is destroyed, it will
//	destroy the VBitMap objects created from the passed resource names.
//
VPicButton::VPicButton(VRatio x, VRatio y, const char **bmNames, VWindow *win,
							  const VStyle& style) 
{
	int w, h, i;
	VBitMap *bm[4];

	internal = TRUE;
	for (i = 0; i < 4; i++) {
		if (bmNames[i]) {
			bm[i] = new VBitMap(bmNames[i]);
		}
		else {
			bm[i] = 0;
		}
	}
	init(&w, &h, bm);
	(*this)(VFrame(x, y, w, h), win, style);
}

// ---------------------------------------------------------------------------
//	Construct a VPicButton object with VFrame 'frame', parent window
//	'pwin' and style 'style'.  The 'style' parameter is optional and it may be
//	set to StyleDefaultButton or StyleNoAutoDefault.  The bitmaps for this
//	button can be provided by one of the setBitmaps() methods.
//
VPicButton::VPicButton(const VFrame& frame, VWindow *pwin, const VStyle& style)
{
	internal = FALSE;
	init(0, 0, 0);
	(*this)(frame, pwin, style);
}

// ---------------------------------------------------------------------------
//	Construct a named VPicButton object with VFrame 'frame', parent window
//	'pwin' and style 'style'.  The 'style' parameter is optional and it may be
//	set to StyleDefaultButton or StyleNoAutoDefault.  The bitmaps for this
//	button can be provided by one of the setBitmaps() methods.
//
VPicButton::VPicButton(const VString& wName, const VFrame& frame, 
						VWindow *pwin, const VStyle& style)
{
	internal = FALSE;
	init(0, 0, 0);
	(*this)(frame, pwin, style, wName);
}


// ----------------------------------------------------------------------------
//	Set the VBitMap objects for this VPicButton. The first two VBitMap objects, 'bitmaps[0]' and
//	'bitmap[1]', are required and they represent the 'unpressed' and 'pressed' 
//	states of the button, respectively. The optional third VBitMap object, 
//	'bitmaps[2]', represents the disabled button image and the optional fourth
//	 VBitMap object, 'bitmaps[3]', represents the image of the button when
//	it has focus.  If the disabled or focus button VBitMaps are not used, then
//	NULL pointers must be provided.  NOTE: the VPicButton does NOT destroy
//	these VBitMap objects when it is destroyed.
//
void VPicButton::setBitMaps(VBitMap **bitmaps)
{
	int w, h;

	if (internal) {
		cleanUp();
	}
	internal = FALSE;
	init(&w, &h, bitmaps);
	resize(w, h);
}

// ----------------------------------------------------------------------------
//	Set the VBitMap objects for this VPicButton from the resource 
//	names 'bmNames'. The first two names, 'bmNames[0]' and 'bmNames[1]', are 
//	required and they are the names of the bitmap resources that represent the 
//	'unpressed' and 'pressed' states of the button, respectively. The optional
//	third string, 'bmNames[2]', is the name of the bitmap resource that 
//	represents the disabled button image and the optional fourth string, 
//	'bmNames[3]', is the name of the bitmap resource that represents the image 
//	of the button when it has focus.  If the disabled or focus images are not 
//	used, then NULL string pointers must be provided for those entries.  
//	NOTE: When the VPicButton is destroyed, it will destroy the VBitMap objects 
//	created from the passed resource names.
//
void VPicButton::setBitMaps(char **bmNames)
{
	int w, h, i;
	VBitMap *bm[4];

	if (internal) {
		cleanUp();
	}
	internal = TRUE;
	for (i = 0; i < 4; i++) {
		if (bmNames[i]) {
			bm[i] = new VBitMap(bmNames[i]);
		}
		else {
			bm[i] = 0;
		}
	}
	init(&w, &h, bm);
	resize(w, h);
}

// ----------------------------------------------------------------------------
//	Construct a VPicButton from the resource control-id 'cntrl' of
//	the resource associated with 'pWin'. This is a non-portable GUI system method.
//
VPicButton::VPicButton(unsigned cntrl, VWindow *pWin)
{
	internal = FALSE;
	getRscWindow(cntrl, pWin);
}

// ----------------------------------------------------------------------------
//	Clean up the internal VBitMap objects.
//
void VPicButton::cleanUp()
{
	if (internal) {
		if (bup) {
			delete bup;
		}
		if (bdn) {
			delete bdn;
		}
		if (bfocus) {
			delete bfocus;
		}
		if (bdisable) {
			delete bdisable;
		}
	}
}
