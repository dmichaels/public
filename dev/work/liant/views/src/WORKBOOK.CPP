// $Header:   Q:/views/common/vcs/workbook.cpv   1.22   May 30 1997 17:17:24   scottt  $ 

//	workbook.cpp
//
//	VWorkBook implementation [Common]
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
//	06/23/95 dgm	Original check-in; original code from kerstinb.
//	08/21/95 dgm	Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//	08/22/95 dgm	Removed unnecessary mouseDbl() implementation.
//	09/20/95 dss	VStyle changes.
//	12/08/95 dgm	Removed winFont references for Motif;
//					we need to clean this up!
//  08/26/96 dgm    Hacked in color-scheme.
//	09/09/96 dgm	Implemented CTRL-PgDn and CTRL-PgUp.
//	09/17/96 dgm	Implemented insert-page capabilities to addPage().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "brush.h"
#include "dialog.h"
#include "location.h"
#include "workbook.h"
#include "workpage.h"
#include "port.h"
#include "clrscm.h"
#include "iterator.h"
#include "pointrry.h"
#include "region.h"
#include "polygon.h"
#include "shade.h"
#include "notifier.h"
#include "rscarch.h"

#ifndef CV_NOARCHIVER
#include "cllbckls.h"
#endif

defineClass(VWorkBook, VControl)
defineArchiveRevision(VWorkBook, 0)

#define SCROLL_SIZE 32
#define PAD_SIZE 16

// --------------------------------------------------------------------------
VWorkBook::VWorkBook()
{
    init();
}

// --------------------------------------------------------------------------
VWorkBook::VWorkBook(const VFrame& f, VWindow *parentwin, const VStyle& style)
{
    init();
    (*this)(f, parentwin, style);
    recalcSizes();
}

// --------------------------------------------------------------------------
VWorkBook::VWorkBook(const VString& name, const VFrame& f, VWindow *parentwin, 
							const VStyle& style)
{
    init();
    (*this)(f, parentwin, style, name);
    recalcSizes();
}

// --------------------------------------------------------------------------
VWorkBook::~VWorkBook()
{
    delete m_pPort;

    thePages.freeContents();
}

// --------------------------------------------------------------------------
void VWorkBook::init()
{
    userTabWidth = 0;

    tabHeight = -1;         // force calculation in recalcSizes

    tabRows = 1;

    currPageNo = -1;
    currPageWin = 0;
    currPageOffset = 0;
    m_pPort = new VPort(this);

    activeMthd = NIL_METHOD;

	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));

#ifndef CV_NOARCHIVER
    activeMthdIndex = -1L;
#endif

	refreshFlag = TRUE;
}

// ---------------------------------------------------------------------------
void VWorkBook::getInfo(VWinInfo& nbinfo) 
{
    VControl::getInfo(nbinfo);
}

// ---------------------------------------------------------------------------
// Arrange for method 'mthd' of the client object 'clnt' of this
// VWorkBook to be called when a new page is activated
//
void VWorkBook::uponPageActive(VObject *clnt, method mthd) 
{ 
    if (clnt != 0) {
        client = clnt; 
    }
    if (mthd != NIL_METHOD) {
        activeMthd = mthd; 
    }
    if (clnt == 0 && mthd == NIL_METHOD) {
        client = 0;
        activeMthd = NIL_METHOD;
    }
}

// --------------------------------------------------------------------------
void VWorkBook::setFont(VFont *f)
{

#if defined(CV_MOTIF)
    VControl::setFont(f);
#else
	winFont= f;
#endif
    if (recalcSizes() && refreshFlag) {
        update();
    }
}

// --------------------------------------------------------------------------
void VWorkBook::setTabDims(int width, int height)
{
    userTabWidth = width;

    if (recalcSizes()) {
        updateTabs();
    }
}

// --------------------------------------------------------------------------
// Copy the caption for page 'pgno' into VString 'result'
//
void VWorkBook::getPageCaption(long pgno, VString& result) const
{
    VWorkPage       *page = getPage(pgno);
    if (page != 0) {
        result = page->theCaption;
    }
}

// --------------------------------------------------------------------------
// Enable/disable the specified page
//
void VWorkBook::enablePage(long pgno, boolean tf)
{
    VWorkPage       *page = getPage(pgno);
    if (page != 0) {
        page->isEnabled = tf;
        updateTabs();
    }
}

