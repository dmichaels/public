// $Header:   Q:/views/win/vcs/cmpview.cpv   1.8   Mar 21 1997 08:42:44   gregm  $

//	cdview.cpp
//
//  Compound Document View
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
//  01/30/95 dss    Initial release.
//  12/08/95 dss    getFrom() syntax correction.
//  12/08/95 dss    Added defineClass.
//	01/09/96 dss	VComponent name change.
//	01/22/96 dss	init: onClipboard.
//	--------------------------------------------------------------------------

#include "pre.h"

#include "fileslct.h"
#include "yesno.h"

defineClass(VComponentView, VComponentWindow)

//**********************************************************************
//
void VComponentView::init()
{
	bAllowLocalActivatation = TRUE;
}

//**********************************************************************
//
VComponentView::VComponentView()
	: OleInPlaceUIWindow(0)
{
    CDBG(   << "In VComponentView's Default Constructor" << endl);

    init();
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
VComponentView::VComponentView(LPVVIEW lpView)
    : VComponentWindow(lpView, new COleInPlaceUIWindow(*this))
//  , OleInPlaceUIWindow()
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentView's Constructor" << endl);

    init();

    OleInPlaceUIWindow = (COleInPlaceUIWindow *) getOleWindowBase();

    aggregate(OleInPlaceUIWindow);

    // !!!dss this is a test
    // CD aggregate parents are created with a ref count of one
    AddRef();
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
VComponentView::VComponentView(LPVVIEW lpView, VString *path, VComponentObject *storageParent)
    : VComponentWindow(lpView, new COleInPlaceUIWindow(*this))
//  , OleInPlaceUIWindow()
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentView's Constructor" << endl);

    init();

    OleInPlaceUIWindow = (COleInPlaceUIWindow *) getOleWindowBase();

    aggregate(OleInPlaceUIWindow);

    // !!!dss this is a test
    // CD aggregate parents are created with a ref count of one
    AddRef();
    
    createStorage(storageParent, path);
}

//**********************************************************************
//
VComponentView::VComponentView(LPVVIEW lpView, COleInPlaceUIWindow *pOleInPlaceUIWindow)
    : VComponentWindow(lpView, pOleInPlaceUIWindow)
    , OleInPlaceUIWindow(pOleInPlaceUIWindow)
{
    CDBG(   << "In VComponentView's Constructor" << endl);

    init();
  
    // TO DO: do we want to aggregate sub-classes
//  aggregate(OleInPlaceUIWindow);
}

//**********************************************************************
//
VComponentView::VComponentView(LPVVIEW lpView, COleInPlaceUIWindow *pOleInPlaceUIWindow,
                 VString *path, VComponentObject *storageParent)
    : VComponentWindow(lpView, pOleInPlaceUIWindow)
    , OleInPlaceUIWindow(pOleInPlaceUIWindow)
{
    CDBG(   << "In VComponentView's Constructor" << endl);

    init();
  
    // TO DO: do we want to aggregate sub-classes
//  aggregate(OleInPlaceUIWindow);

    createStorage(storageParent, path);
}

//**********************************************************************
//
VComponentView::~VComponentView()
{
    CDBG(   << "In VComponentView's Destructor" << endl);
}

//********************************************************************
//
LPVCOMPONENTITEM VComponentView::getFocusedComponentItem()
{
    LPVWINDOW lpWin = getVThis()->getChildFocus();

	// Sanjay Start
	// Added to find in-place-active insertable window
    VOrdCollect *lpKids = getVThis()->childrenOf();
    if (lpKids != 0)
	{
    	DO (*(getVThis()->childrenOf()), VWindow, child)
        	if (child->isHidden()) {
        		VComponentWindow   *lpCDWin = child->getComponentThis();
        		if (lpCDWin && !LPVCOMPONENTITEM(lpCDWin)->isActiveXControl())//)lpCDWin->isA(VComponentItemCls)
           			lpWin = child;
        	}
    	END
	}
	// Sanjay End

    while (lpWin) {
        VComponentWindow   *lpCDWin = lpWin->getComponentThis();
        if (lpCDWin && lpCDWin->isA(VComponentItemCls)) {
            return LPVCOMPONENTITEM(lpCDWin);
        }
        
        lpWin = lpWin->getChildFocus();
    }

    return NULL;
}

