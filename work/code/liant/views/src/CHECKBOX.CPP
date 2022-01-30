// $Header:   Q:/views/win/vcs/checkbox.cpv   1.20   Mar 04 1997 17:16:46   gregm  $

//  checkbox.cpp
//
//	VCheckBox implementation
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
//  05/13/93 pat    added constructor that takes name argument;
//                  added getFrom(), putTo(), realize() and
//                  included archiver.h;
//  05/14/93 pat    added destructor (virtual)
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/09/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  08/16/93 jmd    made name const VString
//  09/22/93 pat    added archive tag
//  10/12/93 pat    initialized checkedFlag realization data
//  10/12/93 jmd    preened
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  03/27/94 pat    CV_WIN32: cast appropriately for SendMessage().
//  11/11/94 dss    Archiver tags macros.
//  05/24/95 jld    inherit toggle() from VButton, 
//					rely on native toggle behavior
//  09/01/95 dss    VStyle changes.
//  01/24/95 evc	Put toggle() back in and overrode VButton::click.
//	01/29/96 dgm	Overrode getPreferredSize() & getSystemBorderSize().
//	04/19/96 glm	removed v1.1 code/preened
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ----------------------------------------------------------------------------

#include "checkbox.h"
#include "inclgrp.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#endif

extern "C" {
#   undef NULL
#   include <string.h>
}

defineClass(VCheckBox, VButton)
defineArchiveRevision(VCheckBox, 0)

// ---------------------------------------------------------------------------
//  Construct a VCheckBox button with no presentation space.
//
VCheckBox::VCheckBox() 
#ifndef CV_NOARCHIVER
        :   checkedFlag(FALSE)
#endif
{
    ;
}

// ---------------------------------------------------------------------------
// Construct a VCheckBox from the resource control-id 'cntrl' of the
// resource associated with 'pwin'. This is a non-portable GUI system method.
//
VCheckBox::VCheckBox(unsigned ctrl, VWindow *pwin) 
#ifndef CV_NOARCHIVER
        :   checkedFlag(FALSE)
