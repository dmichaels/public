// $Header:   Q:/views/win/vcs/cmpappvw.cpv   1.14   Mar 21 1997 08:42:42   gregm  $

//	cdappvw.cpp
//
//  Compound Document AppView
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
//	Revision History:
//	-----------------
//	01/30/95 dss	Initial release.
//  12/08/95 dss    Added defineClass.
//	01/03/96 dss	adjustMdiClient(): VStdAppView support. Use attachWin.
//	01/09/96 dss	VComponent name change.
//	01/30/96 dss	Permanently comment out ole UI init/uninit.
//  04/22/96 evc    Fixed setMenu in the MDI case.  Added
// 					a bit of protection against 0 in  closeMenuItem
//  05/10/96 evc   more defensive programming in ::itemActivate
//  05/28/96 evc   removed a 'cv_assert(lpCloseMenuItem)'
//	--------------------------------------------------------------------------

#include "pre.h"
#include "stdappvw.h"
#include "mdiappvw.h"
// Sanjay Start
#include "cmpcntl.h"
// Sanjay End

defineClass( VComponentAppView, VComponentView)

defineCallback(VComponentAppView, closeMenuItem)

//**********************************************************************
//
void VComponentAppView::init(VPopupMenu *editMenu)
{
	lpCurrentOleObjectVerbMenu = 0;
	lpCloseMenuItem = 0;
	lpFileMenu = 0;
	
	this->lpEditMenu = editMenu;

	if (editMenu) {
		editMenu->uponSelect(this, methodOf(VComponentAppView, editMenuSelected));
	}

//	bOleInitialized = FALSE;

//	menuBars.push(getVThis()->getMenu());

	// delay before dragging should start, in milliseconds
	nDragDelay = GetProfileInt(
			"windows",
			"DragDelay",
			DD_DEFDRAGDELAY
	);

	// minimum distance (radius) before drag should start, in pixels
	nDragMinDist = GetProfileInt(
			"windows",
			"DragMinDist",
			DD_DEFDRAGMINDIST
	);

	// delay before scrolling, in milliseconds
	nScrollDelay = GetProfileInt(
			"windows",
			"DragScrollDelay",
			DD_DEFSCROLLDELAY
	);

	// inset-width of the hot zone, in pixels
	nScrollInset = GetProfileInt(
			"windows",
			"DragScrollInset",
			DD_DEFSCROLLINSET
	);

	// scroll interval, in milliseconds
	nScrollInterval = GetProfileInt(
			"windows",
			"DragScrollInterval",
			DD_DEFSCROLLINTERVAL
	);

	// Do this BEFORE init'ing any ole objects.
	// can't do this here. Must start BEFORE object is constructed.
//	OleStart(notifier->inst());

}

//**********************************************************************
//
VComponentAppView::VComponentAppView()
	: OleInPlaceAppView(0)
{
	CDBG(	<< "In VComponentAppView's Default Constructor" << endl);

	init(0);
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
VComponentAppView::VComponentAppView(LPVAPPVIEW lpAppView, VPopupMenu *editMenu,
					   VString *path)
	: VComponentView(lpAppView, new VOleInPlaceAppView(*this))
//	, OleInPlaceFrame(*this)
//	, OleInPlaceAppView(new VOleInPlaceAppView(*this)) // OleInPlaceFrame sub class
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In VComponentAppView's Constructor" << endl);

	init(editMenu);
	
	OleInPlaceAppView = (VOleInPlaceAppView *) getOleWindowBase();

	aggregate(OleInPlaceAppView);

	// !!!dss this is a test
	// CD aggregate parents are created with a ref count of one
	AddRef();
	      
	// create storage, if path present     
	if (path) {
		// create storage -> no parent!           
		createStorage(0, path);
	}		
	// Sanjay Start
	// Added to set activation mode to locallyActive during initialization
    VOrdCollect *lpKids = getVThis()->childrenOf();
    if (lpKids == 0)
        return;
    DO (*(getVThis()->childrenOf()), VWindow, lpWin)
        VComponentWindow   *lpCDWin = lpWin->getComponentThis();
        if (lpCDWin && (lpCDWin->isA(VComponentItemCls) || lpCDWin->isA(VComponentControlCls))) {
    		if ((notifier->getPainterMode() == TRUE) && LPVCOMPONENTITEM(lpCDWin)->canLocallyActivate())
			    LPVCOMPONENTITEM(lpCDWin)->setActivationState(VComponentItem::locallyActive);
        }
    END	   
	// Sanjay End
}

