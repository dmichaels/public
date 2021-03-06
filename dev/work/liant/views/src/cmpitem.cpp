// $Header:   Q:/views/win/vcs/cmpitem.cpv   1.30   11 Aug 1997 15:23:26   GREGM  $

//  cditem.cpp
//
//  Compound Document Item
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
//  12/09/94 dss    Initial release.
//  11/03/95 mw     Commented out calls to DebugOutputEnumFmtEtc (compile
//                  error.
//  11/06/95 dss    Removed comments on calls to DebugOutputEnumFmtEtc.
//  12/04/95 dss    Remove gets().
//  01/09/96 dss    VComponent name change.
//  01/22/96 dss    onClipboard: now a VComponentItem data member.
//  01/31/96 dss    removed insertSelf(). Replaced static create()'s with
//                  realize(). Replace different item attribute enums with
//                  ItemStyle enum. InitObject changed to initFromNative.
//  02/06/96 dss    realize(): fixed typo
//  04/29/96 evc    Removed a few of Russ's tentative changes mostly
//                  in initFromNative.  His persistence stuff is still here
//  05/07/96 evc	Fix to doVerb that a problem  when one grabs the hatched
//					boarder of a UI active document.
//  05/10/96 evc    Replaced putToX and getFromX with versions supplied by
//					Ray (contractor.)
//					Made a change to getFrom that lets one supply
//					A component enabled window as a parent to
//					rscarch::getObject.
//					Fixed addverb (so the menu supplied by server is
//					"bound" to the VMenuItem or VPopup we convert it to
//					Delete the file used for structured storage during
//					destruction
// 05/16/96  evc    Fixed problem archiving a componentItem more that once.
//					Solution got rid of file based IStorage  in favor
//					of a total in-memory solution based on Rays implementation
//					using CreateILockBytesOnHGlobal .
// 06/07/96  evc    Added a few virtual functions:
//						VComponentItem::sizeCtoV
//                      VComponentItem:: uponInplaceObjectMoveOrResize
//						VComponentItem::uponDataAdvise()
//						VComponentItem::uponViewAdvise()
// 06/12/96  evc    A fix to the borland 5.0 inspired removal of
// 					conversion operators.  N.B. that getOleObject() no
// 					longer converts to LPOLEOBJECT.
//					See call to OleDraw in paint for details.

// Also be aware that when we want to
// get an LPSTORAGE pointer from the VComponentObject::getStorage() method
// thats called alot inside VComponentItem we say,:
// "(LPSTORAGE) & getStorage().getStorage().getUnknown()."  The first getStorage
// gives us a VComponentStorage reference.
// Then the VComponentStorage::getStorage() returns a CRStorage&  which
// has a getUnknown () method that returns a CRUNKNOWN& . Now we take the
// address of that and we have the LPUNKNOWN which is also a
// LPSTORAGE pointer.
//
// 06/18/96  evc     Just a little cleanup of REALLY dead code.
//                   Fixed the resize for inactive objects.
//  --------------------------------------------------------------------------

#include "pre.h"

// Views Includes
#include "notifier.h"
#include "port.h"

// Component Includes
#include "cmpitem.h"
#include "archbyte.h"
#include "cmpcntl.h"

defineClass(VComponentItem, VComponentWindow)
// Start of Sanjay's Code
defineArchiveRevision(VComponentItem, 0)
// End of Sanjay's Code

#define IDM_VERB0       1000
#define HANDLE_SIZE     6

VComponentItem *VComponentItem::onClipboard = 0;

//**********************************************************************
//
void VComponentItem::init()
{
    dwDrawAspect = DVASPECT_CONTENT;
    bCanActivateLocally = TRUE; // can activate remotely
    activationState = inactive;

    lpOleObjectVerbMenu = 0;

    timer = 0;

    // drag/drop source
    bLocalDrag = FALSE;         // is doc source of the drag
    bPendingDrag = FALSE;       // LButtonDown--possible drag pending
    bDragLeave = FALSE;         // has drag left
    ptButDown.x = ptButDown.y = 0; // LButtonDown coordinates

    inFocus = FALSE;

    itemStyle = (ItemStyle) 0;

    bCanBeEmbeddedItem = TRUE;
    bCanBeLinkedItem = TRUE;

    bCanBeEmptyItem = TRUE;
    bCanBeFileItem = TRUE;
    bCanBeDataItem = TRUE;
    bCanBeStaticItem = TRUE;
// Start of Sanjay's Code
	bIsActiveXControl = FALSE;
// End of Sanjay's Code

    OleAdvisory = 0;
    DataAdvisory = 0;

    classId = CLSID_NULL;

    // rhw 3/21/96: Temporary until I figure out david's storage scheme
    m_lpStorage = 0;
	m_lpLockBytes = 0;

//  style = StyleDefault;
//  style = StyleBorder | StyleSizable;
//  style = StyleBorder;
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.  This warning
                                // tells us that we are passing this in
                                // an initializer, before "this" is through
                                // initializing.  This is ok, because
                                // we just store the ptr in the other
                                // constructors
VComponentItem::VComponentItem()
    : VComponentWindow(0)      // late binding
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's default constructor, this = " << this << endl);

    init();

    // rhw 3/20/96: Add this here since all the other constructors did this
    // and I was getting an assert about a duplicate IOleWindow interface.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.  This warning
                                // tells us that we are passing this in
                                // an initializer, before "this" is through
                                // initializing.  This is ok, because
                                // we just store the ptr in the other
                                // constructors
VComponentItem::VComponentItem(const VString& objectType,
                               const VFrame& frame,
                               VComponentWindow& CDParent,
                               const VStyle& style,
// Start of Sanjay's Code
                               boolean isActiveXControl)
// End of Sanjay's Code
//  : VComponentWindow(new VControl())
    : VComponentWindow(new VItemWindow())
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's aggregate constructor, this = " << this << endl);

    init();

// Start of Sanjay's Code
	bIsActiveXControl = isActiveXControl;
// End of Sanjay's Code

    // late GUI construction so the VWindow::addChild() gets called
    // AFTER VComponentItem construction.
    getVThis()->operator()(frame, CDParent.getVThis(), style);
// Start of Sanjay's Code
	getVThis()->hide();
// End of Sanjay's Code

    // VComponentWindow aggregated an OleWindow interface. So does OleInPlaceSite.
    // unexpose the VComponentWindow interface so that the one in OleInPlaceSite
    // is used. Duplicates are not allowed.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);

    if (objectType != "") {
        realize(objectType);
    }
// Start of Sanjay's Code
	getVThis()->show();
// End of Sanjay's Code
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.  This warning
                                // tells us that we are passing this in
                                // an initializer, before "this" is through
                                // initializing.  This is ok, because
                                // we just store the ptr in the other
                                // constructors
VComponentItem::VComponentItem(const VString& objectType,
                               VComponentWindow& CDParent,
                               VWindow& win,
// Start of Sanjay's Code
                               boolean isActiveXControl)
// End of Sanjay's Code
    : VComponentWindow(&win)
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's aggregate constructor, this = " << this << endl);

    init();

// Start of Sanjay's Code
	bIsActiveXControl = isActiveXControl;
// End of Sanjay's Code

    // VComponentWindow aggregated an OleWindow interface. So does OleInPlaceSite.
    // unexpose the VComponentWindow interface so that the one in OleInPlaceSite
    // is used. Duplicates are not allowed.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);

    if (objectType != "") {
        realize(objectType);
    }
}

//----------------------------------------------------------------------------
#pragma warning(disable : 4355)
VComponentItem::VComponentItem(const VString& objectType,
                               VWindow& win,
// Start of Sanjay's Code
                               boolean isActiveXControl)
// End of Sanjay's Code
    : VComponentWindow(&win)
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's aggregate constructor, this = " << this << endl);

    init();

// Start of Sanjay's Code
	bIsActiveXControl = isActiveXControl;
// End of Sanjay's Code

    // VComponentWindow aggregated an OleWindow interface. So does OleInPlaceSite.
    // unexpose the VComponentWindow interface so that the one in OleInPlaceSite
    // is used. Duplicates are not allowed.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);

    if (objectType != "") {
        realize(objectType);
    }
}
//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.  This warning
                                // tells us that we are passing this in
                                // an initializer, before "this" is through
                                // initializing.  This is ok, because
                                // we just store the ptr in the other
                                // constructors
VComponentItem::VComponentItem(const LPDATAOBJECT lpDataObj,
                               const VFrame& frame,
                               VComponentWindow& CDParent,
                               const VStyle& style,
                               int itemStyle,
// Start of Sanjay's Code
                               boolean isActiveXControl)
// End of Sanjay's Code
    : VComponentWindow(new VItemWindow())
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's aggregate constructor, this = " << this << endl);

    init();

// Start of Sanjay's Code
	bIsActiveXControl = isActiveXControl;
// End of Sanjay's Code

    // late GUI construction so the VWindow::addChild() gets called
    // AFTER VComponentItem construction.
    getVThis()->operator()(frame, CDParent.getVThis(), style);

    // VComponentWindow aggregated an OleWindow interface. So does OleInPlaceSite.
    // unexpose the VComponentWindow interface so that the one in OleInPlaceSite
    // is used. Duplicates are not allowed.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);

    realize(lpDataObj);
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.  This warning
                                // tells us that we are passing this in
                                // an initializer, before "this" is through
                                // initializing.  This is ok, because
                                // we just store the ptr in the other
                                // constructors
VComponentItem::VComponentItem(const VPathString& storage,
                               VComponentWindow& CDParent,
                               VWindow& win,
                               int itemStyle,
// Start of Sanjay's Code
                               boolean isActiveXControl)
// End of Sanjay's Code
    : VComponentWindow(&win)
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's aggregate constructor, this = " << this << endl);

    init();

// Start of Sanjay's Code
	bIsActiveXControl = isActiveXControl;
// End of Sanjay's Code

    // VComponentWindow aggregated an OleWindow interface. So does OleInPlaceSite.
    // unexpose the VComponentWindow interface so that the one in OleInPlaceSite
    // is used. Duplicates are not allowed.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);

    realize(storage);
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.  This warning
                                // tells us that we are passing this in
                                // an initializer, before "this" is through
                                // initializing.  This is ok, because
                                // we just store the ptr in the other
                                // constructors
VComponentItem::VComponentItem(const VPathString& storage,
                               const VFrame& frame,
                               VComponentWindow& CDParent,
                               const VStyle& style,
                               int itemStyle,
// Start of Sanjay's Code
                               boolean isActiveXControl)
// End of Sanjay's Code
    : VComponentWindow(new VItemWindow())
    , AdviseSink(new CAdviseSink(*this))
    , OleClientSite(new COleClientSite(*this))
    , OleInPlaceSite(new COleInPlaceSite(*this))
    , DropSource(new CDropSource(*this))
    , OleObject(0)      // late binding
