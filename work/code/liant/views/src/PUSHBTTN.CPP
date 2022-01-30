// $Header:   Q:/views/win/vcs/pushbttn.cpv   1.17   Mar 04 1997 17:55:40   gregm  $ 

//  pushbttn.cpp
//
//	VPushButton implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  04/28/93 pat    added VButton() with name argument;
//                  added getFrom(), putTo(), realize();
//                  included archiver.h
//  05/14/93 pat    added destructor (virtual)
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/08/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  08/16/93 jmd    made name const VString
//  09/22/93 pat    added archive tag
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  03/04/94 pat    CV_WIN32:  GetWindowLong().
//  03/26/94 pat    CV_WIN32: cast appropriately for SendMessage()
//  10/30/94 jmd    preened
//  11/03/94 jmd    preened (missed a bit)
//  11/11/94 dss    Archiver tags macros.
//  05/24/95 jld    toggle() member function inherited from VButton
//  09/01/95 dss    VStyle changes.
//	01/06/96 dgm	Added VFrame-less constructors.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#include "pushbttn.h"
#include "view.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

extern "C" {
#   undef NULL
#   include <string.h>
}

defineClass(VPushButton, VButton)
defineArchiveRevision(VPushButton, 0)

// ---------------------------------------------------------------------------
// Destroy this VPushButton.
//
VPushButton::~VPushButton()
{
	VWindow *top_level_parent = getTopLevelParent();
	if ((top_level_parent != 0) && top_level_parent->isA(VViewCls)) {
		VPushButton *default_button =
					 ((VView *)top_level_parent)->getDefButton();
		if (this == default_button) {
			((VView *)top_level_parent)->defButton = 0;
		}
	}
}

// ---------------------------------------------------------------------------
// Return the pixel dimensions of this VPushButton in '*w' and '*h'
// respectively, given a size in characters of '*w' length
// and '*h' height.
//
void VPushButton::sizeOf(int *w, int *h)
{
    int cw, ch;
    fontSize(&cw, &ch);
    *h = h[0]*ch + ch /2;
    *w = w[0]*cw + (14 *cw /4);
}

// ---------------------------------------------------------------------------
// Construct a VPushButton from the resource control-id 'cntrl' of
// the resource associated with 'pWin'. This is a non-portable GUI system method.
//
VPushButton::VPushButton(unsigned ctrl, VWindow *win)
{
    getRscWindow(ctrl, win);
    long s = GetWindowLong(hWnd, GWL_STYLE); 
    if (s & BS_DEFPUSHBUTTON) {
        isdefault = TRUE;
    }
    else {
        isdefault = FALSE;
    }
}

// ---------------------------------------------------------------------------
// Construct a VPushButton at position 'x','y' of of standard size
// with label 'label' within 'win'. The 'style' parameter may be 
// StyleDefaultButton, StyleNoAutoDefault, or StyleAutoToggle.
//
VPushButton::VPushButton(VRatio x, VRatio y, char *label, VWindow *win, 
                         const VStyle& style)
{
    int w, h = 1;
    isdefault = FALSE;
    if (!label) {
        label = "<unknown>";
    }
    w = strlen(label);
    sizeOf(&w, &h);
    (*this)(VFrame(x, y, w, h), win, style);
    putText(label);
}

// ---------------------------------------------------------------------------
// Construct a VPushButton with frame 'frame', parent 'win', 
// style 'style' and optional label 'label'. By default the 'style' parameter 
// is set to StyleDefault. Optionally, 'style' may be set to StyleDefaultButton
// or StyleNoAutoDefault or StyleAutoToggle.
//
VPushButton::VPushButton(const VFrame &frame, VWindow *win, 
							const VStyle& style, char *label) 
{
    isdefault = FALSE;
    (*this)(frame, win, style);
    if (label) {
        putText(label);
    }
}

//----------------------------------------------------------------------------
//
VPushButton::VPushButton(const VString& wname,
                         const VFrame&  frame, VWindow *pwin, 
                         const VStyle& style, char *label) 
{
    isdefault = FALSE;
    (*this)(frame, pwin, style, wname);
    if (label) {
        putText(label);
    }
}

//----------------------------------------------------------------------------
// Called by the VNotifier to obtain info on the underlying GUI system
// window represented by this VPushButton object.
//
void VPushButton::getInfo(VWinInfo &pushinfo)
{

    VButton::getInfo(pushinfo);
    pushinfo.style &= ~BS_OWNERDRAW;
//  isauto = FALSE;
    if (style.contains(StyleDefaultButton)) {
        pushinfo.style |= BS_DEFPUSHBUTTON;
        isdefault = TRUE;
    }
    else {
        pushinfo.style |= BS_PUSHBUTTON;
    }
}

// ---------------------------------------------------------------------------
// Return TRUE if this VPushButton is highlighted, otherwise return FALSE.
//
boolean VPushButton::isHilited()
{
    return (boolean) (0x0004 & SendMessage(hWnd, BM_GETSTATE, (WPARAM) 0, (LPARAM) 0) );
}

