//	scrollx.cpp -- [UNDER DEVELOPMENT]
//
//	VScrollWindow implementation [Common] -- 2 of 2 (see scrollw.cpp)
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

// ==========================================================================
// ::: VScrollWindow::ScrollData
// ==========================================================================

// This nonesense is just to ease the usage of the ScrollData data-members
// within member-functions which use one portion of code to do handle both
// horizontal and vertical dimension calculations.

#define work_area_unit_wh			    (workAreaUnitWH[d])
#define viewable_work_area_unit_xy	    (viewableWorkAreaUnitXY[d])
#define viewable_work_area_pixel_offset_xy \
										(viewableWorkAreaPixelOffsetXY[d])
#define scroll_unit_wh				    (unitWH[d])
#define view_area_pixel_wh			    (viewAreaPixelWH[d])
#define view_area_extra_pixel_wh	    (viewAreaExtraPixelWH[d])
#define viewable_work_area_unit_wh	    (viewableWorkAreaUnitWH[d])
#define viewable_work_area_unit_sizes   (viewableWorkAreaUnitSizes[d])
#define max_viewable_work_area_unit_xy  (maxViewableWorkAreaUnitXY[d])
#define max_viewable_work_area_pixel_offset_xy \
										(maxViewableWorkAreaPixelOffsetXY[d])
#define maximize_viewable_work_area	    (bit[d].maximizeViewableWorkArea)
#define smooth_scrolling			    (bit[d].smoothScrolling)
#define opposite_origin				    (bit[d].oppositeScrollOrigin)
#define scroll_needed				    (bit[d].scrollNeeded)

#define work_area_unit_w			    (workAreaUnitWH[0])
#define viewable_work_area_unit_x	    (viewableWorkAreaUnitXY[0])
#define viewable_work_area_pixel_offset_x \
										(viewableWorkAreaPixelOffsetXY[0])
#define scroll_unit_w				    (unitWH[0])
#define view_area_pixel_w			    (viewAreaPixelWH[0])
#define view_area_extra_pixel_w		    (viewAreaExtraPixelWH[0])
#define viewable_work_area_unit_widths  (viewableWorkAreaUnitSizes[0])
#define max_viewable_work_area_unit_x   (maxViewableWorkAreaUnitXY[0])
#define max_viewable_work_area_pixel_offset_x \
										(maxViewableWorkAreaPixelOffsetXY[0])
#define maximize_viewable_work_area_w   (bit[0].maximizeViewableWorkArea)
#define smooth_scrolling_x			    (bit[0].smoothScrolling)
#define opposite_origin_x			    (bit[0].oppositeScrollOrigin)
#define scroll_needed_x				    (bit[0].scrollNeeded)

#define work_area_unit_h			    (workAreaUnitWH[1])
#define viewable_work_area_unit_y	    (viewableWorkAreaUnitXY[1])
#define viewable_work_area_pixel_offset_y \
										(viewableWorkAreaPixelOffsetXY[1])
#define scroll_unit_h				    (unitWH[1])
#define view_area_pixel_h			    (viewAreaPixelWH[1])
#define view_area_extra_pixel_h		    (viewAreaExtraPixelWH[1])
#define viewable_work_area_unit_heights (viewableWorkAreaUnitSizes[1])
#define max_viewable_work_area_unit_y	(maxViewableWorkAreaUnitXY[1])
#define max_viewable_work_area_pixel_offset_y \
										(maxViewableWorkAreaPixelOffsetXY[1])
#define maximize_viewable_work_area_h	(bit[1].maximizeViewableWorkArea)
#define smooth_scrolling_y				(bit[1].smoothScrolling)
#define opposite_origin_y				(bit[1].oppositeScrollOrigin)
#define scroll_needed_y					(bit[1].scrollNeeded)

// --------------------------------------------------------------------------
//
VScrollWindow::ScrollData::ScrollData()
{
	for (int d = 0 ; d < 2 ; d++) {
		work_area_unit_wh						= 0;
		viewable_work_area_unit_xy				= 0;
		viewable_work_area_pixel_offset_xy		= 0;
		view_area_pixel_wh						= 0;
		scroll_unit_wh							= 1;
		max_viewable_work_area_unit_xy			= 0;
		max_viewable_work_area_pixel_offset_xy	= 0;
		maximize_viewable_work_area				= 1;
		opposite_origin							= 0;
		smooth_scrolling						= 0;
		scroll_needed							= 0;
		viewable_work_area_unit_wh				= 0;
		viewable_work_area_unit_sizes			= new UnitDataArray;
	}
}