#if !defined(CV_NOARCHIVER)
// --------------------------------------------------------------------------
// Add a page (to be loaded from .vrf file)
// returns the page number
//
long VWorkBook::addPage(const VString& caption, 
                        VRscArchiver& rf, const VString& rname,
                        PagePos pagepos, long pgno)
{
    int insertedPage = -1;
    // create window;
    VWindow *win = loadPageWin(rf, rname);

    // create new page and add to note book
    VWorkPage       *page = new VWorkPage(caption, rname, win, TRUE);
	if ((pgno >= 0) && (pgno + 1 <= getPageCount())) {
	  thePages.insertAt(pgno , page);
	  insertedPage = pgno;
	}
	else {
      thePages.add(page);
	  insertedPage = -1;
	}

    recalcSizes(TRUE);

    if (getPageCount() <= 0) {
        currPageNo = -1;
        currPageWin = 0;
        currPageOffset = 0;
    }
    else {
        if (currPageNo < 0) {
            gotoPage(0);
        }
        else if (win != 0) {
            win->hide();
        }
    }

	if (insertedPage == -1)
	  insertedPage = getPageCount() - 1;

	return insertedPage;
}
#endif

// --------------------------------------------------------------------------
// Add a page to the notebook.
// Assumes the Window was created using *this* VWorkBook as a 
// a parent window
// returns the page number
//
long VWorkBook::addPage(const VString& caption, VWindow *win,
                        PagePos pagepos, long pgno)
{
    int insertedPage = -1;

    // create new page and add to note book
    VWorkPage       *page = new VWorkPage(caption, "", win, FALSE);
	if ((pgno >= 0) && (pgno + 1 <= getPageCount())) {
	  thePages.insertAt(pgno , page);
	  insertedPage = pgno;
	}
	else {
      thePages.add(page);
	  insertedPage = -1;
	}

    recalcSizes(TRUE);

    if (getPageCount() <= 0) {
        currPageNo = -1;
        currPageWin = 0;
        currPageOffset = 0;
    }
    else {
        if (currPageNo < 0) {
            gotoPage(0);
        }
        else if (win != 0) {
            win->hide();
        }
    }

	if (insertedPage == -1)
	  insertedPage = getPageCount() - 1;

	return insertedPage;
}

// --------------------------------------------------------------------------
void VWorkBook::removePage(long pgno)
{
    VWorkPage *page = (VWorkPage *) thePages.removeAt(pgno);

    if (page != 0) {
        delete page->theWindow;
        page->theWindow = 0;

        delete page;
    }

    if (currPageNo >= thePages.count()) {
        currPageNo = thePages.count() - 1;
    }
    if (currPageNo < 0) {
        currPageWin = 0;
    }
    else {
        page = getPage(currPageNo);
        if (page != 0) {
            currPageWin = page->theWindow;
        }
        else {
            currPageWin = 0;
        }
    }

	if (refreshFlag)
	    update();
}

// --------------------------------------------------------------------------
void VWorkBook::removeAllPages()
{
    // delete windows
    DO (thePages, VWorkPage, page)
        if (page != 0) {
            delete page->theWindow;
            page->theWindow = 0;
        }
    END

    thePages.freeContents();
    currPageWin = 0;
    currPageNo = -1;
    currPageOffset = 0;
    currPageOffset = 0;

	if (refreshFlag)
    	update();
}

// --------------------------------------------------------------------------
long VWorkBook::findPage(const VString& caption)
{
	long pgNo = -1;

	DO (thePages, VWorkPage, page)
		if (page != 0) {
			pgNo++;
			if (page->theCaption == caption) {
				return pgNo;
			}
		}
	END

	return -1;
}

// --------------------------------------------------------------------------
boolean VWorkBook::paint()
{
    int             ww, wh;
    sizeOfImage(&ww, &wh);

    m_pPort->open();

	m_pPort->usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
    m_pPort->useFont(getFont());

    // draw frame around page
#if 0
    m_pPort->moveTo(5, wh - 5 - tabHeight);
    m_pPort->lineTo(5, 5);
    m_pPort->lineTo(ww - 3, 5);
    m_pPort->lineTo(ww - 3, wh - 5 - tabHeight);
#endif

    drawTabs();
    drawButtons();

    m_pPort->close();

    return TRUE;
}

