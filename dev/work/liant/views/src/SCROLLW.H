//	scrollw.h -- [UNDER DEVELOPMENT]
//
//	VScrollWindow interface [Common]
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

#ifndef SCROLLW_H
#define SCROLLW_H

// --------------------------------------------------------------------------
//
#include "window.h"
#include "geoform.h"
#include "popupmen.h"
#include "scrollbr.h"
#include <stddef.h>

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VFrame;
CLASS	VGeoForm;
CLASS	VMenuItem;
CLASS	VObject;
CLASS	VOrdCollect;
CLASS	VPanner;
CLASS	VPopupMenu;
CLASS	VScrollBar;
CLASS	VString;
CLASS	VStyle;
CLASS	VUtilityWindow;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// --------------------------------------------------------------------------
//
#ifdef Opposite
#undef Opposite
#endif

// --------------------------------------------------------------------------
// VScrollWindow provides the ability for scrolling of a large "work-area"
// within its "view-area".  Two basic kinds of scrolling are provided for:
//
//	1. Window scrolling.
//	   In this case, the work-area is simply defined by a single work-window
//	   which must be provided by the client with setWorkWindow() which will
//	   be automatically scrolled upon request within a view-window, which
//	   essentially acts as a clippping window.
//	2. Virtual scrolling.
//	   In this case, a view-window is automatically provided which acts
//	   as a canvas upon which the appropriate area of a virtual work-area
//	   will be painted by the client; the client must override the virtual
//	   methods getWorkAreaSize() and paintWorkArea().
//
// Scrolling occurs in units, which may be dynamically defined in either
// dimension as follows:
//
//	1. Pixel-unit scrolling.
//	   In this case, the work-area is scrolled one pixel at a time.
//	   This is the default; it can be set explicitly by passing 1 to any
//	   any of setHorizontalUnits(), setVerticalUnits(), setUnits().
//	2. Fixed-unit scrolling.
//	   In this case, the work-area is scrolled a specified fixed number
//	   of pixels at a time.  This can be set by passing a value greater
//	   than 1 to any of setHorizontalUnits(), setVerticalUnits(), setUnits().
//	3. Variable-unit scrolling.
//	   In this case, the work-area is scrolled a variable number of pixels
//	   at a time depending on the size in pixels of a specific cell within
//	   the work-area; this size is obtained dynamically via the virtual
//	   methods getWorkAreaColumnWidth() and getWorkAreaRowHeight().
//	   This can be set by passing a 0 to any of setHorizontalUnits(),
//	   setVerticalUnits(), setUnitSize().
//
// In addition, the number of units scrolled upon a scrollbar manipulation,
// may be set.
//
CLASS VScrollWindow : public VWindow {

public:
	enum Loc {		Default		= 0,	Opposite	= 1,
					Bottom		= 0,	Top			= 1,
					Right		= 0,	Left		= 1 };

	enum Dim {		Horizontal	=  0,
					Vertical	=  1,
					Both		= -1 };

public:
	CLASS WorkArea {
		void		getWorkAreaSize(long *, long *) const;
		int			getWorkAreaRowHeight(long, void **) const;
		int			getWorkAreaColumnWidth(long, void **) const;
		void		paintWorkArea(VWindow *, long, long, int, int);
	};

public:
					VScrollWindow(const VFrame&, VWindow *,
								  const VStyle& = StyleDefault);
					VScrollWindow(VWindow *,
								  const VStyle& = StyleDefault);
					VScrollWindow(const VString&,
								  const VFrame&, VWindow *,
								  const VStyle& = StyleDefault);
					VScrollWindow(const VString&,
								  VWindow *,
								  const VStyle& = StyleDefault);
					VScrollWindow();
				   ~VScrollWindow();
	VClass		   *iam();

	void			operator()(const VFrame&, VWindow *);
	void			operator()(const VFrame&, VWindow *, const VStyle&);
	void			operator()(const VFrame&, VWindow *, const VStyle&,
							   const VString&);

	// Set/get work-window (for simple/single VWindow scrolling).

	void			setWorkWindow(VWindow *);
	VWindow		   *getWorkWindow() const;

	void			setHorizontalWorkAreaStretchable(boolean);
	void			setVerticalWorkAreaStretchable(boolean);
	boolean			isHorizontalWorkAreaStretchable() const;
	boolean			isVerticalWorkAreaStretchable() const;

	// Get view-window (for dynamic/virtual scrolling).

	VWindow		   *getViewWindow() const;

	// Set/get units -- pixel (1), fixed-unit (>1), variable-unit (0).

	void			setUnits(int);
	void			setUnits(int, int);
	void			getUnits(int *, int *) const;
	void			getUnits(int&, int&) const;
	void			setHorizontalUnits(int);
	void			setVerticalUnits(int);
	int				getHorizontalUnits() const;
	int				getVerticalUnits() const;

	void			setVariableUnitScrolling();
	void			setFixedUnitScrolling(int);
	void			setPixelUnitScrolling();
	void			setHorizontalVariableUnitScrolling();
	void			setHorizontalFixedUnitScrolling(int);
	void			setHorizontalPixelUnitScrolling();
	void			setVerticalVariableUnitScrolling();
	void			setVerticalFixedUnitScrolling(int);
	void			setVerticalPixelUnitScrolling();

	boolean			isHorizontalVariableUnitScrolling() const;
	boolean			isHorizontalFixedUnitScrolling() const;
	boolean			isHorizontalPixelUnitScrolling() const;
	boolean			isVerticalVariableUnitScrolling() const;
	boolean			isVerticalFixedUnitScrolling() const;
	boolean			isVerticalPixelUnitScrolling() const;

	// Scrolling (positive is to the right, negative is to the left)

	void			scroll(long unit_dx, long unit_dy);
	void			scrollHorizontal(long unit_dx);
	void			scrollVertical(long unit_dx);

	void			scrollTo(long unit_x, long unit_y);
	void			scrollHorizontalTo(long unit_x);
	void			scrollVerticalTo(long unit_y);

	void			scrollTo(long unit_x, long unit_y,
							 int pixel_offset_x, int pixel_offset_y);
	void			scrollHorizontalTo(long unit_x, int pixel_offset_x);
	void			scrollVerticalTo(long unit_y, int pixel_offset_y);

	void			scrollPixel(long pixel_dx, long pixel_dy);
	void			scrollHorizontalPixel(long pixel_dx);
	void			scrollVerticalPixel(long pixel_dy);

	boolean			scrollRatioTo(float, float);
	boolean			scrollHorizontalRatioTo(float);
	boolean			scrollVerticalRatioTo(float);

	void			scrollPageUp(long page_dx = 1, long page_dy = 1);
	void			scrollHorizontalPageUp(long page_dx = 1);
	void			scrollVerticalPageUp(long page_dy = 1);

	void			scrollPageDown(long page_dx = 1, long page_dy = 1);
	void			scrollHorizontalPageDown(long page_dx = 1);
	void			scrollVerticalPageDown(long page_dy = 1);

	void			scrollLineUp(long line_dx = 1, long line_dy = 1);
	void			scrollHorizontalLineUp(long line_dx = 1);
	void			scrollVerticalLineUp(long line_dy = 1);

	void			scrollLineDown(long line_dx = 1, long line_dy = 1);
	void			scrollHorizontalLineDown(long line_dx = 1);
	void			scrollVerticalLineDown(long line_dy = 1);

	void			scrollHome();
	void			scrollHorizontalHome();
	void			scrollVerticalHome();

	void			scrollEnd();
	void			scrollHorizontalEnd();
	void			scrollVerticalEnd();

	void			scrollIntoView(long unit_x, long unit_y);
	void			scrollHorizontalIntoView(long unit_x);
	void			scrollVerticalIntoView(long unit_y);

	void			endScroll();

	// Scrollbar.

	VScrollBar	   *getHorizontalScrollBar() const;
	VScrollBar	   *getVerticalScrollBar() const;

	void			setHorizontalScrollBar(VScrollBar *);
	void			setVerticalScrollBar(VScrollBar *);

	// Set/get scrollbar location.

	void			setHorizontalScrollBarPosition(int);
	void			setVerticalScrollBarPosition(int);
	int				getHorizontalScrollBarPosition() const;
	int				getVerticalScrollBarPosition() const;

	// Set/get view-window margin and view-window/scrollbar spacing gap.

	void			setHorizontalMargin(int);
	int				getHorizontalMargin() const;
	void			setVerticalMargin(int);
	int				getVerticalMargin() const;

	void			setHorizontalScrollBarGap(int);
	int				getHorizontalScrollBarGap() const;
	void			setVerticalScrollBarGap(int);
	int				getVerticalScrollBarGap() const;

	// Enable/disable scrollbar use.

	void			useHorizontalScrollBar(boolean);
	void			useVerticalScrollBar(boolean);
	boolean			hasHorizontalScrollBar() const;
	boolean			hasVerticalScrollBar() const;

	void			setHideScrollsWhenNotNeeded(boolean = TRUE);
	boolean			isHideScrollsWhenNotNeeded() const;

	boolean			isHorizontalScrollNeeded() const;
	boolean			isVerticalScrollNeeded() const;

	// Set/get scrollbar visibility.

	void			hideScrollBars(boolean = TRUE);
	void			showScrollBars(boolean = TRUE);
	void			hideHorizontalScrollBar(boolean = TRUE);
	void			hideVerticalScrollBar(boolean = TRUE);
	void			showHorizontalScrollBar(boolean = TRUE);
	void			showVerticalScrollBar(boolean = TRUE);
	boolean			isHorizontalScrollBarShowing() const;
	boolean			isVerticalScrollBarShowing() const;
	boolean			isHorizontalScrollBarHidden() const;
	boolean			isVerticalScrollBarHidden() const;

	// Get/set scrollbar increment values.

	void			setHorizontalScrollBarLineIncrement(int);
	int				getHorizontalScrollBarLineIncrement() const;
	void			setVerticalScrollBarLineIncrement(int);
	int				getVerticalScrollBarLineIncrement() const;

	void			setHorizontalScrollBarPageIncrement(int);
	int				getHorizontalScrollBarPageIncrement() const;
	void			setVerticalScrollBarPageIncrement(int);
	int				getVerticalScrollBarPageIncrement() const;

	// Enable/disable live scrollbar tracking.

	void			setScrollBarTracking(boolean);
	void			setHorizontalScrollBarTracking(boolean);
	boolean			isHorizontalScrollBarTracking() const;
	void			setVerticalScrollBarTracking(boolean);
	boolean			isVerticalScrollBarTracking() const;

	// Enable/disable maximization of viewable work-area size.

	void			setMaximizeViewableWorkArea(boolean = TRUE);
	void			setMaximizeViewableWorkAreaWidth(boolean = TRUE);
	void			setMaximizeViewableWorkAreaHeight(boolean = TRUE);
	boolean			isMaximizeViewableWorkAreaWidth() const;
	boolean			isMaximizeViewableWorkAreaHeight() const;

	// Smooth scrolling.

	void			setSmoothScrolling(boolean = TRUE);
	void			setHorizontalSmoothScrolling(boolean = TRUE);
	void			setVerticalSmoothScrolling(boolean = TRUE);
	boolean			isHorizontalSmoothScrolling() const;
	boolean			isVerticalSmoothScrolling() const;

	// Set/get scrolling origin.

	void			setHorizontalScrollOrigin(int);
	void			setVerticalScrollOrigin(int);
	int				getHorizontalScrollOrigin() const;
	int				getVerticalScrollOrigin() const;

	// Viewable work-area cell (unit) to view-area pixel geometry.

	int				isCellViewable(long unit_x, long unit_y) const;
	int				getCellX(long unit_x, long unit_y) const;
	int				getCellY(long unit_x, long unit_y) const;
	int				getCellWidth(long unit_x, long unit_y) const;
	int				getCellHeight(long unit_x, long unit_y) const;
	boolean			getCellPosition(long unit_x, long unit_y,
									int& pixel_x, int& pixel_y) const;
	boolean			getCellPosition(long unit_x, long unit_y,
									int *pixel_x, int *pixel_y) const;
	boolean			getCellSize(long unit_x, long unit_y,
								int& pixel_w, int& pixel_h) const;
	boolean			getCellSize(long unit_x, long unit_y,
								int *pixel_w, int *pixel_h) const;
	boolean			getCellGeometry(long unit_x, long unit_y,
									int& pixel_x, int& pixel_y,
									int& pixel_w, int& pixel_h) const;
	boolean			getCellGeometry
						(long unit_x, long unit_y,
						 int *pixel_x, int *pixel_y,
						 int *pixel_w, int *pixel_h) const;
	boolean			getCellRangeGeometry
						(long unit_x, long unit_y, int unit_w, int unit_h,
						 int *pixel_x, int *pixel_y,
						 int *pixel_w, int *pixel_h) const;
	boolean			getCellRangeGeometry
						(long unit_x, long unit_y, int unit_w, int unit_h,
						 int& pixel_x, int& pixel_y,
						 int& pixel_w, int& pixel_h) const;

	// Viewable work-area cell (unit) geometry.

	long			getViewableWorkAreaUnitX() const;
	long			getViewableWorkAreaUnitY() const;
	void			getViewableWorkAreaUnitPosition(long& unit_x,
													long& unit_y) const;
	void			getViewableWorkAreaUnitPosition(long *unit_x,
													long *unit_y) const;
	int				getViewableWorkAreaUnitWidth() const;
	int				getViewableWorkAreaUnitHeight() const;
	void			getViewableWorkAreaUnitSize(int& unit_x,
												int& unit_y) const;
	void			getViewableWorkAreaUnitSize(int *unit_x,
												int *unit_y) const;
	void			getViewableWorkAreaUnitGeometry
						(long& unit_x, long& unit_y,
						 int& unit_w, int& unit_h) const;
	void			getViewableWorkAreaUnitGeometry
						(long *unit_x, long *unit_y,
						 int *unit_w, int *unit_h) const;
	void			getViewableWorkAreaRatio
						(float&, float&, float&, float&) const;
	void			getViewableWorkAreaRatio
						(float *, float *, float *, float *) const;
	long			getMaxViewableWorkAreaUnitX() const;
	long			getMaxViewableWorkAreaUnitY() const;

	// Work-area unit geometry.

	long			getWorkAreaUnitWidth() const;
	long			getWorkAreaUnitHeight() const;
	void			getWorkAreaUnitSize(long&, long&) const;
	void			getWorkAreaUnitSize(long *, long *) const;

	// Work-area unit (cell) range to pixel size mapping.

	int				getWorkAreaPixelWidth(long, long) const;
	int				getWorkAreaPixelHeight(long, long) const;
	void			getWorkAreaPixelSize
						(long, long, long, long, int&, int&) const;
	void			getWorkAreaPixelSize
						(long, long, long, long, int *, int *) const;

	// Viewable work-area pixel geometry.

	int				getViewableWorkAreaPixelOffsetX() const;
	int				getViewableWorkAreaPixelOffsetY() const;
	void			getViewableWorkAreaPixelOffsetPosition
						(int&, int&) const;
	void			getViewableWorkAreaPixelOffsetPosition
						(int *, int *) const;

	int				getViewableWorkAreaPixelX() const;
	int				getViewableWorkAreaPixelY() const;
	void			getViewableWorkAreaPixelPosition(int&, int&) const;
	void			getViewableWorkAreaPixelPosition(int *, int *) const;
	int				getViewableWorkAreaPixelWidth() const;
	int				getViewableWorkAreaPixelHeight() const;
	void			getViewableWorkAreaPixelSize(int&, int&) const;
	void			getViewableWorkAreaPixelSize(int *, int *) const;
	void			getViewableWorkAreaPixelGeometry
						(int&, int&, int&, int&) const;
	void			getViewableWorkAreaPixelGeometry
						(int *, int *, int *, int *) const;

	// View-area pixel size.

	int				getViewAreaPixelWidth() const;
	int				getViewAreaPixelHeight() const;
	void			getViewAreaPixelSize(int&, int&) const;
	void			getViewAreaPixelSize(int *, int *) const;

	// View-area pixel to work-area unit (cell) geometry mapping.

	boolean			mapToCell(int x, int y, long&, long&) const;
	boolean			mapToCell(int x, int y, long * = 0, long * = 0) const;
	boolean			mapToCells(int x, int y, int w, int h,
							   long&, long&, int&, int&) const;
	boolean			mapToCells(int x, int y, int w, int h,
							   long * = 0, long * = 0,
							   int * = 0, int * = 0) const;
	// Row/column data.

	void		   *getViewableWorkAreaColumnData(long) const;
	void		   *getViewableWorkAreaRowData(long) const;

	// Updates.

	virtual void	updateView();
	void			updateWorkArea(const VRectangle *);
	void			updateScrollBars();
	void			setEraseViewAreaBackground(boolean);
	boolean			getEraseViewAreaBackground() const;
	void			setEraseViewableWorkAreaBackground(boolean);
	boolean			getEraseViewableWorkAreaBackground() const;
	void			setPaintNub(boolean);
	boolean			getPaintNub() const;

	// AutoScroll.

	boolean			isAutoScrolling() const;

	// Panner Control.

	void			usePanner(boolean = TRUE);
	boolean			hasPanner() const;
	VUtilityWindow *getPannerBox() const;
	VPanner		   *getPanner() const;
	void			showPanner(boolean = TRUE);
	void			hidePanner(boolean = TRUE);
	boolean			isPannerShowing() const;
	boolean			isPannerHidden() const;
	void			associatePanner(VPanner *);
	void			disassociatePanner(VPanner *);

	// Configuration popup-menu.

	virtual VPopupMenu *newConfigurationMenu() const;
	void				useConfigurationMenu(boolean = TRUE);
	boolean				hasConfigurationMenu() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	virtual void		getWorkAreaSize(long *, long *) const;
	virtual int			getWorkAreaRowHeight(long, void **) const;
	virtual int			getWorkAreaColumnWidth(long, void **) const;
	virtual void		paintWorkArea(VWindow *, long, long, int, int);
	virtual void		scrolledHorizontal(long, int);
	virtual void		scrolledVertical(long, int);
	virtual void		pannerShown(boolean);
	virtual VWindow	   *newViewWindow() const;
	virtual VScrollBar *newHorizontalScrollBar() const;
	virtual VScrollBar *newVerticalScrollBar() const;

public:

	CLASS ViewWindow : public VWindow {
	public:
						ViewWindow(VScrollWindow *);
	protected:
		virtual boolean	erased();
		virtual boolean	paint();
		virtual boolean	resized(int, int);
		virtual boolean	frameChildren(int, int, boolean = FALSE);
		virtual boolean	mouseEnter(int);
		virtual boolean	mouseExit(int);
		virtual boolean	mouseDn(int, int);
		virtual boolean	mouseUp(int, int);
		virtual boolean	mouseMv(int, int, int);
		virtual boolean	mouseDbl(int, int);
		virtual boolean	mmouseDn(int, int);
		virtual boolean	mmouseUp(int, int);
		virtual boolean	rmouseDn(int, int);
		virtual boolean	key(int, char);
	private:
		friend			VScrollWindow;
	};

	CLASS Manager : public VGeoForm {
	public:
						Manager() { }
		virtual void	getPreferredSize(int *, int *);
		virtual void	getMinimumSize(int *, int *);
		virtual boolean	getConstrainedSize(int *, int *, int, int);
	};

	CLASS ConfigurationMenu : public VPopupMenu {
	public:
						ConfigurationMenu(VScrollWindow *);
	private:
		void			poppedUp(VPopupMenu *);
		void			poppedUpScrolls(VPopupMenu *);
		void			poppedUpHorizontalScroll(VPopupMenu *);
		void			poppedUpVerticalScroll(VPopupMenu *);
		void			menuPanner(VMenuItem * = 0);
		void			menuSmooth(VMenuItem * = 0);
		void			menuMaximize(VMenuItem * = 0);
		void			menuScrollsNone(VMenuItem * = 0);
		void			menuScrollsTrack(VMenuItem * = 0);
		void			menuHorizontalScrollNone(VMenuItem * = 0);
		void			menuHorizontalScrollPosition(VMenuItem * = 0);
		void			menuHorizontalScrollTrack(VMenuItem * = 0);
		void			menuVerticalScrollNone(VMenuItem * = 0);
		void			menuVerticalScrollPosition(VMenuItem * = 0);
		void			menuVerticalScrollTrack(VMenuItem * = 0);
		void			menuDebug(VMenuItem * = 0);
	private:
		VScrollWindow  *scrollWindow;
	};

public:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// This cache is used to contain information concerning the work-area,
	// its size, position, and its viewable area in units and pixels, and
	// the size in pixels of any insufficient space (i.e. where only a
	// partial unit is viewable).
	//
	// Here are the meanings of the important methods:
	//
	// o getUnits()
	//
	//   Returns the horizontal/vertical unit size.  If this is zero, then
	//   variable-unit scrolling is in effect in which case unit sizes will
	//   be obtained dynamically by querying the client using the virtual
	//   VScrollWindow methods getHorizontal/VerticalUnitSize() for each
	//   unit which comes into view.  Otherwise, if this is greater then
	//   zero, then pixel-unit or fixed-unit scrolling is in effect.
	//
	// o isMaximizeViewableWorkArea()
	//
	//   Returns TRUE if the width/height of the viewiable work-area should
	//   be automatically maximized.
	//
	// o isSmoothScrolling()
	//
	//   Returns TRUE if the work-area should be scrolled as smoothly
	//	 as possible; this is always TRUE for pixel-unit scrolling.
	//
	// o isOppositeScrollOrigin()
	//
	//   Returns TRUE if the the origin of the work-area/view-area is the
	//   opposite of the default.
	//
	// o getWorkAreaPixelOffsetXY()
	//
	//   Returns the number of work-area-units currently scrolled out of
	//   range, i.e. which are not viewable unless they were scrolled into
	//	 range.  This is typically the number of units to the above or to
	//   the left of the view-area, unless the opposite-origin attribute is
	//   set.  In other words, this value is the index of the work-area unit
	//   currently at the top/left (or right/bottom if the opposite-origin
	//   attribute is set) of the view-area.  These values are manipulated
	//   by the end-user through scoll-bars and panners, and using the
	//   VScrollWindow methods scroll(), scrollHorizontal(), scrollVertical()
	//   scrollTo(), scrollHorizontalTo(), and scrollVerticalTo().
	//
	// o getWorkAreaUnitWH()
	//
	//   Returns the size in units of the entire work-area.  This value is
	//   obtained using the VScrollWindow virtual method getWorkAreaSize(),
	//   and is cached until it is explicitly invalidated using the
	//   VScrollWindow method updateWorkArea().
	//
	// o getViewAreaPixelWH()
	//
	//   Returns the size in pixels of the view-area.
	//
	// o getViewableWorkAreaUnitWH()
	//
	//   Returns the size in units of the work-area which is currently
	//   viewable within the view-area.  Note that this *does* include
	//   partially viewable units.
	//
	// o getViewableWorkAreaPixelWH()
	//
	//   Returns the size in pixels of the work-area which is currently
	//   viewable within the view-area.  This value is necessarily
	//   between zero and the the size of the view-area inclusive.
	//
	// o getViewAreaExtraPixelWH()
	//
	//   Returns the size in pixels of any space within the view-area
	//   which is not currently occupied by work-area units.  As a
	//   special case, if this value is negative its absolute value
	//   indicates the number of pixels not viewable in a partially
	//   viewable unit.
	//
	// This cache should be updated when:
	//
	// 1. The view-area is resized (and created).
	// 2. The work-area is scrolled.
	// 3. The programmer explicitly requests it,
	//	  presumably because the work-area changed.
	//
	// One main point of this is to minimize the number of queries to the
	// client or "model" concerning the number and sizes of cells in the
	// work-area and calculations concerning the sumation of the cell sizes.
	//
	CLASS ScrollData {
	public:
						ScrollData();
					   ~ScrollData();

		void			update(VScrollWindow *);
		void			resize(VScrollWindow *, int *, int *, int *);

		void			scroll(VScrollWindow *,
							   long, long, float = -1.0F, float = -1.0F);
		void			scrollTo(VScrollWindow *,
								 long, long, float = -1.0F, float = -1.0F);
		void			scrollHorizontalTo
							(VScrollWindow *, long, float = -1.0F);
		void			scrollVerticalTo
							(VScrollWindow *, long, float = -1.0F);
		void			scrollLine(VScrollWindow *, long, long);
		void			scrollPage(VScrollWindow *, long, long);
		void			scrollHome(VScrollWindow *, boolean, boolean);
		void			scrollEnd(VScrollWindow *, boolean, boolean);
		boolean			scrollRatioTo(VScrollWindow *, float, float);
		boolean			scrollHorizontalRatioTo(VScrollWindow *, float);
		boolean			scrollVerticalRatioTo(VScrollWindow *, float);
		void			scrollPixel(VScrollWindow *, long, long);
		void			scrollIntoView(VScrollWindow *, int, long *);

		void			setUnits(VScrollWindow *, int, int);
		void			setMaximizeViewableWorkArea
							(VScrollWindow *, int, boolean);
		void			setSmoothScrolling
							(VScrollWindow *, int, boolean);
		void			setOppositeScrollOrigin
							(VScrollWindow *, int, boolean);

		int				getUnits(int) const;
		boolean			isMaximizeViewableWorkArea(int) const;
		boolean			isSmoothScrolling(int) const;
		boolean			isOppositeScrollOrigin(int) const;
		boolean			isScrollNeeded(int) const;

		int				getViewAreaPixelWH(int) const;
		long			getWorkAreaUnitWH(int) const;
		long			getWorkAreaPixelWH(const VScrollWindow *,
										   int, long, long) const;
		long			getViewableWorkAreaUnitXY(int) const;
		int				getViewableWorkAreaPixelOffsetXY(int) const;
		int				getViewableWorkAreaUnitWH(int) const;
		int				getViewableWorkAreaPixelWH(int) const;
		void			getViewableWorkAreaRatio(float&, float&,
												 float&, float&) const;
		int				getViewAreaExtraPixelWH(int) const;
		long			getMaxViewableWorkAreaUnitXY(int) const;
		long			getMaxViewableWorkAreaPixelOffsetXY(int) const;

		boolean			getCellGeometry(long, long,
										int&, int&, int&, int&) const;
		boolean			getCellRangeGeometry(long, long, int, int,
											 int&, int&, int&, int&) const;
		boolean			mapToCell(int, int, long&, long&) const;
		boolean			mapToCells(int, int, int, int,
								   long&, long&, int&, int&) const;

		int				retrieveUnitSize	
							(const VScrollWindow *, int, int) const;
		int				retrieveVariableUnitSize
							(const VScrollWindow *, int, int) const;
		int				retrieveUnitSize	
							(const VScrollWindow *, int, int, void **) const;
		int				retrieveVariableUnitSize
							(const VScrollWindow *, int, int, void **) const;
		void			setMaxViewableWorkArea(VScrollWindow *, int = -1);
		int				setScrollPixelOffsetData(VScrollWindow *, int, long);
		void			getScrollPixelData
							(const VScrollWindow *, int, long, long&, int&);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// UnitData is used to contain information about a variable size
		// work-area unit, specifically its size; in the future there
		// may also be a hidden state.
		//
	public:
		struct UnitData {
			void	   *data;
			short		size;
		};

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// UnitDataArray is used to contain information about each variable
		// size work-area unit currently viewable (even partially) within
		// the view-area; viewable includes partially viewable.
		//
	public:
		CLASS UnitDataArray {
		public:
						UnitDataArray();
					   ~UnitDataArray();
			void		clear();
			void		append(int, void * = 0);
			void		prepend(int, void * = 0);
			void		prepend(const UnitDataArray&);
			void		first(int, int = 0);
			void		truncate(int);
			int			count() const;
			UnitData&	get(int) const;
			int			size(int) const;
			void	   *data(int) const;
			int			find(int) const;
			int			sum() const;
			int			sum(int) const;
			int			sum(int, int) const;
			void		set(int, int, void * = 0);
			void		setSize(int, int);
			void		setData(int, void *);
			void		incrementSize(int, int);
		protected:
			enum Const  { InitialArraySize	 = 32,
						  IncrementalArraySize = 32 };
		protected:
			int			allocateUnitDataArray();
			int			incrementUnitDataArraySize();
			void		sumup();
		private:
			UnitData   *unitDataArray;
			short		unitDataArraySize;
			short		firstUnitDataIndex;
			short		lastUnitDataIndex;
			short		sumOfUnitSizes;
		};

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// These are needed for the various types of scrolling,
		// pixel-unit (P), fixed-unit (F), variable-unit (V), smooth (S).
		// 54 bytes.
		//
	private:
		long				workAreaUnitWH[2];					 // PFV
		long				viewableWorkAreaUnitXY[2];			 // PFV
		long				maxViewableWorkAreaUnitXY[2];		 // PFV
		short				maxViewableWorkAreaPixelOffsetXY[2]; // PFV(S)
		short				viewAreaPixelWH[2];					 // PFV
		short				viewAreaExtraPixelWH[2];			 // PFV
		short				unitWH[2];							 // PFV
		short				viewableWorkAreaPixelOffsetXY[2];	 //  FV
		short				viewableWorkAreaUnitWH[2];			 //  FV
		UnitDataArray	   *viewableWorkAreaUnitSizes[2];		 //   V
		struct {
			unsigned char	maximizeViewableWorkArea : 1;		 // PFV
			unsigned char	smoothScrolling : 1;				 //  FV
			unsigned char	oppositeScrollOrigin : 1;			 // PFV
			unsigned char	scrollNeeded : 1;					 // PFV
		} bit[2];

	private:
		friend				VScrollWindow;
		friend				ViewWindow;
		friend				ConfigurationMenu;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver&);
	virtual void		getFrom(VArchiver&);
	virtual void		getFrom(VArchiver&, VObject *, boolean = TRUE);
	virtual void		realize(VWindow *);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	enum Constant {		MaxScrollValue = 32766 };

public:
	virtual void		getPreferredSize(int *, int *) const;
	virtual void		getMinimumSize(int *, int *) const;
protected:
    virtual boolean		mouseEnter(int);
    virtual boolean		mouseExit(int);
    virtual boolean		mouseDn(int, int);
    virtual boolean		mouseMv(int, int, int);
    virtual boolean		mouseUp(int, int);
    virtual boolean		mouseDbl(int, int);
    virtual boolean		rmouseDn(int, int);
    virtual boolean		mmouseDn(int, int);
    virtual boolean		mmouseUp(int, int);
    virtual boolean		mouseWheel(int, int, int, int);
	virtual boolean		key(int, char);
	virtual boolean		paint();
	virtual boolean		resized(int, int);
	virtual VWindow	   *getClientAreaWindow() const;
protected:
	void				initialize();
	void				construct();
	void				createHorizontalScroll();
	void				createVerticalScroll();
	void				createView();
	virtual void		createPanner();
	void				updateConfiguration(int = -1, int = -1);
	void				updateLayout(int = -1, int = -1);
	void				updatePanner(VPanner * = 0, boolean = FALSE);
	void				pannerClosed(VObject * = 0);
	void				pannerMouseDbl(VObject * = 0, int = 0, int = 0);
	void				paintNub(int = -1);
	void				getNubGeometry(int&, int&, int&, int&) const;
	int					getHorizontalScrollThickness() const;
	int					getVerticalScrollThickness() const;
	void				getScrollValues(int, long[3]) const;
	long				getScrollPosition(int, long) const;
	void				setHorizontalScrollValues();
	void				setVerticalScrollValues();
	void				performScroll(int, int);
	void				mapScrollValue(int, long&, float&);
	void				horizontalScrollCallback(long);
	void				horizontalScrollEndCallback();
	boolean				horizontalScrollLineCallback(int);
	boolean				horizontalScrollPageCallback(int);
	void				verticalScrollCallback(long);
	void				verticalScrollEndCallback();
	boolean				verticalScrollLineCallback(int);
	boolean				verticalScrollPageCallback(int);
	void				retrieveWorkAreaSize(long&, long&) const;
	boolean				pannerCallback(long, long);
	void				pannerEndCallback();
	void				beginAutoScroll(int, int);
	void				autoScroll(int, int);
	void				endAutoScroll();
	static void			destroyAutoScrollData();
protected:

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver&, boolean);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section; please KEEP OUT!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
public:
	ScrollData			scrollData;
	struct {
		VScrollBar	   *scrollBar;
		short			lineIncrement;
		short			pageIncrement;
		unsigned char	marginThickness;
		unsigned char	scrollBarGapThickness;
		unsigned short	haveScrollBar : 1;
		unsigned short	manageScrollBar : 1;
		unsigned short	scrollBarHidden : 1;
		unsigned short	oppositeScrollBarPosition : 1;
		unsigned short	scrollBarTracking : 1;
		unsigned short	scrollingDisabled : 1;
		unsigned short	stretchWorkArea : 1;
		unsigned short	showScrollBarEvenWhenNotNeeded : 1;
		unsigned short	inScrollBarCallback : 1;
		unsigned short	ignoreOneScrollBarCallback : 1;
		unsigned short	scaleScrollValues : 1;
	}					dimData[2];
	VWindow			   *workWindow;
	VWindow			   *viewWindow;
	union {
		VPanner		   *thePanner;
		VOrdCollect	   *thePannerList;
	};
	unsigned short		haveViewBorder : 1;
	unsigned short		useTheConfigurationMenu : 1;
	unsigned short		workWindowHasBeenSized : 1;
	unsigned short		useThePanner : 1;
	unsigned short		haveManyPanners : 1;
	unsigned short		haveOwnPanner : 1;
	unsigned short		pannerHidden : 1;
	unsigned short		drawPannerIndicator : 1;
	unsigned short		lookingForFirstResized : 1;
	unsigned short		firstResized : 1;
	unsigned short		eraseViewAreaBackground : 1;
	unsigned short		eraseViewableWorkAreaBackground : 1;
	unsigned short		autoScrollEnabled : 1;
	unsigned short		currentlyAutoScrolling : 1;

