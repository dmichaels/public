// $Header:   Q:/views/common/vcs/bitmapvw.cpv   1.7   May 30 1997 17:14:30   scottt  $ 

//	bitmapvw.cpp
//
//	VBitMapView class implementation [Common]
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
//	05/11/93 jmd	added getMinSize()
//	05/20/93 jmd	preened, inited bmap(0)
//	07/22/93 pat	added constructor that takes name
//	07/23/93 pat	removed free()
//	07/26/93 pat	added getPalette() virtual override;
//					paint() now realizes the bitmap palette
//	08/09/93 jmd	made VFrame const
//	08/13/93 pat	added VArchiver support
//	08/13/93 jmd	made setBitMap update the window
//	09/14/93 pat	preened
//	04/19/94 jmd	paint now uses background brush for fillFrame
//	05/16/95 pkt	preened.
//	05/29/95 atb	ifdef'ed getPalette() for CV_WINDOWS only
//	09/01/95 dss	VStyle changes.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "bitmapvw.h"
#include "port.h"
#include "bitmap.h"
#include "scrollbr.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VBitMapView, VView)

// ---------------------------------------------------------------------------
//
VBitMapView::VBitMapView() 
			:	bmap(0),
				xOffset(0),
				yOffset(0)
{
	;
}

// ---------------------------------------------------------------------------
// Note: user is responsible for deleting the bitmap
//
VBitMapView::VBitMapView(const VFrame &f, VWindow *pwin, const VStyle& style) 
			:	bmap(0),
				xOffset(0),
				yOffset(0),
				VView(f, pwin, style)
{
	// set up scroll bars
	if (vScroll) {
		vScroll->uponChange(this, methodOf(VBitMapView, vtScrollBar));
	}

	if (hScroll) {
		hScroll->uponChange(this, methodOf(VBitMapView, hzScrollBar));
	}

	updateScrollBars();
}

// ---------------------------------------------------------------------------
// Note: user is responsible for deleting the bitmap
//
VBitMapView::VBitMapView(const VString& name,
                         const VFrame &f, 
						 VWindow *pwin,
                         const VStyle& style)
			:	bmap(0),
				xOffset(0),
				yOffset(0),
				VView(name, f, pwin, style)
{
	// set up scroll bars
	if (vScroll) {
		vScroll->uponChange(this, methodOf(VBitMapView, vtScrollBar));
	}

	if (hScroll) {
		hScroll->uponChange(this, methodOf(VBitMapView, hzScrollBar));
	}

	updateScrollBars();
}

// ---------------------------------------------------------------------------
VBitMapView::~VBitMapView()
{
	;
}

// ---------------------------------------------------------------------------
// Attach a new bitmap the window
//
void VBitMapView::setBitMap(VBitMap *bm)
{
	bmap = bm;
	xOffset = 0;
	yOffset = 0;

	if (!isHidden()) {
		updateScrollBars();
		update();
	}
}

// ---------------------------------------------------------------------------
boolean VBitMapView::resized(int w, int h)
{
	if (bmap != 0) {
		int bw, bh;
		bmap->sizeOfImage(&bw, &bh);

		// make sure as much of image as possible is showing
		if (bw - xOffset < w) {
			xOffset = bw - w;
			xOffset = (xOffset < 0) ? 0 : xOffset;
		}
		if (bh - yOffset < h) {
			yOffset = bh - h;
			yOffset = (yOffset < 0) ? 0 : yOffset;
		}

		updateScrollBars();
	}

	return VView::resized(w, h);
}

// ---------------------------------------------------------------------------
// Paint the bitmap
//
boolean VBitMapView::paint()
{
	VPort port(this);
	port.open();
	port.useBrush(getBackground());

	// calculate size of region surrounding bitmap
	int	w, h;
	sizeOfImage(&w, &h);
	VRectangle outer(CornerDim, 0, 0, w, h);

	if (bmap == 0) {
		port.fillRegion(&outer, FALSE);
	}
	else {
		bmap->sizeOfImage(&w, &h);
		VRectangle inner(CornerDim, -xOffset, -yOffset, w, h);

		port.drawBitMap(bmap, -xOffset, -yOffset);
		port.fillFrame(&outer, &inner);
	}

	port.useBrush(0);
	port.close();

	return TRUE;
}