//**********************************************************************
//
VComponentAppView::~VComponentAppView()
{
	CDBG(	<< "In VComponentAppView's Destructor" << endl);

	if (lpCloseMenuItem) {
		delete lpCloseMenuItem;
		lpCloseMenuItem = 0;
	}

	// can't do this here. Must stop AFTER object is destructed.
//	OleStop();
}

//**********************************************************************
//
// Called when a VComponentItem becomes active (object active, not the active
// window).
//
boolean VComponentAppView::itemActivate(LPVCOMPONENTITEM lpItem)
{
	if (getVThis()->isA(VStdMdiAppViewCls)) {
		VStdMdiAppView *lpMdiAppView = (VStdMdiAppView *) getVThis();

		lpMdiAppView->showToolBar(FALSE);
	}

	// File Menu
	VMenu * pMenu = getVThis()->getMenu();
	VPopupMenu		*fileMenu = 0;
	if (pMenu){
		fileMenu = 	 pMenu ->menuAt(0);
	}
	//VPopupMenu		*fileMenu = getVThis()->getMenu()->menuAt(0);

	if (fileMenu) {
		if (lpCloseMenuItem == 0) {
			lpCloseMenuItem = new VMenuItem("Deactivate", 
								   			MULTI_INHERIT_HACK(this), 
											methodOf(VComponentAppView, closeMenuItem));
		}
	fileMenu->addItem(lpCloseMenuItem);
	}

	return FALSE;
}

//**********************************************************************
//
// Called when a VComponentItem becomes deactive (Object deactive, not the active 
// window).
//
boolean VComponentAppView::itemDeactivate(LPVCOMPONENTITEM lpItem)
{
// Start of Sanjay's Code
// Old Code
	//cv_assert(getVThis()->isA(VAppViewCls));
// End of Old Code
// End of Sanjay's Code
	restoreMenuBar();

	if (getVThis()->isA(VStdMdiAppViewCls)) {
		VStdMdiAppView *lpMdiAppView = (VStdMdiAppView *) getVThis();

		lpMdiAppView->showToolBar(TRUE);
	}
	
	// File Menu
	
	VMenu * pMenu = getVThis()->getMenu();
	VPopupMenu		*fileMenu = 0;
	if (pMenu){
		fileMenu = 	 pMenu ->menuAt(0);
	}
//	VPopupMenu		*fileMenu = getVThis()->getMenu()->menuAt(0);
	if (fileMenu) {
		if (lpCloseMenuItem){
			fileMenu->removeItem(lpCloseMenuItem);
		}
	}

	getVThis()->bringToTop();
	getVThis()->takeFocus(); 

	return TRUE;
}

boolean VComponentAppView::bOleInitialized = FALSE;

//********************************************************************
//
void VComponentAppView::OleStart(HINSTANCE hInstance)
{
	DWORD dwVer = OleBuildVersion();
	LPMALLOC lpMalloc = NULL;

	// check to see if we are compatible with this version of the libraries
	if (HIWORD(dwVer) != rmm || LOWORD(dwVer) < rup) {
#if defined( CV_DEBUG)
		CDBG(	<< "WARNING: Incompatible OLE library version" << endl);
#endif
		return;
	}

//!!!dss: to do: why doesn't this work under WIN32?
#if defined( CV_DEBUG) && !defined(CV_WIN32)
	/* OLE2NOTE: Use a special debug allocator to help track down
	**    memory leaks.
	*/
	OleStdCreateDbAlloc(0, &lpMalloc);
#endif

	if (OleInitialize(lpMalloc) == NOERROR) {
		bOleInitialized = TRUE;
	}
	else {
		cv_assert(0);
		return;
	}

#if defined( CV_DEBUG)
	/* OLE2NOTE: release the special debug allocator so that only OLE is
	**    holding on to it. later when OleUninitialize is called, then
	**    the debug allocator object will be destroyed. when the debug
	**    allocator object is destoyed, it will report (to the Output
	**    Debug Terminal) whether there are any memory leaks.
	*/
	if (lpMalloc) lpMalloc->Release();
#endif

#if 0
	// Initialization required for VOLEUI.DLL and VOLEUI32.DLL if we 
	// ever decide to statically link it in. Currently this function is
	// not needed because they are DLL's.

	// NOTE: init OLE BEFORE the UI library.
	if (!OleUIInitialize(notifier->inst(), notifier->hPrevInst))
		{
		cv_assert(0);
		}
#endif

}