#pragma warning (default : 4355)  // Turn the warning back on
{
    CDBG(   << "In VComponentItem's aggregate constructor, this = " << this << endl);

    init();

// Start of Sanjay's Code
	bIsActiveXControl = isActiveXControl;
// End of Sanjay's Code

    // late GUI construction so the VWindow::addChild() gets called
    // AFTER VComponentItem construction.
    getVThis()->operator()(frame, CDParent.getVThis(), style);

    // VComponentWindow aggregated an OleWindow interface. So does OleInPlaceSite.
    // unexpose the VComponentWindow interface so that the one in OleInPlaceSite
    // is used. Duplicates are not allowed.
    unexposeInterface(IID_IOleWindow);

    aggregate( AdviseSink);
    aggregate( OleClientSite);
    aggregate( OleInPlaceSite);
    aggregate( DropSource);

    realize(storage);
}

//**********************************************************************
//
VComponentItem::~VComponentItem()
{
    CDBG(   << "In VComponentItem's Destructor" << endl);

    unloadItem(TRUE);

	//  This can be revisited when we track the OLE
	//  contribution to a menu.  FOr now, if the item were

    if (lpOleObjectVerbMenu) {
        delete lpOleObjectVerbMenu;
        lpOleObjectVerbMenu = 0;
    }

    if (hasOleObject()) {
        getOleObject().Release();
        OleObject = 0;
    }

    if (onClipboard == this) {
        deleteFromClipboard();
        onClipboard = 0;
    }

    // must do this otherwise recursion occurs when VWindow::clean() tries
    // to release it CDObject.
    getVThis()->setComponentThis(0);

    // rhw 4/15/96: Added this temporary storage implementation which should
    // be replaced by the appropriate code to release the storage in the
    // final storage implementation.
    if (m_lpStorage)
        m_lpStorage->Release();

    // !!!DSS: this is strange: deleting the Gui object
//  delete getComponentThis();
}

//**********************************************************************
//
// Creates an item from a data object
//
boolean VComponentItem::realize(LPDATAOBJECT lpDataObj, int itemStyle)
{
    cv_assert(!isRealized());

    // CD aggregeate parent's are created with a ref count of one
//  lpCDItem->AddRef();

    if (!createStorage()) {
        return FALSE;
    }


    DebugOutputEnumStatStg(lpDataObj);
    DebugOutputEnumFmtEtc(lpDataObj);
    DebugOutputEnumStatStg((LPSTORAGE) &(getStorage().getStorage().getUnknown()));

    LPOLEOBJECT     lpOleObject;
    HRESULT         hResult;

    if ((itemStyle & LinkedItem) && canBeLinkedItem()) {
        hResult = OleCreateLinkFromData(
                    lpDataObj,
                    IID_IOleObject,
                    OLERENDER_DRAW,
                    NULL,
                    getOleClientSite(),
                    (LPSTORAGE) &(getStorage().getStorage().getUnknown()),
                    (LPVOID FAR *)&lpOleObject);
    }
    else {

        cv_assert(itemStyle & EmbeddedItem);

        hResult = OleCreateFromData (
                    lpDataObj,
                    IID_IOleObject,
                    OLERENDER_DRAW,
                    NULL,
                    getOleClientSite(),
                    (LPSTORAGE) &(getStorage().getStorage().getUnknown()),
                    (LPVOID FAR *)&lpOleObject);


    }

    if (FAILED(hResult)) {
        goto errExit;
    }

    if (!initFromNative(lpOleObject, EmbeddedItem | DataSource)) {
        goto errExit;
    }

	DebugOutputEnumStatStg((LPSTORAGE) &(getStorage().getStorage().getUnknown()));

    LPDATAOBJECT    pNewDataObj;

    pNewDataObj = 0;
    getOleObject().QueryInterface( IID_IDataObject, (LPVOID FAR*) &pNewDataObj);

    // test after new data obj creation
    DebugOutputDataObj(lpDataObj);
    DebugOutputDataObj(pNewDataObj);
    DebugOutputEnumFmtEtc(pNewDataObj);

    // tell newly created ole object to cache its CF_TEXT data, if any.
    // Initialize the cache with data from the dropped object.
    hResult = getOleObject().Cache(lpDataObj, CF_TEXT);
    if (FAILED(hResult)) {
        goto errExit;
    }

    // test after cache
    DebugOutputDataObj(lpDataObj);
    DebugOutputDataObj(pNewDataObj);
    DebugOutputEnumFmtEtc(pNewDataObj);

    pNewDataObj->Release();

#if 0
    if (canRemotelyActivate()) {
        createOleInPlaceSite();
    }
#endif

	// Sanjay Start
    if ((notifier->getPainterMode() == TRUE) && bCanActivateLocally)
	    setActivationState(locallyActive);
	// Sanjay End

    return TRUE;

errExit:
    if (hasStorage()) {
        getStorage().undo();
        // TO DO: do I have to release the storage
    }

    if (hasOleObject()) {
        lpOleObject->Release();
        lpOleObject = 0;
    }

    return FALSE;
}

//**********************************************************************
//
// Creates a item from a progId
//
boolean VComponentItem::realize(const VString& progId)
{
    // rhw 4/15/96:  Added these variables to make debugging easier (i.e. can't
    // view variables involved in the "get" macros)
    LPOLECLIENTSITE lpOleClientSite = getOleClientSite();
    LPOLEOBJECT     lpOleObject = 0;

    cv_assert(!isRealized());

    if (!createStorage()) {
        return FALSE;
    }

    CLSID   clsid;
    OLEResult hResult;

    hResult = CLSIDFromProgID(progId, &clsid);

    if (FAILED(hResult)) {
        goto errExit;
    }

    // rhw 4/15/96: This is the core of my temporary storage solution.
	// If storage has
    // not been created yet, create some here

    if (!m_lpStorage)
    {
		hResult = CreateILockBytesOnHGlobal(NULL, TRUE, &m_lpLockBytes);

		if (hResult != NOERROR) return FALSE;

		cv_assert(m_lpLockBytes);

		hResult = StgCreateDocfileOnILockBytes(m_lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,
		0,
		&m_lpStorage);

        if (FAILED(hResult)) {
       // Temporary debugging code.  This code gets an english description for
       // the previous error.
            VString result("Unknown Error");
            LPVOID  lpMessageBuffer;

#if defined (CV_WIN32)
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL,
                        hResult,
                        0x409,  // Later, replace with VLocale
                        (LPTSTR) &lpMessageBuffer,
                        0,
                        NULL))
            {
                result = (LPTSTR) lpMessageBuffer;

                LocalFree(lpMessageBuffer);
            }
#else
    result = "The operation failed. Details are unavailable.\r\n";

#endif

            CDBG(   << "In VComponentItem::getFromX, StgOpenStorage() failed: "
                    << result << endl);

            return FALSE;
        }
    }

	/*
    DWORD    advf = ADVF_PRIMEFIRST;
    hResult = OleCreateEx(clsid,
        	              IID_IOleObject,
            	          OLERENDER_DRAW,
						  1,
						  &advf,
						  NULL,
						  NULL,
						  lpOleClientSite,
                          m_lpStorage,
                          (LPVOID FAR *)&lpOleObject);
	*/
#if defined(CV_WIN32)
	try
	{
#endif
    	hResult = OleCreate(clsid,
        	                IID_IOleObject,
            	            OLERENDER_DRAW,
                	        NULL,
                    	    lpOleClientSite,
// rhw 4/15/96: for debugging,
// I use this instead of getOleClientSite()
// rhw 4/15/96:
// Temporarily commented this out.
// Put back when final storage solution is ready
//                      getStorage(),
                        	m_lpStorage,
                        	(LPVOID FAR *)&lpOleObject);
#if defined(CV_WIN32)
	}
	catch(...)
	{
		// TODO: Internationalize
		VString msg0 = "Warning";
		VString msg1 = "This control is not currently supported by Allegris. Please refer to the release notes."; // g_XMsg->getMsg("ocxhnd17");
		VReport::warning(msg0.gets(), getVThis()->getAppLevelParent(), msg1.gets());
    	return FALSE;
	}
#endif

    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "OleCreate failed: " << hResult.asSystemMessage() << endl);
        goto errExit;
    }


	// We used to include "EmptySource".  Should be revisited. EC
    if (!initFromNative(lpOleObject, EmbeddedItem )) {
        goto errExit;
    }

	// Sanjay Start
    if ((notifier->getPainterMode() == TRUE) && bCanActivateLocally)
	    setActivationState(locallyActive);
	// Sanjay End

    return TRUE;

errExit:
    if (hasStorage()) {
        getStorage().undo();
        // TO DO: do I have to release the storage?
    }

    if (hasOleObject()) {
        lpOleObject->Release();
        lpOleObject = 0;
    }

    return FALSE;
}

//**********************************************************************
//
// Create an item from storage
//
boolean VComponentItem::realize(const VPathString& path, int itemStyle)
{
    cv_assert(!isRealized());

    if (!createStorage()) {
        return FALSE;
    }

    HRESULT         hResult;
    LPOLEOBJECT     lpOleObject;

    if (itemStyle & LinkedItem) {
        hResult = OleCreateLinkToFile(path,
                                      IID_IOleObject,
                                      OLERENDER_DRAW,
                                      NULL,
                                      getOleClientSite(),
                                      (LPSTORAGE) &(getStorage().getStorage().getUnknown()),
                                      (LPVOID FAR *)&lpOleObject);
    }
    else {
        hResult = OleCreateFromFile(CLSID_NULL,
                                    path,
                                    IID_IOleObject,
                                    OLERENDER_DRAW,
                                    NULL,
                                    getOleClientSite(),
                                    (LPSTORAGE) &(getStorage().getStorage().getUnknown()),
                                    (LPVOID FAR *)&lpOleObject);
    }

    if (FAILED(hResult)) {
        goto errExit;
    }

    if (!initFromNative(lpOleObject, itemStyle & ~(EmptySource | DataSource) | FileSource)) {
        goto errExit;
    }

	// Sanjay Start
    if ((notifier->getPainterMode() == TRUE) && bCanActivateLocally)
	    setActivationState(locallyActive);
	// Sanjay End

    return TRUE;

errExit:
    if (hasStorage()) {
        getStorage().undo();
        // TO DO: do I have to release the storage?
    }

    if (hasOleObject()) {
        lpOleObject->Release();
        lpOleObject = 0;
    }

    return FALSE;
}

//**********************************************************************
//
boolean VComponentItem::isRealized(void)
{
    // are all my references to external interfaces realized?
    return hasOleObject() && getOleObject().isRealized();
}

//**********************************************************************
//
boolean VComponentItem::createStorage(void)
{
    VComponentWindow   *lpCDTopLevelParent = getVThis()->getTopLevelParent()->getComponentThis();

    //!!!dss When I leave off the scoping operator, MS compiler complains about
    // createStorage() not taking 2 args. It should be finding the overloaded
    // createStorage() through inheritance. This needs to be looked into.
    return VComponentWindow::createStorage(lpCDTopLevelParent, 0);
}

//**********************************************************************
//
// Name: closeItem
//
// Purpose: Close the native object and close the associated storage,
// if it is realized. Optionally
//
//********************************************************************

