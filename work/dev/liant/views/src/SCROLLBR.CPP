// $Header:   Q:/views/win/vcs/scrollbr.cpv   1.19   Mar 04 1997 17:55:48   gregm  $ 

//
//  scrollbr.cpp
//
//	VScrollBar implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//  04/07/93 pat    changed id to VObject *
//  06/21/93 pat    added support for VArchiver; preened.
//                  added constructor that takes name argument.
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/27/93 pat    removed free(); added changeMthdIndex; 
//                  updated findCallback() calls
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/23/93 dgm    added setVertical(), setHorizontal(), 
//                  isHorizontal(), and isVertical().
//  08/25/93 dgm    updated setVertical() and setHorizontal()
//                  to call notifier->recreateWin().
//  09/02/93 pat    added cllbckls.h; updated findCallback() calls
//  09/22/93 pat    added archive tag
//  10/12/93 pat    initialize currentPos realization data.
//  01/27/94 dgm    added additional getText() to avoid hiding warnings.
//  02/02/94 pat    added archive tag check
//  02/09/94 jmd    converted to use long values
//  02/09/94 pat    updated defineArchiveTag(); added ClientCallbackMismatch.
//  02/15/94 pat    added int cast on SetScrollRange() lnBar parameter
//  08/31/94 dss    Added getCtlColor().
//  09/05/94 dss    getInfo(): exStyle initialization.
//  11/11/94 dss    Archiver tags macros.
//  09/01/95 dss    VStyle changes.
//	02/09/96 dgm	Added getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
//	04/23/96 dgm	Added setPageSize(), getPageSize(), and setScrollValues().
//	05/14/96 dgm	Fix in putTo().
//	06/21/96 dgm	Added dragging for VNotifer flag.
//	08/13/96 dgm	Changed page-size processing.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "scrollbr.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "cllbckls.h"
#endif

defineClass(VScrollBar, VControl)
defineArchiveRevision(VScrollBar, 0)

#if defined(CV_WIN32) && defined(SIF_PAGE)
#define CV_USE_SET_SCROLL_INFO
#endif

// ---------------------------------------------------------------------------
//
#define scrollBarType()	(control ? SB_CTL : (vertical ? SB_VERT : SB_HORZ))
#define setScalingFactor(min, max) (scalingFactor = \
									(max) >= (min) ? \
									 ((max) <= 0x7FFF ? \
									  1.0F : float(max) / float(0x7FFF)) : \
									 ((min) <= 0x7FFF ? \
									  1.0F : float(min) / float(0x7FFF)))

// ---------------------------------------------------------------------------
//  Construct a VScrollBar with no presentation space.
//
VScrollBar::VScrollBar() 
{
    init(SB_CTL); 
}

// ---------------------------------------------------------------------------
//  Construct a VScrollBar from the resource control-id 'cntrl' of
//  the resource associated with 'pWin'. This is a non-portable 
//  GUI system method.
//
VScrollBar::VScrollBar(unsigned cntrl, VWindow *pWin)
{
    getRscWindow(cntrl, pWin);
    init(SB_CTL);
}

// ---------------------------------------------------------------------------
//  Construct a VScrollBar with frame 'frame', parent 'win' and style 
//  'style'. If 'style' is StyleVertical or StyleHorizontal, then construct
//  a vertical VScrollBar or a horizontal VScrollBar, respectively.
//
VScrollBar::VScrollBar(const VFrame& frame, VWindow * win, const VStyle& style) 
{
    (*this)(frame, win, style);
    init(SB_CTL);
}

// ---------------------------------------------------------------------------
//  Construct a named VScrollBar with frame 'frame', parent 'win' and style 
//  'style'. If 'style' is StyleVertical or StyleHorizontal, then construct
//  a vertical VScrollBar or a horizontal VScrollBar, respectively.
//
VScrollBar::VScrollBar(const VString& wname, const VFrame& frame, VWindow * win, 
                       const VStyle& style) 
{
    (*this)(frame, win, style, wname);
    init(SB_CTL);
}

// ---------------------------------------------------------------------------
//  Initialize this scrollbar upon construction
//
void VScrollBar::init(unsigned n) 
{
	if (n == SB_CTL) {
		control = 1;
		vertical = 0;
	}
	else if (n == SB_HORZ) {
		control = 0;
		vertical = 0;
	}
	else if (n == SB_VERT) {
		control = 0;
		vertical = 1;
	}
    client = NIL;
    changeMthd = 0;
    endChangeMthd = 0;
	lineClickMthd = 0;
	pageClickMthd = 0;
    hidden = FALSE;
    dotrack = FALSE;
	pageSize = 0;
    scalingFactor = 1.0F;
    setValues(10000L, 0L, 100L, 1000L);
    if (hWnd) {
        setPos(0L);
    }
    currentPos = 0L;
	dragging = 0;

#ifndef CV_NOARCHIVER
    changeMthdIndex = -1L;
	lineClickMthdIndex = -1L;
	pageClickMthdIndex = -1L;
#endif

	disableFocus();
}