// --------------------------------------------------------------------------
void VWorkBook::drawTabs()
{
    VPointArray regionDef;
    long            pgno;
    int             tx;
    int             cnt = getPageCount();
    int             sideOffset = tabHeight/2;
    int             ww, wh, w;

    sizeOfImage(&ww, &wh);
    int startTop, endTop;

	m_pPort->usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
    tx = 3 + SCROLL_SIZE;
    for (pgno = currPageOffset; pgno < cnt; pgno++) {
        VWorkPage       *page = getPage(pgno);

        if (tx >= ww) {
            break;
        }

        w = page->width;

        regionDef.reset();
        if (pgno == currPageOffset || pgno == currPageNo) {
            m_pPort->moveTo(tx,wh - 5 - tabHeight);
            regionDef.add(tx,wh - 5 - tabHeight);
            startTop = tx;
        }
        else {
            m_pPort->moveTo(tx + sideOffset/2, wh - 5 - tabHeight /2);
            startTop = tx+sideOffset - 1;
        }

        m_pPort->lineTo(tx+sideOffset,wh - 5);
        regionDef.add(tx+sideOffset,wh - 5);
        m_pPort->lineTo(tx+w-sideOffset,wh - 5);
        regionDef.add(tx+w-sideOffset,wh - 5);
        if (pgno+1 == currPageNo) {
            m_pPort->lineTo(tx+w-sideOffset/2,wh - 5 -(tabHeight/2));
            endTop = tx+w-2;
        }
        else {
            m_pPort->lineTo(tx+w,wh - 5 - tabHeight);
            regionDef.add(tx+w,wh - 5 - tabHeight);
            endTop = tx+w;
        }


        if (pgno != currPageNo) {
            m_pPort->moveTo(endTop,wh - 5 - tabHeight);
            m_pPort->lineTo(startTop,wh - 5 - tabHeight);
        }
        if (pgno == currPageNo) {
            regionDef.add(startTop+1,wh - 6 - tabHeight);
            VPolygon* regionBorder = new VPolygon(&regionDef);
			m_pPort->useBrush(&VColorScheme::
							   systemBrush(VColorScheme::FlatBackground));
            m_pPort->fillRegion(regionBorder,FALSE);
// redraw the two side lines
            m_pPort->moveTo(tx,wh - 5 - tabHeight);
            m_pPort->lineTo(tx+sideOffset,wh - 5);
            m_pPort->moveTo(tx+w-sideOffset,wh - 5);
            m_pPort->lineTo(tx+w,wh - 5 - tabHeight);
            delete regionBorder;
        }
		m_pPort->usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));
        m_pPort->moveTo(tx + sideOffset, wh - 6);
        m_pPort->lineTo(tx + w - sideOffset, wh - 6);
        if (pgno != currPageNo) {
            if (pgno+1 != currPageNo)
                m_pPort->moveTo(tx + w - sideOffset - 1, wh - 6);
            m_pPort->lineTo(tx + w - 1, wh - 5 - tabHeight);
        }
        if (pgno == currPageNo) {
			m_pPort->usePen(&VColorScheme::
							 systemPen(VColorScheme::FlatForeground));
		}
		else {
			m_pPort->usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedForeground));
		}

        m_pPort->wrtText(page->theCaption.gets(),tx+sideOffset + PAD_SIZE/2,wh - tabHeight - 4);
        tx += w-sideOffset;
    }
    // fill gap at right
	m_pPort->usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
    if (cnt > 0) {
        m_pPort->moveTo(tx+sideOffset, wh - 5 - tabHeight);
        m_pPort->lineTo(ww - 3, wh - 5 - tabHeight);
    }
}

