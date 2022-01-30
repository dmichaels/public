//	scrollw.cpp -- [UNDER DEVELOPMENT]
//
//	VScrollWindow implementation [Common] -- 1 of 2 (see scrollwx.cpp)
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
//	03/12/96 dgm	Original.
// --------------------------------------------------------------------------

#include "scrollw.h"
#include "scrollbr.h"
#include "panner.h"
#include "utilwin.h"
#include "geoframe.h"
#include "geoform.h"
#include "geowin.h"
#include "clrscm.h"
#include "color.h"
#include "brush.h"
#include "rect.h"
#include "port.h"
#include "popupmen.h"
#include "menuitem.h"
#include "menudata.h"
#include "notifier.h"
#include "report.h"
#include <limits.h>
#include <math.h>
#include <time.h>

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

#include <stdio.h>

// ==========================================================================
// ::: VScrollWindow
// ==========================================================================

defineClass(VScrollWindow, VWindow)
defineArchiveRevision(VScrollWindow, 0)

// --------------------------------------------------------------------------
//
#ifdef abs
#undef abs
#endif

#ifdef min
#undef min
#endif

#define abs(a)						((a) < 0 ? (-(a)) : (a))
#define min(a,b)					((a) <= (b) ? (a) : (b))
#define yieldPaint(w)				(notifier->yieldPaint())
#define checkUnsignedCharRange(c)	{ if ((c) < 0) (c) = 0; \
									  else if ((c) > UCHAR_MAX) \
									  		   (c) = UCHAR_MAX; }
#define checkShortRange(c)			{ if ((c) < 0) (c) = 0; \
									  else if ((c) > SHRT_MAX) \
									  		   (c) = SHRT_MAX; }

// --------------------------------------------------------------------------
//
static unsigned long	objectCount			= 0;
static VPort		   *thePort				= 0;
static VRectangle	   *theRectangle		= 0;
static VPopupMenu	   *configurationMenu	= 0;
static boolean			performedAutoScroll	= FALSE;