private:
	friend				ViewWindow;
	friend				ScrollData;
	friend				ConfigurationMenu;
};

extern VClass *VScrollWindowCls;

// --------------------------------------------------------------------------
//
inline void VScrollWindow::ScrollData::UnitDataArray::clear()
{
	firstUnitDataIndex = lastUnitDataIndex = sumOfUnitSizes = 0;
}

inline int VScrollWindow::ScrollData::UnitDataArray::count() const
{
	if (lastUnitDataIndex > firstUnitDataIndex) {
		return lastUnitDataIndex - firstUnitDataIndex;
	}
	else if (lastUnitDataIndex < firstUnitDataIndex)  {
		return unitDataArraySize - (firstUnitDataIndex - lastUnitDataIndex);
	}
	else if (lastUnitDataIndex != 0) {
		return unitDataArraySize;
	}
	else {
		return 0;
	}
}

inline int VScrollWindow::ScrollData::UnitDataArray::sum() const
{
	return sumOfUnitSizes;
}

inline int VScrollWindow::ScrollData::UnitDataArray::size(int index) const
{
	return get(index).size;
}

inline void *VScrollWindow::ScrollData::UnitDataArray::data(int index) const
{
	return get(index).data;
}

// --------------------------------------------------------------------------
//
inline int VScrollWindow::ScrollData::getUnits(int d) const
{
	return unitWH[d];
}

