// $Header:   Q:/views/common/vcs/tablesub.h_v   1.10   May 30 1997 17:16:50   scottt  $ 

//	tablesub.h
//
//	VTableSubView class interface [Common]
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
//	07/19/93 pat	changed class to CLASS in friend declaration.
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	08/23/93 pat	removed CLASS keyword from friend declaration
//	09/22/93 jmd	preened
//	02/01/94 jmd	made private enum public
//	02/09/94 jmd	now uses longs for row,coll addresses
//	05/17/95 pkt	preened.
//	07/07/95 pkt	reverted to version without drag & drop.
//	07/11/95 dgm	merged in drag-and-drop functionality.
//	09/27/95 dgm	made hitTest() public.
//	05/07/96 dgm	Added rmouseDn().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VTABLESUB_H
#define VTABLESUB_H

// ---------------------------------------------------------------------------
//
#include "view.h"
#include "tablevw.h"

#if !defined(CV_NO_DRAG_AND_DROP)
#include "drag.h"
#endif

// ---------------------------------------------------------------------------
//
#if !defined(CV_NO_DRAG_AND_DROP)
CLASS	VDragHandler;
#endif

CLASS VTableSubView : public VView
#if !defined(CV_NO_DRAG_AND_DROP)
	, public VDragHandler
#endif
{
public:
	enum			VTableSubType { CellView, RowView, ColumnView };

					VTableSubView();
					VTableSubView(VTableModel *tm, const VFrame& f,
								  VTableView *p, VTableSubType tp,
								  boolean dragen = FALSE);
					~VTableSubView();

	VClass		   *iam();

	friend VTableView;

	// must be public because of Borland compiler problems
	enum HitType  {	HitNone,
					HitCell,
					HitColumn,
					HitColumnSep,
					HitRow,
					HitRowSep };

	HitType			hitTest(int mx, int my, long *crp, long *ccp);

	// Drag & Drop

#if !defined(CV_NO_DRAG_AND_DROP)
	void			enterTrack(VWindow *, VDragItem *);
	void			leaveTrack(VWindow *, VDragItem *);
	boolean			inTrack(VWindow *, VDragItem *, int, int);
	boolean			acceptDrag(VWindow *, VDragItem *);
	boolean			acceptDrop(VWindow *, VDragItem *);
	void			hiliteTrack(long, long);
#endif

private:
	enum MouseModeType {
					Normal,
					RowReady,
					RowResize,
					ColumnReady,
					ColumnResize,
					Drag };

	VTableView	   *table;
	VTableModel	   *tblModel;

	VTableSubType	type;

	MouseModeType	mouseMode;
	long			resizeNo;
	int				resizeVal;
	int				resizeBar;

	int				winWidth;
	int				winHeight;

	int				maxX;			// used for limiting size of row-
	int				maxY;			// and column-wise highlight

	VPort		   *thePort;

#if !defined(CV_NO_DRAG_AND_DROP)
	long			dropRow;
	long			dropCol;
	boolean			m_dragen;
	VRectangle		mouseRect;
#endif

	boolean			resized(int w, int h);

	int				getRowHeight(long cr);
	int				getColWidth(long cc);

	void			setRowHeight(long cr, int h);
	void			setColWidth(long cc, int w);

	long			getRowCount();
	long			getColCount();

	long			getCellX(long cc);
	long			getCellY(long cr);

	long			getXOffset();
	long			getYOffset();

	long			getRowOffset();
	long			getColOffset();
	long			getCurrRow();
	long			getCurrCol();
	boolean			getNoOutline();

	void			init(VTableModel *tm, VTableView *p, VTableSubType tp);

	void			drawCellHighlight(long cr, long cc);
	void			updateCell(long cr, long cc);

	int				getWidth();
	int				getHeight();

	boolean			erased();
	boolean			key(int, char);
	boolean			paint();
	boolean			mouseUp(int, int);
	boolean			mouseDn(int, int);
	boolean			mouseDbl(int, int);
	boolean			mouseMv(int, int, int);
	boolean			mouseExit(int);
	boolean			mouseEnter(int);
	boolean			rmouseDn(int, int);
#if 0
	boolean			givenFocus();
#endif

#if defined(MOTIF)
	void			disableClipping();
	void			restoreClipping();
#endif

};

extern VClass *VTableSubViewCls;

// ---------------------------------------------------------------------------
//
inline int VTableSubView::getRowHeight(long cr)
{
	return tblModel->getRowHeight(cr);

}

// ---------------------------------------------------------------------------
//
inline int VTableSubView::getColWidth(long cc)
{
	return tblModel->getColWidth(cc);
}

// ---------------------------------------------------------------------------
//
inline void VTableSubView::setRowHeight(long cr, int h)
{
	table->setRowHeight(cr, h);
}

// ---------------------------------------------------------------------------
//
inline void VTableSubView::setColWidth(long cc, int w)
{
	table->setColWidth(cc, w);
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getRowCount()
{
	return tblModel->getRowCount();
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getColCount()
{
	return tblModel->getColCount();
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getCellX(long cc)
{
	return table->getCellX(cc);
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getCellY(long cr)
{
	return table->getCellY(cr);
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getXOffset()
{
	return table->xOffset;
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getYOffset()
{
	return table->yOffset;
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getRowOffset()
{
	return table->rowOffset;
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getColOffset()
{
	return table->colOffset;
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getCurrRow()
{
	return table->currRow;
}

// ---------------------------------------------------------------------------
//
inline long VTableSubView::getCurrCol()
{
	return table->currCol;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableSubView::getNoOutline()
{
	return table->noOutline;
}

// ---------------------------------------------------------------------------
//
inline int VTableSubView::getWidth()
{
	return winWidth;
}

// ---------------------------------------------------------------------------
//
inline int VTableSubView::getHeight()
{
	return winHeight;
}

#endif // VTABLESUB_H
