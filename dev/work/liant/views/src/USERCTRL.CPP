// $Header:   Q:/views/common/vcs/userctrl.cpv   1.10   May 30 1997 17:17:08   scottt  $ 

//  userctrl.cpp
//
//	VUserControl class implementation [Common]
//
//	Allegris Foundation 1.1.00
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
//  10/05/92 jmd    created
//  10/13/92 jmd    added userClassName to archive
//  02/01/94 dgm    minor change to get rid of warnings
//  02/02/94 pmcm   added archive tag check
//  02/09/94 pmcm   updated defineArchiveTag use
//  11/11/94 dss    Archiver tags macros.
//  05/25/95 pkt    preened.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#include "userctrl.h"
#include "ordcllct.h"
#include "port.h"

#ifndef CV_NOARCHIVER
#	include "notifier.h"
#   include "archiver.h"
#   include "cllbckls.h"
#endif

extern "C" {
#   undef NULL
#   include <string.h>
}

defineClass(VUserControl, VControl)
defineArchiveRevision(VUserControl, 1)

// ---------------------------------------------------------------------------
// Construct a VUserControl with no presentation window.
//
VUserControl::VUserControl() 
{ 
    ;
}

// ---------------------------------------------------------------------------
//
VUserControl::~VUserControl() 
{ 
    ;
}

// ---------------------------------------------------------------------------
//
boolean VUserControl::paint()
{
    VString tmp;
    tmp.prints("User Class:\n%s", userClassName.gets());

    VPort port(this);
    port.open();

    int ww, wh;
    sizeOfImage(&ww, &wh);

    VRectangle box(CornerDim, 0, 0, ww, wh);

//  port.wrtText(tmp.gets(), &box, JustifyCenter | JustifyVcenter);
    port.wrtText(tmp.gets(), &box, WordBreak);

    port.close();

    return FALSE;
}

// ===========================================================================
#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VUserControl::putTo(VArchiver& a)
{
    VUserControl::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//  Archive this VUserControl object and, if most derived, its children.
//
//  Type:       What:
//                      VControl base class object data
//
void VUserControl::putTo(VArchiver& a, boolean most_derived)
{
    VControl::putTo(a, FALSE);              //  base object data

    a << VUserControlArchiveTag;

    a.putObject(&userClassName);

    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

// ---------------------------------------------------------------------------
//
void VUserControl::getFrom(VArchiver& a)
{
    VControl::getFrom(a);                       //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VUserControlArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VUserControlArchiveTag));
        return;
    }

    a.getObject(&userClassName);
}

// ---------------------------------------------------------------------------
//
void VUserControl::getFrom(VArchiver& a,
                           VObject *parent_window, boolean do_realize)
{
    VUserControl::getFrom(a);

    if (do_realize) {
        VUserControl::realize((VWindow *)parent_window);
    }

    if (parent_window != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// ---------------------------------------------------------------------------
//
void VUserControl::realize(VWindow *parent_window)
{
    VControl::realize(parent_window);
}

// ---------------------------------------------------------------------------
//
boolean VUserControl::loadResources(VRscArchiver& a)
{
	// Don't create user control if this is the painter
	if (notifier->getPainterMode() == FALSE) {
	    if (getParent() == 0 || userClassName == "") {
	        return TRUE;
	    }
	
	    VClass* cls = VClass::of(userClassName.gets());
	
	    if (cls != 0) {
	        VWindow *newwin;
	        newwin = (VWindow *) cls->construct();
	    
	        if (newwin == 0 || !newwin->isA(VWindowCls)) {
	            delete newwin;
	            return TRUE;
	        }
	    
	        newwin->realize(getParent());
	    
	        if (newwin->isA(VControlCls)) {
	            // get control attributes
	            VString t;
	            getText(t);
	            ((VControl *) newwin)->putText(t);
	            ((VControl *) newwin)->setClient(getClient());
	            ((VControl *) newwin)->setIdNo(getIdNo());
	        }
	    
	        getParent()->replaceChild(this, newwin);
	    
	        // note: 'this' has been deleted
	    }
	}

    return TRUE;
}

#endif // ARCHIVER
// ===========================================================================

