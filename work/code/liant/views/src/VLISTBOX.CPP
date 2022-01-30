// $Header:   Q:/views/common/vcs/vlistbox.cpv   1.7   Jun 13 1997 12:53:42   gregm  $

//	vlistbox.cpp
//
//	VVirtualListBox implementation [Common]
//
//	Allegris Foundation 1.0.00
//	Copyright (c) 1996 by INTERSOLV, Inc.
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
//					!!!todo
//						tabs support?
//						pass 'this' to callback
//	07/26/94 dgm	Updated.
//	08/23/94 dgm	Changed disableFocus() call to setTabStop().
//	08/08/95 jld	changed improve efficiency and deal more gracefully with
//					lists of unknown and changing length
//	08/21/95 dgm	Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//	09/20/95 dss	VStyle changes.
//	12/08/95 dgm	Removed winFont references for Motif;
//					we need to clean this up!
//	12/22/95 tlf	Portable font changes.
// ---------------------------------------------------------------------------

#include "vlistbox.h"
#include "notifier.h"
#include "port.h"
#include "brush.h"
#include "pen.h"
#include "color.h"
#include "clrscm.h"
#include "font.h"
#include "bitmap.h"
#include "timer.h"

#ifndef CV_NOARCHIVER
#	include "archiver.h"
#	include "cllbckls.h"
#endif

#if defined(CV_HAS_SET_AUTO_ERASED)
#define CV_USE_SET_AUTO_ERASED
#endif
#undef CV_USE_SET_AUTO_ERASED

// ---------------------------------------------------------------------------
//
defineClass(VVirtualListBox, VView)
defineArchiveRevision(VVirtualListBox, 0)

// ---------------------------------------------------------------------------
//
VVirtualListBox::VVirtualListBox()
{
	init();
}

// ---------------------------------------------------------------------------
//
VVirtualListBox::VVirtualListBox(const VFrame& f, VWindow *pwin,
								 const VStyle& style, int minrowhgt):
	VView(f, pwin, style + StyleVertical)
{
	init();
	initGUI(minrowhgt);
}

// ---------------------------------------------------------------------------
//
VVirtualListBox::~VVirtualListBox()
{
	delete thePort;
	delete theTimer;
	delete selectionRectangle;
	delete focusRingTemplate;
	delete backgroundBrush;
	delete foregroundPen;
	delete selectedBackgroundBrush;
	delete selectedForegroundPen;
	delete markedForegroundPen;
	delete focusRingPen;
	delete backgroundPen;
}

// ---------------------------------------------------------------------------
//
void VVirtualListBox::init()
{
	client					= 0;
	selectMthd				= 0;
	dblClkMthd				= 0;
	fetchMthd				= 0;
	inScrollCallBack		= 0;

	currRowNo				= -1L;
	currFocusRowNo			= -1L;
	rowOffset				= 0L;
	rowCount				= 0L;
	rowIndent				= 0;
	rowHeight				= 0;
	minRowHeight			= 0;
	scrollFlag				= 0;
	hasFocus				= 0;

// #if !defined(CV_MOTIF)
	winFont					= 0;
// #endif
	thePort					= 0;
	theTimer				= 0;

	backgroundBrush			= 0;
	foregroundPen			= 0;
	selectedBackgroundBrush	= 0;
	selectedForegroundPen	= 0;
	markedForegroundPen		= 0;
	focusRingPen			= 0;
	backgroundPen			= 0;
	

#if defined(MOTIF)
	swSolidFocusRing		= 1;
	swDetachableFocusRing	= 1;
	swThickFocusRing		= 0;
	swFocusRingGap			= 1;
	swNarrowMouseTracking	= 0;
#else
	if( style.contains( StyleDetachFocus ) ) {
		swSolidFocusRing		= 1;
	}
	else {
		swSolidFocusRing		= 0;
	}
	swDetachableFocusRing	= 0;
	swThickFocusRing		= 0;
	swFocusRingGap			= 0;
	swNarrowMouseTracking	= 0;
#endif

	bitmap_x				= 2;

#ifndef CV_NOARCHIVER
	fetchMthdIndex			= -1L;
	selectMthdIndex			= -1L;
	dblClkMthdIndex			= -1L;
#endif

}

