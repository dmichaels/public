// $Header:   Q:/views/common/vcs/tablevw.h_v   1.19   Jun 03 1997 14:15:52   scottt  $ 

//  tablevw.h
//
//  VTableView class interface [Common]
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
//  Revision History:
//  -----------------
//  07/19/93 pat    changed class to CLASS in friend declaration.
//  08/09/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  08/23/93 pat    removed CLASS keyword from friend declaration
//  09/22/93 jmd    preened, made private data protected
//  09/29/93 jmd    added getStyle members
//  10/04/93 jmd    added uponEvent
//  10/05/93 jmd    added cellSizing flag
//  11/16/93 jmd    added dummyModel, realize()
//  01/31/94 jmd    preened, added NoHighlight, changed None to NoBackground
//  02/09/94 jmd    now uses longs for row,coll addresses
//  05/17/95 pkt    merged and preened.
//  07/07/95 pkt    reverted to version without drag & drop.
//  07/11/95 dgm    merged in drag-and-drop functionality.
//  07/13/95 pkt    made m_dragen independent of CV_NO_DRAG_AND_DROP.
//  09/01/95 dss    VStyle changes.
//  09/27/95 dgm    Added get[Cell,Row,Column]View().
//  11/16/95 dgm    Added upon[Column,Row]HeaderClick()
//					and virtual [row,column]HeaderClicked().
//  04/10/96 dgm    Added deselectAllCells() and fixed getCellBox().
//  05/07/96 dgm    Added rowRclicked(),columnRclicked(),cellRclicked().
//					uponCellRclicked(),uponRowRclicked(),uponColumnRclicked().
//  05/13/96 dgm    Updated.
//
//	02/04/97 pkt	Implemented 'getClassDefaultStyle'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTABLEVW_H
#define VTABLEVW_H

// ---------------------------------------------------------------------------
//
#include "view.h"
#include "tablemdl.h"

// ---------------------------------------------------------------------------
//
CLASS VTableSubView;

extern VClass *VTableViewCls;

CLASS VTableView : public VView {
public:
    enum HighlightStyle {
                    Outline,
                    Solid,
                    NoHighlight };
    enum HighlightMode {
                    CellWise,
                    RowWise,
                    ColumnWise };
    enum BGStyle {
                    NoBackground,
                    Grid,
                    Panels };

                    VTableView();
                    ~VTableView();
                    VTableView(VTableModel *tm, const VFrame& f, VWindow *p,
                               int rw = 0, int ch = 0,
                               const VStyle& style = StyleDefault,
                               boolean dragen = FALSE);

    boolean         resized(int w, int h);
    boolean         erased();
    boolean         paint();

    VTableModel    *getTableModel();

    void            resetSize();
    void            updateCell(long cr, long cc);

    VClass         *iam();

    VPort          *getPort();
    VRectangle     *getCellBox();

    VRectangle     *getCellBox(long cr, long cc);

    int             getRowHeight(long cr);
    int             getColWidth(long cc);

    void            setRowHeight(long cr, int h);

    void            setColWidth(long cc, int w);

    long            getColCount();
    long            getRowCount();
    long            getCurrCol();
    long            getCurrRow();

    boolean         isCurrCell(long cr, long cc);
    boolean         isCurrRow(long cr);
    boolean         isCurrCol(long cc);

    void            setHighlight(HighlightStyle s, HighlightMode m);

    HighlightStyle  getHighlightStyle() const;
    HighlightMode   getHighlightMode() const;

    void            setBGStyle(BGStyle s);

    BGStyle         getBGStyle() const;

    void            setIndentLabel(boolean tf);

    boolean         isIndentLabel();

    void            setCellSizing(boolean);
    void            setRowSizing(boolean);
    void            setColumnSizing(boolean);
    boolean         isCellSizing() const;
    boolean         isRowSizing() const;
    boolean         isColumnSizing() const;

    boolean         gotoCell(long cr, long cc);

    virtual boolean key(int k, char ch);
    virtual boolean cellEnter(long cr, long cc);
    virtual boolean cellExit(long cr, long cc);

	virtual boolean	columnHeaderClicked(int);
	virtual boolean	rowHeaderClicked(int);

	virtual void	cellRclicked(long, long, int, int);
	virtual void	rowRclicked(long, int, int);
	virtual void	columnRclicked(long, int, int);

    void            uponEvent(VObject *clnt,
                              method _cellenter = NIL_METHOD,
                              method _cellexit = NIL_METHOD,
                              method _celldblclick = NIL_METHOD);

	void			uponColumnHeaderClick(VObject *, method);
	void			uponRowHeaderClick(VObject *, method);

	void			uponCellRclicked(VObject *, method);
	void			uponRowRclicked(VObject *, method);
	void			uponColumnRclicked(VObject *, method);

    void            updateScrollBars();
    long            getVisibleRows();
    long            getVisibleCols();

    VTableSubView  *getCellView() const;
    VTableSubView  *getRowView() const;
    VTableSubView  *getColumnView() const;

	void			deselectAllCells();

#ifndef CV_NOARCHIVER
    void            realize(VWindow *parent);
#endif

    void            setTableModel(VTableModel *tm, int rw, int ch);

#if 0
    boolean         givenFocus() ;
    void            setChildFocus(VWindow *);
#endif

	virtual VStyle&	getDefaultStyle() const;
	static VStyle&	getClassDefaultStyle();

protected:
    VTableModel     dummyModel;     // for times when model == 0

    VTableModel    *tblModel;

    VTableSubView  *cellView;
    VTableSubView  *rowView;
    VTableSubView  *columnView;

    int             rowViewWidth;
    int             colViewHeight;

    long            lastCellRow;
    long            lastCellCol;
    long            lastCellY;
    long            lastCellX;

    long            visRows;
    long            visCols;

    HighlightStyle  hiStyle;
    HighlightMode   hiMode;
    BGStyle         bgStyle;

    unsigned        indentLabel:1;
    unsigned        cellSizing:1;
    unsigned        rowSizing:1;
    unsigned        columnSizing:1;

    long            currCol;
    long            currRow;
    long            xOffset;
    long            yOffset;
    long            rowOffset;
    long            colOffset;
    boolean         noOutline;
    int             updatingScrollBar;

    VPort          *portptr;            // pointer to sub view port
    VRectangle      cellBox;            // box to paint in subview

    VObject        *client;
    method          enterMthd;
    method          exitMthd;
    method          dblclickMthd;

    boolean (VObject::*rowHeaderClickedMethod)(int);
    boolean (VObject::*columnHeaderClickedMethod)(int);

    void	(VObject::*cellRclickedMthd)(long, long, int, int);
    void	(VObject::*rowRclickedMthd)(long, int, int);
    void	(VObject::*columnRclickedMthd)(long, int, int);

#if 1 /* !defined(CV_NO_DRAG_AND_DROP) */
    boolean         m_dragen;
#endif

    void            init(VTableModel *t, int rw, int ch);

    long            getCellY(long cr);
    long            getCellX(long cc);

    boolean         vtScrollBar(long i);
    boolean         hzScrollBar(long i);
    void            scrollTable(long drows, long dcols);
    boolean         scrollAdjust(long cr, long cc);
    boolean         dblClickCell(long cr, long cc);

    friend VTableSubView;

    void            drawCellHighlight(long cr, long cc,
                                      boolean rchange, boolean cchange);

};

