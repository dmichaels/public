// $Header:   Q:/views/common/vcs/notebook.cpv   1.57   May 30 1997 17:15:56   scottt  $ 

//  notebook.cpp
//
//  VNoteBook implementation [Common]
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  06/23/95 dgm    Original check-in; original code from jmd.
//  07/11/95 dgm    Commented out #include "isrscmgr.h".
//  08/15/95 krb    Added variable size tabs.
//  08/21/95 dgm    Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//  08/22/95 dgm    Removed unnecessary mouseDbl() implementation.
//  08/29/95 dgm    For krb; added setPageCaption().
//  09/15/95 dss    VStyle changes.
//  10/11/95 evc    Fixed bad setFont 
//  12/08/95 dgm    Removed winFont references for Motif;
//                  we need to clean this up!
//  12/22/95 tlf    Portable font changes.
//  08/26/96 dgm    Hacked in color-scheme.
//  09/09/96 dgm    Fixed LEFT/RIGHT arrows to wrap around.
//					Fixed TAB to go to first child of current notepage.
//					Added en/disableTabFocus()/isEn/DisabledTabFocus().
//  09/18/96 dgm    More focus fiddling.
//  01/14/97 djs    Removed #ifdef WINDOWS for focus updates on OS2
//  01/27/97 tlf    Fixed change of 1/14/97 to work for Motif.
//  01/27/97 tlf    Fixed change of 1/14/97 to work for Motif.
//  07/31/97 dgm    Fixed resized() to call frameChildren() for each page,
//					and to invalidate the geometry.  SCR: 5754, 5872.
// --------------------------------------------------------------------------

#include "defs.h"
#include "brush.h"
#include "dialog.h"
#include "location.h"
#include "notebook.h"
#include "notepage.h"
#include "port.h"
#include "clrscm.h"
#include "iterator.h"
#include "rscarch.h"
#include "notifier.h"
#if 0
#include "isrscmgr.h"
#endif

#ifndef CV_NOARCHIVER
#include "cllbckls.h"
#endif

#include "geomgr.h"
#include "geowin.h"

defineClass(VNoteBook, VControl)
defineArchiveRevision(VNoteBook, 0)

// --------------------------------------------------------------------------
VNoteBook::VNoteBook()
{
    init();
}

// --------------------------------------------------------------------------
VNoteBook::VNoteBook(const VFrame& f, VWindow *parentwin, const VStyle& style)
{
    init();
    (*this)(f, parentwin, style);
    recalcSizes();
}

// --------------------------------------------------------------------------
VNoteBook::VNoteBook(const VString& name, const VFrame& f, VWindow *parentwin, const VStyle& style)
{
    init();
    (*this)(f, parentwin, style, name);
    recalcSizes();
}

// --------------------------------------------------------------------------
VNoteBook::~VNoteBook()
{
    thePages.freeContents();
}

// --------------------------------------------------------------------------
void VNoteBook::init()
{
	setGeoManager(new VGeoWindow());
    nativeMode = 0;

    userTabWidth = 0;

    tabWidth = -1;          // force calculation in recalcSizes
    tabHeight = -1;

    tabRows = 1;

    // these are ignored with this version
    tabType = SingleRowTabs;
    tabBinding = SolidBinding;
    tabPlacement = TabTop;

    currPageNo = -1;
    currPageWin = 0;

    activeMthd = NIL_METHOD;

	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));

#ifndef CV_NOARCHIVER
    activeMthdIndex = -1L;
#endif

    refreshFlag = TRUE;

	tabFocusEnabled = 1;
	tabHasFocus = 0;
	handlingMouseDn = 0;
	handlingMouseDnInTab = 0;
}

// ---------------------------------------------------------------------------
void VNoteBook::getInfo(VWinInfo& nbinfo) 
{

#if defined(MS_WINDOWS) | defined(MOTIF)
    nativeMode = 0;
#else
    nativeMode  = style.contains(StyleNative);
#endif

    VControl::getInfo(nbinfo);
}

