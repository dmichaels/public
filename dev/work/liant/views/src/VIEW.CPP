// $Header:   Q:/views/win/vcs/view.cpv   1.33   Mar 04 1997 17:56:00   gregm  $

//  view.cpp
//
//	VView implementation
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
//  03/25/93 pat    preened
//  04/28/93 pat    added VView() and operator() methods with name argument;
//                  getFrom(), putTo(), realize() methods;
//                  included archiver.h
//  07/15/93 pat    added sysmenu to archive data (!!! pat--always 0, for now)
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/25/93 jmd    changed setTitle to const VString, VFrame to const VFrame
//                  removed free()
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/09/93 jmd    removed use of free
//  08/16/93 jmd    made name const VString
//  09/21/93 pat    setTitle() in realize() only if title is not ""
//  09/22/93 pat    added archive tag
//  10/01/93 dgm    added support for StyleMinimize.
//  10/04/93 dgm    fix in stdSize().
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  08/08/94 dgm    Cleaned up styles.
//  08/31/94 dss    Added isTopLevel().
//  09/05/94 dss    getInfo(): exStyle initialization.
//  11/03/94 dss    Moved accelerator, def button, focus mnmgt from VDialog.
//                  Backed out archive tag rev.
//  11/11/94 dgm    Removed delete accels in clean() as freeAll() does this.
//  11/11/94 dss    Archiver tags macros.
//  05/28/95 jld    enterHit() doesn't call clicked() if defButton
//					in disabled or hidden
//  06/23/95 dss    getInfo(): StyleSizable sets WS_BORDER MSW style.
//					Fixes bug where size bars drew without thin, black,
//					external border.
//  08/02/95 evc    fixed setChildFocus so it doesn't GPF when 0 is passed in.
//  08/23/95 dss    setChildFocus(): if no window getting focus then set
//					the default button on.
//  09/01/95 dss    VStyle changes. Removed CPPV_11 code.
//  09/29/95 dgm    Added constrainMinimumSize() and resizeClientArea().
//  10/02/95 dgm    Minor fix in stdSize().
//  10/05/95 dss    Moved 3d default from ctor to getInfo. Added default style
//                      functionality.
//  10/18/95 dgm    Removed constrainMinimumSize() and resizeClientArea().
//                  Added getNonClientAreaSize().
//	10/24/95 dss	takeFocus(): if no kids, set focus to me. solves BUG where
//                  no key events were getting to view.
//	11/01/95 evc    Changed behavioew of setAccel so accelerator set owned
//					by the active top-level window is the one used by notifier
//					to resolve accel keys with.
//	11/01/95 dgm	Added #ifndef CV_NOOLE's.
//					Moved getComponentThis() from view.h to view.cpp.
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	01/09/96 dss	VComponent name change.
//	01/29/96 dgm	Changed isTopLevel() to be a const function.
//  02/19/96 evc    Made defaultStyle CV_NEARDATA
//	05/01/96 glm	added getFromWithTag to pass archive tag back to 
//					VTreeListBox, so old archives can be read.
//	09/26/96 dgm	Draw 3D borders on StyleBorder and Window 95.
//
//	02/04/97 pkt	removed handling of style defaults from 'init'.
//	02/04/97 pkt	made implicit management of styles transient.
//	12/18/96 dgm	Added StyleHelpBox support.
// ----------------------------------------------------------------------------

#include "accel.h"
#include "iterator.h"
#include "notifier.h"
#include "poppwndw.h"
#include "pushbttn.h"
#include "set.h"
#include "view.h"
#include "winclass.h"
#include "cvassert.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "rscarch.h"
#   include "callback.h"
#endif

// Compound Document Objects
#ifndef CV_NOOLE
#include "cmpview.h"
#endif

defineClass(VView, VWindow)
//
// revision 0   original
// revision 1   added accels, iDefButton, iEscButton
//
defineArchiveRevision(VView, 2)

// ----------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER
defineCallback(VView, okPressed)
defineCallback(VView, cancelPressed)
#endif

//----------------------------------------------------------------------------
VStyle& VView::getClassDefaultStyle(void)
{
    static VWinStyle CV_NEARDATA classDefaultStyle(VWindow::getClassDefaultStyle() +
                                       setTransient + Style3D);

    return classDefaultStyle;
}

