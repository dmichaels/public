// $Header:   Q:/views/win/vcs/editline.cpv   1.13   Mar 04 1997 17:55:00   gregm  $ 

//  editline.cpp
//
//	VEditLine implementation.
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  09/15/92 jmd    added support for StyleReadOnly
//  05/14/93 pat    added destructor (virtual)
//  06/21/93 pat    added support for VArchiver, preened some.
//  06/21/93 pat    added constructor with name arg.
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/27/93 pat    removed free()
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  09/22/93 pat    added archive tag
//  10/06/93 dgm    added setHorizontalScroll().
//  11/12/93 pat    moved local variable in setHorizontalScroll() to ifdef
//                  where used.
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  07/19/94 jld    added StyleEditSecure support
//  11/11/94 dss    Archiver tags macros.
//  09/01/95 dss    VStyle changes.
//	01/05/96 dgm	Overrode getPreferredSize().
//	01/29/96 dgm	Preferred-size changes.
//	04/19/96 glm	removed v1.1 code/preened
//	09/25/96 dgm	Respect StyleNoBorder.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "editline.h"
#include "str.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

extern "C" {
#   include <string.h>
}

defineClass(VEditLine, VEditBox)
defineArchiveRevision(VEditLine, 0)

// ---------------------------------------------------------------------------
//  Construct a VEditLine with no presentation space.
//
VEditLine::VEditLine()
{
    useTabs = FALSE;
}

// ---------------------------------------------------------------------------
//  Construct a VEditLine from the resource control-id 'cntrl' of the
//  resource associated with 'pwin'. This is a non-portable GUI system method.
//
VEditLine::VEditLine(unsigned cntrl, VWindow *pwin)
{
    useTabs = FALSE;
    getRscWindow(cntrl, pwin);
}

// ---------------------------------------------------------------------------
//  Construct a VEditLine with frame 'frame' parent 'win' and style
//  'style'. The 'style' parameter may be one of `StyleLeft` (the default), 
//  `StyleRight` or `StyleCenter` or\'ed with `StyleBorder`.
//  
VEditLine::VEditLine(const VFrame& frame, VWindow *win, const VStyle& style)
{
    useTabs = FALSE;
    (*this)(frame, win, style);
}

// ---------------------------------------------------------------------------
//  Construct a VEditLine with frame 'frame' parent 'win' and style
//  'style'. The 'style' parameter may be one of `StyleLeft` (the default), 
//  `StyleRight` or `StyleCenter` or\'ed with `StyleBorder`.
//  
VEditLine::VEditLine(const VString& wname, const VFrame& frame, 
						VWindow *win, const VStyle& style)
{
    useTabs = FALSE;
    (*this)(frame, win, style, wname);
}

// ---------------------------------------------------------------------------
//
void VEditLine::getInfo(VWinInfo& lineinfo)
{
    VEditBox::getInfo(lineinfo);
    
    // replace the style returned from VEditBox.
    lineinfo.style = WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS;

    if (!(style.contains(StyleHidden))) {
        lineinfo.style |= WS_VISIBLE;
    }
    
    if (style.contains(StyleBorder)) {
        lineinfo.style |= WS_BORDER;
    }
        
#if defined(CV_WIN32)
    if (has3dEffects(V_WIN3D_EFFECTS)) {
        lineinfo.style &= ~WS_BORDER;
    	if (!style.contains(StyleNoBorder)) {
	        lineinfo.exStyle |= WS_EX_CLIENTEDGE;
		}
    }
#endif

    if (style.contains(StyleEditSecure)) {
        lineinfo.style |= ES_PASSWORD;
    }

    if (style.contains(StyleCenter)) {
        lineinfo.style |= ES_CENTER;
    }
    else if (style.contains(StyleRight)) {
        lineinfo.style |= ES_RIGHT;
    }
    else { // default = StyleLeft
        lineinfo.style |= ES_LEFT;
    }

    if (style.contains(StyleHorizontal)) {
        lineinfo.style |= ES_AUTOHSCROLL;
    }
    if (style.contains(StyleVertical)) {
        lineinfo.style |= ES_AUTOVSCROLL;
    }
    if (style.contains(StyleReadOnly)) {
        lineinfo.style |= ES_READONLY;
    }
}