inline long VScrollWindow::ScrollData::getWorkAreaUnitWH(int d) const
{
	return workAreaUnitWH[d];
}

inline int VScrollWindow::ScrollData::getViewableWorkAreaPixelWH(int d) const
{
	return viewAreaExtraPixelWH[d] <= 0 ?
		   viewAreaPixelWH[d] :
		   viewAreaPixelWH[d] - viewAreaExtraPixelWH[d];
}

inline long VScrollWindow::ScrollData::getViewableWorkAreaUnitXY(int d) const
{
	return viewableWorkAreaUnitXY[d];
}

inline int VScrollWindow::ScrollData::
						  getViewableWorkAreaPixelOffsetXY(int d) const
{
	return viewableWorkAreaPixelOffsetXY[d];
}

inline int VScrollWindow::ScrollData::getViewableWorkAreaUnitWH(int d) const
{
	return unitWH[d] == 1 ?
		   getViewableWorkAreaPixelWH(d) : viewableWorkAreaUnitWH[d];
}

inline int VScrollWindow::ScrollData::getViewAreaPixelWH(int d) const
{
	return viewAreaPixelWH[d];
}

inline int VScrollWindow::ScrollData::getViewAreaExtraPixelWH(int d) const
{
	return viewAreaExtraPixelWH[d];
}