// ---------------------------------------------------------------------------
//
inline VTableModel *VTableView::getTableModel()
{
    return tblModel;
}

// ---------------------------------------------------------------------------
//
inline VPort *VTableView::getPort()
{
    return portptr;
}

// ---------------------------------------------------------------------------
//
inline VRectangle *VTableView::getCellBox()
{
    return &cellBox;
}

// ---------------------------------------------------------------------------
//
inline int VTableView::getRowHeight(long cr)
{
    return tblModel->getRowHeight(cr);
}

// ---------------------------------------------------------------------------
//
inline int VTableView::getColWidth(long cc)
{
    return tblModel->getColWidth(cc);
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setRowHeight(long cr, int h)
{
    if (tblModel->setRowHeight(cr, h)) {
        resetSize();
    }
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setColWidth(long cc, int w)
{
    if (tblModel->setColWidth(cc, w)) {
        resetSize();
    } 
}

// ---------------------------------------------------------------------------
//
inline long VTableView::getColCount()
{
    return tblModel->getColCount();
}

// ---------------------------------------------------------------------------
//
inline long VTableView::getRowCount()
{
    return tblModel->getRowCount();
}

// ---------------------------------------------------------------------------
//
inline long VTableView::getCurrCol()
{
    return currCol;
}

// ---------------------------------------------------------------------------
//
inline long VTableView::getCurrRow()
{
    return currRow;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isCurrCell(long cr, long cc)
{
    return (cr == currRow) && (cc == currCol);
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isCurrRow(long cr)
{
    return cr == currRow;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isCurrCol(long cc)
{
    return cc == currCol;
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setHighlight(HighlightStyle s, HighlightMode m)
{
    hiStyle = s;
    hiMode = m;
}

// ---------------------------------------------------------------------------
//
inline VTableView::HighlightStyle VTableView::getHighlightStyle() const
{
    return hiStyle;
}

// ---------------------------------------------------------------------------
//
inline VTableView::HighlightMode VTableView::getHighlightMode() const
{
    return hiMode;
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setBGStyle(BGStyle s)
{
    bgStyle = s;
}

// ---------------------------------------------------------------------------
//
inline VTableView::BGStyle VTableView::getBGStyle() const
{
    return bgStyle;
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setIndentLabel(boolean tf)
{
    indentLabel = tf;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isIndentLabel()
{
    return indentLabel;
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setCellSizing(boolean tf)
{
    rowSizing = tf;
    columnSizing = tf;
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setRowSizing(boolean tf)
{
    rowSizing = tf;
}

// ---------------------------------------------------------------------------
//
inline void VTableView::setColumnSizing(boolean tf)
{
    columnSizing = tf;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isCellSizing() const
{
    return rowSizing && columnSizing;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isRowSizing() const
{
    return rowSizing;
}

// ---------------------------------------------------------------------------
//
inline boolean VTableView::isColumnSizing() const
{
    return columnSizing;
}

// ---------------------------------------------------------------------------
//
inline VTableSubView *VTableView::getCellView() const
{
    return cellView;
}

// ---------------------------------------------------------------------------
//
inline VTableSubView *VTableView::getRowView() const
{
    return rowView;
}

// ---------------------------------------------------------------------------
//
inline VTableSubView *VTableView::getColumnView() const
{
    return columnView;
}

#endif // VTABLEVW_H