boolean VComponentItem::closeItem(boolean bSave)
{
    CDBG(   << "In VComponentItem::closeItem("
            << (bSave ? "SAVE" : "NOSAVE")
// rhw 3/20/96: getStorage() is not being set correctly, skip use for now
            << "), closing " /*<< getStorage().getStorageName()*/ << endl);
    HRESULT hResult = 0;

    if (hasOleObject()) {
// rhw 3/20/96: getStorage() is not being set correctly, skip use for now
//        cv_assert(hasStorage() && getStorage().isRealized());

// rhw 4/11/96:  Until the final storage fix, we will just close without saving
// and rely on the invokation of putTo to persist the control's contents.
        hResult = getOleObject().Close(OLECLOSE_NOSAVE);
//        hResult = getOleObject().Close(bSave ? OLECLOSE_SAVEIFDIRTY : OLECLOSE_NOSAVE);
//        if (GetScode(hResult) == OLE_E_PROMPTSAVECANCELLED) {
//            result = TRUE;
//        }

        LPPERSISTSTORAGE    lpPersistStorage = 0;
        HRESULT result;
        result = getOleObject().QueryInterface(IID_IPersistStorage,
                                                (LPVOID*)&lpPersistStorage);
        if (SUCCEEDED(result)) {
            lpPersistStorage->HandsOffStorage();
            lpPersistStorage->Release();
        }

        if (m_lpStorage)
        {
            m_lpStorage->Release();
            m_lpStorage = 0;
        }
    }

    // if we're not cancelled, and we have storage, then close the storage.
    // if there's no ole object, somehow we still have storage, so close it
    // to tie up loose ends.
    if (!SUCCEEDED(hResult) && hasStorage()) {
        // Since the ole object already saved when it closed, don't
        // bother saving, again. If we're a temporary object, then
        // don't keep the storage.
// rhw 3/20/96: getStorage() is not being set correctly, skip use for now
//        getStorage().close(FALSE, getStorage().isTemporary());
    }

    // return cancel (TRUE), if canceled, else return FALSE,
    // even if save error.
    return !SUCCEEDED(hResult);
}

//**********************************************************************
//
// Name: unloadItem
//
// Purpose:
//
//      Close and release all pointers to the object of the VComponentItem
//
// Parameters:
//
//      None
//
// Return Value:
//
//      None
//
//********************************************************************

boolean VComponentItem::unloadItem(boolean bSave)
{
    CDBG(   << "In VComponentItem::unloadItem("
            << (bSave ? "SAVE" : "NOSAVE")
// rhw 3/20/96: getStorage() is not being set correctly yet, just comment
// it out below
            << "), unloading " /*<< getStorage().getStorageName()*/ << endl);

    HRESULT hResult;

    if (!hasOleObject()) {
        return FALSE;
    }

    if (closeItem(bSave)) {    // ensure object is closed; NOP if already closed
        return TRUE;
    }

    // remove the advisories
    {
        LPVIEWOBJECT lpViewObject;

        hResult = getOleObject().QueryInterface(IID_IViewObject, (LPVOID FAR *)&lpViewObject);
        cv_assert(SUCCEEDED(hResult));

        if (lpViewObject)
            {
            // Remove the view advise
            hResult = lpViewObject->SetAdvise(dwDrawAspect, 0, NULL);
            cv_assert(SUCCEEDED(hResult));
            lpViewObject->Release();
            }

        if (OleAdvisory) {
            hResult = getOleObject().Unadvise(OleAdvisory);
            cv_assert(SUCCEEDED(hResult));
            OleAdvisory = 0;
        }

        if (DataAdvisory) {
            LPDATAOBJECT        lpDataObject;
            hResult = getOleObject().QueryInterface(IID_IDataObject,
                    (LPVOID FAR *)&lpDataObject);
            cv_assert(SUCCEEDED(hResult));

            if (lpDataObject) {
                hResult = lpDataObject->DUnadvise(DataAdvisory);
                cv_assert(SUCCEEDED(hResult));
                DataAdvisory = 0;
                lpDataObject->Release();
            }
        }

    }

    if (onClipboard == this) {
        deleteFromClipboard();
        onClipboard = 0;
    }

    // Release the object.
    // This is the last lock, so set the object pointer to NULL,
    // which will make hasOleObject() return FALSE.
    getOleObject().Release();
    OleObject = 0;

    return FALSE;
}

//**********************************************************************
//
// VComponentItem::initFromNative
//
// Purpose:
//
//      Used to initialize a newly create VCOmponentItem from an
//      Ole Object.
//
// Returns:
//
//      TRUE if successfull, else FALSE.
//
//**********************************************************************


boolean VComponentItem::initFromNative(LPOLEOBJECT lpOleObject, int itemStyle)
{
    LPVIEWOBJECT2   lpViewObject2;
    OLEResult       hResult;

    CDBG(   << "In VComponentItem::initFromNative()" << endl);

    this->OleObject = new CROleObject(lpOleObject);

    this->itemStyle = (ItemStyle) itemStyle;

    VComponentAppView  *lpCDApp = getVThis()->getAppLevelParent()->getComponentThis();

#if 0
// rhw 3/20/96: getStorage.getStorage() is a problem, lookinto this later
// Russ had trouble and so do I.  Ed C.
    lpOleObject->SetMoniker(OLEWHICHMK_CONTAINER,
                            getStorage().getStorage().
                            getMoniker(OLEGETMONIKER_FORCEASSIGN, OLEWHICHMK_CONTAINER));
#endif

    // Set a View Advise
    lpOleObject->QueryInterface(IID_IViewObject2,(LPVOID FAR *)&lpViewObject2);
    hResult = lpViewObject2->SetAdvise(dwDrawAspect, ADVF_PRIMEFIRST, getAdviseSink());
    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "SetAdvise failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }

    // Set a Object Advise (for linking)
    hResult = lpOleObject->Advise(getAdviseSink(), &OleAdvisory);
    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "Advise failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }

    // initialize to 1" x 1" in case of failure
    SIZEL       sizel = {1 * HIMETRIC_PER_INCH, 1 * HIMETRIC_PER_INCH};
    // get the initial size of the object
    hResult = lpViewObject2->GetExtent(dwDrawAspect, -1 /*lindex*/,
                                       NULL /*ptd*/, &sizel);
    lpViewObject2->Release();
    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "GetExtent failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }

// Start of Sanjay's Code
	int x, y, w, h;
	VRectangle r;
	getVThis()->getWindowRect(r);
	r.get(Corners, &x, &y, &w, &h);
	move(VOlePosition(x, y, VOlePoint::Pixels));
    size(VOleSize(sizel,VOlePoint::MM100ths));
// Start of Old Code
    // set the initial size
// End of Old Code
// End ofSanjay's Code

    LPOLESTR   lpName;

    hResult = lpOleObject->GetUserType(USERCLASSTYPE_FULL, &lpName);

    if (lpName) {
        itemLongName = lpName;
        OleStdFree(lpName);
    }

    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "GetUserType(USERCLASSTYPE_FULL) failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }

    hResult = lpOleObject->GetUserType(USERCLASSTYPE_SHORT, &lpName);
    if (lpName) {
        itemShortName = lpName;
        OleStdFree(lpName);
    }

    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "GetUserType(USERCLASSTYPE_SHORT) failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }

    hResult = lpOleObject->GetUserClassID(&classId);

    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "GetUserClassID() failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }

    LPDATAOBJECT    lpDataObject = 0;

    hResult = lpOleObject->QueryInterface(IID_IDataObject,
                                          (LPVOID FAR *)&lpDataObject);

// Start of Sanjay's Code
    if (FAILED(hResult) && !isActiveXControl()) {
        CDBG(   << ws(2) << "QueryInterface(IID_IDataObject) failed: " << hResult.asSystemMessage() << endl);
        return FALSE;
    }
    else if (FAILED(hResult) && isActiveXControl()) {
		goto cont;
    }
// Start of old code
    //if (FAILED(hResult)) {
    //    CDBG(   << ws(2) << "QueryInterface(IID_IDataObject) failed: " << hResult.asSystemMessage() << endl);
    //    return FALSE;
    //}
// End of old code
// End of Sanjay's Code

	// Start of Sanjay's Code
	DWORD miscStatus;
	hResult = getOleObject().GetMiscStatus(dwDrawAspect, &miscStatus);
	if(SUCCEEDED(hResult) && !isActiveXControl())
	{
		if(!(miscStatus & OLEMISC_RECOMPOSEONRESIZE))
		bCanActivateLocally	= FALSE;
		goto cont;
    }
	// End of Sanjay's Code

    // Find a format for the Data Change Adivsory.
    LPENUMFORMATETC         enumFmtEtc;
    hResult = lpDataObject->EnumFormatEtc(DATADIR_GET, &enumFmtEtc);
    if (OLE_S_USEREG == GetScode(hResult)) {
        hResult = OleRegEnumFormatEtc(getClassID(), DATADIR_GET, &enumFmtEtc);
    }

    if (FAILED(hResult)) {
        CDBG(   << ws(2) << "OleRegEnumFormatEtc() failed: " << hResult.asSystemMessage() << endl);
		// Sanjay Start
		goto cont;
		// Old Start
        //return FALSE;
		// Old End
		// Sanjay End
    }

    FORMATETC       FmtEtc;
    if (enumFmtEtc) {
#if defined(CV_DEBUG)
        CDBG(   << ws(2) << "Enumerating GET formats for "
                    << getName(ItemLongName) << endl);
        while (enumFmtEtc->Next(1, &FmtEtc, NULL) == S_OK) {
            DebugOutputFormatEtc(&FmtEtc);
        }

        enumFmtEtc->Reset();
#endif
        while ((hResult = enumFmtEtc->Next(1, &FmtEtc, NULL)) == S_OK) {
            // take the first content aspect with a flat storage meduium.
            // From experience, it seems that advisories are more likely
            // to succeed on flat storage mediums. We want content because
            // that's what the user changes and we want to know when then
            // the user changes the content.
            if (FmtEtc.dwAspect == DVASPECT_CONTENT &&
                (FmtEtc.tymed == TYMED_HGLOBAL ||
                 FmtEtc.tymed == TYMED_MFPICT)) {
                break;
            }
        }

        enumFmtEtc->Release();

        // assert that the above while loop found a proper FmtEtc.
        if (FAILED(hResult)) {
            CDBG(   << ws(2) << "enumFmtEtc failed: " << hResult.asSystemMessage() << endl);
            return FALSE;
        }

        // Set a Data Advise. ADVF_NODATA because we only want to know when the data
        // changes, not what has changed.
        hResult = lpDataObject->DAdvise(&FmtEtc, ADVF_NODATA, getAdviseSink(),
                                        &DataAdvisory);
        if (FAILED(hResult)) {
            CDBG(   << ws(2) << "DAdvise failed: " << hResult.asSystemMessage() << endl);
        }
    }
    if (lpDataObject) {
        lpDataObject->Release();
    }

    if (FAILED(hResult)) {
        return FALSE;
    }

// Start of Sanjay's Code
cont:
// End of Sanjay's Code

    VString appTitle("");
    VString docTitle("");

    VAppView* alp = getVThis()->getAppLevelParent();

	if (alp && alp->isA(VAppViewCls)) {
		alp->getTitle(appTitle);
	}

    // ((VView*)getVThis()->getTopLevelParent())->getTitle(docTitle);

    // give the object the name of the container app/document
    // TO DO: do you want to name the object from IOleObject::GetType()?
    lpOleObject->SetHostNames(appTitle, docTitle);

    // inform object handler/DLL object that it is used in the embedding container's context
    OleSetContainedObject(lpOleObject, TRUE);

    if (itemStyle & EmptySource) {
       // force new object to save to guarantee valid object in our storage.
       // OLE 1.0 objects may close w/o saving. this is NOT necessary if the
       // object is created FROM FILE; its data in storage is already valid.
       getOleClientSite().SaveObject();

       // we only want to DoVerb(SHOW) if this is an InsertNew object.
       // we should NOT DoVerb(SHOW) if the object is created FromFile.
       doVerb(OLEIVERB_SHOW);
    }

    return TRUE;
}