inline long VScrollWindow::ScrollData::
						   getMaxViewableWorkAreaUnitXY(int d) const
{
	return maxViewableWorkAreaUnitXY[d];
}

inline long VScrollWindow::ScrollData::
						   getMaxViewableWorkAreaPixelOffsetXY(int d) const
{
	return maxViewableWorkAreaPixelOffsetXY[d];
}

inline boolean VScrollWindow::ScrollData::
							  isMaximizeViewableWorkArea(int d) const
{
	return bit[d].maximizeViewableWorkArea;
}

inline int VScrollWindow::ScrollData::isSmoothScrolling(int d) const
{
	return bit[d].smoothScrolling;
}

inline int VScrollWindow::ScrollData::isOppositeScrollOrigin(int d) const
{
	return bit[d].oppositeScrollOrigin;
}

inline boolean VScrollWindow::ScrollData::isScrollNeeded(int d) const
{
	return bit[d].scrollNeeded;
}

inline void VScrollWindow::ScrollData
						 ::scrollTo(VScrollWindow *sw,
						   			long unit_x, long unit_y,
									float offset_x, float offset_y)
{
	scroll(sw, viewableWorkAreaUnitXY[0] - unit_x,
			   viewableWorkAreaUnitXY[1] - unit_y, offset_x, offset_y);
}

