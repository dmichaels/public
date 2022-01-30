// $Header:   Q:/views/win/vcs/dialog.cpv   1.26   Mar 04 1997 17:54:56   gregm  $ 

//  dialog.cpp
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
//  09/10/92 jmd    added optional style arg to
//						VDialog::VDialog(frame, win, title)
//  07/15/93 pat    added VArchiver support.
//  07/19/93 jmd    made VFrame const in constructor
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/27/93 pat    removed free()
//  08/16/93 jmd    made name const VString
//  08/18/93 jmd    preened, added callbacks, Rsc constructors
//  08/20/93 jmd    removed loadResources()
//  08/25/93 pat    included cllbckls.h, callback.h;
//  08/30/93 pat    unarchived dialog defaults to "TheDialog" only if name not
//                  set
//  09/22/93 pat    added archive tag
//  09/27/93 dgm    fix in setDefButton().
//  10/01/93 dgm    minor fix for modal border in getInfo().
//  10/04/93 dgm    minor fix for modal border in stdSize().
//  10/05/93 jmd    removed loadFromResources
//  10/12/93 pat    initialized iDefButton realization data
//  02/02/94 pat    added archive tag check
//  02/09/94 pat    updated defineArchiveTag use
//  03/27/94 pat    CV_WIN32: cast appropriately for SendMessage().
//  08/17/94 dgm    Added null check in putTo(); fixed up styles.
//  09/05/94 dss    getInfo() buf: exended styles should not be or'ed
//                  with regular styles. Dialogs should have DS_MODALFRAME
//                  style for Ctl3d to work (and in any case).
//  10/11/94 dss    Native dialog support.
//  11/03/94 dss    Moved accelerator, def button, focus mnmgt to view.
//                  Backed out archive tag rev.
//  11/11/94 dss    Archiver tags macros.
//  11/18/94 dss    Need {ok/cancel}Pressed callbacks so that 3.0.1
//                  constructor will find them.
//  01/31/95 evc    Changed way sTitle is filled up to be MBCS safe.
//  09/01/95 dss    VStyle changes.
//  09/27/95 dss    moved default style initializtion from default ctor
//                  to getInfo.
//  09/28/95 dgm    Changed setSysMenu() to pass argument to VSysMenu::minSet()
//                  indicating if it wants to keep the "Size" menu-item; this
//                  fixes a problem in which a sizable dialog could not be
//                  created together with a close-box.
//  09/29/95 dgm    Added constrainMinimumSize() and resizeClientArea().
//  09/27/95 dss    moved default style initializtion from default ctor
//                  o getInfo.
//  10/05/95 dss    GetInfo(): test for only the class default styles that are
//                  missing, not all the styles.
//  10/12/95 dss	Added default style functionality. Removed CPPV_11 code.
//  10/16/95	    call init() from default ctor. Added realizeInit().
//	10/18/95 dgm	Removed constrainMinimumSize() and resizeClientArea().
//					Added getNonClientAreaSize().
//	10/30/95 evc	Added a method as 3 arg to setAccel (with a default of 0).
//  11/01/95 evc    Got rid of setAccel (and put it in VView)
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	12/04/95 pkt	Changed parameters from char* to const char*.
//	02/15/95 evc	Fixed construction of a RC based dialog with native style
//					of WS_THICKFRAME, which implies sizability, to resize.
//	02/19/96 evc	Made the defaultStyle CV_NEARDATA 
//	02/04/97 pkt	Made all implicit use of styles transient.
//	02/04/97 pkt	Removed handling of default styles from 'init'.
//	02/11/97 pkt	Added StyleModalDialog to the class default style.
//	02/11/97 pkt	Changed 'getInfo' to handle default styles more rationally.
// ----------------------------------------------------------------------------

#include "object.h"

#include "accel.h"
#include "class.h"
#include "control.h"
#include "dialog.h"
#include "notifier.h"
#include "sysmenu.h"
#include "pushbttn.h"
#include "iterator.h"

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "rscarch.h"
#   include "callback.h"
#endif

defineClass(VDialog, VPopupWindow)
//
// revision 0   original
// revision 1   removed accels, iDefButton
//
defineArchiveRevision(VDialog, 1)

