// $Header:   Q:/views/win/vcs/mdiview.cpv   1.14   Mar 04 1997 17:55:20   gregm  $ 

//	mdiview.cpp
//
//	VMdiView implementation
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
//	10/12/92 jmd	preened
//	10/12/92 jmd	removed FREESELF macro
//	06/24/93 jmd	preened, removed free()
//	08/08/93 jmd	made VFrame const
//	08/16/93 jmd	renamed name to iconname, made it const
//	08/09/94 dgm	Added archiving support; initialize pName.
//	08/25/94 dgm	Fix in realize().
//	08/31/94 dss	operator(): added StyleSizable.
//	09/05/94 dss	getInfo(): exStyle initialization.
//	11/11/94 dss	Archiver tags macros.
//	09/01/95 dss	VStyle changes.
//	11/01/95 dgm	Added #ifndef CV_NOOLE's.
//	11/20/95 dss	operator(): removed static VStyles variables as it was
//					causing run-time error in dtor.
//	01/09/96 dss	VComponent name change.
//	06/06/96 glm	uncommented destroyWin in destructor
//	08/06/96 pkt	reinstated close() calling quit().
//	02/04/97 pkt	Implemented 'getClassDefaultStyle'.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "mdiview.h"
#include "mdiappvw.h"
#include "notifier.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// Compound Document Objects
#ifndef CV_NOOLE
#include "cmpview.h"
#endif

// ---------------------------------------------------------------------------
//
defineClass(VMdiView, VView)
defineArchiveRevision(VMdiView, 0)

// ---------------------------------------------------------------------------
// Construct an VMdiView with no presentation window.
//
VMdiView::VMdiView()
{
	pName = 0;
}

// ---------------------------------------------------------------------------
// Construct an VMdiView child window of default size with icon name
// name, and parent window win (must be an VMdiAppView object). Using the
// StyleDefault style flag will produce an Mdi child window with the 
// System menu, maximize/minimize controls, title bar, and the thick frame 
// sizable border.  The only other valid styles for VMdiView objects are
// StyleHorizontal and/or StyleVertical.
// 
VMdiView::VMdiView(const VStyle& style, const char *iconname, VWindow *win, char *title)
{
	pName = 0;
	(*this)(iconname, VFrame(0, 0, 0, 0), win, style, title);
}

// ---------------------------------------------------------------------------
// Construct a VMdiView with VFrame frame, parent win (must be
// a VMdiAppView class), style style and title title. The VIcon to be used
// is given by name.  A VFrame object with all zero parameters, that is, 
// VFrame(0,0,0,0), will produce an VMdiView object of default size. 
//
VMdiView::VMdiView(const char *iconname, const VFrame &frame,
				   VWindow *win, const VStyle& style, char *title)
{
	pName = 0;
	(*this)(iconname, frame, win, style, title);
}

// ---------------------------------------------------------------------------
// 
VMdiView::VMdiView(const VString& name,
				   const VStyle& style, const char *iconname,
				   VWindow *win, char *title)
{
	pName = 0;
	(*this)(iconname, VFrame(0, 0, 0, 0), win, style, title, name);
}

// ---------------------------------------------------------------------------
//
VMdiView::VMdiView(const VString& name,
				   const char *iconname, const VFrame &frame,
				   VWindow *win, const VStyle& style, char *title)
{
	pName = 0;
	(*this)(iconname, frame, win, style, title, name);
}

// ---------------------------------------------------------------------------
//
VMdiView::~VMdiView()
{
	notifier->destroyWin(this);
}

// ---------------------------------------------------------------------------
// Called when this VMdiView becomes the active Mdi child window. If
// another VMdiView window is being deactivated, then its pointer is passed 
// to this event in the deactivated parameter.  Otherwise the deactivated
// parameter will be NULL. 
//
boolean VMdiView::activate(VView *deactivated)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
// Called when this VMdiView looses active status.  The VMdiView
// window being activated is passed as a parameter to this event.
//
boolean VMdiView::deactivate(VView *activated)
{
	return FALSE;
}

// ---------------------------------------------------------------------------
// Return the WinMdiChild window type attribute.
//
enum WinType VMdiView::winType() 
{ 
	return WinMdiChild;
}

// ---------------------------------------------------------------------------
// Return TRUE if this MDI Child VView has been maximized, otherwise
// return FALSE.
//
boolean VMdiView::isMaximized()
{
	return IsZoomed(hWnd);
}

// ---------------------------------------------------------------------------
// 	Return TRUE if this MDI Child VView has been iconized (that is, 
// 	minimized), otherwise return FALSE.
//
boolean VMdiView::isIconic()
{
	return IsIconic(hWnd);
}