inline void VScrollWindow::ScrollData
						 ::scrollHorizontalTo(VScrollWindow *sw,
						   					  long unit_x, float offset_x)
{
	scroll(sw, viewableWorkAreaUnitXY[0] - unit_x, 0, offset_x, -1.0F);
}

inline void VScrollWindow::ScrollData
						 ::scrollVerticalTo(VScrollWindow *sw,
						   					long unit_y, float offset_y)
{
	scroll(sw, 0, viewableWorkAreaUnitXY[1] - unit_y, -1.0F, offset_y);
}

inline int VScrollWindow::ScrollData
						::retrieveVariableUnitSize(const VScrollWindow *sw,
								  				   int d, int unit) const
{
	// ToDo!!! Cache non-viewable-but-adjacent work-area unit sizes,
	//		   as well as perhaps other important unit sizes,
	//		   like the last one or the last page worth.

	int size;

	if (unit < viewableWorkAreaUnitXY[d]) {
		size = d == 0 ?
			   ((VScrollWindow *)sw)->getWorkAreaColumnWidth(unit, 0) :
			   ((VScrollWindow *)sw)->getWorkAreaRowHeight(unit, 0);
	}
	else if (unit >= viewableWorkAreaUnitXY[d] +
					 viewableWorkAreaUnitSizes[d]->count()) {
		size = d == 0 ?
			   ((VScrollWindow *)sw)->getWorkAreaColumnWidth(unit, 0) :
			   ((VScrollWindow *)sw)->getWorkAreaRowHeight(unit, 0);
	}
	else {
		size = viewableWorkAreaUnitSizes[d]->
				size(unit - viewableWorkAreaUnitXY[d]);
	}
	if (size < 0) {
		//
		// Special case; for variable-unit scrolling, if the queried
		// unit size is negative then it is dynamically set to the
		// current size of the entire view-area.
		//
		return viewAreaPixelWH[d];
	}
	else {
		return size;
	}
}

inline int VScrollWindow::ScrollData
						::retrieveUnitSize(const VScrollWindow *sw,
						  				   int d, int unit) const
{
	if (unitWH[d] <= 0) {
		return retrieveVariableUnitSize(sw, d, unit);
	}
	else if (unitWH[d] > 1) {
		return unitWH[d];
	}
	else {
		return 1;
	}
}

inline int VScrollWindow::ScrollData	
						::retrieveVariableUnitSize(const VScrollWindow *sw,
								  				   int d, int unit,
												   void **data) const
{
	// ToDo!!! Cache non-viewable-but-adjacent work-area unit sizes,
	//		   as well as perhaps other important unit sizes,
	//		   like the last one or the last page worth.

	int size;

	if (unit < viewableWorkAreaUnitXY[d]) {
		size = d == 0 ?
			   ((VScrollWindow *)sw)->getWorkAreaColumnWidth(unit, data) :
			   ((VScrollWindow *)sw)->getWorkAreaRowHeight(unit, data);
	}
	else if (unit >= viewableWorkAreaUnitXY[d] +
					 viewableWorkAreaUnitSizes[d]->count()) {
		size = d == 0 ?
			   ((VScrollWindow *)sw)->getWorkAreaColumnWidth(unit, data) :
			   ((VScrollWindow *)sw)->getWorkAreaRowHeight(unit, data);
	}
	else {
		*data = viewableWorkAreaUnitSizes[d]->
				data(unit - viewableWorkAreaUnitXY[d]);
		size = viewableWorkAreaUnitSizes[d]->
				size(unit - viewableWorkAreaUnitXY[d]);
	}
	if (size < 0) {
		//
		// Special case; for variable-unit scrolling, if the queried
		// unit size is negative then it is dynamically set to the
		// current size of the entire view-area.
		//
		return viewAreaPixelWH[d];
	}
	else {
		return size;
	}
}

inline int VScrollWindow::ScrollData
						::retrieveUnitSize(const VScrollWindow *sw,
						  				   int d, int unit,
										   void **data) const
{
	if (unitWH[d] <= 0) {
		return retrieveVariableUnitSize(sw, d, unit, data);
	}
	else if (unitWH[d] > 1) {
		return unitWH[d];
	}
	else {
		return 1;
	}
}

// --------------------------------------------------------------------------
//
inline VWindow *VScrollWindow::getWorkWindow() const
{
	return workWindow;
}

inline VWindow *VScrollWindow::getViewWindow() const
{
	return viewWindow;
}

inline void VScrollWindow::getUnits(int *hunits, int *vunits) const
{
	if (hunits != 0) { *hunits = scrollData.getUnits(0); }
	if (vunits != 0) { *vunits = scrollData.getUnits(1); }
	
}

inline void VScrollWindow::getUnits(int& hunits, int& vunits) const
{
	hunits = scrollData.getUnits(0);
	vunits = scrollData.getUnits(1);
}

inline int VScrollWindow::getHorizontalUnits() const
{
	return scrollData.getUnits(0);
}

inline int VScrollWindow::getVerticalUnits() const
{
	return scrollData.getUnits(1);
}

inline boolean VScrollWindow::isHorizontalVariableUnitScrolling() const
{
	return scrollData.getUnits(0) == 0;
}

inline boolean VScrollWindow::isHorizontalFixedUnitScrolling() const
{
	return scrollData.getUnits(0) > 0;
}

inline boolean VScrollWindow::isHorizontalPixelUnitScrolling() const
{
	return scrollData.getUnits(0) == 1;
}

inline boolean VScrollWindow::isVerticalVariableUnitScrolling() const
{
	return scrollData.getUnits(0) == 0;
}

inline boolean VScrollWindow::isVerticalFixedUnitScrolling() const
{
	return scrollData.getUnits(0) > 0;
}

inline boolean VScrollWindow::isVerticalPixelUnitScrolling() const
{
	return scrollData.getUnits(0) == 1;
}