//----------------------------------------------------------------------------
VStyle& VView::getDefaultStyle(void) const
{
    return getClassDefaultStyle();
}

//---------------------------------------------------------------------------
//
void    VView::init() 
{ 
    vScroll = 0;
    hScroll = 0; 
    sysmenu = 0; 
    // model= 0; 
    accels = 0;
    defButton = 0;
    escButton = 0;

#ifndef CV_NOARCHIVER
    iDefButton=-1;
    iEscButton=-1;
#endif

#ifndef CVS_LATER_DEFAULT
    style = setTransient + getDefaultStyle();
#endif

    // Note: can't call notifier during construction of classDefaultStyle
    // so we enforce 3D here. Should be done at construction since 3d
    // availablity is application scope.
#if defined(CV_WIN32) || defined(CV_CTL3D)
    if (notifier->threeDEffectsAvailable()) {
        style += Style3D;
    }
#else
    // no 3d allowed.
    style -= Style3D;
#endif
	dontKey = 0;

}

//----------------------------------------------------------------------------
//  Construct a VView which has no presentation space.
//
//  Note that the presentation space may be constructed later with operator().
//
VView::VView() 
{  
    VView::init();
}

//----------------------------------------------------------------------------
VView::VView(const VString& wname, const VFrame &frame, VWindow *pwin, 
			 const VStyle& style)
{
    VView::init();
    (*this)(frame, pwin, style, wname);
}

//----------------------------------------------------------------------------
//  Construct a VView with the given frame, parent, and style.  The style
//  can be one or more of: 
//
//      StyleDefault, StyleCloseBox, StyleMaximize, StyleSizable, 
//      StyleBorder, StyleCaption, StyleHidden, StyleHorizontal, 
//      or StyleVertical.
//
//  The StyleScrolls style may be used to specify both horizontal and vertical
//  scroll bars.
//
VView::VView(const VFrame &frame, VWindow *parent, const VStyle& style) 
{
    VView::init();
    (*this)(frame, parent, style);
}

//----------------------------------------------------------------------------
//
VView::~VView()
{
    clean();
}

//----------------------------------------------------------------------------
//  Free any auxilary memory associated with this VView.
//
void VView::clean()
{
    if (hScroll) {
        delete hScroll;     // jmd (was free)
        hScroll = 0;
    }
    if (vScroll) {
        delete vScroll;     // jmd (was free)
        vScroll = 0;
    }
    if (sysmenu) {
        delete sysmenu;     // jmd (was free)
        sysmenu = 0;
    }

	if (accels) {
        accels->freeAll();  // this does a delete on itself too!
        accels = 0;
    }
}

//----------------------------------------------------------------------------
//  Supply the VView class information.
//
void VView::getInfo(VWinInfo& viewinfo)
{
    viewinfo.wClass = notifier->CTwindow();

    viewinfo.style = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    viewinfo.exStyle = 0;

    if (style.contains(StyleSizable)) {
#ifdef CVS_USE_TRANSIENT
        style -= setTransient + StyleBorder + StyleModalBorder;
#else
        style -= StyleBorder + StyleModalBorder;
#endif
        viewinfo.style |= WS_BORDER | WS_THICKFRAME;
    }
    else if (style.contains(StyleModalBorder)) {
#ifdef CVS_USE_TRANSIENT
        style -= setTransient + StyleBorder;
#else
        style -= StyleBorder;
#endif
        viewinfo.exStyle |= WS_EX_DLGMODALFRAME;
    }
	else if (style.contains(StyleBorder)) {
        static int CV_NEARDATA windows95 = LOBYTE(LOWORD(GetVersion())) >= 4;
        if (windows95) {
			viewinfo.exStyle |= WS_EX_CLIENTEDGE;
        }       
        else {
			viewinfo.style |= WS_BORDER;
        }
    }

    if (style.contains(StyleCaption)) {
        viewinfo.style |= WS_CAPTION | WS_BORDER;
    }
    if (style.contains(StyleCloseBox)) {
        viewinfo.style |= WS_SYSMENU | WS_CAPTION;
    }
#if defined(WS_EX_CONTEXTHELP)
    if (style.contains(StyleHelpBox)) {
        viewinfo.exStyle |= WS_EX_CONTEXTHELP;
    }
#endif
    if (style.contains(StyleMaximize))  {
        viewinfo.style |=  WS_MAXIMIZEBOX;
    }
    if (style.contains(StyleMinimize))  {
        viewinfo.style |=  WS_MINIMIZEBOX;
    }
    if (style.contains(StyleHorizontal))  {
        viewinfo.style |= WS_HSCROLL;
    }
    if (style.contains(StyleVertical))  {
        viewinfo.style |= WS_VSCROLL;
    }
    if (!style.contains(StyleHidden)) {
        viewinfo.style |= WS_VISIBLE;
	}
}