// ---------------------------------------------------------------------------
// Arrange for method 'mthd' of the client object 'clnt' of this
// VNoteBook to be called when a new page is activated
//
void VNoteBook::uponPageActive(VObject *clnt, method mthd) 
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
void VNoteBook::setFont(VFont *f)
{
    winFont = f;

    if (recalcSizes() && refreshFlag) {
        update();
    }
}

// --------------------------------------------------------------------------
void VNoteBook::setTabStyles(BindingType binding, TabPlacement tabplace)
{
    // ignored under Windows
    tabBinding = binding;
    tabPlacement = tabplace;
}

// --------------------------------------------------------------------------
void VNoteBook::setTabDims(int width, int height, TabType type)
{
    // ignored under Windows
    tabType = type;

    userTabWidth = width;

    if (recalcSizes()) {
        updateTabs();
    }
}

// --------------------------------------------------------------------------
// Copy the caption for page 'pgno' into VString 'result'
//
void VNoteBook::getPageCaption(long pgno, VString& result) const
{
    VNotePage       *page = getPage(pgno);
    if (page != 0) {
        result = page->theCaption;
    }
}

// --------------------------------------------------------------------------
//
void VNoteBook::setPageCaption(long pgno, const VString& result)
{
    VNotePage       *page = getPage(pgno);
    if (page != 0) {
        page->theCaption = result;
        if (recalcSizes()) {
            updateTabs();
        }
    }
}

// --------------------------------------------------------------------------
// Enable/disable the specified page
//
void VNoteBook::enablePage(long pgno, boolean tf)
{
    VNotePage       *page = getPage(pgno);
    if (page != 0) {
        page->isEnabled = tf;
        updateTabs();
    }
}

// --------------------------------------------------------------------------
//
void VNoteBook::enableTabFocus(boolean b)
{
	if (b) {
		if (tabFocusEnabled) {
			return;
		}
		tabFocusEnabled = 1;
	}
	else {
		if (!tabFocusEnabled) {
			return;
		}
		tabFocusEnabled = 0;
	}
}