// ---------------------------------------------------------------------------
// Intercept automatic background erasure.
//
boolean VBitMapView::erased()
{
	return TRUE;
}

// ---------------------------------------------------------------------------
// Scroll the bitmapview the specified amount (in pixels)
//
void VBitMapView::scrollBitMap(int dx, int dy)
{
	// adjust offsets
	dx = (-dx > xOffset) ? -xOffset : dx;
	dy = (-dy > yOffset) ? -yOffset : dy;
	xOffset += dx;
	yOffset += dy;

	scroll(-dx, -dy);
	updateScrollBars();
}

// ---------------------------------------------------------------------------
// update the thumb positions and ranges of the scroll bars
//
void VBitMapView::updateScrollBars()
{
	if (bmap == 0) {
		if (vScroll) {
			vScroll->hide();
		}
		if (hScroll) {
			hScroll->hide();
		}
		return;
	}

	int ww, wh, bw, bh;
	sizeOfImage(&ww, &wh);
	bmap->sizeOfImage(&bw, &bh);

	// update scroll bars
	if (vScroll != 0) {
		if (bh - wh < 0) {
			vScroll->hide();
		}
		else {
			vScroll->setValues(bh - wh, 0, 20, wh);
			vScroll->setPos(yOffset);
			vScroll->show();
		}
	}

	if (hScroll != 0) {
		if (bw - ww < 0) {
			hScroll->hide();
		}
		else {
			hScroll->setValues(bw - ww, 0, 20, ww);
			hScroll->setPos(xOffset);
			hScroll->show();
		}
	}
}

// ---------------------------------------------------------------------------
// the vertical scrollbar has changed position
//
boolean VBitMapView::vtScrollBar(int i)
{
	if (vScroll != 0) {
		if (vScroll->getMax() <= 0) {
			i = 0;
		}
		scrollBitMap(0, i - yOffset);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// the horizontal scrollbar has changed position
//
boolean VBitMapView::hzScrollBar(int i)
{
	if (hScroll != 0) {
		if (hScroll->getMax() <= 0) {
			i = 0;
		}
		scrollBitMap(i - xOffset, 0);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
void VBitMapView::getMinSize(short &w, short &h)
{
	int bw = 1;
	int bh = 1;

	if (bmap != 0) {
		bmap->sizeOfImage(&bw, &bh);
	}

	w = (short) bw;
	h = (short) bh;
}

// ---------------------------------------------------------------------------
//
#ifdef CV_WINDOWS
HPALETTE VBitMapView::getPalette()
{
	return bmap == 0 ? (HPALETTE) 0 : bmap->getPalette();
}
#endif

// ===========================================================================
#ifndef CV_NOARCHIVER

//----------------------------------------------------------------------------
// VBitMapView::putTo
//
void VBitMapView::putTo (VArchiver& a)
{
	VBitMapView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
// VBitMapView::putTo
//
void VBitMapView::putTo (VArchiver& a, boolean most_derived)
{
	VView::putTo (a, FALSE);			// base object data

	a << xOffset;
	a << yOffset;

	if (most_derived) {
		VWindow::putChildrenTo(a);		// children
	}
}

// ---------------------------------------------------------------------------
// VBitMapView::getFrom
//
void VBitMapView::getFrom(VArchiver& a)
{
	VView::getFrom(a);					// base object data

	a >> xOffset;
	a >> yOffset;
}

// ---------------------------------------------------------------------------
// VBitMapView::getFrom
//
void VBitMapView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VBitMapView::getFrom(a);

	if (do_realize) {
		VBitMapView::realize((VWindow *)parent);
	}

	if (parent != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
// VBitMapView::realize
//
void VBitMapView::realize(VWindow *parent)
{
	VView::realize(parent);
	updateScrollBars();
}

// ---------------------------------------------------------------------------

#endif	// ARCHIVER
// ===========================================================================