inline void VScrollWindow::scrollTo(long x, long y)
{
	scroll(getViewableWorkAreaUnitX() - x, getViewableWorkAreaUnitY() - y);
}

inline void VScrollWindow::scrollHorizontalTo(long x)
{
	scrollHorizontal(getViewableWorkAreaUnitX() - x);
}

inline void VScrollWindow::scrollVerticalTo(long y)
{
	scrollVertical(getViewableWorkAreaUnitY() - y);
}

inline void VScrollWindow::scrollTo(long x, long y,
									int offset_x, int offset_y)
{
	if (!dimData[0].scrollingDisabled) {
		if (!dimData[1].scrollingDisabled) {
			scrollData.scroll(this, getViewableWorkAreaUnitX() - x,
									getViewableWorkAreaUnitY() - y,
									offset_x < 0 ? 0.0F : float(offset_x),
									offset_y < 0 ? 0.0F : float(offset_y));
		}
		else {
			scrollData.scroll(this, getViewableWorkAreaUnitX() - x, 0,
									offset_x < 0 ? 0.0F : float(offset_x),
									-1.0F);
		}
	}
	else if (!dimData[1].scrollingDisabled) {
		scrollData.scroll(this, 0, getViewableWorkAreaUnitY() - y,
								-1.0F,
								offset_y < 0 ? 0.0F : float(offset_y));
	}
}

inline void VScrollWindow::scrollHorizontalTo(long x, int offset_x)
{
	if (!dimData[0].scrollingDisabled) {
		scrollData.scroll(this, getViewableWorkAreaUnitX() - x, 0,
								offset_x < 0 ? 0.0F : float(offset_x),
								-1.0F);
	}
}

inline void VScrollWindow::scrollVerticalTo(long y, int offset_y)
{
	if (!dimData[1].scrollingDisabled) {
		scrollData.scroll(this, getViewableWorkAreaUnitY() - y, 0,
								offset_y < 0 ? 0.0F : float(offset_y),
								-1.0F);
	}
}

inline void VScrollWindow::updateView()
{
	scrollData.update(this);
	if (workWindow != 0) { workWindow->update(); }
	if (viewWindow != 0) { viewWindow->update(); }
}

inline void VScrollWindow::updateScrollBars()
{
	setHorizontalScrollValues();
	setVerticalScrollValues();
}

inline void VScrollWindow::setEraseViewAreaBackground(boolean b)
{
	if (b) {
		if (!eraseViewAreaBackground) {
			eraseViewAreaBackground = 1;
			if (viewWindow != 0) { viewWindow->update(); }
		}
	}
	else if (eraseViewAreaBackground) {
		eraseViewAreaBackground = 0;
		if (viewWindow != 0) { viewWindow->update(); }
	}
}

inline boolean VScrollWindow::getEraseViewAreaBackground() const
{
	return eraseViewAreaBackground;
}

inline void VScrollWindow::setEraseViewableWorkAreaBackground(boolean b)
{
	if (b) {
		if (!eraseViewableWorkAreaBackground) {
			eraseViewableWorkAreaBackground = 1;
			if (viewWindow != 0) { viewWindow->update(); }
		}
	}
	else if (eraseViewableWorkAreaBackground) {
		eraseViewableWorkAreaBackground = 0;
		if (viewWindow != 0) { viewWindow->update(); }
	}
}

inline boolean VScrollWindow::getEraseViewableWorkAreaBackground() const
{
	return eraseViewableWorkAreaBackground;
}

inline boolean VScrollWindow::getPaintNub() const
{
	return drawPannerIndicator;
}

inline int VScrollWindow::getHorizontalMargin() const
{
	return dimData[0].marginThickness;
}

inline int VScrollWindow::getVerticalMargin() const
{
	return dimData[1].marginThickness;
}

inline int VScrollWindow::getHorizontalScrollBarGap() const
{
	return dimData[1].scrollBarGapThickness;
}

inline int VScrollWindow::getVerticalScrollBarGap() const
{
	return dimData[0].scrollBarGapThickness;
}

inline int VScrollWindow::getHorizontalScrollBarLineIncrement() const
{
	return dimData[0].lineIncrement;
}

inline int VScrollWindow::getVerticalScrollBarLineIncrement() const
{
	return dimData[1].lineIncrement;
}

inline int VScrollWindow::getHorizontalScrollBarPageIncrement() const
{
	return dimData[0].pageIncrement;
}

inline int VScrollWindow::getVerticalScrollBarPageIncrement() const
{
	return dimData[1].pageIncrement;
}

inline boolean VScrollWindow::hasHorizontalScrollBar() const
{
	return dimData[0].haveScrollBar && (dimData[0].scrollBar != 0);
}

inline boolean VScrollWindow::hasVerticalScrollBar() const
{
	return dimData[1].haveScrollBar && (dimData[1].scrollBar != 0);
}

inline boolean VScrollWindow::isHorizontalScrollNeeded() const
{
	return scrollData.isScrollNeeded(0);
}

inline boolean VScrollWindow::isVerticalScrollNeeded() const
{
	return scrollData.isScrollNeeded(1);
}

inline int VScrollWindow::getHorizontalScrollBarPosition() const
{
	return dimData[0].oppositeScrollBarPosition ? Top : Bottom;
}

inline int VScrollWindow::getVerticalScrollBarPosition() const
{
	return dimData[1].oppositeScrollBarPosition ? Left : Right;
}

inline int VScrollWindow::getHorizontalScrollOrigin() const
{
	return scrollData.isOppositeScrollOrigin(0) ? Top : Bottom;
}

inline int VScrollWindow::getVerticalScrollOrigin() const
{
	return scrollData.isOppositeScrollOrigin(1) ? Top : Bottom;
}

inline VScrollBar *VScrollWindow::getHorizontalScrollBar() const
{
	return hasHorizontalScrollBar() ? dimData[0].scrollBar : 0;
}

inline VScrollBar *VScrollWindow::getVerticalScrollBar() const
{
	return hasVerticalScrollBar() ? dimData[1].scrollBar : 0;
}

inline boolean VScrollWindow::isHideScrollsWhenNotNeeded() const
{
	return !dimData[0].showScrollBarEvenWhenNotNeeded;
}

inline void VScrollWindow::showScrollBars(boolean b)
{
	hideScrollBars(!b);
}

inline void VScrollWindow::showHorizontalScrollBar(boolean b)
{
	hideHorizontalScrollBar(!b);
}

inline void VScrollWindow::showVerticalScrollBar(boolean b)
{
	hideVerticalScrollBar(!b);
}

inline boolean VScrollWindow::isHorizontalScrollBarShowing() const
{
	return hasHorizontalScrollBar() && !dimData[0].scrollBarHidden;
}

inline boolean VScrollWindow::isVerticalScrollBarShowing() const
{
	return hasVerticalScrollBar() && !dimData[1].scrollBarHidden;
}

inline boolean VScrollWindow::isHorizontalScrollBarHidden() const
{
	return !isHorizontalScrollBarShowing();
}

inline boolean VScrollWindow::isVerticalScrollBarHidden() const
{
	return !isVerticalScrollBarShowing();
}

inline long VScrollWindow::getWorkAreaUnitWidth() const
{
	return scrollData.getWorkAreaUnitWH(0);
}

inline long VScrollWindow::getWorkAreaUnitHeight() const
{
	return scrollData.getWorkAreaUnitWH(1);
}

inline void VScrollWindow::getWorkAreaUnitSize(long& w, long& h) const
{
	w = scrollData.getWorkAreaUnitWH(0);
	h = scrollData.getWorkAreaUnitWH(1);
}

inline void VScrollWindow::getWorkAreaUnitSize(long *w, long *h) const
{
	if (w != 0) { *w = scrollData.getWorkAreaUnitWH(0); }
	if (h != 0) { *h = scrollData.getWorkAreaUnitWH(1); }
}

inline int VScrollWindow::getWorkAreaPixelWidth(long unit_x,
												long unit_w) const
{
	return scrollData.getWorkAreaPixelWH(this, 0, unit_x, unit_w);
}

inline int VScrollWindow::getWorkAreaPixelHeight(long unit_y,
												 long unit_h) const
{
	return scrollData.getWorkAreaPixelWH(this, 1, unit_y, unit_h);
}

inline void VScrollWindow::getWorkAreaPixelSize(long unit_x, long unit_y,
												long unit_w, long unit_h,
												int *w, int *h) const
{
	if (w != 0) {
		*w = scrollData.getWorkAreaPixelWH(this, 0, unit_x, unit_w);
	}
	if (h != 0) {
		*h = scrollData.getWorkAreaPixelWH(this, 1, unit_y, unit_h);
	}
}

inline void VScrollWindow::getWorkAreaPixelSize(long unit_x, long unit_y,
												long unit_w, long unit_h,
												int& w, int& h) const
{
	w = scrollData.getWorkAreaPixelWH(this, 0, unit_x, unit_w);
	h = scrollData.getWorkAreaPixelWH(this, 1, unit_y, unit_h);
}

