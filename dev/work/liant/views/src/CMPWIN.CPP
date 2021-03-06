// $Header:   Q:/views/win/vcs/cmpwin.cpv   1.13   Jun 03 1997 14:15:18   scottt  $

//  cmpwin.cpp
//
//  Compound Document Window
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  01/30/95 dss    Initial release.
//  12/07/95 dgm    Overrode the other getFrom().
//  12/08/95 dss    getFrom() syntax correction.
//  12/08/95 dss    Added defineClass.
//  01/09/96 dss    VComponent name change.
//  01/31/96 dss    insertItem(): use VComponentSelect. Use new to construct
//                  VComponentItem instead of static create() method.
//  04/22/96 evc    Fixed ::insertItem so VComponentSelect has a 
//					VItemWindow instead	of a VControl as its third argument.
//					Fixes effix repaint bug.
// 04/30/96  evc    Added  insertItem(POINT pt, VItemWindow * pItemwin)
// 05 10/96  evc    Small change to ~VComponentWindow
// 06/14/96  evc    Added "((CRUnknown&)agent).Release();" to objDropped()
//					This gets rid of an assert about ref count on exit being 1.
// 06/19/96 evc     Added an update() call on getVThis() after an insert.
//					This cleans up the look of the inactive object.
//					Also gave focus to the window that holds 
//					componentitem after a drop.


//  ---------------------------------------------------------------------------

#include "pre.h"

#include "iterator.h"

#include "cmpslct.h"

defineClass(VComponentWindow, VObject)

//**********************************************************************
//
void VComponentWindow::init(void)
{
    // assert the Views object is there
// rhw 4/12/96: Commented this out.  The default constructor (used in
// archiving) will not have a getVThis() yet.
//    cv_assert(getVThis());

    // hook the Views GUI Window up to me, the Compound Document Window.
	// rhw 4/12/96: Had to guard the setting of the componentThis since
	// it won't have one when init'ing the default constructor.
    if (getVThis())
		getVThis()->setComponentThis(this);

    // !!!dss this is a test
    // CD aggregate parents are created with a ref count of one
//  AddRef();

    bDropAllowed = FALSE;
}

//**********************************************************************
//
VComponentWindow::VComponentWindow()
    : VComponentObject(0)
    , DropTarget(0)
    , OleWindow(0)
{
    CDBG(   << "In VComponentWindow's Default Constructor" << endl);

    init();
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
VComponentWindow::VComponentWindow(LPVWINDOW lpWindow)
    : VComponentObject(lpWindow)
    , DropTarget(0)     // late binding
    , OleWindow(new COleWindow(*this))
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentWindow's Constructor" << endl);

    init();
    
    aggregate(OleWindow);

    // !!!dss this is a test
    // CD aggregate parents are created with a ref count of one
    AddRef();
}

//**********************************************************************
//
VComponentWindow::VComponentWindow(LPVWINDOW lpWindow, COleWindow *pOleWindow)
    : VComponentObject(lpWindow)
    , DropTarget(0)     // late binding
    , OleWindow(pOleWindow)
{
    CDBG(   << "In VComponentWindow's Constructor" << endl);

    init();

}

//**********************************************************************
//
VComponentWindow::~VComponentWindow()
{
    CDBG(   << "In VComponentWindow's Destructor" << endl);

    // didn't call VComponentWindow::destroy()
    cv_assert(bDropAllowed == 0);

	if (getVThis()){
		getVThis()->setComponentThis(0);
	}

}

//**********************************************************************
//
COleWindow *VComponentWindow::getOleWindowBase(void)
{
    return OleWindow;
}

//**********************************************************************
//
// Must call this function instead of deleting the CD object or calling
// Release because allowDrops() calls OLE which does... what??? -dss 3/4/95
//
void VComponentWindow::destroy(void)
{
    allowDrops(FALSE);
    Release();
}

//**********************************************************************
//
// Called when a VComponentItem changes activation state.
//
boolean VComponentWindow::itemActivate(LPVCOMPONENTITEM lpItem)
{
    return getVThis()->getTopLevelParent()->getComponentThis()->itemActivate(lpItem);
}

//**********************************************************************
//
// Called when a VComponentItem changes activation state.
//
boolean VComponentWindow::itemDeactivate(LPVCOMPONENTITEM lpItem)
{
    return getVThis()->getTopLevelParent()->getComponentThis()->itemDeactivate(lpItem);
}

