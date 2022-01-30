// $Header:   Q:/views/win/vcs/button.cpv   1.22   Mar 04 1997 17:16:44   gregm  $ 

//
//  button.cpp
//
//  VButton implementation
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
//  04/07/93 pat    changed id to VObject *
//  04/28/93 pat    added VButton() with name argument;
//                  added getFrom(), putTo(), realize(); 
//                  included archiver.h;
//  05/14/93 pat    added destructor (virtual) 
//  05/20/93 jmd    init state to InitialState (BTTNdraw | BTTNenabled)
//  05/20/93 jmd    preened
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/27/93 pat    added clickMthdIndex; used client list in getFrom() and
//                  putTo(); removed free();
//                  updated findCallback() calls
//  08/08/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/20/93 dgm    made StyleBorder archivable (in getInfo()).
//  09/02/93 pat    updated findCallback calls
//  09/22/93 pat    added archive tag
//  10/12/93 pat    initialized isAutoBack realization data
//  10/12/93 jmd    minor preening
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    added ClientCallbackMismatch check
//  02/09/94 pat    updated defineArchiveTag use
//  08/31/94 dss    Added GetCtlColor().
//  09/05/94 dss    getInfo(): exStyle initialization.
//  11/03/94 jmd    preened
//  11/11/94 dss    Archiver tags macros.
//  05/23/95 jld    changes to notify(), paint(), enable(), 
//					givenFocus() and clearFocus() to
//                  maintain the 'state' data member
//  05/30/95 jld    enable() calls VWindow::enableBool() 
//					to maintain default button
//  09/01/95 dss    VStyle changes.
//  01/05/96 dgm    Overrode getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
//	05/20/96 dgm	Added enableBool().
//	09/11/96 dgm	Increase preferred-size just a bit.
//	02/04/97 pkt	Changed 'getFrom' to not redundantly set StyleAutoToggle.
// ----------------------------------------------------------------------------

#include "button.h"
#include "ordcllct.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "cllbckls.h"
#endif

extern "C" {
#   undef NULL
#   include <string.h>
}

defineClass(VButton, VControl)
defineArchiveRevision(VButton, 0)

static const short InitialState = BTTNdraw | BTTNenabled;

// ---------------------------------------------------------------------------
// Construct a VButton with no presentation window.
//
VButton::VButton() 
        : clickMthd(0)
#ifndef CV_NOARCHIVER
        , 
            clickMthdIndex(-1L),
            isAutoBack(FALSE)
#endif
{ 
    state = InitialState;
    isauto = FALSE; 
}

// ---------------------------------------------------------------------------
//  Destroy this VButton.
//
VButton::~VButton()
{
    ;
}

// ---------------------------------------------------------------------------
// Called by the VNotifier to obtain info on the underlying GUI system
// window represented by this VButton object.
//
void VButton::getInfo(VWinInfo &butinfo)
{
    butinfo.wClass = "button";
    butinfo.style = WS_CHILD | BS_OWNERDRAW | WS_CLIPSIBLINGS;
    butinfo.exStyle = 0;

    if (!(style.contains(StyleHidden))) {
        butinfo.style |= WS_VISIBLE;
    }

    if (style.contains(StyleBorder)) {
        butinfo.style |= WS_BORDER;
    }
    isauto = style.contains(StyleAutoToggle) ? TRUE : FALSE;
}

// --------------------------------------------------------------------------
// 
UINT VButton::getCtlColor()
{

#if defined(CV_WIN32)
    return WM_CTLCOLORBTN;
#else
    return CTLCOLOR_BTN;
#endif

}

// ---------------------------------------------------------------------------
void VButton::getMinSize(short &w, short &h)
{
    int wf, hf;

    fontSize(&wf, &hf);
    h = 7 * wf / 4;
    w = 2 * h;
}

