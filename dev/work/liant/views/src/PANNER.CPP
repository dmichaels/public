//	panner.cpp -- [UNDER DEVELOPMENT]
//
//	VPanner implementation
//
//  Allegris Foundation 1.1.00
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
//	04/01/96 dgm	Original.
// --------------------------------------------------------------------------

#include "panner.h"
#include "port.h"
#include "clrscm.h"
#include "brush.h"
#include "rect.h"
#include "str.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

#include <stdio.h>

// --------------------------------------------------------------------------
//
defineClass(VPanner, VControl)
defineArchiveRevision(VPanner, 1)

// --------------------------------------------------------------------------
//
struct DraggingData {
	VWindow	   *thumb;
	int			panner_wh[2];
	int			panner_xy[2];
	int			thumb_wh[2];
	int			thumb_xy[2];
	int			offset_xy[2];
};

// --------------------------------------------------------------------------
//
static long			objectCount			= 0;
static VPort	   *thePort				= 0;
static const int	minimumThumbSize[2]	= { 8, 8 };
static DraggingData	dragging			= { 0, 0, 0, 0, 0, 0 };

// --------------------------------------------------------------------------
//
VPanner::VPanner()
	: VControl()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VPanner::VPanner(const VFrame& frame, VWindow *parentwin,
				 const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VPanner::VPanner(VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VPanner::VPanner(const VString& name,
				 const VFrame& frame, VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VPanner::VPanner(const VString& name,
				 VWindow *parentwin, const VStyle& style)
	: VControl()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VPanner::operator()(const VFrame &frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VPanner::operator()(const VFrame &frame, VWindow *parentwin,
						 const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VPanner::operator()(const VFrame &frame, VWindow *parentwin,
						 const VStyle& style, const VString& name)
{
	VControl::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VPanner::~VPanner()
{
	if (customBackground) {
		delete getBackground();
	}
	if (thumbCustomColors && (theThumb != 0)) {
		delete theThumb->getBackground();
	}
	if (--objectCount == 0) {
		if (thePort != 0) {
			delete thePort; thePort = 0;
		}
	}
}

// --------------------------------------------------------------------------
//
void VPanner::initialize()
{
	if (objectCount++ == 0) {
		thePort = new VPort;
	}
	theThumb				= 0;
	clientObject			= 0;
	beginChangeMethod		= 0;
	changeMethod			= 0;
	endChangeMethod			= 0;
	scrollTracking			= 1;
	currentlyPanning		= 0;
	ltMargin[0]				= 0;
	rbMargin[0]				= 0;
	ltMargin[1]				= 0;
	rbMargin[1]				= 0;
	customBackground		= 0;
	scrollBarBackground		= 1;
	thumbCustomColors		= 0;
	thumbScrollBarColors	= 1;
	thumbBorder				= 1;
}

// --------------------------------------------------------------------------
//
void VPanner::construct()
{
	clipToChildren(TRUE);
	if (scrollBarBackground) {
		setBackground(&VBrush::system(VColorScheme::ShadedHighlight));
	}
	else {
		setBackground(&VBrush::system(VColorScheme::ShadedShadow));
	}
	createThumb();
	theThumb->disableFocus();
	disableFocus();
}

// --------------------------------------------------------------------------
//
void VPanner::createThumb()
{
	theThumb = newThumb(this);
}

// --------------------------------------------------------------------------
//
void VPanner::uponBeginChange(VObject *o, method m)
{
	clientObject = o;
	beginChangeMethod = (void (VObject::*)())m;
}

// --------------------------------------------------------------------------
//
void VPanner::uponChange(VObject *o, method m)
{
	clientObject = o;
	changeMethod = (boolean (VObject::*)(long, long))m;
}

// --------------------------------------------------------------------------
//
void VPanner::uponEndChange(VObject *o, method m)
{
	clientObject = o;
	endChangeMethod = (void (VObject::*)())m;
}

// --------------------------------------------------------------------------
//
void VPanner::beginChange()
{
	if ((clientObject != 0) && (beginChangeMethod != 0)) {
		(clientObject->*beginChangeMethod)();
	}
}

// --------------------------------------------------------------------------
//
boolean VPanner::change(long x, long y)
{
	if ((clientObject != 0) && (changeMethod != 0)) {
		return (clientObject->*changeMethod)(x, y);
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
void VPanner::endChange()
{
	if ((clientObject != 0) && (endChangeMethod != 0)) {
		(clientObject->*endChangeMethod)();
	}
}

// --------------------------------------------------------------------------
//
void VPanner::setMargins(int left, int right, int top, int bottom)
{
	if (left   < 0) { left   = 0; } else if (left   > 255) { left   = 255; }
	if (right  < 0) { right  = 0; } else if (right  > 255) { right  = 255; }
	if (top    < 0) { top    = 0; } else if (top    > 255) { top    = 255; }
	if (bottom < 0) { bottom = 0; } else if (bottom > 255) { bottom = 255; }

	ltMargin[0] = left;
	rbMargin[0] = right;
	ltMargin[1] = top;
	rbMargin[1] = bottom;
}

// --------------------------------------------------------------------------
//
void VPanner::setBackgroundColor(const VColor& color)
{
	if (customBackground) {
		delete getBackground();
	}
	setBackground(new VBrush(color));
	customBackground = 1;
	update();
}

// --------------------------------------------------------------------------
//
void VPanner::setForegroundColor(const VColor& color)
{
	if (theThumb != 0) {
		if (thumbCustomColors) {
			delete theThumb->getBackground();
		}
		theThumb->setBackground(new VBrush(color));
		theThumb->setForeground(color);
		thumbCustomColors = 1;
		theThumb->update();
	}
}

// --------------------------------------------------------------------------
//
void VPanner::setDefaultColors()
{
	if (customBackground) {
		delete getBackground();
		customBackground = 0;
		setBackground(&VBrush::system(VColorScheme::ShadedShadow));
	}
	if (thumbCustomColors && (theThumb != 0)) {
		delete theThumb->getBackground();
		theThumb->setForeground
			(VColor::system(VColorScheme::ShadedBackground));
		theThumb->setBackground
			(&VBrush::system(VColorScheme::ShadedBackground));
		thumbCustomColors = 0;
	}
}

// --------------------------------------------------------------------------
//
VColor& VPanner::getBackgroundColor() const
{
	if (theThumb == 0) {
		return VColor::system(VColorScheme::ShadedShadow);
	}
	else {
		return (VColor&)(((VPanner *)this)->getBackground()->background());
	}
}

// --------------------------------------------------------------------------
//
VColor& VPanner::getForegroundColor() const
{
	if (theThumb == 0) {
		return VColor::system(VColorScheme::ShadedForeground);
	}
	else {
		return (VColor&)
			   (((VPanner *)this)->theThumb->getBackground()->background());
	}
}

// --------------------------------------------------------------------------
//
void VPanner::setThumbBorder(boolean b)
{
	if (b) {
		if (!thumbBorder) {
			thumbBorder = 1;
			if (theThumb != 0) {
				theThumb->update();
			}
		}
	}
	else if (thumbBorder) {
		thumbBorder = 0;
		if (theThumb != 0) {
			theThumb->update();
		}
	}
}

// --------------------------------------------------------------------------
//
void VPanner::updateThumb()
{
	if (theThumb != 0) {
		int xy[2], wh[2];
		getThumbGeometry(xy, wh);
		if (dragging.thumb == theThumb) {
			dragging.thumb_xy[0] = xy[0];
			dragging.thumb_wh[0] = wh[0];
			dragging.thumb_xy[1] = xy[1];
			dragging.thumb_wh[1] = wh[1];
		}
		theThumb->setGeometry(xy[0], xy[1], wh[0], wh[1]);
	}
}

// --------------------------------------------------------------------------
//
void VPanner::beginDragging(int x, int y)
{
	currentlyPanning = 1;
	theThumb->getRel(&dragging.thumb_xy[0], &dragging.thumb_xy[1],
					 &dragging.thumb_wh[0], &dragging.thumb_wh[1]);
	sizeOfImage(&dragging.panner_wh[0], &dragging.panner_wh[1]);
	dragging.panner_wh[0] -= ltMargin[0] + rbMargin[0];
	dragging.panner_wh[1] -= ltMargin[1] + rbMargin[1];
	dragging.panner_xy[0] = ltMargin[0];
	dragging.panner_xy[1] = ltMargin[1];
	dragging.offset_xy[0] = x - dragging.thumb_xy[0];
	dragging.offset_xy[1] = y - dragging.thumb_xy[1];
	dragging.thumb = theThumb;
	notifier->captureMouseFor(this);
	notifier->mouseTracking(TRUE);
	beginChange();
}

// --------------------------------------------------------------------------
//
void VPanner::drag(int drag_x, int drag_y, int track)
{
	if (dragging.thumb != theThumb) {
		return;
	}

	int xy[2]; float fxy[2];

	xy[0] = drag_x - dragging.offset_xy[0];
	xy[1] = drag_y - dragging.offset_xy[1];

	for (int d = 0 ; d < 2 ; d++) {
		if (xy[d] + dragging.thumb_wh[d] >=
			dragging.panner_xy[d] + dragging.panner_wh[d]) {
			xy[d] = dragging.panner_xy[d] +
					dragging.panner_wh[d] - dragging.thumb_wh[d];
		}
		if (xy[d] < dragging.panner_xy[d]) {
			xy[d] = dragging.panner_xy[d];
		}
		if (dragging.thumb_wh[d] < dragging.panner_wh[d]) {
			fxy[d] = float(xy[d] - ltMargin[d]) /
					 float(dragging.panner_wh[d] - dragging.thumb_wh[d]);
		}
		else {
			fxy[d] = 0.0F;
		}
	}

	if ((xy[0] != dragging.thumb_xy[0]) || (xy[1] != dragging.thumb_xy[1])) {
		if (scrollData.setPositions(fxy[0], fxy[1])) {
			if ((track > 0) || ((track < 0) && scrollTracking)) {
				if (change(scrollData.getPosition(0),
						   scrollData.getPosition(1))) {
					int wh[2];
					getThumbGeometry(xy, wh);
					theThumb->setGeometry(dragging.thumb_xy[0] = xy[0],
										  dragging.thumb_xy[1] = xy[1],
										  dragging.thumb_wh[0] = wh[0],
										  dragging.thumb_xy[1] = wh[1]);
					return;
				}
			}
		}
		theThumb->setGeometryPosition(dragging.thumb_xy[0] = xy[0],
									  dragging.thumb_xy[1] = xy[1]);
	}
}

// --------------------------------------------------------------------------
//
void VPanner::endDragging(int x, int y)
{
	if (dragging.thumb != theThumb) {
		return;
	}
	drag(x, y, 1);
	dragging.thumb = 0;
	notifier->mouseTracking(FALSE);
	notifier->releaseCapture();
	currentlyPanning = 0;
	endChange();
}

// --------------------------------------------------------------------------
// Obtain the position and size the thumb should be.
//
void VPanner::getThumbGeometry(int xy[2], int wh[2]) const
{
	int this_wh[2];
	if (dragging.thumb == theThumb) {
		this_wh[0] = dragging.panner_wh[0];
		this_wh[1] = dragging.panner_wh[1];
	}
	else {
		((VPanner *)this)->sizeOfImage(&this_wh[0], &this_wh[1]);
		this_wh[0] -= ltMargin[0] + rbMargin[0];
		this_wh[1] -= ltMargin[1] + rbMargin[1];
	}
	for (int d = 0 ; d < 2 ; d++) {
		if (scrollData.getMinimum(d) == scrollData.getMaximum(d)) {
			xy[d] = ltMargin[d];
			wh[d] = this_wh[d];
		}
		else {
			float page_ratio = scrollData.getPageSizeRatio(d);
			wh[d] = int(page_ratio * this_wh[d] + 0.5F);
			if (wh[d] < minimumThumbSize[d]) {
				wh[d] = minimumThumbSize[d];
			}
			xy[d] = int(scrollData.getPositionRatio(d) *
						(this_wh[d] - wh[d]) + 0.5F) + ltMargin[d];
			if ((wh[d] == this_wh[d]) && (page_ratio < 1.0F)) {
				wh[d] -= 1;
			}
		}
	}
}

// --------------------------------------------------------------------------
//
boolean VPanner::resized(int, int)
{
	updateThumb();
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VPanner::getInternalBorderGeometry(int *left, int *right,
										int *top, int *bottom) const
{
	if (left   != 0) { *left   = ltMargin[0]; }
	if (right  != 0) { *right  = rbMargin[0]; }
	if (top    != 0) { *top    = ltMargin[1]; }
	if (bottom != 0) { *bottom = rbMargin[1]; }
}

// --------------------------------------------------------------------------
//
boolean VPanner::mouseDn(int x, int y)
{
	if (theThumb != 0) {
		int w, h, tx, ty, tw, th;
		sizeOfImage(&w, &h);
		if ((x < ltMargin[0]) || (x >= w - rbMargin[0]) ||
			(y < ltMargin[1]) || (y >= h - rbMargin[1])) {
			return TRUE;
		}
		theThumb->sizeOfWin(&tw, &th);
		if (x + tw >= w - rbMargin[0]) {
			x = w - rbMargin[0] - tw - 1;
		}
		if (x < ltMargin[0]) {
			x = ltMargin[0];
		}
		if (y + th >= h - rbMargin[1]) {
			y = h - rbMargin[1] - th - 1;
		}
		if (y < ltMargin[1]) {
			y = ltMargin[1];
		}
		theThumb->setGeometryPosition(x, y);
		beginDragging(x + 1, y + 1);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::mouseMv(int x, int y, int)
{
	drag(x, y);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::mouseUp(int x, int y)
{
	endDragging(x, y);
	return TRUE;
}

// ---------------------------------------------------------------------------
//
VWindow *VPanner::newThumb(VPanner *parentwin)
{
	return new Thumb(parentwin);
}

// ==========================================================================
// VPanner::ScrollData
// ==========================================================================

VPanner::ScrollData::ScrollData()
{
	for (int d = 0 ; d < 2 ; d++) {
		_min[d] = 0;
		_max[d] = 0;
		_pos[d] = 0;
		_page[d].ratio = 1.0F;
		_usePageRatio[d] = 1;
	}
	_subtractPageFromRange = 1;
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setPosition(int d, long pos)
{
	if (pos < _min[d]) {
		if (_pos[d] != _min[d]) {
			_pos[d] = _min[d];
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else if (_subtractPageFromRange) {
		long page = getPageSize(d);
		if (pos > _max[d] - page + 1) {
			if (_pos[d] != _max[d] - page + 1) {
				_pos[d] = _max[d] - page + 1;
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	else if (pos > _max[d]) {
		if (_pos[d] != _max[d]) {
			_pos[d] = _max[d];
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	if (_pos[d] != pos) {
		_pos[d] = pos;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setPositions(float fx, float fy)
{
	long x = _max[0] - _min[0];
	long y = _max[1] - _min[1];
	if (_subtractPageFromRange) {
		x = x - getPageSize(0) + 1;
		y = y - getPageSize(1) + 1;
	}
	return setPositions(long(x * fx + 0.5F) + _min[0],
						long(y * fy + 0.5F) + _min[1]);
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setPageSize(int d, long page)
{
	if (page < 0) {
		page = 0;
	}
	else if (page > _max[d] - _min[d] + 1) {
		page = _max[d] - _min[d] + 1;
	}
	if (_usePageRatio[d]) {
		_usePageRatio[d] = 0;
	}
	else if (page == _page[d].value) {
		return FALSE;
	}
	_page[d].value = page;
	if (_subtractPageFromRange) {
		if (_page[d].value > _max[d] - _min[d] + 1) {
			_page[d].value = _max[d] - _min[d] + 1;
		}
		if (_pos[d] > _max[d] - _min[d] - _page[d].value + 1) {
			_pos[d] = _max[d] - _min[d] - _page[d].value + 1;
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setPageSizeRatio(int d, float page)
{
	if (page < 0.0F) {
		page = 0.0F;
	}
	else if (page > 1.0F) {
		page = 1.0F;
	}
	if (_usePageRatio[d]) {
		if (_page[d].ratio == page) {
			return FALSE;
		}
	}
	else {
		_usePageRatio[d] = 1;
	}
	_page[d].ratio = page;
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setRange(int d, long min, long max)
{
	if (min > max) {
		long tmp = min;
		min = max;
		max = min;
	}
	if ((min == _min[d]) && (max == _max[d])) {
		return FALSE;
	}
	_min[d] = min;
	_max[d] = max;
	if (!_usePageRatio[d] && (_page[d].value > _max[d] - _min[d] + 1)) {
		_page[d].value = _max[d] - _min[d] + 1;
	}
	if (_pos[d] < _min[d]) {
		_pos[d] = _min[d];
	}
	if (_pos[d] > _max[d]) {
		_pos[d] = _max[d];
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setValues(int d, long min, long max,
											  long pos, long page)
{
	boolean changed = FALSE;

#if 0
	if (currentlyPanning) {
		if (page < 0) {
			page = 0;
		}
		else if (page > _max[d] - _min[d] + 1) {
			page = _max[d] - _min[d] + 1;
		}
		if (_usePageRatio[d] || (_page[d].value != page)) {
			_usePageRatio[d] = 0;
			_page[d].value = page;
			changed = TRUE;
		}
		if (_pos[d] > _max[d] - _min[d] - _page[d] + 1) {
			_pos[d] = _max[d] - _min[d] - _page[d] + 1;
			changed = TRUE;
		}
		return changed;
	}
#endif

	if (min > max) {
		long tmp = min;
		min = max;
		max = min;
	}
	if (page < 0) {
		page = 0;
	}
	else if (page > max - min + 1) {
		page = max - min + 1;
	}
	if (pos < min) {
		pos = min;
	}
	else if (pos > max - min - page + 1) {
		pos = max - min - page + 1;
	}
	if (_min[d] != min) {
		_min[d] = min;
		changed = TRUE;
	}
	if (_max[d] != max) {
		_max[d] = max;
		changed = TRUE;
	}
	if (_usePageRatio[d] || (_page[d].value != page)) {
		_page[d].value = page;
		_usePageRatio[d] = 0;
		changed = TRUE;
	}
	if (_pos[d] != pos) {
		_pos[d] = pos;
		changed = TRUE;
	}
	return changed;
}

// --------------------------------------------------------------------------
//
boolean VPanner::ScrollData::setSubtractPageFromRange(boolean b)
{
	if (b) {
		if (!_subtractPageFromRange) {
			return FALSE;
		}
		for (int d ; d < 2 ; d++) {
			long page = getPageSize(d);
			if (_pos[d] > _max[d] - page) {
				if (_pos[d] != _max[d] - page) {
					_pos[d] = _max[d] - page;
				}
			}
		}
	}
	else if (!_subtractPageFromRange) {
		return FALSE;
	}
	_subtractPageFromRange = b;
	return TRUE;
}

// --------------------------------------------------------------------------
//
float VPanner::ScrollData::getPageSizeRatio(int d) const
{
	if (_usePageRatio[d]) {
		return _page[d].ratio;
	}
	else {
		return float(_page[d].value) / float(_max[d] - _min[d] + 1);
	}
}

// --------------------------------------------------------------------------
//
float VPanner::ScrollData::getPositionRatio(int d) const
{
	if (_subtractPageFromRange) {
		long range = _max[d] - _min[d] - getPageSize(d) + 1;
		if (range > 0) {
			return float(_pos[d] - _min[d]) / float(range);
		}
		else {
			return 0.0F;
		}
	}
	else {
		return float(_pos[d] - _min[d]) / float(_max[d] - _min[d] + 1);
	}
}

// ==========================================================================
// VPanner::Thumb
// ==========================================================================

VPanner::Thumb::Thumb(VPanner *parentwin)
	: VWindow(VFrame(), parentwin)
{
	thePanner = parentwin;
	thisWidth = thisHeight = 0;
	setBackground(&VBrush::system(VColorScheme::ShadedBackground));
}

// --------------------------------------------------------------------------
//
void VPanner::Thumb::getPreferredSize(int *w, int *h) const
{
	if (w != 0) { *w = 75; }
	if (h != 0) { *h = 75; }
}

// --------------------------------------------------------------------------
//
void VPanner::Thumb::getMinimumSize(int *w, int *h) const
{
	if (w != 0) { *w = minimumThumbSize[0]; }
	if (h != 0) { *h = minimumThumbSize[1]; }
}

// ---------------------------------------------------------------------------
//
boolean VPanner::Thumb::resized(int w, int h)
{
	thisWidth = w; thisHeight = h;
	if (dragging.thumb == this) {
		dragging.thumb_wh[0] = w;
		dragging.thumb_wh[1] = h;
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::Thumb::paint()
{
	if (!thePort->openOn(this)) {
		return FALSE;
	}

	if ((thisWidth >= 4) && (thisHeight >= 4) && thePanner->thumbBorder) {

		VPen *shaded_highlight, *shaded_shadow;
		VPen *shaded_dark_highlight, *shaded_dark_shadow;

		if (thePanner->thumbCustomColors) {
			VColorScheme cs;
			cs.setShadedBackground(getForeground());
			shaded_highlight =
				new VPen(*cs.getColor(VColorScheme::ShadedHighlight));
			shaded_shadow = 
				new VPen(*cs.getColor(VColorScheme::ShadedShadow));
			shaded_dark_highlight =
				new VPen(*cs.getColor(VColorScheme::ShadedDarkHighlight));
			shaded_dark_shadow =
				new VPen(*cs.getColor(VColorScheme::ShadedDarkShadow));
		}
		else {
			shaded_highlight =
				&VPen::system(VColorScheme::ShadedHighlight);
			shaded_shadow = 
				&VPen::system(VColorScheme::ShadedShadow);
			shaded_dark_highlight =
				&VPen::system(VColorScheme::ShadedDarkHighlight);
			shaded_dark_shadow =
				&VPen::system(VColorScheme::ShadedDarkShadow);
		}

		// Outer upper/left border.

		if (thePanner->thumbScrollBarColors) {
			thePort->usePen(shaded_dark_highlight);
		}
		else {
			thePort->usePen(shaded_highlight);
		}

		thePort->moveTo(0, 0);
		thePort->lineTo(thisWidth - 1, 0);
		thePort->moveTo(0, 1);
		thePort->lineTo(0, thisHeight - 1);

		// Outer lower/right border.

		thePort->usePen(shaded_dark_shadow);
		thePort->moveTo(0, thisHeight - 1);
		thePort->lineTo(thisWidth, thisHeight - 1);
		thePort->moveTo(thisWidth - 1, 0);
		thePort->lineTo(thisWidth - 1, thisHeight - 1);

		if (!thePanner->thumbCustomColors) {

			// Inner upper/left border.

			if (thePanner->thumbScrollBarColors) {
				thePort->usePen(shaded_highlight);
			}
			else {
				thePort->usePen(shaded_dark_highlight);
			}

			thePort->moveTo(1, 1);
			thePort->lineTo(thisWidth - 2, 1);
			thePort->moveTo(1, 2);
			thePort->lineTo(1, thisHeight - 2);

			// Inner lower/right border.

			thePort->usePen(shaded_shadow);
			thePort->moveTo(1, thisHeight - 2);
			thePort->lineTo(thisWidth - 1, thisHeight - 2);
			thePort->moveTo(thisWidth - 2, 1);
			thePort->lineTo(thisWidth - 2, thisHeight - 2);
		}

		if (thePanner->thumbCustomColors) {
			delete shaded_highlight;
			delete shaded_shadow;
			delete shaded_dark_highlight;
			delete shaded_dark_shadow;
		}
	}

	thePort->close();

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::Thumb::mouseDn(int x, int y)
{
	if (thePanner != 0) {
		mapToScreen(&x, &y);
		thePanner->mapToWin(&x, &y);
		thePanner->beginDragging(x, y);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VPanner::Thumb::mouseUp(int x, int y)
{
	if (thePanner != 0) {
		return thePanner->mouseUp(x, y);
	}
	return TRUE;
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VPanner::putTo(VArchiver& a)
{
	VPanner::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VPanner::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VControl::putTo(a, FALSE);

	// Write the object version tag.

	a << VPannerArchiveTag;

	// Write any object specific data.

		// ToDo!!!

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VPanner::getFrom(VArchiver& a)
{
	// Read the base object data.

	VControl::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VPannerArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VPannerArchiveTag));
		return;
	}

	// Read any object specific data.

		// ToDo!!!
}

// ---------------------------------------------------------------------------
//
void VPanner::getFrom(VArchiver& a, VObject *parentwin, boolean do_realize)
{
	VPanner::getFrom(a);

	if (do_realize) {
		VPanner::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VPanner::realize(VWindow *parentwin)
{
	VControl::realize(parentwin);
	construct();
}

#endif // !CV_NOARCHIVER
