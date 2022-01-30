// $Header:   Q:/views/win/vcs/appview.cpv   1.26   Mar 04 1997 17:16:34   gregm  $ 

//	appview.cpp
//
//	VAppView implementation
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
//	05/29/92 jmd 	Changed test in VAppView::show to StyleMaximized
//	04/12/93 jmd 	preened
//	05/14/93 pat	added constructors that take name arg
//	07/31/93 jmd	modified menu prompt logic
//	08/02/93 pat	added addMenu(); removeMenu()
//					updated call to VMenu() vis-a-vis build flag
//	08/04/93 jmd	rewrote removeMenu()
//	08/08/93 jmd	made VFrame const
//	08/09/93 jmd	made addMenu(); removeMenu() take pointers
//	08/09/93 jmd	removed free
//	08/16/93 jmd	made name const VString, added default name
//	08/18/93 jmd	added closeApp
//	08/23/93 jmd	changed "AppView" to "Main".
//	08/25/93 pat	included cllbckls.h, callback.h;
//	12/29/93 jmd	added help support (first cut)
//	01/08/94 dgm	added addMenu() check to call VStdMdiAppView::setMenuBar()
//					if this object is really a VStdMdiAppView.
//	02/26/94 jmd	added call to notifier in initHelp
//	03/04/94 pat	Win32 changes:  SetClassLong().
//	03/08/94 jmd	added __SC__ linkage stuff
//	03/24/94 pat	turned off menu refresh under destructor.
//	08/04/94 dgm	Added archiver support; straighten out styles;
//					added operator(); added initialize(); added setMenuBar().
//	08/31/94 dss	Ctl3d non-dialog support.
//	10/11/94 dss	Removed help functions. Don't archive help filename.
//			 		Removed PostQuitMessage() from close(). Notifier determines
//			 		when to quit the app. close() function is a notification.
//					call notifier when appViews are ctor'd and dtor'd.
//	11/11/94 dss	Archiver tags macros.
//	03/16/95 dss	added destroyed().
//	05/25/95 jld	changed show() match argument types for VWindow::show()
//	09/01/95 dss	VStyle changes.
//	09/29/95 dgm	Added constrainMinimumSize() and resizeClientArea().
//	10/05/95 dss	GetInfo(): test for only the class default styles that are
//					missing, not all the styles.
//	10/18/95 dgm	Removed constrainMinimumSize() and resizeClientArea().
//					Added getNonClientAreaSize().
//	11/01/95 dgm	Added #ifndef CV_NOOLE's.
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	01/09/96 dss	VComponent name change.
//	01/29/96 dgm	Fix in getSystemBorderSize().
//	01/29/96 dgm	Changed isTopLevel() to be a const function.
//	02/05/96 dss	getInfo(): added border and modalBorder to default style
//					test.
//	02/15/96 glm	removed destroyed()
//	02/16/96 pkt	Changed 'setAppIcon' and 'rscMenu' to take a const char*.
//	04/19/96 glm	removed v1.1 code/preened
//
//	02/11/97 pkt	Added StyleCaption, StyleSizable, StyleCloseBox,
//					StyleMaximize and StyleMinimize to the class default style.
//	02/04/97 pkt	Changed 'getInfo' to manage transient styles.
//	02/11/97 pkt	Changed 'getInfo' to handle default styles more rationally.
// ----------------------------------------------------------------------------

#include "appview.h"
#include "stdmdiap.h"		// for addMenu
#include "winclass.h"
#include "iterator.h"
#include "notifier.h"		// for closeApp

// Compound Document Includes
#ifndef CV_NOOLE
#include "cmpappvw.h"
#endif

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "callback.h"
#endif

//	The VAppView class is a subclass of VView that is designed
//	to be the top level view object for an application.  Typically, an
//	application will have only one instance of a subclass of VAppView. 
//	VAppView is the only subclass of the VWindow class that supports a
//	menu bar.
//

// ---------------------------------------------------------------------------
//
#if defined(__SC__) && !defined(DLL)
// force linkage of WinMain for Symantec
extern "C" int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
static FARPROC WinMainAddr = (FARPROC) WinMain;
#endif