// ---------------------------------------------------------------------------
//  Destroy this VEditLine.
//
VEditLine::~VEditLine()
{
    ;
}

// ---------------------------------------------------------------------------
//
void VEditLine::getMinSize(short& w, short& h)
{
    int fw, fh;

    fontSize(&fw, &fh);
    w = fw;
    h = fh;
}

// ---------------------------------------------------------------------------
//  The implementation of this method in VEditLine passes the
//  event to its parent (see `VWindow::enterHit()`).
//
boolean VEditLine::enterHit()
{
    if (parent) {
        return parent->enterHit();
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
//  Return the minimum width and height in screen pixel units for
//  legibility of this VEditLine.
//
void VEditLine::getMin(int *w, int *h)
{
    int tw, th;

    VEditBox::getMin(w, h);
    fontSize(&tw, &th);

    tw *= sizeOfText() + 2;
    th += th / 2;

    if (tw > *w) {
        *w = tw;
    }

    if (th > *h) {
        *h = th;
    }
}

// ---------------------------------------------------------------------------
//  The implementation of this method in VEditLine accepts focus and
//  sets its text as selected. (see `VWindow::takeFocus()`).
//
boolean VEditLine::takeFocus()
{
    boolean ret;

    if (ret = VWindow::takeFocus()) {
        selectAll(NIL);
    }
    return ret;
}

void VEditLine::setHorizontalScroll(boolean b)
{
#if 0
    long s;
#endif

    if (b) {
        if (hasHorizontalScroll()) {
            return;
        }
        else if (hWnd != 0) {
            style += StyleHorizontal;
            #if 0
            s = GetWindowLong(hWnd, GWL_STYLE) | ES_AUTOHSCROLL;
            SetWindowLong(hWnd, GWL_STYLE, s);
            #else
            notifier->recreateWin(this);
            #endif
        }
    }
    else if (!hasHorizontalScroll()) {
        return;
    }
    else if (hWnd != 0) {
        style -= StyleHorizontal;
        #if 0
        s = GetWindowLong(hWnd, GWL_STYLE) & ~ES_AUTOHSCROLL;
        SetWindowLong(hWnd, GWL_STYLE, s);
        #else
        notifier->recreateWin(this);
        #endif
    }
}

// ---------------------------------------------------------------------------
//
void VEditLine::getPreferredSize(int *width, int *height) const
{
	queryPreferredSize(width, height, FALSE, FALSE);
	if (!style.contains(StyleNoBorder)) {
		if (width  != 0) { *width  += 4; }
		if (height != 0) { *height += 4; }
	}
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//
void VEditLine::putTo(VArchiver& a)
{
    VEditLine::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//
void VEditLine::putTo(VArchiver& a, boolean most_derived)
{
    VEditBox::putTo(a, FALSE);              //  base object data

    a << VEditLineArchiveTag;

    if (most_derived) {                     //  children
        VWindow::putChildrenTo(a);
    }
}

// --------------------------------------------------------------------------
//
void VEditLine::getFrom(VArchiver& a)
{
    VEditBox::getFrom(a);                       //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VEditLineArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VEditLineArchiveTag));
        return;
    }
}

// --------------------------------------------------------------------------
//
void VEditLine::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VEditLine::getFrom(a);              

    if (do_realize) {
        VEditLine::realize((VWindow *)data);
    }

    if (data != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
//
void VEditLine::realize(VWindow *pwin)
{
    VEditBox::realize(pwin);
}

// --------------------------------------------------------------------------

#endif //   ARCHIVER    
// ===========================================================================