//----------------------------------------------------------------------------
//  Build a presentation space for this VView and name the view.
//
void VView::operator()(const VFrame &frame, VWindow *pwin, const VStyle& style,
                       const VString& wname) 
{
    VWindow::operator()(frame, pwin, style, wname);

    if(style.contains(StyleVertical)) {
        vScroll = new VScrollBar((boolean)TRUE,(VWindow *)this);
    }
    if(style.contains(StyleHorizontal)) {
        hScroll = new VScrollBar((boolean)FALSE,(VWindow *)this);
    }
}

//----------------------------------------------------------------------------
//  Build a presentation space for this VView.
//
void VView::operator()(const VFrame &frame, VWindow *parent, const VStyle& style) 
{
	VWindow::operator()(frame, parent, style);

    if(style.contains(StyleVertical)) {
        vScroll = new VScrollBar((boolean) TRUE, (VWindow *) this);
    }
    if(style.contains(StyleHorizontal)) {
        hScroll = new VScrollBar((boolean) FALSE, (VWindow *) this);
    }
}

//----------------------------------------------------------------------------
//  Increment '*width' and '*height' by the amount added to the client area of
//  this VView for a given style specificaton.  For example, if this VView
//  has the sizable style, then the extra thickness of the frame beyond the
//  client area is added to '*width' and '*h', repectively.  The 'style' can
//  be one of: StyleCloseBox, StyleSizable, StyleTitle, StyleHorizontal, 
//  or StyleVertical.
//
void VView::stdSize(const VStyle& style, int *width, int *height)
{
    VWindow::stdSize(style, width, height);

    if (style.contains(StyleSizable)) {
        *width  += (GetSystemMetrics(SM_CXFRAME)  * 2) -
				   (GetSystemMetrics(SM_CXBORDER) * 2);
        *height += (GetSystemMetrics(SM_CYFRAME)  * 2) - 
                   (GetSystemMetrics(SM_CYBORDER) * 2);
    }

    if (style.contains(StyleCaption))  {
        //
        // Note that according to MS docs, SM_CYCAPTION includes
        // the SM_CYBORDER (!), so we need to subtract it out here.
        // DGM, 10.4.93.
        //
        *height += (GetSystemMetrics(SM_CYCAPTION) -
                    GetSystemMetrics(SM_CYBORDER));
    }
    if (style.contains(StyleHorizontal))  {
        *height += GetSystemMetrics(SM_CYHSCROLL);
    }
    if (style.contains(StyleVertical))  {
        *width += GetSystemMetrics(SM_CXVSCROLL);
    }
}

//----------------------------------------------------------------------------
//  Supply the minimum size of this VView.
//
void VView::getMinSize(short &width, short &height)
{
    width   = (GetSystemMetrics(SM_CXVSCROLL) * 2)
                 + GetSystemMetrics(SM_CXHSCROLL);
    width  +=  GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXMIN);

    height  = (GetSystemMetrics(SM_CYVSCROLL) * 2) 
                + GetSystemMetrics(SM_CYHSCROLL);
    height +=  GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYMIN) 
                + GetSystemMetrics(SM_CYMENU);
}

//----------------------------------------------------------------------------
//  Set this VWindow to be a bordered, sizable window. Note that this method
//  does not cause the window to be redisplayed.  Use this method before the
//  window becomes visible, or send the update message after using it.
//
void VView::setSizable()
{
    long  s;

    s = GetWindowLong(hWnd, GWL_STYLE); 
    s |= WS_THICKFRAME | WS_BORDER;
	SetWindowLong(hWnd, GWL_STYLE, s);
    style += StyleSizable;
    frame(); 
}       