// ---------------------------------------------------------------------------
//
defineClass(VAppView, VView)
defineArchiveRevision(VAppView, 0)

// ---------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER
defineCallback(VAppView, closeApp)
#endif

// ---------------------------------------------------------------------------
//
extern VClass *VPopupWindowCls;

// ---------------------------------------------------------------------------
//	Construct an named VAppView with the given frame and style.  This
//	VAppView has no menu yet.  Specify StyleMaximized to have this 
//	VAppView initially displayed maximized, or StyleMinimized to have 
//	this VAppView initially displayed in iconic form.  The default
//	style is StyleDefault.
//
VAppView::VAppView(const VString& wname, const VFrame &frame, 
					const VStyle& style)
	: VView()
{
	initialize();
	(*this)(frame, style, wname);
}

// ---------------------------------------------------------------------------
//	Construct a named VAppView of default size with no menu.
//	
VAppView::VAppView(const VString& wname)
	: VView()
{
	initialize();
	(*this)(VFrame(0.9F, 0.9F), StyleDefault, wname);
}

// ---------------------------------------------------------------------------
// Construct an VAppView with frame frame, style style with no
// menu.  Style may be StyleMaximized to have this VAppView initially
// displayed maximized, or StyleMinimized to have this VAppView initially
// displayed in iconinc form.
//
VAppView::VAppView(const VFrame& frame, const VStyle& style)
	: VView()
{
	initialize();
	(*this)(frame, style, "Main");
}

// ---------------------------------------------------------------------------
// Construct an VAppView of default size with no menu. If buildIt
// is non-zero, then the presentation window is built. Otherwise use the VView 
// operaator operator()(VFrame&, VWindow*) to build the presentation window.
//
VAppView::VAppView(int do_realize)
	: VView()
{
	initialize();
	if (do_realize) {
		(*this)(VFrame(0.9F, 0.9F), StyleDefault, "Main");
	}
}

// ---------------------------------------------------------------------------
//
void VAppView::operator()(const VFrame& frame, const VStyle& style)
{
	VView::operator()(frame, (VWindow *)0, style);
}

// ---------------------------------------------------------------------------
//
void VAppView::operator()(const VFrame& frame, VWindow *, const VStyle& style)
{
	VView::operator()(frame, (VWindow *)0, style);
}

// ---------------------------------------------------------------------------
//
void VAppView::operator()(const VFrame& frame, const VStyle& style,
						  const VString& name)
{
	VView::operator()(frame, (VWindow *)0, style, name);
}

// ---------------------------------------------------------------------------
//
void VAppView::operator()(const VFrame& frame, VWindow *, const VStyle& style,
						  const VString& name)
{
	VView::operator()(frame, (VWindow *)0, style, name);
}

// ---------------------------------------------------------------------------
VAppView::~VAppView()
{
	notifier->appViewDeleted(this);

	refreshMenu(FALSE);		//	do not redraw when we delete menu components

	if (menu) {
		delete menu;		// jmd 08/09/93 (was free)
		menu = 0;
		SetMenu(hWnd, 0);	
	}

	if (appIcon) {
		delete appIcon;
	}
}

// ---------------------------------------------------------------------------
//
void VAppView::initialize()
{
	menuUpdate	= 1;
	menu		= 0;
	appIcon		= 0;
	inStartCreation = FALSE;
	notifier->appViewConstructed(this);
}

// ---------------------------------------------------------------------------
//
VStyle& VAppView::getClassDefaultStyle()
{
#ifndef CVS_RATIONAL_DEFAULT
    return VView::getClassDefaultStyle();
#else
    static VWinStyle CV_NEARDATA classDefaultStyle(VView::getClassDefaultStyle() +
                                       setTransient + StyleCaption + StyleCloseBox +
									   StyleMinimize + StyleMaximize + StyleSizable);

    return classDefaultStyle;
#endif
}

// ---------------------------------------------------------------------------
//
VStyle& VAppView::getDefaultStyle() const
{
    return getClassDefaultStyle();
}