defineCallback(VDialog, okPressed)
defineCallback(VDialog, cancelPressed)

//----------------------------------------------------------------------------
VStyle& VDialog::getClassDefaultStyle(void)
{
#ifndef CVS_RATIONAL_DEFAULT
    static VWinStyle CV_NEARDATA 
	classDefaultStyle(VPopupWindow::getClassDefaultStyle() +
    setTransient + StyleCloseBox + StyleCaption);
#else
    static VWinStyle CV_NEARDATA 
	classDefaultStyle(VPopupWindow::getClassDefaultStyle() +
    setTransient + StyleCloseBox + StyleCaption + StyleModalBorder);
#endif

    return classDefaultStyle;
}

//----------------------------------------------------------------------------
VStyle& VDialog::getDefaultStyle(void) const
{
    return getClassDefaultStyle();
}

// ---------------------------------------------------------------------------
// The constructor used by VDialog subclasses generated by the mkdlg
// utility. It assumes 'x', 'y', 'w', and 'h' are in 'resource' coordinates. 
// NOTE: This method may not be portable.
//
VDialog::VDialog(int x, int y, int w, int h, VWindow *pWin, const VStyle& style)
{
    result = FALSE;
    long l = GetDialogBaseUnits();
    w = (w *LOWORD(l)) /4;
    h = (h *HIWORD(l)) /8;
    x = (x *LOWORD(l)) /4;
    y = (y *HIWORD(l)) /8;
    VDialog::stdSize(style, &w, &h);
    init();
    (*this)(VFrame(x, y, w, h), pWin, style);
	 realizeInit(0);
}

// ---------------------------------------------------------------------------
// Construct a VDialog with VFrame 'frame', parent window 'win'
// and title 'title'.
//
VDialog::VDialog(const VFrame &frame, VWindow *win, const char *title, const VStyle& style) 
        : result(0)
{
    init();
    (*this)(frame, win, style);
	 realizeInit(title);
}

// ---------------------------------------------------------------------------
VDialog::VDialog(const VString& wname, const VFrame &frame, VWindow *pwin, 
                    const char *title, const VStyle& style) 
        :   result(0)
{
    init();
    (*this)(frame, pwin, style, wname);
	 realizeInit(title);
}

// ---------------------------------------------------------------------------
// Construct a VDialog from the GUI system resource 's' with parent
// 'pWin'. NOTE: This method may not be portable.
//
VDialog::VDialog(const char *s, VWindow *pWin) 
        : result(0)
{
    parent = pWin;
    hWnd = 0;

	 init();

	 notifier->createDialog(this, s);
	 update(); 


	// This is a partial mapping of native styles established 
	// in the dialog editor to our abstract styles to get 
	// sizable dialogs to work. Note, we assume a default 
	// of non-sizable.

	long nativeS = GetWindowLong(hWnd, GWL_STYLE);

	if (((nativeS & WS_THICKFRAME) == WS_THICKFRAME)) {
		// Thickframe means we must allow sizing
#ifdef CVS_USE_TRANSIENT
		style += setTransient + StyleSizable;
#else
		style += StyleSizable;
#endif
	}

	if (((nativeS & WS_SYSMENU) == WS_SYSMENU)) {
#ifdef CVS_USE_TRANSIENT
		style += setTransient + StyleCloseBox;
#else
		style += StyleCloseBox;
#endif
	}


	 int x, y, w, h;
    getAbs(&x, &y, &w, &h);
    winFrame.set(x, y, w, h, CornerDim);
    fromRsc = 1;

    if (!hWnd) {
        if (HIWORD((long)s) == 0) { // see if MAKEINTRESOURCE was used
            print("dialog resource %d not found\n", (long)s);
        }
        else {
            print("dialog resource %s not found\n", s);
        }
        return;
    }

    unsigned n = (unsigned) SendMessage(hWnd, WM_GETTEXTLENGTH, 
                                                    (WPARAM) 0, (LPARAM) 0);

    if (n == 0xffff) {      // strange MS Windows bug
        n = 256;
    }

    char * tempbuf = new char[n+1];
    SendMessage(hWnd, WM_GETTEXT, (WPARAM)(n + 1), (LPARAM)tempbuf);
    sTitle = tempbuf;
    delete [] tempbuf;

    setSysMenu();
}

