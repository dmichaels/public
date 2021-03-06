// $Header:   Q:/views/win/vcs/ITEMWIN.CPv   1.7   Mar 04 1997 14:12:48   gregm  $

//  itemwin.cpp
//
//  Compound Document Window
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
//  02/13/95 dss    Initial release.
//  04/22/96 evc    Incorrect initializer causes VWindow's styles to
//                  be used CreateWin is called.  Should use
//                  VItemWindow's styles.
//  ---------------------------------------------------------------------------

#include "pre.h"

#include "itemwin.h"

defineClass(VItemWindow, VWindow)

//**********************************************************************
//
VItemWindow::VItemWindow()
{
}

//**********************************************************************
//
VItemWindow::VItemWindow(const VFrame& frame, VWindow *parent, const VStyle& style)
    //: VWindow (frame, parent, style)
{
(*this) (frame, parent, style);
}

//**********************************************************************
//
VItemWindow::~VItemWindow()
{
}

//**********************************************************************
//
void VItemWindow::getInfo(VWinInfo &info)
{
    VWindow::getInfo(info);
    info.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

// ===========================================================================
#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VItemWindow::putTo(VArchiver& a)
{
    VItemWindow::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//  Archive this VItemWindow object and, if most derived, its children.
//
//  Type:       What:
//                      VWindow base class object data
//
void VItemWindow::putTo(VArchiver& a, boolean most_derived)
{
    // contrary to most archivals
    // put derived object data before base object data

//    a << VItemWindowArchiveTag;

    a << 0L;

    a.putObject(&userClassName);

    VWindow::putTo(a, FALSE);              //  base object data

    if (most_derived) {
        VWindow::putChildrenTo(a);          //  children
    }
}

// ---------------------------------------------------------------------------
//
void VItemWindow::getFrom(VArchiver& a)
{
    // contrary to most archivals
    // get derived object data before base object data

    long tag;
    a >> tag;
#if 0
    if (!archiveTagOk(tag, VItemWindowArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VItemWindowArchiveTag));
        return;
    }
#endif

    a.getObject(&userClassName);

    // Don't create user component if this is the painter
    if (notifier->getPainterMode() == FALSE) {
        // before calling VWindow::getFrom
        // check to see if user class is available
        if (userClassName.size() != 0) {
            // construct component object of owner class and set
            VClass* cls = VClass::of(userClassName.gets());
            if (cls != 0 && (cls->isa(VComponentWindowCls))) {
                VComponentWindow* componentWin = (VComponentWindow*) cls->construct();
                setComponentThis(componentWin);
            }
        }
    }

    VWindow::getFrom(a);                    //  base object data
}

// ---------------------------------------------------------------------------
//
void VItemWindow::getFrom(VArchiver& a,
                           VObject *parent_window, boolean do_realize)
{
    VItemWindow::getFrom(a);

    if (do_realize) {
        VItemWindow::realize((VWindow *)parent_window);
    }

    if (parent_window != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// ---------------------------------------------------------------------------
//
void VItemWindow::realize(VWindow *parent_window)
{
    VWindow::realize(parent_window);
	// Sanjay Start
    LPVCOMPONENTITEM lpCDItem = LPVCOMPONENTITEM(getComponentThis());
	DWORD miscStatus;
	HRESULT hResult = lpCDItem->getOleObject().GetMiscStatus(DVASPECT_CONTENT, &miscStatus);
	if(SUCCEEDED(hResult) && lpCDItem->isActiveXControl())
	{
		if((miscStatus & OLEMISC_INVISIBLEATRUNTIME) || (miscStatus & OLEMISC_NOUIACTIVATE))
		{
			// Set in-place site window style to be invisible
    		if (notifier->getPainterMode() == TRUE) {
				build(StyleHidden);
				hide();
			}
		}
    }
//    if ((notifier->getPainterMode() == TRUE) && lpCDItem->canLocallyActivate())
//	    lpCDItem->setActivationState(VComponentItem::locallyActive);
	// Sanjay End
}

#endif // ARCHIVER
// ===========================================================================