// ---------------------------------------------------------------------------
// Initialize the VAppView VWinInfo structure.
//
void VAppView::getInfo(VWinInfo& appinfo)
{
	VView::getInfo(appinfo);

#ifdef CVS_RATIONAL_DEFAULT
	boolean rational = TRUE;
#else
	boolean rational = FALSE;
#endif	
	if (   rational
		&& style.contains(StyleCaption)
		&& style.contains(StyleCloseBox)
		&& style.contains(StyleMinimize)
		&& style.contains(StyleMaximize)
		&& style.contains(StyleSizable)) {

		// Eliminate the styles that are implicit in WS_OVERLAPPEDWINDOW.
		appinfo.style &= ~(WS_BORDER | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
										// title bar. border, sizable (thick border), system menu, min & maximize
		appinfo.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	}
	else if (   rational
			 || style.contains(StyleCaption)
			 || style.contains(StyleCloseBox)
			 || style.contains(StyleMinimize)
			 || style.contains(StyleMaximize)
			 || style.contains(StyleSizable)
			 || style.contains(StyleBorder)
			 || style.contains(StyleModalBorder)) {

		// A peculiar explicit style is present that abrogates the
		// ordinary VAppView defaults.

		appinfo.style &= ~(WS_CHILD | WS_CLIPSIBLINGS);
		appinfo.style |= WS_OVERLAPPED; // title bar & border
	}
	else {
										// title bar. border, sizable (thick border), system menu, min & maximize
		appinfo.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;

#ifdef CVS_USE_TRANSIENT
		style += setTransient + StyleCaption + StyleCloseBox +
				  StyleMinimize + StyleMaximize + StyleSizable;
#else
		style += StyleCaption + StyleCloseBox +
				  StyleMinimize + StyleMaximize + StyleSizable;
#endif
	}
}
// ---------------------------------------------------------------------------
//
UINT VAppView::getCtlColor()
{
	return VView::getCtlColor();
}

// ---------------------------------------------------------------------------
// Called when the application window should be freed. 
// An VAppView object will typically check with the user to see
// if it is OK to quit before closing.  When the last VAppView object
// is closed, the application is terminated. 
//
boolean VAppView::close()
{
#if 0
	if (children) {
		// poll the kiddies
		DO(children, VView, child)
			if (child->isA(VViewCls) && child->close()) {
				return TRUE;
			}
		END 
	}
#endif
                    
#ifndef CV_NOOLE
	if (getComponentThis()) {
		// if the component overrides close, then return abort close.
		if (getComponentThis()->close()) {
			return TRUE;
		}
	}
#endif

	if (vectorEvent(VWindow::Quit)) {
		return TRUE;
	}

	return quit();
}

// ---------------------------------------------------------------------------
// Called when the user has clicked the close box on the application 
// title bar or when closeApp() is called. This method is usually
// sub classed to override the default behavior, which is to allow
// application quit.
//
boolean VAppView::quit()
{
#if 0
	if (children) {
		// poll the kiddies
		DO(children, VView, child)
			if (child->isA(VViewCls) && child->quit()) {
				return TRUE;
			}
		END 
	}
#endif

#ifndef CV_NOOLE
	if (getComponentThis()) {
		// if the component overrides quit, then return abort close.
		if (getComponentThis()->quit()) {
			return TRUE;
		}
	}
#endif

	return FALSE;
}

// ---------------------------------------------------------------------------
// Called by the VNotifier when the menubar of this VAppView has been
// clicked on. Provides an opportunity to initialize menu items by setting
// attributes such as the enable/disable state. 
// The implementation of this method in VAppView does nothing.
//
boolean VAppView::menuSelected(VPopupMenu *m)
{
	if (m) {
		m->selected();
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Add m to the menubar of this VAppView where m is of class
// VMenuItem or VPopupMenu. Construct a VMenu object if one is not
// already constructed.
//
void VAppView::addPopup(VObject *m)
{
	if (!menu) {
		menu = new VMenu(TRUE);
		menu->app = this;
		SetMenu(hWnd, menu->hMenu);
	}
	menu->addItem(m);
	if (menuUpdate) {
		drawMenu();  
	}
}

// ---------------------------------------------------------------------------
// Remove m from the menubar of this VAppView
// where m is of class VMenuItem or VPopupMenu.
//
void VAppView::removePopup(VObject *m)
{
	menu->removeItem(m);
	if (menuUpdate) {
		drawMenu();  
	}
}

// ---------------------------------------------------------------------------
// The implementation of this method in VAppView ignores focus events.
//
boolean VAppView::nextFocus() 
{ 
	return FALSE;
}

// ---------------------------------------------------------------------------
// The implementation of this method in VAppView ignores focus events.
//
boolean VAppView::prevFocus() 
{ 	
	return FALSE; 
}

// ---------------------------------------------------------------------------
// If b is TRUE, then update (that is, redraw the menubar) the VMenu
// of this VAppView when a VPopupMenu is added or removed, otherwise dont
// update the menubar.  The default action is to update.
//
void VAppView::refreshMenu(boolean b)
{
	if (!menuUpdate && b) {
		drawMenu();
	}
	menuUpdate = b;
}

// ---------------------------------------------------------------------------
// Set the the application's icon from the resource specified by
// the C string s.  This method will create an VIcon object and then
// destroy it when this VAppView is destroyed. Return TRUE if successful, 
// otherwise return FALSE.
//
boolean VAppView::setAppIcon(const char *s)
{
	appIcon = new VIcon(s);
	return setAppIcon(appIcon);
}

// ---------------------------------------------------------------------------
// Redraw the menubar of this VAppView.
//
void VAppView::drawMenu()
{
	DrawMenuBar(hWnd);
}

// ---------------------------------------------------------------------------
// Return TRUE if this application has been iconized, otherwise return FALSE.
//
boolean VAppView::isIconic()
{
	return IsIconic(hWnd);
}

// ---------------------------------------------------------------------------
// Called by the VNotifier when this VAppView has been iconized. 
// The width and height of the icon is given in w and h, respectively.  
//
boolean VAppView::iconized(int w, int h)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
// Construct the menubar and popup menus from the MS Windows or OS/2 PM 
// resource name and set the callbacks for the menu items for object client
// from the two arrays ids and mthds.  Ids is an array of integer values
// corresponding to the menu resource ids as they occur in the resource
// script.  Mthds is an array of the methods to be called on object client
// when these items are selected. 
// There must be a one-to-one corespondence between the resource id array
// and the callback method array. The resource array, ids, must have -1 in the
// last entry and, therefore, a -1 must not be used as a menu resource id.  The
// resource id for a menu separator must be set to zero in the ids array, and
// the coresponding element in the mthds array must be set to 0.
//
void VAppView::rscMenu(const char *name, int *ids, 
						method *mthds, VObject *client)
{
	VMenu *prevMenu = menu;
	menu = new VMenu(name, ids, mthds, client);
	menu->app = this;
	SetMenu(hWnd, menu->hMenu);
	if (menuUpdate) {
		drawMenu();
	}
	if (prevMenu) {
		prevMenu->app = 0;
		delete prevMenu;			// jmd-08/09/93 (was free)
	}  
}

// ---------------------------------------------------------------------------
// Show this VAppView maximized if style is StyleMaximized,
// otherwise show this VAppView minimized if style is StyleMinimized.
//
void VAppView::show(const VStyle& style)
{
	if (style.contains(StyleMaximized)) {
		ShowWindow(hWnd, SW_SHOWMAXIMIZED);
		UpdateWindow(hWnd);
	}
	else if (style.contains(StyleMinimized)) {
		ShowWindow(hWnd, SW_SHOWMINIMIZED);  
		UpdateWindow(hWnd);
	}
	else {
		VWindow::show(TRUE);
	}
}

// ---------------------------------------------------------------------------
// Replace the VMenuItem or VPopupMenu object m at the index 
// position of the menubar (menu indices are counted from zero).
//
void VAppView::replacePopup(VObject *m, int index)
{
	boolean mu = menuUpdate;
	menuUpdate = FALSE;
	menu->removeItem(menu->menuAt(index));
	menu->addItem(m, index);
	menuUpdate = mu;
	if (menuUpdate) {
		drawMenu();  
	}
}

// ---------------------------------------------------------------------------
//	Insert the VMenuItem or VPopupMenu object m at the position
// index of the menubar (menu indices are counted from zero).
//
void VAppView::insertPopup(VObject *m, int index)
{
	menu->addItem(m, index);
	if (menuUpdate) {
		drawMenu();  
	}
}

// ---------------------------------------------------------------------------
// Set this application/s icon to VIcon object icon.  Return TRUE 
// if successful, otherwise return FALSE. Note: The VIcon object icon will
// be destroyed when this VAppView object is destroyed.
//
boolean VAppView::setAppIcon(VIcon *icon)
{
	appIcon = icon;
	HICON hI = appIcon->handle();
	if (hI) {
#ifdef CV_WIN32
		SetClassLong(hWnd, GCL_HICON, (LONG) hI);
#else
		SetClassWord(hWnd, GCW_HICON, (WORD) hI);
#endif
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Callback for closing the AppView
// Designed for use in the application menu
//
boolean VAppView::closeApp(VMenuItem *m)
{
	notifier->closeWindow(this);
	return TRUE;
}

// ---------------------------------------------------------------------------
// Set the status bar prompt to msg
// if msg == 0, then restore original prompt
// (derived classes are responsible for actually doing this)
//
boolean VAppView::displayPrompt(VString *msg)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
//
//
boolean VAppView::isTopLevel() const
{
	return TRUE;
}

// ---------------------------------------------------------------------------
// Terminate modal operating mode and cause a return from the 
// 'modal()' method call.
// 
void VAppView::endModal()
{

	// Because appViews are more persisent than dialogs, make the
	// default behavior not hide the appView.
	notifier->endModal(FALSE);
}

// ---------------------------------------------------------------------------
// Set this VAppView into modal operating mode which disables
// mouse/keyboard events to all windows within the application except
// this VAppView.  This VAppView remains modal (that is, does not return)
// until endModal() is called.
//
void VAppView::modal()
{
	notifier->startModal(this);
}

// ---------------------------------------------------------------------------
//
void VAppView::getSystemBorderSize(int *width, int *height) const
{
	VView::getSystemBorderSize(width, height);

	if ((height != 0) && (menu != 0)) {
		*height += GetSystemMetrics(SM_CYMENU) + 1;
	}
}

// ---------------------------------------------------------------------------
//	Adds the popups of the given menu to this application views menu object.
//
void VAppView::addMenu(VMenu *m)
{
	if (this->isA(VStdMdiAppViewCls)) {
		((VStdMdiAppView *)this)->setMenuBar(m);
		return;
	}
	if ((m == 0) || ((menu != 0) && (m == menu))) {
		//
		// Attempt to add from same menu.
		//
		return;
	}
	DO (m->mitems(), VObject, item)
		addPopup(item);
	END
}

// ---------------------------------------------------------------------------
//
void VAppView::setMenuBar(VMenu *m)
{
	addMenu(m);
}

// ---------------------------------------------------------------------------
// Removes the popups of the given menu from this application view's menu
// object.
//
void VAppView::removeMenu(VMenu *m)
{
	if (menu != 0 && m != 0) {
		if (m == menu) {
			while (menu->mitems().count() > 0) {
				menu->removeItem(menu->menuAt(0));
			}
		}
		else {
			DO (m->mitems(), VObject, item)
				menu->removeItem(item);
			END
		}
	}

	if (menuUpdate) {
		drawMenu();  
	}
}


// ===========================================================================
#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VAppView::putTo(VArchiver& a)
{
	VAppView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VAppView::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VView::putTo(a, FALSE);

	// Write the object version tag.

	a << VAppViewArchiveTag;

	// Write any object specific data.
	
	long dummy = 0;
	a << dummy;							// icon

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VAppView::getFrom(VArchiver& a)
{
	// Read the base object data.

	VView::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;

	if (!archiveTagOk(tag, VAppViewArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VAppViewArchiveTag));
		return;
	}

	// Read any object specific data.

	long dummy = 0;
	a >> dummy;								// icon
}

// ---------------------------------------------------------------------------
//
void VAppView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VAppView::getFrom(a);

	if (do_realize) {
		VAppView::realize((VWindow *)parent);
	}

	VWindow::getChildrenFrom(a, do_realize);
}

// ---------------------------------------------------------------------------
//
void VAppView::realize(VWindow *parentw)
{
	VView::realize(parentw);
}

// ---------------------------------------------------------------------------

#endif // CV_NOARCHIVER
// ===========================================================================