//----------------------------------------------------------------------------
//  Return a pointer to the VSysMenu object that is associated with the
//  close box of this VView; NIL,  if this VView does not have a
//  close box.
//
VSysMenu *VView::getSysMenu()
{
    if (!sysmenu && (style.contains(StyleCloseBox))) {
        sysmenu = new VSysMenu(this);
    }
    return sysmenu;
}

//----------------------------------------------------------------------------
//  This implementation of close hides this VView and returns
//  TRUE to the VNotifier indicating that no further action should be taken.
//  Classes derived from VView can overload this method to achieve 
//  different results.  For example, returning FALSE to the VNotifier will
//  result in this VView (and all of its childern) being destroyed. 
//
boolean VView::close()
{
    hide();

#ifndef CV_NOOLE
    if (getComponentThis()) {
        // if the component overrides close, then return abort close.
        if (getComponentThis()->close()) {
            return TRUE;
        }
    }
#endif

    return TRUE;
}

//----------------------------------------------------------------------------
//
boolean VView::activate(VView *v)
{
#ifndef CV_NOOLE
    if (getComponentThis()) {
        getComponentThis()->activate(v);
	}
#endif
    // always return FALSE to let the OS process the deactivation:
    // we are using this message as a notification, not overriding it.
    return FALSE;
}

//----------------------------------------------------------------------------
//
boolean VView::deactivate(VView *v)
{
#ifndef CV_NOOLE
    if (getComponentThis()) {
        getComponentThis()->deactivate(v);
    }
#endif
    // always return FALSE to let the OS process the deactivation:
    // we are using this message as a notification, not overriding it.
    return FALSE;
}

//----------------------------------------------------------------------------
//  Set the title (caption) of this VView to 's'.
//
void VView::setTitle(const VString& s)
{
    if (!(style.contains(StyleCaption))) {
        long n = GetWindowLong(hWnd, GWL_STYLE); 
        n |= WS_CAPTION;
        SetWindowLong(hWnd, GWL_STYLE, n);
        style += StyleCaption;
    }
    sTitle = s;
    SetWindowText(hWnd, (LPSTR)s.gets());
    frame(); 
}

//----------------------------------------------------------------------------
//  Return the type of window this is.
//
enum WinType VView::winType()
{
    return WinView;
}

//----------------------------------------------------------------------------
//  Copy up to 'n' characters of the title (caption) of this VView 
//  to 's'.  Return the number of characters copied or zero if there is no
//  title.
//
int VView::getTitle(char *s, int n)
{
    return((int) sTitle.gets(s, n));
}

//----------------------------------------------------------------------------
//  Copy up to 'n' characters of the title (caption) of this VView 
//  into VString object 's'.  Return the number of characters copied
//  or zero if there is no title.
//
int VView::getTitle(VString &s)
{
    int n = (int)sTitle.size();
    s = (char *)sTitle.gets();
    return n;
}

//----------------------------------------------------------------------------
//
//
boolean VView::isTopLevel() const
{
	// Views are top level if the aren't resizable or they have a modal
    // border.
    //!!!dss 9/19/95 - the or'ing of the contains is a hack until the 
    // contains/exclusive bug is fixed.
    return style.contains(StyleSizable) || style.contains(StyleModalBorder);
}

//----------------------------------------------------------------------------
boolean VView::takeFocus()
{
	return VWindow::takeFocus();
}

//----------------------------------------------------------------------------
boolean VView::givenFocus()
{
    return VWindow::givenFocus();
}

//----------------------------------------------------------------------------
// Set the current child VWindow having the focus to 'win'.  If 'win'
// is not a VPushButton object, then reset the 'default' status on the original
// VPushButton to have that style (that is, the VPushButton object that was
// passed with the setDefButton() call).
//
void VView::setChildFocus(VWindow *child) 
{ 
    VWindow::setChildFocus(child);

    if ((child != 0) && (defButton != 0)) {
		VWindow *focus_window = notifier->getFocus();
		if ((focus_window == 0) || !focus_window->isA(VPushButtonCls)) {
			defButton->setDefault(TRUE);
		}
	}
}