#if !defined(CV_NOARCHIVER)
// --------------------------------------------------------------------------
// Add a page (to be loaded from .vrf file)
// returns the page number
//
long VNoteBook::addPage(const VString& caption, 
                        VRscArchiver& rf, const VString& rname,
                        PageType pgtype, PagePos pagepos ,
                        long pgno)
{
    int insertedPage = -1;
    // create window;
    VWindow *win = loadPageWin(rf, rname);

    // create new page and add to note book
    VNotePage       *page = new VNotePage(caption, rname, win, TRUE);
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
// Assumes the Window was created using *this* VNoteBook as a 
// a parent window
// returns the page number
//
long VNoteBook::addPage(const VString& caption, VWindow *win,
                        PageType pgtype, PagePos pagepos,
                        long pgno)
{
    int insertedPage = -1;

    // create new page and add to note book
    VNotePage       *page = new VNotePage(caption, "", win, FALSE);
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
void VNoteBook::removePage(long pgno)
{
    VNotePage *page = (VNotePage *) thePages.removeAt(pgno);

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
void VNoteBook::removeAllPages()
{
    // delete windows
    DO (thePages, VNotePage, page)
        if (page != 0) {
            delete page->theWindow;
            page->theWindow = 0;
        }
    END

    thePages.freeContents();
    currPageWin = 0;
    currPageNo = -1;

    if (refreshFlag)
        update();
}

// --------------------------------------------------------------------------
long VNoteBook::findPage(const VString& caption)
{
    long pgNo = -1;

    DO (thePages, VNotePage, page)
        if (page != 0) {
            pgNo++;
            if (page->theCaption == caption) {
                return pgNo;
            }
        }
    END

    return -1;
}

// #if defined(CV_WINDOWS)
#if 1

// --------------------------------------------------------------------------
//
VWindow *VNoteBook::getFocusCandidate(int mode) const
{
	if (tabFocusEnabled) {
		if (tabHasFocus || handlingMouseDnInTab ||
			(((VWindow *)this)->getChildFocus() == 0) ||
			(currPageWin == 0) || (currPageWin->getChildFocus() == 0)) {
			return (VWindow *)this;
		}
		else if (mode > 0) {
			((VNoteBook *)this)->tabHasFocus = 1;
			return (VWindow *)this;
		}
	}
	return VControl::getFocusCandidate(mode);
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::takeFocus()
{
	return VControl::takeFocus();
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::clearFocus()
{
	tabHasFocus = 0;
	updateTabs();
	return VControl::clearFocus();
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::givenFocus()
{
	if ((getParent() != 0) && !isTopLevel()) {
		getParent()->setChildFocus(this);
	}
	setChildFocus(0);
	tabHasFocus = 1;
	updateTabs();
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean	VNoteBook::nextChildFocus(VWindow *child)
{
	return VControl::nextChildFocus(child);
}

// --------------------------------------------------------------------------
//
boolean	VNoteBook::prevChildFocus(VWindow *child)
{
	return VControl::prevChildFocus(child);
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::controlNextFocus()
{
	if (currPageNo < getPageCount() - 1) {
		gotoPage(currPageNo + 1);
	}
	else {
		gotoPage(0);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::controlPrevFocus()
{
	if (currPageNo > 0) {
		gotoPage(currPageNo - 1);
	}
	else {
		gotoPage(getPageCount());
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
VWindow *VNoteBook::getPrevChildFocusCandidate(VWindow *this_child) const
{
	if (!tabFocusEnabled) {
		return VControl::getPrevChildFocusCandidate(this_child);
	}
	VWindow *focus_candidate = VControl::
							   getPrevChildFocusCandidate(this_child);
	for (VWindow *w = focus_candidate ; (w != 0) && !w->isTopLevel() ;
				  w = w->getParent()) {
		if (w == (VWindow *)this) {
			return focus_candidate;
		}
	}
	if (this_child == 0) {
		return VControl::getPrevChildFocusCandidate(0);
	}
	else {
		((VNoteBook *)this)->tabHasFocus = 1;
		return (VWindow *)this;
	}
}

// --------------------------------------------------------------------------
//
VWindow *VNoteBook::getNextChildFocusCandidate(VWindow *this_child) const
{
	return VControl::getNextChildFocusCandidate(this_child);
}

// #ifdef OLD_FOCUS
#else

// --------------------------------------------------------------------------
//
boolean VNoteBook::takeFocus()
{
#if 0
    if (isHidden() || !isEnabled()) {
        return FALSE;
    }
	if (!tabHasFocus && (currPageWin != 0)) {
		if (getParent() != 0) {
			getParent()->setChildFocus(this);
		}
		return currPageWin->takeFocus();
	}
	else {
		return VControl::takeFocus();
	}
#else
	return VControl::takeFocus();
#endif
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::givenFocus()
{
	if (isHidden()) {
		return FALSE;
	}
	if (tabFocusEnabled && handlingMouseDn) {
		updateTabs();
		tabHasFocus = 1;
	}
	else {
		if (currPageWin != 0) {
			tabHasFocus = 0;
			currPageWin->takeFocus();
		}
		else {
			tabHasFocus = 1;
		}
	}
	return VControl::givenFocus();
}

// --------------------------------------------------------------------------
//
boolean VNoteBook::clearFocus()
{
	if (isHidden()) {
		return FALSE;
	}
	tabHasFocus = 0;
	updateTabs();
	return VControl::clearFocus();
}

#endif

// --------------------------------------------------------------------------
boolean VNoteBook::paint()
{
    VPort   port(this);

    int             ww, wh;
    sizeOfImage(&ww, &wh);

    port.open();

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
    port.useFont(getFont());

    // draw frame around page
    port.moveTo(3, wh - 3);
    port.lineTo(ww - 3, wh - 3);
    port.lineTo(ww - 3, tabHeight + 3);

    port.moveTo(3, wh - 3);
    port.lineTo(3, tabHeight + 3);

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedShadow));

    port.moveTo(4, wh - 4);
    port.lineTo(ww - 4, wh - 4);
    port.lineTo(ww - 4, tabHeight + 4);

    port.moveTo(5, wh - 5);
    port.lineTo(ww - 5, wh - 5);
    port.lineTo(ww - 5, tabHeight + 5);

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));

    port.moveTo(4, wh - 4);
    port.lineTo(4, tabHeight + 4);

    port.moveTo(5, wh - 5);
    port.lineTo(5, tabHeight + 5);

    // draw tabs
    VRectangle      tab;
    long            pgno; 
    int             tx;
    int             cnt = getPageCount();
    int             w;

    tx = 3;
    for (pgno = 0; tx < ww && pgno < cnt; pgno++) {
        VNotePage       *page = getPage(pgno);
        if (page != 0) {
            w = page->width;

            if (pgno == currPageNo) {
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedDarkShadow));
                port.moveTo(tx, tabHeight + 4);
                port.lineTo(tx, 6);
                port.lineTo(tx + 4, 2);
                port.lineTo(tx + w - 4, 2);
                port.lineTo(tx + w, 6);
                port.lineTo(tx + w, tabHeight + 4);

				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedShadow));
                port.moveTo(tx + w - 4, 3);
                port.lineTo(tx + w - 1, 6);
                port.lineTo(tx + w - 1, tabHeight + 6);

                port.moveTo(tx + w - 4, 4);
                port.lineTo(tx + w - 2, 6);
                port.lineTo(tx + w - 2, tabHeight + 7);

				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedHighlight));
                port.moveTo(tx + 1, tabHeight + 6);
                port.lineTo(tx + 1, 6);
                port.lineTo(tx + 4, 3);
                port.lineTo(tx + w - 3, 3);

                port.moveTo(tx + 2, tabHeight + 6);
                port.lineTo(tx + 2, 6);
                port.lineTo(tx + 4, 4);
                port.lineTo(tx + w - 4, 4);
            }
            else {
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedDarkShadow));
                port.moveTo(tx, tabHeight + 4);
                port.lineTo(tx, 6);
                port.lineTo(tx + 4, 2);
                port.lineTo(tx + w - 4, 2);
                port.lineTo(tx + w, 6);
                port.lineTo(tx + w, tabHeight + 4);
                port.lineTo(tx, tabHeight + 4);

				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedHighlight));
                port.moveTo(tx + 1, tabHeight + 3);
                port.lineTo(tx + 1, 6);
                port.lineTo(tx + 4, 3);
                port.lineTo(tx + w - 4, 3);

                if (pgno == 0) {
                    port.moveTo(tx + 1, tabHeight + 5);
                    port.lineTo(tx + w + 1, tabHeight + 5);
                    port.moveTo(tx + 1, tabHeight + 6);
                    port.lineTo(tx + w + 1, tabHeight + 6);
                }
                else {
                    port.moveTo(tx, tabHeight + 5);
                    port.lineTo(tx + w + 1, tabHeight + 5);
                    port.moveTo(tx - 1, tabHeight + 6);
                    port.lineTo(tx + w + 1, tabHeight + 6);
                }

				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedDarkShadow));
                port.moveTo(tx + w - 4, 3);
                port.lineTo(tx + w - 1, 6);
                port.lineTo(tx + w - 1, tabHeight + 4);

            }

            if (page->isEnabled) {
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedForeground));
            }
            else {
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedShadow));
            }

            tab.set(CornerDim, tx + 4, 6, w - 8, tabHeight - 2);
            port.wrtText(page->theCaption.gets(), &tab, JustifyCenter);

			if (tabFocusEnabled && (page->isEnabled) &&
			    ((notifier->getFocus() == this) && (pgno == currPageNo))) {
#if defined(CV_WINDOWS)
				int left, top, right, bottom;
				tab.get(Corners, &left, &top, &right, &bottom);
				RECT r;
				r.left   = left + 1;
				r.top    = top;
				r.right  = right;
				r.bottom = bottom;
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedForeground));
				port.useBrush(&VColorScheme::
							   systemBrush(VColorScheme::ShadedBackground));
				DrawFocusRect(port.getHDC(), &r);
