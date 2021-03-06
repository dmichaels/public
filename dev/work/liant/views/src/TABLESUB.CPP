// $Header:   Q:/views/common/vcs/tablesub.cpv   1.15   May 30 1997 17:16:48   scottt  $ 

//	tablesub.cpp
//
//	VTableSubView class implementation [Common]
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
//	10/05/93 jmd	added cellSizing flag
//	11/12/93 pmcm	integer casts on lastx, lasty in paint() to quell warnings.
//	01/31/94 jmd	added no highlight support
//	01/31/94 jmd	now uses tablevw brush for background clear, added VColor::
//	02/01/94 jmd	replaced nasty ifdefs
//	02/09/94 jmd	now uses longs for row,coll addresses
//	06/28/94 jmd	now uses getExposedRectangle
//	05/17/95 pkt	preened and merged.
//	07/07/95 pkt	reverted to version without drag & drop.
//	07/11/95 dgm	merged in drag-and-drop functionality.
//	09/20/95 dss	replaced constant zero with StyleDefault VView ctor.
//  11/16/95 dgm    Added support for VTableView::[row,column]HeaderClicked().
//  05/07/96 dgm    Added rmouseDn().
//  05/13/96 dgm    Updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "tablesub.h"

#include "port.h"
#include "brush.h"
#include "font.h"
#include "notifier.h"
#include "shade.h"
#include "pen.h"

defineClass(VTableSubView, VView)

// ---------------------------------------------------------------------------
//
#if !defined(CV_NO_DRAG_AND_DROP)
static long mouseCol = -1L;
static long mouseRow = -1L;
#endif

// ---------------------------------------------------------------------------
//
VTableSubView::VTableSubView()
{
#if !defined(CV_NO_DRAG_AND_DROP)
	m_dragen = 0;
#endif
 	init(0, 0, CellView);		//error
}

// ---------------------------------------------------------------------------
//
VTableSubView::VTableSubView(VTableModel *tm, const VFrame& f, 
							 VTableView *pwin,
							 VTableSubType tp,
							 boolean dragen)
	: VView(f, (VWindow *)pwin, StyleDefault)
{
#if !defined(CV_NO_DRAG_AND_DROP)
	m_dragen = dragen;
#endif
	init(tm, pwin, tp);
}

// ---------------------------------------------------------------------------
//
VTableSubView::~VTableSubView()
{
	delete thePort;

#if !defined(CV_NO_DRAG_AND_DROP)
	if (m_dragen && type == CellView) {
		g_DragManager->removeHandler(this, this);
	}
#endif
}

// ---------------------------------------------------------------------------
//
void VTableSubView::init(VTableModel *tm, VTableView *pwin, VTableSubType tp)
{
	table = pwin;
	tblModel = tm;
	type = tp;

	mouseMode = Normal;
	resizeNo = 0;			// row or column being resized
	resizeVal = 0;			// starting mouse location for resize
	resizeBar = 0;			// location of resize bar

	sizeOfImage(&winWidth, &winHeight);

	maxX = winWidth;
	maxY = winHeight;

	thePort = new VPort(this);

#if !defined(CV_NO_DRAG_AND_DROP)
	if (m_dragen && type == CellView) {
		g_DragManager->addHandler(this, this);
		trackContent(TRUE);
	}
	dropRow = -1;
	dropCol = -1;
#endif
}