// ---------------------------------------------------------------------------
//
void VScrollBar::getInfo(VWinInfo &scrollinfo)
{
    scrollinfo.wClass = "scrollbar";
    scrollinfo.style = WS_CHILD;
    scrollinfo.exStyle = 0;

    if (!(style.contains(StyleHidden))) {
        scrollinfo.style |= WS_VISIBLE;
    }

    if (style.contains(StyleVertical)) {
        scrollinfo.style |= SBS_VERT;
    }
    else {
        scrollinfo.style |= SBS_HORZ;
    }
}

// --------------------------------------------------------------------------
// 
UINT VScrollBar::getCtlColor()
{

#if defined(CV_WIN32)
    return WM_CTLCOLORSCROLLBAR;
#else
    return CTLCOLOR_SCROLLBAR;
#endif

}

// ---------------------------------------------------------------------------
//
VScrollBar::~VScrollBar()
{
    if (!control) {
        hWnd = 0;
    }
}

// ---------------------------------------------------------------------------
//
void VScrollBar::sizeOf(int *w, int *h)
{
    *w = GetSystemMetrics(SM_CXHSCROLL);
    *h = GetSystemMetrics(SM_CYVSCROLL);
}

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b)) 
#endif

// ---------------------------------------------------------------------------
//  
void VScrollBar::getMinSize(short &w, short &h)
{
    w = max(GetSystemMetrics(SM_CXVSCROLL), 
        GetSystemMetrics(SM_CXHSCROLL));
    h = max(GetSystemMetrics(SM_CYVSCROLL), 
        GetSystemMetrics(SM_CYHSCROLL));
}

// ---------------------------------------------------------------------------
//  Return the current value of this VScrollBar.
//
long VScrollBar::getPos()
{
    return currentPos;

//  return scaleValue(GetScrollPos(hWnd, scrollBarType()));
}

// ---------------------------------------------------------------------------
//  Scroll this VScrollBar one line down.
//
void VScrollBar::lineDown()
{
	if ((client != 0) && (lineClickMthd != 0)) {
		if ((client->*lineClickMthd)(1)) {
			return;
		}
	}

    long newpos = getPos() + lineAmt;
    if (newpos > maxVal) {
        newpos = maxVal;
    }
    else if (newpos < minVal) {
        newpos = minVal;
    }
	if (pageSize > 0) {
		long maxpos = maxVal - (pageSize - 1);
		if (newpos > maxpos) {
			newpos = maxpos;
		}
	}
    track(newpos);
}

// ---------------------------------------------------------------------------
//  Scroll this VScrollBar one line up.
//
void VScrollBar::lineUp()
{
	if ((client != 0) && (lineClickMthd != 0)) {
		if ((client->*lineClickMthd)(-1)) {
			return;
		}
	}

    long newpos = getPos() - lineAmt;
    if (newpos < minVal) {
        newpos = minVal;
    }
    else if (newpos > maxVal) {
        newpos = maxVal;
    }
    track(newpos);
}

// ---------------------------------------------------------------------------
//  Scroll this VScrollBar one page down.
//
void VScrollBar::pageDown()
{
	if ((client != 0) && (pageClickMthd != 0)) {
		if ((client->*pageClickMthd)(1)) {
			return;
		}
	}

    long newpos = getPos() + pageAmt;
    if (newpos > maxVal) {
        newpos = maxVal;
    }
    else if (newpos < minVal) {
        newpos = minVal;
    }
	if (pageSize > 0) {
		long maxpos = maxVal - (pageSize - 1);
		if (newpos > maxpos) {
			newpos = maxpos;
		}
	}
    track(newpos);
}

// ---------------------------------------------------------------------------
//  Scroll this VScrollBar one page up.
//
void VScrollBar::pageUp()
{
	if ((client != 0) && (pageClickMthd != 0)) {
		if ((client->*pageClickMthd)(-1)) {
			return;
		}
	}

    long newpos = getPos() - pageAmt;
    if (newpos < minVal) {
        newpos = minVal;
    }
    else if (newpos > maxVal) {
        newpos = maxVal;
    }
    track(newpos);
}

