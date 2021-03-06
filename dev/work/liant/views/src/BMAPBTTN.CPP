// $Header:   Q:/views/win/vcs/bmapbttn.cpv   1.9   Mar 20 1997 18:31:26   gregm  $ 

//	bmapbttn.cpp
//
//	VBitMapButton implementation
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
//	07/18/93 jmd	created
//	07/25/93 jmd	fixed problem in init args
//	07/29/93 jmd	added test for change in setImageIndex
//	07/31/93 jmd	fixed imageLen logic
//	07/31/93 jmd	added prompt
//	08/09/93 jmd	made VFrame const
//	08/12/93 dgm	return VPushButton::erased() from erased()
//					rather than TRUE if there are no bitmaps.
//	08/15/93 jmd	added else to ::paint logic
//	08/15/93 jmd	made changes to frame/seq stuff
//	08/16/93 jmd	added name, archiving
//	08/20/93 jmd	added loadResources()
//  09/01/93 dgm	M8 changes; cast != "" to (const char *)
//  09/23/93 dgm	fixed object-name vs. resource-name confusion
//					in in loadResource().
//  02/27/94 dgm	fix in loadResource() to return FALSE if there
//					is no bitmap name.
//					fix in paint() to erase the area around the bitmap if
//					the bitmap button dimensions are larger than the bitmap.
//  03/04/94 pmcm	CV_WIN32: updated GetWindowLong(), added MoveToEx().
//	03/27/94 pmcm	CV_WIN32: cast appropriately for SendMessage().
//	04/08/94 jmd	added popup prompt support
//	04/19/94 jmd	paint now uses background brush for fillFrame
//	04/19/94 jmd	added isOpen test to loadResource
//	05/02/94 jmd	now the bitmap is centered if the window is too big
//	08/17/94 dgm	Set the internal flag to TRUE in loadResources().
//	10/11/94 dss	Added displayPrompt(), formerly found in notifier.
//			 		BMapButton was relying on the notifier to get to its
//			 		appView parent. Now, it climbs its parent list to find
//			 		its appView parent.
//	10/28/94 jmd	moved tooltip support up to control
//	10/31/94 dgm	Added realize() support.
//	05/23/95 jld	changed paint() to update 'state' data member and choose
//					DownIndex correctly
//	05/23/95 jld	added hilite() and isHilited() to override VPushButton:: members
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#include "bmapbttn.h"
#include "winclass.h"
#include "wintypes.h"
#include "notifier.h"		// for prompt
#include "port.h"
#include "appview.h"

#ifndef CV_NOARCHIVER
#	include "rscarch.h"
#endif

defineClass(VBitMapButton, VPushButton)

const int UpIndex		= 0;
const int DownIndex		= 1;
const int DisabledIndex	= 2;
const int FocusIndex	= 3;

// ---------------------------------------------------------------------------
// Construct a VBitMapButton button with no presentation space.
//
VBitMapButton::VBitMapButton()
{
	internal = FALSE;
	startSeq = 0;

	int	w, h;
	init(&w, &h, 0, 0);
}

// ---------------------------------------------------------------------------
//
VBitMapButton::VBitMapButton(const VFrame &frame, VWindow *pwin, const VStyle& style)
{
	internal = FALSE;
	startSeq = 0;

	int	w, h;
	init(&w, &h, 0, 0);
	(*this)(frame, pwin, style);
}

// ---------------------------------------------------------------------------
//
VBitMapButton::VBitMapButton(const VString& wname, const VFrame &frame, VWindow *pwin, const VStyle& style)
{
	internal = FALSE;
	startSeq = 0;

	int	w, h;
	init(&w, &h, 0, 0);
	(*this)(frame, pwin, style, wname);
}