//**********************************************************************
//
boolean VComponentWindow::allowDrops(boolean bAllow)
{
    if (bAllow) {
        if (bDropAllowed == FALSE) {
            if (0 == DropTarget) {
                DropTarget = new CDropTarget(*this);
                // dss: an optimization is to not delete the DropTarget
                // and just unaggregate and reaggreate it.
                // This may involve artificial ref counting.
                aggregate(DropTarget);
            }

            // It is *REQUIRED* to hold a strong LOCK on the object that is
            // registered as drop target. this call will result in at least one
            // ref count held on our aggregate parent. Later, we will
            // unlock this lock which will make our aggregate parent's 
            // ref count go to 0. When the aggregate parent's ref count 
            // goes to 0, it will be deleted.
//          CoLockObjectExternal (getDropTarget(), TRUE, 0);
            getDropTarget().AddRef();

            // Register our window as a DropTarget
            HRESULT hResult = RegisterDragDrop(getVThis()->hwnd(), getDropTarget());
            cv_assert(SUCCEEDED(hResult));
            bDropAllowed = TRUE;

        }
    }
    else if (bDropAllowed) {
        HRESULT hResult = RevokeDragDrop(getVThis()->hwnd());
        cv_assert(SUCCEEDED(hResult));
        bDropAllowed = FALSE;

        unAggregate(DropTarget);
        
        // Unlock the lock. DropTarget will get released, which will 
        // release the aggregate parent, since it is the controlling unknown, 
        // which will make the aggregate parent's ref count go to 0, and 
        // the it will be deleted (-dss: YUK! what a round-about way of 
        // doing this).
        // -dss 3/12/95 I'm not sure this is true, anymore.
//      CoLockObjectExternal (getDropTarget(), FALSE, TRUE);
        // final release
        getDropTarget().Release();
        DropTarget = 0;
    }

    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::pasteItem(POINT pt)
{
    LPDATAOBJECT lpDataObject;

    HRESULT hResult = OleGetClipboard(&lpDataObject);

    if (SUCCEEDED(hResult) && lpDataObject) {
        LPVCOMPONENTITEM lpItem;

        lpItem = new VComponentItem(lpDataObject,
                                    VFrame(0.0F, 0.0F, 1.0F, 1.0F),
                                    *this,  
                                    StyleDefault,
                                    EmbeddedItem);

        if (lpItem) {
            lpItem->move(VOlePosition(pt, VOlePoint::Pixels));
        }
    }

    return SUCCEEDED(hResult);


}



//**********************************************************************
// This is a method that lets
// the caller (presumably a appview) choose the VItemWindow
// that eventually gets to parent the component item we are creating.
// That itemwindow will be the VThis in the componentwindow
// that is part of the componentitem we will create.
// This lets us is subclass this window
// and re-implement methods like takeFocus etc
//
boolean VComponentWindow::insertItem(POINT pt, VItemWindow * pItemwin)
{
    VComponentItem      *pItem = 0;

    VComponentSelect("Insert Component",
                     pItem,
                     *this,
					 pItemwin );
			  

    if (pItem) {
        pItem->move(VOlePosition(pt, VOlePoint::Pixels));
		pItem->getVThis()->update();
    }

    return pItem != 0;
}

//**********************************************************************
//
boolean VComponentWindow::insertItem(POINT pt)
{
    VComponentItem      *pItem = 0;

    VComponentSelect("Insert Component",
                     pItem,
                     *this,
              //new VControl( VFrame(0.0F, 0.0F, 1.0F, 1.0F), getVThis()));
			  // This VControl doesn't have correct styles EC
              new VItemWindow( VFrame(0.0F, 0.0F, 1.0F, 1.0F), getVThis()));
			  // This is one (of two) "componentwindows"
			  // associated with the "componentitem" that will be created.
			  // It's hwnd will be passed to the server and made the
			  // inplaceactive object window's parent.

    if (pItem) {
        pItem->move(VOlePosition(pt, VOlePoint::Pixels));
    }

    return pItem != 0;
}

//********************************************************************
//
// ********** VWindow events **********
//
//********************************************************************

//********************************************************************
//
boolean VComponentWindow::paint()
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::resized(int w, int h)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::moved(int w, int h)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::mouseDbl(int x, int y)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::mouseDn(int x, int y)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::mouseEnter(int buttonStatus)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::mouseExit(int buttonStatus)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::mouseMv(int x, int y, int buttonStatus)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::mouseUp(int x, int y)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::rmouseDbl(int x, int y)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::rmouseDn(int x, int y)
{
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::rmouseUp(int x, int y)
{
    return FALSE;
}

//**********************************************************************
//
void VComponentWindow::destroyed(void)
{
    // we must revoke the drop before this window is destroyed.
    allowDrops(FALSE);
}

//**********************************************************************
//
boolean VComponentWindow::close()
{
    CDBG(   << "In VComponentWindow::close" << endl);

#if 1
    VOrdCollect *lpKids = getVThis()->childrenOf();

    if (lpKids == 0) {
        return FALSE;
    }
    
    // TO DO: I think that VWindow::close() should do this work to allow
    // overridden close not to call VComponentWindow::close().
    // This implies that VWindow::close should be changed to broadcast
    // close to all its children. Alternatively, all the top level windows
    // could sub-class close() and do the broadcast. -dss 03/07/95
    DO (lpKids, VWindow, lpWin)
        VComponentWindow *lpCDWin = lpWin->getComponentThis();
        if (lpCDWin) {

            // if any of the kids override the close, stop and return
            // that fact.
            if (lpCDWin->close()) {
                return TRUE;
            }
        }
    END
#endif

    // Return FALSE to allow VWindow processing
    return FALSE;
}

//**********************************************************************
//
boolean VComponentWindow::quit()
{
    CDBG(   << "In VComponentWindow::quit" << endl);

#if 1
    VOrdCollect *lpKids = getVThis()->childrenOf();

    if (lpKids == 0) {
        return FALSE;
    }

    // TO DO: I think that VWindow::quit() should do this work to allow
    // overridden quit not to call VComponentWindow::quit().
    // This implies that VWindow::quit should be changed to broadcast
    // close to all its children. Alternatively, all the top level windows
    // could sub-class quit() and do the broadcast. -dss 03/07/95
    DO (lpKids, VWindow, lpWin)
        VComponentWindow *lpCDWin = lpWin->getComponentThis();
        if (lpCDWin) {
            // if any of the kids override the quit, stop and return
            // that fact.
            if (lpCDWin->quit()) {
                return TRUE;
            }
        }
    END
#endif

    // Return FALSE to allow VWindow processing
    return FALSE;
}

//**********************************************************************
//
//  Returns
//      TRUE - success, FALSE - failure
//
boolean VComponentWindow::connectStorage(void)
{
    CDBG(   << "In VComponentWindow::connectStorage" << endl);
    
    if (getStorage().open() == CancelButton) {
        return TRUE;
    }

    VOrdCollect *lpKids = getVThis()->childrenOf();

    if (lpKids == 0) {
        return TRUE;
    }

    DO (lpKids, VWindow, lpWin)
        VComponentWindow *lpCDWin = lpWin->getComponentThis();
        if (lpCDWin) {
            // if any of the kids fail, stop and return
            // that fact.
            if (!lpCDWin->connectStorage()) {
                return FALSE;
            }
        }
    END

    return TRUE;
}

//**********************************************************************
//
//  Returns
//      TRUE - success, FALSE - failure
//
boolean VComponentWindow::disconnectStorage(void)
{
    CDBG(   << "In VComponentWindow::disconnectStorage" << endl);

    VOrdCollect *lpKids = getVThis()->childrenOf();

    if (lpKids != 0) {
        DO (lpKids, VWindow, lpWin)
            VComponentWindow *lpCDWin = lpWin->getComponentThis();
            if (lpCDWin) {
                // if any of the kids fail, stop and return
                // that fact.
                if (!lpCDWin->disconnectStorage()) {
                    return FALSE;
                }
            }
        END
        
    }

    if (getStorage().close(TRUE, FALSE) == CancelButton) {
        return TRUE;
    }
    
    return TRUE;
}

//**********************************************************************
//
//  Returns
//      TRUE - success, FALSE - failure
//
boolean VComponentWindow::save(void)
{
    CDBG(   << "In VComponentWindow::save" << endl);

    VOrdCollect *lpKids = getVThis()->childrenOf();

    // Save the kids FIRST! This allows the sub-storage saves to
    // trickle up to the root.
    
    // TO DO: re-think this. This assumes the storage of parent 
    // CDWindows are always higher in the storage hierarchy. This
    // is only the default (if the CDObject creates its storage.
    // The programmer may apply a different configuration.
    if (lpKids != 0) {
        DO (lpKids, VWindow, lpWin)
            VComponentWindow *lpCDWin = lpWin->getComponentThis();
            if (lpCDWin) {
                // if any of the kids fail, stop and return
                // that fact.
                if (!lpCDWin->save()) {
                    return FALSE;
                }
            }
        END
        
    }
    
    // Save the parent last.
    return getStorage().save() == YesButton;
}

//**********************************************************************
//
// Returns
//      TRUE - if the drop is accepted
//      FALSE - if the drop is ignored/not accepted
//
boolean VComponentWindow::objDropped(const CRDataObject& agent,
                              const VOlePosition &pos, 
                              DWORD pdwEffect)
{
    LPVCOMPONENTITEM lpItem;

    lpItem = new VComponentItem((LPDATAOBJECT) & agent.getUnknown(),
                                VFrame(0.0F, 0.0F, 1.0F, 1.0F),
                                *this,  
                                StyleDefault,
                                pdwEffect == DROPEFFECT_LINK ? LinkedItem : EmbeddedItem);
	((CRUnknown&)agent).Release();
    lpItem->move(pos);

	getVThis()->getParent()->setChildFocus(getVThis());
    return TRUE;
}

// ============================================================================
#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
//
void VComponentWindow::putTo(VArchiver &a)
{
    a << bDropAllowed;
} // putTo()

// ---------------------------------------------------------------------------
//
void VComponentWindow::getFrom(VArchiver& a)
{
    a >> bDropAllowed;
} // getFrom()

// ---------------------------------------------------------------------------
//
// rhw 4/12/96: Had to give the name "parent" to the second argument
// for use in method.
void VComponentWindow::getFrom(VArchiver& a, VObject *parent, boolean)
{
    VComponentWindow::getFrom(a);

	// rhw 4/12/96: Had to set the ComponentThis of the parent
	lpVObject = parent;
	((VWindow*) parent)->setComponentThis(this);
}

#endif // #ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
boolean VComponentWindow::isDirty(void)
{
    return getStorage().isDirty();
}                                  