// ---------------------------------------------------------------------------
//  Return TRUE if this VScrollBar is hidden (not displayed on screen), 
//  else return FALSE. 
//
boolean VScrollBar::isHidden()
{
    if (control) {
        return VControl::isHidden();
    }
    else  {
        return hidden;
    }
}

// ---------------------------------------------------------------------------
//
void VScrollBar::showBool(boolean on, boolean update)
{
    if (!control) {
        if (hWnd) {
            if (on) {
                int minv = int(float(minVal) / scalingFactor);
                int maxv = int(float(maxVal) / scalingFactor);
                SetScrollRange(hWnd, scrollBarType(), minv, maxv, TRUE);
            }
            else {
                SetScrollRange(hWnd, scrollBarType(), 0, 0, TRUE); 
            }
        }
        hidden = !on;
    }
    else {
        VWindow::showBool(on, update);
    }
}

// ---------------------------------------------------------------------------
//  Set the current value of this VScrollBar to 'val' and, if tracking
//  is enabled, call the change method of the client with it.
//
void VScrollBar::track(long val)
{
	int maxval = (pageSize > 0) ? maxVal - (pageSize - 1) : maxVal;
	if (val < minVal) {
		val = minVal;
	}
	else if (val > maxval) {
		val = maxval;
	}
	if (dragging) {
    	currentPos = val;
	}
	else {
	    setPos(val);
	}
    if (dotrack && (client != 0) && (changeMthd != 0)) {
        client->perform(changeMthd, val);
    }
}

// ---------------------------------------------------------------------------
//  Arrange for method 'mthd' of the client 'clnt' to be called when
//  this 'VScrollBar'\'s value has changed.
//
void VScrollBar::uponChange(VObject *clnt, method mthd) 
{ 
    client = clnt;
    changeMthd = mthd;
}

// ---------------------------------------------------------------------------
//
void VScrollBar::uponEndChange(VObject *o, method m)
{ 
    client = o;
    endChangeMthd = (boolean (VObject::*)())m;
}

// ---------------------------------------------------------------------------
//
void VScrollBar::uponLineClick(VObject *o, method m)
{ 
    client = o;
    lineClickMthd = (boolean (VObject::*)(int))m;
}

// ---------------------------------------------------------------------------
//
void VScrollBar::uponPageClick(VObject *o, method m)
{ 
    client = o;
    pageClickMthd = (boolean (VObject::*)(int))m;
}

// ---------------------------------------------------------------------------
//  Set the top and bottom (left and right) range values of this
//  VScrollBar to 'hi' and 'lo', respectively.
//
void VScrollBar::setRange(long hi, long lo)
{
    minVal = lo;
    maxVal = hi;
	setScalingFactor(lo, hi);
    long minv = int(float(minVal) / scalingFactor);
    long maxv = int(float(maxVal) / scalingFactor);
    SetScrollRange(hWnd, scrollBarType(), minv, maxv, TRUE);
    track(getPos());
}

// ---------------------------------------------------------------------------
//  Set this VScrollBar\'s lineAmt and pageAmt to 'line' and 'page', 
//  respectively.
//
void VScrollBar::setScrollAmts(long line, long page)
{
    lineAmt = line;
    pageAmt = page;
    track(getPos());
}

// ---------------------------------------------------------------------------
//  Set the current value of this VScrollBar to 'val'. 
//
void VScrollBar::setPos(long val)
{
    currentPos = val;
    int v = int(float(val) / scalingFactor);
    SetScrollPos(hWnd, scrollBarType(), v, TRUE);
}

// ---------------------------------------------------------------------------
//  Set the top and bottom (left and right) range values of this
//  VScrollBar to 'hi' and 'lo' and set the lineAmt and pageAmt to
//  'line' and 'page', respectively.
//
void VScrollBar::setValues(long hi, long lo, long line, long page)
{
    minVal = lo;
    maxVal = hi;
    lineAmt = line;
    pageAmt = page;

	int minv, maxv;

	setScalingFactor(lo, hi);

    if (hWnd) {
        minv = int(float(minVal) / scalingFactor);
        maxv = int(float(maxVal) / scalingFactor);
        SetScrollRange(hWnd, scrollBarType(), minv, maxv, TRUE);
    }
    track(getPos());
}