// --------------------------------------------------------------------------
//
VScrollWindow::VScrollWindow()
	: VWindow()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VScrollWindow::VScrollWindow(const VFrame& frame, VWindow *parentwin,
							 const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VScrollWindow::VScrollWindow(VWindow *parentwin, const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VScrollWindow::VScrollWindow(const VString& name,
							 const VFrame& frame, VWindow *parentwin,
							 const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VScrollWindow::VScrollWindow(const VString& name,
							 VWindow *parentwin, const VStyle& style)
	: VWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VScrollWindow::operator()(const VFrame& frame, VWindow *parentwin,
							   const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VScrollWindow::operator()(const VFrame& frame, VWindow *parentwin,
							   const VStyle& style, const VString& name)
{
	if (style.contains(StyleNoBorder)) {
		VWindow::operator()(frame, parentwin, style, name);
	}
	else {
		VWindow::operator()(frame, parentwin,
							style + StyleBorder + Style3D, name);
	}
	construct();
}

// --------------------------------------------------------------------------
//
VScrollWindow::~VScrollWindow()
{
	if (haveManyPanners) {
		delete thePannerList;
	}

	if (--objectCount == 0) {
		delete configurationMenu;	configurationMenu	= 0;
		delete thePort;				thePort				= 0;
		delete theRectangle;		theRectangle		= 0;
		destroyAutoScrollData();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::initialize()
{
	if (objectCount++ == 0) {
		thePort = new VPort;
		theRectangle = new VRectangle;
	}

	workWindow										= 0;
	viewWindow										= 0;
	thePanner										= 0; // thePannerList
	haveManyPanners									= 0;
	haveOwnPanner									= 0;
	useThePanner									= 1;
	drawPannerIndicator								= 1;
	pannerHidden									= 1;
	useTheConfigurationMenu							= 1;
	haveViewBorder									= 0;
	workWindowHasBeenSized							= 0;
	lookingForFirstResized							= 1;
	firstResized									= 0;
	eraseViewAreaBackground							= 1;
	eraseViewableWorkAreaBackground					= 0;
	autoScrollEnabled								= 1;
	currentlyAutoScrolling							= 0;

	for (int d = 0 ; d < 2 ; d++) {
		dimData[d].scrollBar						= 0;
		dimData[d].lineIncrement					= 1;
		dimData[d].pageIncrement					= 1;
		dimData[d].haveScrollBar					= 1;
		dimData[d].manageScrollBar					= 1;
		dimData[d].scrollBarHidden					= 1;
		dimData[d].oppositeScrollBarPosition		= 0;
		dimData[d].scrollBarTracking				= 1;
		dimData[d].showScrollBarEvenWhenNotNeeded	= 0;
		dimData[d].scrollingDisabled				= 0;
		dimData[d].marginThickness					= 0;
		dimData[d].scrollBarGapThickness			= 0;
		dimData[d].stretchWorkArea					= 0;
		dimData[d].scaleScrollValues				= 0;
		dimData[d].inScrollBarCallback				= 0;
		dimData[d].ignoreOneScrollBarCallback		= 0;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::construct()
{
	setGeoManager(new Manager());
	createHorizontalScroll();
	createVerticalScroll();
	createView();
	updateLayout();
	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));
	clipToChildren(TRUE);
	clipToSiblings(TRUE);
	scrollData.update(this);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::createHorizontalScroll()
{
	if (dimData[0].haveScrollBar && (dimData[0].scrollBar == 0)) {
		dimData[0].scrollBar = newHorizontalScrollBar();
		dimData[0].scrollBar->tracking(dimData[0].scrollBarTracking);
		dimData[0].scrollBar->setScrollAmts(1, 10);
		dimData[0].scrollBar->setPos(0);
		dimData[0].scrollBar->setTabStop(FALSE);
		dimData[0].scrollBar->uponChange
			(this, methodOf(VScrollWindow, horizontalScrollCallback));
		dimData[0].scrollBar->uponEndChange
			(this, methodOf(VScrollWindow, horizontalScrollEndCallback));
		dimData[0].scrollBar->uponLineClick
			(this, methodOf(VScrollWindow, horizontalScrollLineCallback));
		dimData[0].scrollBar->uponPageClick
			(this, methodOf(VScrollWindow, horizontalScrollPageCallback));
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::createVerticalScroll()
{
	if (dimData[1].haveScrollBar && (dimData[1].scrollBar == 0)) {
		dimData[1].scrollBar = newVerticalScrollBar();
		dimData[1].scrollBar->tracking(dimData[1].scrollBarTracking);
		dimData[1].scrollBar->setScrollAmts(1, 10);
		dimData[1].scrollBar->setPos(0);
		dimData[1].scrollBar->setTabStop(FALSE);
		dimData[1].scrollBar->uponChange
			(this, methodOf(VScrollWindow, verticalScrollCallback));
		dimData[1].scrollBar->uponEndChange
			(this, methodOf(VScrollWindow, verticalScrollEndCallback));
		dimData[1].scrollBar->uponLineClick
			(this, methodOf(VScrollWindow, verticalScrollLineCallback));
		dimData[1].scrollBar->uponPageClick
			(this, methodOf(VScrollWindow, verticalScrollPageCallback));
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::createView()
{
	if (viewWindow == 0) {
		viewWindow = newViewWindow();
	}
}

// --------------------------------------------------------------------------
//
VWindow *VScrollWindow::newViewWindow() const
{
	return new ViewWindow((VScrollWindow *)this);
}

// --------------------------------------------------------------------------
//
VScrollBar *VScrollWindow::newHorizontalScrollBar() const
{
	VScrollBar *scrollbar =
		new VScrollBar(VFrame(), (VScrollWindow *)this,
					   StyleHorizontal + StyleHidden);
	int thickness;
	scrollbar->determinePreferredSize(0, &thickness);
	scrollbar->setFrame(VFrame(0, thickness));
	return scrollbar;
}

// --------------------------------------------------------------------------
//
VScrollBar *VScrollWindow::newVerticalScrollBar() const
{
	VScrollBar *scrollbar =
		new VScrollBar(VFrame(), (VScrollWindow *)this,
					   StyleVertical + StyleHidden);
	int thickness;
	scrollbar->determinePreferredSize(&thickness, 0);
	scrollbar->setFrame(VFrame(thickness, 0));
	return scrollbar;
}

// --------------------------------------------------------------------------
//
int VScrollWindow::getHorizontalScrollThickness() const
{
	if (hasHorizontalScrollBar() && dimData[0].manageScrollBar) {
		int scroll_thickness, dummy;
		dimData[0].scrollBar->sizeOfWin(&dummy, &scroll_thickness);
		return scroll_thickness + dimData[1].scrollBarGapThickness;
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
int VScrollWindow::getVerticalScrollThickness() const
{
	if (hasVerticalScrollBar() && dimData[1].manageScrollBar) {
		int scroll_thickness, dummy;
		dimData[1].scrollBar->sizeOfWin(&scroll_thickness, &dummy);
		return scroll_thickness + dimData[0].scrollBarGapThickness;
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setWorkWindow(VWindow *window)
{
	if ((window == 0) || (window->getParent() != viewWindow)) {
		return;
	}
	workWindow = window;
	workWindow->setDontFrame(TRUE);
	scrollData.update(this);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setUnits(int units)
{
	scrollData.setUnits(this, -1, units);
	updateConfiguration();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setUnits(int hunits, int vunits)
{
	scrollData.setUnits(this, 0, hunits);
	scrollData.setUnits(this, 1, vunits);
	updateConfiguration();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalUnits(int units)
{
	scrollData.setUnits(this, 0, units);
	updateConfiguration();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalUnits(int units)
{
	scrollData.setUnits(this, 1, units);
	updateConfiguration();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::useHorizontalScrollBar(boolean b)
{
	if (b) {
		if (!dimData[0].haveScrollBar) {
			dimData[0].haveScrollBar = 1;
			createHorizontalScroll();
			updateConfiguration();
		}
	}
	else if (dimData[0].haveScrollBar) {
		hideHorizontalScrollBar();
		dimData[0].haveScrollBar = 0;
		updateConfiguration();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::useVerticalScrollBar(boolean b)
{
	if (b) {
		if (!dimData[1].haveScrollBar) {
			dimData[1].haveScrollBar = 1;
			createVerticalScroll();
			updateConfiguration();
		}
	}
	else if (dimData[1].haveScrollBar) {
		hideVerticalScrollBar();
		dimData[1].haveScrollBar = 0;
		updateConfiguration();
	}
}
// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollBar(VScrollBar *scrollbar)
{
	if (scrollbar == 0) {
		useHorizontalScrollBar(FALSE);
	}
	else if (dimData[0].scrollBar != scrollbar) {
		VScrollBar *doomed_scrollbar =
					dimData[0].scrollBar != 0 ? dimData[0].scrollBar : 0;
		dimData[0].scrollBar = scrollbar;
		dimData[1].manageScrollBar = dimData[1].manageScrollBar &&
							  (dimData[0].scrollBar->getParent() != this);
		updateLayout();
		delete doomed_scrollbar;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollBar(VScrollBar *scrollbar)
{
	if (scrollbar == 0) {
		useVerticalScrollBar(FALSE);
	}
	else if (dimData[1].scrollBar != scrollbar) {
		VScrollBar *doomed_scrollbar =
					dimData[1].scrollBar != 0 ? dimData[1].scrollBar : 0;
		dimData[1].scrollBar = scrollbar;
		dimData[1].manageScrollBar = dimData[1].manageScrollBar &&
							  (dimData[1].scrollBar->getParent() != this);
		updateLayout();
		delete doomed_scrollbar;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollBarPosition(int position)
{
	if (position != Opposite) {
		if (dimData[0].oppositeScrollBarPosition) {
			dimData[0].oppositeScrollBarPosition = 0;
			updateLayout();
		}
	}
	else if (!dimData[0].oppositeScrollBarPosition) {
		dimData[0].oppositeScrollBarPosition = 1;
		updateLayout();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollBarPosition(int position)
{
	if (position != Opposite) {
		if (dimData[1].oppositeScrollBarPosition) {
			dimData[1].oppositeScrollBarPosition = 0;
			updateLayout();
		}
	}
	else if (!dimData[1].oppositeScrollBarPosition) {
		dimData[1].oppositeScrollBarPosition = 1;
		updateLayout();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalMargin(int margin)
{
	checkUnsignedCharRange(margin);
	if (dimData[0].marginThickness != margin) {
		dimData[0].marginThickness = margin;
		updateLayout();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalMargin(int margin)
{
	checkUnsignedCharRange(margin);
	if (dimData[1].marginThickness != margin) {
		dimData[1].marginThickness = margin;
		updateLayout();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollBarGap(int spacing)
{
	checkUnsignedCharRange(spacing);
	if (dimData[1].scrollBarGapThickness != spacing) {
		dimData[1].scrollBarGapThickness = spacing;
		if (isHorizontalScrollBarShowing()) {
			updateLayout();
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollBarGap(int spacing)
{
	checkUnsignedCharRange(spacing);
	if (dimData[0].scrollBarGapThickness != spacing) {
		dimData[0].scrollBarGapThickness = spacing;
		if (isVerticalScrollBarShowing()) {
			updateLayout();
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollBarLineIncrement(int increment)
{
	checkShortRange(increment);
	if (increment == 0) { increment = 1; }
	dimData[0].lineIncrement = increment;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollBarLineIncrement(int increment)
{
	checkShortRange(increment);
	if (increment == 0) { increment = 1; }
	dimData[1].lineIncrement = increment;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollBarPageIncrement(int page_increment)
{
	checkShortRange(page_increment);
	dimData[0].pageIncrement = page_increment;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollBarPageIncrement(int page_increment)
{
	checkShortRange(page_increment);
	dimData[1].pageIncrement = page_increment;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setScrollBarTracking(boolean b)
{
	setHorizontalScrollBarTracking(b);
	setVerticalScrollBarTracking(b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollBarTracking(boolean b)
{
	dimData[0].scrollBarTracking = b;
	if (hasHorizontalScrollBar()) {
		dimData[0].scrollBar->tracking(dimData[0].scrollBarTracking);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollBarTracking(boolean b)
{
	dimData[1].scrollBarTracking = b;
	if (hasVerticalScrollBar()) {
		dimData[1].scrollBar->tracking(dimData[1].scrollBarTracking);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHideScrollsWhenNotNeeded(boolean b)
{
	dimData[0].showScrollBarEvenWhenNotNeeded = !b;
	dimData[1].showScrollBarEvenWhenNotNeeded = !b;
	updateConfiguration();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::hideScrollBars(boolean b)
{
	if (b) {
		if (isHorizontalScrollBarShowing() && dimData[0].manageScrollBar) {
			dimData[0].scrollBarHidden = 1;
			dimData[0].scrollBar->hide();
			dimData[0].scrollBar->setDontFrame(TRUE);
			if (isVerticalScrollBarShowing()) {
				dimData[1].scrollBarHidden = 1;
				dimData[1].scrollBar->hide();
				dimData[1].scrollBar->setDontFrame(TRUE);
			}
			updateLayout();
		}
		else if (isVerticalScrollBarShowing() && dimData[1].manageScrollBar) {
			dimData[1].scrollBarHidden = 1;
			dimData[1].scrollBar->hide();
			dimData[1].scrollBar->setDontFrame(TRUE);
			updateLayout();
		}
	}
	else if (!isHorizontalScrollBarShowing() && dimData[0].manageScrollBar) {
		dimData[0].scrollBarHidden = 0;
		dimData[0].scrollBar->setDontFrame(FALSE);
		dimData[0].scrollBar->show();
		if (!isVerticalScrollBarShowing()) {
			dimData[1].scrollBarHidden = 0;
			dimData[1].scrollBar->setDontFrame(FALSE);
			dimData[1].scrollBar->show();
		}
		updateLayout();
	}
	else if (!isVerticalScrollBarShowing() && dimData[1].manageScrollBar) {
		dimData[1].scrollBarHidden = 0;
		dimData[1].scrollBar->setDontFrame(FALSE);
		dimData[1].scrollBar->show();
		updateLayout();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::hideHorizontalScrollBar(boolean b)
{
	if (hasHorizontalScrollBar() && dimData[0].manageScrollBar) {
		if (b) {
			if (!dimData[0].scrollBarHidden) {
				dimData[0].scrollBarHidden = 1;
				dimData[0].scrollBar->hide();
				dimData[0].scrollBar->setDontFrame(TRUE);
				updateLayout();
			}
		}
		else if (dimData[0].scrollBarHidden) {
			dimData[0].scrollBarHidden = 0;
			dimData[0].scrollBar->setDontFrame(FALSE);
			dimData[0].scrollBar->show();
			updateLayout();
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::hideVerticalScrollBar(boolean b)
{
	if (hasVerticalScrollBar() && dimData[1].manageScrollBar) {
		if (b) {
			if (!dimData[1].scrollBarHidden) {
				dimData[1].scrollBarHidden = 1;
				dimData[1].scrollBar->hide();
				dimData[1].scrollBar->setDontFrame(TRUE);
				updateLayout();
			}
		}
		else if (dimData[1].scrollBarHidden) {
			dimData[1].scrollBarHidden = 0;
			dimData[1].scrollBar->setDontFrame(FALSE);
			dimData[1].scrollBar->show();
			updateLayout();
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalWorkAreaStretchable(boolean b)
{
	if (b) {
		if (!dimData[0].stretchWorkArea) {
			dimData[0].stretchWorkArea = 1;
			updateConfiguration();
		}
	}
	else if (dimData[0].stretchWorkArea) {
		dimData[0].stretchWorkArea = 0;
		updateConfiguration();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalWorkAreaStretchable(boolean b)
{
	if (b) {
		if (!dimData[1].stretchWorkArea) {
			dimData[1].stretchWorkArea = 1;
			updateConfiguration();
		}
	}
	else if (dimData[1].stretchWorkArea) {
		dimData[1].stretchWorkArea = 0;
		updateConfiguration();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setMaximizeViewableWorkArea(boolean b)
{
	scrollData.setMaximizeViewableWorkArea(this, -1, b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setMaximizeViewableWorkAreaWidth(boolean b)
{
	scrollData.setMaximizeViewableWorkArea(this, 0, b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setMaximizeViewableWorkAreaHeight(boolean b)
{
	scrollData.setMaximizeViewableWorkArea(this, 1, b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setSmoothScrolling(boolean b)
{
	scrollData.setSmoothScrolling(this, -1, b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalSmoothScrolling(boolean b)
{
	scrollData.setSmoothScrolling(this, 0, b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalSmoothScrolling(boolean b)
{
	scrollData.setSmoothScrolling(this, 1, b);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollOrigin(int origin)
{
	if (origin != Opposite) {
		if (scrollData.isOppositeScrollOrigin(0)) {
			scrollData.setOppositeScrollOrigin(this, 0, FALSE);
		}
	}
	else if (!scrollData.isOppositeScrollOrigin(0)) {
		scrollData.setOppositeScrollOrigin(this, 0, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollOrigin(int origin)
{
	if (origin != Opposite) {
		if (scrollData.isOppositeScrollOrigin(1)) {
			scrollData.setOppositeScrollOrigin(this, 1, FALSE);
		}
	}
	else if (!scrollData.isOppositeScrollOrigin(1)) {
		scrollData.setOppositeScrollOrigin(this, 1, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setPaintNub(boolean b)
{
	if (b) {
		if (drawPannerIndicator) {
			return;
		}
	}
	else {
		if (drawPannerIndicator) {
			return;
		}
	}
	drawPannerIndicator = b;
	update();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::getScrollValues(int d, long max_pos_page[3]) const
{
	if (scrollData.getWorkAreaUnitWH(d) <= 0) {
		max_pos_page[0] = max_pos_page[1] = max_pos_page[2] = 0;
		return;
	}

	if (dimData[d].scaleScrollValues) {
	ScaleScrollValues:
		max_pos_page[0] = MaxScrollValue;
		float page_ratio = float(scrollData.getViewableWorkAreaUnitWH(d)) /
						   float(scrollData.getWorkAreaUnitWH(d));
		if (scrollData.getViewableWorkAreaPixelOffsetXY(d) > 0) {
			page_ratio -=
				(float(scrollData.getViewableWorkAreaPixelOffsetXY(d)) /
				 float(scrollData.retrieveUnitSize
				 	   (this, d,
					    scrollData.getViewableWorkAreaUnitXY(d)))) *
				(1.0F / scrollData.getWorkAreaUnitWH(d));
		}
		if (scrollData.getViewAreaExtraPixelWH(d) < 0) {
			page_ratio -=
				(float(-scrollData.getViewAreaExtraPixelWH(d)) /
				 float(scrollData.retrieveUnitSize
				 	   (this, d,
					    scrollData.getViewableWorkAreaUnitXY(d) +
						scrollData.getViewableWorkAreaUnitWH(d) - 1))) *
				(1.0F / scrollData.getWorkAreaUnitWH(d));
		}
		if (!scrollData.isMaximizeViewableWorkArea(d) &&
			 scrollData.isSmoothScrolling(d) && (page_ratio < 1.0F)) {
			page_ratio *= 0.5F;
		}
		if (page_ratio < 1.0F) {
			max_pos_page[2] = long(page_ratio * (max_pos_page[0] + 1));
			float values_per_unit;
			if (scrollData.getMaxViewableWorkAreaPixelOffsetXY(d) > 0) {
				values_per_unit =
					float(max_pos_page[0] - max_pos_page[2] + 1) /
					(float(scrollData.getMaxViewableWorkAreaUnitXY(d)) +
					 (float(scrollData.
					 		getMaxViewableWorkAreaPixelOffsetXY(d)) /
					  float(scrollData.retrieveUnitSize
					  		(this, d,
							 scrollData.getMaxViewableWorkAreaUnitXY(d)))));
			}
			else if (scrollData.getMaxViewableWorkAreaUnitXY(d) > 0) {
				values_per_unit =
					float(max_pos_page[0] - max_pos_page[2] + 1) /
					float(scrollData.getMaxViewableWorkAreaUnitXY(d));
			}
			else {
				values_per_unit = 0.0F;
			}
			max_pos_page[1] =
				long(scrollData.getViewableWorkAreaUnitXY(d) *
					 values_per_unit + 0.5F);
			if (scrollData.getViewableWorkAreaPixelOffsetXY(d) > 0) {
				max_pos_page[1] +=
					long((float(scrollData.
								getViewableWorkAreaPixelOffsetXY(d)) /
						  float(scrollData.retrieveUnitSize
						 	    (this, d,
							     scrollData.getViewableWorkAreaUnitXY(d)))) *
						 values_per_unit + 0.5F);
			}
		}
		else {
			max_pos_page[1] = 0;
			max_pos_page[2] = MaxScrollValue + 1;
		}
	}
	else {
		if (scrollData.getViewableWorkAreaUnitWH(d) <
			scrollData.getWorkAreaUnitWH(d)) {
			max_pos_page[2] = scrollData.getViewableWorkAreaUnitWH(d);
			max_pos_page[0] = scrollData.getMaxViewableWorkAreaUnitXY(d) +
							  max_pos_page[2] - 1;
			if (max_pos_page[0] > MaxScrollValue) {
				((VScrollWindow *)this)->dimData[d].scaleScrollValues = 1;
				goto ScaleScrollValues;
			}
			else {
				max_pos_page[1] = scrollData.getViewableWorkAreaUnitXY(d);
			}
		}
		else {
			max_pos_page[0] = scrollData.getWorkAreaUnitWH(d) - 1;
			max_pos_page[1] = 0;
			max_pos_page[2] = scrollData.getWorkAreaUnitWH(d);
		}
	}
	if (max_pos_page[2] == 0) {
		max_pos_page[2] = 1;
	}
}

// --------------------------------------------------------------------------
//
long VScrollWindow::getScrollPosition(int d, long page) const
{
	if (dimData[d].scaleScrollValues) {
		float pos, values_per_unit;
		if (scrollData.getMaxViewableWorkAreaPixelOffsetXY(d) > 0) {
			values_per_unit =
				float(MaxScrollValue - page + 1) /
				 (float(scrollData.getMaxViewableWorkAreaUnitXY(d)) +
				  (float(scrollData.getMaxViewableWorkAreaPixelOffsetXY(d)) /
				   float(scrollData.retrieveUnitSize
				   		 (this, d,
						  scrollData.getMaxViewableWorkAreaUnitXY(d)))));
				
		}
		else {
			long divisor = scrollData.getMaxViewableWorkAreaUnitXY(d);
			if (divisor != 0) {
				values_per_unit =
					float(MaxScrollValue - page + 1) / float(divisor);
			}
			else {
				values_per_unit = 0.0F;
			}
		}
		pos = scrollData.getViewableWorkAreaUnitXY(d) * values_per_unit;
		if (scrollData.getViewableWorkAreaPixelOffsetXY(d) > 0) {
			int divisor = scrollData.retrieveUnitSize
						  (this, d, scrollData.getViewableWorkAreaUnitXY(d));
			if (divisor != 0) {
				pos += (float(scrollData.getViewableWorkAreaPixelOffsetXY(d)) /
					    float(divisor)) * values_per_unit;
			}
		}
		return long(pos);
	}
	else {
		return scrollData.getViewableWorkAreaUnitXY(d);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::mapScrollValue(int d, long& value, float& offset)
{
	if (dimData[d].scaleScrollValues) {
		long max_value = dimData[d].scrollBar->getMax() -
						 dimData[d].scrollBar->getPageSize() + 1;
		float values_per_unit;
		if (scrollData.getMaxViewableWorkAreaPixelOffsetXY(d) > 0) {
			int last_unit_size =
				scrollData.retrieveUnitSize
					(this, d, scrollData.getMaxViewableWorkAreaUnitXY(d));
			values_per_unit =
				float(max_value) /
				 (float(scrollData.getMaxViewableWorkAreaUnitXY(d)) +
				  (float(scrollData.
				  		 getMaxViewableWorkAreaPixelOffsetXY(d)) /
				   float(last_unit_size)));
		}
		else if (scrollData.getMaxViewableWorkAreaUnitXY(d) > 0) {
			values_per_unit = float(max_value) /
							  float(scrollData.
							  		getMaxViewableWorkAreaUnitXY(d));
		}
		else {
			values_per_unit = float(max_value);
		}
		float v = float(value) / values_per_unit;
		value = long(v);
		if (scrollData.isSmoothScrolling(d)) {
			offset = v - float(value);
		}
		else {
			offset = 0.0F;
		}
	}
	else {
		offset = 0.0F;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::updateWorkArea(const VRectangle *r)
{
	scrollData.update(this);
	if ((viewWindow != 0) && (r != 0)) {
		viewWindow->update((VRectangle *)r, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::updateConfiguration(int new_w, int new_h)
{
	boolean	called_from_resized;
	int		view_area_border_w, view_area_border_h;
	int		show_horizontal_scroll, show_vertical_scroll;
	int		new_view_area_wh[2], old_view_area_wh[2];
	int		reduce_wh_if_scroll_needed_yx[2], scroll_dxy[2];

	// Calculate the new view-area size (taking into account borders).

	if (new_w < 0) {
		called_from_resized = FALSE;
		if (new_h < 0) {
			sizeOfImage(&new_w, &new_h);
		}
		else {
			int dummy;
			sizeOfImage(&new_w, &dummy);
		}
	}
	else if (new_h < 0) {
		int dummy;
		called_from_resized = FALSE;
		sizeOfImage(&dummy, &new_h);
	}
	else {
		called_from_resized = TRUE;
	}

	if ((new_w <= 0) || (new_h <= 0)) {
		return;
	}

	viewWindow->getSystemBorderSize
		(&view_area_border_w, &view_area_border_h);

	new_view_area_wh[0] = new_w - (view_area_border_w +
								   dimData[0].marginThickness);
	new_view_area_wh[1] = new_h - (view_area_border_h +
								   dimData[1].marginThickness);

//	new_view_area_wh[0] = new_w - view_area_border_w;
//	new_view_area_wh[1] = new_h - view_area_border_h;

//	if (((new_view_area_wh[0] - dimData[0].marginThickness) <= 0) ||
//		((new_view_area_wh[1] - dimData[1].marginThickness) <= 0)) {
//		return;
//	}

	// Update the scroll-data.

	
	if (dimData[0].showScrollBarEvenWhenNotNeeded) {
		reduce_wh_if_scroll_needed_yx[1] = 0;
		new_view_area_wh[1] -= getHorizontalScrollThickness();
	}
	else {
		reduce_wh_if_scroll_needed_yx[1] = getHorizontalScrollThickness();
	}
	if (dimData[1].showScrollBarEvenWhenNotNeeded) {
		reduce_wh_if_scroll_needed_yx[0] = 0;
		new_view_area_wh[0] -= getVerticalScrollThickness();
	}
	else {
		reduce_wh_if_scroll_needed_yx[0] = getVerticalScrollThickness();
	}

//	reduce_wh_if_scroll_needed_yx[0] += dimData[0].marginThickness;
//	reduce_wh_if_scroll_needed_yx[1] += dimData[1].marginThickness;

	old_view_area_wh[0] = scrollData.getViewAreaPixelWH(0);
	old_view_area_wh[1] = scrollData.getViewAreaPixelWH(1);

	scrollData.resize(this, new_view_area_wh,
							reduce_wh_if_scroll_needed_yx, scroll_dxy);

	// See if we need to enable/disable, show/hide,
	// or update the range/position of the scrolls.

	if (hasHorizontalScrollBar() &&
		(isHorizontalScrollNeeded() ||
		 dimData[0].showScrollBarEvenWhenNotNeeded)) {
		show_horizontal_scroll = 1;
	}
	else {
		show_horizontal_scroll = 0;
	}
	if (hasVerticalScrollBar() &&
		(isVerticalScrollNeeded() ||
		 dimData[1].showScrollBarEvenWhenNotNeeded)) {
		show_vertical_scroll = 1;
	}
	else {
		show_vertical_scroll = 0;
	}

	dimData[0].scrollBar->setDontFrame(!show_horizontal_scroll);
	dimData[1].scrollBar->setDontFrame(!show_vertical_scroll);

	// Enable/disable and show/hide the scrolls as necessary,
	// and set the range and page-size appropriately.

	if (isHorizontalScrollNeeded()) {
		if (show_horizontal_scroll) {
			dimData[0].scrollBar->enable();
		}
		setHorizontalScrollValues();
	}
	else if (show_horizontal_scroll) {
		dimData[0].scrollBar->disable();
	}

	if (isVerticalScrollNeeded()) {
		if (show_vertical_scroll) {
			dimData[1].scrollBar->enable();
		}
		setVerticalScrollValues();
	}
	else if (show_vertical_scroll) {
		dimData[1].scrollBar->disable();
	}

	// Update the attachments now.

	updateLayout(show_horizontal_scroll, show_vertical_scroll);

	// Call the base class resized method now if necessary (tricky).

	if (called_from_resized) {
		VWindow::resized(new_w, new_h);
	}

	if (show_horizontal_scroll) {
		dimData[0].scrollBarHidden = 0;
		dimData[0].scrollBar->show();
	}
	else {
		dimData[0].scrollBarHidden = 1;
		dimData[0].scrollBar->hide();
	}
	if (show_vertical_scroll) {
		dimData[1].scrollBarHidden = 0;
		dimData[1].scrollBar->show();
	}
	else {
		dimData[1].scrollBarHidden = 1;
		dimData[1].scrollBar->hide();
	}

	// showHorizontalScrollBar(show_horizontal_scroll);
	// showVerticalScrollBar(show_vertical_scroll);

	// If we have a work-area window then stretch it if necessary.

	if (workWindow != 0) {
		int wx, wy, ww, wh, dummy;
		workWindow->determinePreferredSize(&ww, &wh);
		if (dimData[0].stretchWorkArea && !isHorizontalScrollNeeded() &&
			(scrollData.getViewAreaExtraPixelWH(0) > 0)) {
			if (dimData[1].stretchWorkArea && !isVerticalScrollNeeded() &&
				(scrollData.getViewAreaExtraPixelWH(1) > 0)) {
				workWindow->setGeometry
					(0, 0, scrollData.getViewAreaPixelWH(0),
						   scrollData.getViewAreaPixelWH(1));
			}
			else {
				workWindow->getRel(&dummy, &wy, &dummy, &dummy);
				workWindow->setGeometry
					(0, wy, scrollData.getViewAreaPixelWH(0), wh);
				performScroll(0, scroll_dxy[1]);
			}
		}
		else if (dimData[1].stretchWorkArea && !isVerticalScrollNeeded() &&
				 (scrollData.getViewAreaExtraPixelWH(1) > 0)) {
			workWindow->getRel(&wx, &dummy, &dummy, &dummy);
			workWindow->setGeometry
				(wx, 0, ww, scrollData.getViewAreaPixelWH(1));
			performScroll(scroll_dxy[0], 0);
		}
		else {
			workWindow->setGeometrySize(ww, wh);
			performScroll(scroll_dxy[0], scroll_dxy[1]);
		}
	}
	else {
		performScroll(scroll_dxy[0], scroll_dxy[1]);
		if (dimData[0].stretchWorkArea &&
			(new_view_area_wh[0] != old_view_area_wh[0]) &&
			(getHorizontalUnits() == 0) &&
			(getViewableWorkAreaUnitX() +
			 getViewableWorkAreaUnitWidth() == getWorkAreaUnitWidth())) {
			long unit = getViewableWorkAreaUnitX() +
						getViewableWorkAreaUnitWidth() - 1;
			if (scrollData.retrieveVariableUnitSize(this, 0, unit) >
				getWorkAreaColumnWidth(unit, 0)) {
				VRectangle r(Corners, getCellX(unit, 0), 0,
									  getViewAreaPixelWidth(),
									  getViewAreaPixelHeight());
				viewWindow->update(&r, TRUE);
			}
		}
		if (dimData[1].stretchWorkArea &&
			(new_view_area_wh[1] != old_view_area_wh[1]) &&
			(getVerticalUnits() == 0) &&
			(getViewableWorkAreaUnitY() +
			 getViewableWorkAreaUnitHeight() == getWorkAreaUnitHeight())) {
			long unit = getViewableWorkAreaUnitY() +
						getViewableWorkAreaUnitHeight() - 1;
			
			if (scrollData.retrieveVariableUnitSize(this, 1, unit) >
				getWorkAreaRowHeight(unit, 0)) {
				VRectangle r(Corners, 0, getCellY(0, unit),
										 getViewAreaPixelWidth(),
										 getViewAreaPixelHeight());
				viewWindow->update(&r, TRUE);
			}
		}
	}

	// Finally, update any panners.

	updatePanner(0, TRUE);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::updateLayout(int show_horizontal_scroll,
								 int show_vertical_scroll)
{
	if (show_horizontal_scroll < 0) {
		show_horizontal_scroll = isHorizontalScrollBarShowing();
	}
	if (show_vertical_scroll < 0) {
		show_vertical_scroll = isVerticalScrollBarShowing();
	}
	if (show_horizontal_scroll && dimData[0].manageScrollBar) {
		if (show_vertical_scroll && dimData[1].manageScrollBar) {
			if (dimData[1].oppositeScrollBarPosition) {
				attachWin(dimData[0].scrollBar,	VAttach::Left,
											VAttach::Sibling,
											0,
											dimData[1].scrollBar);
				attachWin(dimData[0].scrollBar,	VAttach::Right,
											VAttach::Parent);
				attachWin(dimData[1].scrollBar,	VAttach::Left,
											VAttach::Parent);
				attachWin(dimData[1].scrollBar,	VAttach::Right,
											VAttach::UnAttached);
			}
			else {
				attachWin(dimData[0].scrollBar,	VAttach::Right,
											VAttach::Sibling,
											0,
											dimData[1].scrollBar);
				attachWin(dimData[0].scrollBar,	VAttach::Left,
											VAttach::Parent);
				attachWin(dimData[1].scrollBar,	VAttach::Right,
											VAttach::Parent);
				attachWin(dimData[1].scrollBar,	VAttach::Left,
											VAttach::UnAttached);
			}
			if (dimData[0].oppositeScrollBarPosition) {
				attachWin(dimData[1].scrollBar,	VAttach::Top,
											VAttach::Sibling,
											0,
											dimData[0].scrollBar);
				attachWin(dimData[1].scrollBar,	VAttach::Bottom,
											VAttach::Parent);
				attachWin(dimData[0].scrollBar,	VAttach::Top,
											VAttach::Parent);
				attachWin(dimData[0].scrollBar,	VAttach::Bottom,
											VAttach::UnAttached);
			}
			else {
				attachWin(dimData[1].scrollBar,	VAttach::Bottom,
											VAttach::Sibling,
											0,
											dimData[0].scrollBar);
				attachWin(dimData[1].scrollBar,	VAttach::Top,
											VAttach::Parent);
				attachWin(dimData[0].scrollBar,	VAttach::Bottom,
											VAttach::Parent);
				attachWin(dimData[0].scrollBar,	VAttach::Top,
											VAttach::UnAttached);
			}
			if (viewWindow != 0) {
				if (dimData[0].oppositeScrollBarPosition) {
					attachWin(viewWindow,	VAttach::Top,
											VAttach::Sibling,
											dimData[1].scrollBarGapThickness,
											dimData[0].scrollBar);
					attachWin(viewWindow,	VAttach::Bottom,
											VAttach::Parent,
											dimData[1].marginThickness);
				}
				else {
					attachWin(viewWindow,	VAttach::Bottom,
											VAttach::Sibling,
											dimData[1].scrollBarGapThickness,
											dimData[0].scrollBar);
					attachWin(viewWindow,	VAttach::Top,
											VAttach::Parent,
											dimData[1].marginThickness);
				}
				if (dimData[1].oppositeScrollBarPosition) {
					attachWin(viewWindow,	VAttach::Left,
											VAttach::Sibling,
											dimData[0].scrollBarGapThickness,
											dimData[1].scrollBar);
					attachWin(viewWindow,	VAttach::Right,
											VAttach::Parent,
											dimData[0].marginThickness);
				}
				else {
					attachWin(viewWindow,	VAttach::Right,
											VAttach::Sibling,
											dimData[0].scrollBarGapThickness,
											dimData[1].scrollBar);
					attachWin(viewWindow,	VAttach::Left,
											VAttach::Parent,
											dimData[0].marginThickness);
				}
			}
		}
		else {
			attachWin(dimData[0].scrollBar,		VAttach::Left,
											VAttach::Parent);
			attachWin(dimData[0].scrollBar,		VAttach::Right,
											VAttach::Parent);
			if (dimData[0].oppositeScrollBarPosition) {
				attachWin(dimData[0].scrollBar,	VAttach::Top,
											VAttach::Parent);
				attachWin(dimData[0].scrollBar,	VAttach::Bottom,
											VAttach::UnAttached);
			}
			else {
				attachWin(dimData[0].scrollBar,	VAttach::Bottom,
											VAttach::Parent);
				attachWin(dimData[0].scrollBar,	VAttach::Top,
											VAttach::UnAttached);
			}
			if (viewWindow != 0) {
				if (dimData[1].oppositeScrollBarPosition) {
					attachWin(viewWindow,	VAttach::Left,
											VAttach::Parent);
					attachWin(viewWindow,	VAttach::Right,
											VAttach::Parent,
											dimData[0].marginThickness);
				}
				else {
					attachWin(viewWindow,	VAttach::Left,
											VAttach::Parent,
											dimData[0].marginThickness);
					attachWin(viewWindow,	VAttach::Right,
												VAttach::Parent);
				}
				if (dimData[0].oppositeScrollBarPosition) {
					attachWin(viewWindow,	VAttach::Top,
											VAttach::Sibling,
											dimData[1].scrollBarGapThickness,
											dimData[0].scrollBar);
					attachWin(viewWindow,	VAttach::Bottom,
											VAttach::Parent,
											dimData[1].marginThickness);
				}
				else {
					attachWin(viewWindow,	VAttach::Bottom,
											VAttach::Sibling,
											dimData[1].scrollBarGapThickness,
											dimData[0].scrollBar);
					attachWin(viewWindow,	VAttach::Top,
											VAttach::Parent,
											dimData[1].marginThickness);
				}
			}
		}
	}
	else if (show_vertical_scroll && dimData[1].manageScrollBar) {
			attachWin(dimData[1].scrollBar,		VAttach::Top,
											VAttach::Parent);
			attachWin(dimData[1].scrollBar,		VAttach::Bottom,
											VAttach::Parent);
			if (dimData[1].oppositeScrollBarPosition) {
				attachWin(dimData[1].scrollBar,	VAttach::Left,
											VAttach::Parent);
				attachWin(dimData[1].scrollBar,	VAttach::Right,
											VAttach::UnAttached);
			}
			else {
				attachWin(dimData[1].scrollBar,	VAttach::Right,
											VAttach::Parent);
				attachWin(dimData[1].scrollBar,	VAttach::Left,
											VAttach::UnAttached);
			}
			if (viewWindow != 0) {
				if (dimData[0].oppositeScrollBarPosition) {
					attachWin(viewWindow,	VAttach::Top,
											VAttach::Parent);
					attachWin(viewWindow,	VAttach::Bottom,
											VAttach::Parent,
											dimData[1].marginThickness);
				}
				else {
					attachWin(viewWindow,	VAttach::Top,
											VAttach::Parent,
											dimData[1].marginThickness);
					attachWin(viewWindow,	VAttach::Bottom,
											VAttach::Parent);
				}
				if (dimData[1].oppositeScrollBarPosition) {
					attachWin(viewWindow,	VAttach::Left,
											VAttach::Sibling,
											dimData[0].scrollBarGapThickness,
											dimData[1].scrollBar);
					attachWin(viewWindow,	VAttach::Right,
											VAttach::Parent,
											dimData[0].marginThickness);
				}
				else {
					attachWin(viewWindow,	VAttach::Right,
											VAttach::Sibling,
											dimData[0].scrollBarGapThickness,
											dimData[1].scrollBar);
					attachWin(viewWindow,	VAttach::Left,
											VAttach::Parent,
											dimData[0].marginThickness);
				}
			}
	}
	else if (viewWindow != 0) {
		if (dimData[0].oppositeScrollBarPosition) {
			attachWin(viewWindow, VAttach::Top,	VAttach::Parent);
			attachWin(viewWindow, VAttach::Bottom,
								  VAttach::Parent,
								  dimData[1].marginThickness);
		}
		else {
			attachWin(viewWindow, VAttach::Top,
								  VAttach::Parent,
								  dimData[1].marginThickness);
			attachWin(viewWindow, VAttach::Bottom, VAttach::Parent);
		}
		if (dimData[1].oppositeScrollBarPosition) {
			attachWin(viewWindow, VAttach::Left, VAttach::Parent);
			attachWin(viewWindow, VAttach::Right,
								  VAttach::Parent,
								  dimData[0].marginThickness);
		}
		else {
			attachWin(viewWindow, VAttach::Left,
								  VAttach::Parent,
								  dimData[0].marginThickness);
			attachWin(viewWindow, VAttach::Right, VAttach::Parent);
		}
	}
	frameChildren();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setHorizontalScrollValues()
{
	if (hasHorizontalScrollBar()) {
		long max_pos_page[3];
		getScrollValues(0, max_pos_page);
		dimData[0].scrollBar->setScrollValues
			(0, max_pos_page[0], max_pos_page[2], max_pos_page[1]);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::setVerticalScrollValues()
{
	if (hasVerticalScrollBar()) {
		long max_pos_page[3];
		getScrollValues(1, max_pos_page);
		dimData[1].scrollBar->setScrollValues
			(0, max_pos_page[0], max_pos_page[2], max_pos_page[1]);
	}
}

// --------------------------------------------------------------------------
// Scroll the contents of this VScrollWindow by the given amount relative to
// the current position.  A positive value indicates a scroll to the right or
// bottom; a negative value indicates a scroll to the left or top.  If the
// value is greater than the amount available to scroll it is not error, the
// contents are just scrolled as much as they can be.  Scrolling is done by
// units whatever they are defined as (i.e. pixel, fixed-unit, variable-unit).
//
void VScrollWindow::scroll(long dx, long dy)
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scroll(this, dx, dy);
		}
		else {
			scrollData.scroll(this, dx, 0);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scroll(this, 0, dy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontal(long dx)
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scroll(this, dx, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVertical(long dy)
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scroll(this, 0, dy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollLineUp(long nlx, long nly)
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollLine(this, -nlx, -nly);
		}
		else {
			scrollData.scrollLine(this, -nlx, 0);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scrollLine(this, 0, -nly);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalLineUp(long nlx)
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scrollLine(this, -nlx, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalLineUp(long nly)
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scrollLine(this, 0, -nly);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollLineDown(long nlx, long nly)
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollLine(this, nlx, nly);
		}
		else {
			scrollData.scrollLine(this, nlx, 0);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scrollLine(this, 0, nly);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalLineDown(long nlx)
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scrollLine(this, nlx, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalLineDown(long nly)
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scrollLine(this, 0, nly);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollPageUp(long npx, long npy)
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollPage(this, -npx, -npy);
		}
		else {
			scrollData.scrollPage(this, -npx, 0);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scrollPage(this, 0, -npy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalPageUp(long npx)
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scrollPage(this, -npx, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalPageUp(long npy)
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scrollPage(this, 0, -npy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollPageDown(long npx, long npy)
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollPage(this, npx, npy);
		}
		else {
			scrollData.scrollPage(this, npx, 0);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scrollPage(this, 0, npy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalPageDown(long npx)
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scrollPage(this, npx, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalPageDown(long npy)
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scrollPage(this, 0, npy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalHome()
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scrollHome(this, TRUE, FALSE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalHome()
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scrollHome(this, FALSE, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHome()
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollHome(this, TRUE, TRUE);
		}
		else {
			scrollData.scrollHome(this, TRUE, FALSE);
		}
	}
	else {
		scrollData.scrollHome(this, FALSE, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalEnd()
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scrollEnd(this, TRUE, FALSE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalEnd()
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scrollEnd(this, FALSE, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollEnd()
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollEnd(this, TRUE, TRUE);
		}
		else {
			scrollData.scrollEnd(this, TRUE, FALSE);
		}
	}
	else {
		scrollData.scrollEnd(this, FALSE, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollPixel(long pixel_dx, long pixel_dy)
{
	scrollData.scrollPixel(this, -pixel_dx, -pixel_dy);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalPixel(long pixel_dx)
{
	scrollData.scrollPixel(this, -pixel_dx, 0);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalPixel(long pixel_dy)
{
	scrollData.scrollPixel(this, 0, -pixel_dy);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::scrollRatioTo(float x, float y)
{
	return scrollData.scrollRatioTo(this, x, y);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::scrollHorizontalRatioTo(float x)
{
	return scrollData.scrollHorizontalRatioTo(this, x);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::scrollVerticalRatioTo(float y)
{
	return scrollData.scrollVerticalRatioTo(this, y);
}

// --------------------------------------------------------------------------
// Scroll the specified cell into view if not currently visible or if only
// partially visible.
//
void VScrollWindow::scrollIntoView(long unit_x, long unit_y)
{
	long unit_xy[2] = { unit_x, unit_y };
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scrollIntoView(this, -1, unit_xy);
		}
		else {
			scrollData.scrollIntoView(this, 0, unit_xy);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scrollIntoView(this, 1, unit_xy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollHorizontalIntoView(long unit_x)
{
	if (!dimData[0].scrollingDisabled) {
		long unit_xy[2] = { unit_x, 0 };
		scrollData.scrollIntoView(this, 0, unit_xy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrollVerticalIntoView(long unit_y)
{
	if (!dimData[1].scrollingDisabled) {
		long unit_xy[2] = { 0, unit_y };
		scrollData.scrollIntoView(this, 1, unit_xy);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::endScroll()
{
	setHorizontalScrollValues();
	setVerticalScrollValues();
	updatePanner();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::horizontalScrollCallback(long value)
{
	if (dimData[0].ignoreOneScrollBarCallback) {
		dimData[0].ignoreOneScrollBarCallback = 0;
		return;
	}
	float offset;
	mapScrollValue(0, value, offset);
	dimData[0].inScrollBarCallback = 1;
	scrollData.scrollHorizontalTo(this, value, offset);
	dimData[0].inScrollBarCallback = 0;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::horizontalScrollEndCallback()
{
	setHorizontalScrollValues();
	updatePanner(0, TRUE);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::horizontalScrollLineCallback(int direction)
{
	dimData[0].ignoreOneScrollBarCallback = 1;
	scrollData.scrollLine(this, -direction * dimData[0].lineIncrement, 0);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::horizontalScrollPageCallback(int direction)
{
	dimData[0].ignoreOneScrollBarCallback = 1;
	scrollData.scrollPage(this, -direction * dimData[0].pageIncrement, 0);
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::verticalScrollCallback(long value)
{
	if (dimData[1].ignoreOneScrollBarCallback) {
		dimData[1].ignoreOneScrollBarCallback = 0;
		return;
	}
	float offset;
	float vvv = float(value);
	mapScrollValue(1, value, offset);
	dimData[1].inScrollBarCallback = 1;
	scrollData.scrollVerticalTo(this, value, offset);
	dimData[1].inScrollBarCallback = 0;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::verticalScrollEndCallback()
{
	setVerticalScrollValues();
	updatePanner(0, TRUE);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::verticalScrollLineCallback(int direction)
{
	dimData[1].ignoreOneScrollBarCallback = 1;
	scrollData.scrollLine(this, 0, -direction * dimData[1].lineIncrement);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::verticalScrollPageCallback(int direction)
{
	dimData[1].ignoreOneScrollBarCallback = 1;
	scrollData.scrollPage(this, 0, -direction * dimData[1].pageIncrement);
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrolledHorizontal(long, int)
{
}

// --------------------------------------------------------------------------
//
void VScrollWindow::scrolledVertical(long, int)
{
}

// --------------------------------------------------------------------------
// If we are in virtual-scroll mode, then scroll the contents of the contents
// of the view-window by the given offsets, or if we are in window-scroll
// mode, then scroll the work-window by the given offsets.  A positive dx/dy
// means scroll to the right/bottom; a negative dx/sy means scroll to the
// left/top.  Approriate expose/paint events will be generated.  Note that
// if dx/dy is greater or equal to the view-window's width/height then no
// scrolling is necessary, it's really just an adjustment and expose/paint
// event generation.
//
void VScrollWindow::performScroll(int dx, int dy)
{
	if ((dx == 0) && (dy == 0)) {
		return;
	}
	if (workWindow == 0) {
		if ((abs(dx) >= scrollData.getViewAreaPixelWH(0)) ||
			(abs(dy) >= scrollData.getViewAreaPixelWH(1))) {
			viewWindow->update();
		}
		else {
			viewWindow->scroll(dx, dy);
			yieldPaint(viewWindow);
		}
	}
	else {
		workWindow->setGeometryOffset(dx, dy);
		yieldPaint(workWindow);
	}
	if (dx != 0) {
		if (hasHorizontalScrollBar() && !dimData[0].inScrollBarCallback &&
			!dimData[0].scrollBar->isHidden() &&
			 dimData[0].scrollBar->isEnabled()) {
			dimData[0].scrollBar->setPos
				(getScrollPosition(0, dimData[0].scrollBar->getPageSize()));
		}
		scrolledHorizontal(scrollData.getViewableWorkAreaUnitXY(0),
						   scrollData.getViewableWorkAreaPixelOffsetXY(0));
	}
	if (dy != 0) {
		if (hasVerticalScrollBar() && !dimData[1].inScrollBarCallback &&
			!dimData[1].scrollBar->isHidden() &&
			 dimData[1].scrollBar->isEnabled()) {
			dimData[1].scrollBar->setPos
				(getScrollPosition(1, dimData[1].scrollBar->getPageSize()));
		}
		scrolledVertical(scrollData.getViewableWorkAreaUnitXY(1),
						 scrollData.getViewableWorkAreaPixelOffsetXY(1));
	}
	updatePanner();
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::resized(int w, int h)
{
	if (viewWindow == 0) {
		return FALSE;
	}
	if ((w > 0) && (h > 0)) {
		if (lookingForFirstResized) {
			lookingForFirstResized = 0;
			firstResized = 1;
		}
		else {
			firstResized = 0;
		}
		updateConfiguration(w, h);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::getPreferredSize(int *w, int *h) const
{
	long work_area_unit_w, work_area_unit_h;
	int work_area_pixel_w, work_area_pixel_h;

	retrieveWorkAreaSize(work_area_unit_w, work_area_unit_h);
	getWorkAreaPixelSize(0, 0, work_area_unit_w, work_area_unit_h,
						 &work_area_pixel_w, &work_area_pixel_h);
	static int screen_w = 0, screen_h = 0;
	if (screen_w == 0) { notifier->getScreenSize(&screen_w, &screen_h); }
	if (dimData[0].showScrollBarEvenWhenNotNeeded) {
		work_area_pixel_w += getHorizontalScrollThickness();
		work_area_pixel_h += getVerticalScrollThickness();
	}
	if (viewWindow != 0) {
		int view_border_w, view_border_h;
		viewWindow->getSystemBorderSize(&view_border_w, &view_border_h);
		work_area_pixel_w += view_border_w;
		work_area_pixel_h += view_border_h;
	}
	if (work_area_pixel_w > screen_w) { work_area_pixel_w = screen_w; }
	if (work_area_pixel_h > screen_h) { work_area_pixel_h = screen_h; }
	if (w != 0) { *w = work_area_pixel_w; }
	if (h != 0) { *h = work_area_pixel_h; }
}

// --------------------------------------------------------------------------
//
void VScrollWindow::getMinimumSize(int *w, int *h) const
{
	if (w != 0) {
		if (isVerticalScrollBarShowing()) {
			*w = getVerticalScrollThickness();
		}
		else {
			*w = 0;
		}
	}
	if (h != 0) {
		if (isHorizontalScrollBarShowing()) {
			*h = getHorizontalScrollThickness();
		}
		else {
			*h = 0;
		}
	}
	if (viewWindow != 0) {
		int view_border_w, view_border_h;
		viewWindow->getSystemBorderSize(&view_border_w, &view_border_h);
		if (w != 0) { *w += view_border_w; }
		if (h != 0) { *h += view_border_h; }
	}
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::paint()
{
	paintNub();
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::paintNub(int panner_showing)
{
	if (!drawPannerIndicator ||
		(!useThePanner && !useTheConfigurationMenu)) {
		return;
	}
	int x, y, w, h;
	getNubGeometry(x, y, w, h);
	if ((w > 0) && (h > 0)) {
		if (panner_showing < 0) { panner_showing = isPannerShowing(); } 
		if (thePort->openOn(this)) {
			thePort->useBrush(&VBrush::system
							  (VColorScheme::ShadedBackground));
			theRectangle->set(CornerDim, x, y, w, h);
			thePort->fillRegion(theRectangle, FALSE);
			x += 2, y += 2; w -= 4, h -= 4;
			theRectangle->set(CornerDim, x, y, w, h);
			VColorScheme::Color fg, bg;
			if (panner_showing) {
				fg = VColorScheme::ShadedForeground;
				bg = VColorScheme::ShadedForeground;
			}
			else {
				fg = VColorScheme::ShadedShadow;
				bg = VColorScheme::ShadedShadow;
			}
			thePort->usePen(&VPen::system(fg));
			thePort->useBrush(&VBrush::system(bg));
			thePort->frameRegion(theRectangle);
			x += 2, y += 2;
			w -= 4, h -= 4;
			w = int(w * 0.75F);
			h = int(h * 0.75F);
			theRectangle->set(CornerDim, x, y, w, h);
			thePort->fillRegion(theRectangle, FALSE);
			thePort->close();
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::getNubGeometry(int& x, int& y, int& w, int& h) const
{
	if (isHorizontalScrollBarShowing() &&
		isVerticalScrollBarShowing()) {
		int this_w, this_h, dummy;
		((VScrollWindow *)this)->sizeOfImage(&this_w, &this_h);
		dimData[0].scrollBar->sizeOfWin(&dummy, &h);
		dimData[1].scrollBar->sizeOfWin(&w, &dummy);
		if (getHorizontalScrollBarPosition() == Bottom) {
			y = this_h - h + 1;
			if (getVerticalScrollBarPosition() == Right) {
				x = this_w - w;
			}
			else {
				x = 0;
			}
		}
		else {
			y = 0;
			if (getVerticalScrollBarPosition() == Right) {
				x = this_w - w;
			}
			else {
				x = 0;
			}
		}
	}
	else {
		x = y = w = h = 0;
	}
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseEnter(int b)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseExit(int b)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseDn(int x, int y)
{
	if (currentlyAutoScrolling) {
		endAutoScroll();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseMv(int x, int y, int b)
{
	if (currentlyAutoScrolling) {
		autoScroll(x, y);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseUp(int x, int y)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseDbl(int, int)
{
	if (useThePanner) {
		showPanner(!isPannerShowing());
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mouseWheel(int delta, int x, int y, int)
{
	if (currentlyAutoScrolling) {
		endAutoScroll();
	}
	if (delta < 0) {
		scrollVerticalLineUp(-delta);
	}
	else if (delta > 0) {
		scrollVerticalLineDown(delta);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::rmouseDn(int x, int y)
{
	if (currentlyAutoScrolling) {
		endAutoScroll();
	}
	if (useTheConfigurationMenu) {
		if (configurationMenu == 0) {
			configurationMenu = newConfigurationMenu();
		}
		mapToScreen(&x, &y);
		configurationMenu->popupAt(x, y);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mmouseDn(int x, int y)
{
	if (autoScrollEnabled) {
		if (currentlyAutoScrolling) {
			endAutoScroll();
		}
		else {
			beginAutoScroll(x, y);
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::mmouseUp(int x, int y)
{
	if (currentlyAutoScrolling) {
		if (performedAutoScroll) {
			endAutoScroll();
		}
		else {
			beginAutoScroll(x, y);
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::key(int virtual_key, char ascii_key)
{
	if (currentlyAutoScrolling) {
		endAutoScroll();
	}
	switch (virtual_key) {
	case K_Up:
		scrollVerticalLineDown();
		break;
	case K_Down:
		scrollVerticalLineUp();
		break;
	case K_Left:
		scrollHorizontalLineDown();
		break;
	case K_Right:
		scrollHorizontalLineUp();
		break;
	case K_Prior:
		scrollVerticalPageDown();
		break;
	case K_Next:
		scrollVerticalPageUp();
		break;
	case K_Home:
		scrollVerticalHome();
		break;
	case K_End:
		scrollVerticalEnd();
		break;
	}

	if (VWindow::key(virtual_key, ascii_key)) {
		return TRUE;
	}
	else if ((getParent() != 0) && !isTopLevel()) {
		return getParent()->key(virtual_key, ascii_key);
	}
	else {
		return FALSE;
	}
}

// ==========================================================================
// ::: Panner Support
// ==========================================================================

void VScrollWindow::createPanner()
{
	if (!haveOwnPanner) {
		VUtilityWindow *panner_box =
			new VUtilityWindow((VScrollWindow *)this, StyleHidden);
		panner_box->setTitle("Panner");
		panner_box->setCloseBehavior(VUtilityWindow::HideOnClose);
		panner_box->uponClose(this, methodOf(VScrollWindow, pannerClosed));
		panner_box->uponMouseDbl(this, methodOf(VScrollWindow, pannerMouseDbl));
		VPanner *panner = new VPanner(VFrame(0, 0, 1.0F, 1.0F),
									  panner_box, StyleBorder + Style3D);
		haveOwnPanner = 1;
		if (haveManyPanners) {
			thePannerList->insertAt(0, panner);
		}
		else {
			((VScrollWindow *)this)->thePanner = panner;
		}
		panner_box->frameWindow();
		int x, y, w, h, pw, ph;
		getAbs(&x, &y, &w, &h);
		panner_box->sizeOfWin(&pw, &ph);
		panner_box->setGeometryPosition(x + w - pw, y + h - ph);
		panner->uponChange
			(this, methodOf(VScrollWindow, pannerCallback));
		panner->uponEndChange
			(this, methodOf(VScrollWindow, pannerEndCallback));
		updatePanner(panner, TRUE);
	}
}

// --------------------------------------------------------------------------
//
VUtilityWindow *VScrollWindow::getPannerBox() const
{
	VPanner *panner = getPanner();
	return (VUtilityWindow *)getPanner()->getParent()->getParent();
}

// --------------------------------------------------------------------------
//
VPanner *VScrollWindow::getPanner() const
{
	if (!haveOwnPanner) {
		((VScrollWindow *)this)->createPanner();
	}
	if (haveManyPanners) {
		return (VPanner *)thePannerList->idAt(0);
	}
	else {
		return thePanner;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::showPanner(boolean b)
{
	if (b) {
		if (isPannerHidden()) {
			getPannerBox()->show();
			pannerHidden = 0;
			pannerShown(TRUE);
			paintNub();
		}
	}
	else if (isPannerShowing()) {
		getPannerBox()->hide();
		pannerHidden = 1;
		pannerShown(FALSE);
		paintNub();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::pannerClosed(VObject *)
{
	pannerHidden = 1;
	pannerShown(FALSE);
	paintNub(0);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::pannerShown(boolean)
{
}

// --------------------------------------------------------------------------
//
void VScrollWindow::pannerMouseDbl(VObject *, int, int)
{
	hidePanner();
}

// --------------------------------------------------------------------------
//
void VScrollWindow::disassociatePanner(VPanner *panner)
{
	if (panner != 0) {
		if (haveManyPanners) {
			if (haveOwnPanner && (thePannerList->idAt(0) == panner)) {
				haveOwnPanner = 0;
			}
			thePannerList->removeSame(panner);
		}
		panner->uponChange(0, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::associatePanner(VPanner *panner)
{
	if (panner == 0) {
		return;
	}
	if (haveOwnPanner) {
		if (haveManyPanners) {
			if (panner == thePannerList->idAt(0)) {
				return;
			}
		}
		else if (panner == thePanner) {
			return;
		}
	}
	if (!haveManyPanners) {
		haveManyPanners = 1;
		VPanner *the_panner = thePanner;
		thePannerList = new VOrdCollect;
		if (the_panner != 0) { thePannerList->add(the_panner); }
		thePannerList->add(panner);
	}
	else if (!thePannerList->findSame(panner)) {
		thePannerList->add(panner);
	}
	panner->uponChange(this, methodOf(VScrollWindow, pannerCallback));
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::pannerCallback(long x, long y)
{
	float xoffset, yoffset;
	mapScrollValue(0, x, xoffset);
	mapScrollValue(1, y, yoffset);
	scrollData.scrollTo(this, x, y, xoffset, yoffset);
	return FALSE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::pannerEndCallback()
{
	setHorizontalScrollValues();
	setVerticalScrollValues();
	updatePanner(0, TRUE);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::updatePanner(VPanner *panner, boolean all)
{
	if (panner != 0) {
		if (!panner->isPanning()) {
			if (all) {
				long max_pos_page_x[3], max_pos_page_y[3];
				getScrollValues(0, max_pos_page_x);
				getScrollValues(1, max_pos_page_y);
				panner->setValues(0, max_pos_page_x[0],
									 max_pos_page_x[1],
									 max_pos_page_x[2],
								  0, max_pos_page_y[0],
									 max_pos_page_y[1],
									 max_pos_page_y[2]);
			}
			else {
				panner->setPosition
					(getScrollPosition(0, panner->getHorizontalPageSize()),
					 getScrollPosition(1, panner->getVerticalPageSize()));
			}
		}
	}
	else if (haveManyPanners) {
		DO (*thePannerList, VPanner, panner)
			updatePanner(panner, all);
		END
	}
	else if (thePanner != 0) {
		updatePanner(thePanner, all);
	}
}

// ==========================================================================
// ::: Auto-Scroll Support
// ==========================================================================

#if !defined(CV_NO_AUTO_SCROLL)

// --------------------------------------------------------------------------
//
#include "timer.h"
#include "mouscrsr.h"
#include "bitmap.h"
#include "cv.rc"

// --------------------------------------------------------------------------
//
CLASS AutoScroll : public VObject {
public:
	static void			begin(VScrollWindow *, int, int);
	static void			update(int, int);
	static void			end();
	static void			cleanup();
private:
	enum Constants {
						NeutralZone	= 4
	};
						AutoScroll();
					   ~AutoScroll();
	static int			getTimePerElement(int, int);
	void				timeout(VObject * = 0);
private:
	VScrollWindow	   *scrollWindow;
	int					originX;
	int					originY;
	int					directionX;
	int					directionY;
	long				lastTime;
	long				timeX;
	long				timeY;
	VTimer				timer;
	VMouseCursor	   *cursorNSEW;
	VMouseCursor	   *cursorNS;
	VMouseCursor	   *cursorEW;
	VMouseCursor	   *cursorNE;
	VMouseCursor	   *cursorNW;
	VMouseCursor	   *cursorSE;
	VMouseCursor	   *cursorSW;
	VMouseCursor	   *cursorN;
	VMouseCursor	   *cursorS;
	VMouseCursor	   *cursorE;
	VMouseCursor	   *cursorW;
	VBitMap			   *bitmapNSEW;
	VBitMap			   *bitmapNS;
	VBitMap			   *bitmapEW;
	static AutoScroll  *data;
};

// --------------------------------------------------------------------------
//
AutoScroll *AutoScroll::data = 0;

// --------------------------------------------------------------------------
//
AutoScroll::AutoScroll()
{
	scrollWindow		= 0;
	originX				= 0;
	originY				= 0;
	directionX			= 0;
	directionY			= 0;
	timeX				= 0;
	timeY				= 0;
	lastTime			= 0;
	cursorNSEW			= new VMouseCursor(CvRcPanNsewCursor);
	cursorNS			= new VMouseCursor(CvRcPanNsCursor);
	cursorEW			= new VMouseCursor(CvRcPanEwCursor);
	cursorNE			= new VMouseCursor(CvRcPanNeCursor);
	cursorNW			= new VMouseCursor(CvRcPanNwCursor);
	cursorSE			= new VMouseCursor(CvRcPanSeCursor);
	cursorSW			= new VMouseCursor(CvRcPanSwCursor);
	cursorN				= new VMouseCursor(CvRcPanNCursor);
	cursorS				= new VMouseCursor(CvRcPanSCursor);
	cursorE				= new VMouseCursor(CvRcPanECursor);
	cursorW				= new VMouseCursor(CvRcPanWCursor);
	bitmapNSEW			= new VBitMap(CvRcPanNsewBitmap);
	bitmapNS			= new VBitMap(CvRcPanNsBitmap);
	bitmapEW			= new VBitMap(CvRcPanEwBitmap);
	timer.uponTimeout(this, methodOf(AutoScroll, timeout));
}

// --------------------------------------------------------------------------
//
AutoScroll::~AutoScroll()
{
	delete cursorNSEW;
	delete cursorNS;
	delete cursorEW;
	delete cursorNE;
	delete cursorNW;
	delete cursorSE;
	delete cursorSW;
	delete cursorN;
	delete cursorS;
	delete cursorE;
	delete cursorW;
	delete bitmapNSEW;
	delete bitmapNS;
	delete bitmapEW;
}

// --------------------------------------------------------------------------
//
void AutoScroll::begin(VScrollWindow *w, int x, int y)
{
	if (data == 0) {
		data = new AutoScroll;
	}
	data->scrollWindow	= w;
	data->originX		= x;
	data->originY		= y;
	notifier->captureMouseFor(data->scrollWindow->getViewWindow());
	notifier->mouseTracking(TRUE);
	data->scrollWindow->setCursor(data->cursorNSEW);
}

// --------------------------------------------------------------------------
//
#include "appview.h"
void AutoScroll::update(int x, int y)
{
	int dx = x - data->originX;
	int dy = y - data->originY;

	if (dx < 0) {
		if (-dx <= NeutralZone) {
			data->directionX = 0;
			data->timeX = -1;
		}
		else {
			data->directionX = 1;
			data->timeX = getTimePerElement(0, -dx);
		}
	}
	else if (dx > 0) {
		if (dx <= NeutralZone) {
			data->directionX = 0;
			data->timeX = -1;
		}
		else {
			data->directionX = -1;
			data->timeX = getTimePerElement(0, dx);
		}
	}
	else {
		data->directionX = 0;
		data->timeX = -1;
	}
	if (dy < 0) {
		if (-dy <= NeutralZone) {
			data->directionY = 0;
			data->timeY = -1;
		}
		else {
			data->directionY = 1;
			data->timeY = getTimePerElement(1, -dy);
		}
	}
	else if (dy > 0) {
		if (dy <= NeutralZone) {
			data->directionY = 0;
			data->timeY = -1;
		}
		else {
			data->directionY = -1;
			data->timeY = getTimePerElement(1, dy);
		}
	}
	else {
		data->directionY = 0;
		data->timeY = -1;
	}

	if (data->directionX > 0) {
		if (data->directionY > 0) {
			data->scrollWindow->setCursor(data->cursorNW);
		}
		else if (data->directionY < 0) {
			data->scrollWindow->setCursor(data->cursorSW);
		}
		else {
			data->scrollWindow->setCursor(data->cursorW);
		}
	}
	else if (data->directionX < 0) {
		if (data->directionY > 0) {
			data->scrollWindow->setCursor(data->cursorNE);
		}
		else if (data->directionY < 0) {
			data->scrollWindow->setCursor(data->cursorSE);
		}
		else {
			data->scrollWindow->setCursor(data->cursorE);
		}
	}
	else if (data->directionY > 0) {
		data->scrollWindow->setCursor(data->cursorN);
	}
	else if (data->directionY < 0) {
		data->scrollWindow->setCursor(data->cursorS);
	}
	else {
		data->scrollWindow->setCursor(data->cursorNSEW);
		data->timer.stop();
		return;
	}
//	data->lastTime = clock();
	data->timeout();
	data->timer.start(10);
}

// --------------------------------------------------------------------------
//
void AutoScroll::end()
{
	data->timer.stop();
	notifier->mouseTracking(FALSE);
	notifier->releaseCapture();
	data->scrollWindow->setCursor(0);
}

// --------------------------------------------------------------------------
//
int AutoScroll::getTimePerElement(int d, int p)
{
	static int first_time = 1;
	static int screen_size[2];

	if (first_time) {
		first_time = 0;
		notifier->getScreenSize(&screen_size[0], &screen_size[1]);
		screen_size[0] = int(screen_size[0] * 0.4F);
		screen_size[1] = int(screen_size[1] * 0.4F);
	}

	const long max_time	= 500;	// milliseconds
	const long min_time	=  10;	// milliseconds

	if (p < 0) {
		p = -p;
	}
	if (p <= NeutralZone) {
		return 0;
	}
	else if (p >= screen_size[d]) {
		return min_time;
	}
	else {
		return int(((1.0F - (float(p) / float(screen_size[d]))) *
					(max_time - min_time)) + 0.5F + min_time);
	}
}

// --------------------------------------------------------------------------
//
#include "str.h"
void AutoScroll::timeout(VObject *)
{
	clock_t current_time = 0 /*clock()*/;
long timey = timeY;
	if ((timeX > 0) && ((timeX -= current_time - lastTime) < 0)) {
		timeX = 0;
	}
	if ((timeY > 0) && ((timeY -= current_time - lastTime) < 0)) {
		timeY = 0;
	}
	if ((timeX == 0) || (timeY == 0)) {
		performedAutoScroll = TRUE;
		scrollWindow->scroll(timeX == 0 ? directionX : 0,
							 timeY == 0 ? directionY : 0);
	}
//	lastTime = clock();
VString s;s.prints("CURTIME:%ld TIMEY:%ld -> %ld LASTIME:%ld",current_time,timey,timeY,lastTime);OutputDebugString(s.gets());
	if ((timeX == 0) || (timeY == 0)) {
s.prints("    SCROLL!");OutputDebugString(s.gets());
	}
}

// --------------------------------------------------------------------------
//
void AutoScroll::cleanup()
{
	delete data; data = 0;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::beginAutoScroll(int x, int y)
{
	currentlyAutoScrolling = TRUE;
	performedAutoScroll = FALSE;
	AutoScroll::begin(this, x, y);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::autoScroll(int x, int y)
{
	AutoScroll::update(x, y);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::endAutoScroll()
{
	AutoScroll::end();
	currentlyAutoScrolling = FALSE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::destroyAutoScrollData()
{
	AutoScroll::cleanup();
}

#else

// --------------------------------------------------------------------------
//
void beginAutoScroll(int, int)	{}
void autoScroll(int, int)		{}
void endAutoScroll()			{}
void destroyAutoScrollData()	{}

#endif

// ==========================================================================
// ::: VScrollWindow::Manager
// ==========================================================================

void VScrollWindow::Manager::getPreferredSize(int *w, int *h)
{
	VScrollWindow *sw = (VScrollWindow *)getWindow();
	if (sw != 0) { sw->getPreferredSize(w, h); }
	addNonClientAreaSize(w, h);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::Manager::getConstrainedSize(int *, int *, int, int)
{
	return FALSE;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::Manager::getMinimumSize(int *w, int *h)
{
	VScrollWindow *sw = (VScrollWindow *)getWindow();
	if (sw != 0) { sw->getMinimumSize(w, h); }
	addNonClientAreaSize(w, h);
}

// ==========================================================================
// ::: VScrollWindow::ViewWindow
// ==========================================================================

VScrollWindow::ViewWindow::ViewWindow(VScrollWindow *scrollwindow)
	: VWindow()
{
	setGeoManager(new VGeoWindow());
	setRedrawUponResize(FALSE);
	if (scrollwindow->haveViewBorder) {
		(*this)(VFrame(), scrollwindow, StyleBorder + Style3D);
	}
	else {
		(*this)(VFrame(), scrollwindow, StyleNoBorder);
	}
	clipToChildren(TRUE);
	setBackground(&VColorScheme::systemBrush(VColorScheme::FlatBackground));
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ViewWindow::erased()
{
	VScrollWindow *sw = (VScrollWindow *)getParent();
	if (sw->workWindow == 0) {
		if (sw->eraseViewAreaBackground &&
			sw->eraseViewableWorkAreaBackground) {
			return VWindow::erased();
		}
		else {
			return TRUE;
		}
	}
	else if (sw->isHorizontalWorkAreaStretchable() &&
			 sw->isVerticalWorkAreaStretchable()) {
		return TRUE;
	}
	else {
		return VWindow::erased();
	}
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ViewWindow::paint()
{
	int left, top, right, bottom, width, height;

	notifier->getExposedRectangle(&left, &top, &right, &bottom);
	if (((width = right - left) <= 0) || ((height = bottom - top) <= 0)) {
		return TRUE;
	}

	VScrollWindow *sw = ((VScrollWindow *)getParent());

	// Paint the portion of the view-area not occupied by the work-area,
	// or the paint the background of the viewable work-area if desired;
	// note that if both of these areas are to be painted by us, then
	// it will be done all at once in erased(), not here.

	if (sw->eraseViewAreaBackground) {
		if (!sw->eraseViewableWorkAreaBackground) {
			if (sw->getViewAreaPixelHeight() >
				sw->getViewableWorkAreaPixelHeight()) {
				thePort->openOn(this);
				thePort->useBrush(getBackground());
				theRectangle->set(CornerDim,
								  0,
								  sw->getViewableWorkAreaPixelHeight(),
								  sw->getViewAreaPixelWidth(),
								  sw->getViewAreaPixelHeight() -
								  sw->getViewableWorkAreaPixelHeight());
				thePort->fillRegion(theRectangle, FALSE);
				if (sw->getViewAreaPixelWidth() >
					sw->getViewableWorkAreaPixelWidth()) {
					theRectangle->set(CornerDim,
									  sw->getViewableWorkAreaPixelWidth(),
									  0,
									  sw->getViewAreaPixelWidth() -
									  sw->getViewableWorkAreaPixelWidth(),
									  sw->getViewAreaPixelHeight());
					thePort->fillRegion(theRectangle, FALSE);
				}
				thePort->close();
			}
			else if (sw->getViewAreaPixelWidth() >
					 sw->getViewableWorkAreaPixelWidth()) {
				thePort->openOn(this);
				thePort->useBrush(getBackground());
				theRectangle->set(CornerDim,
								  sw->getViewableWorkAreaPixelWidth(),
								  0,
								  sw->getViewAreaPixelWidth() -
								  sw->getViewableWorkAreaPixelWidth(),
								  sw->getViewableWorkAreaPixelHeight());
				thePort->fillRegion(theRectangle, FALSE);
				thePort->close();
			}
		}
	}
	else if (sw->eraseViewableWorkAreaBackground) {
		if (thePort->openOn(this)) {
			thePort->useBrush(getBackground());
			theRectangle->set(Corners, left, top, right, bottom);
			thePort->fillRegion(theRectangle, FALSE);
			thePort->close();
		}
	}

	// Allow the client to paint the work-area.

	long unit_x, unit_y; int unit_w, unit_h;
	if (sw->mapToCells(left, top, width, height,
					   unit_x, unit_y, unit_w, unit_h)) { 
		sw->paintWorkArea(this, unit_x, unit_y, unit_w, unit_h);
	}

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::resized(int, int)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::frameChildren(int, int, boolean)
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::mouseEnter(int b)
{
	return ((VScrollWindow *)getParent())->mouseEnter(b);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::mouseExit(int b)
{
	return ((VScrollWindow *)getParent())->mouseExit(b);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::mouseDn(int x, int y)
{
	return ((VScrollWindow *)getParent())->mouseDn(x, y);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::mouseUp(int x, int y)
{
	return ((VScrollWindow *)getParent())->mouseUp(x, y);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::mouseMv(int x, int y, int b)
{
	return ((VScrollWindow *)getParent())->mouseMv(x, y, b);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::mouseDbl(int x, int y)
{
	return ((VScrollWindow *)getParent())->mouseDbl(x, y);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::rmouseDn(int x, int y)
{
	return ((VScrollWindow *)getParent())->rmouseDn(x, y);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ViewWindow::mmouseDn(int x, int y)
{
	return ((VScrollWindow *)getParent())->mmouseDn(x, y);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ViewWindow::mmouseUp(int x, int y)
{
	return ((VScrollWindow *)getParent())->mmouseUp(x, y);
}

// --------------------------------------------------------------------------
//
boolean	VScrollWindow::ViewWindow::key(int virtual_key, char ascii_key)
{
	return ((VScrollWindow *)getParent())->key(virtual_key, ascii_key);
}

// ==========================================================================
// ::: VScrollWindow::ConfigurationMenu
// ==========================================================================

VPopupMenu *VScrollWindow::newConfigurationMenu() const
{
	return new ConfigurationMenu((VScrollWindow *)this);
}

// --------------------------------------------------------------------------
//
VScrollWindow::ConfigurationMenu::ConfigurationMenu(VScrollWindow *sw)
	: VPopupMenu(VMenuData("&Options", "",
						   methodOf(ConfigurationMenu, poppedUp), this))
{
	scrollWindow = sw;

	addMenuItem(VMenuData
		("&Panner", "",
		 methodOf(ConfigurationMenu, menuPanner), this, 1000));

#if 1
	addMenuItem(VMenuData());
	addMenuItem(VMenuData
		("&Smooth", "",
		 methodOf(ConfigurationMenu, menuSmooth), this, 1001));
	addMenuItem(VMenuData
		("&Maximize", "",
		 methodOf(ConfigurationMenu, menuMaximize), this, 1002));
	addMenuItem(VMenuData());
#endif

	VPopupMenu *popup1 =
		new VPopupMenu(VMenuData("&Scrollbars", "",
								 methodOf(ConfigurationMenu,
								 		  poppedUpScrolls), this, 2000));
	addMenuItem(VMenuData(popup1));
	addMenuItem(VMenuData());
	addMenuItem(VMenuData("&Debug", "",
						  methodOf(ConfigurationMenu, menuDebug), this));

	popup1->addMenuItem(VMenuData
		("&None", "",
		 methodOf(ConfigurationMenu, menuScrollsNone), this, 1000));
	popup1->addMenuItem(VMenuData());
	VPopupMenu *popup2 =
		new VPopupMenu(VMenuData("&Horizontal", "",
								  methodOf(ConfigurationMenu,
								  		   poppedUpHorizontalScroll),
								 this));
	popup1->addMenuItem(VMenuData(popup2));
	popup2->addMenuItem(VMenuData
		("&None", "",
		 methodOf(ConfigurationMenu,
		 		  menuHorizontalScrollNone), this, 1000));
	popup2->addMenuItem(VMenuData());
	popup2->addMenuItem(VMenuData
		("&Top", "",
		 methodOf(ConfigurationMenu,
		 		  menuHorizontalScrollPosition), this, 2000));
	popup2->addMenuItem(VMenuData
		("&Bottom", "",
		 methodOf(ConfigurationMenu,
		 		  menuHorizontalScrollPosition), this, 3000));
	popup2->addMenuItem(VMenuData());
	popup2->addMenuItem(VMenuData
		("&Track", "",
		 methodOf(ConfigurationMenu,
		 		  menuHorizontalScrollTrack), this, 4000));

	VPopupMenu *popup3 = new VPopupMenu(VMenuData
		("&Vertical", "",
		 methodOf(ConfigurationMenu,
		 		  poppedUpVerticalScroll), this));
	popup1->addMenuItem(VMenuData(popup3));
	popup3->addMenuItem(VMenuData
		("&None", "",
		 methodOf(ConfigurationMenu,
		 		  menuVerticalScrollNone), this, 1000));
	popup3->addMenuItem(VMenuData());
	popup3->addMenuItem(VMenuData
		("&Left", "",
		 methodOf(ConfigurationMenu,
		 		  menuVerticalScrollPosition), this, 2000));
	popup3->addMenuItem(VMenuData
		("&Right", "",
		 methodOf(ConfigurationMenu,
		 		  menuVerticalScrollPosition), this, 3000));
	popup3->addMenuItem(VMenuData());
	popup3->addMenuItem(VMenuData
		("&Track", "",
		 methodOf(ConfigurationMenu,
		 		  menuVerticalScrollTrack), this, 4000));
	popup1->addMenuItem(VMenuData());
	popup1->addMenuItem(VMenuData
		("&Track", "",
		 methodOf(ConfigurationMenu,
		 		  menuScrollsTrack), this, 2000));
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::poppedUp(VPopupMenu *pm)
{
	if ((pm != 0) && (scrollWindow != 0)) {
		pm->checkMenuItem(1000, scrollWindow->isPannerShowing());
#if 1
		pm->checkMenuItem
			(1001, scrollWindow->isHorizontalSmoothScrolling() &&
				   scrollWindow->isVerticalSmoothScrolling());
		pm->checkMenuItem
			(1002, scrollWindow->isMaximizeViewableWorkAreaWidth() &&
				   scrollWindow->isMaximizeViewableWorkAreaHeight());
#endif
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::poppedUpScrolls(VPopupMenu *pm)
{
	if ((pm != 0) && (scrollWindow != 0)) {
		pm->checkMenuItem(1000, !scrollWindow->hasHorizontalScrollBar() &&
								!scrollWindow->hasVerticalScrollBar());
		pm->checkMenuItem(2000, scrollWindow->isHorizontalScrollBarTracking() &&
								scrollWindow->isVerticalScrollBarTracking());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::poppedUpHorizontalScroll(VPopupMenu *pm)
{
	if ((pm != 0) && (scrollWindow != 0)) {
		pm->checkMenuItem(1000, !scrollWindow->hasHorizontalScrollBar());
		pm->checkMenuItem(2000, scrollWindow->getHorizontalScrollBarPosition()
								== VScrollWindow::Opposite);
		pm->checkMenuItem(3000, scrollWindow->getHorizontalScrollBarPosition()
								== VScrollWindow::Default);
		pm->checkMenuItem(4000, scrollWindow->isHorizontalScrollBarTracking());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::poppedUpVerticalScroll(VPopupMenu *pm)
{
	if ((pm != 0) && (scrollWindow != 0)) {
		pm->checkMenuItem(1000, !scrollWindow->hasVerticalScrollBar());
		pm->checkMenuItem(2000, scrollWindow->getVerticalScrollBarPosition()
								== VScrollWindow::Opposite);
		pm->checkMenuItem(3000, scrollWindow->getVerticalScrollBarPosition()
								== VScrollWindow::Default);
		pm->checkMenuItem(4000, scrollWindow->isVerticalScrollBarTracking());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuPanner(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->showPanner(!scrollWindow->isPannerShowing());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuSmooth(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->setSmoothScrolling
			(!scrollWindow->isHorizontalSmoothScrolling() &&
			 !scrollWindow->isVerticalSmoothScrolling());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuMaximize(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->setMaximizeViewableWorkAreaWidth
			(!scrollWindow->isMaximizeViewableWorkAreaWidth());
		scrollWindow->setMaximizeViewableWorkAreaHeight
			(!scrollWindow->isMaximizeViewableWorkAreaHeight());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuScrollsNone(VMenuItem *)
{
	if (scrollWindow != 0) {
		if (scrollWindow->hasHorizontalScrollBar() ||
			scrollWindow->hasVerticalScrollBar()) {
			scrollWindow->useHorizontalScrollBar(FALSE);
			scrollWindow->useVerticalScrollBar(FALSE);
		}
		else {
			scrollWindow->useHorizontalScrollBar(TRUE);
			scrollWindow->useVerticalScrollBar(TRUE);
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuScrollsTrack(VMenuItem *)
{
	if (scrollWindow != 0) {
		if (scrollWindow->isHorizontalScrollBarTracking() &&
			scrollWindow->isVerticalScrollBarTracking()) {
			scrollWindow->setHorizontalScrollBarTracking(FALSE);
			scrollWindow->setVerticalScrollBarTracking(FALSE);
		}
		else {
			scrollWindow->setHorizontalScrollBarTracking(TRUE);
			scrollWindow->setVerticalScrollBarTracking(TRUE);
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuHorizontalScrollNone(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->
			useHorizontalScrollBar(!scrollWindow->hasHorizontalScrollBar());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::
					menuHorizontalScrollPosition(VMenuItem *mi)
{
	if ((mi != 0) && (scrollWindow != 0)) {
		switch (((VMenuItem *)mi)->getIdNo()) {
		case 2000:
			scrollWindow->
				setHorizontalScrollBarPosition(VScrollWindow::Opposite);
			break;
		case 3000:
			scrollWindow->
				setHorizontalScrollBarPosition(VScrollWindow::Default);
			break;
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuHorizontalScrollTrack(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->setHorizontalScrollBarTracking
			(!scrollWindow->isHorizontalScrollBarTracking());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuVerticalScrollNone(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->
			useVerticalScrollBar(!scrollWindow->hasVerticalScrollBar());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::
					menuVerticalScrollPosition(VMenuItem *mi)
{
	if ((mi != 0) && (scrollWindow != 0)) {
		switch (mi->getIdNo()) {
		case 2000:
			scrollWindow->
				setVerticalScrollBarPosition(VScrollWindow::Opposite);
			break;
		case 3000:
			scrollWindow->
				setVerticalScrollBarPosition(VScrollWindow::Default);
			break;
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuVerticalScrollTrack(VMenuItem *)
{
	if (scrollWindow != 0) {
		scrollWindow->setVerticalScrollBarTracking
			(!scrollWindow->isVerticalScrollBarTracking());
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ConfigurationMenu::menuDebug(VMenuItem *)
{
	if (scrollWindow != 0) {
		VScrollWindow *x = scrollWindow;
		VString s;
		s.prints("workWindow: %s\n"
				 "viewWindow: %s\n"
				 "scrollUnits: %d, %d\n"
				 "viewAreaSize: %d, %d\n"
				 "workAreaUnitSize: %d, %d\n"
				 "viewableWorkAreaUnitSize: %d, %d\n"
				 "viewableWorkAreaUnitPosition: %d, %d\n"
				 "viewableWorkAreaPixelOffset: %d, %d\n"
				 "viewAreaExtraPixelSize: %d, %d\n"
				 "maxViewableWorkAreaUnitPosition: %d, %d\n"
				 "maxViewableWorkAreaPixelOffsetPosition: %d, %d\n"
				 "maximizeViewableWorkArea: %s, %s\n"
				 "smoothScrolling: %s, %s\n"
				 "scrollsNeeded: %s, %s\n",
				 x->workWindow == 0 ?
				 	"null" : x->workWindow->iam()->nameOf(),
				 x->viewWindow == 0 ?
				 	"null" : x->viewWindow->iam()->nameOf(),
				 x->scrollData.unitWH[0],
				 x->scrollData.unitWH[1],
				 x->scrollData.viewAreaPixelWH[0],
				 x->scrollData.viewAreaPixelWH[1],
				 x->scrollData.workAreaUnitWH[0],
				 x->scrollData.workAreaUnitWH[1],
				 x->scrollData.viewableWorkAreaUnitWH[0],
				 x->scrollData.viewableWorkAreaUnitWH[1],
				 x->scrollData.viewableWorkAreaUnitXY[0],
				 x->scrollData.viewableWorkAreaUnitXY[1],
				 x->scrollData.viewableWorkAreaPixelOffsetXY[0],
				 x->scrollData.viewableWorkAreaPixelOffsetXY[1],
				 x->scrollData.viewAreaExtraPixelWH[0],
				 x->scrollData.viewAreaExtraPixelWH[1],
				 x->scrollData.maxViewableWorkAreaUnitXY[0],
				 x->scrollData.maxViewableWorkAreaUnitXY[1],
				 x->scrollData.maxViewableWorkAreaPixelOffsetXY[0],
				 x->scrollData.maxViewableWorkAreaPixelOffsetXY[1],
				 x->scrollData.bit[0].maximizeViewableWorkArea ?
				 	"true" : "false",
				 x->scrollData.bit[1].maximizeViewableWorkArea ?
				 	"true" : "false",
				 x->scrollData.bit[0].smoothScrolling ?
				 	"true" : "false",
				 x->scrollData.bit[1].smoothScrolling ?
				 	"true" : "false",
				 x->scrollData.bit[0].scrollNeeded ?
				 	"true" : "false",
				 x->scrollData.bit[1].scrollNeeded ?
				 	"true" : "false");
		VReport::dialog("VScrollWindow Debug", scrollWindow, s.gets());
	}
}

// ===========================================================================
// ::: Archiver Support
// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VScrollWindow::putTo(VArchiver& a)
{
	VScrollWindow::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VScrollWindow::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VWindow::putTo(a, FALSE);

	// Write the object version tag.

	a << VScrollWindowArchiveTag;

	// Write any object specific data.

	// ToDo ...

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VScrollWindow::getFrom(VArchiver& a)
{
	// Read the base object data.

	VWindow::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VScrollWindowArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VScrollWindowArchiveTag));
		return;
	}

	// Read any object specific data.

	// ToDo ...
}

// ---------------------------------------------------------------------------
//
void VScrollWindow::getFrom(VArchiver& a,
							VObject *parentwin, boolean do_realize)
{
	VScrollWindow::getFrom(a);

	if (do_realize) {
		VScrollWindow::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VScrollWindow::realize(VWindow *parentwin)
{
	VWindow::realize(parentwin);

	construct();
}

#endif // !CV_NOARCHIVER
