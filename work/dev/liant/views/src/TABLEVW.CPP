// $Header:   Q:/views/common/vcs/tablevw.cpv   1.18   Jun 03 1997 14:15:50   scottt  $ 

//	tablevw.cpp
//
//	VTableView class implementation [Common]
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
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	09/22/93 jmd	preened
//	10/03/93 jmd	changed to scrolling by cells
//	10/04/93 jmd	added uponEvent
//	10/05/93 jmd	added cellSizing flag
//	11/16/93 jmd	added dummyModel, added realize
//	02/09/94 jmd	now uses longs for row,coll addresses
//	02/10/94 jmd	no longer deletes the table model
//	07/13/94 dss	fixes for scrolling problems
//	05/17/95 pkt	merged and preened.
//	07/07/95 pkt	reverted to version without drag & drop.
//	07/11/95 dgm	merged in drag-and-drop functionality.
//	09/01/95 dss	VStyle changes.
//  11/16/95 dgm    Added upon[Column,Row]HeaderClick()
//					and virtual [row,column]HeaderClicked().
//  04/10/96 dgm    Added deselectAllCells() and fixed getCellBox().
//  05/07/96 dgm    Added rowRclicked(),columnRclicked(),cellRclicked().
//					uponCellRclicked(),uponRowRclicked(),uponColumnRclicked().
//  05/13/96 dgm    Updated.
//  05/30/96 dgm    Update current row/column in resetSize().
//	06/14/96 glm	initilalize rowSizing, columnSizing in init()
//
//	02/04/97 pkt	Implemented 'getClassDefaultStyle'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

#include "tablevw.h"
#include "tablesub.h"
#include "notifier.h"
#include "scrollbr.h"
#include "port.h"
#include "shade.h"
#include "cvassert.h"

defineClass(VTableView, VView)

// ---------------------------------------------------------------------------
VTableView::VTableView()
{
	m_dragen = FALSE;
 	init(&dummyModel, 0, 0);
}

// ---------------------------------------------------------------------------
VTableView::VTableView(VTableModel *tm, const VFrame& f, VWindow *pwin,
					   int rw,
					   int ch,
					   const VStyle& style,
					   boolean dragen)
#if defined(CVS_USE_TRANSIENT) && defined(CVS_LATER_DEFAULT)
	// StyleVertical and StyleHorizontal handled as defaults.
	: VView(f, pwin, style)
#else
	: VView(f, pwin, style + StyleVertical + StyleHorizontal)
#endif
{
	m_dragen = dragen;
	init(tm, rw, ch);
}

// ---------------------------------------------------------------------------
VTableView::~VTableView()
{
	if (tblModel != &dummyModel) {
		delete tblModel;
	}
	tblModel = 0;
}

// ---------------------------------------------------------------------------
void VTableView::init(VTableModel *tm, int rw, int ch)
{
	tblModel = tm;
	updatingScrollBar = 0;

	client = 0;
	enterMthd = NIL_METHOD;
	exitMthd = NIL_METHOD;
	dblclickMthd = NIL_METHOD;
	columnHeaderClickedMethod = 0;
	rowHeaderClickedMethod = 0;
    cellRclickedMthd = 0;
    rowRclickedMthd = 0;
    columnRclickedMthd = 0;

	currRow = -1L;			// current cell
	currCol = -1L;
	xOffset = 0L;			// offset of cells from window
	yOffset = 0L;
	rowOffset = 0L;			// top-left cell
	colOffset = 0L;
	noOutline = FALSE;		// suppress outline draw (used during scrolling)

	cellSizing = TRUE;
	columnSizing = TRUE;
	rowSizing = TRUE;
	hiStyle = Outline;
	hiMode = CellWise;
	indentLabel = TRUE;
	bgStyle = Grid;

	portptr = 0;			// pointer to sub view port

	lastCellY = 0L;			// "hint" data
	lastCellX = 0L;
	lastCellRow = 0L;
	lastCellCol = 0L;
	visRows	= -1L;
	visCols	= -1L;

	if (hwnd() == 0) {
		rowViewWidth = 0;
		colViewHeight = 0;
		columnView = 0;
		rowView = 0;
		cellView = 0;
	}
	else {
		// create sub views
		int w, h;
		sizeOfImage(&w, &h);
		rowViewWidth = rw;
		colViewHeight = ch;

		if (ch > 0) {
			columnView = new VTableSubView(tm, VFrame(rw, 0, w - rw, ch),
								 this, VTableSubView::ColumnView);
		}
		else {
			columnView = 0;
		}

		if (rw > 0) {
			rowView = new VTableSubView(tm, VFrame(0, ch, rw, h - ch),
									 this, VTableSubView::RowView);
		}
		else {
			rowView = 0;
		}

		cellView = new VTableSubView(tm, VFrame(rw, ch, w - rw, h - ch),
									 this, VTableSubView::CellView, m_dragen);
		// set up scroll bars
		if (vScroll) {
			vScroll->uponChange(this, methodOf(VTableView, vtScrollBar));
		}

		if (hScroll) {
			hScroll->uponChange(this, methodOf(VTableView, hzScrollBar));
		}

		updateScrollBars();
	}
}