// ---------------------------------------------------------------------------
//
void VVirtualListBox::initGUI(int minrowhgt)
{

#if defined(CV_USE_AUTO_ERASED)
	this->setAutoErased(TRUE);
	this->setBackground(this->getBackground());
#endif

	thePort = new VPort(this);
// #if !defined(CV_MOTIF)
	thePort->useFont(winFont);
// #endif

	thePort->open();
	rowHeight = thePort->textLineSpace();
	thePort->close();

	selectionRectangle = new VRectangle;
	focusRingTemplate = new VRectangle;

	minRowHeight = minrowhgt;
	if (rowHeight <= minRowHeight) {
		rowHeight = minRowHeight;
	}

	if (swFocusRingGap) {
		rowHeight++;
		bitmap_x++;
	}
	if (swThickFocusRing) {
		rowHeight++;
		bitmap_x++;
	}

	// set up scroll bars
	if (vScroll != 0) {
		vScroll->uponChange(this, methodOf(VVirtualListBox, vtScrollBar));
		vScroll->tracking(FALSE);
#if defined(MOTIF)
		vScroll->setTabStop(FALSE);
#endif
	}

	VColor background_color, foreground_color;
	VColor selected_background_color, selected_foreground_color;
	VColor focus_ring_color;

	VColorScheme color_scheme;

#if 1
	color_scheme.getColor
		(VColorScheme::FlatBackground, background_color);
	color_scheme.getColor
		(VColorScheme::FlatForeground, foreground_color);
	color_scheme.getColor
		(VColorScheme::FlatBackgroundSelect, selected_background_color);
	color_scheme.getColor
		(VColorScheme::FlatForegroundSelect, selected_foreground_color);
	color_scheme.getColor
		(VColorScheme::FlatForeground, focus_ring_color);
#else
#if defined(MOTIF)
	this->getBackgroundColor(background_color);
	this->getForegroundColor(foreground_color);
	selected_background_color	= foreground_color;
	selected_foreground_color	= background_color;
	focus_ring_color			= selected_background_color;
#else
	background_color			= VColor(VColor::White);
	foreground_color			= VColor(VColor::Black);
	selected_background_color	= foreground_color;
	selected_foreground_color	= background_color;
	focus_ring_color			= VColor(VColor::Red);
#endif
#endif

	backgroundBrush				= new VBrush(background_color);
	foregroundPen				= new VPen(foreground_color);
	selectedBackgroundBrush		= new VBrush(selected_background_color);
	selectedForegroundPen		= new VPen(selected_foreground_color);
	markedForegroundPen			= new VPen();

	focusRingPen				= new VPen(focus_ring_color);
	focusRingPen->pattern(swSolidFocusRing ? SolidLine : DotLine);
	focusRingPen->width(swThickFocusRing ? 2 : 1);

	backgroundPen				= new VPen(background_color);

	setBackground(backgroundBrush);

	updateScrollBars();
}


// ---------------------------------------------------------------------------
// Set the minimum height, in pixels, of each row in the list.
//
void VVirtualListBox::setMinRowHeight(int mh)
{
	minRowHeight = mh;

	if (rowHeight <= minRowHeight) {
		rowHeight = minRowHeight;
	}

	if (swFocusRingGap) {
		rowHeight++;
	}
	if (swThickFocusRing) {
		rowHeight++;
	}

	this->update();
}

// ---------------------------------------------------------------------------
// Set the VFont object used by the VirtualListBox.
//
void VVirtualListBox::setFont(VFont *f)
{
// #if !defined(CV_MOTIF)
	winFont = f;
// #endif
	thePort->useFont(f);

	thePort->open();
	rowHeight = thePort->textLineSpace();
	thePort->close();

	if (rowHeight <= minRowHeight) {
		rowHeight = minRowHeight;
	}

	if (swFocusRingGap) {
		rowHeight++;
	}
	if (swThickFocusRing) {
		rowHeight++;
	}

	update();
}

