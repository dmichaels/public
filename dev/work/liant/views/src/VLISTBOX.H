// $Header:   Q:/views/common/vcs/vlistbox.h_v   1.3   May 30 1997 17:17:14   scottt  $

//	vlistbox.h
//
//	VVirtualListBox interface [Common]
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
//	07/29/94 jmd	Original.
//	07/26/94 dgm	Updated.
//	01/30/95 ***	3.0.5 release for OSF/Motif.
//	08/21/95 dgm	Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//	09/20/95 dss	VStyle changes.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VVLISTBOX_H
#define VVLISTBOX_H

// ---------------------------------------------------------------------------
//
#include "view.h"
#include "str.h"
#include "color.h"

// ---------------------------------------------------------------------------
//
CLASS	VBitMap;
CLASS	VBrush;
CLASS	VColor;
CLASS	VFont;
CLASS	VPen;
CLASS	VPort;
CLASS	VRectangle;
CLASS	VTimer;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// ---------------------------------------------------------------------------
//
CLASS VVirtualListBox : public VView {
public:

	struct		RowData {
							RowData(long rowcount = 0);
		VString				theText;
		VBitMap			   *theBitMap;
		long				rowCount;
		boolean				isMarked;
		VColor				markedColor;
	};

				VVirtualListBox(const VFrame& frame,
								VWindow *parent_window,
								const VStyle& style = StyleDefault,
								int minimum_row_height = 1);
				VVirtualListBox();
			   ~VVirtualListBox();
	VClass	   *iam();

	void		uponEvent(VObject *client, method get_row_data,
										   method select_notify = 0,
										   method double_click_notify = 0);

	void		getStringAt(long row, VString& s);
	void		selectItem(long row);
	long		selectedItem() const;

	void		setFont(VFont *f);

	void		setMinRowHeight(int mh);
	int			getMinRowHeight() const;
	void		setTextIndent(int i);
	int			getTextIndent() const;

	void		setTracking(boolean);
	boolean		isTracking() const;

	// Archival and Retrieval

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a);
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *pwin,
						boolean do_realize = TRUE);
	void		realize(VWindow *pwin);
	integer		getFetchMethodIndex() const;
	void		setFetchMethodIndex(integer);
	integer		getSingleClickMethodIndex() const;
	void		setSingleClickMethodIndex(integer);
	integer		getDoubleClickMethodIndex() const;
	void		setDoubleClickMethodIndex(integer);
#endif

protected:
	boolean		paint();
	boolean		resized(int w, int h);
	boolean		mouseDn(int mx, int my);
	boolean		mouseDbl(int mx, int my);
	boolean		mouseUp(int mx, int my);
	boolean		mouseMv(int mx, int my, int bStat);
	boolean 	key(int k, char ch);
	boolean		givenFocus();
	boolean		clearFocus();
public:
	boolean		takeFocus();
protected:

	long		getRowFromPosition(int x, int y);

	boolean		drawRow(long, boolean, boolean, boolean = FALSE);
	void		drawFocusRing(long, boolean);

	void		init();
	void		initGUI(int minrowhgt);
	boolean		vtScrollBar(long i);
	boolean		updateScrollBars();

	void		scrollList(long drow);
	boolean		gotoRow(long row,
						boolean new_selection = TRUE,
						boolean using_key = FALSE);
	boolean		getRowData(long row, RowData&);
	boolean		timesUp(VTimer *t);

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

private:
	VPort	   *thePort;
	VPen	   *foregroundPen;
	VBrush	   *backgroundBrush;
	VPen	   *selectedForegroundPen;
	VBrush	   *selectedBackgroundBrush;
	VPen	   *markedForegroundPen;
	VPen	   *focusRingPen;
	VPen	   *backgroundPen;
	VTimer	   *theTimer;
	VRectangle *selectionRectangle;
	VRectangle *focusRingTemplate;

	int			scrollFlag; 	/* -1 mouse above, 1 mouseBelow, 0 no scroll */
	int			inScrollCallBack;

	short		rowHeight;		/* height of each row (pixels) */
	long		rowOffset;		/* number of rows scrolled */
	long		page;			/* number of rows that can be shown */
	long		rowCount;		/* total number of rows */

	VObject	   *client;
	method		selectMthd;
	method		dblClkMthd;
	method		fetchMthd;

	int			minRowHeight;	/* minumum height of each row (pixels) */
	int			rowIndent;		/* amount to indent each row text (pixels) */
	int			bitmap_x;		/* amount to indent each row bitmap (pixels) */
	long		currRowNo;		/* current row number */
	long		currFocusRowNo;	/* current focused row number */

	int			hasFocus : 1;

	int			swSolidFocusRing : 1;
	int			swDetachableFocusRing : 1;
	int			swThickFocusRing : 1;
	int			swFocusRingGap : 1;
	int			swNarrowMouseTracking : 1;

#ifndef CV_NOARCHIVER
	integer		fetchMthdIndex;
	integer		selectMthdIndex;
	integer		dblClkMthdIndex;
#endif

};

extern VClass *VVirtualListBoxCls;

// ---------------------------------------------------------------------------
//
inline long VVirtualListBox::selectedItem() const
{
	return currRowNo;
}

// ---------------------------------------------------------------------------
// Set the size, in pixels, to indent the text for each row in the list.
//
inline void VVirtualListBox::setTextIndent(int i)
{
	rowIndent = i;
	this->update();
}

// ---------------------------------------------------------------------------
//
inline int VVirtualListBox::getTextIndent() const
{
	return rowIndent;
}

// ---------------------------------------------------------------------------
// Return the minimum height, in pixels, of each row in the list.
//
inline int VVirtualListBox::getMinRowHeight() const
{
	return minRowHeight;
}

// ---------------------------------------------------------------------------
//
inline long VVirtualListBox::getRowFromPosition(int, int y)
{
	return (y / rowHeight) + rowOffset;
}

// ---------------------------------------------------------------------------
//
inline VVirtualListBox::RowData::RowData(long rowcount)
{
	theBitMap = 0;
	rowCount = rowcount;
	isMarked = FALSE;
	markedColor = 0ul;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
inline integer VVirtualListBox::getFetchMethodIndex() const
{
	return fetchMthdIndex;
}

// ---------------------------------------------------------------------------
//
inline void VVirtualListBox::setFetchMethodIndex(integer index)
{
	fetchMthdIndex = index;
}

// ---------------------------------------------------------------------------
//
inline integer VVirtualListBox::getSingleClickMethodIndex() const
{
	return selectMthdIndex;
}

// ---------------------------------------------------------------------------
//
inline void VVirtualListBox::setSingleClickMethodIndex(integer index)
{
	selectMthdIndex = index;
}

// ---------------------------------------------------------------------------
//
inline integer VVirtualListBox::getDoubleClickMethodIndex() const
{
	return dblClkMthdIndex;
}

// ---------------------------------------------------------------------------
//
inline void VVirtualListBox::setDoubleClickMethodIndex(integer index)
{
	dblClkMthdIndex = index;
}

#endif // CV_NOARCHIVER

#endif // VVLISTBOX_H