// ---------------------------------------------------------------------------
boolean VMdiView::close()
{
#ifndef CV_NOOLE
	if (getComponentThis()) {
		// if CDThis overrides close, then return abort close.
		if (getComponentThis()->close()) {
			return TRUE;
		}
	}
#endif

	if (!quit()) {
		return FALSE; 	// close and free this window
	}
	else {
		return TRUE;
	}
}

// ---------------------------------------------------------------------------
// Minimize this VMdiView child window.
//
void VMdiView::minimize()
{
	ShowWindow(hWnd, SW_SHOWMINIMIZED);  
	UpdateWindow(hWnd);
}

// ---------------------------------------------------------------------------
// Maximize this VMdiView child window.
//
void VMdiView::maximize()
{
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);
}

// ---------------------------------------------------------------------------
// Build the MS VWindow presentation space associated with this 
// VMdiView with VFrame frame, parent win (must be a VMdiAppView class),
// style style and title title. The VIcon to be used is given by name.
// This method should be used in constructors of subclasses of VMdiView instead
// of an initializer list to insure that the activate() event is generated
// on creation.  A VFrame object with all zero parameters, that is, 
// VFrame(0, 0, 0, 0), will produce an VMdiView object of default size. 
//
void VMdiView::operator()(const char *iconname, const VFrame &frame,
						  VWindow *win, const VStyle& style, char *title)
{
	pName = iconname;
	if (title) {
		sTitle = title;
	}

	VView::operator()(frame, ((VMdiAppView *)win)->clientOf(),
#if defined(CVS_USE_TRANSIENT) && defined(CVS_LATER_DEFAULT)
							// extra styles handled by default in VWindow::build
							style);
#else
							style + 
							StyleSizable + 
							StyleCaption + 
							StyleMinimize + 
							StyleMaximize + 
							StyleCloseBox);
#endif
}

// ---------------------------------------------------------------------------
//
void VMdiView::operator()(const char *iconname, const VFrame &frame,
						  VWindow *win, const VStyle& style, char *title,
						  const VString& name)
{
	pName = iconname;
	if (title) {
		sTitle = title;
	}

	VView::operator()(frame, ((VMdiAppView *)win)->clientOf(),
#if defined(CVS_USE_TRANSIENT) && defined(CVS_LATER_DEFAULT)
							// extra styles handled by default in VWindow::build
							style);
#else
							style + 
							StyleCaption + 
							StyleMinimize + 
							StyleMaximize +
							StyleCloseBox, 
							name);
#endif
}

// ---------------------------------------------------------------------------
void VMdiView::getInfo(VWinInfo& info)
{
	if ((pName == 0) || (pName[0] == '\0')) {
		info.wClass = "VMdiView";
	}
	else {
		info.wClass = (char *)pName;
	}
	info.style = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	info.exStyle = 0;

	if (!style.contains(StyleHidden)) {
		info.style |= WS_VISIBLE;
	}
	if (style.contains(StyleMinimized)) {
		info.style |= WS_MINIMIZE;
	}
	if (style.contains(StyleMaximized)) {
		info.style |= WS_MAXIMIZE;
	}
	if (style.contains(StyleHorizontal)) {
		info.style |= WS_HSCROLL;
	}
	if (style.contains(StyleVertical)) {
		info.style |= WS_VSCROLL;
	}
}

// --------------------------------------------------------------------------
//
VStyle& VMdiView::getClassDefaultStyle(void)
{
	static VWinStyle CV_NEARDATA default_style
								 (VView::getClassDefaultStyle() +
								  setTransient +
								  StyleSizable + 
								  StyleCaption + 
								  StyleMinimize + 
								  StyleMaximize + 
								  StyleCloseBox);
	return default_style;
}

// --------------------------------------------------------------------------
//
VStyle& VMdiView::getDefaultStyle(void) const
{
	return getClassDefaultStyle();
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VMdiView::putTo(VArchiver& a)
{
	VMdiView::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VMdiView::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VView::putTo(a, FALSE);

	// Write the object version tag.

	a << VMdiViewArchiveTag;

	// Write any object specific data.
	
	;

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VMdiView::getFrom(VArchiver& a)
{
	// Read the base object data.

	VView::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;

	if (!archiveTagOk(tag, VMdiViewArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VMdiViewArchiveTag));
		return;
	}

	// Read any object specific data.

	;
}

// ---------------------------------------------------------------------------
//
void VMdiView::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
	VMdiView::getFrom(a);

	if (do_realize) {
		VMdiView::realize((VWindow *)parent);
	}

	if (parent != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VMdiView::realize(VWindow *parentw)
{
	if ((parentw != 0) && parentw->isA(VMdiAppViewCls)) {
		parentw = ((VMdiAppView *)parentw)->clientOf();
	}
	VView::realize(parentw);
}

#endif // ARCHIVER