inline long VScrollWindow::getViewableWorkAreaUnitX() const
{
	return scrollData.getViewableWorkAreaUnitXY(0);
}

inline long VScrollWindow::getViewableWorkAreaUnitY() const
{
	return scrollData.getViewableWorkAreaUnitXY(1);
}

inline void VScrollWindow::
			getViewableWorkAreaUnitPosition(long& x, long& y) const
{
	x = scrollData.getViewableWorkAreaUnitXY(0);
	y = scrollData.getViewableWorkAreaUnitXY(1);
}

inline void VScrollWindow::
			getViewableWorkAreaUnitPosition(long *x, long *y) const
{
	if (x != 0) { *x = scrollData.getViewableWorkAreaUnitXY(0); }
	if (y != 0) { *y = scrollData.getViewableWorkAreaUnitXY(1); }
}

inline int VScrollWindow::getViewableWorkAreaUnitWidth() const
{
	return scrollData.getViewableWorkAreaUnitWH(0);
}

inline int VScrollWindow::getViewableWorkAreaUnitHeight() const
{
	return scrollData.getViewableWorkAreaUnitWH(1);
}

inline void VScrollWindow::getViewableWorkAreaUnitSize(int& w, int& h) const
{
	w = getViewableWorkAreaUnitWidth();
	h = getViewableWorkAreaUnitHeight();
}

inline void VScrollWindow::getViewableWorkAreaUnitSize(int *w, int *h) const
{
	if (w != 0) { *w = getViewableWorkAreaUnitWidth(); }
	if (h != 0) { *h = getViewableWorkAreaUnitHeight(); }
}

inline void VScrollWindow::
			getViewableWorkAreaUnitGeometry(long& x, long& y,
											int& w, int& h) const
{
	x = getViewableWorkAreaUnitX();
	y = getViewableWorkAreaUnitY();
	w = getViewableWorkAreaUnitWidth();
	h = getViewableWorkAreaUnitHeight();
}

inline void VScrollWindow::
			getViewableWorkAreaUnitGeometry(long *x, long *y,
											int *w, int *h) const
{
	if (x != 0) { *x = getViewableWorkAreaUnitX(); }
	if (y != 0) { *y = getViewableWorkAreaUnitY(); }
	if (w != 0) { *w = getViewableWorkAreaUnitWidth(); }
	if (h != 0) { *h = getViewableWorkAreaUnitHeight(); }
}

inline void VScrollWindow::getViewableWorkAreaRatio(float& x, float& y,
													float& w, float& h) const
{
	scrollData.getViewableWorkAreaRatio(x, y, w, h);
}

inline void VScrollWindow::getViewableWorkAreaRatio(float *x, float *y,
													float *w, float *h) const
{
	float dummy;
	if (x == 0) { x = &dummy; }
	if (y == 0) { y = &dummy; }
	if (w == 0) { w = &dummy; }
	if (h == 0) { h = &dummy; }
	scrollData.getViewableWorkAreaRatio(*x, *y, *w, *h);
}

inline int VScrollWindow::getViewableWorkAreaPixelOffsetX() const
{
	return scrollData.getViewableWorkAreaPixelOffsetXY(0);
}

inline int VScrollWindow::getViewableWorkAreaPixelOffsetY() const
{
	return scrollData.getViewableWorkAreaPixelOffsetXY(1);
}

inline void VScrollWindow::
		    getViewableWorkAreaPixelOffsetPosition(int& x, int& y) const
{
	x = scrollData.getViewableWorkAreaPixelOffsetXY(0);
	y = scrollData.getViewableWorkAreaPixelOffsetXY(1);
}

inline void VScrollWindow::
		    getViewableWorkAreaPixelOffsetPosition(int *x, int *y) const
{
	if (x != 0) { *x = scrollData.getViewableWorkAreaPixelOffsetXY(0); }
	if (y != 0) { *y = scrollData.getViewableWorkAreaPixelOffsetXY(1); }
}

inline int VScrollWindow::getViewableWorkAreaPixelX() const
{
	return scrollData.isOppositeScrollOrigin(0) &&
		   (scrollData.getViewAreaExtraPixelWH(0) > 0) ? 
			scrollData.getViewAreaExtraPixelWH(0) : 0;
}

inline int VScrollWindow::getViewableWorkAreaPixelY() const
{
	return scrollData.isOppositeScrollOrigin(1) &&
		   (scrollData.getViewAreaExtraPixelWH(1) > 0) ? 
			scrollData.getViewAreaExtraPixelWH(1) : 0;
}

inline int VScrollWindow::getViewableWorkAreaPixelWidth() const
{
	return scrollData.getViewableWorkAreaPixelWH(0);
}

inline int VScrollWindow::getViewableWorkAreaPixelHeight() const
{
	return scrollData.getViewableWorkAreaPixelWH(1);
}

inline void VScrollWindow::
			getViewableWorkAreaPixelPosition(int& x, int& y) const
{
	x = getViewableWorkAreaPixelX();
	y = getViewableWorkAreaPixelY();
}

inline void VScrollWindow::
			getViewableWorkAreaPixelPosition(int *x, int *y) const
{
	if (x != 0) { *x = getViewableWorkAreaPixelX(); }
	if (y != 0) { *y = getViewableWorkAreaPixelY(); }
}

inline void VScrollWindow::getViewableWorkAreaPixelSize(int& w, int& h) const
{
	w = getViewableWorkAreaPixelWidth();
	h = getViewableWorkAreaPixelHeight();
}

inline void VScrollWindow::getViewableWorkAreaPixelSize(int *w, int *h) const
{
	if (w != 0) { *w = getViewableWorkAreaPixelWidth(); }
	if (h != 0) { *h = getViewableWorkAreaPixelHeight(); }
}

inline void VScrollWindow::
			getViewableWorkAreaPixelGeometry(int& x, int& y,
											 int& w, int& h) const
{
	x = getViewableWorkAreaPixelX();
	y = getViewableWorkAreaPixelY();
	w = getViewableWorkAreaPixelWidth();
	h = getViewableWorkAreaPixelHeight();
}

inline void VScrollWindow::
			getViewableWorkAreaPixelGeometry(int *x, int *y,
											 int *w, int *h) const
{
	if (x != 0) { *x = getViewableWorkAreaPixelX(); }
	if (y != 0) { *y = getViewableWorkAreaPixelY(); }
	if (w != 0) { *w = getViewableWorkAreaPixelWidth(); }
	if (h != 0) { *h = getViewableWorkAreaPixelHeight(); }
}

inline int VScrollWindow::getViewAreaPixelWidth() const
{
	return scrollData.getViewAreaPixelWH(0);
}

inline int VScrollWindow::getViewAreaPixelHeight() const
{
	return scrollData.getViewAreaPixelWH(1);
}

inline void VScrollWindow::getViewAreaPixelSize(int *w, int *h) const
{
	if (w != 0) { *w = scrollData.getViewAreaPixelWH(0); }
	if (h != 0) { *h = scrollData.getViewAreaPixelWH(1); }
}

inline void VScrollWindow::getViewAreaPixelSize(int& w, int& h) const
{
	w = scrollData.getViewAreaPixelWH(0);
	h = scrollData.getViewAreaPixelWH(1);
}

inline long VScrollWindow::getMaxViewableWorkAreaUnitX() const
{
	return scrollData.getMaxViewableWorkAreaUnitXY(0);
}

inline long VScrollWindow::getMaxViewableWorkAreaUnitY() const
{
	return scrollData.getMaxViewableWorkAreaUnitXY(1);
}

inline int VScrollWindow::getCellX(long unit_x, long unit_y) const
{
	int pixel_x, dummy;
	scrollData.getCellGeometry(unit_x, unit_y, pixel_x, dummy, dummy, dummy);
	return pixel_x;
}

inline int VScrollWindow::getCellY(long unit_x, long unit_y) const
{
	int pixel_y, dummy;
	scrollData.getCellGeometry(unit_x, unit_y, dummy, pixel_y, dummy, dummy);
	return pixel_y;
}

inline int VScrollWindow::getCellWidth(long unit_x, long unit_y) const
{
	int pixel_w, dummy;
	scrollData.getCellGeometry(unit_x, unit_y, dummy, dummy, pixel_w, dummy);
	return pixel_w;
}

inline int VScrollWindow::getCellHeight(long unit_x, long unit_y) const
{
	int pixel_h, dummy;
	scrollData.getCellGeometry(unit_x, unit_y, dummy, dummy, dummy, pixel_h);
	return pixel_h;
}

inline boolean VScrollWindow::getCellPosition(long unit_x, long unit_y,
											  int& pixel_x,
											  int& pixel_y) const
{
	int dummy;
	return scrollData.getCellGeometry(unit_x, unit_y,
									  pixel_x, pixel_y, dummy, dummy);
}