//********************************************************************
//
void VComponentAppView::OleStop(void)
{
	if (bOleInitialized) {

#if 0
		// De-initialization for VOLEUI.DLL and VOLEUI32.DLL. Just like 
		// OleUIInitialize, this function is needed ONLY if we are using 
		// the static link version of the OLE UI libraries.

		// NOTE: uninit the UI library BEFORE uninit'ing OLE.
		OleUIUninitialize();
#endif

		OleUninitialize();

		bOleInitialized = FALSE;
	}
	
}

//********************************************************************
//
boolean VComponentAppView::editMenuSelected(VPopupMenu *editMenu)
{
	cv_assert(editMenu);
	cv_assert(editMenu->isA(VPopupMenuCls));

	LPVCOMPONENTITEM lpCDItem = getFocusedComponentItem();

	// if a focused cditem exists and its verb's haven't been added, 
	// then add them to the end of the edit menu.
	//
	// TO DO: if the current ole obj verb menu is not the same
	// as the currently focused ole object's verb menu,
	// add the currently focused ole object's verb menu to the edit
	// menu.
	if (lpCDItem && lpCurrentOleObjectVerbMenu == 0) {

		lpCurrentOleObjectVerbMenu = 
			lpCDItem->addVerbs(editMenu, int(editMenu->mitems().count()));
	}

	return TRUE;
}

#if 0
//********************************************************************
//
LPVDROPWINDOW VComponentAppView::getFocusedDropWindow()
{

	LPVWINDOW lpWin = getVThis()->getChildFocus();

	while (lpWin) {

		if (lpWin->isA(VComponentItemCls)) {
			return LPVDROPWINDOW(lpWin);
		}
		
		lpWin = lpWin->getChildFocus();
	}

	return NULL;
}
#endif

//********************************************************************
//
void VComponentAppView::loseFocus()
{
	lpCurrentOleObjectVerbMenu = 0;
}

//********************************************************************
//
// Replaces VAppView::setMenuBar() which just calls addMenu() which
// appends the popups from the argument menu to the current menubar.
//
// Saves the current menubar on the menu bar stack.
//
void VComponentAppView::setMenuBar(VMenu *m)
{
	// rhw 4/15/96: "m" can be NULL!  Had to guard this code by checking to
	// make sure it was not NULL before using it.
	if (m){
		m->setApp(getVThis());
		cv_assert(m->hmenu());
		this->setMenuBar((HMENU) m->hmenu());
	}
}

//********************************************************************
//
// Replaces VAppView::setMenuBar() which just calls addMenu() which
// appends the popups from the argument menu to the current menubar.
//
// Saves the current menubar on the menu bar stack.
//
void VComponentAppView::setMenuBar(HMENU m)
{
	cv_assert(m);

	if (getVThis()->isA(VMdiAppViewCls)) {
	#if defined(CV_WIN16)

		::SendMessage(((VMdiAppView FAR*)getVThis())->clientOf()->hwnd(),
			WM_MDISETMENU, 0, MAKELONG(m, 0));
	#else
		 SendMessage(((VMdiAppView FAR*)getVThis())->clientOf()->hwnd(),
			WM_MDISETMENU, (WPARAM) (HMENU)m , (LPARAM) 0);

	#endif
	}
	
	// else SDI
	else {
		::SetMenu(getVThis()->hwnd(), m);
	}

	getVThis()->drawMenu();
}

// ---------------------------------------------------------------------------
//
// returns the previous menu bar after restoration.
//
#if 0
VMenu *VComponentAppView::restoreMenuBar()
{
	VMenu	*prevMenuBar = (VMenu *) menuBars.pop();

	cv_assert(prevMenuBar);

	setMenuBar(prevMenuBar);

	return prevMenuBar;
}
#else
void VComponentAppView::restoreMenuBar()
{
	setMenuBar(getVThis()->getMenu());
}
#endif

//**********************************************************************
//
boolean VComponentAppView::deactivate(VWindow *w)
{
	LPOLEINPLACEACTIVEOBJECT lpIPActObj = 
		getOleInPlaceUIWindow().getOleInPlaceActiveObject();

	// if there's an object in place active, let it know we are deactivating.
	if (lpIPActObj) {
		lpIPActObj->OnFrameWindowActivate(FALSE);
	}

	// always return FALSE to let the OS process the deactivation:
	// we are using this message as a notification, not overriding it.
	return FALSE;
}