// ---------------------------------------------------------------------------
//
boolean VVirtualListBox::getRowData(long row, RowData& rowdata)
{
	if ((client != 0) && (fetchMthd != 0) &&
		(client->perform(fetchMthd, row, long(&rowdata))) )
		{
			rowCount = rowdata.rowCount;
			return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Called when this object has been given the keyboard focus.
//	
boolean VVirtualListBox::givenFocus() 
{
	if (parent != 0) {
		parent->setChildFocus(this);
	}
	drawFocusRing(currFocusRowNo, TRUE);
	hasFocus = 1;
	return FALSE; 
}

// ---------------------------------------------------------------------------
boolean VVirtualListBox::clearFocus()
{
	drawFocusRing(currFocusRowNo, FALSE);
	hasFocus = 0;
	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VVirtualListBox::takeFocus()
{
	return VWindow::takeFocus();
}

// ---------------------------------------------------------------------------
void VVirtualListBox::getStringAt(long row, VString& s)
{
	RowData rdata(rowCount);
	getRowData(row, rdata);
	s = rdata.theText;
}

// ---------------------------------------------------------------------------
//
void VVirtualListBox::selectItem(long row)
{
	gotoRow(row, TRUE, TRUE);
}

// ---------------------------------------------------------------------------
void VVirtualListBox::uponEvent(VObject *clnt, method fetch, 
								method select, method dblmthd)
{
	if (clnt != 0) {
		client = clnt; 
	}
	if (fetch != 0) {
		fetchMthd = fetch; 
	}
	if (select != 0) {
		selectMthd = select; 
	}
	if (dblmthd != 0) {
		dblClkMthd = dblmthd; 
	}

	if (clnt == 0) {
		client = 0;
		selectMthd = 0;
		dblClkMthd = 0;
		fetchMthd = 0;
	}

	// get row data
	RowData rdata(rowCount);
	getRowData(0L, rdata);
}

// ---------------------------------------------------------------------------
boolean VVirtualListBox::resized(int ww, int wh)
{
	// test if we need to scroll to compensate for new height
	if (rowHeight > 0) {
		page = long(wh / rowHeight);
		
		if (swThickFocusRing) {
			focusRingTemplate->set(CornerDim, 1, 1, ww - 1, rowHeight - 1);
			selectionRectangle->set(CornerDim, 2, 2, ww - 4, rowHeight - 4);
		}
		else {
			focusRingTemplate->set(CornerDim, 0, 0, ww, rowHeight);
			selectionRectangle->set(CornerDim, 1, 1, ww - 2, rowHeight - 2);
		}
		if (swFocusRingGap) {
			selectionRectangle->inset(1, 1);
		}
	}

	if (rowHeight > 0 && rowOffset > 0L && ((page + rowOffset) > rowCount)) {
		long drows = (page + rowOffset) - rowCount;
		drows = (drows > rowOffset) ? rowOffset : drows;
		scrollList(-drows);
	}
	else {
		updateScrollBars();
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// update the thumb positions and ranges of the scroll bars
// returns TRUE if the window was repainted
//
boolean VVirtualListBox::updateScrollBars()
{
	long	linesNotVisible;
	boolean ret = FALSE;

	// update scroll bars
	if (vScroll != 0) {
		if (rowCount == 0 || rowHeight == 0) {
			vScroll->hide();
		}
		else {
			linesNotVisible = rowCount - page;
			if (linesNotVisible < 0L) {
				if (!vScroll->isHidden() || rowOffset != 0L) {
					vScroll->hide();
					rowOffset = 0L;
					update();
					ret = TRUE;
				}
			}
			else {
				vScroll->setPos(rowOffset);
				vScroll->setValues(linesNotVisible, 0L, 1L, page);
				if (vScroll->isHidden()) {
					vScroll->show();
				}
			}
		}
	}

	return ret;
}

// ---------------------------------------------------------------------------
// the vertical scrollbar has changed position (i is in rows)
//
boolean VVirtualListBox::vtScrollBar(long i)
{
	inScrollCallBack++;

	if (vScroll && inScrollCallBack <= 1) {
		if (vScroll->getMax() <= 0L) {
			i = 0L;
		}
		scrollList(i - rowOffset);
	}

	inScrollCallBack--;

	return TRUE;
}

// ---------------------------------------------------------------------------
// Scroll the list the specified amount (in rows)
//
void VVirtualListBox::scrollList(long drow)
{
	// adjust offsets
	drow = (-drow > rowOffset) ? -rowOffset : drow;
	rowOffset += drow;

	if (drow > page || -drow > page) {
		update();
	}
	else {
		scroll(0, int(-drow * rowHeight));
	}

	updateScrollBars();
}

// ---------------------------------------------------------------------------
boolean VVirtualListBox::paint()
{
	if (thePort == 0) {
		return TRUE;
	}
	if (!thePort->open()) {
		return FALSE;
	}

	VRectangle exposed_rectangle;
	notifier->getExposedRectangle(exposed_rectangle);

	if ((exposed_rectangle.getWidth() <= 0) ||
		(exposed_rectangle.getHeight() <= 0)) {
		return TRUE;
	}

	int ymin, ymax, dummy;
	exposed_rectangle.get(Corners, &dummy, &ymin, &dummy, &ymax);

	long startrow	= (ymin / rowHeight) + rowOffset;
	long endrow		= (ymax / rowHeight) + rowOffset;
	long row		= startrow; 
	while( row <= endrow && drawRow(row, row == currRowNo,
									hasFocus && (row == currFocusRowNo),
									TRUE) )
		row++;

	thePort->close();

	updateScrollBars();

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VVirtualListBox::drawRow(long row,
							  boolean selected,
							  boolean focused,
							  boolean background_already_drawn)
{
	RowData	rowdata(rowCount);
 
 	if (!getRowData(row, rowdata)) {
		return FALSE;
	}

	if ((row < rowOffset) || (row > rowOffset + page)) {
		return TRUE;
	}

	if (thePort == 0) {
		return TRUE;
	}
	boolean	port_was_open = thePort->isOpen();
	if (!port_was_open) {
		if (!thePort->open()) {
			return TRUE;
		}
	}

	int	y = int((row - rowOffset) * rowHeight);

	if (selected) {
		thePort->useBrush(selectedBackgroundBrush);
		selectionRectangle->move(0, y);
		if (swFocusRingGap && !background_already_drawn) {
			// erase gap between focus ring and selection
			thePort->usePen(backgroundPen);
			backgroundPen->width(1);
			thePort->frameRegion(selectionRectangle);
		}
		thePort->fillRegion(selectionRectangle, FALSE);
		selectionRectangle->move(0, -y);
	}
	else if (!background_already_drawn) {
		thePort->useBrush(backgroundBrush);
		selectionRectangle->move(0, y);
		if (swFocusRingGap) {
			// expand selection rectangle to erase gap 
			// between focus ring and selection
			selectionRectangle->inset(-1, -1);
			thePort->fillRegion(selectionRectangle, FALSE);
			selectionRectangle->inset(1, 1);
		}
		else {
			thePort->fillRegion(selectionRectangle, FALSE);
		}
		selectionRectangle->move(0, -y);
	}

	if (rowdata.isMarked) {
		markedForegroundPen->color(rowdata.markedColor);
		thePort->usePen(markedForegroundPen);
	}
	else {
		thePort->usePen(selected ? selectedForegroundPen : foregroundPen);
	}

	int	text_w, text_h, text_y;
	thePort->textSize(rowdata.theText, &text_w, &text_h);
	text_y = y + (rowHeight / 2) - (text_h / 2);
	thePort->wrtText(rowdata.theText, rowIndent, text_y);

	if (rowdata.theBitMap != 0) {
		int bitmap_w, bitmap_h, bitmap_y;
		rowdata.theBitMap->sizeOfImage(&bitmap_w, &bitmap_h);
		bitmap_y = y + (rowHeight / 2) - (bitmap_h / 2);
		thePort->drawBitMap(rowdata.theBitMap, bitmap_x, bitmap_y);
	}

	drawFocusRing(row, focused);

	if (!port_was_open) {
		thePort->close();
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VVirtualListBox::drawFocusRing(long row, boolean focused)
{
	if ((thePort == 0) ||
		(row < rowOffset) || (row > rowOffset + page)) {
		return;
	}

	boolean	port_was_open = thePort->isOpen();

	if (!port_was_open) {
		if (!thePort->open()) {
			return;
		}
	}

	if (focused) {
		thePort->usePen(focusRingPen);
	}
	else {
		thePort->usePen(backgroundPen);
		backgroundPen->width(focusRingPen->width());
	}

	int y = int((row - rowOffset) * rowHeight);
	focusRingTemplate->move(0, y);
	thePort->frameRegion(focusRingTemplate);
	focusRingTemplate->move(0, -y);

	if (!port_was_open) {
		thePort->close();
	}
}

// ---------------------------------------------------------------------------
//
boolean VVirtualListBox::gotoRow(long row,
							  boolean select_row, boolean focus_on_row)
{
	if (row < 0L) {
		row = 0L;
	}

	if( select_row && focus_on_row ) {
		if( currRowNo == row ) {
			drawFocusRing( row, TRUE );
			// avoid the selection callback
			select_row = FALSE;
		}
		else if( drawRow(row, TRUE, TRUE) ) { 
				drawRow(currRowNo, FALSE, FALSE);
			 }
		else {
			return FALSE;
		}
		currRowNo = row;

		if( currFocusRowNo != currRowNo ) {
			drawFocusRing( currFocusRowNo, FALSE );
		}
		currFocusRowNo = row;
	}
	else if( select_row ) {
		if( currRowNo == row ) {
			// avoid the selection callback
			select_row = FALSE;
		}
		else if( drawRow(row, TRUE, FALSE) ) {
				drawRow(currRowNo, FALSE, (currRowNo == currFocusRowNo) );
		}
		else {
			return FALSE;
		}
		currRowNo = row;
	}
	else if( focus_on_row ) { 
		if( currFocusRowNo != row ) {
			drawFocusRing( currFocusRowNo, FALSE );
			drawFocusRing( row, TRUE );
			currFocusRowNo = row;
		}
		// else nothing to do
	}
		
	// test if new row is visible

	if (row < rowOffset) {
		scrollList(row - rowOffset);
	}
	else {
  		long k = rowOffset + page;
		if (row >= k) {
			scrollList(row - k + 1);
		}
	}

	if (select_row && (client != 0) && (selectMthd != 0)) {
		client->perform(selectMthd, currRowNo);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Mouse pressed in window
//
boolean VVirtualListBox::mouseDn(int, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

	long		row = (my / rowHeight) + rowOffset;

	gotoRow(row, TRUE, TRUE);

	notifier->captureMouseFor(this);
	notifier->mouseTracking(TRUE);

	scrollFlag = 0;
	theTimer = new VTimer;
	if (theTimer != 0) {
		theTimer->uponTimeout(this, methodOf(VVirtualListBox, timesUp));
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VVirtualListBox::mouseDbl(int, int)
{
	if (!isEnabled()) {
		return FALSE;
	}

	if (currRowNo >= 0L && client != 0 && dblClkMthd != 0) {
		client->perform(dblClkMthd, currRowNo);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Mouse moved in window
//
boolean VVirtualListBox::mouseMv(int mx, int my, int)
{
	if (!isEnabled()) {
		return FALSE;
	}

	int			ww, wh;
	sizeOfImage(&ww, &wh);

	if (swNarrowMouseTracking && (mx < 0 || mx > ww)) {
		if (theTimer != 0) {
			theTimer->stop();
		}
		return TRUE;
	}
	if (my < 0 || my > wh) {
		if (theTimer != 0) {
			scrollFlag = (my < 0) ? -1 : 1;
			theTimer->start(50);
		}
	}
	else {
		if (scrollFlag != 0) {
			scrollFlag = 0;
			if (theTimer != 0) {
				theTimer->stop();
			}
		}

		long	row = (my / rowHeight) + rowOffset;
		gotoRow(row, TRUE, !swDetachableFocusRing);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Mouse released in window
//
boolean VVirtualListBox::mouseUp(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

	delete theTimer;
	theTimer = 0;
	scrollFlag = 0;

 	int ww, wh;
	sizeOfImage(&ww, &wh);
	if( mx < 0 || mx > ww || my < 0 || my > wh ) {
		gotoRow(currFocusRowNo, TRUE, TRUE);
	}
	else {
		gotoRow(currRowNo, TRUE, TRUE);
	}

	notifier->mouseTracking(FALSE);
	notifier->releaseCapture();

	return TRUE;
}

// ---------------------------------------------------------------------------
boolean VVirtualListBox::timesUp(VTimer *t)
{
	t->pause();

	gotoRow(currRowNo + scrollFlag, TRUE, !swDetachableFocusRing);
	t->resume();

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Key pressed within the VVirtualListBox
//
boolean VVirtualListBox::key(int k, char)
{
	switch(k) {
	case K_Up:
		gotoRow(currRowNo - 1L, TRUE, TRUE);
		break;

	case K_Down:
		gotoRow(currRowNo + 1L, TRUE, TRUE);
		break;

	case K_space:
		break;

	case K_Return:
		if (currRowNo >= 0L && client != 0 && dblClkMthd != 0) {
			client->perform(dblClkMthd, currRowNo);
		}
		break;

	case K_Prior:
		if (rowHeight > 0) {
			scrollList( -page );
		}
		break;

 	case K_Next:
		if (rowHeight > 0) {
			scrollList( page );
		}
		break;

	case K_Home:
		gotoRow(0L, FALSE, FALSE);
		break;

	case K_End:
		gotoRow(rowCount - 1L, FALSE, FALSE);
		break;

	case K_Tab:
		if ((parent != 0) && !isTopLevel()) {
			parent->nextChildFocus(this);
		}
		break;

	case (K_Tab | K_Mod_Shift):
		if ((parent != 0) && !isTopLevel()) {
			parent->prevChildFocus(this);
		}
		break;
	}

	return TRUE;
}
// ---------------------------------------------------------------------------
//
void VVirtualListBox::setTracking(boolean b)
{
	if (vScroll != 0) {
		vScroll->tracking(b);
	}
}

// ---------------------------------------------------------------------------
//
boolean VVirtualListBox::isTracking() const
{
	if (vScroll != 0) {
		return vScroll->isTracking();
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------

#ifndef CV_NOARCHIVER
// --------------------------------------------------------------------------
// VVirtualListBox::putTo
//
void VVirtualListBox::putTo(VArchiver& a)
{
	VVirtualListBox::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
// VVirtualListBox::putTo
//
void VVirtualListBox::putTo(VArchiver& a, boolean most_derived)
{
	//	base object data

	VView::putTo(a, FALSE); 

	a << VVirtualListBoxArchiveTag;

	a << currRowNo;
	a << long(minRowHeight);
	a << long(rowIndent);

	//	name of fetch callback and the class to which it belongs
	const char *method_name	= 0;			
	const char *class_name 	= 0;			

	VCallbackList::findCallback(fetchMthd, class_name, method_name, fetchMthdIndex);

	a << (char *) class_name; 
	a << (char *) method_name;

	//	name of select callback and the class to which it belongs

	method_name	= 0;			
	class_name 	= 0;			

	VCallbackList::findCallback(selectMthd, class_name, method_name, selectMthdIndex);

	a << (char *) class_name; 
	a << (char *) method_name;

	//	name of double click callback and the class to which it belongs

	method_name	= 0;			
	class_name 	= 0;			

	VCallbackList::findCallback(dblClkMthd, class_name, method_name, dblClkMthdIndex);

	a << (char *) class_name; 
	a << (char *) method_name;

	//	return list or index?

	a << 0L;		// place holder //

	//	children

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// --------------------------------------------------------------------------
// VVirtualListBox::getFrom
//
void VVirtualListBox::getFrom(VArchiver& a)
{
	//	base object data
	VView::getFrom(a);

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VVirtualListBoxArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VVirtualListBoxArchiveTag));
		return;
	}

	a >> currRowNo;
	a >> minRowHeight;
	a >> rowIndent;

	//	fetch method
	char	method_name[256];
	char	class_name[256];
	char   *method_name_ptr = method_name;
	char   *class_name_ptr 	= class_name;

	a >> class_name_ptr; 	
	a >> method_name_ptr;	

	method fm = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
															&fetchMthdIndex);

	if (client != 0 && fm != 0) {
		if (!client->isA(VClass::of(class_name_ptr))) {
			a.abort(VArchiver::ClientCallbackMismatch);
			return;
		}
	}

	//	select method
	a >> class_name_ptr; 	
	a >> method_name_ptr;	

	method sm = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
															&selectMthdIndex);

	if (client != 0 && sm != 0) {
		if (!client->isA(VClass::of(class_name_ptr))) {
			a.abort(VArchiver::ClientCallbackMismatch);
			return;
		}
	}

	//	double click method

	a >> class_name_ptr; 	
	a >> method_name_ptr;	

	method dcm = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
															&dblClkMthdIndex);

	if (client != 0 && dcm != 0) {
		if (!client->isA(VClass::of(class_name_ptr))) {
			a.abort(VArchiver::ClientCallbackMismatch);
			return;
		}
	}

	//	return list or index?

	boolean b;
	a >> b;			// ignore!!!

	uponEvent(client, fm, sm, dcm);
}

// --------------------------------------------------------------------------
// VVirtualListBox::getFrom
//
void VVirtualListBox::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VVirtualListBox::getFrom(a);

	if (do_realize) {
		VVirtualListBox::realize((VWindow *)data);
	}

	if (data != 0) {
		VWindow::getChildrenFrom(a);
	}
}

// --------------------------------------------------------------------------
// VVirtualListBox::realize
//
void VVirtualListBox::realize(VWindow *pwin)
{
	VView::realize(pwin);

	initGUI(minRowHeight);
}

#endif // CV_NOARCHIVER	