// --------------------------------------------------------------------------
void VWorkBook::drawButtons(buttonState left, buttonState right)
{
    boolean fRightDisabled;
    boolean fLeftDisabled;

    VShade *pShade = new VShade;
	pShade->setColors
		(VColorScheme::systemColor(VColorScheme::ShadedHighlight),
		 VColorScheme::systemColor(VColorScheme::ShadedShadow),
		 VColorScheme::systemColor(VColorScheme::ShadedBackground));

    VPointArray leftBShape;
    VPointArray rightBShape;
    int     ww, wh;
    sizeOfImage(&ww, &wh);

    if (getPageCount() == 0)
        return;             // don't draw buttons if there are no tabs

   // create scroll buttons
    leftBShape.add(11, wh - tabHeight + 4);
    leftBShape.add(15, wh - tabHeight + 8);
    leftBShape.add(15, wh - tabHeight);
    leftBShape.add(11, wh - tabHeight + 4);
    rightBShape.add(7,wh - tabHeight);
    rightBShape.add(7,wh - tabHeight + 8);
    rightBShape.add(11,wh - tabHeight + 4);
    rightBShape.add(7,wh - tabHeight);
    VPolygon* regionBorder = new VPolygon(&leftBShape);  

//    m_pPort->open();

    // validate buttons
    if (currPageOffset > 0)
        fLeftDisabled = FALSE;
    else {
        fLeftDisabled = TRUE;
        left = up;
    }
    if (currPageOffset >= (getPageCount() - 1)) {
        fRightDisabled = TRUE;
        right = up;
    }
    else
        fRightDisabled = FALSE;
    VRectangle buttonRect(CornerDim,3,wh - 4 - tabHeight,SCROLL_SIZE/2,SCROLL_SIZE/2);
    if (left == down) {
        pShade->panel(Indented,buttonRect,1,*m_pPort);
//        pShade->panel(Pressed,buttonRect,1,*m_pPort);
        regionBorder->move(1,1);
    }
    else {
        pShade->panel(Raised,buttonRect,1,*m_pPort);
    }
    if (fLeftDisabled) {
		m_pPort->useBrush(&VColorScheme::
						   systemBrush(VColorScheme::ShadedShadow));
    }
    else {
		m_pPort->useBrush(&VColorScheme::
						   systemBrush(VColorScheme::ShadedForeground));
    }
    m_pPort->fillRegion(regionBorder,FALSE);
    regionBorder->set(&rightBShape);
    buttonRect.move(SCROLL_SIZE/2,0);
    if (right == down) {
         pShade->panel(Indented,buttonRect,1,*m_pPort);
//         pShade->panel(Pressed,buttonRect,1,*m_pPort);
        regionBorder->move(1+SCROLL_SIZE/2,1);
    }
    else {
        pShade->panel(Raised,buttonRect,1,*m_pPort);
        regionBorder->move(SCROLL_SIZE/2,0);
    }
    if (fRightDisabled) {
		m_pPort->useBrush(&VColorScheme::
						   systemBrush(VColorScheme::ShadedShadow));
    }
    else {
		m_pPort->useBrush(&VColorScheme::
						   systemBrush(VColorScheme::ShadedForeground));
    }
    m_pPort->fillRegion(regionBorder,FALSE);
    delete regionBorder;
    delete pShade;
//    m_pPort->close();
}

// --------------------------------------------------------------------------
boolean VWorkBook::erased() 
{
    return VWindow::erased();
}

// --------------------------------------------------------------------------
boolean VWorkBook::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}

    int     ww, wh;
    int     i;
    sizeOfImage(&ww, &wh);

    if (my > (wh - tabHeight - 4) && mx > 3) {
        if (mx > 3 + SCROLL_SIZE) { // tab button
		    int sideOffset = tabHeight/2;
    		int start = SCROLL_SIZE + 3;
			int count = getPageCount();
            for (i = currPageOffset; i < count; i++) {
                VWorkPage* page = getPage(i);
                start += page->width - sideOffset;

				if (i == currPageOffset)
					start += 3*sideOffset/4;

                if (mx < start) {
                    gotoPage(i);
                    break;
                }
            }
        }
        else if (my < (wh - tabHeight - 4 + SCROLL_SIZE/2)) {
            notifier->captureMouseFor(this);
            if (mx > 3 + SCROLL_SIZE / 2) {  // right button
                if (currPageOffset < getPageCount() - 1) {
                    currPageOffset++;
                    updateTabs();
//                    drawTabs();
                    m_pPort->open();
                    drawButtons(up, down);
                    m_pPort->close();
                }
            }
            else { // left button
                if (currPageOffset > 0) {
                    currPageOffset--;
                    updateTabs();
//                    drawTabs();
                    m_pPort->open();
                    drawButtons(down, up);
                    m_pPort->close();
                }
            }
        }
    }

    return TRUE;
}

// --------------------------------------------------------------------------
boolean VWorkBook::mouseUp(int x, int y)
{   
	if (!isEnabled()) {
		return FALSE;
	}
//    if (m_inTracking) {
        m_pPort->open();
        drawButtons(up,up);
        m_pPort->close();
        notifier->releaseCapture();
//    }
    return TRUE;
}

