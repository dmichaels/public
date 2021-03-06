// $Header:   Q:/views/win/vcs/stdmdivw.cpv   1.12   Mar 04 1997 17:55:52   gregm  $ 

//  stdmdivw.cpp
//
//	VStdMdiView implementation
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
//  03/30/93 jmd    created
//  07/20/93 jmd    integrated david's many changes
//  07/26/93 dgm    removed realize().
//  08/08/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  02/18/94 dgm    fixed non-default constructor to create presention space.
//  08/16/94 dgm    Added archiver support; added operator();
//                  add do_realize to VMdiAppView(); added initialize().
//  08/25/94 dgm    Fix in realize().
//  11/11/94 dss    Archiver tags macros.
//  09/01/95 dss    VStyle changes.
//	11/01/95 dgm	Added #ifndef CV_NOOLE's.
//	01/09/96 dss	VComponent name change.
// ----------------------------------------------------------------------------

#include "stdmdivw.h"
#include "stdmdiap.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// Compound Document Objects
#ifndef CV_NOOLE
#include "cmpview.h"
#endif

// ---------------------------------------------------------------------------
//
defineClass(VStdMdiView, VMdiView)
defineArchiveRevision(VStdMdiView, 0)

// ---------------------------------------------------------------------------
VStdMdiView::VStdMdiView()
{
    initialize();
}

// ---------------------------------------------------------------------------
VStdMdiView::VStdMdiView(const VFrame& f, VStdMdiAppView *win, 
                         const VStyle& style)
    : VMdiView()
{
    initialize();
    (*this)("", f, win, style, "");
}

// ---------------------------------------------------------------------------
VStdMdiView::VStdMdiView(const VString& name,
                         const VFrame& f, VStdMdiAppView *win, 
                         const VStyle& style)
    : VMdiView()
{
    initialize();
    (*this)("", f, win, style, "", name);
}

// ---------------------------------------------------------------------------

void VStdMdiView::initialize()
{
    mainWin = 0;
    menuBar = 0;
    toolBar = 0;
    statusBar = 0;
    isClosed = FALSE;
}

// ---------------------------------------------------------------------------
VStdMdiView::~VStdMdiView()
{
    if (!isClosed && getMainWin()) {
        isClosed = TRUE;
        getMainWin()->childClosed(this, TRUE);
    }
}

// ---------------------------------------------------------------------------
void VStdMdiView::operator()(const char *iconname,
                             const VFrame& frame, VStdMdiAppView *win,
                             const VStyle& style, char *title)
{
    mainWin = win;

    if (getMainWin()) {
        getMainWin()->childCreated(this);
    }

    VMdiView::operator()(iconname, frame, win, style, title);
}

// ---------------------------------------------------------------------------
void VStdMdiView::operator()(const char *iconname,
                             const VFrame& frame, VStdMdiAppView *win,
                             const VStyle& style, char *title,
                             const VString& name)
{
    mainWin = win;

    if (getMainWin()) {
        getMainWin()->childCreated(this);
    }

    VMdiView::operator()(iconname, frame, win, style, title, name);
}

// ---------------------------------------------------------------------------
// activate(): the MDI window has been activated
//
boolean VStdMdiView::activate(VView *m)
{
    if (getMainWin()) {
        getMainWin()->childActive(this);
    }
#ifndef CV_NOOLE
    if (getComponentThis()) {
        getComponentThis()->activate(m);
    }
#endif  
    return TRUE;
}

// ---------------------------------------------------------------------------
// deactivate(): the MDI window has been deactivated
//
boolean VStdMdiView::deactivate(VView *m)
{
#ifndef CV_NOOLE
    if (getComponentThis()) {
        getComponentThis()->deactivate(m);
    }
#endif  
    return TRUE;
}

// ---------------------------------------------------------------------------
// the user has closed the window; notify the main window
//
boolean VStdMdiView::close()
{
#ifndef CV_NOOLE
    if (getComponentThis()) {
        // if CDThis overrides close, then return abort close.
        if (getComponentThis()->close()) {
            return TRUE;
        }
    }
#endif

    if (!isClosed && getMainWin()) {
        isClosed = TRUE;
        getMainWin()->childClosed(this, TRUE /* tony/george fix */ /*FALSE*/);
    }

    return FALSE;
}

// ---------------------------------------------------------------------------
// classActive() a window of our class-type has become active 
//
void VStdMdiView::classActive()
{
}

// ---------------------------------------------------------------------------
// classInactive() a window of our class-type has become inactive 
//
void VStdMdiView::classInactive()
{
}

// ---------------------------------------------------------------------------
// instanceActive() this window has become active 
// (if window is of a different class than previous window, classActive()
// is called first)
//
void VStdMdiView::instanceActive()
{
}

// ---------------------------------------------------------------------------
// instanceInactive() this window has become inactive 
// (if next window is of a different class than this, classInactive()
// is called afterwards)
//
void VStdMdiView::instanceInactive()
{
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VStdMdiView::putTo(VArchiver& a)
{
    VStdMdiView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VStdMdiView::putTo(VArchiver& a, boolean most_derived)
{
    // Write the base object data.

    VMdiView::putTo(a, FALSE);

    // Write the object version tag.

    a << VStdMdiViewArchiveTag;

    // Write any object specific data.
    
    ;

    // Write any children.

    if (most_derived) {
        VWindow::putChildrenTo(a);
    }
}

// ---------------------------------------------------------------------------
//
void VStdMdiView::getFrom(VArchiver& a)
{
    // Read the base object data.

    VMdiView::getFrom(a);

    // Read object version tag and verify.

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VStdMdiViewArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VStdMdiViewArchiveTag));
        return;
    }

    // Read any object specific data.

    ;
}

// ---------------------------------------------------------------------------
//
void VStdMdiView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VStdMdiView::getFrom(a);

    if (do_realize) {
        VStdMdiView::realize((VWindow *)parent);
    }

    VWindow::getChildrenFrom(a, do_realize);
}

// ---------------------------------------------------------------------------
//
void VStdMdiView::realize(VWindow *parentw)
{
    if ((parentw != 0) && parentw->isA(VStdMdiAppViewCls)) {
        mainWin = (VStdMdiAppView *)parentw;
    }
    VMdiView::realize(parentw);
    if (mainWin != 0) {
        mainWin->childCreated(this);
    }
}

#endif // ARCHIVER
