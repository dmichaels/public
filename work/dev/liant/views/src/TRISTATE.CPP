// $Header:   Q:/views/win/vcs/tristate.cpv   1.10   Mar 04 1997 17:56:00   gregm  $ 

//  tristate.cpp
//
//	VTriState inplementation
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
//  05/18/93 pat    fixed toggle() to call VCheckBox::toggle();
//                  enable() to also grey the checkbox when disabling;
//                  checkbox when disabled.
//                  enabled() to use IsEnabled(), not just check greyed state.
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/09/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  08/16/93 jmd    made name const VString
//  09/22/93 pat    added archive tag
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  03/26/94 pat    CV_WIN32: cast appropriately for SendMessage()
//  11/11/94 dss    Archiver tags macros.
//  09/01/95 dss    VStyle changes.
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "tristate.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

extern "C" {
#   include <string.h>
}

defineClass(VTriState, VCheckBox)
defineArchiveRevision(VTriState, 0)

// ---------------------------------------------------------------------------
//  Destroy this VTriState.
//
VTriState::~VTriState()
{
    ;
}

// ---------------------------------------------------------------------------
//  Construct a VTriState button with no presentation space.
//
VTriState::VTriState()
{
	 ;
}

// ---------------------------------------------------------------------------
//    Construct a VTriState button from the resource control-id 'cntrl'
//    of the resource associated with 'pWin'. This is a non-portable GUI system
//    method.
//
VTriState::VTriState(unsigned ctrl, VWindow *pWin) 
{
    getRscWindow(ctrl, pWin);
}

// ---------------------------------------------------------------------------
//  Construct a VTriState button of default size, and label 'label'
//  at position 'x', 'y' within VWindow 'win'.
//
VTriState::VTriState(VRatio x, VRatio y, char *label, VWindow *win, 
							const VStyle& style)
{
    int w = 1, h = 1;
    if (label) {
        w = strlen(label);
    }
    sizeOf(&w, &h);
    (*this)(VFrame(x, y, w, h), win, style);
    putText(label);
}

// ---------------------------------------------------------------------------
// Construct a VTriState with frame 'frame' parent 'win', 
// style 'style' and optional label 'label'.
// 
VTriState::VTriState(const VFrame& frame, VWindow *win, const VStyle& style, char *label) 
{
    (*this)(frame, win, style);
    if (label) {
        putText(label);
    }
}

//---------------------------------------------------------------------------
// 
VTriState::VTriState(const VString& wname, const VFrame& frame, 
                        VWindow *win, const VStyle& style, char *label) 
{
    (*this)(frame, win, style, wname);
    if (label) {
        putText(label);
    }
}

//---------------------------------------------------------------------------
// Called by the VNotifier to obtain info on the underlying GUI system
// window represented by this VTriState object.
// 
void VTriState::getInfo(VWinInfo &triinfo)
{
    VButton::getInfo(triinfo);
    triinfo.style &= ~BS_OWNERDRAW;
    triinfo.style |= style.contains(StyleAutoToggle) ? BS_AUTO3STATE: BS_3STATE;
}  

//  --------------------------------------------------------------------------
//  Return TRUE if this VTriState button is currently enabled, otherwise 
//  return FALSE.
//
boolean VTriState::enabled()
{
    return isEnabled();
//  return(2 == SendMessage(hWnd, BM_GETSTATE, (WPARAM) 0, (LPARAM) 0) 
//          ? FALSE: TRUE);
}


// ---------------------------------------------------------------------------
//  Return TRUE if checked, otherwise return FALSE. 
// 
boolean VTriState::checked()
{
    return  1 == SendMessage(hWnd, BM_GETCHECK, (WPARAM)0, (LPARAM)0) 
            ? TRUE: FALSE;
}

// ---------------------------------------------------------------------------
//  If 'b' is TRUE, then enable this button for selection.
//  if 'b' is FALSE, then disable this button and grey the check box.
// 
boolean VTriState::enable(boolean b)
{
    VButton::enable(b);
    if (!b) {
        //  the VTriState button's checkbox is gray when disabled
        SendMessage(hWnd, BM_SETCHECK, (WPARAM) 2, (LPARAM) 0);
    }
    return(TRUE);
}

// ---------------------------------------------------------------------------
//  Toggle the checked state of this Tristate button.  
//  If it is not enabled, do nothing.
// 
boolean VTriState::toggle()
{
    // StyleAutoToggle does it for us
    return(TRUE);
}

// ===========================================================================
#ifndef CV_NOARCHIVER

//---------------------------------------------------------------------------
// 
void VTriState::putTo(VArchiver& a)
{
    VTriState::putTo(a, TRUE);
}

//---------------------------------------------------------------------------
// 
void VTriState::putTo(VArchiver& a, boolean most_derived)
{
    VCheckBox::putTo(a, FALSE);             //  base object data

    a << VTriStateArchiveTag;
    
    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

//---------------------------------------------------------------------------
// 
void VTriState::getFrom(VArchiver& a)
{
    VCheckBox::getFrom(a);                  //  base object data
    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VTriStateArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VTriStateArchiveTag));
        return;
    }
}

//---------------------------------------------------------------------------
// 
void VTriState::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VTriState::getFrom(a);

    if (do_realize) {
        VTriState::realize((VWindow *)parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

//---------------------------------------------------------------------------
//
void VTriState::realize(VWindow *parent)
{
    VCheckBox::realize(parent);

    //  disabled tristate buttons have a grey checkbox.  Greying the checkbox
    //  is accomplished with the GUI same message (BM_SETCHECK) as 
    //  VCheckBox::check just used, so re-send the message.

    enable(isEnabled());                
}

//---------------------------------------------------------------------------

#endif  //  CV_NOARCHIVER
// ===========================================================================