inline boolean VScrollWindow::getCellPosition(long unit_x, long unit_y,
											  int *pixel_x,
											  int *pixel_y) const
{
	int px, py;
	boolean status = getCellPosition(unit_x, unit_y, px, py);
	if (pixel_x != 0) { *pixel_x = px; }
	if (pixel_y != 0) { *pixel_y = py; }
	return status;
}

inline boolean VScrollWindow::getCellSize(long unit_x, long unit_y,
										  int& pixel_w, int& pixel_h) const
{
	int dummy;
	return scrollData.getCellGeometry(unit_x, unit_y,
									  dummy, dummy, pixel_w, pixel_h);
}

inline boolean VScrollWindow::getCellSize(long unit_x, long unit_y,
										  int *pixel_w, int *pixel_h) const
{
	int pw, ph;
	boolean status = getCellSize(unit_x, unit_y, pw, ph);
	if (pixel_w != 0) { *pixel_w = pw; }
	if (pixel_h != 0) { *pixel_h = ph; }
	return status;
}

inline boolean VScrollWindow::getCellGeometry(long unit_x, long unit_y,
											  int& pixel_x,
											  int& pixel_y,
											  int& pixel_w,
											  int& pixel_h) const
{
	return scrollData.getCellGeometry(unit_x, unit_y,
									  pixel_x, pixel_y, pixel_w, pixel_h);
}

inline boolean VScrollWindow::getCellGeometry(long unit_x, long unit_y,
											  int *pixel_x,
											  int *pixel_y,
											  int *pixel_w,
											  int *pixel_h) const
{
	int px, py, pw, ph;
	boolean status = getCellGeometry(unit_x, unit_y, px, py, pw, ph);
	if (pixel_x != 0) { *pixel_x = px; }
	if (pixel_y != 0) { *pixel_y = py; }
	if (pixel_w != 0) { *pixel_w = pw; }
	if (pixel_h != 0) { *pixel_h = ph; }
	return status;
}

inline boolean VScrollWindow::getCellRangeGeometry(long unit_x, long unit_y,
												   int unit_w, int unit_h,
												   int& pixel_x,
												   int& pixel_y,
												   int& pixel_w,
												   int& pixel_h) const
{
	return scrollData.getCellRangeGeometry(unit_x, unit_y, unit_w, unit_h,
										   pixel_x, pixel_y,
										   pixel_w, pixel_h);
}

inline boolean VScrollWindow::getCellRangeGeometry(long unit_x, long unit_y,
												   int unit_w, int unit_h,
												   int *pixel_x,
												   int *pixel_y,
												   int *pixel_w,
												   int *pixel_h) const
{
	int px, py, pw, ph;
	boolean status = scrollData.getCellRangeGeometry(unit_x, unit_y,
													 unit_w, unit_h,
													 px, py, pw, ph);
	if (pixel_x != 0) { *pixel_x = px; }
	if (pixel_y != 0) { *pixel_y = py; }
	if (pixel_w != 0) { *pixel_w = pw; }
	if (pixel_h != 0) { *pixel_h = ph; }
	return status;
}

inline boolean VScrollWindow::mapToCell(int pixel_x, int pixel_y,
										long& unit_x, long& unit_y) const
{
	return scrollData.mapToCell(pixel_x, pixel_y, unit_x, unit_y);
}

inline boolean VScrollWindow::mapToCell(int pixel_x, int pixel_y,
										 long *unit_x, long *unit_y) const
{
	long ux, uy;
	boolean status = scrollData.mapToCell(pixel_x, pixel_y, ux, uy);
	if (unit_x != 0) { *unit_x = ux; }
	if (unit_y != 0) { *unit_y = uy; }
	return status;
}

inline boolean VScrollWindow::mapToCells(int pixel_x, int pixel_y,
										 int pixel_w, int pixel_h,
										 long& unit_x, long& unit_y,
										 int& unit_w, int& unit_h) const
{
	return scrollData.mapToCells(pixel_x, pixel_y, pixel_w, pixel_h,
								 unit_x, unit_y, unit_w, unit_h);
}

inline boolean VScrollWindow::mapToCells(int pixel_x, int pixel_y,
										 int pixel_w, int pixel_h,
										 long *unit_x, long *unit_y,
										 int *unit_w, int *unit_h) const
{
	long ux, uy; int uw, uh;
	boolean status = scrollData.mapToCells(pixel_x, pixel_y,
										   pixel_w, pixel_h,
										   ux, uy, uw, uh);
	if (unit_x != 0) { *unit_x = ux; } if (unit_w != 0) { *unit_w = uw; }
	if (unit_y != 0) { *unit_y = uy; } if (unit_w != 0) { *unit_h = uh; }
	return status;
}

inline boolean VScrollWindow::isHorizontalWorkAreaStretchable() const
{
	return dimData[0].stretchWorkArea;
}

inline boolean VScrollWindow::isVerticalWorkAreaStretchable() const
{
	return dimData[1].stretchWorkArea;
}

inline boolean VScrollWindow::isAutoScrolling() const
{
	return currentlyAutoScrolling;
}

inline void VScrollWindow::usePanner(boolean b)
{
	if (b) {
		if (!useThePanner) {
			useThePanner = 1;
		}
	}
	else if (useThePanner) {
		if (isPannerShowing()) {
			hidePanner();
		}
		useThePanner = 0;
	}
}

inline boolean VScrollWindow::hasPanner() const
{
	return haveOwnPanner;
}

inline void VScrollWindow::hidePanner(boolean b)
{
	showPanner(!b);
}

inline boolean VScrollWindow::isPannerShowing() const
{
	return !pannerHidden;
}

inline boolean VScrollWindow::isPannerHidden() const
{
	return pannerHidden;
}

inline void VScrollWindow::useConfigurationMenu(boolean b)
{
	useTheConfigurationMenu = b;
}

inline boolean VScrollWindow::hasConfigurationMenu() const
{
	return useTheConfigurationMenu;
}

inline VWindow *VScrollWindow::getClientAreaWindow() const
{
	return viewWindow != 0 ? viewWindow : (VWindow *)this;
}

inline void VScrollWindow::getWorkAreaSize(long *, long *) const
{
}

inline int VScrollWindow::getWorkAreaColumnWidth(long, void **) const
{
	return 1;
}

inline int VScrollWindow::getWorkAreaRowHeight(long, void **) const
{
	return 1;
}

inline void VScrollWindow::paintWorkArea(VWindow *, long, long, int, int)
{
}

inline boolean VScrollWindow::isHorizontalScrollBarTracking() const
{
	return hasHorizontalScrollBar() ?
		   dimData[0].scrollBar->isTracking() : dimData[0].scrollBarTracking;
}

inline boolean VScrollWindow::isVerticalScrollBarTracking() const
{
	return hasVerticalScrollBar() ?
		   dimData[1].scrollBar->isTracking() : dimData[1].scrollBarTracking;
}

inline boolean VScrollWindow::isHorizontalSmoothScrolling() const
{
	return scrollData.isSmoothScrolling(0);
}

inline boolean VScrollWindow::isVerticalSmoothScrolling() const
{
	return scrollData.isSmoothScrolling(1);
}

inline boolean VScrollWindow::isMaximizeViewableWorkAreaWidth() const
{
	return scrollData.isMaximizeViewableWorkArea(0);
}

inline boolean VScrollWindow::isMaximizeViewableWorkAreaHeight() const
{
	return scrollData.isMaximizeViewableWorkArea(1);
}

inline void VScrollWindow::retrieveWorkAreaSize(long& w, long& h) const
{
	if (workWindow != 0) {
		int window_w, window_h;
		workWindow->sizeOfWin(&window_w, &window_h);
		w = window_w;
		h = window_h;
	}
	else {
		getWorkAreaSize(&w, &h);
	}
}

inline void *VScrollWindow::getViewableWorkAreaColumnData(long unit) const
{
	if ((scrollData.unitWH[0] <= 0) &&
	    (unit >= scrollData.viewableWorkAreaUnitXY[0]) &&
	    (unit < scrollData.viewableWorkAreaUnitXY[0] +
				scrollData.viewableWorkAreaUnitSizes[0]->count())) {
		return scrollData.viewableWorkAreaUnitSizes[0]->
				data(unit - scrollData.viewableWorkAreaUnitXY[0]);
	}
	return 0;
}

inline void *VScrollWindow::getViewableWorkAreaRowData(long unit) const
{
	if ((scrollData.unitWH[1] <= 0) &&
	    (unit >= scrollData.viewableWorkAreaUnitXY[1]) &&
	    (unit < scrollData.viewableWorkAreaUnitXY[1] +
				scrollData.viewableWorkAreaUnitSizes[1]->count())) {
		return scrollData.viewableWorkAreaUnitSizes[1]->
				data(unit - scrollData.viewableWorkAreaUnitXY[1]);
	}
	return 0;
}

#endif // SCROLLW_H