// ---------------------------------------------------------------------------
// Called from the VNotifier when this VButton has been clicked on. 
// If a client and callback method have been established, then this method
// notifies the 'client' object of the click via the callback method.  If this
// VButton is automatic, then toggle it. 
//
boolean VButton::clicked()
{
    if (isauto) {
       toggle();
    }

    if (client && clickMthd != NIL_METHOD) {
        client->perform(clickMthd, (long)this);
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
// Construct a VButton with frame 'frame', parent 'win', and style 'style'.
//
VButton::VButton(const VFrame &frame, VWindow *win, const VStyle& style) 
        : clickMthd(0)
#ifndef CV_NOARCHIVER
        , 
            clickMthdIndex(-1L),
            isAutoBack(FALSE)
#endif
{
    (*this)(frame, win, style);
}

//----------------------------------------------------------------------------
VButton::VButton(const VString& wname, const VFrame &frame, VWindow *win, const VStyle& style) 
        : clickMthd(0)
#ifndef CV_NOARCHIVER
        , 
            clickMthdIndex(-1L),
            isAutoBack(FALSE)
#endif
{
    (*this)(frame, win, style, wname);
}

//----------------------------------------------------------------------------
// The implementation of this method in VButton calls enterHit()
// on it's parent.
//
boolean VButton::enterHit()
{
    return parent->enterHit();
}

// ---------------------------------------------------------------------------
// Return, in '*w' and '*h' respectively, the minimum width and height
// in screen pixel units for ledgibility of this VButton\'s label.
//
void VButton::getMin(int *w, int *h)
{
    int tw, th;

    fontSize(&tw, &th);
    *w = tw * (sizeOfText()+3);
    *h = th * 7 / 4;
}

// ---------------------------------------------------------------------------
// Called when the visual state of this user drawn VButton 
// changes. Upon call, the 'state' instance variable is set to one or more
// of the following flags or\'ed together: 
// BTTNdraw - draw the entire button,
// BTTNenabled - the button is enabled else disabled,
// BTTNselectDelta - the selection status of the button has changed,
// BTTNselect - ON if button is selected,
// BTTNfocusDelta - the keyboard of this button has changed, and
// BTTNfocus - ON if the button has focus.
//
boolean VButton::paint()
{
    state &= ~BTTNfocusDelta;
    state &= ~BTTNselectDelta;
    return FALSE;
}

// ---------------------------------------------------------------------------
// Called from the VNotifier when this VButton has been clicked.
//
boolean VButton::notify(int code)
{
    if (code == BN_USERDRAW) {
        DRAWITEMSTRUCT *ds = notifier->userDraw();

        if (ds->itemAction & ODA_FOCUS) {
            state |= BTTNfocusDelta;
        }
        if (ds->itemAction & ODA_SELECT) {
            state |= BTTNselectDelta;
        }
        if (ds->itemAction & ODA_DRAWENTIRE) {
            state |= BTTNdraw;
        }
        if (ds->itemState & ODS_DISABLED) {
            state &= ~BTTNenabled;
        }
        if (ds->itemState & ODS_FOCUS) {
            state |= BTTNfocus;
        }
        if (ds->itemState & ODS_SELECTED) {
            state |= BTTNselect;
        }
        else {
            state &= ~BTTNselect;
        }
        int ret = paint();  
        return ret;
    }

    if (code == ACCELcode) {
        clicked();
        return TRUE;
    }

    if (code == BN_CLICKED && !notifier->buttonStatus()) {
        if ( state & BTTNskipClick ) {
            state &= ~BTTNskipClick;
        }
        else {
            clicked();
        }
        return TRUE;
    }
    else if (code == BN_CLICKED) {
    if ( state & BTTNskipClick ) {
       state &= ~BTTNskipClick;
    }
    return FALSE;
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
// If 'b' is TRUE, then enable this VButton for selection by the user,
// otherwise disable it. 
//
boolean VButton::enable(boolean b)
{
	enableBool(b);
	return FALSE;
}

// ---------------------------------------------------------------------------
//
void VButton::enableBool(boolean b)
{
	if (b) {
		state |= BTTNenabled;
	}
	else {
		state &= ~BTTNenabled;
	}
	VControl::enableBool(b);
}

// ---------------------------------------------------------------------------
//
boolean VButton::givenFocus()
{
    state |= BTTNfocus;
    return VControl::givenFocus();
}

// ---------------------------------------------------------------------------
//
boolean VButton::clearFocus()
{
    state &= ~BTTNfocus;
    return VControl::clearFocus();
}

// ---------------------------------------------------------------------------
// Called when this VButton is to be toggled.  Implemented
// by subclasses. The implementation of this method in VButton does
// nothing.
//
boolean VButton::toggle() 
{ 
    return (boolean) (state & BTTNenabled); 
}

// ---------------------------------------------------------------------------
// Arrange for method 'mthd' of the client object 'clnt' of this
// VButton to be called when it is clicked (see VObject for description
// of the method typedef).
//
void VButton::uponClick(VObject *clnt, method mthd) 
{ 
    if (clnt) {
        client = clnt; 
    }
    if (mthd) {
        clickMthd = mthd; 
    }
    if (!clnt && mthd == NIL_METHOD) {
        client = NIL, clickMthd = NIL;
    }
}

// ---------------------------------------------------------------------------
// Construct a VButton from the the resource control-id 'cntrl' of
// the resource associated with 'pWin'. This is a non-portable GUI system method.
//
VButton::VButton(unsigned ctrl, VWindow *pWin) 
        :  clickMthd(0)
#ifndef CV_NOARCHIVER
        , 
            clickMthdIndex(-1L),
            isAutoBack(FALSE)
#endif
{
    getRscWindow(ctrl, pWin);
    state = InitialState;
}

// ---------------------------------------------------------------------------
// Return the type of this window as a WinControl.
//
enum WinType VButton::winType()
{
    return WinControl;
}

// ---------------------------------------------------------------------------
//
void VButton::getPreferredSize(int *width, int *height) const
{
    queryPreferredSize(width, height, TRUE, TRUE);
	if (width != 0) { *width += 8; }
}

// ===========================================================================
#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
void VButton::putTo(VArchiver& a)
{
    VButton::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//  Archive this VButton object and, if most derived, its children.
//
//  Type:       What:
//                      VControl base class object data
//      char *      name of click callback method's client's class
//      char *      name of click callback method   
//      boolean     auto-toggle flag
//
void VButton::putTo(VArchiver& a, boolean most_derived)
{
    VControl::putTo(a, FALSE);              //  base object data

    a << VButtonArchiveTag;

    const char *click_class_name    = 0;            
    const char *click_method_name   = 0;            

    VCallbackList::findCallback(clickMthd, click_class_name, click_method_name, 
                                                            clickMthdIndex);

    a << (char *) click_class_name;         //  click method's class name
    a << (char *) click_method_name;        //  click method name

    a << isauto;                            //  auto toggle?

    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

// --------------------------------------------------------------------------
void VButton::getFrom(VArchiver& a)
{
    VControl::getFrom(a);                       //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VButtonArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VButtonArchiveTag));
        return;
    }

    char        click_method_name[256];
    char *      click_method_name_ptr = click_method_name;
    char        click_class_name[256];
    char *      click_class_name_ptr = click_class_name;

    a >> click_class_name_ptr;                  //  click method's class name
    a >> click_method_name_ptr;                 //  click method name

    method m = VCallbackList::findCallback(click_class_name_ptr, 
                                    click_method_name_ptr, &clickMthdIndex);

    if (client != 0 && m != 0) {
        if (!client->isA(VClass::of(click_class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch);
            return;
        }
    }

    uponClick(client, m);

    a >> isAutoBack;                            //  auto toggle?

#ifndef CVS_DYNAMIC
	// pkt - 2/3/97 This is obviated by setting isauto explicitly in realize().
    //!!!dss - 8/14/95 why is VWindow setting the style?
    if( isAutoBack ) {
        style += StyleAutoToggle;
    }
    else {
        style -= StyleAutoToggle;
    }
#endif
}

// --------------------------------------------------------------------------
void VButton::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VButton::getFrom(a);

    if (do_realize) {
        VButton::realize((VWindow *)parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
void VButton::realize(VWindow *parent)
{
    VControl::realize(parent);

    //  VButton messes with isauto during window creation (see getInfo()).
    //  this is why we set it, here, to the value that we retrieved 
    //  from the archive.

    isauto = isAutoBack;        

    (void)enable(isEnabled());      

    // delayed client resolution
    if (client == 0 && clientIndex == -1 && clickMthd != 0) {
        client = VCallbackList::findClientForCallback(clickMthd, this);
    }

}

// --------------------------------------------------------------------------

#endif  //  CV_NOARCHIVER
// ===========================================================================