#endif
{ 
    getRscWindow(ctrl, pwin);
    if (hWnd) { //if autotoggle in resource, let MS Windows handle it.
        long s = GetWindowLong(hWnd, GWL_STYLE);
        if ((s & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX) {
            isauto = TRUE;
        }
        else {
            isauto = FALSE;
        }
    }   
}

// ---------------------------------------------------------------------------
// Construct a VCheckBox of standard size to hold label 'label'
// at position 'x', 'y' within 'win'. The default style for VCheckBox
// buttons is StyleAutoToggle.
//
VCheckBox::VCheckBox(VRatio x, VRatio y, const char *label, 
						VWindow *win, const VStyle& style)
#ifndef CV_NOARCHIVER
        :   checkedFlag(FALSE)
#endif
{
    int w, h = 1;
    if (!label) {
        label = "<unknown>";
    }
    w = strlen(label);
    sizeOf(&w, &h);
    (*this)(VFrame(x, y, w, h), win, style);
    putText(label);
}

// ---------------------------------------------------------------------------
// Construct a VCheckBox with frame 'frame' parent 'win', 
// style 'style', and optional label 'label'.  The default style for
// VCheckBox buttons is StyleAutoToggle.
//
VCheckBox::VCheckBox(const VFrame &frame, VWindow *win, 
						const VStyle& style, const char *label) 
#ifndef CV_NOARCHIVER
        :   checkedFlag(FALSE)
#endif
{
    (*this)(frame, win, style);
    if (label) {
        putText(label);
    }
}

// ---------------------------------------------------------------------------
//  Construct a VCheckBox with frame 'frame' parent 'win', 
//  style 'style', and optional label 'label'.  The default style for
//  VCheckBox buttons is StyleAutoToggle.
//
VCheckBox::VCheckBox(const VString& wname, const VFrame &frame, VWindow *win, 
                    const VStyle& style, const char *label) 
#ifndef CV_NOARCHIVER
        :   checkedFlag(FALSE)
#endif
{
    (*this)(frame, win, style, wname);
    if (label) {
        putText(label);
    }
}

// ---------------------------------------------------------------------------
//  Destroy this VCheckBox.
//
VCheckBox::~VCheckBox()
{
    ;
}

// ---------------------------------------------------------------------------
// Return the pixel dimensions of this VCheckBox in '*w' and '*h'
// respectively, given a size in characters of '*w' length
// and '*h' height.
//
void VCheckBox::sizeOf(int *w, int *h)
{
    int cw, ch;

    fontSize(&cw, &ch);
    *w = (w[0]+1) *cw + GetSystemMetrics(SM_CXHTHUMB);
    *h = h[0]*ch + ch /4;
}

// ---------------------------------------------------------------------------
// Called by the VNotifier to obtain info on the underlying GUI system
// window represented by this VCheckBox object. 
//
void VCheckBox::getInfo(VWinInfo &chkinfo)
{
    VButton::getInfo(chkinfo);
    chkinfo.style &= ~BS_OWNERDRAW;
    chkinfo.style |= style.contains(StyleAutoToggle) ? BS_AUTOCHECKBOX : BS_CHECKBOX;
}

// ----------------------------------------------------------------------------
//
boolean VCheckBox::notify(int code)
{
	if (code == ACCELcode) {
		if (hWnd != 0) {
			if (!VWindow::checkLosingFocus()) {
				takeFocus();
				long s = GetWindowLong(hWnd, GWL_STYLE);
				if ((s & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX) {
					check(!checked());
				}
				return TRUE;
			}
		}
	}
	return VButton::notify(code);
}

// ---------------------------------------------------------------------------
// Return TRUE if this VCheckBox is is checked, otherwise return FALSE.
//
boolean VCheckBox::checked()
{
    return SendMessage(hWnd, BM_GETCHECK, (WPARAM) 0, (LPARAM) 0) 
            ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
//  If 'b' is TRUE, then set the check mark on this VCheckBox,
//  otherwise remove the check mark.
//
void VCheckBox::check(boolean b)
{
    SendMessage(hWnd, BM_SETCHECK, (WPARAM) b ? TRUE: FALSE, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
//
boolean VCheckBox::givenFocus()
{
    return VButton::givenFocus();
}

// ---------------------------------------------------------------------------
//
boolean VCheckBox::clearFocus()
{
    return VButton::clearFocus();
}

// ---------------------------------------------------------------------------
// Toggle the state of this VCheckBox.
//
boolean VCheckBox::toggle()
{
	check(checked() ? FALSE : TRUE);
	return TRUE;
}

boolean VCheckBox::clicked()
{
    if (isauto) {
       long s = GetWindowLong(hWnd, GWL_STYLE);
	   if (!((s & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX)) {
		   toggle();
	   }
    }

    if ((client != 0) && (clickMthd != 0)) {
        client->perform(clickMthd, long(this));
    }

	VWindow *parent = getParent();
	if (parent != 0) {
		extern VClass *VInclusiveGroupCls;
		if (parent->isA(VInclusiveGroupCls)) {
			((VInclusiveGroup *)parent)->clicked(this);
		}
	}

    return TRUE;
}

// ---------------------------------------------------------------------------
//
void VCheckBox::getPreferredSize(int *width, int *height) const
{
	queryPreferredSize(width, height, FALSE, FALSE);
	if (height != 0) { if (*height < 16) { *height = 16; } }
}

// ---------------------------------------------------------------------------
//
void VCheckBox::getSystemBorderSize(int *width, int *height) const
{
    VButton::getSystemBorderSize(width, height);
	if (width != 0) { *width += 18; }
}

// --------------------------------------------------------------------------
// 
UINT VCheckBox::getCtlColor()
{
#if defined(CV_WIN32)
    return WM_CTLCOLORSTATIC;
#else
    return CTLCOLOR_STATIC;
#endif
}

// ===========================================================================
#ifndef CV_NOARCHIVER
 
//---------------------------------------------------------------------------
// 
void VCheckBox::putTo(VArchiver& a)
{
    VCheckBox::putTo(a, TRUE);
}

//---------------------------------------------------------------------------
//  Archive this VCheckBox object and, if most derived, its children.
//
//  Type:       What:
//                  VButton base class object data
//      boolean     checked flag
//
void VCheckBox::putTo(VArchiver& a, boolean most_derived)
{
    VButton::putTo(a, FALSE);               //  base object data

    a << VCheckBoxArchiveTag;

    a << checked();                         //  is checkbox checked?
 
    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

//---------------------------------------------------------------------------
// 
void VCheckBox::getFrom(VArchiver& a)
{
    VButton::getFrom(a);                    //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VCheckBoxArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VCheckBoxArchiveTag));
        return;
    }

    a >> checkedFlag;                       //  checkbox checked?
}

//---------------------------------------------------------------------------
// 
void VCheckBox::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VCheckBox::getFrom(a);

    if (do_realize) {
        VCheckBox::realize((VWindow *)parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

//---------------------------------------------------------------------------
//
void VCheckBox::realize(VWindow *parent)
{
    VButton::realize(parent);

    check(checkedFlag);
}

//---------------------------------------------------------------------------

#endif  //  ARCHIVER
// ===========================================================================