// --------------------------------------------------------------------------
// go to the specified page number, return FALSE if page number is
// invalid, page is disabled, or already showing the page.
//
boolean VWorkBook::gotoPage(long pgno)
{
    int     cnt = getPageCount();
    long     oldpn = currPageNo;

    if (cnt <= 0) {
        currPageNo = -1;
        currPageOffset = 0;
        return FALSE;
    }

    if (pgno <= 0) {
        currPageNo = 0;
    }
    else if (pgno >= cnt - 1) {
        currPageNo = cnt - 1;
    }
    else {
        currPageNo = pgno;
    }

    if (currPageNo == oldpn) {
        return FALSE;
    }

    VWorkPage       *page = getPage(currPageNo);
    if (page == 0 || !page->isEnabled) {
        currPageNo = oldpn;
        return FALSE;
    }

    updateTabs();

    // update page
    VWindow         *old = currPageWin;
    currPageWin = page->theWindow;

    // invoke callback
    if (client && activeMthd != NIL_METHOD) {
        client->perform(activeMthd, (long)this);
    }

    if (currPageWin != 0) {
        currPageWin->show();
    }
    if (old != 0) {
        old->hide();
    }

    return TRUE;
}

// --------------------------------------------------------------------------
// Force repaint of tabs
//
void VWorkBook::updateTabs()
{
    if (refreshFlag && tabHeight > 0) {
//      m_pPort->open();
//      drawTabs();
//      m_pPort->close();

        int             ww, wh;
        sizeOfImage(&ww, &wh);
//        VRectangle tab_area(CornerDim,3 + SCROLL_SIZE,wh - 6 - tabHeight,ww,tabHeight + 6);
        VRectangle tab_area(CornerDim,3,wh - 6 - tabHeight,ww,tabHeight + 6);
        update(&tab_area);
    }
}

#if !defined(CV_NOARCHIVER)
// --------------------------------------------------------------------------
// private: load a window from the vrf file
//
VWindow *VWorkBook::loadPageWin(VRscArchiver& rf, const VString& rname)
{
    VWindow *win = 0;
    VDialog  dlg;
    VDialog *dp = 0;        // points to dlg if successful

    boolean isopen = rf.isOpen();

    if (!isopen) {
        if (!rf.open(VRscArchiver::ReadOnly)) {
            return 0;
        }
    }

    // load as dialog, don't realize
    dp = (VDialog *) rf.getObject(rname, &dlg, VDialogCls, this, FALSE);

    if (dp != 0) {
        // load any additional resource (such as bitmaps or custom controls)
        dp->loadResources(rf);

        // create a new window on the notebook
        win = new VWindow(pageFrame, this, StyleHidden);
        if (win != 0) {
            win->setBackground(getBackground());
            dp->realizeChildren(win);
            win->frameChildren();
        }
    }

    if (!isopen) {
        rf.close();
    }

    return win;
}
#endif

// --------------------------------------------------------------------------
// Private: computes the size of the page frame and the tab sizes.
// returns TRUE if something changed
//
boolean VWorkBook::recalcSizes(boolean force)
{
    boolean changed = FALSE;

    int             ww, wh;
    sizeOfImage(&ww, &wh);

    if (getPageCount() > 0) {
        int             fw, fh;
        fontSize(&fw, &fh);

//        tabHeight = h + 6;
        if (tabHeight != fh + 2) {
            tabHeight = fh + 2;
            changed = TRUE;
        }

        m_pPort->open();
	    m_pPort->useFont(getFont());

        int w, h;

        DO (thePages, VWorkPage, page)
            if (page) {
                m_pPort->textSize((char *)page->theCaption.gets(), &w, &h);

                if (userTabWidth <= 0) {
                    // auto size
                    w += tabHeight + PAD_SIZE;
                }
                else {
                    w = userTabWidth;
                }
        
                if (page->width != w) {
                    page->width = w;
                    changed = TRUE;
                }
            }
        END

        m_pPort->close();
    }
    else {
        if (tabHeight != 0) {
            tabHeight = 0;
            changed = TRUE;
        }
    }

    if (!force && !changed) {
        // no change
        return FALSE;
    }

    pageFrame.set(6, 7, ww - 10, wh - 12 - tabHeight);
//    pageFrame.set(6, tabHeight + 7, ww - 10, wh - 12 - tabHeight);

    // resize pages
    DO (thePages, VWorkPage, page)
        if (page != 0) {
            VWindow *win = page->theWindow;
            if (win != 0) {
                win->move(pageFrame);
            }
        }
    END

    return TRUE;
}