//**********************************************************************
//
//	Called when the object get keyboard focus.
//
boolean VComponentAppView::activate(VWindow *w)
{
	LPOLEINPLACEACTIVEOBJECT lpIPActObj = 
		getOleInPlaceUIWindow().getOleInPlaceActiveObject();

	// if there's an object in place active, let it know we are activating.
	if (lpIPActObj) {
		lpIPActObj->OnFrameWindowActivate(TRUE);
	}
	
	// always return FALSE to let the OS process the deactivation:
	// we are using this message as a notification, not overriding it.
	return FALSE;
}

//**********************************************************************
//
// TO DO: unOLEize this method: LPCBORDERWIDTHS -> LPCTOOLSPACE
//
STDMETHODIMP VComponentAppView::setToolSpace(LPCBORDERWIDTHS lpToolSpace)
{
	CDBG(	<< "In VComponentAppView::setToolSpace, tool space = "); // NO endl! 
	
    if (lpToolSpace) {
		CDBG(  << *lpToolSpace << endl);
	} else {
		CDBG(  << "<null>" << endl);
	}

	return adjustMdiClient(lpToolSpace);
}

//**********************************************************************
//
// TO DO: unOLEize this method: LPCBORDERWIDTHS -> LPCTOOLSPACE
//
STDMETHODIMP VComponentAppView::adjustMdiClient(LPCBORDERWIDTHS lpToolSpace)
{
	VAppView	*lpAppView = getVThis();
	VWindow		*lpClient = 0;

	if (lpAppView->isA(VMdiAppViewCls)) {
		lpClient = ((VMdiAppView*)lpAppView)->clientOf();
	}
	else if (lpAppView->isA(VStdAppViewCls)) {
		lpClient = ((VStdAppView*)lpAppView)->getClientWindow();
	}

	if (lpClient) {
		if (lpToolSpace) {
			lpAppView->attachWin(lpClient, VAttach::Top,	VAttach::Parent, 
				lpToolSpace->top);
			lpAppView->attachWin(lpClient, VAttach::Left,	VAttach::Parent, 
				lpToolSpace->left);
			lpAppView->attachWin(lpClient, VAttach::Bottom,	VAttach::Parent, 
				lpToolSpace->bottom);
			lpAppView->attachWin(lpClient, VAttach::Right,	VAttach::Parent, 
				lpToolSpace->right);
		}
		else {
			lpAppView->attachWin(lpClient, VAttach::Top,	VAttach::Parent, 0);
			lpAppView->attachWin(lpClient, VAttach::Left,	VAttach::Parent, 0);
			lpAppView->attachWin(lpClient, VAttach::Bottom,	VAttach::Parent, 0);
			lpAppView->attachWin(lpClient, VAttach::Right,	VAttach::Parent, 0);
		}

		lpAppView->frameChildren();

#if 0
		CDBG(	<< "    new client rect = (" << left << ", " << top
				<< ", " << right - left << ", " << bottom - top << ')' 
				<< endl);
#endif
	}

	return NOERROR;
}

//********************************************************************
//
boolean VComponentAppView::createStorage(VComponentObject *storageParent,
								  VString *lpStorageName)
{
	// appViews are always the root storage
	// DEBUG mode: assert no parent, RETAIL mode: ignore parent
	cv_assert(storageParent == 0);

	// only create if path specified. This overrides the VView behavior.
	if (lpStorageName) {
		// bypass VView::createStorage().
		return VComponentWindow::createStorage(0, lpStorageName);
	}

	return FALSE;
} 

//********************************************************************
//
boolean VComponentAppView::closeMenuItem(VMenuItem *)
{
	// To DO: CDItem should have an activate/deactivate method
	// Get rid of the OLE!
	//cv_assert(getFocusedComponentItem());


// Sanjay Start
    LPVCOMPONENTITEM       pCDItem;
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
        	lpOleInPlaceObject->InPlaceDeactivate();
// Start Old Code
	//if (getFocusedComponentItem()){
		//getFocusedComponentItem()->getOleInPlaceSite().getOleInPlaceObject().UIDeactivate();
// End Old Code
// Sanjay End
	}
	return TRUE;
}