// ---------------------------------------------------------------------------
// If 'b' is TRUE, then set this VPushButton to the default
// push button style, otherwise set it to the standard style. 
//
void VPushButton::setDefault(boolean b)
{
	if (b) {
		VWindow *top_level_parent = getTopLevelParent();
		if ((top_level_parent != 0) && top_level_parent->isA(VViewCls)) {
			VPushButton *current_default_button =
						 ((VView *)top_level_parent)->getDefButton();
			if ((current_default_button != 0) &&
				(current_default_button != this) &&
				 current_default_button->isdefault) {
				current_default_button->isdefault = FALSE;
				HWND hwnd = current_default_button->hwnd();
				if (hwnd != 0) {
					SendMessage(hwnd, BM_SETSTYLE,
								WPARAM(BS_PUSHBUTTON), LPARAM(1));
				}
			}
		}
		isdefault = TRUE;
		SendMessage(hWnd, BM_SETSTYLE, WPARAM(BS_DEFPUSHBUTTON), LPARAM(1));
		if (fromRsc) {
#if defined(CV_WIN32)
			SendMessage(GetParent(hWnd), DM_SETDEFID, 
						WPARAM(GetWindowLong(hWnd, GWL_ID)), LPARAM(0));
#else
			SendMessage(GetParent(hWnd), DM_SETDEFID, 
						(WPARAM) GetWindowWord(hWnd, GWW_ID), (LPARAM) 0);
#endif
		}
	}
	else {
		isdefault = FALSE;
		SendMessage(hWnd, BM_SETSTYLE, WPARAM(BS_PUSHBUTTON), LPARAM(1));
	}
}

// ---------------------------------------------------------------------------
// Highlight this VPushButton if 'b' is TRUE, otherwise unhighlight it. 
//
void VPushButton::hilite(boolean b)
{
    SendMessage(hWnd, BM_SETSTATE, (WPARAM) b, (LPARAM) 0);   
}

// ---------------------------------------------------------------------------
// Construct a VPushButton button with no presentation space.
//
VPushButton::VPushButton() 
{ 
//  isauto = FALSE; 
    isdefault = FALSE; 
}

// ---------------------------------------------------------------------------
// Called when this VWindow has been given the keyboard focus.  The
// default behavior is to accept the focus and, if enabled, accept the default
// push button style.
//  
boolean VPushButton::givenFocus() 
{
    if (!style.contains(StyleNoAutoDefault)) {
        setDefault(TRUE); 
    }
    VButton::givenFocus();
    return FALSE;
}

// ---------------------------------------------------------------------------
// The implementation of this method calls the clicked() method if
// this VPushButton has the default push button style.
//
boolean VPushButton::enterHit()
{
    if (isdefault) {
        return clicked();
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
// Called when this VWindow has just lost the keyboard focus. 
//
boolean VPushButton::clearFocus()
{
    if (!style.contains(StyleNoAutoDefault)) {
		VWindow *top_level_parent = getTopLevelParent();
		if ((top_level_parent != 0) && top_level_parent->isA(VViewCls)) {
			if (((VView *)top_level_parent)->getDefButton() != this) {
        		setDefault(FALSE);
			}
		}
    }
    VButton::clearFocus();
    return FALSE;
}

// ---------------------------------------------------------------------------
//
void VPushButton::getSystemBorderSize(int *width, int *height) const
{
	if (width  != 0) { *width  = 2 * 2; }
	if (height != 0) { *height = 2 * 2; }
}

// ===========================================================================
#ifndef CV_NOARCHIVER

//---------------------------------------------------------------------------
// 
void VPushButton::putTo(VArchiver& a)
{
    VPushButton::putTo(a, TRUE);
}

//---------------------------------------------------------------------------
// 
void VPushButton::putTo(VArchiver& a, boolean most_derived)
{
    VButton::putTo(a, FALSE);               //  base object data

    a << VPushButtonArchiveTag;

    a << isdefault;                         //  is default?

    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

//---------------------------------------------------------------------------
// 
void VPushButton::getFrom(VArchiver& a)
{
    VButton::getFrom(a);                    //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VPushButtonArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VPushButtonArchiveTag));
        return;
    }

    a >> isdefault;                         //  is default?
}

//---------------------------------------------------------------------------
// 
void VPushButton::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VPushButton::getFrom(a);

    if (do_realize) {
        VPushButton::realize((VWindow *) parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

//---------------------------------------------------------------------------
//
void VPushButton::realize(VWindow *parent)
{
    VButton::realize(parent);
	if (isDefault()) {
		VWindow *top_level_parent = getTopLevelParent();
		if ((top_level_parent != 0) && top_level_parent->isA(VViewCls)) {
    		((VView *)top_level_parent)->setDefButton(this);
		}
	}
}

#endif // CV_NOARCHIVER