// --------------------------------------------------------------------------
boolean VWorkBook::resized(int w, int h)
{
    recalcSizes(TRUE);
    return TRUE;
}

// ---------------------------------------------------------------------------
// If 'b' is FALSE, then disable the refresh of this VListBox during
// list item update, for example, adding strings, otherwise enable display
// refresh.  If refresh is enabled, the display of list items will flicker
// with a large number of updates.
//
void VWorkBook::refresh(boolean b)
{
	refreshFlag = b;
	if (b) {
		updateTabs();
	}
}

// ---------------------------------------------------------------------------
//
boolean VWorkBook::key(int virtual_key, char ascii_key)
{
	switch (virtual_key) {

	case K_Mod_Control | K_Next:				// CTRL-PgDn
		if (currPageNo < getPageCount() - 1) {
			gotoPage(currPageNo + 1);
		}
		else {
			gotoPage(0);
		}
		break;

	case K_Mod_Control | K_Prior:				// CTRL-PgUp
		if (currPageNo > 0) {
			gotoPage(currPageNo - 1);
		}
		else {
			gotoPage(getPageCount());
		}
		break;
	}

    return VControl::key(virtual_key, ascii_key);
}

// ===========================================================================
#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
void VWorkBook::putTo(VArchiver& a)
{
    VWorkBook::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//      Archive this VWorkBook object and, if most derived, its children.
//
//      Type:           What:
//                                              VControl base class object data
//              char *          name of activatePage callback method's client's class
//              char *          name of activatePage callback method    
//
void VWorkBook::putTo(VArchiver& a, boolean most_derived)
{
    VControl::putTo(a, FALSE);                              //      base object data

    a << VWorkBookArchiveTag;

    const char *active_class_name   = 0;                    
    const char *active_method_name  = 0;                    

    VCallbackList::findCallback(activeMthd, active_class_name, active_method_name, 
                                                            activeMthdIndex);

    a << (char *) active_class_name;                //      active method's class name
    a << (char *) active_method_name;               //      active method name

    // collection of pages
    a.putObject(&thePages);

//      if (most_derived) {
//              VWindow::putChildrenTo(a);                      //      children
//      }
}

// --------------------------------------------------------------------------
void VWorkBook::getFrom(VArchiver& a)
{
    VControl::getFrom(a);                                           //      base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VWorkBookArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VWorkBookArchiveTag));
        return;
    }

    char            active_method_name[256];
    char *          active_method_name_ptr = active_method_name;
    char            active_class_name[256];
    char *          active_class_name_ptr = active_class_name;

    a >> active_class_name_ptr;                     //      active method's class name
    a >> active_method_name_ptr;            //      active method name

    method m = VCallbackList::findCallback(active_class_name_ptr, 
                                    active_method_name_ptr, &activeMthdIndex);

    if (client != 0 && m != 0) {
        if (!client->isA(VClass::of(active_class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    uponPageActive(client, m);

    a.getObject(&thePages);

    // gets reset in realize
    currPageNo = -1;
    currPageWin = 0;
    currPageOffset = 0;
}

// --------------------------------------------------------------------------
void VWorkBook::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VWorkBook::getFrom(a);

    if (do_realize) {
        VWorkBook::realize((VWindow *)parent);
    }

//      if (parent != 0) {
//              VWindow::getChildrenFrom(a, do_realize);
//      }
}

// --------------------------------------------------------------------------
void VWorkBook::realize(VWindow *parent)
{
    VControl::realize(parent);
}

// --------------------------------------------------------------------------
// Load any resources needed by this window or its children
// (i.e. the pages)
//
boolean VWorkBook::loadResources(VRscArchiver& a)
{
    boolean isopen = a.isOpen();

    if (!isopen) {
        if (!a.open(VRscArchiver::ReadOnly)) {
            return FALSE;
        }
    }

    // loop through pages, load 'em if they need it
    DO (thePages, VWorkPage, page)
        if (page != 0 && page->isVrfWindow && page->theWindow == 0) {
            page->theWindow = loadPageWin(a, page->theWindowName);
            if (page->theWindow != 0) {
                page->theWindow->frameChildren();
                page->theWindow->hide();
            }
        }
    END

    if (!isopen) {
        a.close();
    }

    gotoPage(0);
    return TRUE;
}

#endif // CV_NOARCHIVER