//**********************************************************************
//
boolean VComponentView::allowLocalActivation(boolean bAllow)
{
	boolean tmp = bAllowLocalActivatation;
	bAllowLocalActivatation = bAllow;
	return tmp;
}

//**********************************************************************
//
// Called when a VComponentItem changes activation state.
//
boolean VComponentView::itemActivate(LPVCOMPONENTITEM lpItem)
{
    return getVThis()->getAppLevelParent()->getComponentThis()->itemActivate(lpItem);
}

//**********************************************************************
//
// Called when a VComponentItem changes activation state.
//
boolean VComponentView::itemDeactivate(LPVCOMPONENTITEM lpItem)
{
    return getVThis()->getAppLevelParent()->getComponentThis()->itemDeactivate(lpItem);
}

//**********************************************************************
//
boolean VComponentView::activate(VView *v)
{
    // always return FALSE to let the OS process the deactivation:
    // we are using this message as a notification, not overriding it.
    return FALSE;
}

//**********************************************************************
//
boolean VComponentView::deactivate(VView *v)
{
    // always return FALSE to let the OS process the deactivation:
    // we are using this message as a notification, not overriding it.
    return FALSE;
}

//**********************************************************************
//
boolean VComponentView::resized(int w, int h)
{
    getOleInPlaceUIWindow().resized( w, h);

    return FALSE;
}

//**********************************************************************
//
boolean VComponentView::mouseDn(int x, int y)
{
    LPVCOMPONENTITEM       pCDItem;

    // deactivate the active item, if any.
// Sanjay Start
	LPOLEINPLACEOBJECT	lpOleInPlaceObject; 
    if ((pCDItem = getFocusedComponentItem()) && pCDItem->isActive() && 
    	(pCDItem->isActiveXControl() || !pCDItem->canLocallyActivate())) {
		pCDItem->getOleObject().QueryInterface(IID_IOleInPlaceObject, (LPVOID FAR*) &lpOleInPlaceObject);
        if(lpOleInPlaceObject)
        	lpOleInPlaceObject->UIDeactivate();
	}
    else if ((pCDItem = getFocusedComponentItem()) && pCDItem->isActive() && 
    	(!pCDItem->isActiveXControl() && pCDItem->canLocallyActivate())) {
		pCDItem->getOleObject().QueryInterface(IID_IOleInPlaceObject, (LPVOID FAR*) &lpOleInPlaceObject);
        if(lpOleInPlaceObject)
        	lpOleInPlaceObject->UIDeactivate();
// Start Old Code
    //if ((pCDItem = getFocusedComponentItem()) && pCDItem->isActive()) {
        //pCDItem->getOleInPlaceSite().getOleInPlaceObject().UIDeactivate();
// End Old Code
// Sanjay End
    }

    return TRUE;
}

//********************************************************************
//
boolean VComponentView::createStorage(VComponentObject *storageParent,
                               VString *lpStorageName)
{
    VComponentObject   *cdTop;
    
    // if no storage specified, use the appview parent as storage
    if (storageParent == 0) {
        // find the appView.
        cdTop = getVThis()->getAppLevelParent()->getComponentThis();

        // If the appView has no storage, create root storage
        if (!cdTop->hasStorage()) {
            cdTop = 0;
        }
    }

    // else use the storage specified on the call.
    else {
        cdTop = storageParent;
    }

    return VComponentWindow::createStorage(cdTop, lpStorageName);
}

//**********************************************************************
//
boolean VComponentView::close()
{
    CDBG(   << "In VComponentView::close" << endl);

    VPathString         newName;
    enum yesNoCancel    querySaveResult;

    querySaveResult = querySave(newName);

    // if cancel, then return cancel
    if (querySaveResult == CancelButton) {
        return TRUE;
    }
                     
    // if we're overridden, pass it on.
    // NOTE: close() does close processing on the kids.

    // TO DO: an optimization: if querySaveResult is NoButton, then
    // all the kids don't have to save, but they don't know this,
    // so they are saving anyway. CHANGE THIS! Perhaps a save state
    // in the parent that that the kids could look at.
    if (VComponentWindow::close()) {
        return TRUE;
    }

    // Now that all the kids have had a chance to save, save ourself
    if (querySaveResult == YesButton) {

        // if a name was given, rename the storage.
        if (newName.size() > 0) {
            // if error return not done.
            if (!renameStorage(newName)) {
                return TRUE;
            }
        }
        
        // else just do save
        else if (!save()) {
            return TRUE;
        }
    }
    
    return FALSE;
}

