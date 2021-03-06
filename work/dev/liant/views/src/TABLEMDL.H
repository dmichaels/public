// $Header:   Q:/views/common/vcs/tablemdl.h_v   1.8   May 30 1997 17:16:46   scottt  $ 

//	tablemdl.h
//
//	VTableModel class interface [Common]
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
//	07/12/93 pkt	merged Windows and Motif versions.
//	08/09/93 jmd	removed free
//	09/22/93 jmd	preened
//	02/09/94 jmd	now uses longs for row,coll addresses
//	05/17/95 pkt	preened.
//	07/07/95 pkt	reverted to version without drag & drop.
//	07/11/95 dgm	merged in drag-and-drop functionality.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VTABLEMDL_H
#define VTABLEMDL_H

#include "object.h"

CLASS	VClass;
CLASS	VObject;
CLASS	VPopupMenu;
CLASS	VTableView;

#if !defined(CV_NO_DRAG_AND_DROP)
CLASS	VDragItem;
#endif

extern VClass *VTableModelCls;

CLASS VTableModel : public VObject {
public:
					VTableModel();
					~VTableModel();
					
	void			init();

	int				getDefRowHeight();
	int				getDefColWidth();
	void			setDefRowHeight(int h);
	void			setDefColWidth(int w);

	virtual int		getRowHeight(long cr);
	virtual int		getColWidth(long cc);

	virtual long	getHeight();
	virtual long	getWidth();

	virtual boolean	setRowHeight(long cr, int h);
	virtual boolean	setColWidth(long cc, int w);

	virtual long	getRowCount();
	virtual long	getColCount();

	virtual	void	paintCell(VTableView *table, long cr, long cc);
	virtual	void	paintRowLabel(VTableView *table, long cr);
	virtual	void	paintColLabel(VTableView *table, long cc);

	VClass		   *iam();

#if !defined(CV_NO_DRAG_AND_DROP)
	virtual	void		hitCell(VTableView *, long, long);
	virtual	void		hitColumn(VTableView *, long);
	virtual	void		hitRow(VTableView *, long);
	virtual	VPopupMenu *getCellMenu(VTableView *, long, long);
	virtual	boolean		acceptDrag(VTableView *, VDragItem *);
	virtual	boolean		acceptOver(VTableView *, VDragItem *, long, long);
	virtual	boolean		acceptDrop(VTableView *, VDragItem *, long, long);
	virtual VObject	   *getObject(VTableView *, long, long);
#endif

private:
	int				defColWidth;
	int				defRowHeight;
};

// ---------------------------------------------------------------------------
//
inline int VTableModel::getDefRowHeight() 
{
	return defRowHeight; 
}

// ---------------------------------------------------------------------------
//
inline int VTableModel::getDefColWidth() 
{
	return defColWidth; 
}

// ---------------------------------------------------------------------------
//
inline void	VTableModel::setDefRowHeight(int h) 
{
	defRowHeight = h; 
}

// ---------------------------------------------------------------------------
//
inline void	VTableModel::setDefColWidth(int w) 
{
	defColWidth = w; 
}

#endif // VTABLEMDL_H