// ---------------------------------------------------------------------------
// If page-size is supported, then it controls the length of the scroll-bar
// thumb.  It is typically set in such a way that it is representative of
// the proportion of the total work-area which is currently viewable within
// the view-area.  If it is used, its value must be between 1 and maxVal.
// If it is not used or supported, its value is 0.
//
void VScrollBar::setPageSize(long page)
{
#if defined(CV_USE_SET_SCROLL_INFO)
	pageSize = page;
	if (maxVal > minVal) {
		if (pageSize > maxVal - minVal) {
			pageSize = maxVal - minVal;
		}
	}
	else if (pageSize > minVal - maxVal) {
		pageSize = minVal - maxVal;
	}
	if (hWnd != 0) {
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO); 
		if (pageSize > 0) {
			scrollinfo.fMask = SIF_PAGE;
			scrollinfo.nPage = pageSize;
			SetScrollInfo(hWnd, scrollBarType(), &scrollinfo, TRUE); 
		}
		else {
			scrollinfo.fMask = SIF_PAGE;
			scrollinfo.nPage = pageSize;
			SetScrollInfo(hWnd, scrollBarType(), &scrollinfo, TRUE); 
		}
	}
#endif
}

// ---------------------------------------------------------------------------
//
void VScrollBar::setScrollValues(long min, long max, long page)
{
	setRange(max, min);
	setPageSize(page);
}

// ---------------------------------------------------------------------------
//
void VScrollBar::setScrollValues(long min, long max, long page, long pos)
{
    minVal = min;
    maxVal = max;
	setScalingFactor(min, max);
    long minv = int(float(minVal) / scalingFactor);
    long maxv = int(float(maxVal) / scalingFactor);
    SetScrollRange(hWnd, scrollBarType(), minv, maxv, TRUE);
	setPageSize(page);
	setPos(pos);
}

// ---------------------------------------------------------------------------
//
void VScrollBar::setScrollValues(long min, long max, long page, long pos,
								 long line_amount, long page_amount)
{
    lineAmt = line_amount;
    pageAmt = page_amount;
	setScrollValues(min, max, page, pos);
}

// ---------------------------------------------------------------------------
//  Overloaded to do nothing since VScrollBar objects have no text
//  content.
//
void VScrollBar::putText(const VString& s) 
{
    return;
} 

// ---------------------------------------------------------------------------
//  Return zero to reflect the fact that there is no text content
//  in VScrollBar objects.
//
int VScrollBar::getText(char *s, unsigned i) 
{
    s = 0; 
    i = 0; 
    return 0;
} 

// ---------------------------------------------------------------------------
//
int VScrollBar::getText(VString& s)
{
    s.reset();
    return 0;
}

// ---------------------------------------------------------------------------
//  
enum WinType VScrollBar::winType() 
{ 
    return WinControl; 
}

// ---------------------------------------------------------------------------
//  PRIVATE: return a special vertical/horizontal VScrollBar.
//  
VScrollBar::VScrollBar(boolean vertical, VWindow *aWin)
{
    hWnd = aWin->hwnd();
    client = aWin;
    init(vertical ? SB_VERT : SB_HORZ);
}

// ---------------------------------------------------------------------------
//  PRIVATE: Called when the user has stopped scrolling
//  this VScrollBar. 'Val' is its current value. 
//
void VScrollBar::endScroll(long val)
{
	if (val < minVal) {
		val = minVal;
	}
	else if (val > maxVal) {
		val = maxVal;
	}
    setPos(val);
    if (client != 0) {
		if (changeMthd != 0) {
	        client->perform(changeMthd, val);
		}
		if (endChangeMthd != 0) {
			(client->*endChangeMthd)();
		}
    }
}

// ---------------------------------------------------------------------------
void VScrollBar::setVertical(boolean b)
{
    long s;
    if (b) {
        if (style.contains(StyleVertical)) {
            return;
        }
        style +=  StyleVertical;
        style -= StyleHorizontal;
        s = GetWindowLong(hWnd, GWL_STYLE);
        s |=  SBS_VERT;
        s &= ~SBS_HORZ;
    }
    else {
        if (!style.contains(StyleVertical)) {
            return;
        }
        style +=  StyleHorizontal;
        style -= StyleVertical;
        s = GetWindowLong(hWnd, GWL_STYLE);
        s |=  SBS_HORZ;
        s &= ~SBS_VERT;
    }
    SetWindowLong(hWnd, GWL_STYLE, s);
    notifier->recreateWin(this);
}

// ---------------------------------------------------------------------------
void VScrollBar::setHorizontal(boolean b)
{
    setVertical(!b);
}

// ---------------------------------------------------------------------------
boolean VScrollBar::isVertical() const
{
    return style.contains(StyleVertical);
}

// ---------------------------------------------------------------------------
boolean VScrollBar::isHorizontal() const
{
    return (!isVertical());
}