//**********************************************************************
//
boolean VComponentView::quit()
{
    CDBG(   << "In VComponentView::quit" << endl);

    VPathString         newName;
    enum yesNoCancel    querySaveResult;
                            
    // query the user. We want all the user interface up front, BEFORE
	// we recursively save the views children. This allow the user to
    // cancel and still have the children unaffected by a close/save
    // operation.
    querySaveResult = querySave(newName);

    // if cancel, then return cancel
    if (querySaveResult == CancelButton) {
        return TRUE;
    }
                     
    // if we're overridden, pass it on.
    // NOTE: quit() does close processing on the kids.
    
    // TO DO: an optimization: if querySaveResult is NoButton, then
    // all the kids don't have to save, but they don't know this,
    // so they are saving anyway. Perhaps a save state
    // in the parent that that the kids could look at.
    if (VComponentWindow::quit()) {
        return TRUE;
    }
    
    // Now that all the kids have had a chance to save, save ourself
    if (querySaveResult == YesButton) {

        // if a name was given, rename the storage.
        if (newName.size() > 0) {
            // if error return not done.
            if (!renameStorage(newName)) {
                return TRUE;
            }
        }
        
        // else just do save
        else if (!save()) {
            return TRUE;
        }
    }
    
    return FALSE;
}

//**********************************************************************
//
// Returns:
//      TRUE - if succeeded, FALSE - if failed
//
boolean VComponentView::renameStorage(VPathString& newName)
{
    boolean     renamed;
                                 
    // if no name, then return not done (and continue processing).
    if (newName.size() == 0) {
        return TRUE;
    }

    if (!disconnectStorage()) {
        VString     title;
        getVThis()->getTitle(title);
        
        VReport::warning("Save Failure", getVThis()->getAppLevelParent(),
                         "Could not save %s:\n%s.", 
                         title.gets(),
                         getStorage().getResult().asUserMessage().gets());
        return FALSE;
    }
                                 
    // rename the storage, now that its closed.
    // Save the result because we want to re-connect the storage, in case
    // the rename failed. We should be reconnected to the original
    // storage because the storage name won't change on rename failure.
    renamed = (getStorage().rename(newName) == YesButton);
    
    if (!renamed) {
        VReport::warning("Save Failure", getVThis()->getAppLevelParent(),
                         "Could not save to %s:\n%s.",
                         newName.gets(),
                         getStorage().getResult().asUserMessage().gets());
    }
    
    if (!connectStorage()) {
        VString     title;
        getVThis()->getTitle(title);
        
        VReport::warning("Save Failure", getVThis()->getAppLevelParent(),
                         "Could not save %s:\n%s.", 
                         title.gets(),
                         getStorage().getResult().asUserMessage().gets());
        return FALSE;
    }

    return renamed;
}

//**********************************************************************
//
// Queries the user to save
//
//  Returns:
//      YesButton - do the save
//      NoButton - don't do the save
//      CancelButton - don't do the save and user is requesting cancel.
//
enum yesNoCancel VComponentView::querySave(VPathString& newName)
{
    // if top level storage is dirty, prompt user for save
    if (hasStorage() && getStorage().isRoot() && getStorage().isDirty()) {
        enum yesNoCancel    result;
        VString             title;

        getVThis()->getTitle(title);
        
        result = VYesNo::question(TRUE, 
                                YesButton, 
                                "Save",
                                getVThis()->getAppLevelParent(),
                                "Do you want to save changes to %s?",
                                  title.gets());
                                                
        if (result == CancelButton) {
            return CancelButton;
        }
        
        else if (result == YesButton) {
            boolean     rename = FALSE;
            
            if (getStorage().isTemporary()) {
                VString     filter("All Files (*.*)|*.*");
                int         index = 0;
                
                rename = VFileSelect::dialog(
                                    VFileSelect::DlgStyle(VFileSelect::SaveAs | 
                                                          VFileSelect::ConfirmReplace),
                                    "Save As",
                                    newName,
                                    getVThis()->getAppLevelParent(),
                                    &filter,
                                    &index);                                                

                // return cancel if cancelled                           
                if (!rename) {
                    return CancelButton;
                }
                                    
                cv_assert(newName.size() > 0);
            } // if temp
                           
            return YesButton;
            
        } // else yesButton
    } // has dirty storage
    