//**********************************************************************
//
// VComponentItem::paint
//
// Purpose:
//
//      Paints the object
//
// Parameters:
//
// Return Value:
//
//********************************************************************

boolean VComponentItem::paint()
{

    // don't paint if UI active
    if (isLocallyActive()) {
        return FALSE;
    }

    VRectangle   updateRect;

    BOOL bExposures = notifier->getExposedRectangle(updateRect);

    cv_assert(bExposures);

    RECT updRect;
    RECT itemRect;

    // need to check to make sure there is a valid object
    // available.  This is needed if there is a paint msg
    // between the time that VComponentItem is instantiated
    // and OleUIInsertObject returns.
    if (!hasOleObject()) {
        return FALSE;
    }

// Start of Sanjay's Code
    HDC hDC = getVThis()->getHndl();
// Start of Old Code
    //HDC hDC = getVThis()->getHndl();
// End of Old Code
// End of Sanjay's Code

    RECT    rect;

// #define  DSS_DEBUG

#if defined(DSS_DEBUG)
    COLORREF    oldForeColor = SetTextColor(hDC, RGB(0, 0, 0));
    COLORREF    oldBackColor = SetBkColor(hDC, RGB(255, 255, 255));
    int         oldBkMode = SetBkMode(hDC, OPAQUE);

    GetClipBox( hDC, &rect);
// Start of Sanjay's Code
    GetClientRect(getComponentThis()->hwnd(), &rect);
// Start of Old Code
   // GetClientRect(getComponentThis()->hwnd(), &rect);
// End of Old Code
// End of Sanjay's Code
    HRGN newRgn = CreateRectRgnIndirect(&rect);
    SelectClipRgn( hDC, newRgn);
    DeleteObject(newRgn);
#endif

    bounds().getBounds(itemRect, VOlePoint::Pixels);
	// Start of Sanjay's Code
	// Remove Windows offset from the containor's origin
	itemRect.right -= itemRect.left;
	itemRect.bottom -= itemRect.top;
	itemRect.left = 0;
	itemRect.top = 0;
	// End of Sanjay's Code

    rect = itemRect;

    int     left, top, right, bottom;
    updateRect.get(CornerDim, &left, &top, &right, &bottom);

    updRect.left = left;
    updRect.top = top;
    updRect.right = right;
    updRect.bottom = bottom;

    // TO DO: check the we intersect the update rect, else don't paint

    LPLOGPALETTE pColorSet = NULL;
    LPVIEWOBJECT lpView = NULL;

    // get a pointer to IViewObject
    getOleObject().QueryInterface(IID_IViewObject,(LPVOID FAR *) &lpView);

    // if the QI succeeds, get the LOGPALETTE for the object
    if (lpView)
        lpView->GetColorSet(dwDrawAspect, -1, NULL, NULL, NULL, &pColorSet);

    HPALETTE hPal=NULL;
    HPALETTE hOldPal=NULL;

    // if a LOGPALETTE was returned (not guarateed), create the palette and
    // realize it.  NOTE: A smarter application would want to get the LOGPALETTE
    // for each of its visible objects, and try to create a palette that
    // satisfies all of the visible objects.  ALSO: OleStdFree() is use to
    // free the returned LOGPALETTE.
    if ((pColorSet))
        {
        hPal = CreatePalette((const LPLOGPALETTE) pColorSet);
        hOldPal = SelectPalette(hDC, hPal, FALSE);
        RealizePalette(hDC);
        OleStdFree(pColorSet);
        }

    // draw the object

	// we used to used a conversion that's disallowed by borland 5.0
	// i.e. we could convert the return value of getOleObject() into
	// LPUNKNOWN via getOleObject().getUnknown()

    OleDraw((LPUNKNOWN) & getOleObject().getUnknown(), dwDrawAspect, hDC, &itemRect);

    // if the object is remotely active, draw a hatch rect.
    if (isRemotelyActive())
        {
        HBRUSH hBrush = CreateHatchBrush ( HS_BDIAGONAL, RGB(0,0,0) );
        HBRUSH hOldBrush = (HBRUSH) SelectObject (hDC, hBrush);
        SetROP2(hDC, R2_MASKPEN);
        Rectangle (hDC, itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        }

    // by process of elimation, object is inactive, so draw the focus.
    else if (inFocus) {
        drawFocus();
    }

    // if we created a palette, restore the old one, and destroy
    // the object.
    if (hPal)
        {
        SelectPalette(hDC,hOldPal,FALSE);
        DeleteObject(hPal);
        }

    // if a view pointer was successfully returned, it needs to be released.
    if (lpView)
        lpView->Release();


#if defined(DSS_DEBUG)
    //!!!dss - DEBUG - restore the old DC state
    SetTextColor(hDC, oldForeColor);
    SetBkColor(hDC, oldBackColor);
    SetBkMode(hDC, oldBkMode);
#endif

    getVThis()->ungetHndl();

    return TRUE;
}

//**********************************************************************
//
// Name: doVerb
//
// Arguments:
//
//      lpMsg - ptr to MS Windows msg struct. Can be NULL.
//
//**********************************************************************

boolean VComponentItem::doVerb(int iVerb, MSG FAR *lpMsg)
{
    RECT        rect;
    int         x, y, w, h;


	HRESULT  hResult;
	LPVIEWOBJECT2 lpViewObject2;

	// get a pointer to IViewObject2
	if (hasOleObject()){
	hResult = getOleObject().QueryInterface(
			IID_IViewObject2,(LPVOID FAR *)&lpViewObject2);
	}
	if (SUCCEEDED(hResult)) {
		SIZEL		sizel;
		// get extent of the object
		// NOTE: this method will never be remoted; it can be called w/i
		// this async method, contrary to what the documentation says.
		// Got this from Microsoft Simpdnd example.
		lpViewObject2->GetExtent(DVASPECT_CONTENT, -1 , NULL, &sizel);
		lpViewObject2->Release();

   		size(VOleSize(sizel,VOlePoint::MM100ths));
	}

	 // get the rectangle of the object
	bounds().getBounds(x, y, w, h, VOlePoint::Pixels);





    rect.left = x;
    rect.top = y;
    rect.right = x + w;
    rect.bottom = y + h;

    CDBG(   << "bf DoVerb()" << endl);

    HRESULT status = 0;

// rhw 4/15/96: I decided to protect the DoVerb call so that debugging is
// a little more graceful when problems occur.  If we don't have an IOleObject
// at this point, the fact that the DoVerb did not execute is the least of the
// problems.
    if (hasOleObject()) {
        status = getOleObject().DoVerb( iVerb,
                                        lpMsg,
                                        getOleClientSite(),
                                        0, // rhw 4/12/96: Doc says this should be always be zero
	// Start of Sanjay's Code
										getVThis()->getTopLevelParent()->hwnd(),
	// Start of old code
                       //                 getVThis()->hwnd(),
	// End of old code
	// End of Sanjay's Code
                                        &rect);
    }

    CDBG(   << "af DoVerb()" << endl);

    return SUCCEEDED(status);
}

#if 0
//**********************************************************************
//
HWND VComponentItem::hwnd()
{
    return getDocBase()->getView()->hwnd();
}

//**********************************************************************
//
HDC VComponentItem::getHndl()
{
    HDC hDC = lpDocBase->getView->getHndl();

    // To Do: set the clip rect to the item rect
    return hDC;
}

//**********************************************************************
//
void VComponentItem::ungetHndl()
{
    getDocBase()->getView()->ungetHndl();
}

//**********************************************************************
//
boolean VComponentItem::takeFocus()
{
    getDocBase()->getView()->setChildFocus(*this);

    return TRUE;
}
#endif

//**********************************************************************
//
boolean VComponentItem::givenFocus(void)
{
    if (!hasOleObject()) {
        return TRUE;
    }
    inFocus = TRUE;

    // if we get the focus and we're IP active, give the focus
    // to the IP active win.
    // This condition can occur, on an ACTIVATEAPP. The AppView will
    // pass focus to the cur focused child, etc. If the CDItem was the
    // last views object.

    // rhw 4/5/96: Kerstin says that this might be better placed in
    // takeFocus(), but since Steire #ifdef it out it would require
    // a special rebuild of everything from Wojtowicz, so I will leave it
    // here for now and see if I can get it to work.
// Start Sanjay's Code
    LPVCOMPONENTITEM       pCDItem;
	LPOLEINPLACEOBJECT	lpOleInPlaceObject;
    if ((pCDItem = getVThis()->getAppLevelParent()->getComponentThis()->getFocusedComponentItem())
    	&& (pCDItem != this) && pCDItem->isActive() &&
    	(!pCDItem->isActiveXControl() && pCDItem->canLocallyActivate()) &&
    	(!isActiveXControl() && canLocallyActivate())) {
		pCDItem->getOleObject().QueryInterface(IID_IOleInPlaceObject, (LPVOID FAR*) &lpOleInPlaceObject);
        if(lpOleInPlaceObject)
		{
        	lpOleInPlaceObject->UIDeactivate();
			itemDeactivate(pCDItem);
		}
	}
	if(bCanActivateLocally)
   		setActivationState(locallyActive);
// Start of Old Code
// End of Old Code
   //	setActivationState(locallyActive);
// End Sanjay's Code (Uncommented old code

// Start Sanjay's Code
    if (isLocallyActive()) {
// Start of Old Code
   // if (isActive()) {
// End of Old Code
// End Sanjay's Code
//(Uncommented old code
// This is a nice idea, but cause the CDItem to get a KillFocus when the
// IP window gets the SetFocus causing CDItem::clearFocus() to
// deactivate the IP window.
        HWND    hWnd = 0;

        // we could  get the IPActiveObject, but since we know we're
        // UI active, then this should work.
// rhw 4/11/96: Something strange is going on the line below that I commented
// out!  This line of code causes an access violation.  That is, one of the
// "get" methods is returning NULL, but because of the macros, I can't examine
// any variables to determine which one :<.  There seems to be some real problems
// with how the underlying IOleInPlaceFrame, IOleInPlaceUIWindow, IOleInPlaceSite,
// IOleInPlaceObject, and IOleInPlaceActiveObject interfaces are implemented and
// how they are initialized and associated with each other in the VComponent
// wrapper code.
// Start Sanjay's Code
		LPOLEINPLACEOBJECT	lpOleInPlaceObject;
		getOleObject().QueryInterface(IID_IOleInPlaceObject, (LPVOID FAR*) &lpOleInPlaceObject);
    	if(lpOleInPlaceObject)
    		lpOleInPlaceObject->GetWindow(&hWnd);
        if(hWnd);
        	SetFocus(hWnd);
// Start of Old Code
    //	getOleInPlaceSite().getOleInPlaceObject().GetWindow(&hWnd);

    //    cv_assert(hWnd);
    //    SetFocus(hWnd);
// End of Old Code
// End Sanjay's Code (Uncommented old code
    }
    // else not active so draw focus
    else {
        drawFocus();
    }

    return TRUE;
}

//**********************************************************************
//
boolean VComponentItem::clearFocus(void)
{

    // TO DO: we don't want to do this if the clear focus resulted from a
    // deactivate app. Do this only when the focus changes and the View is
    // stil active.
// Start Sanjay's Code
    if (isLocallyActive()) {
// Start of Old Code
   // if (isActive()) {
// End of Old Code
// End Sanjay's Code
// Start Sanjay's Code
/*
	LPOLEINPLACEOBJECT	lpOleInPlaceObject;
	getOleObject().QueryInterface(IID_IOleInPlaceObject, (LPVOID FAR*) &lpOleInPlaceObject);
	if(bCanActivateLocally)
	{
    	if(lpOleInPlaceObject)
    		lpOleInPlaceObject->InPlaceDeactivate();
	}
	else
	{
		itemDeactivate(this);
	}
*/
// Start of Old Code
//!!!dss testing
//      getOleInPlaceSite().getOleInPlaceObject().UIDeactivate();
// End of old code
// End Sanjay's Code (Uncommented old code
    }
    else {
// Start Sanjay's Code
        undrawFocus();
// Start of Old Code
//        undrawFocus();
// End of old code
// End Sanjay's Code (Uncommented old code
    }

    inFocus = FALSE;
    return TRUE;
}

//**********************************************************************
//
void VComponentItem::drawFocus(void)
{
// Start of Sanjay's Code
	// Don't draw hached border if it is an OCX control
	if(this->isA(VComponentControlCls))
		return;
// End of Sanjay's Code
    VStyle& style = getVThis()->getStyle();

    if (style.contains(StyleBorder)) {
        DrawOleObjectBorder(TRUE, style.contains(StyleSizable));
    }

}

//**********************************************************************
//
void VComponentItem::undrawFocus(void)
{
// Start of Sanjay's Code
	// Don't draw hached border if it is an OCX control
	if(this->isA(VComponentControlCls))
		return;
// End of Sanjay's Code
    VStyle& style = getVThis()->getStyle();

    if (style.contains(StyleBorder)) {
        DrawOleObjectBorder(FALSE, style.contains(StyleSizable));
    }
}

//**********************************************************************
//
void VComponentItem::copyToClipboard(void)
{
    LPDATAOBJECT lpDataObj;

    CDBG(   << "in VComponentItem::copyToClipboard" << endl);

#if 1
    // Create a data transfer object by cloning the existing OLE object
    CDataObject FAR* pDataXferObj = CDataObject::Create(this,NULL);
    if (!pDataXferObj) {
        CDBG(   << "    out of memory cloning data object." << endl);
        return;
    }
    // initially obj is created with 0 refcnt. this QI will make it go to 1.
    pDataXferObj->QueryInterface(IID_IDataObject, (LPVOID FAR*)&lpDataObj);
#elif 0
    // this will put dynamic data on the clipboard. It will change
    // as the object changes because the data object is from the the object
    // itself, instead of from a copy.
    getOleObject().QueryInterface(IID_IDataObject,
            (LPVOID FAR*)&lpDataObj);
#else
    // NOTE: this requires a running server
    // this will get a snapshot of the data. It will not change as the
    // object changes.
    getOleObject().GetClipboardData(0, &lpDataObj);
#endif

    cv_assert(lpDataObj);

    DebugOutputEnumFmtEtc(lpDataObj);
    DebugOutputDataObj(lpDataObj);

    CDBG(   << "    setting DataObj " << lpDataObj << " to clipboard" << endl);

    // put out data transfer object on the clipboard. this API will AddRef.
    OleSetClipboard(lpDataObj);

    // Give ownership of data transfer object to clipboard
//  pDataXferObj->Release();    // test. I think this is that same thing as
                                // lpDataObj, because the pointers are related.
    lpDataObj->Release();

    onClipboard = this;
}

//**********************************************************************
//
void VComponentItem::deleteFromClipboard(void)
{
    LPDATAOBJECT lpDataObject;
    HRESULT hResult = OleGetClipboard(&lpDataObject);

    CDBG(   << "in VComponentItem::deleteFromClipboard" << endl);

    if (hResult == S_OK) {
        // Remove our data transfer object from clipboard if it is there.
        // this will leave HGLOBAL based data behind on the clipboard
        // including OLE 1.0 compatibility formats.
        OleFlushClipboard();
    }

    if (lpDataObject) {
        lpDataObject->Release();
    }

}

//**********************************************************************
//
#if 0
void VComponentItem::createOleClientSite(void)
{
    lpOleClientSite = new COleClientSite(this);
    cv_assert(lpOleClientSite);
}

//**********************************************************************
//
void VComponentItem::createOleInPlaceSite(void)
{
    lpOleInPlaceSite = new COleInPlaceSite(this);
    cv_assert(lpOleInPlaceSite);
}
#endif

#if 0
//**********************************************************************
//
// Set the Object Rect in Pixels
//
void VComponentItem::setBounds(const RECT& rect)
{
    bounds.left = XformWidthInPixelsToHimetric(NULL,(int)rect.left);
    bounds.top = XformHeightInPixelsToHimetric(NULL,(int)rect.top);
    bounds.right = XformWidthInPixelsToHimetric(NULL,(int)rect.right);
    bounds.bottom = XformHeightInPixelsToHimetric(NULL,(int)rect.bottom);
}

//**********************************************************************
//
// Set the Object Rect in Pixels
//
void VComponentItem::getBounds(RECT& rect)
{
    rect.left = XformWidthInPixelsToHimetric(NULL,(int)bounds.left);
    rect.top = XformHeightInPixelsToHimetric(NULL,(int)bounds.top);
    rect.right = XformWidthInPixelsToHimetric(NULL,(int)bounds.right);
    rect.bottom = XformHeightInPixelsToHimetric(NULL,(int)bounds.bottom);
}

//**********************************************************************
//
// Set the Object Size, in Pixels
//
void VComponentItem::setPos(const RECT& rect)
{
    bounds.left = XformWidthInPixelsToHimetric(NULL,(int)rect.left);
    bounds.top = XformHeightInPixelsToHimetric(NULL,(int)rect.top);
}

//**********************************************************************
//
void VComponentItem::setSize(const RECT& rect)
{
    bounds.right = XformWidthInPixelsToHimetric(NULL,(int)rect.right);
    bounds.bottom = XformHeightInPixelsToHimetric(NULL,(int)rect.bottom);
}

#endif

#if 0

// don't need this now that doc items are VWindows.

//**********************************************************************
//
// QueryDrag
//
// Purpose:
//
//      Check to see if Drag operation should be initiated based on the
//      current position of the mouse.
//
// Parameters:
//
//      POINT pt                - position of mouse
//
// Return Value:
//
//      BOOL                    - TRUE if drag should take place,
//                                else FALSE
// Comments:
//
//********************************************************************

BOOL VComponentItem::QueryDrag(POINT pt)
{
// Start of Sanjay's Code
	pt.x += getPosition().x();
	pt.y += getPosition().y();
// End of Sanjay's Code
    // if pt is within rect of object, then start drag
    if (getChildFocus())
        {
        return getChildFocus()->bounds().pointIn(VOlePoint(pt)) ? TRUE : FALSE;
        }
    else
        return FALSE;
}
#endif

//**********************************************************************
//
// DoDragDrop
//
// Purpose:
//
//      Actually perform a drag/drop operation with the current
//      selection in the source document.
//
// Parameters:
//
//      none.
//
// Return Value:
//
//      DWORD                    - returns the result effect of the
//                                 drag/drop operation:
//                                      DROPEFFECT_NONE,
//                                      DROPEFFECT_COPY,
//                                      DROPEFFECT_MOVE, or
//                                      DROPEFFECT_LINK
//
// Comments:
//
//********************************************************************

DWORD VComponentItem::DoDragDrop (void)
{
    DWORD       dwEffect     = 0;
    LPDATAOBJECT lpDataObj;

    CDBG(   << "In VComponentItem::DoDragDrop" << endl);

    // Create a data transfer object by cloning the existing OLE object
    CDataObject FAR* pDataXferObj = CDataObject::Create(this ,NULL);

    if (!pDataXferObj) {
        CDBG(   << "    out of memory cloning data object." << endl);
        return DROPEFFECT_NONE;
    }

    // initially obj is created with 0 refcnt. this QI will make it go to 1.
    pDataXferObj->QueryInterface(IID_IDataObject, (LPVOID FAR*)&lpDataObj);
    cv_assert(lpDataObj);

//  bLocalDrop     = FALSE;
    bLocalDrag     = TRUE;

    ::DoDragDrop ( lpDataObj,
                 getDropSource(),
                 DROPEFFECT_COPY,   // we only allow copy
                 &dwEffect
                 );

    bLocalDrag     = FALSE;

    /* if after the Drag/Drop modal (mouse capture) loop is finished
    **    and a drag MOVE operation was performed, then we must delete
    **    the selection that was dragged.
    */
    if ( (dwEffect & DROPEFFECT_MOVE) != 0 ) {
        // ... delete source object here (we do NOT support MOVE)
    }

    pDataXferObj->Release();    // this should destroy the DataXferObj

    return dwEffect;
}

//**********************************************************************
//
boolean VComponentItem::dragDelayTimeout()
{
    // drag time delay threshhold exceeded -- start drag
    ReleaseCapture();

    delete timer;
    timer = 0;

    bPendingDrag = FALSE;

    // perform the modal drag/drop operation.
    DoDragDrop( );

    return TRUE;
}

//**********************************************************************
//
// Name: addVerbs
//
// Description:
//
//      Adds the objects verbs to the edit menu.
//
// Parameters:
//
//      theMenu - the menu to add the verb menu item (or popup) to.
//      iPosition - zero based position where the menu item will be created
//
// Returns:
//
//      the verb menu item or popup
//
//********************************************************************

VObject *VComponentItem::addVerbs(VPopupMenu *theMenu, int iPosition)
{
    // don't re-add the ole object's verb menu.
    //
    // TO DO: move this into CDItem. Its OK, for now,
    // because we assume one doc item per document.
    //
    if (lpOleObjectVerbMenu == 0) {
        HMENU   hmenuOleObjectVerbMenu = 0;
		LPOLEOBJECT lpOleObject =  (LPOLEOBJECT)
		&getOleObject(). getUnknown();
		// we used to used a conversion thats disallowed by borland 5.0

        //!!!dss test for failure
        OleUIAddVerbMenu ( (LPOLEOBJECT) lpOleObject,
                           NULL,
                           theMenu->hmenu(),
                           iPosition + 1,   // +1 -> 1 based position
                           IDM_VERB0,
                           0,           // no maximum verb IDM enforced
                           FALSE,
                           1,
                           &hmenuOleObjectVerbMenu);

        // get the text of the newly created menu item
        char    menuText[25];
        GetMenuString(theMenu->hmenu(), iPosition, menuText,
                              sizeof(menuText) - 1, MF_BYPOSITION);
        menuText[sizeof(menuText) - 1] = '\0';

        // if OleUIAddVerbMenu() created a popup,
        // so create a VPopupMenu object from it.
        if (hmenuOleObjectVerbMenu) {
            // construct a Views menu from a native menu handle
            lpOleObjectVerbMenu = new VPopupMenu(hmenuOleObjectVerbMenu, menuText);

            int wCount = GetMenuItemCount(hmenuOleObjectVerbMenu);
            int wItem;
            for (wItem = 0; wItem < wCount; wItem++) {
                int     wID = GetMenuItemID(hmenuOleObjectVerbMenu, wItem);
                char    menuText[25];

                GetMenuString(hmenuOleObjectVerbMenu, wCount, menuText,
                                sizeof(menuText) - 1, MF_BYPOSITION);
                menuText[sizeof(menuText) - 1] = '\0';

                VMenuItem *menuItem = new VMenuItem(wID,
                                  menuText,
                                  MULTI_INHERIT_HACK(this),
                                  methodOf(VComponentItem, oleVerbMenu));

                // NOTE: addMenuItem() will create a menu item. We just want to
                // suck up the native items into Views.
                ((VPopupMenu *)lpOleObjectVerbMenu)->bindItem(menuItem);
				menuItem->oleDeletes(TRUE);
            } // for
        }

        // OleUIAddVerbMenu() didn't create a popup, just a menu item.
        // so create a VMenuItem object from it
        else {
            // get the Id of the newly created menu item
            int menuId = GetMenuItemID(theMenu->hmenu(), iPosition);

            // construct a Views menu item from a native menu handle
            lpOleObjectVerbMenu = new VMenuItem(menuId,
                                                menuText,
                                                MULTI_INHERIT_HACK(this),
                                                methodOf(VComponentItem, oleVerbMenu));
        }

        // don't do additem() as this will natively add the menu item,
        // which has already been done by ole.
		 if (lpOleObjectVerbMenu){
			theMenu->bindItem(lpOleObjectVerbMenu);
			theMenu->oleDeletes(TRUE);
		 }
    }

    // else, verb menu already created, so add it to the menu, passed
    else {
		if (lpOleObjectVerbMenu){
	        theMenu->bindItem(lpOleObjectVerbMenu);
			theMenu->oleDeletes(TRUE);
		}
    }

    return lpOleObjectVerbMenu;
}

//**********************************************************************
//
VWinRect& VComponentItem::move(const VOlePosition& pos)
{
	// Return if the new position is same as the old position
	RECT    itemRect;
	bounds().getBounds(itemRect, VOlePoint::Pixels);
	if((itemRect.left == pos.left()) && (itemRect.top == pos.top()))
		return objBounds;
	else
	{
		objBounds = pos;
		getVThis()->move(pos.left(),pos.top());
	}
// gregm // objBounds = pos;

//  setPhysical(VRectangle(objBounds));
//  setFrame(VFrame(objBounds));

    return objBounds;
}

// Sanjay Start
//**********************************************************************
//
boolean VComponentItem::moved(int x, int y)
{
	move(VOlePosition(x, y, VOlePoint::Pixels));
	return TRUE;
}
// Sanjay End
//**********************************************************************
//
VWinRect& VComponentItem::size(const VOleSize& size)
{
// Start of Sanjay's Code
	// Return if the new size is same as the old size
    RECT    itemRect;
	bounds().getBounds(itemRect, VOlePoint::Pixels);
	if(((itemRect.right - itemRect.left) == size.w()) && ((itemRect.bottom - itemRect.top) == size.h()))
		return objBounds;
	else
	{
    	objBounds = size;
		bounds().getBounds(itemRect, VOlePoint::Pixels);
		getVThis()->resize((int)(itemRect.right - itemRect.left),(int)(itemRect.bottom - itemRect.top));
	}
// End of Sanjay's Code

//  setPhysical(VRectangle(objBounds));
//  setFrame(VFrame(objBounds));

    return objBounds;
}

//**********************************************************************
//
boolean VComponentItem::resized(int w, int h)
{
    CDBG(   << "In VComponentItem::resized, w = " << w << ", h = " << h << endl);

    // return if no object
    if (!hasOleObject()) {
        return TRUE;
    }

    // if the CDItem is locally active, update the objects knowledge of
    // the containers extents.
    //
    // We must do this to change the container clipping so that dragging
    // an Locally Active object around will clip to the top level window's
    // client area.
    //
    // if the CDItem is inactive, the item's OLE Object could still be in
    // place active (like when server connection caching is enabled). This
    // would causes the Ole Object to display some of it's in place
    // decorations (which I believe is a bug).
    if (isLocallyActive()) {
        LPOLEINPLACEOBJECT  pOleInPlaceObject;

//      getOleObject().QueryInterface(IID_IOleInPlaceObject,
//                              (LPVOID FAR*)&pOleInPlaceObject);

        pOleInPlaceObject = &getOleInPlaceSite().getOleInPlaceObject();

        // if i've got the in place object, tell it about the resize.
        if (pOleInPlaceObject) {
// Start of Sanjay's Code
            RECT    clipRect;
            RECT    itemRect= {0, 0, w, h};
			SIZEL sizel;
			sizel.cx = w;
			sizel.cy = h;
    		size(VOleSize(sizel,VOlePoint::Pixels));
// Start of old code
            //RECT    clipRect = {0, 0, w, h};
            //RECT    itemRect;
// End of old code
// End of Sanjay's code

            bounds().getBounds(itemRect, VOlePoint::Pixels);

// Start of Sanjay's Code
			GetClientRect(getVThis()->getTopLevelParent()->hwnd(), &clipRect);
            pOleInPlaceObject->SetObjectRects(&itemRect, &clipRect);
// Start of old code
           // pOleInPlaceObject->SetObjectRects(&itemRect, &clipRect);
// End of old code
// End of Sanjay's code
//          pOleInPlaceObject->Release();
			uponInplaceObjectMoveOrResize(); // maybe do a bit more in a
			// derived class
        }
    }
// Start of Sanjay's Code
    else if (isRemotelyActive()) {
		RECT    itemRect;
		SIZEL sizel;
		sizel.cx = w;
		sizel.cy = h;
    	size(VOleSize(sizel,VOlePoint::Pixels));
		bounds().getBounds(itemRect, VOlePoint::Pixels);

		VOleSize    size(itemRect.right - itemRect.left, itemRect.bottom - itemRect.top, VOlePoint::Pixels);
        HRESULT hResult = getOleObject().SetExtent(dwDrawAspect,
                                                    &size.getPoint(sizel, VOlePoint::MM100ths) );
//        cv_assert(SUCCEEDED(hResult));
    }
// End of Sanjay's Code
// Start of Sanjay's Code
    else if (!isActive() && bCanActivateLocally) {
// Start of old code
   // else if (!isActive()) {
// End of old code
// End of Sanjay's Code
		RECT    itemRect;
// Start of Sanjay's Code
		SIZEL sizel;
		sizel.cx = w;
		sizel.cy = h;
    	size(VOleSize(sizel,VOlePoint::Pixels));
// End of Sanjay's code
		bounds().getBounds(itemRect, VOlePoint::Pixels);

        //VOleSize    size(w, h, VOlePoint::Pixels);
		VOleSize    size(itemRect.right - itemRect.left, itemRect.bottom - itemRect.top, VOlePoint::Pixels);
// Start of Sanjay's Code
// Start of old code
        //SIZEL       sizel;
// End of old code
// End of Sanjay's code

// Start of Sanjay's Code
		DWORD miscStatus;
		BOOL bRunToResize = FALSE;
		HRESULT hResult = getOleObject().GetMiscStatus(dwDrawAspect, &miscStatus);
		if(SUCCEEDED(hResult))
		{
			if((miscStatus & OLEMISC_RECOMPOSEONRESIZE) && bCanActivateLocally)
			{
				bRunToResize = TRUE;
				OleRun(&(getOleObject().getUnknown()));
			}
		}

        hResult = getOleObject().SetExtent(dwDrawAspect,
                                                    &size.getPoint(sizel, VOlePoint::MM100ths) );

        // rhw 4/2/96: This is currently failing for insertables saying that
        // it fails since the object is NOT running.  Object needs to be
        // running to perform this operation.
// Start of Sanjay's Code
// Start of old code
        //cv_assert(SUCCEEDED(hResult));
		if(bRunToResize)
		{
			getOleObject().Update();
			getOleObject().Close(OLECLOSE_SAVEIFDIRTY);
		}
// End of old code
// End of Sanjay's code
    }
// Start of Sanjay's Code
	else if(!isActive() && !bCanActivateLocally) {
		RECT    itemRect;
		SIZEL sizel;
		sizel.cx = w;
		sizel.cy = h;
    	size(VOleSize(sizel,VOlePoint::Pixels));
		bounds().getBounds(itemRect, VOlePoint::Pixels);

		VOleSize    size(itemRect.right - itemRect.left, itemRect.bottom - itemRect.top, VOlePoint::Pixels);
        HRESULT hResult = getOleObject().SetExtent(dwDrawAspect,
                                                    &size.getPoint(sizel, VOlePoint::MM100ths) );
       	if(!SUCCEEDED(hResult))
		{
    		LPVIEWOBJECT2   lpViewObject2;
			hResult = getOleObject().QueryInterface(
				IID_IViewObject2,(LPVOID FAR *)&lpViewObject2);
			if (SUCCEEDED(hResult)) {
				SIZEL		sizel;
				// get extent of the object
				lpViewObject2->GetExtent(DVASPECT_CONTENT, -1 , NULL, &sizel);
				lpViewObject2->Release();
   				this->size(VOleSize(sizel,VOlePoint::MM100ths));
				bounds().getBounds(itemRect, VOlePoint::Pixels);
			}
		}
	}
// End of Sanjay's code

    return TRUE;
}

//**********************************************************************
//
boolean VComponentItem::mouseDbl(int x, int y)
{
    POINT   pt;
    MSG     msg;
    WPARAM  keyState = 0;

    pt.x = x;
    pt.y = y;

// Start of Sanjay's Code
    POINT   origPt = pt;
	pt.x += getPosition().x();
	pt.y += getPosition().y();
// End of Sanjay's Code

    // if the point falls outside the objects bounds, return.
    if ( !bounds().pointIn(VOlePoint(pt, VOlePoint::Pixels)) ) {
        return FALSE;
    }

    if (GetKeyState(VK_CONTROL))
        keyState |= MK_CONTROL;
    if (GetKeyState(VK_MBUTTON))
        keyState |= MK_MBUTTON;
    if (GetKeyState(VK_RBUTTON))
        keyState |= MK_RBUTTON;
    if (GetKeyState(VK_SHIFT))
        keyState |= MK_SHIFT;

// Start of Sanjay's Code
   // msg.hwnd = getVThis()->getTopLevelParent()->hwnd();
   // msg.lParam = MAKELPARAM(y + getPosition().y(), x + getPosition().x());
// Start of old code
   msg.hwnd = getVThis()->hwnd();
   msg.lParam = MAKELPARAM(y, x);
// End of old message
// End of Sanjay's Code
    msg.message = WM_LBUTTONDOWN;
    msg.wParam = keyState;
    msg.time = GetMessageTime();
    msg.pt = origPt;

    // Execute object's default verb
    // TO DO : create some VComponentItem::verbs
    doVerb(OLEIVERB_PRIMARY, &msg);
    return TRUE;
}

//**********************************************************************
//
boolean VComponentItem::mouseDn(int x, int y)
{
    POINT pt;

    pt.x = x;
    pt.y = y;

// Start of Sanjay's Code
	pt.x += getPosition().x();
	pt.y += getPosition().y();
// End of Sanjay's Code

    // if the point falls outside the objects bounds, return.
    if ( !bounds().pointIn(VOlePoint(pt, VOlePoint::Pixels)) ) {
        return FALSE;
    }

    /* OLE2NOTE:
    **    we do NOT want to start a drag immediately; we want to
    **    wait until the mouse moves a certain threshold. or a
    **    certain amount of time has elapsed. if
    **    LButtonUp comes before the drag is started, then
    **    the bPendingDrag state is cleared. we must capture
    **    the mouse to ensure the modal state is handled
    **    properly.
    */
    bPendingDrag = TRUE;
    ptButDown = pt;

    cv_assert(timer == 0);

    timer = new VTimer(MULTI_INHERIT_HACK(this),
                        methodOf(VComponentItem, dragDelayTimeout));
    timer->start(getDragDelay());

    SetCapture(getVThis()->hwnd());

    return TRUE;
}

//**********************************************************************
//
boolean VComponentItem::mouseUp(int x, int y)
{
    if (bPendingDrag)
    {
        /* ButtonUP came BEFORE distance/time threshholds were
        **    exceeded. clear fPendingDrag state.
        */
        ReleaseCapture();

        delete timer;
        timer = 0;

        bPendingDrag = FALSE;
    }
	if(!isActiveXControl() && bCanActivateLocally)
	{
    	POINT   pt;
    	MSG     msg;
    	WPARAM  keyState = 0;

    	pt.x = x;
    	pt.y = y;

    	POINT   origPt = pt;
		pt.x += getPosition().x();
		pt.y += getPosition().y();

    	// if the point falls outside the objects bounds, return.
    	if ( !bounds().pointIn(VOlePoint(pt, VOlePoint::Pixels)) ) {
        	return FALSE;
    	}

    	if (GetKeyState(VK_CONTROL))
        	keyState |= MK_CONTROL;
    	if (GetKeyState(VK_MBUTTON))
        	keyState |= MK_MBUTTON;
    	if (GetKeyState(VK_RBUTTON))
       		keyState |= MK_RBUTTON;
    	if (GetKeyState(VK_SHIFT))
        	keyState |= MK_SHIFT;

   		msg.hwnd = getVThis()->hwnd();
   		msg.lParam = MAKELPARAM(y, x);
    	msg.message = WM_LBUTTONDOWN;
    	msg.wParam = keyState;
    	msg.time = GetMessageTime();
    	msg.pt = origPt;

    	doVerb(OLEIVERB_PRIMARY, &msg);
	}
	// Sanjay End
    return TRUE;
}

//**********************************************************************
//
boolean VComponentItem::mouseMv(int x, int y, int bDown)
{
    if (bPendingDrag) {
        POINT pt = ptButDown;
        int   nDragMinDist = getDragMinDist();

        if (! ( ((pt.x - nDragMinDist) <= x)
                && (x <= (pt.x + nDragMinDist))
                && ((pt.y - nDragMinDist) <= y)
                && (y <= (pt.y + nDragMinDist)) ) ) {
            // mouse moved beyond threshhold to start drag
            ReleaseCapture();

            delete timer;
            timer = 0;

            bPendingDrag = FALSE;

            // perform the modal drag/drop operation.
            DoDragDrop( );
        }
    }

    return TRUE;
}

//**********************************************************************
//
VWinRect& VComponentItem::bounds()
{
    return objBounds;
}

//**********************************************************************
//
// TO DO: I think this should be part of the container and it should
// call VComponentItem::doVerb().
//
boolean VComponentItem::oleVerbMenu(VMenuItem *mitem)
{
    cv_assert(mitem->isA(VMenuItemCls));
    cv_assert(mitem->getIdNo() >= IDM_VERB0);

    doVerb(mitem->getIdNo() - IDM_VERB0);

    return TRUE;
}

//**********************************************************************
//
int VComponentItem::getDragDelay()
{
    // delay before dragging should start, in milliseconds
    return GetProfileInt(
            "windows",
            "DragDelay",
            DD_DEFDRAGDELAY);
}

//**********************************************************************
//
int VComponentItem::getDragMinDist()
{
    // minimum distance (radius) before drag should start, in pixels
    return GetProfileInt(
            "windows",
            "DragMinDist",
            DD_DEFDRAGMINDIST);
}

//**********************************************************************
//
int VComponentItem::getScrollDelay()
{
    // delay before scrolling, in milliseconds
    return GetProfileInt(
            "windows",
            "DragScrollDelay",
            DD_DEFSCROLLDELAY);
}

//**********************************************************************
//
int VComponentItem::getScrollInset()
{
    return GetProfileInt(
            "windows",
            "DragScrollInset",
            DD_DEFSCROLLINSET);
}

//**********************************************************************
//
int VComponentItem::getScrollInterval()
{
    return GetProfileInt(
            "windows",
            "DragScrollInterval",
            DD_DEFSCROLLINTERVAL);
}

//**********************************************************************
//
void VComponentItem::destroy(void)
{
    unloadItem(TRUE);

    // destroy LAST!
    VComponentWindow::destroy();
}

//**********************************************************************
//
boolean VComponentItem::close()
{
    CDBG(   << "In VComponentItem::close" << endl);

    // if we're overridden, pass it on.
    if (VComponentWindow::close()) {
        return TRUE;
    }

    // unload the item. Return cancel is user canceled the unload.
    if (unloadItem(TRUE)) {
        return TRUE;
    }

    return FALSE;
}

//**********************************************************************
//
boolean VComponentItem::quit()
{
    CDBG(   << "In VComponentItem::quit" << endl);

    // if we're overridden, pass it on.
    if (VComponentWindow::quit()) {
        return TRUE;
    }

    // unload the item. Return cancel is user canceled the unload.
// -> don't unload the item. hands off storage
#if 0
    if (unloadItem(TRUE)) {
        return TRUE;
    }
#elif 1
    // deactivate, if active. NOP if not active.
    setActivationState(inactive);

//  // close: save & keep
//  if (getStorage().close(TRUE, FALSE) == CancelButton) {
//      return TRUE;
//  }

    // disconnectStorage();
    closeItem(TRUE);
#endif

    return FALSE;
}

//**********************************************************************
//
void VComponentItem::setActivationState(VComponentItem::ActivationStates newState)
{
    ActivationStates previousActivationState = activationState;

    activationState = newState;

    switch (newState)
    {
        case inactive:

            // if the item was locally active, call the notification
            // method
            if (previousActivationState == locallyActive) {
                itemDeactivate(this);
            }
        break;

        case locallyActive:
            // if the item was not already locally active, call the
            // notification method
            if (previousActivationState != locallyActive) {
                itemActivate(this);
                doVerb(OLEIVERB_INPLACEACTIVATE);
            }
        break;

    } // switch
}

//**********************************************************************
//
void VComponentItem::DrawOleObjectBorder(boolean draw, boolean drawHandles)
{
    if (drawHandles) {
//      RECT    rect;

//      bounds().getBounds(rect, VOlePoint::Pixels);

        DrawItemHandles(
            *this,
            OLEUI_HANDLES_INSIDE,
            HANDLE_SIZE,
            draw);
    }
    else {
        VRectangle  rect = bounds();
        VPort       port(getVThis());
        VPen        pen;

        // if erasing
        if (!draw) {
            // get this windows background color, if one is defined, else
            // use white.
            // TO DO: use the system background color.
            if (getVThis()->getBackground()) {
                pen.color(getVThis()->getBackground()->background());
            }
            else {
                pen.color(VColor::White);
            }
        }
        else {
            pen.color(VColor::Black);
        }

        if (isLinkedItem()) {
            pen.pattern(DashLine);
        }
        else {
            pen.pattern(SolidLine);
        }

//      port.rule(XorRule);
//      port.rule(draw ? CopyRule : XorRule);
        port.usePen(&pen);
        port.open();
        port.frameRegion(&rect);
        port.close();
    }

}

//**********************************************************************
//
//  Returns
//      TRUE - success, FALSE - failure
//
boolean VComponentItem::connectStorage(void)
{
    CDBG(   << "In VComponentItem::connectStorage" << endl);

    if (!VComponentWindow::connectStorage()) {
        return FALSE;
    }

    LPPERSISTSTORAGE    lpPersistStorage;
    OLEResult   result;
    result = getOleObject().QueryInterface(IID_IPersistStorage,
                                           (LPVOID*)&lpPersistStorage);
    if (lpPersistStorage) {
        lpPersistStorage->SaveCompleted((LPSTORAGE) &(getStorage().getStorage().getUnknown()));
        lpPersistStorage->Release();
    }
    else {
        CDBG(   << "    ...QueryInterface(IID_IPersistStorage) failed: "
                << result.asSystemMessage() << endl);
        return FALSE;
    }

    return TRUE;
}

//**********************************************************************
//
//  Returns
//      TRUE - success, FALSE - failure
//
boolean VComponentItem::disconnectStorage(void)
{
    CDBG(   << "In VComponentItem::disconnectStorage" << endl);

    if (getStorage().isRealized()) {

        LPPERSISTSTORAGE    lpPersistStorage;
        OLEResult   result;
        result = getOleObject().QueryInterface(IID_IPersistStorage,
                                                (LPVOID*)&lpPersistStorage);
        if (lpPersistStorage) {
            lpPersistStorage->HandsOffStorage();
            lpPersistStorage->Release();
        }
        else {
            CDBG(   << "    ...QueryInterface(IID_IPersistStorage) failed: "
                    << result.asSystemMessage() << endl);
            return FALSE;
        }

        if (!VComponentWindow::disconnectStorage()) {
            return FALSE;
        }
    }

    return TRUE;
}

// ============================================================================
#ifndef CV_NOOLEARCHIVER

// ---------------------------------------------------------------------------
//
void VComponentItem::putTo(VArchiver &a)
{
    VComponentWindow::putTo(a);

    // put the item bounds. This is not the VWindow frame.
    // TO DO: uncomment when archiver support in WinRect, OlePoint,
    // a << bounds();
// Start of Sanjay's Code
    a << VComponentItemArchiveTag;
    a.putObject((VWinRect*)&objBounds);
    a << bIsActiveXControl;
// End of Sanjay's Code

    a.putSelf(*this, &VComponentItem::putToX);
} // putTo()

// ---------------------------------------------------------------------------
//
void VComponentItem::putToX(VObject& obj, VArchiver& a,
                     VArchiver::AtMethodGetType getObjPos,
                     VArchiver::AtMethodSetType setObjPos)
{
	HRESULT 	hrErr;
    // OLE Storage Open Mode (see OLE documentation on STATSTG)
    DWORD   	grfMode =   STGM_DIRECT | STGM_WRITE | STGM_CONVERT |
                        	STGM_SHARE_EXCLUSIVE;

    VComponentItem&        item = (VComponentItem&) obj;

#if 0

	// RG: new begin
		hrErr = CreateILockBytesOnHGlobal(NULL, TRUE, &item.m_lpLockBytes);

		if (hrErr != NOERROR)
   			return;
		cv_assert(item.m_lpLockBytes);


		hrErr = StgCreateDocfileOnILockBytes(item.m_lpLockBytes,
									STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,
							  		0,
							  		&item.m_lpStorage);
		if (hrErr != NOERROR)
		{
			item.m_lpLockBytes->Release();
			item.m_lpLockBytes = NULL;
			return;
		}
   		cv_assert(item.m_lpStorage != NULL);


	// RG: if we have a suitable storage, then proceed writing to a stream
	// use stroage to point to stream and save this stuff to a stream so we can cal getsize(),



#endif

	LPPERSISTSTORAGE 	pPersistStorage;


    hrErr = item.getOleObject().QueryInterface(IID_IPersistStorage,(LPVOID FAR*)&pPersistStorage);
	if (hrErr == NOERROR)
	{

		hrErr = ::OleSave(pPersistStorage, item.m_lpStorage,TRUE);
		pPersistStorage->SaveCompleted(NULL);


		//hrErr = pPersistStorage->Save(item.m_lpStorage, FALSE);
// Start of Sanjay's Code
	//	hrErr = pPersistStorage->SaveCompleted(NULL);
// End of Sanjay's Code
		pPersistStorage->Release();
		item.m_lpStorage->Commit(STGC_OVERWRITE);
		cv_assert(StgIsStorageILockBytes(item.m_lpLockBytes) == NOERROR);
		if (hrErr != NOERROR)
			return;

		 // RG: attempt to get the handle to the global memory
 		HGLOBAL hStorage;

		hrErr = GetHGlobalFromILockBytes(item.m_lpLockBytes, &hStorage);
 		if (hrErr != NOERROR)
			return;

 		// RG: first write a byte count
 		STATSTG statstg;
 		hrErr = item.m_lpLockBytes->Stat(&statstg, STATFLAG_NONAME);
 		if (hrErr != NOERROR)
			return;
 		cv_assert(statstg.cbSize.HighPart == 0);   // RG: don't support >4GB objects
 		DWORD dwBytes = statstg.cbSize.LowPart;
 		a << dwBytes;
		// RG: write the contents of the block
 		LPVOID lpBuf = GlobalLock(hStorage);
 		cv_assert(lpBuf != NULL);
 		a.write((const char *)lpBuf, (UINT)dwBytes);
 		::GlobalUnlock(hStorage);
	}


	return;
	}
	// RG: new end


// ---------------------------------------------------------------------------
//
void VComponentItem::getFrom(VArchiver& a)
{
    VComponentWindow::getFrom(a);

// Start of Sanjay's Code
// Start of Old Code
   // VWinRect        itemBounds;
// End of Old Code
// End of Sanjay's Code

    // get the item bounds. This is not the VWindow frame.
    // TO DO: uncomment when archiver support in WinRect, OlePoint,
    // a >> itemBounds;
// Start of Sanjay's Code
    if (getArchiveTagRev(currentWindowTag) >= 7) {
    	long tag;
    	a >> tag;
    	if (!archiveTagOk(tag, VComponentItemArchiveTag)) {
        	a.abort(archiveTagAbortCheck(tag, VComponentItemArchiveTag));
        	return;
    	}
		a.getObject((VWinRect*)&objBounds);
    	a >> bIsActiveXControl;
	}
// End of Sanjay's Code

    a.getSelf(*this, &VComponentItem::getFromX);

// Start of Sanjay's Code
// Start of Old Code
    // setBounds(itemBounds);
// End of Old Code
// End of Sanjay's Code

} // getFrom()

//
void VComponentItem::getFromX(VObject& obj, VArchiver& a,
                       VArchiver::AtMethodGetType getObjPos,
                       VArchiver::AtMethodSetType setObjPos)
{
	HRESULT 	hrErr;
	DWORD 	dwBytes;

    VComponentItem&        item = (VComponentItem&) obj;

	cv_assert(item.m_lpStorage == NULL);
	cv_assert(item.m_lpLockBytes == NULL);

	// RG: read number of bytes in the ILockBytes
	a >> dwBytes;

	// RG: allocate enough memory to read entire block
	HGLOBAL hStorage = ::GlobalAlloc(GMEM_SHARE|GMEM_MOVEABLE, dwBytes);
	if (hStorage == NULL)
		return;

	// RG: pump this into a buffer tit
	LPVOID lpBuf = ::GlobalLock(hStorage);
	cv_assert(lpBuf != NULL);
	DWORD dwBytesRead = a.read((char *)lpBuf, dwBytes);
	::GlobalUnlock(hStorage);

	// RG: throw exception in case of partial object
	if (dwBytesRead != dwBytes)
	{
		::GlobalFree(hStorage);
		return;
	}

	// RG: make an ILockbytes array on a VComponentItem with proper safeties
	hrErr = CreateILockBytesOnHGlobal(hStorage, TRUE, &item.m_lpLockBytes);
	if (hrErr != NOERROR)
	{
		::GlobalFree(hStorage);
		return;
	}
	cv_assert(item.m_lpLockBytes != NULL);
	cv_assert(StgIsStorageILockBytes(item.m_lpLockBytes) == NOERROR);

	// RG: open
	hrErr = ::StgOpenStorageOnILockBytes(item.m_lpLockBytes, NULL,
		STGM_SHARE_EXCLUSIVE|STGM_READWRITE, NULL, 0, &item.m_lpStorage);
	if (hrErr != NOERROR)

	{
		// RG: ILockBytes::Release will GlobalFree the hStorage
		item.m_lpLockBytes->Release();
		item.m_lpLockBytes = NULL;
		return;
	}


	LPOLEOBJECT         	pOleObject 	= NULL;
    LPOLECLIENTSITE			lpClientSite 		= item.getOleClientSite();

	hrErr = OleLoad(item.m_lpStorage, IID_IOleObject, lpClientSite, (LPVOID *)&pOleObject);

	if (hrErr != NOERROR)
	{
		cv_assert(pOleObject != NULL);
		// RG: ILockBytes::Release will GlobalFree the hStorage
		item.m_lpLockBytes->Release();
		item.m_lpLockBytes = NULL;
		return;
	}

	// rhw 4/15/96: I went this route instead of the realize route
	cv_assert(item.initFromNative(pOleObject, EmbeddedItem));

//    item.getOleObject().realize(pOleObject);
//    lpStorage->Release();
}


// ---------------------------------------------------------------------------
//
void VComponentItem::getFrom(VArchiver& a, VObject *parent, boolean doRealize)
{
    // rhw 4/15/96: Tie the Foundation object to the OLE object and vise-versa.
    // This needed to be and this looked like the place to do it
    lpVObject = parent;

	// evc 5/9/96
	// if the parent has a VComponent contained in it donot
	// overwrite it.  Buf if it doesnt, set it to the component item
	// This lets a getObject of a componentItem establish a
	// parent that is a window with an
	// already existing component window, OR establish a fresh
	// relationship between a componentItem and an up-until-now
	// non-component-item parent.


	if (! ((VWindow*) parent)->getComponentThis()){
		((VWindow*) parent)->setComponentThis(this);
	}
    getFrom(a);
    if (this->isA(VComponentControlCls)) {
        ((VComponentControl*)this)->init();
    }
}

#endif // CV_NOARCHIVER
//----------------------------------------------------------
// This allows custom behavior for IAdviseSink::OnViewChange
//----------------------------------------------------------
void 		 VComponentItem::uponViewAdvise(){
	RECT		bounds;
	VWinRect&   wr = this->bounds();
	RECT& rl= wr.getBounds(bounds, VOlePoint::Pixels);

	// Start of Sanjay's Code
	// Remove Windows offset from the containor's origin
	rl.right -= rl.left;
	rl.bottom -= rl.top;
	rl.left = 0;
	rl.top = 0;
	// End of Sanjay's Code
// Start of Sanjay's Code
	//InvalidateRect(getVThis()->getTopLevelParent()->hwnd(), &rl, TRUE);
// Start of Old Code
	InvalidateRect(getVThis()->hwnd(), &rl, TRUE);
// End of Old Code
// End of Sanjay's Code
}

void 		VComponentItem::uponDataAdvise(){
	// sometimes the size of the embedding changes
	RECT		bounds;
	VWinRect&   wr = this->bounds();
	RECT& rl= wr.getBounds(bounds, VOlePoint::Pixels);


	// Start of Sanjay's Code
	// Remove Windows offset from the containor's origin
	rl.right -= rl.left;
	rl.bottom -= rl.top;
	rl.left = 0;
	rl.top = 0;
	// End of Sanjay's Code
// Start of Sanjay's Code
	//InvalidateRect(getVThis()->getTopLevelParent()->hwnd(), &rl, TRUE);
// Start of Old Code
	InvalidateRect(getVThis()->hwnd(), &rl, TRUE);
// End of Old Code
// End of Sanjay's Code
}

void 		 VComponentItem:: uponInplaceObjectMoveOrResize(){
	// this is a virtual funtion that derived classes
	// can implement to assign custom
	// behavior when the inplace embedded object changes its size of
	// position within its parent window (the "itemwindow")
}

void 				VComponentItem::sizeCtoV(){
	// locally active means we are inplace or ui-inplace
	// active.  If we are not "in-place", there would be no
	// inplace-item window to resize.
	if (isLocallyActive()) {
		int x,y;
		getVThis()->sizeOfWin (&x, &y); // size of the itemWindow
// Start of Sanjay's Code
	//	RECTL newRect = {getPosition().x(), getPosition().y(),x,y};      // put it in a RECTL
// Start of Old Code
		RECTL newRect = {0,0,x,y};      // put it in a RECTL
// End of Old Code
// End of Sanjay's Code
		getOleInPlaceSite(). OnPosRectChange ((const RECT *) &newRect);
		// OnPosRectChange know what to do
	}
}