#else
				port.usePen(&VColorScheme::
							 systemPen(VColorScheme::ShadedForeground));
				VColorScheme::systemPen(VColorScheme::ShadedForeground).
					pattern(DotLine);
				port.frameRegion(&tab);
				VColorScheme::systemPen(VColorScheme::ShadedForeground).
					pattern(SolidLine);
#endif
			}

            tx += w;
        }
    }

    // fill gap at right
	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow));
    port.moveTo(tx, tabHeight + 4);
    port.lineTo(ww - 3, tabHeight + 4);

	port.usePen(&VColorScheme::systemPen(VColorScheme::ShadedHighlight));
    if (cnt <= 0) {
        tx++;
    }
    port.moveTo(tx, tabHeight + 5);
    port.lineTo(ww - 3, tabHeight + 5);

    if (cnt <= 0) {
        tx++;
    }
    port.moveTo(tx - 1, tabHeight + 6);
    port.lineTo(ww - 4, tabHeight + 6);

    port.close();

    return TRUE;
}

// --------------------------------------------------------------------------
boolean VNoteBook::erased() 
{
    return VWindow::erased();
}

// --------------------------------------------------------------------------
boolean VNoteBook::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	handlingMouseDn = 1;
    if (my < (tabHeight + 4) && mx > 3) {
        int ww, wh;
        sizeOfImage(&ww, &wh);
    
        long pgno; 
        int cnt = getPageCount();
        int tx = 3;

        for (pgno = 0; tx < ww && pgno < cnt; pgno++) {
            VNotePage *page = getPage(pgno);
            if (page != 0) {
                tx += page->width;

                if (mx < tx) {
					handlingMouseDnInTab = 1;
                    gotoPage(pgno);
					if (tabFocusEnabled && page->isEnabled) {
						takeFocus();
					}
                    break;
                }
            }
        }
    }
	handlingMouseDn = 0;
	handlingMouseDnInTab = 0;
    return TRUE;
}