// ---------------------------------------------------------------------------
//
VBitMapButton::VBitMapButton(	VRatio x,
								VRatio y,
								VBitMap *bm,
								short fcount,
								VWindow *pwin,
								const VStyle& style,
								short seqlen,
								short start)
{
	internal = FALSE;

	seqlen = (seqlen <= 0) ? 1 : seqlen;
	short maxval = fcount / seqlen;
	startSeq = (start < maxval) ? start : 0;

	int	w, h;
	init(&w, &h, bm, fcount, seqlen);

	(*this)(VFrame(x, y, w, h), pwin, style);
}

// ---------------------------------------------------------------------------
//
VBitMapButton::VBitMapButton(	VRatio x,
								VRatio y,
								const VString& bmname,
								short fcount,
								VWindow *pwin,
								const VStyle& style,
								short seqlen,
								short start)
{
	internal = TRUE;

	seqlen = (seqlen <= 0) ? 1 : seqlen;
	short maxval = fcount / seqlen;
	startSeq = (start < maxval) ? start : 0;

	bmapName = bmname;
	VBitMap *bm = new VBitMap(bmname);

	int	w, h;
	init(&w, &h, bm, fcount, seqlen);

	(*this)(VFrame(x, y, w, h), pwin, style);
}

// ---------------------------------------------------------------------------
// Initialize the internal VBitMap objects of this VBitMapButton object
// and return the actual width and height of this button.
//
void VBitMapButton::init(int *w, int *h, VBitMap *bmap,
							short fcount, short seqlen)
{
	if (bmap != 0) {
		bmap->sizeOfImage(w, h);
		imageBitMap = bmap;
		if (fcount < 0) {
			// get data from bitmap
			frameCount = bmap->getFrameCount();
			seqLen = bmap->getSequenceLength();
		}
		else {
			frameCount = fcount;
			seqLen = seqlen;
		}

		frameCount = (frameCount == 0) ? 0 : frameCount;
		seqLen = (seqLen == 0) ? 0 : seqLen;

		frameHeight = *h / frameCount;
		*h = frameHeight;
	}
	else {
		*w = *h = 0;
		imageBitMap = 0;
		frameCount = 0;
		startSeq = 0;
		seqLen = 0;
		frameHeight = 0;
	}
}

// ---------------------------------------------------------------------------
VBitMapButton::~VBitMapButton()
{
	cleanUp();
}

// ---------------------------------------------------------------------------
// Clean up the internal VBitMap object.
//
void VBitMapButton::cleanUp()
{
	if (internal) {
		delete imageBitMap;
	}

	imageBitMap = 0;
	frameCount = 0;
	startSeq = 0;
	frameHeight = 0;
	seqLen = 0;
}

// ---------------------------------------------------------------------------
//
void VBitMapButton::setStartSequence(short start)
{
	short maxval = frameCount / seqLen;
	short i = (start < maxval) ? start : 0;

	if (startSeq != i) {
		startSeq = i;
		update();
	}
}

// ---------------------------------------------------------------------------
// If fcount < 0, use bitmap data
//
void VBitMapButton::setBitMap(VBitMap* bm, short fcount, short seqlen)
{
	if (internal) {
		cleanUp();
	}
	internal = FALSE;
	int w, h;
	init(&w, &h, bm, fcount, seqlen);
	resize(w, h);
}

// ---------------------------------------------------------------------------
//
void VBitMapButton::setBitMap(const VString& bmname, short fcount, short seqlen)
{
	VBitMap *bm;

	if (internal) {
		cleanUp();
	}

	internal = TRUE;
	bmapName = bmname;
	bm = new VBitMap(bmname);

	int w, h;
	init(&w, &h, bm, fcount, seqlen);
	resize(w, h);
}

// ---------------------------------------------------------------------------
// Called by the VNotifier to obtain info on the underlying GUI system
// window represented by this VBitMapButton object.
//
void VBitMapButton::getInfo(VWinInfo& winfo)
{
	VButton::getInfo(winfo);

	if (style.contains(StyleDefaultButton)) {
		isdefault = TRUE;
	}
}