// ---------------------------------------------------------------------------
void VTableView::uponEvent(	VObject *clnt, method _cellenter,
							method _cellexit, method _celldblclick)
{
	client = clnt;

	if (client != 0) {
		enterMthd = _cellenter;
		exitMthd = _cellexit;
		dblclickMthd = _celldblclick;
	}
}

// ---------------------------------------------------------------------------
//
void VTableView::uponColumnHeaderClick(VObject *o, method m)
{
	client = o;
	columnHeaderClickedMethod = (boolean (VObject::*)(int))m;
}

// ---------------------------------------------------------------------------
//
void VTableView::uponRowHeaderClick(VObject *o, method m)
{
	client = o;
	rowHeaderClickedMethod = (boolean (VObject::*)(int))m;
}

// ---------------------------------------------------------------------------
//
boolean VTableView::columnHeaderClicked(int column)
{
	if ((client != 0) && (columnHeaderClickedMethod != 0)) {
		return (client->*columnHeaderClickedMethod)(column);
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
//
boolean VTableView::rowHeaderClicked(int row)
{
	if ((client != 0) && (rowHeaderClickedMethod != 0)) {
		return (client->*rowHeaderClickedMethod)(row);
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
// Reset the position hints.
// This should be called whenever cell widths or heights change
// or whenever label sizes change
//
void VTableView::resetSize()
{
	lastCellY = 0L;			// "hint" data
	lastCellX = 0L;
	lastCellRow = 0L;
	lastCellCol = 0L;
	visRows = -1L;
	visCols = -1L;

	long new_row_count = getRowCount();
	long new_col_count = getColCount();
	if (currRow >= new_row_count) {
		currRow = new_row_count - 1;
	}
	if (currCol >= new_col_count) {
		currCol = new_col_count - 1;
	}

	updateScrollBars();
}

// ---------------------------------------------------------------------------
boolean VTableView::resized(int w, int h)
{
	long dr = 0, dc = 0;	 // deltas to scroll, in (row, column)

	visRows = -1L;
	visCols = -1L;

	// reposition subviews
	if (rowView != 0) {
		rowView->move(0, colViewHeight, rowViewWidth, h - colViewHeight);
	}

	if (columnView != 0) {
		columnView->move(rowViewWidth, 0, w - rowViewWidth, colViewHeight);
	}

	if (cellView != 0) {
		int oldW, oldH;
		int diffW, diffH;

		cellView->sizeOfImage(&oldW, &oldH);

		// new - old
		diffW = (w - rowViewWidth) - oldW;
		diffH = (h - colViewHeight) - oldH;

		// move the view, first, so that the get{Col,Row}Xxx() functions use 
		// the new size.
		cellView->move(rowViewWidth, colViewHeight, w - rowViewWidth, h - colViewHeight);

		// if the distance between the left most visible column and
		// the right most column of the model is less than the
		// number of visible columns (of the view), then the view
		// extends past the the model. If the colOffset is not zero
		// (meaning the view is scrolled), then scroll the view so that
		// the right most column of the model aligns with the right side
		// of the view.
		if ((colOffset > 0) &&
			((getColCount() - colOffset) <= getVisibleCols())) {
			long	ColCnt = colOffset;
			long	x = getCellX(ColCnt);
			long	stopX = x > diffW ? x - diffW : 0;

			while (x > stopX && ColCnt > 0) {
				x = getCellX(--ColCnt);
				dc--;
			}
		}

		// if the distance between the top most visible row and
		// the bottom most row of the model is less than the
		// number of visible rows (of the view), then the view
		// extends past the the model. If the rowOffset is not zero
		// (meaning the view is scrolled), then scroll the view so that
		// the bottom most column of the model aligns with the bottom
		// of the view.
		if ((rowOffset > 0) &&
			((getRowCount() - rowOffset) <= getVisibleRows())) {
			long	RowCnt = rowOffset;
			long	y = getCellY(RowCnt);
			long	stopY = y > diffH ? y - diffH : 0;

			while (y > stopY && RowCnt > 0) {
				y = getCellY(--RowCnt);
				dr--;
			}
		}
	}

	if (dr != 0 || dc != 0) {
		scrollTable(dr, dc);
	}
	
	if (updatingScrollBar == 0) {
		updateScrollBars();
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VTableView::erased()
{
	return TRUE;
}

// ---------------------------------------------------------------------------
// Draw the upper corner nub
//
boolean VTableView::paint()
{
	if (rowView != 0 && columnView != 0) {

		int w, h;
		sizeOfImage(&w, &h);
		if (rowViewWidth > 0 && colViewHeight > 0) {
			VRectangle nub(CornerDim, 0, 0, rowViewWidth, colViewHeight);
			VShade	shd;
			shd.setBevelThickness(1);
			VPort port(this);
			port.open();
			shd.panel(Raised, nub, 1, port);
			port.close();
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VTableView::gotoCell(long cr, long cc)
{
	if (cr < 0L || cc < 0L || cr >= getRowCount() || cc >= getColCount()) {
		return FALSE;
	}

	long oldcr = currRow;
	long oldcc = currCol;

	if (oldcr != cr || oldcc != cc) {
		if (!cellExit(currRow, currCol)) {
			return FALSE;
		}

		currRow = cr;
		currCol = cc;

		// erase old cell outline
		drawCellHighlight(oldcr, oldcc, (cr != oldcr), (cc != oldcc));
		scrollAdjust(cr, cc);

		// draw new cell outline
		drawCellHighlight(cr, cc, (cr != oldcr), (cc != oldcc));

		cellEnter(currRow, currCol);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VTableView::dblClickCell(long cr, long cc)
{
	if (cr < 0 || cc < 0 || cr >= getRowCount() || cc >= getColCount()) {
		return FALSE;
	}

	if (client != 0 && dblclickMthd != NIL_METHOD) {
		return client->perform(dblclickMthd, (long)this, cr, cc);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Draw/Erase outline around specified cell (using Xor paint)
// Use the current Highlight style and mode
//
void VTableView::drawCellHighlight(long cr, long cc, boolean rchange, boolean cchange)
{
	if (cc < 0L || cr < 0L) {
		return;
	}

//	if (hiStyle != NoHighlight) {
		cellView->drawCellHighlight(cr, cc);
//	}

	if (indentLabel && columnView && cchange) {
		columnView->drawCellHighlight(cr, cc);
	}
	if (indentLabel && rowView && rchange) {
		rowView->drawCellHighlight(cr, cc);
	}
}

// ---------------------------------------------------------------------------
// Repaint a cell (for user use).
//
void VTableView::updateCell(long cr, long cc)
{
	cellView->updateCell(cr, cc);
}

// ---------------------------------------------------------------------------
// Calculate the X location for the given cell column
// (relative to the entire table, not the window (i.e., ignore scrolling))
//
long VTableView::getCellX(long cc)
{
	long x = lastCellX;

	if (lastCellCol < cc) {
		while (lastCellCol < cc) {
			x += getColWidth(lastCellCol++);
		}
	}
	else if (lastCellCol > cc) {
		while (lastCellCol > cc) {
			x -= getColWidth(--lastCellCol);
		}
	}

	// reset hints
	lastCellX = x;

	return x;
}

// ---------------------------------------------------------------------------
// Calculate the Y location for the given cell row
// (relative to the entire table, not the window (i.e., ignore scrolling))
//
long VTableView::getCellY(long cr)
{
	long y = lastCellY;

	if (lastCellRow < cr) {
		while (lastCellRow < cr) {
			y += getRowHeight(lastCellRow++);
		}
	}
	else if (lastCellRow > cr) {
		while (lastCellRow > cr) {
			y -= getRowHeight(--lastCellRow);
		}
	}

	// reset hints
	lastCellY = y;

	return y;
}

// ---------------------------------------------------------------------------
// Return the number of rows currently (entirely) visible
//
long VTableView::getVisibleRows()
{
	if (visRows < 0L) {
		int wh = (cellView != 0) ? cellView->getHeight() : 0;
		long count = getRowCount() - rowOffset;

		// count rows
		long cr;
		for (cr = rowOffset, visRows = 0L;
			 getCellY(cr + 1) < yOffset + wh && visRows < count;
			 cr++, visRows++) {
			;
		}
	}

	return visRows;
}

// ---------------------------------------------------------------------------
// Return the number of columns currently (entirely) visible
//
long VTableView::getVisibleCols()
{
	if (visCols < 0L) {
		int ww = (cellView != 0) ? cellView->getWidth() : 0;
		long count = getColCount() - colOffset;

		// count columns
		long cc;
		for (cc = colOffset, visCols = 0L;
			 getCellX(cc + 1) < xOffset + ww && visCols < count;
			 cc++, visCols++) {
			;
		}
	}

	return visCols;
}

// ---------------------------------------------------------------------------
// Scroll the table the specified amount (in rows)
//
void VTableView::scrollTable(long drows, long dcols)
{
	if (cellView == 0) {
		return;
	}

	// adjust offsets
	drows = (-drows > rowOffset) ? -rowOffset : drows;
	rowOffset += drows;

	dcols = (-dcols > colOffset) ? -colOffset : dcols;
	colOffset += dcols;

	long	yo, dy = 0L;
	long	xo, dx = 0L;

	if (drows != 0) {
		yo = getCellY(rowOffset);
		dy = yo - yOffset;
		yOffset = yo;
	}

	if (dcols != 0) {
		xo = getCellX(colOffset);
		dx = xo - xOffset;
		xOffset = xo;
	}

	noOutline = TRUE;

	if ( dy > (long) cellView->getHeight() ||
	    -dy > (long) cellView->getHeight() ||
		 dx > (long) cellView->getWidth() ||
		-dx > (long) cellView->getWidth()) {

		cellView->update();
 		if (columnView && dx != 0L) {
			columnView->update();
		}
		if (rowView && dy != 0L) {
			rowView->update();
		}
	}
	else {
		cellView->scroll((int) -dx, (int) -dy);
 		if (columnView && dx != 0L) {
			columnView->scroll((int) -dx, 0);
		}
		if (rowView && dy != 0L) {
			rowView->scroll(0, (int) -dy);
		}
	}

	noOutline = FALSE;

	// since rowOffset and/or colOffset changed, reset these flags so
	// getVisible{Row,Col}() will use new values.
	visRows = -1L;
	visCols = -1L;

	updateScrollBars();
}

// ---------------------------------------------------------------------------
// Update the thumb positions and ranges of the scroll bars
//
void VTableView::updateScrollBars()
{
	long	max, line, page;

	updatingScrollBar++;

	// update scroll bars
	if (vScroll != 0) {
		long hy = getRowCount();
		line = 1L;
		page = getVisibleRows();
		max = hy - page;

		// if all the columns in the model are visible, we don't need the
		// vertical scroll bar
		if (max == 0L) {
			cv_assert(rowOffset == 0L);
			if (!vScroll->isHidden()) {
				vScroll->hide();
				yOffset = 0L;
			}
		}
		else {
			cv_assert(max > 0L);
			vScroll->setValues(max, 0L, line, page);
			vScroll->setPos(rowOffset);
			if (vScroll->isHidden()) {
				vScroll->show();
			}
		}
	}

	if (hScroll != 0) {
		long wx = getColCount();
		line = 1L;
		page = getVisibleCols();
		max = wx - page;

		// if all the rows in the model are visible, we don't need the
		// horizontal scroll bar
		if (max == 0L) {
			cv_assert(colOffset == 0L);
			if (!hScroll->isHidden()) {
				hScroll->hide();
				xOffset = 0L;
			}
		}
		else {
			cv_assert(max > 0L);
			hScroll->setValues(max, 0L, line, page);
			hScroll->setPos(colOffset);
			if (hScroll->isHidden()) {
				hScroll->show();
			}
		}
	}

	updatingScrollBar--;
}

// ---------------------------------------------------------------------------
// The vertical scrollbar has changed position
//
boolean VTableView::vtScrollBar(long i)
{
	if (vScroll) {
		long vx = vScroll->getMax();
		if (vx <= 0L) {
			i = 0L;
		}
		// erase cell outline, scroll, redraw cell outline
		drawCellHighlight(currRow, currCol, FALSE, FALSE);
		scrollTable(i - rowOffset, 0L);
		drawCellHighlight(currRow, currCol, FALSE, FALSE);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// The horizontal scrollbar has changed position
//
boolean VTableView::hzScrollBar(long i)
{
	if (hScroll) {
		long hx = hScroll->getMax();
		if (hx <= 0L) {
			i = 0L;
		}
		// erase cell outline, scroll, redraw cell outline
		drawCellHighlight(currRow, currCol, FALSE, FALSE);
		scrollTable(0L, i - colOffset);
		drawCellHighlight(currRow, currCol, FALSE, FALSE);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Scroll the table so that cell (cr, cc) is visible,
// return TRUE if table repainted.
//
boolean VTableView::scrollAdjust(long cr, long cc)
{
	if (cellView == 0) {
		return FALSE;
	}

	long	row_adj = 0;
	long	col_adj = 0;

	int		winw = cellView->getWidth();
	int		winh = cellView->getHeight();

	// compute row adjustment
	if (cr < rowOffset) {
		row_adj = cr - rowOffset;
	}
	else if (cr >= rowOffset + getVisibleRows()) {
		int h = winh - getRowHeight(cr);
		for (; cr > 0L; cr--) {
			h -= getRowHeight(cr - 1);
			if (h < 0) {
				break;
			}
		}
		row_adj = cr - rowOffset;
	}

	// compute column adjustment
	if (cc < colOffset) {
		col_adj = cc - colOffset;
	}
	else if (cc >= colOffset + getVisibleCols()) {
		int w = winw - getColWidth(cc);

		for (; cc > 0L; cc--) {
			w -= getColWidth(cc - 1);
			if (w < 0) {
				break;
			}
		}
		col_adj = cc - colOffset;
	}

	// do it
	if (row_adj != 0L || col_adj != 0L) {
		scrollTable(row_adj, col_adj);
		return TRUE;
	}

	return FALSE;
}


// ---------------------------------------------------------------------------
// Fill CellBox with the coords for the specified cell,
// return a pointer to it.
//
VRectangle *VTableView::getCellBox(long cr, long cc)
{
	if (cr < 0L || cc < 0L || cr >= getRowCount() || cc >= getColCount()) {
		cellBox.set(CornerDim, 0, 0, 0, 0);
	}
	else {
		cellBox.set(CornerDim,
					int(getCellX(cc) - xOffset),
				    int(getCellY(cr) - yOffset),
					getColWidth(cc), getRowHeight(cr));
	}
	return &cellBox;
}

// ---------------------------------------------------------------------------
// Key pressed within the VTableView
//
boolean VTableView::key(int k, char /* ch */)
{
	switch (k) {
	case K_Up:
		gotoCell(getCurrRow() - 1L, getCurrCol());
		break;

	case K_Down:
		gotoCell(getCurrRow() + 1L, getCurrCol());
		break;

	case K_Prior:
		gotoCell(getCurrRow() - getVisibleRows(), getCurrCol());
		break;

 	case K_Next:
		gotoCell(getCurrRow() + getVisibleRows(), getCurrCol());
		break;

	case K_Right:
		gotoCell(getCurrRow(), getCurrCol() + 1L);
		break;

	case K_Left:
		gotoCell(getCurrRow(), getCurrCol() - 1L);
		break;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Cursor just entered this cell (return value is ignored)
//
boolean VTableView::cellEnter(long cr, long cc)
{
	if (client != 0 && enterMthd != NIL_METHOD) {
		return client->perform(enterMthd, (long) this, cr, cc);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Cursor about to leave a cell (return FALSE to prevent)
//
boolean VTableView::cellExit(long cr, long cc)
{
	if (client != 0 && exitMthd != NIL_METHOD) {
		return client->perform(exitMthd, (long)this, cr, cc);
	}

	return TRUE;
}


// ---------------------------------------------------------------------------
void VTableView::setTableModel(VTableModel *tm, int rw, int ch)
{
	if (tblModel != &dummyModel) {
		delete tblModel;
	}

	if (cellView != 0) {
		delete cellView;
	}
	if (rowView != 0) {
		delete rowView;
	}
	if (columnView != 0) {
		delete columnView;
	}

	init(tm, rw, ch);
}

// ---------------------------------------------------------------------------
//
void VTableView::deselectAllCells()
{
	long cr = getCurrRow();
	long cc = getCurrCol();
	currRow = currCol = -1L;
	switch (getHighlightMode()) {
		case CellWise:
		{
			updateCell(cr, cc);
			break;
		}
		case RowWise:
		{
			VRectangle *r = getCellBox(cr, cc);
			int x, y, w, h;
			r->get(CornerDim, &x, &y, &w, &h);
			r->set(CornerDim, 0, y, cellView->getWidth(), h);
			cellView->update(r, TRUE);
			break;
		}
		case ColumnWise:
		{
			VRectangle *r = getCellBox(cr, cc);
			int x, y, w, h;
			r->get(CornerDim, &x, &y, &w, &h);
			r->set(CornerDim, x, 0, w, cellView->getHeight());
			cellView->update(r, TRUE);
			break;
		}
	}

}

// ---------------------------------------------------------------------------
//
void VTableView::cellRclicked(long cr, long cc, int mx, int my)
{
    if ((client != 0) && (cellRclickedMthd != 0)) {
		(client->*cellRclickedMthd)(cr, cc, mx, my);
	}
}

// ---------------------------------------------------------------------------
//
void VTableView::rowRclicked(long cr, int mx, int my)
{
    if ((client != 0) && (rowRclickedMthd != 0)) {
		(client->*rowRclickedMthd)(cr, mx, my);
	}
}

// ---------------------------------------------------------------------------
//
void VTableView::columnRclicked(long cc, int mx, int my)
{
    if ((client != 0) && (columnRclickedMthd != 0)) {
		(client->*columnRclickedMthd)(cc, mx, my);
	}
}

// ---------------------------------------------------------------------------
//
void VTableView::uponCellRclicked(VObject *o, method m)
{
	client = o;
    cellRclickedMthd = (void (VObject::*)(long, long, int, int))m;
}

// ---------------------------------------------------------------------------
//
void VTableView::uponRowRclicked(VObject *o, method m)
{
	client = o;
    rowRclickedMthd = (void (VObject::*)(long, int, int))m;
}

// ---------------------------------------------------------------------------
//
void VTableView::uponColumnRclicked(VObject *o, method m)
{
	client = o;
    columnRclickedMthd = (void (VObject::*)(long, int, int))m;
}

#if 0

// ---------------------------------------------------------------------------
// 
boolean VTableView::givenFocus() 
{
	if (parent) {
		parent->setChildFocus(this);
	}
	return FALSE; 
}

// ---------------------------------------------------------------------------
// 
void  VTableView::setChildFocus(VWindow *win)
{
	VView::setChildFocus(win);
	if (parent) {
		parent->setChildFocus(this);
	}
}

#endif

// --------------------------------------------------------------------------
//
VStyle& VTableView::getClassDefaultStyle(void)
{
	static VWinStyle CV_NEARDATA default_style
								 (VView::getClassDefaultStyle() +
								  setTransient +
								  StyleHorizontal +
								  StyleVertical);
	return default_style;
}

// --------------------------------------------------------------------------
//
VStyle& VTableView::getDefaultStyle(void) const
{
	return getClassDefaultStyle();
}

//---------------------------------------------------------------------------

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
void VTableView::realize(VWindow *parent_window)
{
#if defined(CVS_USE_TRANSIENT) && defined(CVS_LATER_DEFAULT)
	// StyleVertical and StyleHorizontal handled as defaults.
#else
	style = StyleVertical + StyleHorizontal;
#endif

	VView::realize(parent_window);

	updateScrollBars();
}

#endif // ARCHIVER