// ---------------------------------------------------------------------------
//
void VScrollBar::getPreferredSize(int *width, int *height) const
{
	static int vertical_width, horizontal_height;

#if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
	static int first_time = 1;
	if (first_time) {
		static int windows95 = LOBYTE(LOWORD(GetVersion())) >= 4;
		if (windows95) {
			first_time = 0;
			NONCLIENTMETRICS ncm;
			ncm.cbSize = sizeof(ncm);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
			vertical_width = ncm.iScrollWidth;
			horizontal_height = ncm.iScrollHeight;
		}
		else {
			vertical_width = 14;
			horizontal_height = 14;
		}
	}
#else
	vertical_width = 14;
	horizontal_height = 14;
#endif
	if (width != 0) {
		if (isVertical()) {
			*width = vertical_width;
		}
		else {
			*width = 100;
		}
	}
	if (height != 0) {
		if (isHorizontal()) {
			*height = horizontal_height;
		}
		else {
			*height = 100;
		}
	}
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//  Put this object to the archive.  This method is called from 
//  VArchiver::putObject().
//
void VScrollBar::putTo(VArchiver& a)
{
    VScrollBar::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//  Put this object to the archive by archiving the base class layer, 
//  then the object data of this class layer.
//
//  If 'most_derived' is FALSE, then this method has been called from a derived
//  class.  That is, this class is a base to some derived class and we should
//  not archive our children, if any.
//
//  If 'most_derived' is TRUE, then this method is being called directly from a 
//  VArchiver.  That is, this class is the most derived class layer for this 
//  object and we archive our children, if any.
//
void VScrollBar::putTo(VArchiver& a, boolean most_derived)
{
    VControl::putTo(a, FALSE);          //  base object data

    a << VScrollBarArchiveTag;
    
    a << maxVal;
    a << minVal;
    a << lineAmt;
    a << pageAmt;
    a << getPos();
	boolean b = dotrack; a << b;

    const char *change_class_name   = 0;            
    const char *change_method_name  = 0;            

    VCallbackList::findCallback(changeMthd, change_class_name,  
                                    change_method_name, changeMthdIndex);

    a << (char *) change_class_name;        //  change method's class name
    a << (char *) change_method_name;       //  change method name

    if (most_derived) {
        VWindow::putChildrenTo(a);
    }
}

// --------------------------------------------------------------------------
//  Get this object from the archive by retrieving the object 
//  data of the base class and then the object data of this class.
//
//  This method is invoked by the getFrom() of this most-derived class or by
//  the getFrom() of a derived class.
//
void VScrollBar::getFrom(VArchiver& a)
{
    VControl::getFrom(a);                   //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VScrollBarArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VScrollBarArchiveTag));
        return;
    }
    
	boolean track;
    a >> maxVal;
    a >> minVal;
    a >> lineAmt;
    a >> pageAmt;
    a >> currentPos;
    a >> track; dotrack = track;

    char        method_name[256];
    char *      method_name_ptr = method_name;
    char        class_name[256];
    char *      class_name_ptr = class_name;

    a >> class_name_ptr;                //  change method's class name
    a >> method_name_ptr;               //  change method name

    method changeM = VCallbackList::findCallback(class_name_ptr, 
                                        method_name_ptr, &changeMthdIndex);

    if (client != 0 && changeM != 0) {
        if (!client->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    uponChange(client, changeM);
}

// --------------------------------------------------------------------------
//  Get this object from the archive.  The 'data' parameter is a VWindow *
//  that is our parent window.  In response, retrieve our object data,
//  realize the presentation space under the parent, and finally, retrieve
//  our children.
//
//  This method is invoked by VArchiver::getObject().
//
void VScrollBar::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VScrollBar::getFrom(a);             

    if (do_realize) {
        VScrollBar::realize((VWindow *)data);
    }

    if (data != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
//  Realize the presentation space of this object.  Upon entry, the
//  Views object exists, but no GUI object.  First, the realize() method of 
//  our base class is invoked to build the GUI object, then we set any 
//  additional object data that affects the GUI.
//
//  This method is invoked by the getFrom() method of this most-derived 
//  class or by the getFrom() of a derived class.
//
void VScrollBar::realize(VWindow *pwin)
{
    VControl::realize(pwin);

    setValues(maxVal, minVal, lineAmt, pageAmt);
    track(currentPos);

	// delayed client resolution
	if (client == 0 && clientIndex == -1 && changeMthd != 0) {
		client = VCallbackList::findClientForCallback(changeMthd, this);
	}
}

// --------------------------------------------------------------------------

#endif // CV_NOARCHIVER  
// ===========================================================================