// ---------------------------------------------------------------------------
// Draw the default frame for this VBitMapButton object.
//
void VBitMapButton::drawDefFrame()
{
	int x, y, w, h;

	HDC hdc = getHndl();
	sizeOfImage(&w, &h);
	w--; h--;
	for (x = y = 0; x < 1; x++, y++, w--, h--) {
		MoveToEx(hdc, x, y, 0);

		LineTo(hdc, w, y);
		LineTo(hdc, w, h);
		LineTo(hdc, x, h);
		LineTo(hdc, x, y);
	}
	ungetHndl();
}

// ---------------------------------------------------------------------------
// Called when the visual state of this VBitMapButton 
// changes. Upon call, the state instance variable is set to one or more
// of the following flags or\'ed together: 
// BTTNdraw - draw the entire button; 
// BTTNenabled - the button is enabled else disabled; 
// BTTNselectDelta - the selection status of the button has changed; 
// BTTNselect - ON if button is selected;
// BTTNfocusDelta - the keyboard focus of this button has changed; and
// BTTNfocus - ON if the button has focus.
//
boolean VBitMapButton::paint()
{

	if (imageBitMap == 0) {
		return FALSE;
	}

	int		index;		// index of image within the bitmap

	if (!(state & BTTNenabled)) {
		// disabled
		index = DisabledIndex;
	}
	else if (state & BTTNselect) {
		// down
		index = DownIndex;
	}
	else if (state & BTTNfocus) {
		// focus
		index = FocusIndex;
	}
	else {
		// up
		index = UpIndex;
	}

	state &= ~BTTNselectDelta;
	state &= ~BTTNfocusDelta;
	
	if (index >= seqLen) {
		index = UpIndex;
	}

	index += (startSeq * seqLen);
	index = (index < frameCount) ? index : 0;

	int w, h;
	sizeOfImage(&w, &h);
	VRectangle outer(CornerDim, 0, 0, w, h);

	int	bw, bh;
	imageBitMap->sizeOfImage(&bw, &bh);

	// create the bitmap "inner" rectangle centered in the window
	VRectangle inner(CenterDim, w / 2, h / 2, bw, frameHeight);

	int x, y, d1, d2;
	inner.get(CornerDim, &x, &y, &d1, &d2);

	VBitBltr bitbltr(imageBitMap, this);
	bitbltr.bitbltr(0, frameHeight * index, bw, frameHeight, x, y, CopyRule);

	if ((w > bw) || (h > frameHeight)) {
		// clear around bitmap
		VRectangle outer(CornerDim, 0, 0, w, h);
		VRectangle inner(CornerDim, 0, 0, bw, frameHeight);

		VPort port(this);
		port.open();
		port.useBrush(getBackground());
		port.fillFrame(&outer, &inner);
		port.useBrush(0);
		port.close();
	}

	if (isdefault && (state & BTTNenabled)) {
		drawDefFrame();
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Don't erase background of pic button.
//	This eliminates unsightly "flash" when button is displayed
//
boolean VBitMapButton::erased()
{
	if (imageBitMap == 0) {
		return VPushButton::erased();
	}
	else {
		return TRUE;
	}
}

// ---------------------------------------------------------------------------
// No highlighting for VBitMapButtons 
//
void VBitMapButton::hilite(boolean b)
{
	;   
}

// ---------------------------------------------------------------------------
// Always return FALSE. VBitMapButton object do not highlight.
//
boolean VBitMapButton::isHilited()
{
	return FALSE;
}

// ---------------------------------------------------------------------------
// If b is TRUE, then set this VBitMapButton to the default
// push button style, otherwise set it to the standard style. 
//
void VBitMapButton::setDefault(boolean b)
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

// ---------------------------------------------------------------------------
boolean  VBitMapButton::displayPrompt(VString *msg)
{
	VWindow		*parent;

	for (parent = getParent(); parent ;parent = parent->getParent()) {
		if (parent->isA(VAppViewCls)) {
			((VAppView*)parent)->displayPrompt(msg);
			return TRUE;
		}
	}

	return FALSE;
}
// ---------------------------------------------------------------------------
//
HPALETTE VBitMapButton::getPalette()
{
	if (imageBitMap != 0) {
		return imageBitMap->getPalette();
	}
	else {
		return VPushButton::getPalette();
	}
}

#ifdef CV_MESSAGE_ON_CLICK
// ---------------------------------------------------------------------------
boolean VBitMapButton::mouseDn(int mx, int my)
{
	if (promptString != (const char *)"") {
		displayPrompt(&promptString);
	}

	return VPushButton::mouseDn(mx, my);
}

// ---------------------------------------------------------------------------
boolean VBitMapButton::mouseUp(int mx, int my)
{
	if (promptString != (const char *)"") {
		displayPrompt(0);
	}

	return VPushButton::mouseUp(mx, my);
}

#else	// MESSAGE_ON_ENTER
// ---------------------------------------------------------------------------
boolean VBitMapButton::mouseEnter(int bstat)
{
	// display status bar prompt
	if (promptString != (const char *)"") {
		displayPrompt(&promptString);
	}

	// pass up message for tooltip support
	return VPushButton::mouseEnter(bstat);
}

// ---------------------------------------------------------------------------
boolean VBitMapButton::mouseExit(int bstat)
{
	// reset the status bar prompt
	if (promptString != (const char *)"") {
		displayPrompt(0);
	}

	// pass up message for tooltip support
	return VPushButton::mouseExit(bstat);
}

#endif

// ===========================================================================
#ifndef CV_NOARCHIVER

//---------------------------------------------------------------------------
// 
void VBitMapButton::putTo(VArchiver& a)
{
	VBitMapButton::putTo(a, TRUE);
}

//---------------------------------------------------------------------------
// 
void VBitMapButton::putTo(VArchiver& a, boolean most_derived)
{
	VPushButton::putTo(a, FALSE);			//	base object data

	a << frameCount;			// number of frames in the bitmap
	a << seqLen;				// number of frames per sequence
	a.putObject(&promptString);	// status bar message

	a.putObject(&bmapName);		// name of bitmap

	if (most_derived) {
		VWindow::putChildrenTo(a);			//	children
	}
}

//---------------------------------------------------------------------------
// 
void VBitMapButton::getFrom(VArchiver& a)
{
	VPushButton::getFrom(a); 		// base object data
	a >> frameCount;				// number of frames in the bitmap
	a >> seqLen;					// number of frames per sequence
	a.getObject(&promptString);		// status bar message

	a.getObject(&bmapName);			// name of bitmap
}

//---------------------------------------------------------------------------
// 
void VBitMapButton::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VBitMapButton::getFrom(a);

	if (do_realize) {
		VBitMapButton::realize((VWindow *) parent);
	}

	if (parent != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

//---------------------------------------------------------------------------
//
void VBitMapButton::realize(VWindow *parent)
{
	VPushButton::realize(parent);

	VBitMap *bm = new VBitMap(bmapName);

	if (bm != 0) {
		if (!bm->isValid()) {
			delete bm;
			return;
		}
		int w, h;
		init(&w, &h, bm, frameCount, seqLen);
		resize(w, h);
	}
}

// --------------------------------------------------------------------------
// Load any resources needed by this window or its children
//
boolean VBitMapButton::loadResources(VRscArchiver& a)
{
	if (bmapName == (const char *)"") {
		return FALSE;
	}
	VBitMap *bmap = 0;

	boolean isopen = a.isOpen();

	if (!isopen) {
		if (!a.open(VRscArchiver::ReadOnly)) {
			return FALSE;
		}
	}

	bmap = (VBitMap *)a.getObject(bmapName, 0, VBitMapCls, 0, TRUE);

	if (!isopen) {
		a.close();
	}

	if (bmap == 0) {
		return FALSE;
	}
	setBitMap(bmap, -1);
	internal = TRUE;
	return TRUE;
}

//---------------------------------------------------------------------------

#endif	//	ARCHIVER
// ===========================================================================