// ---------------------------------------------------------------------------
// Set the VPushButton object that will initially have the 'default'
// push button status attribute to 'pb'. This VPushButton object will always
// be set to the 'default' status when no other VPushButton in this VDialog
// has focus.
//
void VView::setDefButton(VPushButton *pb)
{
	VWindow *top_level_parent = getTopLevelParent();
	if ((top_level_parent != this) &&
		(top_level_parent != 0) && top_level_parent->isA(VViewCls)) {
		((VView *)top_level_parent)->setDefButton(pb);
		return;
	}

    if ((pb != defButton) && ((defButton == 0) || !defButton->hasFocus())) {
        if (defButton != 0) {
            defButton->setDefault(FALSE);
        }
        if ((defButton = pb) != 0) {
            pb->setDefault(TRUE);
        }
    }
}

// ---------------------------------------------------------------------------
// Set the VPushButton object that will initially have the 'escape'
// push button status attribute to 'pb'.
//
void VView::setEscButton(VPushButton *pb)
{
    if (pb != escButton) {
        // escape buttons should never display as the default.
        if ((escButton = pb) != 0) {
        	cv_assert(defButton != escButton);
            escButton->setDefault(FALSE);
        }
    }
}

// This adds an Accelerator to the set of Accelerators that 'belong' to the
// VView.  If the VView is active and a "top level" window, 
// then the notifier will resolve Accelerator key mapping to this set.
// NOTE that this routine used to add the Accelerator to the
// global set of accels that hung off the notifier, and was global to 
// the system.

// Set an accerator key for window 'win' to the virtual key code 'key', 
// where 'win' is a child window (typically a VControl) of this VView.
// WHen the method is 0 the some default processing occurs depending on
// the kind of object the 'win' is. 
// If 'win' is a VControl object, then when the virtual key 'key' is pressed
// the notify() event for that control will be called.  Hence, if 'win' is a
//  VButton, then its clicked() method will be called; or if 'win' is a
// VListBox or an VEditLine, then they will receive the keyboard focus.
//
void VView::setAccel(int key, VWindow *win,  method  mthd)
{
    if (!accels) {
        accels = new VSet;
    }

    accels->add(new VAccelerator(key, win, mthd));
}
//---------------------------------------------------------------------------
void VView::unsetAccel(int key)
{
	VObject *obj;
	VAccelerator a(key);
	if (!accels || key == K_NULL) {
		return;
	}
	if (obj = accels->remove(&a)) {
		delete obj;		
	}
}

//----------------------------------------------------------------------------
boolean VView::ok(VButton *)
{
    return FALSE;
}

//----------------------------------------------------------------------------
boolean VView::cancel(VButton *)
{
    return FALSE;
}

// ---------------------------------------------------------------------------
// Callback for OK button. The default
// behavior is to close the dialog successfully. 
//
boolean VView::okPressed(VButton *b)
{
    return ok(b);
}

// ---------------------------------------------------------------------------
// Callback for Cancel button. The default
// behavior is to close the dialog successfully.
//
boolean VView::cancelPressed(VButton *b)
{
    return cancel(b);
}

//----------------------------------------------------------------------------
// The implementation of this event response function is to invoke the 
// clicked() operation on the VPushButton object that currently has the 
// default status.
//
boolean VView::enterHit()
{
	if (isTopLevel() && (defButton != 0) &&
						 defButton->isEnabled() && !defButton->isHidden()) {
		return defButton->clicked();
	}
	else {
		return ok(0);
	}
}

//----------------------------------------------------------------------------
// The implementation of this event response function is to invoke the 
// clicked() operation on the VPushButton object that currently has the 
// escape status.
//
boolean VView::escapeHit()
{
	if (escButton != 0) {
		return escButton->clicked();
	}
	else {
		return cancel(0);
	}
}

// ----------------------------------------------------------------------------
//j
boolean VView::key(int virtual_key, char ascii_key)
{
	if (VWindow::key(virtual_key, ascii_key)) {
		return TRUE;
	}
	else {
		return propagateKey(virtual_key, ascii_key);
	}
}

// ---------------------------------------------------------------------------
//
void VView::getSystemBorderSize(int *width, int *height) const
{
	int w, h;

	VWindow::getSystemBorderSize(width, height);

    if (style.contains(StyleVertical))  {
        w = GetSystemMetrics(SM_CXVSCROLL);
    }
	else {
		w = 0;
	}
    if (style.contains(StyleHorizontal))  {
        h = GetSystemMetrics(SM_CYHSCROLL);
    }
	else {
		h = 0;
	}

	if (width  != 0) { *width  += w; }
	if (height != 0) { *height += h; }
}

