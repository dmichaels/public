//	imbox.cpp
//
//	VImageBox implementation
//
//  Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of this    |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	08/24/96 dgm	Original.
//	01/13/97 djs	#ifdef'd ::getPalette for OS/2 - TEMP
// --------------------------------------------------------------------------

#include "imbox.h"
#include "image.h"
#include "rect.h"
#include "clrscm.h"
#include "color.h"
#include "brush.h"
#include "pen.h"
#include "bitmap.h"
#include "port.h"
#include "str.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "rscarch.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VImageBox, VControl)
defineArchiveRevision(VImageBox, 0)

// --------------------------------------------------------------------------
//
#define ucharRangeCheck(n)	{ if ((n) < 0) (n) = 0; \
							  else if ((n) > 255) { (n) = 255; } }

// --------------------------------------------------------------------------
//
static unsigned long	objectCount		= 0;
static VPort		   *thePort			= 0;
static VRectangle	   *theRectangle	= 0;
static VString		   *theString		= 0;

// --------------------------------------------------------------------------
//
VImageBox::VImageBox()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VImageBox::VImageBox(VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VImageBox::VImageBox(const VFrame& frame, VWindow *parentwin,
					 const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VImageBox::VImageBox(VWindow *parentwin, VImage *image, int index,
					 const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
	setImage(image, index);
}

// --------------------------------------------------------------------------
//
VImageBox::VImageBox(const VString& name,
					 VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VImageBox::VImageBox(const VString& name,
					 const VFrame& frame, VWindow *parentwin,
					 const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VImageBox::VImageBox(const VString& name,
					 VWindow *parentwin, VImage *image, int index,
					 const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
	setImage(image, index);
}

// --------------------------------------------------------------------------
//
void VImageBox::operator()(VWindow *parentwin)
{
	(*this)(VFrame(), parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VImageBox::operator()(VWindow *parentwin, const VStyle& style)
{
	(*this)(VFrame(), parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VImageBox::operator()(VWindow *parentwin,
						   const VStyle& style, const VString& name)
{
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VImageBox::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VImageBox::operator()(const VFrame& frame, VWindow *parentwin,
						   const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VImageBox::operator()(const VFrame& frame, VWindow *parentwin,
						   const VStyle& style, const VString& name)
{
	VControl::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VImageBox::~VImageBox()
{
	destruct();
	if (--objectCount == 0) {
		destructStatic();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::initialize()
{
	if (objectCount++ == 0) {
		constructStatic();
	}

	currentWidth				= 0;
	currentHeight				= 0;
	theImage					= 0;
	imageMajorIndex				= 0;
	imageMinorIndex				= 0;
	textWidth					= 0;
	textHeight					= 0;
	drawMode					= 0;
	currentlyDisabled			= 0;
	currentlyDisplayingImage	= 1;
	currentlyDisplayingText		= 0;
	currentlyShowingBorder		= 0;
	borderStyle					= NoBorder;
	borderThickness				= 0;
	leftMargin					= 0;
	rightMargin					= 0;
	topMargin					= 0;
	bottomMargin				= 0;
	textPlacement				= Below;
	textImageGap				= 2;
	textImageDisplacementX		= 0;
	textImageDisplacementY		= 0;
	immediatePaintUpdate		= 1;
	amUsingTheImageDrawMode		= 0;
	imageIsMine					= 0;
	receivedPaint				= 0;
}

// --------------------------------------------------------------------------
//
void VImageBox::construct()
{
	setAutoErased(FALSE);
	setTabStop(FALSE);

#if defined(CV_WINDOWS)
	//
	// Allegris Foundation bug (MS/Windows) workaround! We don't get
	// in initial resized() message; we could fix this by changing
	// the notifier to check for WM_SHOWWINDOW and to send a resized()
	// if the window has not previously been sent a resized() message.
	//
	int w, h;
	sizeOfImage(&w, &h);
	currentWidth = w;
	currentHeight = h;
#endif
}

// --------------------------------------------------------------------------
//
void VImageBox::destruct()
{
	if (imageIsMine) {
		imageIsMine = 0;
		if (theImage != 0) {
			delete theImage;
			theImage = 0;
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::constructStatic()
{
	thePort			= new VPort;
	theRectangle	= new VRectangle;
	theString		= new VString;
}

// --------------------------------------------------------------------------
//
void VImageBox::destructStatic()
{
	delete thePort;			thePort			= 0;
	delete theRectangle;	theRectangle	= 0;
	delete theString;		theString		= 0;
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(VImage *image, int index)
{
	if (image == theImage) {
		return TRUE;
	}
	if (imageIsMine) {
		imageIsMine = 0;
		if (theImage != 0) {
			delete theImage;
		}
	}
	theImage = image;
	imageMajorIndex = index;
	if (immediateGeometryUpdate) {
		updateGeometry(-1);
	}
	if (immediatePaintUpdate) {
		repaintBackgroundAndContents();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VString&
								  native_resource_or_bitmap_file_name,
							int major_count, int minor_count, int index)
{
	VImage *image = new VImage(native_resource_or_bitmap_file_name,
							   major_count, minor_count);
	if (image == 0) { return FALSE; }
	imageIsMine = 1;
	return setImage(image, index);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const char *
								  native_resource_or_bitmap_file_name,
							int major_count, int minor_count, int index)
{
	VImage *image = new VImage(native_resource_or_bitmap_file_name,
							   major_count, minor_count);
	if (image == 0) { return FALSE; }
	imageIsMine = 1;
	return setImage(image, index);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VPathString& bitmap_file_name,
							int major_count, int minor_count, int index)
{
	VImage *image =
			new VImage(bitmap_file_name, major_count, minor_count);
	if (image == 0) { return FALSE; }
	imageIsMine = 1;
	return setImage(image, index);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VString& vrf_bitmap_name,
							const VPathString& vrf_file_name,
							int major_count, int minor_count, int index)
{
	VImage *image =
			new VImage(vrf_bitmap_name, vrf_file_name,
					   major_count, minor_count);
	if (image == 0) { return FALSE; }
	imageIsMine = 1;
	return setImage(image, index);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VString& vrf_bitmap_name,
							const VString& vrf_file_name,
							int major_count, int minor_count, int index)
{
	return setImage(vrf_bitmap_name, VPathString(vrf_file_name));
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VString& vrf_bitmap_name,
							const char *vrf_file_name,
							int major_count, int minor_count, int index)
{
	return setImage(vrf_bitmap_name, VPathString(vrf_file_name));
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const char *vrf_bitmap_name,
							const VPathString& vrf_file_name,
							int major_count, int minor_count, int index)
{
	return setImage(VString(vrf_bitmap_name), vrf_file_name);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const char *vrf_bitmap_name,
							const VString& vrf_file_name,
							int major_count, int minor_count, int index)
{
	return setImage(VString(vrf_bitmap_name), VPathString(vrf_file_name));
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const char *vrf_bitmap_name,
							const char *vrf_file_name,
							int major_count, int minor_count, int index)
{
	return setImage(VString(vrf_bitmap_name), VPathString(vrf_file_name));
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VBitMap *bitmap,
							int major_count, int minor_count, int index)
{
	VImage *image = new VImage(bitmap, major_count, minor_count);
	if (image == 0) {
		return FALSE;
	}
	imageIsMine = 1;
	return setImage(image, index);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(const VBitMap& bitmap,
							int major_count, int minor_count, int index)
{
	VImage *image = new VImage(bitmap, major_count, minor_count);
	if (image == 0) {
		return FALSE;
	}
	imageIsMine = 1;
	return setImage(image, index);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::setImage(int image)
{
	if (image == 0) {
		if (imageIsMine) {
			imageIsMine = 0;
			if (theImage != 0) {
				delete theImage;
			}
		}
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setImageIndex(int major_index, int minor_index)
{
	if (theImage != 0) {
		if (major_index >= theImage->getItemMajorCount()) {
			major_index = theImage->getItemMajorCount() - 1;
		}
		if (minor_index >= theImage->getItemMinorCount()) {
			minor_index = theImage->getItemMinorCount() - 1;
		}
	}
	if ((imageMajorIndex != major_index) ||
		(imageMinorIndex != minor_index)) {
		imageMajorIndex = major_index;
		imageMinorIndex = minor_index;
		if (immediatePaintUpdate) {
			repaintBackgroundAndContents();
		}
	}
}

// --------------------------------------------------------------------------
//
int VImageBox::getImageMajorCount() const
{
	if (theImage != 0) {
		return theImage->getItemMajorCount();
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
int VImageBox::getImageMinorCount() const
{
	if (theImage != 0) {
		return theImage->getItemMinorCount();
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setText(const VString& text)
{
	VControl::putText(text);
	calculateTextSize();
	if (!currentlyDisplayingText) {
		currentlyDisplayingText = 1;
		if (immediateGeometryUpdate) {
			updateGeometry(-1);
		}
		if (immediatePaintUpdate) {
			repaint();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::displayImage(boolean b)
{
	if (b) {
		if (!currentlyDisplayingImage) {
			currentlyDisplayingImage = 1;
			if (immediateGeometryUpdate) {
				updateGeometry(-1);
			}
			if (immediatePaintUpdate) {
				repaint();
			}
		}
	}
	else if (currentlyDisplayingImage) {
		currentlyDisplayingImage = 0;
		if (immediateGeometryUpdate) {
			updateGeometry(-1);
		}
		if (immediatePaintUpdate) {
			repaint();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::displayText(boolean b)
{
	if (b) {
		if (!currentlyDisplayingText) {
			currentlyDisplayingText = 1;
			if (immediateGeometryUpdate) {
				updateGeometry(-1);
			}
			if (immediatePaintUpdate) {
				repaint();
			}
		}
	}
	else if (currentlyDisplayingText) {
		currentlyDisplayingText = 0;
		if (immediateGeometryUpdate) {
			updateGeometry(-1);
		}
		if (immediatePaintUpdate) {
			repaint();
		}
	}
}

// --------------------------------------------------------------------------
// Whether the border is shown or not does *not* effect the size of the
// button, only whether or not the button *has* a border; it has a border
// if the border style is *not* NoBorder.
//
void VImageBox::showBorder(boolean b)
{
	if (b) {
		if (currentlyShowingBorder) {
			return;
		}
		currentlyShowingBorder = 1;
		if (immediatePaintUpdate) {
			repaintBorder();
		}
	}
	else {
		if (!currentlyShowingBorder) {
			return;
		}
		currentlyShowingBorder = 0;
		if (immediatePaintUpdate) {
			thePort->openOn(this);
			paintNullBorder(*thePort);
			thePort->close();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setBorderStyle(int style)
{
	if (style != borderStyle) {
		int thickness;
		switch (style) {
		case NoBorder:
			thickness = 0;
			currentlyShowingBorder = 0;
			break;
		case SunkenBorder:
			thickness = 1;
			currentlyShowingBorder = 1;
			break;
		case SunkenThickBorder:
			thickness = 2;
			currentlyShowingBorder = 1;
			break;
		case RaisedBorder:
			thickness = 1;
			currentlyShowingBorder = 1;
			break;
		case RaisedThickBorder:
			thickness = 2;
			currentlyShowingBorder = 1;
			break;
		case EtchedInBorder:
			thickness = 2;
			currentlyShowingBorder = 1;
			break;
		case EtchedOutBorder:
			thickness = 2;
			currentlyShowingBorder = 1;
			break;
		case FlatBorder:
			thickness = 1;
			currentlyShowingBorder = 1;
			break;
		case FlatThickBorder:
			thickness = 2;
			currentlyShowingBorder = 1;
			break;
		case FlatDoubleBorder:
			thickness = 3;
			currentlyShowingBorder = 1;
			break;
		default:
			return;
		}
		borderStyle = style;
		if (thickness < borderThickness) {
			borderThickness = thickness;
			if (immediateGeometryUpdate) {
				updateGeometry(-1);
			}
			if (immediatePaintUpdate) {
				repaint();
			}
		}
		else if (immediatePaintUpdate) {
			borderThickness = thickness;
			repaint();
		}
		else {
			borderThickness = thickness;
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setMargins(int lrtb)
{
	ucharRangeCheck(lrtb);
	if ((lrtb != leftMargin) || (lrtb != rightMargin) ||
		(lrtb != topMargin)  || (lrtb != bottomMargin)) {
		leftMargin   = lrtb;
		rightMargin  = lrtb;
		topMargin    = lrtb;
		bottomMargin = lrtb;
		if (immediateGeometryUpdate) {
			updateGeometry(-1);
		}
		if (immediatePaintUpdate) {
			repaint();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setMargins(int lr, int tb)
{
	ucharRangeCheck(lr);
	ucharRangeCheck(tb);
	if ((lr != leftMargin) || (lr != rightMargin) ||
		(tb != topMargin)  || (tb != bottomMargin)) {
		leftMargin   = lr;
		rightMargin  = lr;
		topMargin    = tb;
		bottomMargin = tb;
		if (immediateGeometryUpdate) {
			updateGeometry(-1);
		}
		if (immediatePaintUpdate) {
			repaint();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setMargins(int l, int r, int t, int b)
{
	ucharRangeCheck(l);
	ucharRangeCheck(r);
	ucharRangeCheck(t);
	ucharRangeCheck(b);
	if ((l != leftMargin) || (r != rightMargin) ||
		(t != topMargin)  || (b != bottomMargin)) {
		leftMargin   = l;
		rightMargin  = r;
		topMargin    = t;
		bottomMargin = b;
		if (immediateGeometryUpdate) {
			updateGeometry(-1);
		}
		if (immediatePaintUpdate) {
			repaint();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setTextPlacement(VImageBox::Placement placement)
{
	if (textPlacement != placement) {
		boolean update_geometry = textPlacement % 2 != placement % 2;
		textPlacement = placement;
		if (currentlyDisplayingText) {
			if (update_geometry) {
				if (immediateGeometryUpdate) {
					updateGeometry(-1);
				}
				if (immediatePaintUpdate) {
					repaint();
				}
			}
			else if (immediatePaintUpdate) {
				repaintBackgroundAndContents();
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setTextImageGap(int gap)
{
	ucharRangeCheck(gap);
	if (textImageGap != gap) {
		textImageGap = gap;
		if (currentlyDisplayingText) {
			if (immediateGeometryUpdate) {
				updateGeometry(-1);
			}
			if (immediatePaintUpdate) {
				repaint();
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setTextImageDisplacement(int dxy)
{
	ucharRangeCheck(dxy);
	textImageDisplacementX = dxy;
	textImageDisplacementY = dxy;
	if (immediatePaintUpdate) {
		repaintBackgroundAndContents();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setTextImageDisplacement(int dx, int dy)
{
	ucharRangeCheck(dx);
	ucharRangeCheck(dy);
	textImageDisplacementX = dx;
	textImageDisplacementY = dy;
	if (immediatePaintUpdate) {
		repaintBackgroundAndContents();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::setImageDrawMode(int mode)
{
	if (mode != drawMode) {
		drawMode = mode;
		if (immediatePaintUpdate) {
			repaintBackgroundAndContents();
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::useTheImageDrawMode(boolean b)
{
	if (b) {
		if (amUsingTheImageDrawMode) {
			return;
		}
		amUsingTheImageDrawMode = 1;
	}
	else {
		if (!amUsingTheImageDrawMode) {
			return;
		}
		amUsingTheImageDrawMode = 0;
	}
	if (immediatePaintUpdate) {
		repaintBackgroundAndContents();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::disable(boolean b)
{
	if (b) {
		if (currentlyDisabled) {
			return;
		}
		currentlyDisabled = 1;
	}
	else if (!currentlyDisabled) {
		return;
	}
	else {
		currentlyDisabled = 0;
	}
	if (immediatePaintUpdate) {
		repaintBackgroundAndContents();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::enableBool(boolean b)
{
	enable(b);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::resized(int w, int h)
{
	currentWidth = w;
	currentHeight = h;
	return VControl::resized(w, h);
}

// --------------------------------------------------------------------------
//
boolean VImageBox::erased()
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VImageBox::paint()
{
	receivedPaint = 1;
	thePort->openOn(this);
	paintBorder(*thePort);
	paintBackground(*thePort);
	paintContents(*thePort);
	thePort->close();
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VImageBox::paintBorder(VPort& port)
{
	if (isBorderHidden()) {
		return;
	}
	switch (borderStyle) {
	case SunkenBorder:
		paintSunkenBorder(port);
		break;
	case SunkenThickBorder:
		paintSunkenThickBorder(port);
		break;
	case RaisedBorder:
		paintRaisedBorder(port);
		break;
	case RaisedThickBorder:
		paintRaisedThickBorder(port);
		break;
	case EtchedInBorder:
		paintEtchedInBorder(port);
		break;
	case EtchedOutBorder:
		paintEtchedOutBorder(port);
		break;
	case FlatBorder:
		paintFlatBorder(port);
		break;
	case FlatThickBorder:
		paintFlatThickBorder(port);
		break;
	case FlatDoubleBorder:
		paintFlatDoubleBorder(port);
		break;
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::paintBackground(VPort& port)
{
	VBrush *brush = getBackground();
	if (brush == 0) {
		port.useBrush(&VColorScheme::
					   systemBrush(VColorScheme::ShadedBackground));
	}
	else {
		port.useBrush(brush);
	}
	if (isBorderHidden()) {
		theRectangle->set(CornerDim, 0, 0, getWidth(), getHeight());
	}
	else {
		theRectangle->set(CornerDim,
						  getClientAreaX(), getClientAreaY(),
						  getClientAreaWidth(), getClientAreaHeight());
	}
	port.fillRegion(theRectangle, FALSE);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintContents(VPort& port)
{
	int x, y, w, h;

	x = getClientAreaX() + getLeftMargin();
	y = getClientAreaY() + getTopMargin();
	w = getClientAreaWidth() - getHorizontalMarginThickness();
	h = getClientAreaHeight() - getVerticalMarginThickness();

	int ix, iy, tx, ty, itw, ith;

	if (isDisplayingImage() && (getImage() != 0)) {
		if (isDisplayingText() && (getText(*theString) > 0)) {
			//
			// Display text and image.
			//
			switch (getTextPlacement()) {
			case Above:
			case Below:
				if (w >= getTextWidth()) {
					tx = (w - getTextWidth()) / 2;
				}
				else {
					tx = 0;
				}
				if (w >= getImageWidth()) {
					ix = (w - getImageWidth()) / 2;
				}
				else {
					ix = 0;
				}
				ith = getImageHeight() + getTextHeight() + getTextImageGap();
				if (getTextPlacement() == Above) {
					if (h >= ith) {
						ty = (h - ith) / 2;
					}
					else {
						ty = 0;
					}
					iy = ty + getTextHeight() + getTextImageGap();
				}
				else {
					if (h >= ith) {
						iy = (h - ith) / 2;
					}
					else {
						iy = 0;
					}
					ty = iy + getImageHeight() + getTextImageGap();
				}
				break;
			case Left:
			case Right:
				if (h >= getTextHeight()) {
					ty = (h - getTextHeight()) / 2;
				}
				else {
					ty = 0;
				}
				if (h >= getImageHeight()) {
					iy = (h - getImageHeight()) / 2;
				}
				else {
					iy = 0;
				}
				itw = getImageWidth() + getTextWidth() + getTextImageGap();
				if (getTextPlacement() == Left) {
					if (w >= itw) {
						tx = (w - itw) / 2;
					}
					else {
						tx = 0;
					}
					ix = tx + getTextWidth() + getTextImageGap();
				}
				else {
					if (w >= itw) {
						ix = (w - itw) / 2;
					}
					else {
						ix = 0;
					}
					tx = ix + getImageWidth() + getTextImageGap();
				}
				break;
			}
			tx += x + textImageDisplacementX;
			ty += y + textImageDisplacementY;
			ix += x + textImageDisplacementX;
			iy += x + textImageDisplacementY;
			if (isDisabled()) {
				//
				// Display disabled text and image.
				//
				getImage()->draw(&port, ix, iy,
								 imageMajorIndex, imageMinorIndex,
								 VImage::Disabled);
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedHighlight));
				port.wrtText(theString->gets(), tx + 1, ty + 1);
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedShadow));
				port.wrtText(theString->gets(), tx, ty);
			}
			else {
				//
				// Display enabled text and image.
				//
				if (drawMode != 0) {
					getImage()->draw(&port, ix, iy,
									 imageMajorIndex, imageMinorIndex,
									 drawMode);
				}
				else if (amUsingTheImageDrawMode) {
					getImage()->draw(&port, ix, iy,
									 imageMajorIndex, imageMinorIndex);
				}
				else {
					getImage()->draw(&port, ix, iy,
									 imageMajorIndex, imageMinorIndex,
									 VImage::Default);
				}
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedForeground));
				port.wrtText(theString->gets(), tx, ty);
			}
		}
		else {
			//
			// Display image.
			//
			if (w >= getImageWidth()) {
				ix = (w - getImageWidth()) / 2;
			}
			else {
				ix = 0;
			}
			if (h >= getImageHeight()) {
				iy = (h - getImageHeight()) / 2;
			}
			else {
				iy = 0;
			}
			ix += x + textImageDisplacementX;
			iy += y + textImageDisplacementY;
			if (isDisabled()) {
				//
				// Display disabled image.
				//
				getImage()->draw(&port, ix, iy,
								 imageMajorIndex, imageMinorIndex,
								 VImage::Disabled);
			}
			else {
				//
				// Display enabled image.
				//
				if (drawMode != 0) {
					getImage()->draw(&port, ix, iy,
									 imageMajorIndex, imageMinorIndex,
									 drawMode);
				}
				else if (amUsingTheImageDrawMode) {
					getImage()->draw(&port, ix, iy,
									 imageMajorIndex, imageMinorIndex);
				}
				else {
					getImage()->draw(&port, ix, iy,
									 imageMajorIndex, imageMinorIndex,
									 VImage::Default);
				}
			}
		}
	}
	else if (isDisplayingText() && (getText(*theString) > 0)) {
		//
		// Display text.
		//
		if (w >= getTextWidth()) {
			tx = (w - getTextWidth()) / 2;
		}
		else {
			tx = 0;
		}
		if (h >= getTextHeight()) {
			ty = (h - getTextHeight()) / 2;
		}
		else {
			ty = 0;
		}
		tx += x + textImageDisplacementX;
		ty += y + textImageDisplacementY;
		if (isDisabled()) {
			//
			// Display disabled text.
			//
			port.usePen(&VColorScheme::
						 systemPen(VColorScheme::ShadedHighlight));
			port.wrtText(theString->gets(), tx + 1, ty + 1);
			port.usePen(&VColorScheme::
						 systemPen(VColorScheme::ShadedShadow));
			port.wrtText(theString->gets(), tx, ty);
		}
		else {
			//
			// Display enabled text.
			//
			port.usePen(&VColorScheme::
						 systemPen(VColorScheme::ShadedForeground));
			port.wrtText(theString->gets(), tx, ty);
		}
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::paintSunkenBorder(VPort& port)
{
	// Draw the top/left outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
	port.moveTo(0, 0);
	port.lineTo(getWidth() - 1, 0);
	port.moveTo(0, 1);
	port.lineTo(0, getHeight() - 1);

	// Draw the bottom/right outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	port.moveTo(0, getHeight() - 1);
	port.lineTo(getWidth(), getHeight() - 1);
	port.moveTo(getWidth() - 1, 0);
	port.lineTo(getWidth() - 1, getHeight() - 1);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintSunkenThickBorder(VPort& port)
{
	// Draw the top/left outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
	port.moveTo(0, 0);
	port.lineTo(getWidth() - 1, 0);
	port.moveTo(0, 1);
	port.lineTo(0, getHeight() - 1);

	// Draw the bottom/right outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	port.moveTo(0, getHeight() - 1);
	port.lineTo(getWidth(), getHeight() - 1);
	port.moveTo(getWidth() - 1, 0);
	port.lineTo(getWidth() - 1, getHeight() - 1);

	// Draw the top/left inner shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
	port.moveTo(1, 1);
	port.lineTo(getWidth() - 2, 1);
	port.moveTo(1, 2);
	port.lineTo(1, getHeight() - 2);

	// Draw the bottom/right inner shadow.
	
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkHighlight));
	port.moveTo(1, getHeight() - 2);
	port.lineTo(getWidth() - 1, getHeight() - 2);
	port.moveTo(getWidth() - 2, 1);
	port.lineTo(getWidth() - 2, getHeight() - 2);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintRaisedBorder(VPort& port)
{
	// Draw the top/left outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	port.moveTo(0, 0);
	port.lineTo(getWidth() - 1, 0);
	port.moveTo(0, 1);
	port.lineTo(0, getHeight() - 1);

	// Draw the bottom/right outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
	port.moveTo(0, getHeight() - 1);
	port.lineTo(getWidth(), getHeight() - 1);
	port.moveTo(getWidth() - 1, 0);
	port.lineTo(getWidth() - 1, getHeight() - 1);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintRaisedThickBorder(VPort& port)
{
	// Draw the top/left outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	port.moveTo(0, 0);
	port.lineTo(getWidth() - 1, 0);
	port.moveTo(0, 1);
	port.lineTo(0, getHeight() - 1);

	// Draw the bottom/right outer shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
	port.moveTo(0, getHeight() - 1);
	port.lineTo(getWidth(), getHeight() - 1);
	port.moveTo(getWidth() - 1, 0);
	port.lineTo(getWidth() - 1, getHeight() - 1);

	// Draw the top/left inner shadow.

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkHighlight));
	port.moveTo(1, 1);
	port.lineTo(getWidth() - 2, 1);
	port.moveTo(1, 2);
	port.lineTo(1, getHeight() - 2);

	// Draw the bottom/right inner shadow.
	
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
	port.moveTo(1, getHeight() - 2);
	port.lineTo(getWidth() - 1, getHeight() - 2);
	port.moveTo(getWidth() - 2, 1);
	port.lineTo(getWidth() - 2, getHeight() - 2);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintEtchedInBorder(VPort& port)
{
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
	theRectangle->set(CornerDim, 0, 0, getWidth() - 1, getHeight() - 1);
	port.frameRegion(theRectangle);
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	port.moveTo(1, 1);
	port.lineTo(getWidth() - 2, 1);
	port.moveTo(0, getHeight() - 1);
	port.lineTo(getWidth(), getHeight() - 1);
	port.moveTo(1, 2);
	port.lineTo(1, getHeight() - 2);
	port.moveTo(getWidth() - 1, 0);
	port.lineTo(getWidth() - 1, getHeight() - 1);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintEtchedOutBorder(VPort& port)
{
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	theRectangle->set(CornerDim, 0, 0, getWidth() - 1, getHeight() - 1);
	port.frameRegion(theRectangle);
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
	port.moveTo(1, 1);
	port.lineTo(getWidth() - 2, 1);
	port.moveTo(0, getHeight() - 1);
	port.lineTo(getWidth(), getHeight() - 1);
	port.moveTo(1, 2);
	port.lineTo(1, getHeight() - 2);
	port.moveTo(getWidth() - 1, 0);
	port.lineTo(getWidth() - 1, getHeight() - 1);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintFlatBorder(VPort& port)
{
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	theRectangle->set(CornerDim, 0, 0, getWidth(), getHeight());
	port.frameRegion(theRectangle);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintFlatThickBorder(VPort& port)
{
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
	theRectangle->set(CornerDim, 0, 0, getWidth(), getHeight());
	port.frameRegion(theRectangle);
	theRectangle->inset(1, 1);
	port.frameRegion(theRectangle);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintFlatDoubleBorder(VPort& port)
{
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedForeground));
	theRectangle->set(CornerDim, 0, 0, getWidth(), getHeight());
	port.frameRegion(theRectangle);
	theRectangle->inset(2, 2);
	port.frameRegion(theRectangle);
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedBackground));
	theRectangle->inset(-1, -1);
	port.frameRegion(theRectangle);
}

// --------------------------------------------------------------------------
//
void VImageBox::paintNullBorder(VPort& port)
{
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedBackground));
	if (borderThickness > 1) {
		theRectangle->set(CornerDim,
						  borderThickness / 2, borderThickness / 2,
						  getWidth() - borderThickness / 2,
						  getHeight() - borderThickness / 2);
		VColorScheme::
			systemPen(VColorScheme::ShadedBackground).width(borderThickness);
		port.frameRegion(theRectangle);
		VColorScheme::systemPen(VColorScheme::ShadedBackground).width(1);
	}
	else {
		theRectangle->set(CornerDim, 0, 0, getWidth(), getHeight());
		port.frameRegion(theRectangle);
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::repaint()
{
	if (receivedPaint) {
		paint();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::repaintBackgroundAndContents()
{
	if (receivedPaint) {
		thePort->openOn(this);
		paintBackground(*thePort);
		paintContents(*thePort);
		thePort->close();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::repaintContents()
{
	if (receivedPaint) {
		thePort->openOn(this);
		//
		// If we are drawing transparently, then repainting
		// the contents implies repainting the background.
		//
		int draw_mode = amUsingTheImageDrawMode && (theImage != 0) ?
						theImage->getDrawMode() : drawMode;
		if (draw_mode & VImage::Transparent) {
			paintBackground(*thePort);
		}
		paintContents(*thePort);
		thePort->close();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::repaintBorder()
{
	if (receivedPaint) {
		thePort->openOn(this);
		paintBorder(*thePort);
		thePort->close();
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::systemColorChanged()
{
	if (theImage != 0) {
		theImage->systemColorChanged();
	}
	paint();
}

// --------------------------------------------------------------------------
//
void VImageBox::setFont(VFont *font)
{
	VControl::setFont(font);
	calculateTextSize();
}

// --------------------------------------------------------------------------
//
void VImageBox::calculateTextSize()
{
	if (getText(*theString) > 0) {
		thePort->openOn(this);
		thePort->useFont(getFont());
		int w, h;
		thePort->textSize(theString->gets(), &w, &h);
		textWidth = w + 1, textHeight = h;
		thePort->close();
	}
	else {
		textWidth = textHeight = 0;
	}
}

// --------------------------------------------------------------------------
//
void VImageBox::getPreferredSize(int *w, int *h) const
{
	if (w != 0) {
		*w = getNonClientAreaHorizontalThickness() +
			 getHorizontalMarginThickness();
		if (currentlyDisplayingImage) {
			*w += getImageWidth();
		}
		if (currentlyDisplayingText) {
			if (!currentlyDisplayingImage) {
				*w += getTextWidth();
			}
			else switch (getTextPlacement()) {
			case Above:
			case Below:
				if (getTextWidth() > getImageWidth()) {
					*w += getTextWidth() - getImageHeight();
				}
				break;
			case Left:
			case Right:
				*w += getTextWidth() + getTextImageGap() + 2;
				break;
			}
		}
	}
	if (h != 0) {
		*h = getNonClientAreaVerticalThickness() +
			 getVerticalMarginThickness();
		if (currentlyDisplayingImage) {
			*h += getImageHeight();
		}
		if (currentlyDisplayingText) {
			if (!currentlyDisplayingImage) {
				*h += getTextHeight();
			}
			else switch (getTextPlacement()) {
			case Above:
			case Below:
				*h += getTextHeight() + getTextImageGap();
				break;
			case Left:
			case Right:
				if (getTextHeight() > getImageHeight()) {
					*h += getTextHeight() - getImageHeight();
				}
				break;
			}
		}
	}
}

// ----------------------------------------------------------------------------
//
void VImageBox::getInternalBorderGeometry(int *left, int *right,
                	                      int *top, int *bottom) const
{
    if (left   != 0) { *left   = borderThickness + leftMargin;   }
    if (right  != 0) { *right  = borderThickness + rightMargin;  }
    if (top    != 0) { *top    = borderThickness + topMargin;    }
    if (bottom != 0) { *bottom = borderThickness + bottomMargin; }
}

#ifndef PM
// ---------------------------------------------------------------------------
//
HPALETTE VImageBox::getPalette()
{
	if (isDisplayingImage() && (getImage() != 0) &&
							   (getImage()->getOriginalBitmap() != 0)) {
		return getImage()->getOriginalBitmap()->getPalette();
	}
	else {
		return VControl::getPalette();
	}
}
#endif

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VImageBox::putTo(VArchiver& a)
{
	VImageBox::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VImageBox::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VControl::putTo(a, FALSE);

	// Write the object version tag.

	a << VImageBoxArchiveTag;

	// Write any object specific data.

	a.putObject(theImage);

	a << short(imageMajorIndex);
	a << short(drawMode);
	a << short(borderStyle);
	a << short(borderThickness);
	a << short(leftMargin);
	a << short(rightMargin);
	a << short(topMargin);
	a << short(bottomMargin);
	a << short(textPlacement);
	a << short(textImageGap);
	a << short(textImageDisplacementX);
	a << short(textImageDisplacementY);
	a << char(currentlyDisabled);
	a << char(currentlyToggledIn);
	a << char(currentlyDisplayingImage);
	a << char(currentlyDisplayingText);
	a << char(amUsingTheImageDrawMode);
	a << char(0);
}

// ---------------------------------------------------------------------------
//
void VImageBox::getFrom(VArchiver& a)
{
	// Read the base object data.

	VControl::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VImageBoxArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VImageBoxArchiveTag));
		return;
	}

	// Read any object specific data.

	theImage = (VImage *)a.getObject(0);

	char c; short s;

	a >> s; imageMajorIndex = s;
	a >> s; drawMode = s;
	a >> s; borderStyle = s;
	a >> s; borderThickness = s;
	a >> s; leftMargin = s;
	a >> s; rightMargin = s;
	a >> s; topMargin = s;
	a >> s; bottomMargin = s;
	a >> s; textPlacement = s;
	a >> s; textImageGap = s;
	a >> s; textImageDisplacementX = s;
	a >> s; textImageDisplacementY = s;
	a >> c; currentlyDisabled = c;
	a >> c; currentlyToggledIn = c;
	a >> c; currentlyDisplayingImage = c;
	a >> c; currentlyDisplayingText = c;
	a >> c; amUsingTheImageDrawMode = c;
}

// ---------------------------------------------------------------------------
//
void VImageBox::getFrom(VArchiver& a,
						VObject *parentwin, boolean do_realize)
{
	VImageBox::getFrom(a);

	if (do_realize) {
		VImageBox::realize((VWindow *)parentwin);
	}
}

// ---------------------------------------------------------------------------
//
void VImageBox::realize(VWindow *parentwin)
{
	VControl::realize(parentwin);
	construct();
}

// ---------------------------------------------------------------------------
//
boolean VImageBox::loadResources(VRscArchiver& ra)
{
	if (theImage == 0) {
		return FALSE;
	}
	return theImage->loadResources(ra);
}

#endif // !CV_NOARCHIVER
