// $Header:   Q:/views/common/vcs/tablemdl.cpv   1.8   May 30 1997 17:16:46   scottt  $ 

//	tablemdl.cpp
//
//	VTableModel class implementation [Common]
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
//  08/09/93 jmd	removed free
//	09/22/93 jmd	preened
//	02/09/94 jmd	now uses longs for row,coll addresses
//	05/17/95 pkt	preened.
//	07/07/95 pkt	reverted to version without drag & drop.
//	07/11/95 dgm	merged in drag-and-drop functionality.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "tablemdl.h"
#include "tablevw.h"
#include "port.h"

defineClass(VTableModel, VObject)

// ---------------------------------------------------------------------------
//
VTableModel::VTableModel()
{
	init();
}

// ---------------------------------------------------------------------------
//
VTableModel::~VTableModel()
{
	;
}

// ---------------------------------------------------------------------------
//
void VTableModel::init()
{
	defRowHeight = 20;
	defColWidth = 30;
}

// ---------------------------------------------------------------------------
// Return the width of the entire table
// (or a good guess at least (needed for scroll bars))
//
long VTableModel::getWidth()
{
	long	i, cols;
	long	w = 0L;

	cols = getColCount();

	// inefficient: should be overriden by derived classed
	for (i = 0L; i < cols; i++) {
		w += getColWidth(i);
	}
	return w;
}

// ---------------------------------------------------------------------------
// Return the height of the entire table
// (or a good guess at least (needed for scroll bars))
//
long VTableModel::getHeight()
{
	long	i, rows;
	long	h = 0L;

	rows = getRowCount();

	// inefficient: should be overriden by derived classed
	for (i = 0L; i < rows; i++) {
		h += getRowHeight(i);
	}
	return h;
}

// ---------------------------------------------------------------------------
// Return the width of column 'cc'
//
int VTableModel::getColWidth(long /* cc */)
{
	return getDefColWidth();
}

// ---------------------------------------------------------------------------
// Return the height of row 'cr'
//
int VTableModel::getRowHeight(long /* cr */)
{
	return getDefRowHeight();
}

// ---------------------------------------------------------------------------
// Set the height of row 'cr'
// Return TRUE if proccessed.
//
boolean VTableModel::setRowHeight(long /* cr */, int /* h */)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
// Set the width of column 'cc'
// Return TRUE if proccessed.
//
boolean VTableModel::setColWidth(long /* cc */, int /* w */)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
// Return the number of columns in the table.
//
long VTableModel::getColCount()
{
	return 1L;
}

// ---------------------------------------------------------------------------
// Return the number of rows in the table.
//
long VTableModel::getRowCount()
{
	return 1L;
}

// ---------------------------------------------------------------------------
// Paint the contents of cell (cr, cc).
//
void VTableModel::paintCell(VTableView * /* table */,
							long /* cr */, long /* cc */)
{
	;
}

// ---------------------------------------------------------------------------
// Paint row label for row 'cr'
//
void VTableModel::paintRowLabel(VTableView * /* table */, long /* cr */)
{
	;
}

// ---------------------------------------------------------------------------
// Paint column label for column 'cc'
//
void VTableModel::paintColLabel(VTableView * /* table */, long /* cc */)
{
	;
}

#if !defined(CV_NO_DRAG_AND_DROP)

// ---------------------------------------------------------------------------
//
void VTableModel::hitCell(VTableView *table, long cr, long cc)
{
	table->gotoCell(cr, cc);
}

// ---------------------------------------------------------------------------
//
void VTableModel::hitColumn(VTableView *table, long cc)
{
	table->gotoCell(table->getCurrRow(), cc);
}

// ---------------------------------------------------------------------------
//
void VTableModel::hitRow(VTableView *table, long cr)
{
	table->gotoCell(cr, table->getCurrCol());
}

// ---------------------------------------------------------------------------
//
VPopupMenu* VTableModel::getCellMenu(VTableView *table, long cr, long cc)
{
	return 0;
}

// ---------------------------------------------------------------------------
//
boolean VTableModel::acceptDrag(VTableView *table, VDragItem *dragItem)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VTableModel::acceptOver(VTableView *table,
								VDragItem *dragItem, long cr, long cc)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VTableModel::acceptDrop(VTableView *table,
								VDragItem *dragItem, long cr, long cc)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
//
VObject* VTableModel::getObject(VTableView *table, long cr, long cc)
{
	return 0;
}

#endif