// --------------------------------------------------------------------------
//
VScrollWindow::ScrollData::~ScrollData()
{
	delete viewable_work_area_unit_widths;
	delete viewable_work_area_unit_heights;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::setUnits(VScrollWindow *, int d, int units)
{
	scroll_unit_wh = units;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::
					setMaximizeViewableWorkArea(VScrollWindow *sw, int d,
												boolean b)
{
	boolean do_both_dimensions;
	long unit_dxy[2]; float offset_xy[2];

  	unit_dxy[0] = unit_dxy[1] =  0;
	offset_xy[0] = offset_xy[1] = -1.0F;

	if ((d != 0) && (d != 1)) {
		d = 0;
		do_both_dimensions = TRUE;
	}
	else {
		do_both_dimensions = FALSE;
	}

  Begin:

	if (b) {
		if (!maximize_viewable_work_area) {
			maximize_viewable_work_area = 1;
			setMaxViewableWorkArea(sw, d);
			if (view_area_extra_pixel_wh > 0) {
				if (viewable_work_area_unit_xy >
					max_viewable_work_area_unit_xy) {
					unit_dxy[d] = viewable_work_area_unit_xy -
							 max_viewable_work_area_unit_xy;
				}
				if (smooth_scrolling) {
					int unit_size =
						retrieveUnitSize
							(sw, d, max_viewable_work_area_unit_xy);
					offset_xy[d] =
						float(max_viewable_work_area_pixel_offset_xy) /
						float(unit_size);
				}
			}
			if (d == 0) {
				sw->setHorizontalScrollValues();
			}
			else {
				sw->setVerticalScrollValues();
			}
		}
	}
	else if (maximize_viewable_work_area) {
		maximize_viewable_work_area = 0;
		setMaxViewableWorkArea(sw, d);
		if (d == 0) {
			sw->setHorizontalScrollValues();
		}
		else {
			sw->setVerticalScrollValues();
		}
	}

	if (do_both_dimensions) {
		d = 1;
		do_both_dimensions = FALSE;
		goto Begin;
	}

	scroll(sw, unit_dxy[0], unit_dxy[1], offset_xy[0], offset_xy[1]);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::setSmoothScrolling(VScrollWindow *sw,
												   int d, boolean b)
{
	boolean do_both_dimensions, do_update = FALSE;

	if ((d != 0) && (d != 1)) {
		d = 0;
		do_both_dimensions = TRUE;
	}
	else {
		do_both_dimensions = FALSE;
	}

  Begin:

	if (b) {
		if (!smooth_scrolling) {
			smooth_scrolling = 1;
			sw->dimData[d].scaleScrollValues = 1;
			do_update = TRUE;
		}
	}
	else if (smooth_scrolling) {
		smooth_scrolling = 0;
		sw->dimData[d].scaleScrollValues = 0;
		viewable_work_area_pixel_offset_xy = 0;
		do_update = TRUE;
	}

	if (do_both_dimensions) {
		d = 1;
		do_both_dimensions = FALSE;
		goto Begin;
	}

	if (do_update) {
		sw->updateView();
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::setOppositeScrollOrigin(VScrollWindow *sw,
														int d, boolean b)
{
	opposite_origin = b;
}

// --------------------------------------------------------------------------
// Map the given pixel location (x,y) within the view-area to the
// corresponding viewable work-area cell.
//
boolean VScrollWindow::ScrollData::mapToCell(int pixel_x, int pixel_y,
											 long& c, long& r) const
{
	if ((pixel_x >= view_area_pixel_w) || (pixel_x < 0) ||
		(pixel_y >= view_area_pixel_h) || (pixel_y < 0) ||
		((view_area_extra_pixel_w > 0) &&
		 (pixel_x >= view_area_pixel_w - view_area_extra_pixel_w)) ||
		((view_area_extra_pixel_h > 0) &&
		 (pixel_y >= view_area_pixel_h - view_area_extra_pixel_h))) {
		return FALSE;
	}
	if (scroll_unit_w > 1) {
		//
		// Fixed-unit scrolling.
		//
		pixel_x = (pixel_x + viewable_work_area_pixel_offset_x) /
				   scroll_unit_w;
	}
	else if (scroll_unit_w <= 0) {
		//
		// Variable-unit scrolling.
		//
		if ((pixel_x = viewable_work_area_unit_widths->
				 find(pixel_x + viewable_work_area_pixel_offset_x)) < 0) {
			return FALSE;
		}
	}
	if (scroll_unit_h > 1) {
		//
		// Fixed-unit scrolling.
		//
		pixel_y = (pixel_y + viewable_work_area_pixel_offset_y) /
				   scroll_unit_h;
	}
	else if (scroll_unit_h <= 0) {
		//
		// Variable-unit scrolling.
		//
		if ((pixel_y =
			 viewable_work_area_unit_heights->
			 find(pixel_y + viewable_work_area_pixel_offset_y)) < 0) {
			return FALSE;
		}
	}
	c = pixel_x + viewable_work_area_unit_x;
	r = pixel_y + viewable_work_area_unit_y;
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ScrollData
					 ::mapToCells(int x, int y, int w, int h,
					   			  long& ux, long& uy, int& uw, int& uh) const
{
	int xy[2], wh[2], lastxy, uwh, d = 0;
	long uxy;

	xy[0] = x; wh[0] = w;
	xy[1] = y; wh[1] = h;

  Begin:

	if ((xy[d] >= view_area_pixel_wh) || (wh[d] <= 0)) {
		return FALSE;
	}

	if (xy[d] < 0) {
		xy[d] = 0;
	}

	if ((lastxy = xy[d] + wh[d] - 1) >= view_area_pixel_wh) {
		lastxy = view_area_pixel_wh - 1;
	}
	if (view_area_extra_pixel_wh > 0) {
		if (xy[d] >= view_area_pixel_wh - view_area_extra_pixel_wh) {
			return FALSE;
		}
		else if (lastxy >= view_area_pixel_wh - view_area_extra_pixel_wh) {
			lastxy = view_area_pixel_wh - view_area_extra_pixel_wh - 1;
		}
	}
	if (scroll_unit_wh > 1) {
		//
		// Fixed-unit scrolling.
		//
		xy[d] = (xy[d] + viewable_work_area_pixel_offset_xy) /
				 scroll_unit_wh;
		lastxy = (lastxy + viewable_work_area_pixel_offset_xy) /
				  scroll_unit_wh;
	}
	else if (scroll_unit_wh <= 0) {
		//
		// Variable-unit scrolling.
		//
		if ((xy[d] =
			 viewable_work_area_unit_sizes->find
			 	(xy[d] + viewable_work_area_pixel_offset_xy)) < 0) {
			return FALSE;
		}
		if ((lastxy =
			 viewable_work_area_unit_sizes->find
			 	(lastxy + viewable_work_area_pixel_offset_xy)) < 0) {
			lastxy = viewable_work_area_unit_wh - 1;
		}
	}

	uxy = xy[d] + viewable_work_area_unit_xy;
	uwh = (lastxy + viewable_work_area_unit_xy) - uxy + 1;

	if (d == 0) {
		ux = uxy;
		uw = uwh;
		d = 1;
		goto Begin;
	}
	else {
		uy = uxy;
		uh = uwh;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
// If the given work-area cell is not viewable, then return 0, otherwise if
// it is completely viewable, then return 1, otherwise if it is partially
// viewable then return -1.
//
int VScrollWindow::isCellViewable(long unit_x, long unit_y) const
{
	if ((unit_x >= scrollData.getViewableWorkAreaUnitXY(0)) &&
		(unit_x < scrollData.getViewableWorkAreaUnitXY(0) +
				  scrollData.getViewableWorkAreaUnitWH(0)) &&
	    (unit_y >= scrollData.getViewableWorkAreaUnitXY(1)) &&
		(unit_y < scrollData.getViewableWorkAreaUnitXY(1) +
				  scrollData.getViewableWorkAreaUnitWH(1))) {
		if (((unit_x == scrollData.getViewableWorkAreaUnitXY(0)) &&
			 (scrollData.getViewableWorkAreaPixelOffsetXY(0) > 0)) ||
			((unit_y == scrollData.getViewableWorkAreaUnitXY(1)) &&
			 (scrollData.getViewableWorkAreaPixelOffsetXY(1) > 0)) ||
			((unit_x == scrollData.getViewableWorkAreaUnitXY(0) +
						scrollData.getViewableWorkAreaUnitWH(0) - 1) &&
			 (scrollData.getViewAreaExtraPixelWH(0) < 0)) ||
			((unit_y == scrollData.getViewableWorkAreaUnitXY(1) +
						scrollData.getViewableWorkAreaUnitWH(1) - 1) &&
			 (scrollData.getViewAreaExtraPixelWH(1) < 0))) {
			return -1;
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ScrollData::getCellRangeGeometry(long c, long r,
														int nc, int nr,
														int& x, int& y,
														int& w, int& h) const
{
	int viewable_work_area_unit_w, viewable_work_area_unit_h, cc, rr;

	if ((nc <= 0) || (nr <= 0) ||
		((cc = c + nc - 1) < viewable_work_area_unit_x) ||
		((rr = r + nr - 1) < viewable_work_area_unit_y) ||
		(c >= viewable_work_area_unit_x + (viewable_work_area_unit_w =
										   getViewableWorkAreaUnitWH(0))) ||
		(r >= viewable_work_area_unit_y + (viewable_work_area_unit_h =
										   getViewableWorkAreaUnitWH(1)))) {
		return FALSE;
	}

	if (c <= viewable_work_area_unit_x) {
		x = -viewable_work_area_pixel_offset_x;
	}
	else if (scroll_unit_w == 1) {
		x = c - viewable_work_area_unit_x;
	}
	else if (scroll_unit_w > 1) {
		x = (c - viewable_work_area_unit_x) * scroll_unit_w;
	}
	else {
		x = viewable_work_area_unit_widths->
			sum(c - viewable_work_area_unit_x - 1);
	}
	if (cc >= viewable_work_area_unit_x + viewable_work_area_unit_w - 1) {
		w = view_area_pixel_w - x - view_area_extra_pixel_w;
	}
	else if (scroll_unit_w == 1) {
		w = (cc - viewable_work_area_unit_x + 1) - x;
	}
	else if (scroll_unit_h > 1) {
		w = (cc - viewable_work_area_unit_x + 1) * scroll_unit_w - x;
	}
	else {
		if (c <= viewable_work_area_unit_x) {
			w = viewable_work_area_unit_widths->sum(cc) - x;
		}
		else {
			w = viewable_work_area_unit_widths->
				sum(c - viewable_work_area_unit_x, nc) - x;
		}
	}

	if (r <= viewable_work_area_unit_y) {
		y = -viewable_work_area_pixel_offset_y;
	}
	else if (scroll_unit_h == 1) {
		y = r - viewable_work_area_unit_y;
	}
	else if (scroll_unit_w > 1) {
		y = (r - viewable_work_area_unit_y) * scroll_unit_h;
	}
	else {
		y = viewable_work_area_unit_heights->
			sum(r - viewable_work_area_unit_y - 1);
	}
	if (rr >= viewable_work_area_unit_y + viewable_work_area_unit_h - 1) {
		h = view_area_pixel_h - y - view_area_extra_pixel_h;
	}
	else if (scroll_unit_h == 1) {
		h = (rr - viewable_work_area_unit_y + 1) - y;
	}
	else if (scroll_unit_h > 1) {
		h = (rr - viewable_work_area_unit_y + 1) * scroll_unit_h - y;
	}
	else {
		if (r <= viewable_work_area_unit_y) {
			h = viewable_work_area_unit_heights->sum(rr) - y;
		}
		else {
			h = viewable_work_area_unit_heights->
				sum(r - viewable_work_area_unit_y, nr) - y;
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ScrollData::getCellGeometry(long c, long r,
												   int& x, int& y,
												   int& w, int& h) const
{
	boolean viewable_x, viewable_y;

	if ((c >= viewable_work_area_unit_x) &&
		(c < viewable_work_area_unit_x + getViewableWorkAreaUnitWH(0))) {
		if (scroll_unit_w == 1) {
			w = 1;
			x = c - viewable_work_area_unit_x;
		}
		else if (scroll_unit_w > 1) {
			w = scroll_unit_w;
			x = ((c - viewable_work_area_unit_x) * scroll_unit_w) -
				 viewable_work_area_pixel_offset_x;
		}
		else {
			w = viewable_work_area_unit_widths->
				size(c -= viewable_work_area_unit_x);
			x = (c == 0) ? -viewable_work_area_pixel_offset_x :
							viewable_work_area_unit_widths->sum(c) -
							viewable_work_area_pixel_offset_x;
		}
		viewable_x = TRUE;
	}
	else {
		x = w = 0;
		viewable_x = FALSE;
	}
	if ((r >= viewable_work_area_unit_y) &&
		(r < viewable_work_area_unit_y + getViewableWorkAreaUnitWH(1))) {
		if (scroll_unit_h == 1) {
			h = 1;
			y = r - viewable_work_area_unit_y;
		}
		else if (scroll_unit_h > 1) {
			h = scroll_unit_h;
			y = ((r - viewable_work_area_unit_y) * scroll_unit_h) -
				 viewable_work_area_pixel_offset_y;
		}
		else {
			h = viewable_work_area_unit_heights->
				size(r -= viewable_work_area_unit_y);
			y = (r == 0) ? -viewable_work_area_pixel_offset_y :
							viewable_work_area_unit_heights->sum(r) -
							viewable_work_area_pixel_offset_y;
		}
		viewable_y = TRUE;
	}
	else {
		y = h = 0;
		viewable_y = FALSE;
	}
	return viewable_x && viewable_y;
}

// --------------------------------------------------------------------------
//
long VScrollWindow::ScrollData::getWorkAreaPixelWH(const VScrollWindow *sw,
												   int d,
												   long xy, long wh) const
{
	if (scroll_unit_wh == 1) {
		return wh;
	}
	else if (scroll_unit_wh > 1) {
		return wh * scroll_unit_wh;
	}
	else {
		long pixel_wh = 0;
		for (long unit = xy ; unit < xy + wh ; unit++) {
			pixel_wh += retrieveVariableUnitSize(sw, d, unit);
		}
		return pixel_wh;
	}
}

// --------------------------------------------------------------------------
// Returns the position and size of the viewable work-area in relation to
// the total work-area as floating point values in the range 0.0 to 1.0.
//
void VScrollWindow::ScrollData
				  ::getViewableWorkAreaRatio(float& fx, float& fy,
				  							 float& fw, float& fh) const
{
	long viewable_work_area_unit_w = getViewableWorkAreaUnitWH(0);
	if (max_viewable_work_area_unit_x > 0) {
		fx = float(viewable_work_area_unit_x) /
			 float(max_viewable_work_area_unit_x);
	}
	else {
		fx = 0.0F;
	}
	if (work_area_unit_w > 0) {
		fw = float(viewable_work_area_unit_w) / float(work_area_unit_w);
	}
	else {
		fw = 1.0F;
	}
	long viewable_work_area_unit_h = getViewableWorkAreaUnitWH(1);
	if (max_viewable_work_area_unit_y > 0) {
		fy = float(viewable_work_area_unit_y) /
			 float(max_viewable_work_area_unit_y);
	}
	else {
		fy = 0.0F;
	}
	if (work_area_unit_h > 0) {
		fh = float(viewable_work_area_unit_h) / float(work_area_unit_h);
	}
	else {
		fh = 1.0F;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::update(VScrollWindow *sw)
{
	sw->retrieveWorkAreaSize(work_area_unit_w, work_area_unit_h);
	for (int d = 0 ; d < 2 ; d++) {
		view_area_pixel_wh						= 0;
		view_area_extra_pixel_wh				= 0;
		max_viewable_work_area_unit_xy			= 0;
		max_viewable_work_area_pixel_offset_xy	= 0;
		scroll_needed							= 0;
		viewable_work_area_unit_wh				= 0;
		viewable_work_area_unit_sizes->clear();
		if (scroll_unit_wh == 1) {
			if (viewable_work_area_pixel_offset_xy > 0) {
				viewable_work_area_pixel_offset_xy = 0;
			}
		}
	}
	sw->updateConfiguration();
}

// --------------------------------------------------------------------------
// This should be called when the view-area is resized.
// It will update the scroll-data-cache as follows.
//
// These data-members may be recomputed/set:
//
//		view_area_pixel_wh
//		view_area_extra_pixel_wh
//	  *	viewable_work_area_unit_wh
//	 **	viewable_work_area_unit_sizes
//		scroll_needed
//
// If maximize_viewable_work_area is set, then these data-members
// may also be recomputed/set:
//
//		viewable_work_area_unit_xy
//		work_area_unit_pixel_offset_xy
//
void VScrollWindow::ScrollData::resize(VScrollWindow *sw,
									   int *new_view_area_pixel_wh,
									   int *reduce_wh_if_scroll_needed_yx,
									   int *scroll_dxy)
{
  	int				d = 0, change_wh[2], reducing_wh[2];
	int				view_area_unit_size, fraction;
	long			inrange_work_area_size;
	long			maxized_viewable_work_area_unit_xy[2];
	int				maxized_view_area_extra_pixel_wh[2];
	int				maxized_viewable_work_area_pixel_offset_xy[2];
	int				maxized_viewable_work_area_unit_wh[2];
	UnitDataArray	maxized_viewable_work_area_unit_sizes[2];

	reducing_wh[0] = reducing_wh[1] = 0;

  Begin:

	if (new_view_area_pixel_wh[d] <= 0) {
		goto End;
	}

	change_wh[d] = new_view_area_pixel_wh[d] - view_area_pixel_wh;
	view_area_pixel_wh = new_view_area_pixel_wh[d];

	if (change_wh[d] < 0) {
		//
		// Here, we are shrinking.
		//
		if (scroll_unit_wh == 1) {
			//
			// Pixel-unit scrolling -- shrinking.
			//
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			if (inrange_work_area_size > view_area_pixel_wh) {
				//
				// Here, there are one or more inrange work-area
				// units which are not viewable at all.
				//
				view_area_extra_pixel_wh = 0;
			}
			else {
				//
				// Here, all inrange work-area units are viewable.
				//
				view_area_extra_pixel_wh =
					int(view_area_pixel_wh - inrange_work_area_size);
			}
		}
		else if (scroll_unit_wh > 1) {
			//
			// Fixed-unit scrolling -- shrinking.
			//
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			view_area_unit_size = view_area_pixel_wh / scroll_unit_wh;
			if (viewable_work_area_pixel_offset_xy > 0) {
				view_area_unit_size++;
				fraction = (view_area_pixel_wh -
						    (scroll_unit_wh -
							 viewable_work_area_pixel_offset_xy)) %
							scroll_unit_wh;
			}
			else {
				fraction = view_area_pixel_wh % scroll_unit_wh;
			}
			if (fraction != 0) {
				view_area_unit_size++;
			}
			if (inrange_work_area_size > view_area_unit_size) {
				//
				// Here, there are one or more inrange work-area
				// units which are not viewable at all.
				//
				viewable_work_area_unit_wh = view_area_unit_size;
				if (fraction != 0) {
					view_area_extra_pixel_wh = fraction - scroll_unit_wh;
				}
				else {
					view_area_extra_pixel_wh = 0;
				}
			}
			else {
				//
				// Here, all inrange work-area units are viewable.
				//
				viewable_work_area_unit_wh = (int)inrange_work_area_size;
				view_area_extra_pixel_wh =
					view_area_pixel_wh -
					viewable_work_area_unit_wh * scroll_unit_wh +
					viewable_work_area_pixel_offset_xy;
			}
		}
		else {
			//
			// Variable-unit scrolling -- shrinking.
			//
			if (-change_wh[d] > view_area_extra_pixel_wh) {
				viewable_work_area_unit_sizes->
					truncate(view_area_pixel_wh +
							 viewable_work_area_pixel_offset_xy);
				viewable_work_area_unit_wh =
					viewable_work_area_unit_sizes->count();
				view_area_extra_pixel_wh =
					view_area_pixel_wh -
					(viewable_work_area_unit_sizes->sum() -
					 viewable_work_area_pixel_offset_xy);
			}
			else {
				view_area_extra_pixel_wh += change_wh[d];
			}
#if 1
			if (sw->dimData[d].stretchWorkArea &&
				(scroll_unit_wh == 0) &&
				(viewable_work_area_unit_xy +
				 viewable_work_area_unit_wh == work_area_unit_wh) &&
				(view_area_extra_pixel_wh < 0)) {
				int size; void *data;
				if (d == 0) {
					size = sw->getWorkAreaColumnWidth
								(viewable_work_area_unit_xy +
								 viewable_work_area_unit_wh - 1, &data);
			
				}
				else {
					size = sw->getWorkAreaRowHeight
								(viewable_work_area_unit_xy +
								 viewable_work_area_unit_wh - 1, &data);
				}
				int index = work_area_unit_wh -
							viewable_work_area_unit_xy - 1;
				int last_size = viewable_work_area_unit_sizes->size(index);
				if (last_size > size) {
					//
					// This unit was previously stretched.
					//
					view_area_extra_pixel_wh += last_size - size;
					viewable_work_area_unit_sizes-> set(index, size, data);
				}
			}
#endif
		}
	}
	else if (change_wh[d] > 0) {
		//
		// Here, we are growing.
		//
		if (scroll_unit_wh == 1) {
			//
			// Pixel-unit scrolling -- growing.
			//
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			if (inrange_work_area_size >= view_area_pixel_wh) {
				//
				// Here, there are one or more inrange work-area
				// units which are not viewable at all.
				//
				view_area_extra_pixel_wh = 0;
			}
			else {
				//
				// Here, all inrange work-area units are viewable.
				//
				view_area_extra_pixel_wh =
					int(view_area_pixel_wh - inrange_work_area_size);
			}
		}
		else if (scroll_unit_wh > 1) {
			//
			// Fixed-unit scrolling -- growing.
			//
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			view_area_unit_size = view_area_pixel_wh / scroll_unit_wh;
			if (viewable_work_area_pixel_offset_xy > 0) {
				view_area_unit_size++;
				fraction = (view_area_pixel_wh -
						    (scroll_unit_wh -
							 viewable_work_area_pixel_offset_xy)) %
							scroll_unit_wh;
			}
			else {
				fraction = view_area_pixel_wh % scroll_unit_wh;
			}
			if (fraction != 0) {
				view_area_unit_size++;
			}
			if (inrange_work_area_size > view_area_unit_size) {
				//
				// Here, there are one or more inrange work-area
				// units which are not viewable at all.
				//
				viewable_work_area_unit_wh = view_area_unit_size;
				if (fraction != 0) {
					view_area_extra_pixel_wh = fraction - scroll_unit_wh;
				}
				else {
					view_area_extra_pixel_wh = 0;
				}
			}
			else {
				//
				// Here, all inrange work-area units are viewable.
				//
				viewable_work_area_unit_wh = (int)inrange_work_area_size;
				view_area_extra_pixel_wh =
					view_area_pixel_wh -
					viewable_work_area_unit_wh * scroll_unit_wh +
					viewable_work_area_pixel_offset_xy;
			}
		}
		else {
			//
			// Variable-unit scrolling -- growing.
			//
			if (view_area_extra_pixel_wh > 0) {
				//
				// Here, we are already viewing the end of the work-area;
				// no new units are becoming viewable.  We may end up
				// scrolling more units into range/view to maximize
				// the viewable work-area.
				//
				view_area_extra_pixel_wh += change_wh[d];
			}
			else if (-view_area_extra_pixel_wh > change_wh[d]) {
				//
				// Here, the growth is only enough to show more
				// of a an already partially viewable unit;
				// no new units are becoming viewable.
				//
				view_area_extra_pixel_wh += change_wh[d];
			}
			else {
				//
				// Here, new work-area units may be brought into view.
				//
				long first_unviewable_unit = viewable_work_area_unit_xy +
											 viewable_work_area_unit_wh;
				if (first_unviewable_unit >= work_area_unit_wh) {
					//
					// Here, there are no more work-area units to be
					// brought into view.  We may need to scroll more
					// units into range/view if we should to maximize
					// the viewable work-area.
					//
					view_area_extra_pixel_wh += change_wh[d];
				}
				else {
					//
					// Here, we will bring work-area units into
					// view (there exists at least one) until we run
					// out of work-area units or view-area space.
					//
					int unit_size;
					int total_unit_size =
						viewable_work_area_unit_sizes->sum() -
						viewable_work_area_pixel_offset_xy;
					for (long unit = first_unviewable_unit ;
						 unit < work_area_unit_wh ; unit++) {
						void *data;
						unit_size =
							retrieveVariableUnitSize(sw, d, unit, &data);
						viewable_work_area_unit_sizes->
							append(unit_size, data);
						viewable_work_area_unit_wh++;
						if ((total_unit_size +=
							 unit_size) >= view_area_pixel_wh) {
							//
							// Here, we've filled up the view-area
							// with newly viewable work-area units.
							//
							break;
						}
					}
					view_area_extra_pixel_wh =
						view_area_pixel_wh - total_unit_size;
				}
			}
		}
	}

	// Calculate but don't apply the data for maximizing the viewable
	// work-area; we do this in the maxized_XXX variables.  We need
	// this information to tell us whether or not scroll-bars are needed.
	// This needs to be done whether or not we are in maximize-viewable-
	// work-area mode since we maximize the viewable work-area in any
	// case if it would result in scroll-bars not being needed at all.
	// If it turns out we do need to maximize, we will use this data.

	if (view_area_extra_pixel_wh < 0) {
		if (smooth_scrolling ||
			(viewable_work_area_unit_wh <= work_area_unit_wh)) {
			scroll_needed = 1;
			scroll_dxy[d] = 0;
		}
		else {
			scroll_needed = 0;
			scroll_dxy[d] = 0;
		}
	}
	else if (view_area_extra_pixel_wh == 0) {
		if ((viewable_work_area_unit_xy > 0) ||
			(viewable_work_area_pixel_offset_xy > 0)) {
			scroll_needed = 1;
		}
		else {
			scroll_needed = (scroll_unit_wh == 1) ?
							 work_area_unit_wh > view_area_pixel_wh :
							 work_area_unit_wh > viewable_work_area_unit_wh;
		}
		scroll_dxy[d] = 0;
	}
	else if (scroll_unit_wh == 1) {
		//
		// Pixel-unit scrolling.
		//
		if (viewable_work_area_unit_xy > 0) {
			if (view_area_extra_pixel_wh < viewable_work_area_unit_xy) {
				scroll_dxy[d] = view_area_extra_pixel_wh;
				maxized_viewable_work_area_unit_xy[d] =
					viewable_work_area_unit_xy - view_area_extra_pixel_wh;
				maxized_view_area_extra_pixel_wh[d] = 0;
				scroll_needed = 1;
			}
			else {
				scroll_dxy[d] = viewable_work_area_unit_xy;
				maxized_view_area_extra_pixel_wh[d] =
					view_area_extra_pixel_wh - viewable_work_area_unit_xy;
				maxized_viewable_work_area_unit_xy[d] = 0;
				scroll_needed = 0;
			}
		}
		else {
			scroll_needed = 0;
			scroll_dxy[d] = 0;
		}
	}
	else {
		//
		// Fixed-unit or variable-unit scrolling.
		//
		maxized_viewable_work_area_unit_xy[d] = viewable_work_area_unit_xy;
		maxized_view_area_extra_pixel_wh[d] = view_area_extra_pixel_wh;
		maxized_viewable_work_area_pixel_offset_xy[d] =
			viewable_work_area_pixel_offset_xy;
		maxized_viewable_work_area_unit_wh[d]  = viewable_work_area_unit_wh;
		if (maxized_viewable_work_area_pixel_offset_xy[d] > 0) {
			//
			// Here, we have pixels offset out of range which can be
			// pulled into range to cover the extra view-area pixels.
			//
			if (maxized_view_area_extra_pixel_wh[d] <
				maxized_viewable_work_area_pixel_offset_xy[d]) {
				//
				// Here, there is enough pixels offset out
				// of range to be pulled into range to
				// cover the extra view-area pixels.
				//
				scroll_dxy[d] = maxized_view_area_extra_pixel_wh[d];
				maxized_viewable_work_area_pixel_offset_xy[d] -=
					maxized_view_area_extra_pixel_wh[d];
				maxized_view_area_extra_pixel_wh[d] = 0;
			}
			else {
				scroll_dxy[d] =
					maxized_viewable_work_area_pixel_offset_xy[d];
				maxized_view_area_extra_pixel_wh[d] -=
					maxized_viewable_work_area_pixel_offset_xy[d];
				maxized_viewable_work_area_pixel_offset_xy[d] = 0;
			}
		}
		else {
			scroll_dxy[d] = 0;
		}
		if ((maxized_view_area_extra_pixel_wh[d] > 0) &&
			(maxized_viewable_work_area_unit_xy[d] > 0)) {
			if (scroll_unit_wh > 1) {
				//
				// Fixed-unit scrolling.
				//
				if (maxized_view_area_extra_pixel_wh[d] >= scroll_unit_wh) {
					int scroll_pixels, scroll_units,
						view_area_extra_unit_size =
							maxized_view_area_extra_pixel_wh[d] /
							scroll_unit_wh;
					if (view_area_extra_unit_size <=
						maxized_viewable_work_area_unit_xy[d]) {
						scroll_units = view_area_extra_unit_size;
					}
					else {
						scroll_units = maxized_viewable_work_area_unit_xy[d];
					}
					scroll_pixels = scroll_units * scroll_unit_wh;
					scroll_dxy[d] += scroll_pixels;
					maxized_viewable_work_area_unit_xy[d] -= scroll_units;
					maxized_viewable_work_area_unit_wh[d] += scroll_units;
					maxized_view_area_extra_pixel_wh[d] -= scroll_pixels;
				}
				if (smooth_scrolling &&
					(maxized_view_area_extra_pixel_wh[d] > 0) &&
					(maxized_viewable_work_area_unit_xy[d] > 0)) {
					fraction =
						maxized_view_area_extra_pixel_wh[d] % scroll_unit_wh;
					maxized_viewable_work_area_pixel_offset_xy[d] =
						scroll_unit_wh - fraction;
					maxized_view_area_extra_pixel_wh[d] = 0;
					maxized_viewable_work_area_unit_xy[d]--;
					maxized_viewable_work_area_unit_wh[d]++;
					scroll_dxy[d] += fraction;
				}
			}
			else {
				//
				// Variable-unit scrolling.
				//
				if (reducing_wh[d]) {
					//
					// Here, this is the second pass for this dimension.
					//
					maxized_viewable_work_area_unit_sizes[d].clear();
				}
				long unit; int unit_size;
				for (unit = maxized_viewable_work_area_unit_xy[d] - 1 ;
					 unit >= 0 ; unit--) {
					void *data;
					unit_size = retrieveVariableUnitSize(sw, d, unit, &data);
					if ((maxized_view_area_extra_pixel_wh[d] -=
						 unit_size) >= 0) {
						maxized_viewable_work_area_unit_xy[d]--;
						maxized_viewable_work_area_unit_wh[d]++;
						maxized_viewable_work_area_unit_sizes[d].
							prepend(unit_size, data);
						scroll_dxy[d] += unit_size;
						if (maxized_view_area_extra_pixel_wh[d] == 0) {
							break;
						}
					}
					else {
						maxized_view_area_extra_pixel_wh[d] += unit_size;
						if (smooth_scrolling) {
							maxized_viewable_work_area_unit_xy[d]--;
							maxized_viewable_work_area_unit_wh[d]++;
							maxized_viewable_work_area_unit_sizes[d].
								prepend(unit_size, data);
							maxized_viewable_work_area_pixel_offset_xy[d] =
								unit_size -
								(maxized_view_area_extra_pixel_wh[d] %
								 unit_size);
							maxized_view_area_extra_pixel_wh[d] = 0;
							// scroll_dxy[d] += change_wh[d];
							scroll_dxy[d] +=
								unit_size -
								maxized_viewable_work_area_pixel_offset_xy[d];
						}
						break;
					}
				}
			}
		}
		else if (scroll_unit_wh <= 0) {
			maxized_viewable_work_area_unit_sizes[d].clear();
		}
		scroll_needed = (work_area_unit_wh >
						 maxized_viewable_work_area_unit_wh[d]) ||
						(maxized_viewable_work_area_unit_xy[d] > 0) ||
						(maxized_view_area_extra_pixel_wh[d] < 0) ||
						(maxized_viewable_work_area_pixel_offset_xy[d] > 0);
	}

	// Do the other dimension, unless we're in the reduction phase.

	if (!reducing_wh[0] && !reducing_wh[1] && (d == 0)) {
		d = 1;
		goto Begin;
	}

	// Handle the recalculation of the horizontal data if a vertical
	// scroll-bar is needed and if the width should be reduced in this
	// case, and likewise with the vertical data if a horizontal scroll-bar
	// is needed and if the height should be reduced in this case.

	if (!reducing_wh[0] &&
		(reduce_wh_if_scroll_needed_yx[0] > 0) && scroll_needed_y) {
		new_view_area_pixel_wh[0] -= reduce_wh_if_scroll_needed_yx[0];
		reducing_wh[0] = 1;
		d = 0;
		goto Begin;
	}
	else if (!reducing_wh[1] &&
			 (reduce_wh_if_scroll_needed_yx[1] > 0) && scroll_needed_x) {
		new_view_area_pixel_wh[1] -= reduce_wh_if_scroll_needed_yx[1];
		reducing_wh[1] = 1;
		d = 1;
		goto Begin;
	}

  	d = 0;

  BeginII:

	// If viewable work-area maximization is possible and if we are either
	// in maximize-viewable-work-area mode or if the maximization would
	// result in scroll-bars being unneeded, then apply the maximization
	// calculated above.

	if ((scroll_dxy[d] != 0) &&
		(maximize_viewable_work_area || !scroll_needed)) {
		viewable_work_area_unit_xy = maxized_viewable_work_area_unit_xy[d];
		view_area_extra_pixel_wh = maxized_view_area_extra_pixel_wh[d];
		if (scroll_unit_wh != 1) {
			viewable_work_area_pixel_offset_xy =
				maxized_viewable_work_area_pixel_offset_xy[d];
			viewable_work_area_unit_wh = 
				maxized_viewable_work_area_unit_wh[d];
			if (scroll_unit_wh <= 0) {
				viewable_work_area_unit_sizes->
					prepend(maxized_viewable_work_area_unit_sizes[d]);
			}
		}
	}
	else {
		scroll_dxy[d] = 0;
	}

	setMaxViewableWorkArea(sw, d);

#if 1
	if (sw->dimData[d].stretchWorkArea &&
		(scroll_unit_wh == 0) &&
		(viewable_work_area_unit_wh > 0) &&
		(viewable_work_area_unit_xy +
		 viewable_work_area_unit_wh == work_area_unit_wh) &&
		(view_area_extra_pixel_wh > 0)) {
		viewable_work_area_unit_sizes->
			incrementSize(work_area_unit_wh -
						  viewable_work_area_unit_xy - 1,
						  view_area_extra_pixel_wh);
		view_area_extra_pixel_wh = 0;
	}
#endif

  End:

	if (d == 0) {
		d = 1;
		goto BeginII;
	}

	// ToDo!!! viewableWorkAreaUnitWH[1] and bits[1].scrollNeeded
	// (at least) are *not* set up correctly here!!!
}

// --------------------------------------------------------------------------
// This should be called when the work-area of the given VScrollWindow is
// scrolled.  This will update the scroll data based on the given unit change
// in the work-area location and offset within the work-area unit.  The given
// offset is interpreted as follows:
//
//     1. If it is greater than or equal to one, then it is interpreted as
//        a pixel offset within the unit - any fractional part is discarded.
//     2. If it is greater than or equal to zero and less than one,
//        then it is interpreted as a percentage of the size of the unit.
//	   3. If it is less than zero, then if the unit change is zero,
//        then it is ignored, otherwise it it treated as if it were zero.
//
// These data-members may be recomputed/set:
//
//		viewable_work_area_unit_xy
//		viewable_work_area_pixel_offset_xy
//		view_area_extra_pixel_wh
//	  * viewable_work_area_unit_wh
//	 ** viewable_work_area_unit_sizes
//
// This will also perform the actual scroll.
//
void VScrollWindow::ScrollData::scroll(VScrollWindow *sw,
									   long unit_dx, long unit_dy,
									   float offset_x, float offset_y)
{
	int		d = 0;
	int		unit_dxy[2];
	long	scroll_dxy[2];
	float	offset_xy[2];
	int		view_area_unit_size, fraction, scroll;
	int		unit_size, total_unit_size, unit_index;
	long	unit, first_unit, inrange_work_area_size;
	long	old_viewable_work_area_unit_xy;
	int		old_viewable_work_area_pixel_offset_xy;

	unit_dxy[0]	  = unit_dx;	unit_dxy[1]	  = unit_dy;
	offset_xy[0]  = offset_x;	offset_xy[1]  = offset_y;
	scroll_dxy[0] = 0;			scroll_dxy[1] = 0;

  Begin:

  	if (!scroll_needed) {
		goto End;
	}

	old_viewable_work_area_unit_xy = viewable_work_area_unit_xy;
	old_viewable_work_area_pixel_offset_xy =
		viewable_work_area_pixel_offset_xy;

	if (unit_dxy[d] > 0) {
		//
		// Scrolling work-area to the right.
		//
		if (unit_dxy[d] > viewable_work_area_unit_xy) {
			//
			// Too much; set it to the max.
			//
			if ((unit_dxy[d] = viewable_work_area_unit_xy) == 0) {
				goto EndUnitDxy;
			}
		}
		if (scroll_unit_wh == 1) {
			//
			// Pixel-unit scrolling -- to the right.
			//
			viewable_work_area_unit_xy -= unit_dxy[d];
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			view_area_extra_pixel_wh =
				int(view_area_pixel_wh - inrange_work_area_size);
			if (view_area_extra_pixel_wh < 0) {
				view_area_extra_pixel_wh = 0;
			}
			scroll_dxy[d] = unit_dxy[d];
		}
		else if (scroll_unit_wh > 1) {
			//
			// Fixed-unit scrolling -- to the right.
			//
			viewable_work_area_unit_xy -= unit_dxy[d];
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			view_area_unit_size = view_area_pixel_wh / scroll_unit_wh;
			fraction = view_area_pixel_wh % scroll_unit_wh;
			if (fraction != 0) {
				view_area_unit_size++;
			}
			if (inrange_work_area_size > view_area_unit_size) {
				//
				// Here, there are one or more inrange work-area
				// units which are not viewable at all.
				//
				viewable_work_area_unit_wh = view_area_unit_size;
				if (fraction != 0) {
					view_area_extra_pixel_wh = fraction - scroll_unit_wh;
				}
				else {
					view_area_extra_pixel_wh = 0;
				}
			}
			else {
				//
				// Here, all inrange work-area units are viewable.
				//
				viewable_work_area_unit_wh = (int)inrange_work_area_size;
				view_area_extra_pixel_wh =
					view_area_pixel_wh -
					viewable_work_area_unit_wh * scroll_unit_wh;
			}
			scroll_dxy[d] = unit_dxy[d] * scroll_unit_wh;
			if ((scroll_dxy[d] / scroll_unit_wh) != unit_dxy[d]) {
				//
				// Overflow.
				//
				scroll_dxy[d] = 32767;
			}
		}
		else {
			//
			// Variable-unit scrolling -- to the right.
			//
			total_unit_size = 0;
			unit_index = viewable_work_area_unit_sizes->count();
			for (unit = viewable_work_area_unit_xy - unit_dxy[d];
				 unit < viewable_work_area_unit_xy ; unit++) {
				void *data;
				unit_size = retrieveVariableUnitSize(sw, d, unit, &data);
				viewable_work_area_unit_sizes->append(unit_size, data);
				if ((total_unit_size += unit_size) >= view_area_pixel_wh) {
					//
					// We've filled up the view-area with work-area
					// units; all currently viewable units are being
					// pushed out of view.
					//
					viewable_work_area_unit_sizes->
						first(unit_index, -total_unit_size);
					break;
				}
			}
			if (unit == viewable_work_area_unit_xy) {
				//
				// Some currently viewable units will still be viewable.
				//
				viewable_work_area_unit_sizes->
					first(unit_index, view_area_pixel_wh);
			}
			viewable_work_area_unit_wh =
				viewable_work_area_unit_sizes->count();
			view_area_extra_pixel_wh =
				view_area_pixel_wh - viewable_work_area_unit_sizes->sum();
			viewable_work_area_unit_xy -= unit_dxy[d];
			scroll_dxy[d] = total_unit_size;
		}
	}
	else if (unit_dxy[d] < 0) {
		//
		// Scrolling work-area to the left.
		//
		if ((viewable_work_area_unit_xy - unit_dxy[d]) >
			 max_viewable_work_area_unit_xy) {
			//
			// Too much; set it to the max.
			//
			unit_dxy[d] = viewable_work_area_unit_xy -
						  max_viewable_work_area_unit_xy;
			offset_xy[d] = max_viewable_work_area_pixel_offset_xy;
			if (unit_dxy[d] == 0) {
				goto EndUnitDxy;
			}
		}
		if (scroll_unit_wh == 1) {
			//
			// Pixel-unit scrolling -- to the left.
			//
			viewable_work_area_unit_xy -= unit_dxy[d];
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			view_area_extra_pixel_wh =
				int(view_area_pixel_wh - inrange_work_area_size);
			if (view_area_extra_pixel_wh < 0) {
				view_area_extra_pixel_wh = 0;
			}
			scroll_dxy[d] = unit_dxy[d];
		}
		else if (scroll_unit_wh > 1) {
			//
			// Fixed-unit scrolling -- to the left.
			//
			viewable_work_area_unit_xy -= unit_dxy[d];
			inrange_work_area_size =
				work_area_unit_wh - viewable_work_area_unit_xy;
			view_area_unit_size = view_area_pixel_wh / scroll_unit_wh;
			fraction = view_area_pixel_wh % scroll_unit_wh;
			if (fraction != 0) {
				view_area_unit_size++;
			}
			if (inrange_work_area_size > view_area_unit_size) {
				//
				// Here, there are one or more inrange work-area
				// units which are not viewable at all.
				//
				if (fraction != 0) {
					view_area_extra_pixel_wh = fraction - scroll_unit_wh;
				}
				else {
					view_area_extra_pixel_wh = 0;
				}
				viewable_work_area_unit_wh = view_area_unit_size;
			}
			else {
				//
				// Here, all inrange work-area units are viewable.
				//
				viewable_work_area_unit_wh = (int)inrange_work_area_size;
				view_area_extra_pixel_wh =
					view_area_pixel_wh -
					viewable_work_area_unit_wh * scroll_unit_wh;
			}
			scroll_dxy[d] = unit_dxy[d] * scroll_unit_wh;
			if ((scroll_dxy[d] / scroll_unit_wh) != unit_dxy[d]) {
				//
				// Overflow.
				//
				scroll_dxy[d] = 32767;
			}
		}
		else {
			//
			// Variable-unit scrolling -- to the left.
			//
			UnitDataArray old_viewable_work_area_unit_sizes;
			long old_work_area_unit_xy = viewable_work_area_unit_xy;
			viewable_work_area_unit_xy -= unit_dxy[d];
			if (-unit_dxy[d] >= viewable_work_area_unit_wh) {
				//
				// Here, all currently viewable work-area
				// units are being scrolled out of range.
				//
				viewable_work_area_unit_sizes->clear();
				first_unit = viewable_work_area_unit_xy;
				total_unit_size = 0;
				scroll_dxy[d] = -view_area_pixel_wh;
			}
			else {
				//
				// Here, some currently viewable work-area
				// units will remain viewable and inrange.
				//
				scroll_dxy[d] =
					-viewable_work_area_unit_sizes->sum(-unit_dxy[d]);
				viewable_work_area_unit_sizes->first(-unit_dxy[d]);
				first_unit = viewable_work_area_unit_xy +
							 viewable_work_area_unit_sizes->count();
				total_unit_size = viewable_work_area_unit_sizes->sum();
			}
			if (total_unit_size < view_area_pixel_wh) {
				for (unit = first_unit ; unit < work_area_unit_wh ; unit++) {
					void *data;
					unit_size = retrieveVariableUnitSize(sw, d, unit, &data);
					viewable_work_area_unit_sizes->append(unit_size, data);
					if ((total_unit_size += unit_size) >=
						 view_area_pixel_wh) {
						break;
					}
				}
			}
			if (viewable_work_area_unit_xy < old_work_area_unit_xy) {
				//
				// Oops, don't scroll back to the right any farther
				// than the current work-area position was.
				//
				viewable_work_area_unit_xy = old_work_area_unit_xy;
				scroll_dxy[d] = 0;
			}
			else {
				viewable_work_area_unit_wh =
					viewable_work_area_unit_sizes->count();
				view_area_extra_pixel_wh =
					view_area_pixel_wh - total_unit_size;
			}
		}
	}

  EndUnitDxy:

	if (scroll_unit_wh == 1) {
		goto End;
	}

	if (viewable_work_area_pixel_offset_xy > 0) {
		if (scroll_dxy[d] != 0) {
			if (abs(scroll_dxy[d]) < view_area_pixel_wh) {
				scroll_dxy[d] += viewable_work_area_pixel_offset_xy;
			}
			viewable_work_area_pixel_offset_xy = 0;
		}
	}

	if (offset_xy[d] < 0.0F) {
		if (scroll_dxy[d] == 0) {
			goto End;
		}
		else {
			fraction = 0;
		}
	}
	else if (offset_xy[d] == 0.0F) {
		fraction = 0;
	}
	else if (offset_xy[d] >= 1) {
		fraction = int(offset_xy[d]);
	}
	else {
		unit_size = scroll_unit_wh > 1 ?
					scroll_unit_wh : viewable_work_area_unit_sizes->size(0);
		fraction = int(offset_xy[d] * float(unit_size) + 0.5);
		if (fraction >= unit_size) {
			fraction = unit_size - 1;
		}
	}

	scroll = scroll_dxy[d] != 0 ?
			 -fraction : viewable_work_area_pixel_offset_xy - fraction;

	scroll = setScrollPixelOffsetData(sw, d, scroll);

	if ((scroll_unit_wh <= 0) &&
		(scroll < 0) &&
		(scroll_dxy[d] >= view_area_pixel_wh)) {
		if ((old_viewable_work_area_unit_xy > viewable_work_area_unit_xy) &&
			(old_viewable_work_area_unit_xy < viewable_work_area_unit_xy +
											  viewable_work_area_unit_wh)) {
			scroll_dxy[d] =
				viewable_work_area_unit_sizes->
					sum(old_viewable_work_area_unit_xy -
						viewable_work_area_unit_xy) -
					viewable_work_area_pixel_offset_xy +
					old_viewable_work_area_pixel_offset_xy;
		}
	}
	else {
		//
		// Overflow?
		//
		scroll_dxy[d] += scroll;
	}

  End:

	if (d == 0) {
		d = 1;
		goto Begin;
	}

	sw->performScroll(scroll_dxy[0], scroll_dxy[1]);
}

// --------------------------------------------------------------------------
// Set the pixel offset within the unit within the first inrange work-area
// unit; this a represents the number of pixels within the first inrange
// work-area unit which is to be scrolled out of range.  This is how smooth
// scrolling with fixed-unit or variable-unit scrolling is achieved.  Scroll
// this number of pixels to the left, but not more than the size of the first
// viewable unit.  It is *assumed* that this is *not* pixel-unit scrolling.

int VScrollWindow::ScrollData
				 ::setScrollPixelOffsetData(VScrollWindow *sw, int d,
											long pixel_dxy)
{
	int unit_size;
	long unit, inrange_work_area_size;

	viewable_work_area_pixel_offset_xy -= (int)pixel_dxy;

	if ((viewable_work_area_unit_xy == max_viewable_work_area_unit_xy) &&
		(viewable_work_area_pixel_offset_xy >
		 max_viewable_work_area_pixel_offset_xy)) {
		viewable_work_area_pixel_offset_xy += (int)pixel_dxy;
		return 0;
	}

	if (pixel_dxy > 0) {
		if (viewable_work_area_pixel_offset_xy < 0) {
			if ((pixel_dxy += viewable_work_area_pixel_offset_xy) == 0) {
				return 0;
			}
			viewable_work_area_pixel_offset_xy = 0;
		}
	}
	else if (pixel_dxy < 0) {
		unit_size = scroll_unit_wh > 1 ?
					scroll_unit_wh : viewable_work_area_unit_sizes->size(0);
		if (viewable_work_area_pixel_offset_xy >= unit_size) {
			pixel_dxy += viewable_work_area_pixel_offset_xy - unit_size + 1;
			viewable_work_area_pixel_offset_xy = unit_size - 1;
		}
	}
	else {
		return 0;
	}

	if (scroll_unit_wh > 1) {
		//
		// Fixed-unit scrolling.
		//
		if (view_area_extra_pixel_wh > 0) {
			//
			// Here, there is extra space in the view-area;
			// all inrange work-area units are viewable.
			//
			view_area_extra_pixel_wh -= (int)pixel_dxy;
		}
		else {
			//
			// Here, there is no extra space in the view-area.
			//
			view_area_extra_pixel_wh =
				view_area_pixel_wh -
				((viewable_work_area_unit_wh * scroll_unit_wh) -
				 viewable_work_area_pixel_offset_xy);
			if (view_area_extra_pixel_wh > 0) {
				inrange_work_area_size =
					work_area_unit_wh - viewable_work_area_unit_xy;
				if (inrange_work_area_size >
					viewable_work_area_unit_wh) {
					//
					// Here, there are one or more inrange work-area
					// units which are not viewable at all.
					//
					view_area_extra_pixel_wh -= scroll_unit_wh;
					viewable_work_area_unit_wh++;
				}
			}
		}
	}
	else {
		//
		// Variable-unit scrolling.
		//
		if (pixel_dxy > 0) {
			viewable_work_area_unit_sizes->
				truncate(view_area_pixel_wh +
						 viewable_work_area_pixel_offset_xy);
			viewable_work_area_unit_wh =
				viewable_work_area_unit_sizes->count();
			view_area_extra_pixel_wh =
				view_area_pixel_wh -
				(viewable_work_area_unit_sizes->sum() -
				 viewable_work_area_pixel_offset_xy);
		}
		else if ((view_area_extra_pixel_wh > 0) ||
				 ((view_area_extra_pixel_wh < 0) &&
				  (view_area_extra_pixel_wh <= pixel_dxy))) {
			view_area_extra_pixel_wh =
				view_area_pixel_wh -
				(viewable_work_area_unit_sizes->sum() -
				 viewable_work_area_pixel_offset_xy);
		}
		else {
			view_area_extra_pixel_wh =
				-(pixel_dxy - view_area_extra_pixel_wh);
			for (unit = viewable_work_area_unit_xy +
						viewable_work_area_unit_wh ;
				 unit < work_area_unit_wh ; unit++) {
				void *data;
				unit_size = retrieveVariableUnitSize(sw, d, unit, &data);
				viewable_work_area_unit_sizes->append(unit_size, data);
				if ((view_area_extra_pixel_wh -= unit_size) <= 0) {
					break;
				}
			}
			viewable_work_area_unit_wh =
				viewable_work_area_unit_sizes->count();
		}
	}
	return int(pixel_dxy);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData
				  ::scrollLine(VScrollWindow *sw, long nlx, long nly)
{
	int d = 0;
	long nlxy[2], unit_dxy[2]; float offset_xy[2];

	nlxy[0] = nlx;
	nlxy[1] = nly;

  Begin:

	if (smooth_scrolling) {
		int pixel_offset_xy;
		getScrollPixelData(sw, d, nlxy[d], unit_dxy[d], pixel_offset_xy);
		offset_xy[d] = float(pixel_offset_xy);
	}
	else {
		unit_dxy[d] = nlxy[d];
		offset_xy[d] = -1.0F;
	}

	if (d == 0) {
		d = 1;
		goto Begin;
	}

	scroll(sw, unit_dxy[0], unit_dxy[1], offset_xy[0], offset_xy[1]);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData
				  ::scrollPage(VScrollWindow *sw, long npx, long npy)
{
	int d = 0; long npxy[2], unit_dxy[2]; float offset_xy[2];

  	npxy[0] = npx; offset_xy[0] = -1.0F;
  	npxy[1] = npy; offset_xy[1] = -1.0F;

  Begin:

  	unit_dxy[d] = 0;

	if ((npxy[d] < 0) || (scroll_unit_wh > 0)) {
		if (smooth_scrolling &&
			 (view_area_extra_pixel_wh < 0) &&
			 ((viewable_work_area_unit_xy +
			   viewable_work_area_unit_wh) == work_area_unit_wh)) {
			unit_dxy[d] = 0;
			if (npxy[d] < 0) {
				offset_xy[d] = viewable_work_area_pixel_offset_xy -
							   view_area_extra_pixel_wh - 1.0F;
			}
			else {
				offset_xy[d] = viewable_work_area_pixel_offset_xy +
							   view_area_extra_pixel_wh + 1.0F;
			}
		}
		else {
			int n = viewable_work_area_unit_wh;
			if ((view_area_extra_pixel_wh < 0) && (n > 1)) {
				n--;
			}
			unit_dxy[d] = npxy[d] * n;
		}
	}
	else {
		while (npxy[d]-- > 0) {
			long unit; int total_unit_size = 0;
			for (unit = viewable_work_area_unit_xy - 1 ; unit >= 0 ;
				 unit--) {
				total_unit_size += retrieveVariableUnitSize(sw, d, unit);
				if (total_unit_size > view_area_pixel_wh) {
					unit_dxy[d] = viewable_work_area_unit_xy - unit - 1;
					if (unit_dxy[d] == 0) {
						unit_dxy[d] = 1;
					}
					break;
				}
				else if (total_unit_size == view_area_pixel_wh) {
					unit_dxy[d] = viewable_work_area_unit_xy - unit;
					break;
				}
			}
			if (unit < 0) {
				unit_dxy[d] = viewable_work_area_unit_xy;
			}
		}
		if ((unit_dxy[d] == 0) && smooth_scrolling &&
			(viewable_work_area_pixel_offset_xy > 0)) {
			offset_xy[d] = 0.0F;
		}
	}

	if (d == 0) {
		d = 1;
		goto Begin;
	}

	scroll(sw, unit_dxy[0], unit_dxy[1], offset_xy[0], offset_xy[1]);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData
				  ::scrollHome(VScrollWindow *sw, boolean h, boolean v)
{
	if (h) {
		if (v) {
			scrollTo(sw, 0, 0);
		}
		else {
			scrollVerticalTo(sw, 0);
			scrollHorizontalTo(sw, 0);
		}
	}
	else if (v) {
		scrollVerticalTo(sw, 0);
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData
				  ::scrollEnd(VScrollWindow *sw, boolean h, boolean v)
{
	if (h) {
		if (v) {
		}
		else {
		}
	}
	else if (v) {
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData
				  ::scrollPixel(VScrollWindow *sw,
								long pixel_dx, long pixel_dy)
{
	if ((scroll_unit_w != 1) && !smooth_scrolling_x) {
		pixel_dx = 0;
	}
	if ((scroll_unit_h != 1) && !smooth_scrolling_y) {
		pixel_dy = 0;
	}
	scrollLine(sw, pixel_dx, pixel_dy);
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ScrollData
					 ::scrollRatioTo(VScrollWindow *sw, float x, float y)
{
	int d = 0; long unit_xy[2]; float xy[2], offset_xy[2];

	xy[0] = x; xy[1] = y;

  Begin:

  	if ((xy[d] >= 0.0F) && (xy[d] <= 1.0F)) {

		if (sw->dimData[d].scaleScrollValues) {
			float f = xy[d] * float(max_viewable_work_area_unit_xy) + 0.5F;
			unit_xy[d] = long(f);
			offset_xy[d] = f - unit_xy[d];
			
		}
		else {
			unit_xy[d] = long(xy[d] * max_viewable_work_area_unit_xy + 0.5F);
			offset_xy[d] = 0.0F;
		}
	}
	else {
		unit_xy[d] = viewable_work_area_unit_x;
		offset_xy[d] = -1.0F;
	}

	if (d == 0) {
		d = 1;
		goto Begin;
	}

	if ((unit_xy[0]   == viewable_work_area_unit_x) &&
		(unit_xy[1]   == viewable_work_area_unit_y) &&
		(offset_xy[0] == viewable_work_area_pixel_offset_x) &&
		(offset_xy[0] == viewable_work_area_pixel_offset_y)) {
		return FALSE;
	}
	else {
		scrollTo(sw, unit_xy[0], unit_xy[1], offset_xy[0], offset_xy[1]);
		return TRUE;
	}
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ScrollData
					 ::scrollHorizontalRatioTo(VScrollWindow *sw, float x)
{
	long unit_x; float offset_x;
	if (sw->dimData[0].scaleScrollValues) {
		unit_x = long(x * maxViewableWorkAreaUnitXY[0]);
		if (maxViewableWorkAreaUnitXY[0] < MaxScrollValue) {
			offset_x = x - (float(unit_x) /
							float(maxViewableWorkAreaUnitXY[0]));
		}
		else {
			offset_x = 0.0F;
		}
	}
	else {
		unit_x = long(x * maxViewableWorkAreaUnitXY[0]);
		offset_x = -1.0F;
	}
	if ((unit_x == viewable_work_area_unit_x) &&
		(offset_x == viewable_work_area_pixel_offset_x)) {
		return FALSE;
	}
	else {
		scrollHorizontalTo(sw, unit_x, offset_x);
		return TRUE;
	}
}

// --------------------------------------------------------------------------
//
boolean VScrollWindow::ScrollData
					 ::scrollVerticalRatioTo(VScrollWindow *sw, float y)
{
	long unit_y; float offset_y;
	if (sw->dimData[1].scaleScrollValues) {
		unit_y = long(y * maxViewableWorkAreaUnitXY[1]);
		if (maxViewableWorkAreaUnitXY[1] < MaxScrollValue) {
			offset_y = y - (float(unit_y) /
							float(maxViewableWorkAreaUnitXY[1]));
		}
		else {
			offset_y = 0.0F;
		}
	}
	else {
		unit_y = long(y * maxViewableWorkAreaUnitXY[1]);
		offset_y = -1.0F;
	}
	if ((unit_y == viewable_work_area_unit_y) &&
		(offset_y == viewable_work_area_pixel_offset_y)) {
		return FALSE;
	}
	else {
		scrollVerticalTo(sw, unit_y, offset_y);
		return TRUE;
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::
					scrollIntoView(VScrollWindow *sw, int d, long *uxy)
{
	boolean	do_both_dimensions;
	long	unit_xy[2];
	float	offset_xy[2];
	
	if (d < 0) {
		d = 0;
		do_both_dimensions = TRUE;
	}
	else if (d == 1) {
		unit_xy[0] = viewableWorkAreaUnitXY[0];
		offset_xy[0] = viewableWorkAreaPixelOffsetXY[0];
		do_both_dimensions = FALSE;
	}
	else {
		unit_xy[1] = viewableWorkAreaUnitXY[1];
		offset_xy[1] = viewableWorkAreaPixelOffsetXY[1];
		do_both_dimensions = FALSE;
	}

  Begin:

	if (uxy[d] < viewable_work_area_unit_xy) {
		//
		// Here, the specified unit is not visible at all;
		// it is out of range, i.e. before (to the left/top
		// of) the first viewable unit.
		//
		unit_xy[d] = uxy[d];
		offset_xy[d] = -1.0F;
	}
	else if (uxy[d] >= viewable_work_area_unit_xy +
					   viewable_work_area_unit_wh) {
		//
		// Here, the specified unit is not visible at all;
		// it is in range but out of view, i.e. after (to
		// the right/bottom of) the last viewable unit.
		//
		int size = 0; long unit;
		for (unit = uxy[d] - 1; unit >= 0 ; unit--) {
			size += retrieveUnitSize(sw, d, unit);
			if (size >= view_area_pixel_wh) {
				unit++;
				break;
			}
		}
		//
		// Even for smooth scrolling, we will scroll into
		// view on unit boundaries; this seem most sensible.
		//
		unit_xy[d] = unit;
		offset_xy[d] = -1.0F;
	}
	else {
		//
		// Here, the specified unit is visible, at least paritally.
		//
		if ((viewable_work_area_unit_xy == uxy[d]) &&
			(viewable_work_area_pixel_offset_xy > 0)) {
			//
			// Here, the specified unit is partially visible;
			// the left/top portion of it is out of range.
			//
			unit_xy[d] = uxy[d];
			offset_xy[d] = 0.0F;
		}
		else if ((uxy[d] == viewable_work_area_unit_xy +
							viewable_work_area_unit_wh - 1) &&
				 (view_area_extra_pixel_wh < 0)) {
			//
			// Here, the specified unit is partially visible;
			// the right/bottom portion is out of view.
			//
			int size = view_area_extra_pixel_wh;
			long first_unit = viewable_work_area_unit_xy, unit;
			if (viewable_work_area_pixel_offset_xy > 0) {
				size += retrieveUnitSize(sw, d, viewable_work_area_unit_xy) -
						viewable_work_area_pixel_offset_xy;
				if (size >= 0) {
					unit_xy[d] = viewable_work_area_unit_xy + 1;
					offset_xy[d] = -1.0F;
					goto End;
				}
				first_unit++;
			}
			for (unit = first_unit ; unit < uxy[d] ; unit++) {
				size += retrieveUnitSize(sw, d, unit);
				if (size >= 0) {
					break;
				}
			}
			unit_xy[d] = uxy[d] > unit ? unit + 1 : unit;
			offset_xy[d] = -1.0F;
		}
		else {
			//
			// Here, the specified unit is totally visible; do nothing.
			//
			unit_xy[d] = viewable_work_area_unit_xy;
			offset_xy[d] = viewable_work_area_pixel_offset_xy;
		}
	}

  End:

	if (do_both_dimensions) {
		do_both_dimensions = FALSE;
		d = 1;
		goto Begin;
	}

	scrollTo(sw, unit_xy[0], unit_xy[1], offset_xy[0], offset_xy[1]);
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData
				  ::getScrollPixelData(const VScrollWindow *sw, int d,
									   long pixel_dxy,
									   long& unit_dxy, int& pixel_offset_xy)
{
	if (scroll_unit_wh == 1) {
		unit_dxy = pixel_dxy;
		return;
	}
	else if (smooth_scrolling) {
		if (pixel_dxy > 0) {
			if (viewable_work_area_pixel_offset_xy > 0) {
				if (viewable_work_area_pixel_offset_xy < pixel_dxy) {
					pixel_dxy -= viewable_work_area_pixel_offset_xy;
					unit_dxy = 0;
					pixel_offset_xy = 0;
				}
				else {
					unit_dxy = 0;
					pixel_offset_xy =
						viewable_work_area_pixel_offset_xy - pixel_dxy;
					return;
				}
			}
			else {
				unit_dxy = 0;
				pixel_offset_xy = 0;
			}
			if ((pixel_dxy > 0) && (viewable_work_area_unit_xy > 0)) {
				if (scroll_unit_wh > 1) {
					unit_dxy = pixel_dxy / scroll_unit_wh + 1;
					int fraction = pixel_dxy % scroll_unit_wh;
					if (fraction > 0) {
						pixel_offset_xy = scroll_unit_wh - fraction;
					}
				}
				else {
					for (long unit = viewable_work_area_unit_xy - 1 ;
						 unit >= 0 ; unit--) {
						int unit_size =
							retrieveVariableUnitSize(sw, d, unit);
						unit_dxy++;
						if ((pixel_dxy -= unit_size) < 0) {
							pixel_offset_xy = -pixel_dxy;
							break;
						}
						else if (pixel_dxy == 0) {
							pixel_offset_xy = 0;
							break;
						}
					}
				}
			}
			return;
		}
		else if (pixel_dxy < 0) {
			if (viewable_work_area_pixel_offset_xy > 0) {
				int unit_size =
					scroll_unit_wh > 1 ?
					scroll_unit_wh : viewable_work_area_unit_sizes->size(0);
				if (unit_size -
					viewable_work_area_pixel_offset_xy <= -pixel_dxy) {
					if (viewable_work_area_unit_xy <
						max_viewable_work_area_unit_xy) {
						pixel_dxy +=
							unit_size - viewable_work_area_pixel_offset_xy;
						unit_dxy = -1;
						pixel_offset_xy = 0;
					}
					else {
						pixel_dxy = 0;
						unit_dxy = 0;
						pixel_offset_xy = -1;
					}
				}
				else {
					unit_dxy = 0;
					pixel_offset_xy =
						viewable_work_area_pixel_offset_xy - pixel_dxy;
					return;
				}
			}
			else {
				unit_dxy = 0;
				pixel_offset_xy = 0;
			}
			if (pixel_dxy < 0) {
				if (scroll_unit_wh > 1) {
					unit_dxy += pixel_dxy / scroll_unit_wh;
					int fraction = -pixel_dxy % scroll_unit_wh;
					if (fraction > 0) {
						pixel_offset_xy = fraction;
					}
				}
				else {
					for (long unit = viewable_work_area_unit_xy ;
						 unit < work_area_unit_wh ; unit++) {
						int unit_size =
							retrieveVariableUnitSize(sw, d, unit);
						if ((pixel_dxy += unit_size) > 0) {
							pixel_offset_xy = unit_size - pixel_dxy;
							break;
						}
						else if (pixel_dxy == 0) {
							pixel_offset_xy = 0;
							break;
						}
						else if (viewable_work_area_unit_xy + unit_dxy >=
								 max_viewable_work_area_unit_xy) {
							break;
						}
						else {
							unit_dxy--;
						}
					}
				}
			}
			return;
		}
	}
	unit_dxy = 0;
	pixel_offset_xy = -1;
}

// --------------------------------------------------------------------------
// Figure out the maximum viewable work-area unit position.
// This is so we can set the range of any scroll-bars correctly.
//
void VScrollWindow::ScrollData
				  ::setMaxViewableWorkArea(VScrollWindow *sw, int d)
{
	boolean do_both_dimensions;

	if ((d != 0) && (d != 1)) {
		d = 0;
		do_both_dimensions = TRUE;
	}
	else {
		do_both_dimensions = FALSE;
	}

  Begin:

	if (scroll_unit_wh == 1) {
		//
		// Pixel-unit scrolling.
		//
		if (maximize_viewable_work_area) {
			max_viewable_work_area_unit_xy =
				work_area_unit_wh - view_area_pixel_wh;
		}
		else {
			max_viewable_work_area_unit_xy = work_area_unit_wh - 1;
		}
	}
	else if (scroll_unit_wh > 1) {
		//
		// Fixed-unit scrolling.
		//
		if (maximize_viewable_work_area) {
			max_viewable_work_area_unit_xy =
				work_area_unit_wh - view_area_pixel_wh / scroll_unit_wh;
			if (smooth_scrolling) {
				int fraction = view_area_pixel_wh % scroll_unit_wh;
				if (fraction != 0) {
					max_viewable_work_area_unit_xy--;
					max_viewable_work_area_pixel_offset_xy =
						scroll_unit_wh - fraction;
				}
				else {
					max_viewable_work_area_pixel_offset_xy = 0;
				}
			}
		}
		else {
			max_viewable_work_area_unit_xy = work_area_unit_wh - 1;
			if (smooth_scrolling) {
				max_viewable_work_area_pixel_offset_xy = scroll_unit_wh - 1;
			}
		}
	}
	else {
		//
		// Variable-unit scrolling.
		//
		if (maximize_viewable_work_area) {
			//
			// I really hate doing this; after all the trouble I went to
			// to minimize the row/column size queries to the model to only
			// those occasions where they're actually coming into view.
			//
			int unit_size, total_unit_size = 0;
			long unit, first_unit = work_area_unit_wh - 1;
			for (unit = first_unit ; unit >= 0 ; unit--) {
				unit_size = retrieveVariableUnitSize(sw, d, unit);
				if ((total_unit_size += unit_size) > view_area_pixel_wh) {
					if (smooth_scrolling) {
						max_viewable_work_area_unit_xy =
							work_area_unit_wh - (first_unit - unit) - 1;
						max_viewable_work_area_pixel_offset_xy =
							total_unit_size - view_area_pixel_wh;
					}
					else if (unit == first_unit) {
						max_viewable_work_area_unit_xy =
							work_area_unit_wh - 1;
					}
					else {
						max_viewable_work_area_unit_xy =
							work_area_unit_wh - (first_unit - unit);
					}
					break;
				}
				else if (total_unit_size == view_area_pixel_wh) {
					max_viewable_work_area_unit_xy =
						work_area_unit_wh - (first_unit - unit) - 1;
					if (smooth_scrolling) {
						max_viewable_work_area_pixel_offset_xy = 0;
					}
					break;
				}
			}
			if (unit == -1) {
				max_viewable_work_area_unit_xy = 0;
				if (smooth_scrolling) {
					if (view_area_pixel_wh < unit_size) {
						max_viewable_work_area_pixel_offset_xy =
							unit_size - view_area_pixel_wh;
					}
					else {
						max_viewable_work_area_pixel_offset_xy = 0;
					}
				}
			}
		}
		else {
			max_viewable_work_area_unit_xy = work_area_unit_wh - 1;
			if (smooth_scrolling) {
				int unit_size =
					retrieveVariableUnitSize(sw, d, work_area_unit_wh - 1);
				max_viewable_work_area_pixel_offset_xy = unit_size - 1;
			}
			else {
				max_viewable_work_area_pixel_offset_xy = 0;
			}
		}
	}

	if (do_both_dimensions) {
		d = 1;
		do_both_dimensions = FALSE;
		goto Begin;
	}
}

// ==========================================================================
// ::: VScrollWindow::ScrollData::UnitDataArray
// ==========================================================================

// --------------------------------------------------------------------------
//
VScrollWindow::ScrollData::UnitDataArray::UnitDataArray()
{
	// Note that lastUnitDataIndex points to the next slot *after*
	// the actual last item in the array; lastUnitDataIndex can
	// only be zero if firstUnitDataIndex is also zero, in which
	// case the array is empty; it is full if lastUnitDataIndex equals
	// firstUnitDataIndex but is not zero, or if firstUnitDataIndex
	// is zero then if lastUnitDataIndex equals unitDataArraySize.

	unitDataArray	   = 0;
	unitDataArraySize  = 0;
	firstUnitDataIndex = 0;
	lastUnitDataIndex  = 0;
	sumOfUnitSizes	   = 0;
}

// --------------------------------------------------------------------------
//
VScrollWindow::ScrollData::UnitDataArray::~UnitDataArray()
{
	if (unitDataArray != 0) { delete [] unitDataArray;	}
}

// --------------------------------------------------------------------------
//
int VScrollWindow::ScrollData::UnitDataArray::allocateUnitDataArray()
{
	unitDataArraySize = InitialArraySize;
	unitDataArray = new UnitData[unitDataArraySize];
	return unitDataArray != 0 ? 1 : 0;
}

// --------------------------------------------------------------------------
//
int VScrollWindow::ScrollData::
				   UnitDataArray::incrementUnitDataArraySize()
{
	int size = unitDataArraySize + IncrementalArraySize, i;
	UnitData *array = new UnitData[size]; if (array == 0) { return 0; }
	for (i = 0 ; i < unitDataArraySize ; i++) {
		array[i] = unitDataArray[firstUnitDataIndex++];
		if (firstUnitDataIndex == lastUnitDataIndex) {
			i++;
			break;
		}
		if (firstUnitDataIndex == unitDataArraySize) {
			firstUnitDataIndex = 0;
		}
	}
	delete [] unitDataArray;
	unitDataArray = array;
	unitDataArraySize = size;
	firstUnitDataIndex = 0;
	lastUnitDataIndex = i;
	return 1;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray::append(int size, void *data)
{
	if (unitDataArray == 0) {
		if (!allocateUnitDataArray()) {
			return;
		}
	}
	if (lastUnitDataIndex > firstUnitDataIndex) {
		if (lastUnitDataIndex < unitDataArraySize) {
			unitDataArray[lastUnitDataIndex].size = size;
			unitDataArray[lastUnitDataIndex].data = data;
			lastUnitDataIndex++;
		}
		else {
			if (firstUnitDataIndex == 0) {
				if (!incrementUnitDataArraySize()) {
					return;
				}
				unitDataArray[lastUnitDataIndex].size = size;
				unitDataArray[lastUnitDataIndex].data = data;
				lastUnitDataIndex++;
			}
			else {
				unitDataArray[0].size = size;
				unitDataArray[0].data = data;
				lastUnitDataIndex = 1;
			}
		}
	}
	else if (lastUnitDataIndex < firstUnitDataIndex)  {
		unitDataArray[lastUnitDataIndex].size = size;
		unitDataArray[lastUnitDataIndex].data = data;
		lastUnitDataIndex++;
	}
	else {
		if (firstUnitDataIndex != 0) {
			if (!incrementUnitDataArraySize()) {
				return;
			}
		}
		unitDataArray[lastUnitDataIndex].size = size;
		unitDataArray[lastUnitDataIndex].data = data;
		lastUnitDataIndex++;
	}
	sumOfUnitSizes += size;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray::prepend(int size, void *data)
{
	if (unitDataArray == 0) {
		if (!allocateUnitDataArray()) {
			return;
		}
	}
	if (firstUnitDataIndex == 0) {
		if (lastUnitDataIndex == unitDataArraySize) {
			if (!incrementUnitDataArraySize()) {
				return;
			}
		}
		firstUnitDataIndex = unitDataArraySize - 1;
	}
	else if (lastUnitDataIndex == firstUnitDataIndex) {
		if (!incrementUnitDataArraySize()) {
			return;
		}
		firstUnitDataIndex = unitDataArraySize - 1;
	}
	else {
		firstUnitDataIndex--;
	}
	unitDataArray[firstUnitDataIndex].size = size;
	unitDataArray[firstUnitDataIndex].data = data;
	sumOfUnitSizes += size;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::
					UnitDataArray::prepend(const UnitDataArray& uda)
{
	for (int i = uda.count() - 1 ; i >= 0 ; i--) {
		prepend(uda.size(i), uda.data(i));
	}
}

// --------------------------------------------------------------------------
// Sets the first item in the array to be the item of the given index.
// If the given sum argument is greater than zero then the items which were
// in front of the new first item will be put at the end of the array and
// the array will also be truncated such that the sum of all the unit sizes
// in the array does not exceed that sum by more than one unit.  Otherwise,
// the items which were in front of the new first item will be discarded and
// if the given sum argument is less than zero then it is *assumed* to be
// the negative of the correct sum of the units now in the array.
//
void VScrollWindow::ScrollData::UnitDataArray::first(int index, int sum)
{
	if ((unitDataArray == 0) || (index <= 0)) {
		return;
	}
	if (firstUnitDataIndex < lastUnitDataIndex) {
		int old_first_unit_data_index = firstUnitDataIndex;
		if ((firstUnitDataIndex += index) >= lastUnitDataIndex) {
			firstUnitDataIndex =
				lastUnitDataIndex - old_first_unit_data_index - 1;
		}
		if (sum > 0) {
			//
			// Place items which were first, last.
			//
			if ((firstUnitDataIndex > 0) ||
				(lastUnitDataIndex < unitDataArraySize)) {
				while (1) {
					if (lastUnitDataIndex == unitDataArraySize) {
						lastUnitDataIndex = 0;
					}
					unitDataArray[lastUnitDataIndex] =
						unitDataArray[old_first_unit_data_index++];
					lastUnitDataIndex++;
					if (old_first_unit_data_index == firstUnitDataIndex) {
						break;
					}
				}
			}
			else {
				lastUnitDataIndex = firstUnitDataIndex;
			}
			truncate(sum);
			return;
		}
	}
	else if (lastUnitDataIndex < firstUnitDataIndex) {
		int old_first_unit_data_index = firstUnitDataIndex;
		if ((firstUnitDataIndex += index) >= unitDataArraySize) {
			firstUnitDataIndex =
				index - (unitDataArraySize - old_first_unit_data_index);
			if (firstUnitDataIndex >= lastUnitDataIndex) {
				firstUnitDataIndex = lastUnitDataIndex - 1;
			}
		}
		if (sum > 0) {
			//
			// Place items which were first, last.
			//
			while (1) {
				unitDataArray[lastUnitDataIndex++] =
					unitDataArray[old_first_unit_data_index++];
				if (old_first_unit_data_index == unitDataArraySize) {
					old_first_unit_data_index = 0;
				}
				if (old_first_unit_data_index == firstUnitDataIndex) {
					break;
				}
				if (lastUnitDataIndex == unitDataArraySize) {
					lastUnitDataIndex = 0;
				}
			}
			truncate(sum);
			return;
		}
	}
	else if (firstUnitDataIndex > 0) {
		if (firstUnitDataIndex + index >= unitDataArraySize) {
			firstUnitDataIndex =
				index - (unitDataArraySize - firstUnitDataIndex);
			if (firstUnitDataIndex == lastUnitDataIndex) {
				firstUnitDataIndex--;
			}
		}
		else {
			firstUnitDataIndex += index;
		}
		if (sum > 0) {
			//
			// Place items which were first, last.
			//
			lastUnitDataIndex = firstUnitDataIndex;
			truncate(sum);
			return;
		}
	}
	else {
		//
		// Empty.
		//
	}
	if (sum < 0) {
		//
		// Discared items which were first.
		// Assume the given value is the negative of the correct new sum.
		//
		sumOfUnitSizes = -sum;
	}
	else {
		//
		// Discared items which were first.
		//
		sumup();
	}
}

// --------------------------------------------------------------------------
// Truncates the array such that the sum of all the unit sizes in the array
// does not exceed the given sum by more than one unit.  I.e. removes units
// from the end of the array which are totally out of range of the given sum
// value.
//
void VScrollWindow::ScrollData::UnitDataArray::truncate(int sum)
{
	if ((sumOfUnitSizes <= sum) || (unitDataArray == 0)) {
		return;
	}
	for (int i = lastUnitDataIndex - 1 ; 1 ; i--) {
		if (i < 0) {
			i = unitDataArraySize - 1;
		}
		if ((sumOfUnitSizes -= unitDataArray[i].size) < sum) {
			lastUnitDataIndex = i + 1;
			sumOfUnitSizes += unitDataArray[i].size;
			break;
		}
		if (i == firstUnitDataIndex) {
			sumOfUnitSizes += unitDataArray[i].size;
			break;
		}
	}
}

// --------------------------------------------------------------------------
//
VScrollWindow::ScrollData::UnitData&
	VScrollWindow::ScrollData::UnitDataArray::get(int index) const
{
	if (firstUnitDataIndex == 0) {
		if (index < lastUnitDataIndex) {
			return unitDataArray[index];
		}
	}
	else if (lastUnitDataIndex > firstUnitDataIndex) {
		index += firstUnitDataIndex;
		if (index < lastUnitDataIndex) {
			return unitDataArray[index];
		}
	}
	else {
		index += firstUnitDataIndex;
		if (index < unitDataArraySize) {
			return unitDataArray[index];
		}
		else {
			index -= firstUnitDataIndex +
					 (unitDataArraySize - firstUnitDataIndex);
			if (index < lastUnitDataIndex) {
				return unitDataArray[index];
			}
		}
	}
	static int first_time = 1; static UnitData dummy;
	if (first_time) { first_time = 0; dummy.size = 0; }
	return dummy;
}

// --------------------------------------------------------------------------
// Return the index of the unit data item which contains the given point.
// If the point is out of range then return negative.
//
int VScrollWindow::ScrollData::UnitDataArray::find(int point) const
{
	if ((lastUnitDataIndex == 0) && (firstUnitDataIndex == 0)) {
		return -1;
	}
	int sum = 0;
	for (int i = firstUnitDataIndex ; 1 ; ) {
		sum += unitDataArray[i].size;
		if (sum > point) {
			if (firstUnitDataIndex == 0) {
				return i;
			}
			else if ((firstUnitDataIndex < lastUnitDataIndex) || // necessary?
					 (i >= firstUnitDataIndex)) {
				return i - firstUnitDataIndex;
			}
			else {
				return i + (unitDataArraySize - firstUnitDataIndex);
			}
		}
		else if (++i == lastUnitDataIndex) {
			break;
		}
		else if (i == unitDataArraySize) {
			i = 0;
		}
	}
	return -2;
}

// --------------------------------------------------------------------------
// Return the sum of the first n units.
//
int VScrollWindow::ScrollData::UnitDataArray::sum(int n) const
{
	int sum_of_unit_sizes = 0;
	if (unitDataArray != 0) {
		for (int i = firstUnitDataIndex ; n > 0 ; n--) {
			sum_of_unit_sizes += unitDataArray[i].size;
			if (++i == lastUnitDataIndex) {
				return sum_of_unit_sizes;
			}
			if (i == unitDataArraySize) {
				i = 0;
			}
		}
	}
	return sum_of_unit_sizes;
}

// --------------------------------------------------------------------------
// Return the sum of the first n units starting at unit s.
//
int VScrollWindow::ScrollData::UnitDataArray::sum(int s, int n) const
{
	int sum_of_unit_sizes = 0;
	if (unitDataArray != 0) {
		int i = firstUnitDataIndex;
		if (i >= lastUnitDataIndex) {
			return 0;
		}
		if (i >= unitDataArraySize) {
			i -= unitDataArraySize;
		}
		for ( ; n > 0 ; n--) {
			sum_of_unit_sizes += unitDataArray[i].size;
			if (++i == lastUnitDataIndex) {
				return sum_of_unit_sizes;
			}
			if (i == unitDataArraySize) {
				i = 0;
			}
		}
	}
	return sum_of_unit_sizes;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray::sumup()
{
	sumOfUnitSizes = 0;
	if (unitDataArray != 0) {
		for (int i = firstUnitDataIndex ; 1 ; ) {
			sumOfUnitSizes += unitDataArray[i].size;
			if (++i == lastUnitDataIndex) {
				return;
			}
			if (i == unitDataArraySize) {
				i = 0;
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray
							  ::set(int index, int size, void *data)
{
	UnitData& unit_data = get(index);
	sumOfUnitSizes += size - unit_data.size;
	unit_data.size = size;
	unit_data.data = data;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray::setSize(int index, int size)
{
	UnitData& unit_data = get(index);
	sumOfUnitSizes += size - unit_data.size;
	unit_data.size = size;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray::setData(int index, void *data)
{
	UnitData& unit_data = get(index);
	unit_data.data = data;
}

// --------------------------------------------------------------------------
//
void VScrollWindow::ScrollData::UnitDataArray
							  ::incrementSize(int index, int increment)
{
	UnitData& unit_data = get(index);
	unit_data.size += increment;
	sumOfUnitSizes += increment;
}