// --------------------------------------------------------------------------
// go to the specified page number, return FALSE if page number is
// invalid, page is disabled, or already showing the page.
//
boolean VNoteBook::gotoPage(long pgno)
{
    int     cnt = getPageCount();
    long     oldpn = currPageNo;

    if (cnt <= 0) {
        currPageNo = -1;
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

    VNotePage       *page = getPage(currPageNo);
    if (page == 0 || !page->isEnabled) {
        currPageNo = oldpn;
        return FALSE;
    }

    updateTabs();

    // update page

	VWindow *old_currpagewin = currPageWin;

	currPageWin = page->theWindow;

	if (old_currpagewin != 0) {
		old_currpagewin->hide();
	}

    // invoke callback

	if ((client != 0) && (activeMthd != 0)) {
		client->perform(activeMthd, long(this));
	}

	if (currPageWin != 0) {
		currPageWin->bringToTop();
		currPageWin->show();
    }

	if (currPageWin != 0) {
		if (!tabFocusEnabled || !tabHasFocus) {
			currPageWin->takeFocus();
		}
		else {
			takeFocus();
		}
	}

    return TRUE;
}

// --------------------------------------------------------------------------
// Force repaint of tabs
//
void VNoteBook::updateTabs()
{
    if (refreshFlag && tabHeight > 0) {
        int             ww, wh;
        sizeOfImage(&ww, &wh);
        VRectangle      tab_area(CornerDim, 0, 0, ww, tabHeight + 7);
    
        update(&tab_area);
    }
}
//----------------------------------------------------------------------------
//  Called when a key has been pressed on the keyboard.  'Key' is the
//  virtual keycode of the key combination that was pressed and 'ch' is the
//  ASCII code. 
//
boolean VNoteBook::key(int virtual_key, char ascii_key)
{
	switch (virtual_key) {
	case K_Left:
		if (tabFocusEnabled) {
			if (currPageNo > 0) {
				gotoPage(currPageNo - 1);
			}
			else {
				gotoPage(getPageCount());
			}
			return TRUE;
		}
		break;
	case K_Right:
		if (tabFocusEnabled) {
			if (currPageNo < getPageCount() - 1) {
				gotoPage(currPageNo + 1);
			}
			else {
				gotoPage(0);
			}
			return TRUE;
		}
		break;
	case K_End:
		if (tabFocusEnabled) {
			gotoPage(getPageCount());
			return TRUE;
		}
		break;
	case K_Home:
		if (tabFocusEnabled) {
			gotoPage(0);
			return TRUE;
		}
		break;
	case K_Mod_Control | K_Mod_Shift | K_Tab:
		return key(K_Left, ascii_key);
	case K_Mod_Control | K_Tab:
		return key(K_Right, ascii_key);
	case K_Tab:
		if (tabFocusEnabled) {
        	if (currPageWin != 0) {
				currPageWin->nextChildFocus(0);
			}
		}
		return TRUE;
	}

    return VControl::key(virtual_key, ascii_key);
}

#if !defined(CV_NOARCHIVER)
// --------------------------------------------------------------------------
// private: load a window from the vrf file
//
VWindow *VNoteBook::loadPageWin(VRscArchiver& rf, const VString& rname)
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
#endif //CV_NOARCHIVER

// --------------------------------------------------------------------------
// Private: computes the size of the page frame and the tab sizes.
// returns TRUE if something changed
//
boolean VNoteBook::recalcSizes(boolean force)
{
    boolean changed = FALSE;

    int             ww, wh;
    sizeOfImage(&ww, &wh);

    if (getPageCount() > 0) {
        int             fw, fh;
        fontSize(&fw, &fh);

        if (tabHeight != fh + 4) {
            tabHeight = fh + 4;
            changed = TRUE;
        }

        VPort   port(this);
        port.open();
        port.useFont(getFont());

        int w, h;

        DO (thePages, VNotePage, page)
            if (page) {
                if (userTabWidth <= 0) {
                    // auto size
                    port.textSize((char *)page->theCaption.gets(), &w, &h);
                    w += tabHeight;
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

        port.close();
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

    pageFrame.set(6, tabHeight + 7, ww - 10, wh - 12 - tabHeight);

    // resize pages
    DO (thePages, VNotePage, page)
        if (page != 0) {
            VWindow *win = page->theWindow;
            if (win != 0) {
                win->move(pageFrame);
            }
        }
    END

    return TRUE;
}

// ---------------------------------------------------------------------------
//
static void invalidateGeometryOfChildren(VGeoManager *geomanager)
{
	if (geomanager != 0) {
		geomanager->setForceFrameChildren(TRUE);
		VOrdCollect *children = geomanager->getChildren();
		if (children != 0) {
			DO (*children, VGeoFrame, child)
				if (child->isManager()) {
					invalidateGeometryOfChildren((VGeoManager *)child);
				}
				child->update();
			END
		}
	}
}

// ---------------------------------------------------------------------------
//
static void invalidateGeometryFromHereDown(VGeoFrame *geoframe)
{
	if (geoframe != 0) {
		if (geoframe->isManager()) {
			invalidateGeometryOfChildren((VGeoManager *)geoframe);
		}
		geoframe->update();
	}
}

// --------------------------------------------------------------------------
boolean VNoteBook::resized(int w, int h)
{
    return frameChildren(w, h);
}

// --------------------------------------------------------------------------
boolean VNoteBook::frameChildren(int w, int h, boolean test)
{
	if (!test) {
		invalidateGeometryFromHereDown(getGeoFrame());
		recalcSizes(TRUE);
		DO (thePages, VNotePage, page)
			VWindow *window = page->getWindow();
			if (window != 0) {
				invalidateGeometryFromHereDown(window->getGeoFrame());
				window->frameChildren();
			}
		END
	}
    return TRUE;
}

// ---------------------------------------------------------------------------
// If 'b' is FALSE, then disable the refresh of this VListBox during
// list item update, for example, adding strings, otherwise enable display
// refresh.  If refresh is enabled, the display of list items will flicker
// with a large number of updates.
//
void VNoteBook::refresh(boolean b)
{
    refreshFlag = b;
    if (b) {
        updateTabs();
    }
}

// ===========================================================================
#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
void VNoteBook::putTo(VArchiver& a)
{
    VNoteBook::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//      Archive this VNoteBook object and, if most derived, its children.
//
//      Type:           What:
//                                              VControl base class object data
//              char *          name of activatePage callback method's client's class
//              char *          name of activatePage callback method    
//
void VNoteBook::putTo(VArchiver& a, boolean most_derived)
{
    VControl::putTo(a, FALSE);                              //      base object data

    a << VNoteBookArchiveTag;

    a << (long) tabType;
    a << (long) tabBinding;
    a << (long) tabPlacement;

    const char *active_class_name   = 0;                    
    const char *active_method_name  = 0;                    

    VCallbackList::findCallback(activeMthd, active_class_name, active_method_name, 
                                                            activeMthdIndex);

    a << (char *) active_class_name;                //      active method's class name
    a << (char *) active_method_name;               //      active method name

    // collection of pages
    a.putObject(&thePages);

    // put pages as children
    if (most_derived) {
        VWindow::putChildrenTo(a);                      //      children
    }
}

// --------------------------------------------------------------------------
void VNoteBook::getFrom(VArchiver& a)
{
    VControl::getFrom(a);                                           //      base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VNoteBookArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VNoteBookArchiveTag));
        return;
    }

    long x;
    a >> x;
    tabType = (TabType) x;
    a >> x;
    tabBinding = (BindingType) x;
    a >> x;
    tabPlacement = (TabPlacement) x;

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
}

// --------------------------------------------------------------------------
void VNoteBook::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VNoteBook::getFrom(a);

    if (do_realize) {
        VNoteBook::realize((VWindow *)parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
	}
}

// --------------------------------------------------------------------------
void VNoteBook::realize(VWindow *parent)
{
    VControl::realize(parent);

    // loop through pages, set theWindow
    if (parent != 0) {
        integer index = 0;
        DO (thePages, VNotePage, page)
            if (page != 0 && !page->isVrfWindow) {
                page->theWindow = (VWindow *) childrenOf()->idAt(index);
                page->theWindow->hide();
                index++;
            }
        END
    }

    // delayed client resolution
    if (client == 0 && clientIndex == -1 && activeMthd != 0) {
        client = VCallbackList::findClientForCallback(activeMthd, this);
    }
}

// --------------------------------------------------------------------------
// Load any resources needed by this window or its children
// (i.e. the pages)
//
boolean VNoteBook::loadResources(VRscArchiver& a)
{
    boolean isopen = a.isOpen();

    if (!isopen) {
        if (!a.open(VRscArchiver::ReadOnly)) {
            return FALSE;
        }
    }

    // loop through pages, load 'em if they need it
    DO (thePages, VNotePage, page)
        if (page != 0 && page->isVrfWindow && page->theWindow == 0) {
            page->theWindow = loadPageWin(a, page->theWindowName);
            if (page->theWindow != 0) {
                page->theWindow->frameChildren();
                page->theWindow->hide();
            }
        }
        else if (page != 0 && page->theWindow != 0) {
			page->theWindow->loadResources(a);
		}
    END

    if (!isopen) {
        a.close();
    }

    gotoPage(0);
    return TRUE;
}

#endif // CV_NOARCHIVER