// ---------------------------------------------------------------------------
//
void VView::enableBool(boolean b)
{
	VWindow::enableBool(b);
	if (hScroll != 0) {
		if (b) {
			hScroll->enable();
		}
		else {
			hScroll->disable();
		}
	}
	if (vScroll != 0) {
		if (b) {
			vScroll->enable();
		}
		else {
			vScroll->disable();
		}
	}
}

//---------------------------------------------------------------------------
//
VComponentView *VView::getComponentThis()
{
	return (VComponentView *) VWindow::getComponentThis();
}

// ===========================================================================

#ifndef CV_NOARCHIVER

//----------------------------------------------------------------------------
// VView::putTo
//
void VView::putTo (VArchiver& a)
{
    VView::putTo(a, TRUE);
}

//---------------------------------------------------------------------------
// VView::putTo
//
void VView::putTo (VArchiver& a, boolean most_derived)
{
    VWindow::putTo (a, FALSE);          //  base object data

    a << VViewArchiveTag;
    
    a.putObject(&sTitle);               //  title
    a.putObject(0);                     //  system menu !!!

    if (VViewTagRev >= 1) {
        a.putObject(accels);                        //  accelerator set
        if ((defButton != 0) &&                 //  default button
            (getParent() != 0) &&
            (getParent()->archiveChildrenOf() != 0)) {
            a << (getParent()->archiveChildrenOf()->indexOfSame(defButton) + 1);
        }
        else {
            a << 0;
        }

        if ((escButton != 0) &&                 //  escape button
            (getParent() != 0) &&
            (getParent()->archiveChildrenOf() != 0)) {
            a << (getParent()->archiveChildrenOf()->indexOfSame(escButton) + 1);
        }
        else {
            a << 0;
        }
    }

    if (most_derived) {
        VWindow::putChildrenTo(a);      //  children
    }
}

//---------------------------------------------------------------------------
// VView::getFrom
//
void VView::getFrom(VArchiver& a)
{
    VWindow::getFrom(a);                    //  base object data
    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VViewArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VViewArchiveTag));
        return;
    }
    a.getObject(&sTitle);                   //  title
    sysmenu = (VSysMenu *) a.getObject(0);  //  system menu !!!pat -- not implemented

    if (getArchiveTagRev(tag) >= 1) {
        accels = (VSet *) a.getObject(0);
        a >> iDefButton;                            //  default button index    
        a >> iEscButton;                            //  escape button index 
    }


}

//---------------------------------------------------------------------------
// This is a copy of the getFrom method, except it allows us to pass the 
// the VView tag back to a derived class
void VView::getFromWithTag(VArchiver& a, long *vTag)
{
    VWindow::getFrom(a);                    //  base object data
    long tag;
    a >> tag;

	*vTag = tag;

    if (!archiveTagOk(tag, VViewArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VViewArchiveTag));
        return;
    }
    a.getObject(&sTitle);                   //  title
    sysmenu = (VSysMenu *) a.getObject(0);  //  system menu !!!pat -- not implemented

    if (getArchiveTagRev(tag) >= 1) {
        accels = (VSet *) a.getObject(0);
        a >> iDefButton;                            //  default button index    
        a >> iEscButton;                            //  escape button index 
    }

}

//---------------------------------------------------------------------------
// VView::getFrom
//
void VView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VView::getFrom(a);

    if (do_realize) {
        VView::realize((VWindow *)parent);
    }

    if (parent != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }

    if (iDefButton > 0) {
        defButton = (VPushButton *) childrenOf()->idAt(iDefButton - 1);
    }

    if (iEscButton > 0) {
        escButton = (VPushButton *) childrenOf()->idAt(iEscButton - 1);
    }

}

//---------------------------------------------------------------------------
// VView::realize
//
void VView::realize(VWindow *parent)
{
    VWindow::realize(parent);

    if (sTitle.size() > 0) {
        //  setting the title may change border features, only set
        //  title if title is not the empty string
        //
        setTitle(sTitle.gets());
    }

    if(style.contains(StyleVertical)) {
        vScroll = new VScrollBar((boolean) TRUE, (VWindow *) this);
    }

    if(style.contains(StyleHorizontal)) {
        hScroll = new VScrollBar((boolean) FALSE, (VWindow *) this);
    }
}


#endif // ARCHIVER