// ---------------------------------------------------------------------------
// Construct a VDialog with no presentation window.
//
VDialog::VDialog() 
        : result(0)
{
	init();
}

// ---------------------------------------------------------------------------
VDialog::~VDialog()
{
}

// ---------------------------------------------------------------------------
void VDialog::init(void)
{
    nativeDialog = FALSE;
#ifndef CVS_LATER_DEFAULT
    style = setTransient + getDefaultStyle();
#endif
}

// ---------------------------------------------------------------------------
void VDialog::realizeInit(const char *s)
{
    if (s)  {
        setTitle(s);
    }
    
    setSysMenu();
}

// ---------------------------------------------------------------------------
// Return GUI system dependent information
//
void VDialog::getInfo(VWinInfo &dialoginfo)
{
    VPopupWindow::getInfo(dialoginfo);

    dialoginfo.wClass = notifier->DLGwindow();

#ifdef CVS_RATIONAL_DEFAULT
	boolean rational = TRUE;
#else
	boolean rational = FALSE;
#endif	

	if (rational && style.contains(StyleModalBorder)) {
        dialoginfo.exStyle |= WS_EX_DLGMODALFRAME;
	}
    //!!!dss 9/19/95 - the or'ing of the contains is a hack until the 
    // contains/exclusive bug is fixed.
    // set a default border, if none is specified
    else if (rational ||
			 style.contains(StyleBorder) ||
			   style.contains(StyleModalBorder) ||
				style.contains(StyleSizable) ||
				style.contains(StyleCaption) ||
				style.contains(StyleCloseBox) ||
				style.contains(StyleMaximize) ||
				style.contains(StyleMinimize)) {

		dialoginfo.style |= DS_MODALFRAME;
		dialoginfo.style &= ~WS_VISIBLE;
	}
	else {
#ifdef CVS_USE_TRANSIENT
		style += setTransient + StyleModalBorder;
#else
        style += StyleModalBorder;
#endif
        dialoginfo.exStyle |= WS_EX_DLGMODALFRAME;
    }
}

// ---------------------------------------------------------------------------
// Terminate modal dialog operating mode and cause a return
// from the 'modal()' method call. 
// 
void VDialog::endModal()
{
    notifier->endModal();
}

// ---------------------------------------------------------------------------
// Set this VDialog into modal operating mode which disables
// mouse/keyboard events to all windows within the application except
// this VDialog.  This VDialog remains modal (that is, does not return)
// until endModal() is called.
//
void VDialog::modal()
{
    notifier->startModal(this);
}

// ---------------------------------------------------------------------------
// Add the width and height of the border of this VDialog to
// '*w' and '*h', respectively.
//
void VDialog::stdSize(const VStyle& style, int *w, int *h)
{
    VView::stdSize(style, w, h);
    if (style.contains(StyleModalBorder)) {
        *w += GetSystemMetrics(SM_CXDLGFRAME) *2;
        *h += GetSystemMetrics(SM_CYDLGFRAME) *2;
        *w += GetSystemMetrics(SM_CXBORDER) *2;
        *h += GetSystemMetrics(SM_CYBORDER) *2;
    } 
}

// ---------------------------------------------------------------------------
// Set the VSysMenu object associatied with the 'close box' of this
// VDialog to the minimal set, that is, remove all but the 'Close' and 'Move'
// functions.
//
void VDialog::setSysMenu()
{
    VSysMenu *sm = getSysMenu();
    if (sm) {
        sm->minSet(style.contains(StyleSizable));
    }
}

// ---------------------------------------------------------------------------
// Called when the user clicked an OK button. The default
// behavior is to close the dialog unsuccessfully. 
// The user is free to use this method as a callback for their OK button or
// to reimplement it in subclasses.
//
boolean VDialog::ok(VButton *)
{
    result = TRUE;

	if (notifier->isModalWindow(this))
      endModal();
	else
	  return VPopupWindow::close();

    return TRUE;
}