// ---------------------------------------------------------------------------
// Prevent background erasure, we'll do it ourselves
//
boolean VTableSubView::erased()
{
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::resized(int w, int h)
{
	winWidth = w;
	winHeight = h;

	maxX = winWidth;
	maxY = winHeight;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Draw/Erase outline around specified cell (using Xor paint)
// Use the current Highlight style and mode
//
void VTableSubView::drawCellHighlight(long cr, long cc)
{
	int		w, h;
	long	x, y;

	if (table->indentLabel && type == RowView && cr >= 0) {
		VShade	shd;
		shd.setBevelThickness(1);

		thePort->open();
		table->portptr = thePort;
		h = getRowHeight(cr);
		y = getCellY(cr);
		table->getCellBox()->set(CornerDim, 0, (int) (y - 1L - getYOffset()), winWidth, h + 1);
		shd.panel((getCurrRow() == cr) ? Pressed : Raised, *(table->getCellBox()), 1, *thePort);
		tblModel->paintRowLabel(table, cr);
		thePort->close();
		return;
	}

	if (table->indentLabel && type == ColumnView && cc >= 0) {
		VShade	shd;
		shd.setBevelThickness(1);

		thePort->open();
		table->portptr = thePort;
		w = getColWidth(cc);
		x = getCellX(cc);
		table->getCellBox()->set(CornerDim, (int)(x - 1L - getXOffset()), 0, w + 1, winHeight);
		shd.panel((getCurrCol() == cc) ? Pressed : Raised, *(table->getCellBox()), 1, *thePort);
		tblModel->paintColLabel(table, cc);
		thePort->close();
		return;
	}

	if (cc < 0L || cr < 0L || type != CellView) {
		return;
	}

	x = getCellX(cc);
	y = getCellY(cr);
	h = getRowHeight(cr);
	w = getColWidth(cc);

	VRectangle outBox;

	switch (table->hiMode) {
	case VTableView::CellWise:
		outBox.set(CornerDim, (int)(x - getXOffset()),
							  (int)(y - getYOffset()), w, h);
		break;

	case VTableView::RowWise:
		outBox.set(CornerDim, 0, (int) (y - getYOffset()), maxX, h);
		break;

	case VTableView::ColumnWise:
		outBox.set(CornerDim, (int) (x - getXOffset()), 0, w, maxY);
		break;
	}

	// jmd 1/31/94
	if (table->getHighlightStyle() != VTableView::NoHighlight) {

		VPen pn(VColor::Black);
		pn.width(3);

		thePort->open();
		thePort->usePen(&pn);
		thePort->rule(NotXorRule);
		if (table->hiStyle == VTableView::Outline) {
			thePort->frameRegion(&outBox);
		}
		else {
			thePort->invertRegion(&outBox);
		}
		thePort->rule(CopyRule);
		thePort->usePen(0);
		thePort->close();
	}
}

// ---------------------------------------------------------------------------
// Repaint a cell.
//
void VTableSubView::updateCell(long cr, long cc)
{
	if (type == CellView) {
		long x = getCellX(cc) - getXOffset();
		long y = getCellY(cr) - getYOffset();
		int w = getColWidth(cc);
		int h = getRowHeight(cr);
		VRectangle cell(CornerDim, (int) (x - 1), (int) (y - 1), w + 1, h + 1);
		update(&cell);
	}
}

// ---------------------------------------------------------------------------
// Paint the table
//
boolean VTableSubView::paint()
{
	VRectangle	cell;
	VRectangle	box;
	VShade		shd;

	shd.setBevelThickness(1);

	int			exmin, exmax, eymin, eymax;
	VRectangle  exprect;
	notifier->getExposedRectangle(exprect);

	exprect.get(Corners, &exmin, &eymin, &exmax, &eymax);

	long	x, y;
	long	cr, cc, cr_start, cc_start;
	int		w, h;
	long	rows = getRowCount();
	long	cols = getColCount();

	thePort->open();
	table->portptr = thePort;

	// translate window coords to table coords
	long xmin = exmin + getXOffset();
	long ymin = eymin + getYOffset();
	long xmax = exmax + getXOffset();
	long ymax = eymax + getYOffset();

	if (type == RowView) {
		// find top edge
		for (cr_start = getRowOffset();
			 getCellY(cr_start + 1L) < ymin;
			 cr_start++) {
			;
		}

		// paint row labels
		for (cr = cr_start; (y = getCellY(cr)) <= ymax; cr++) {
			if (cr < rows) {
				h = getRowHeight(cr);
				table->getCellBox()->set(CornerDim, 0, (int) (y - 1L - getYOffset()), winWidth, h + 1);
				shd.panel((table->indentLabel && getCurrRow() == cr) ? Pressed : Raised,
							 *(table->getCellBox()), 1, *thePort);
				table->getCellBox()->inset(1, 1);
				tblModel->paintRowLabel(table, cr);
			}
			else {
				// fill region beneath the labels
				h = (int) (ymax - y);
				box.set(CornerDim, 0, (int) (y - 1L - getYOffset()), winWidth, h + 1);
				shd.panel(Raised, box, 1, *thePort);
				break;
			}
		}
	}

	// column labels
	if (type == ColumnView) {
		// find left edge
		for (cc_start = getColOffset();
			 getCellX(cc_start + 1L) < xmin;
			 cc_start++) {
			;
		}

		for (cc = cc_start; (x = getCellX(cc)) <= xmax; cc++) {
			if (cc < cols) {
				w = getColWidth(cc);
				table->getCellBox()->set(CornerDim, (int)(x - 1L - getXOffset()), 0, w + 1, winHeight);
				shd.panel((table->indentLabel && getCurrCol() == cc) ? Pressed : Raised,
						 *(table->getCellBox()), 1, *thePort);
				table->getCellBox()->inset(1, 1);
				tblModel->paintColLabel(table, cc);
			}
			else {
				// fill region beside the labels
				w = (int) (xmax - x);
				box.set(CornerDim, (int) (x - 1L - getXOffset()), 0, w + 1, winHeight);
				shd.panel(Raised, box, 1, *thePort);
				break;
			}
		}
	}

	if (type == CellView) {
		// find top left corner
		for (cr_start = getRowOffset();
			 getCellY(cr_start + 1L) < ymin;
			 cr_start++) {
			;
		}

		for (cc_start = getColOffset();
			 getCellX(cc_start + 1) < xmin;
			 cc_start++) {
			;
		}

		// paint cells
		if (table->bgStyle != VTableView::Panels) {
			// erase background
#if 1
			// pkt - taken from Motif during 5/17/95 merge
			thePort->useBrush(getBackground());
			// thePort->fillRegion(&exprect, FALSE);
			thePort->eraseRegion(&exprect);
#else
			thePort->useBrush(table->getBackground());
			thePort->fillRegion(&exprect, FALSE);
#endif
			thePort->useBrush(0);
		}

		// draw background
		if (table->bgStyle == VTableView::Grid) {
			// draw grid lines
			VPen pn;
			if (depthOfImage() > 1) {
				pn.color(VColor::LightGray);
			}
			pn.width(1);
			thePort->usePen(&pn);

			// find last visible row and column
			for (cr = cr_start; ((y = getCellY(cr)) <= ymax) && cr < rows; cr++) {
				;
			}
			long lasty = y;
			maxY = (cr < rows) ? winHeight : (int) lasty;

			for (cc = cc_start; ((x = getCellX(cc)) <= xmax) && cc < cols; cc++) {
				;
			}
			long lastx = x;
			maxX = (cc < cols) ? winWidth : (int) lastx;
			
			// horizontal lines
			for (cr = cr_start; ((y = getCellY(cr + 1)) <= ymax) && cr < rows; cr++) {
				thePort->moveTo(exmin, (int)(y - 1L - getYOffset()));
				thePort->lineTo((int)(lastx - getXOffset()), (int)(y - 1L - getYOffset()));
			}

			// vertical lines
			for (cc = cc_start; ((x = getCellX(cc + 1)) <= xmax) && cc < cols; cc++) {
				thePort->moveTo((int)(x - 1L - getXOffset()), eymin);
				thePort->lineTo((int)(x - 1L - getXOffset()), (int) (lasty - getYOffset()));
			}
			thePort->usePen(0);
		}
		else if (table->bgStyle == VTableView::Panels) {
			// draw panels
			VRectangle r;
			for (cc = cc_start; ((x = getCellX(cc)) <= xmax); cc++) {
				if (cc < cols) {
					w = getColWidth(cc);
					r.set(CornerDim, (int) (x - getXOffset()), -1, w, winHeight + 2);
					shd.panel(Raised, r, 0, *thePort);
				}
				else {
					// fill region to the right
					w = (int) (xmax - x);
					r.set(CornerDim, (int) (x - getXOffset()), -1, w, winHeight + 2);
					shd.panel(Raised, r, 0, *thePort);
					break;
				}
			}

			maxX = winWidth;
			maxY = winHeight;
		}

		// draw cells
		for (cr = cr_start; ((y = getCellY(cr)) <= ymax) && cr < rows; cr++) {
			h = getRowHeight(cr);
			for (cc = cc_start; ((x = getCellX(cc)) <= xmax) && cc < cols; cc++) {
				// draw the cell
				w = getColWidth(cc);
				table->getCellBox()->set(CornerDim,
									   	(int) (x - 1L - getXOffset()),
									   	(int) (y - 1L - getYOffset()),
										w + 1, h + 1);
				tblModel->paintCell(table, cr, cc);
			}
		}
	}

	thePort->close();

	if (type == CellView && !getNoOutline()) {
		drawCellHighlight(getCurrRow(), getCurrCol());
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::mouseEnter(int /* bStat */)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (type != CellView) {
#if defined(MOTIF)
		if ((mouseMode != RowResize) && (mouseMode != ColumnResize)) {
		}
		else {
			return TRUE;
		}
#endif
		notifier->mouseTracking(TRUE);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::mouseExit(int /* bStat */)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (type != CellView) {
#if defined(MOTIF)
		if ((mouseMode != RowResize) && (mouseMode != ColumnResize)) {
		}
		else {
			return TRUE;
		}
#endif
		notifier->mouseTracking(FALSE);

		if (mouseMode != Normal) {
			notifier->restoreCursor();
			mouseMode = Normal;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::mouseMv(int mx, int my, int bStat)
{
	if (!isEnabled()) {
		return FALSE;
	}

	long	cr, cc;
	HitType hit;

#if !defined(CV_NO_DRAG_AND_DROP) && !defined(CV_NOARCHIVER)
	if (m_dragen && bStat && type == CellView && mouseMode == Drag) {
		if (!mouseRect.pointIn(mx, my)) {
			mouseMode = Normal;
			notifier->releaseCapture();
			// use mouseCol; use mouseRow; as cell to drag
			VObject* pObject = tblModel->getObject(table, mouseRow, mouseCol);
			if (pObject) {
				VDragItem dragItem;
				dragItem.put(pObject, FALSE);
				g_DragManager->trackDrag(this, &dragItem);
			}
			notifier->mouseTracking(FALSE);
		}
		return TRUE;
	}
#endif

	if (!table->cellSizing) {
		// cellSizing disabled
		return TRUE;
	}

	if (!bStat && type != CellView) {
		hit = hitTest(mx, my, &cr, &cc);
		if (mouseMode == Normal && hit == HitRowSep) {
			notifier->setCursor(SizeNS);
			mouseMode = RowReady;
			resizeNo = cr;
			resizeVal = my;
		}
		else if (mouseMode == Normal && hit == HitColumnSep) {
			if (table->isColumnSizing()) {
				notifier->setCursor(SizeWE);
			}
			mouseMode = ColumnReady;
			resizeNo = cc;
			resizeVal = mx;
		}
		else if (mouseMode == RowReady) {
			if ((resizeVal - my) > 2 || (my - resizeVal) > 2) {
				notifier->restoreCursor();
				mouseMode = Normal;
			}
		}
		else if (mouseMode == ColumnReady) {
			if ((resizeVal - mx) > 2 || (mx - resizeVal) > 2) {
				notifier->restoreCursor();
				mouseMode = Normal;
			}
		}
		else if (mouseMode != Normal) {
			notifier->restoreCursor();
			mouseMode = Normal;
		}
	}
	else {
		// button pressed
		if (mouseMode == RowResize) {
			thePort->moveTo(0, resizeBar);
			thePort->lineTo(winWidth, resizeBar);
			thePort->moveTo(0, my);
			thePort->lineTo(winWidth, my);
			resizeBar = my;
		}
		else if (mouseMode == ColumnResize) {
			thePort->moveTo(resizeBar, 0);
			thePort->lineTo(resizeBar, winHeight);
			thePort->moveTo(mx, 0);
			thePort->lineTo(mx, winHeight);
			resizeBar = mx;
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

	long	cr, cc;
	HitType hit;

	if (mouseMode == RowReady) {
		mouseMode = RowResize;
#if defined(MOTIF)
		disableClipping();
#endif

		thePort->open();
		thePort->rule(NotXorRule);
		thePort->moveTo(0, my);
		thePort->lineTo(winWidth, my);
		resizeBar = my;
		
		notifier->captureMouseFor(this);
#if defined(MOTIF)
		notifier->mouseTracking(TRUE);
#endif
	}
	else if (mouseMode == ColumnReady) {
		mouseMode = ColumnResize;
#if defined(MOTIF)
		disableClipping();
#endif

		thePort->open();
		thePort->rule(NotXorRule);
		thePort->moveTo(mx, 0);
		thePort->lineTo(mx, winHeight);
		resizeBar = mx;

		notifier->captureMouseFor(this);
#if defined(MOTIF)
		notifier->mouseTracking(TRUE);
#endif
	}
	else if (mouseMode == Normal) {
		int h;
		hit = hitTest(mx, my, &cr, &cc);
		switch (hit) {
		case HitCell:
#if !defined(CV_NO_DRAG_AND_DROP)
			tblModel->hitCell(table, cr, cc);
			mouseMode = Drag;
			mouseRow   = cr;
			mouseCol   = cc;
			h = int(getRowHeight(cr));
			mouseRect.set(CenterDim, mx, my, h / 2, h / 2);
			notifier->captureMouseFor(this);
			notifier->mouseTracking(TRUE);
#else
			table->gotoCell(cr, cc);
#endif
			break;
		case HitColumn:
			if (!table->columnHeaderClicked(cc)) {
#if !defined(CV_NO_DRAG_AND_DROP)
				tblModel->hitColumn(table, cc);
#else
				table->gotoCell(getCurrRow(), cc);
#endif
			}
			break;
		case HitRow:
			if (!table->rowHeaderClicked(cr)) {
#if !defined(CV_NO_DRAG_AND_DROP)
				tblModel->hitRow(table, cr);
#else
				table->gotoCell(cr, getCurrCol());
#endif
			}
			break;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::mouseDbl(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

	long cr, cc;

	if (mouseMode == Normal) {
		if (hitTest(mx, my, &cr, &cc) == HitCell) {
			table->dblClickCell(cr, cc);
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::mouseUp(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

#if !defined(CV_NO_DRAG_AND_DROP)
	if (m_dragen && mouseMode == Drag) {
		notifier->releaseCapture();
		notifier->mouseTracking(FALSE);
		mouseMode = Normal;
	}
#endif

	if (mouseMode != Normal) {
		notifier->releaseCapture();
		notifier->restoreCursor();

		if (mouseMode == RowResize) {
			thePort->moveTo(0, resizeBar);
			thePort->lineTo(winWidth, resizeBar);
			thePort->rule(CopyRule);
			thePort->close();
#if defined(MOTIF)
			restoreClipping();
#endif

			// resize row
			int dh = my - resizeVal;
			int h = getRowHeight(resizeNo) + dh;
			h = (h < 1) ? 1 : h;
			setRowHeight(resizeNo, h);
			table->update();
		}
		else if (mouseMode == ColumnResize) {
			thePort->moveTo(resizeBar, 0);
			thePort->lineTo(resizeBar, winHeight);
			thePort->rule(CopyRule);
			thePort->close();
#if defined(MOTIF)
			restoreClipping();
#endif

			// resize column
			int dw = mx - resizeVal;
			int w = getColWidth(resizeNo) + dw;
			w = (w < 1) ? 1 : w;
			setColWidth(resizeNo, w);
			table->update();
		}

		mouseMode = Normal;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::rmouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

	long cr, cc;
	int table_mx = mx, table_my = my;
	mapToScreen(&table_mx, &table_my);
	table->mapToWin(&table_mx, &table_my);
	switch (hitTest(mx, my, &cr, &cc)) {
		case HitCell:
			table->cellRclicked(cr, cc, table_mx, table_my);
			return TRUE;
		case HitColumn:
		case HitColumnSep:
			table->columnRclicked(cc, table_mx, table_my);
			return TRUE;
		case HitRow:
		case HitRowSep:
			table->rowRclicked(cr, table_mx, table_my);
			return TRUE;
		case HitNone: default:
			return VView::rmouseDn(mx, my);
	}
}

#if defined(MOTIF)
// ---------------------------------------------------------------------------
//
static boolean parent_was_clipping = FALSE;

void VTableSubView::disableClipping()
{
	parent_was_clipping = parent->clipToChildren();
	if (parent_was_clipping) {
		parent->clipToChildren(FALSE);
	}
}

void VTableSubView::restoreClipping()
{
	if (parent_was_clipping) {
		parent->clipToChildren(TRUE);
	}
}
#endif

// ---------------------------------------------------------------------------
//	See where (mx, my) hits.
//
//	Returns:
//	--------
//	HitNone			nothing hit.
//	HitCell			cell hit, address in (crp, ccp)
//	HitColumn		hit column label, col # in ccp
//	HitColumnSep	hit column label separator, col # in ccp (-1 for label)
//	HitRow			hit row label, col # in ccp
//	HitRowSep		hit row label separator, row # in crp (-1 for label)
//
VTableSubView::HitType VTableSubView::hitTest(int mx, int my, long *crp, long *ccp)
{
	long	cr, cc;
	int		x, y;
	long	rows = getRowCount();
	long	cols = getColCount();

	*crp = -1L;
	*ccp = -1L;

	// scan columns
	HitType colHit = HitNone;
	for (cc = getColOffset(); cc < cols; cc++) {
		x = (int)(getCellX(cc + 1L) - getXOffset());
		if (mx > x - 2 && mx < x + 2 && table->isColumnSizing()) {
			*ccp = cc;
			colHit = HitColumnSep;
			break;
		}
		else if (mx < x) {
			*ccp = cc;
			colHit = HitColumn;
			break;
		}
		else if (x > winWidth) {
			break;
		}
	}

	// scan rows
	HitType rowHit = HitNone;
	for (cr = getRowOffset(); cr < rows; cr++) {
		y = (int)(getCellY(cr + 1L) - getYOffset());
		if (my > y - 2 && my < y + 2 && table->isRowSizing()) {
			*crp = cr;
			rowHit = HitRowSep;
			break;
		}
		else if (my < y) {
			*crp = cr;
			rowHit = HitRow;
			break;
		}
		else if (y > winHeight) {
			break;
		}
	}

	if (rowHit == HitNone && colHit == HitNone) {
		return HitNone;
	}

	if (type == ColumnView) {
		// mouse over column label
		return colHit;
	}

	if (type == RowView) {
		// mouse over row label
		return rowHit;
	}

	return HitCell;
}

// ---------------------------------------------------------------------------
// Key pressed...
//
boolean VTableSubView::key(int k, char ch)
{
	// pass on to tableView
	return table->key(k, ch);
}

#if 0

// ---------------------------------------------------------------------------
// 
boolean VTableSubView::givenFocus()
{
	if (parent) {
		parent->setChildFocus(this);
	}
	return FALSE;
}

#endif

#if !defined(CV_NO_DRAG_AND_DROP)

// ---------------------------------------------------------------------------
//
void VTableSubView::enterTrack(VWindow* a_pWindow, VDragItem* a_dragItem)
{
	dropRow = -1;
	dropCol = -1;
}

// ---------------------------------------------------------------------------
//
void VTableSubView::leaveTrack(VWindow* a_pWindow, VDragItem* a_dragItem)
{
	if (dropRow >= 0 || dropCol >= 0) {
		hiliteTrack(dropRow, dropCol);
	}
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::inTrack(VWindow* a_pWindow,
							   VDragItem* a_dragItem, int mx, int my)
{
	long  cr, cc;
	HitType hit;
	boolean acceptDrop = FALSE;
	
	hit = hitTest(mx, my, &cr, &cc);
	if (hit != HitCell) {
		cr = -1;
		cc = -1;
	}
	
	acceptDrop = tblModel->acceptOver(table, a_dragItem, cr, cc);
	if (!acceptDrop) {
		cr = -1;
		cc = -1;
	}
	
	if (dropRow != cr || dropCol != cc) {
		if (dropRow >= 0 || dropCol >= 0) {
			hiliteTrack(dropRow, dropCol);
		}
		
		dropRow = cr;
		dropCol = cc;
		
		if (dropRow >= 0 || dropCol >= 0) {
			hiliteTrack(dropRow, dropCol);
		}
	}
	
	return acceptDrop;
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::acceptDrag(VWindow* a_pWindow, VDragItem* a_dragItem)
{
	return tblModel->acceptDrag(table, a_dragItem);
}

// ---------------------------------------------------------------------------
//
boolean VTableSubView::acceptDrop(VWindow* a_pWindow, VDragItem* a_dragItem)
{
	return tblModel->acceptDrop(table, a_dragItem, dropRow, dropCol);
}

// ---------------------------------------------------------------------------
//
void VTableSubView::hiliteTrack(long cr, long cc)
{
	long x = getCellX(cc);
	long y = getCellY(cr);
	int  h = getRowHeight(cr);
	int  w = getColWidth(cc);
	
	VRectangle outBox;
	
	switch (table->hiMode) {
		case VTableView::CellWise:
			outBox.set(CornerDim, (int)(x - getXOffset())+2,
							(int)(y - getYOffset()), w-4, h);
			break;
		
		case VTableView::RowWise:
			outBox.set(CornerDim, 0, (int) (y - getYOffset()), maxX, h);
			break;
		
		case VTableView::ColumnWise:
			outBox.set(CornerDim, (int) (x - getXOffset()), 0, w, maxY);
			break;
	}
	
	VPen hiPen(VColor::Magenta);
	
	thePort->open();
	thePort->usePen(&hiPen);
	thePort->rule(XorRule);
	
	thePort->fillRegion(&outBox);
	
	thePort->rule(CopyRule);
	thePort->usePen(0);
	thePort->close();
}

#endif