    return NoButton;
}   

//**********************************************************************
//
//  Returns:
//      TRUE - saved
//      FALSE - didn't save
//
boolean VComponentView::save()
{
    CDBG(   << "In VComponentView::save" << endl);
    
    if (!hasStorage() || !getStorage().isRealized()) {
        return TRUE;
    }
     
    if (isDirty()) {
        if (getStorage().isTemporary()) {
            
            // if cancel return cancel.
            if (!saveInto()) {
                return FALSE;
            }
            
        } // if temp
        
        // ...else we're persistent, so just save
        else {
            return VComponentWindow::save();
        } // else persistent
        
    } // if dirty
    
    // not dirty, so just return save OK
    return TRUE;
}

//**********************************************************************
//
//  Returns:
//      TRUE - saved
//      FALSE - didn't save
//
boolean VComponentView::saveInto(void)
{
    CDBG(   << "In VComponentView::saveInto" << endl);
    
    if (!hasStorage() || !getStorage().isRealized()) {
        return TRUE;
    }
     
    boolean         saved;
    VPathString     newName;
    
    // if cancel return cancel.
    if (!(saved = saveAs(newName))) {
        return FALSE;
    }
            
#if 0
    // if error return cancel.
    if (!renameStorage(newName)) {
        return FALSE;
    }
        
#else
    // TO DO: do you have to save the children since putTo's imply 
    // individual OleSave's into the archive
    // To Do: should we just Commit all the sub-storage's and then
    // do the recursive IStorage:CopyTo() to the ArchiveByte's IStorage.
    // How do we instantiate the CDItem from this?

    // save the children.
    // if error, return error
    if (!VComponentWindow::save()) {
        return FALSE;
    }

    VRscArchiver        a(newName);

    // open resource file
    if (saved = a.open(VArchiver::Create)) {
        // TO DO: note: must use getComponentThis() to archive the VView portion. This is
        // compile time bound using class inheritance but since we use object 
        // inheritance, the binding is loose. Find a way to inform/prevent the 
        // programmer from doing a putObject(this) instead of a putObject(getComponentThis).

        // To DO: can we archive objects with dup names? If not we need to handle
        // this situation.
        VString     title;
        getVThis()->getTitle(title);
        
        // Save self to resource. If failed...
        saved = a.putObject(title, getVThis());

        a.close();
    }
#endif  
    
    // return save failed
    return saved;
}

//**********************************************************************
//
//  Returns:
//      TRUE - new name available
//      FALSE - cancel
//
boolean VComponentView::saveAs(VPathString& newName)
{
    CDBG(   << "In VComponentView::saveAs" << endl);
    
    if (!hasStorage() || !getStorage().isRealized()) {
        return TRUE;
    }
    
    VString     filter("All Files (*.*)|*.*");
    int         index = 0;

    return VFileSelect::dialog(
                        VFileSelect::DlgStyle(VFileSelect::SaveAs | 
                                            VFileSelect::ConfirmReplace),
                        "Save As",
                        newName,
                        getVThis()->getAppLevelParent(),
                        &filter,
                        &index);
}

// ============================================================================
#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
//
void VComponentView::putTo(VArchiver &a)
{
    VComponentWindow::putTo(a);
} // putTo()

// ---------------------------------------------------------------------------
//
void VComponentView::getFrom(VArchiver& a)
{
    VComponentWindow::getFrom(a);
} // getFrom()

// ---------------------------------------------------------------------------
//
void VComponentView::getFrom(VArchiver& a, VObject *, boolean)
{
	VComponentWindow::getFrom(a);
}

#endif // #ifndef CV_NOARCHIVER