// ---------------------------------------------------------------------------
// Called when the user clicked a cancel button. The default
// behavior is to close the dialog unsuccessfully.
// The user is free to use this method as a callback or to reimplement it
// in subclasses.
//
boolean VDialog::cancel(VButton *)
{
    result = FALSE;

	if (notifier->isModalWindow(this))
      endModal();
	else
	  return VPopupWindow::close();

    return TRUE;
}


// ---------------------------------------------------------------------------
// Callback for OK button. The default
// behavior is to inherit the VView okPressed method. 
// 
//
boolean VDialog::okPressed(VButton *b)
{
    return VView::okPressed(b);
}

// ---------------------------------------------------------------------------
// Callback for Cancel button. The default
// behavior is to inherit the VView cancelPressed method.
//
boolean VDialog::cancelPressed(VButton *b)
{
    return VView::cancelPressed(b);
}

// ---------------------------------------------------------------------------
// Called when the user double-clicked on the close box. The default
// behavior is to close the dialog unsuccessfully.
// The user is free to use this method as a callback or to reimplement it
// in subclasses.
//
boolean VDialog::close()
{
    result = FALSE;

	if (notifier->isModalWindow(this))
      endModal();
	else
	  return VPopupWindow::close();

    return TRUE;
}

// ---------------------------------------------------------------------------
//
void VDialog::getSystemBorderSize(int *width, int *height) const
{
	VPopupWindow::getSystemBorderSize(width, height);
}

// ---------------------------------------------------------------------------
//
UINT VDialog::getCtlColor()
{
#if defined(CV_WIN32)
    return WM_CTLCOLORDLG;
#else
    return CTLCOLOR_DLG;
#endif
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//  Put this object to the archive.  This method is called from 
//  VArchiver::putObject().
//
void VDialog::putTo(VArchiver& a)
{
    VDialog::putTo(a, TRUE);
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
void VDialog::putTo(VArchiver& a, boolean most_derived)
{
    VPopupWindow::putTo(a, FALSE);          //  base object data

    a << VDialogArchiveTag;

    if (VDialogTagRev < 1) {
        a.putObject(accels);                //  accelerator set
        if (defButton != 0) {                   //  default button
            a << (getParent()->archiveChildrenOf()->indexOfSame(defButton) + 1);
        }
        else {
            a << 0;
        }
    }

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
void VDialog::getFrom(VArchiver& a)
{
    VPopupWindow::getFrom(a);                   //  base object data
    
    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VDialogArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VDialogArchiveTag));
        return;
    }

    // Archiver Rev 0 dialogs used to contain the accelerator set and the 
    // def push button. Now inherited from VWindow, so set the protected data.
    if (getArchiveTagRev(tag) < 1) {
        accels = (VSet *) a.getObject(0);
        a >> iDefButton;                            //  default button index    
    }
}

// --------------------------------------------------------------------------
//  Get this object from the archive.  The 'data' parameter is a VWindow *
//  that is our parent window.  In response, retrieve our object data,
//  realize the presentation space under the parent, and finally, retrieve
//  our children.
//
//  This method is invoked by VArchiver::getObject().
//
void VDialog::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VDialog::getFrom(a);                

    if (do_realize) {
        VDialog::realize((VWindow *)data);
        VString *name = getName();
        if (name == 0 || name->size() == 0) {
            setName("TheDialog");   //!!!jmd
        }
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
void VDialog::realize(VWindow *pwin)
{
    VPopupWindow::realize(pwin);
}

#endif //   ARCHIVER    
// ===========================================================================

void VDialog::setFont(VFont *f){
// Iterate thru all the children.
// If they donot have a font of there own, call their setFont.
//  with this dialog's font object (which is inherited from VWindow)
    VObject *p;

    if ( childrenOf() ){
        VIterator i(childrenOf());
        i.start();
        while (p = i++) {
            if (p->iam()->isa(VWindowCls)){
                if (!((VWindow*)p)->getFont()){
                    ((VWindow*)p)->setFont(f);
                }
            }
        }
    }
    winFont= f;
}
