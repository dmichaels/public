// $Header:   Q:/views/win/vcs/window.cpv   1.134   22 Nov 1996 15:11:14   davidmi  $

//  window.cpp
//
//  VWindow implementation
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
//  07/17/92 jmd    preened, added named windows
//  09/09/92 jmd    changed 'all' arg from boolean to int to fix type promotion bug
//  09/09/92 jmd    fixed (char *) bug in wrtText
//  09/17/92 jmd    added names
//  10/12/92 jmd    removed FREESELF macro
//
//  03/05/93 pat    changed notifier. to notifier-> in ::getParentAbs
//  04/07/93 pat    changed id to VObject *
//  04/27/93 pat    added VWindow() and operator() that take names
//  04/29/93 pat    added getFrom(), putTo(), realize(), findWindow()
//                  included archiver.h;
//  05/20/93 jmd    added geometry management
//  06/17/93 pat    added hiddenFlag and enabledFlag for realization data
//  06/28/93 dgm    added obscured(), postPaint(), enablePostPaint().
//                  added clipToChildren() and clipToSiblings().
//                  fixed setCursor to set the cursor immediately, i.e.
//                  without waiting for the next mouse-move message.
//  06/30/93 dgm    added setFrameRatios().
//  07/15/93 pat    added getArchiveEnable(), setArchiveEnable(), and
//                  archiveEnableFlag; used 'em under putTo();
//                  included curChildFocus in archival of children.
//  07/19/93 jmd    added frameChildren test mode, test for popups, detachWin
//  07/19/93 jmd    removed free(), NIL to 0, made VFrame const
//  07/20/93 jmd    changed rgbColor to VColor
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  07/28/93 pat    changed findWindow() to take const VString& name
//  07/31/93 jmd    made count, indexOf, hash, etc. const
//  08/09/93 dgm    changed update(VRectangle *, boolean) work with a null
//                  VRectangle* argument --> update the entire window.
//  08/16/93 jmd    made name const VString
//  08/18/93 dgm    removed setRatioX().
//  08/19/93 dgm    added null check to putAttachTo().
//  08/20/93 jmd    added failure test to getChildrenFrom()
//  08/20/93 dgm    added hasBorder() and setBorder(boolean).
//  08/20/93 jmd    added loadResources()
//  08/21/93 jmd    fixed frameChildren return value
//  08/23/93 dgm    added setFrameUnits().
//  08/23/93 pat    moved window.h include to top
//  08/23/93 pat    added foreground color to archive data
//  08/23/93 pat    added paletteChanged()
//  08/23/93 dgm    fix in setFrameUnits().
//  08/31/93 dgm    fix in clean(); unset parent->curChildFocus.
//  08/31/93 mrp    Changed VWindow::isEqual so that it verifies that the
//                  parameter "obj" is, in fact, a VWindow before comparing.
//  09/01/93 pat    added 2 ints to archive data as future min size provision;
//                  included brushinfo.h and added VBrushInfo to archive.
//  09/01/93 dgm    M8 changes.
//  09/22/93 pat    added archive tag; archived base VDialog layer
//  09/23/93 pat    added defaultForeground flag to archive data
//  10/05/93 dgm    added bringToTop(VWindow *).
//  10/05/93 dgm    fixed realize() to realize the children.
//  10/05/93 jmd    made VFrame::get pass window in build()
//                  (so that Font16 will work for dialogs)
//  10/06/93 jmd    added replaceChild()
//  10/06/93 dgm    initialize model.
//  10/09/93 jmd    made getPhysical not inline
//  10/12/93 pat    initialized attachSibling realization data.
//  10/12/93 jmd    added call to recreateWin in setBorder, preened
//  11/12/93 pat    took care of integer conversion and unreferenced variable
//                  warnings under M8
//
//  01/04/94 jmd    added help routines
//  01/27/94 dgm    minor change for Borland C++ 4.0.
//  02/02/94 pat    updated VArchiver::abort() use
//  02/02/94 pat    added archive tag check
//  02/04/94 dgm    added virtual keyUp()
//  02/09/94 pat    updated defineArchiveTag use
//  02/11/94 pat    added VArchiver::ClassSpecific abort message.
//  02/17/94 pat    type cast some ord collect indices to ints in assignments
//  02/23/94 dgm    sanity check in getClientRel (BC).
//  02/26/94 jmd    added private hInstance member for VNotifier use
//  02/26/94 jmd    added test for hWnd in setPhysical
//  02/26/94 jmd    sanity check in getRel/getAbs (BC).
//  03/08/94 pat    STRICT:
//                  replaced direct access of VMouseCursor hpCursor member
//                  with VMouseCursor::getHndl().
//                  cast HWND to long in hash();
//                  cast in SendMessage appropriately.
//  03/26/94 pat    CV_WIN32:
//                  changed include brushinfo.h to brushinf.h;
//                  added explicit int cast on assignments from POINT and
//                  RECT long members;
//                  updated update().
//  03/26/94 pat    CV_WIN32: cast appropriately for SendMessage()
//  03/14/94 pat    CV_WIN32: changed type of user() message param to UINT
//  06/28/94 jmd    fixes for children in VWindow::realizeChildren
//  08/05/94 dgm    Changes for VAppView archiving support.
//  08/15/94 dgm    Removed old postpaint support.
//  08/17/94 dss    Tab Stops.
//  08/23/94 dgm    Added tab stop archiving support (archiver revision 1).
//  08/31/94 dss    Reverse tab stop bug fix. Ctl3d non-dialog support.
//  09/05/94 dss    getInfo(): exStyle initialization.
//  10/11/94 dss    clean(): hide the window, before destroying the children.
//                  This prevents the visual side-effect of seeing the children
//                  die.
//  11/03/94 dgm    Backed out new revision for saving.
//  11/04/94 dss    added escapeHit().
//  11/11/94 dss    Archiver tags macros.
//  11/17/94 dgm    Fix in clean() to remove from parent's children list
//                  even (especially) if the parent's hWnd is null.
//
//  03/16/95 dss    added destroyed().
//  03/22/95 jld    setCursor() doesn't SetCursor for hidden windows
//  03/27/95 jld    removed #ifdef ARCHIVE_ZORDER, always on
//  05/30/95 jld    added shedFocus() member function
//  06/08/95 jld    made helpId a long
//  07/03/95 dgm    Merged in Intersolv changes:
//                  - Archive FALSE isHidden flag for VControls; this is
//                    a less obtrusive hack than the one made to VControl::
//                    isHidden() which always returned FALSE while archiving,
//  09/01/95 dss    VStyle changes.
//  09/26/95 dss    Removed CPPV_11 code. build(): only set style from arg
//                      if window style wasn't set. getFrom(): added
//                      code to convert old styles to new ones.
//  09/29/95 dgm    VGeoFrame changes.
//  10/06/95 dgm    #ifdef'd VGeoFrame code (CV_USE_GEOFRAME).
//  10/09/95 evc    Made font namagement part of VWindow(was in VControl).
//                  Also fixed it so VWindows that are parented by VDialogs AND
//                  whose VFont is the same as the VDialog will putTo
//                  a VFont of 0 (so they are restored with the VDiolog's font
//                  instead of their own.)
//  10/09/95 dgm    Geometry change in determinePreferredSize().
//  10/11/95 evc    in putTo, check for null parent before doing ->isa
//  10/12/95 dss    Added default style functionality.
//  10/13/95 dss    Fixed getFrom() bug. Wrong style class used.
//  10 16/95 evc    Added setFont to realizeChildren; improved putTo
//  10/18/95 dgm    Added copy(VWindow *new_parent)
//                  and copyNonArchiveData(const VWindow *) --> protected.
//                  Added miscellaneous geometry management related functions.
//  10/19/95 dgm    Removed inline typo for getConstrainedSize().
//  10/19/95 dgm    Fix in attachWin().
//  10/20/95 dgm    Added reparent().
//  10/23/95 evc    Added deletion of the default font gotten from an archive
//  11/01/95 dgm    Geometry management changes.
//  11/01/95 dgm    Added #ifndef CV_NOOLE's.
//  11/02/95 dgm    Changed getNonClientAreaSize() to getNonClientArea().
//  11/10/95 dgm    Removed getNonClientArea(); added getSystemBorderSize().
//                  Changed getClientWindow() to getClientAreaWindow().
//                  Added getInternalBorderGeometry().
//                  Implemented VGeoFrame/Manager archiving (rev.4).
//  11/13/95 dgm    Added setDontFrame() and isDontFrame().
//  11/14/95 dgm    Minor fix in getSystemBorderSize().
//  11/14/95 dgm    Fix/hack in resize() and move() to interpret a zero
//                  width/height as such, rather than as a preferred-size.
//  11/15/95 dgm    Archiving/reparent fix in realizeChildren().
//  11/16/95 dgm    Added insertChild(), childAdded(), and childRemoved().
//  11/20/95 dgm    Added getGeoIndex().
//  11/29/95 dgm    Added call to VGeoFrame::getFromDone() in getFrom().
//  11/30/95 dgm    Added updateGeometry().
//                  Call updateGeometry from and various strategic spots.
//  12/04/95 dgm    Fixed typo in resize().
//  12/04/95 dgm    Commented out getFromDone() for now; not correct;
//                  was causing crash; look into it later.
//  12/05/95 dgm    Changed updateGeometry().
//  12/06/95 dgm    Changed setArchiveEnable() to
//                  call VGeoFrame::setDontArchive().
//  12/21/95 mjw    Changed update() and update(VRectangle*, boolean)
//                  to not update with a null window handle
//  01/02/96 dss    resized: DONT return if there's a geo-manager.
//  01/09/96 dss    VComponent name change.
//  01/24/96 dgm    Set convertOldToGeoFrames = FALSE if archive contains
//                  new geometry; symptom was in the Omega painter, when
//                  going into test-mode a toolbar wasn't framing right.
//  01/29/96 dgm    Changed getInfo() to setup 3D sunken border
//                  if StyleBorder + Style3D; other style changes.
//  01/29/96 dgm    Changed isTopLevel() to be a const function.
//  01/31/96 evc    Added code to set pBackground from archived brushinfo
//  02/01/96 dgm    Added VTreeListBoxCls declaration -- someone forgot.
//  02/06/96 dgm    Added support for boolean return value from
//                  frameChildren() if test-mode flag is set.
//  02/19/96 evc    made all static data inside functions CV_NEARDATA
//  02/28/96 dgm    Fix in build().
//  03/05/96 evc    Small fix to clean() that insures notifier::focus
//                  never stays set to value of a destructed window
//                  if that window had focus.
//  03/12/96 dgm    Fix to begin/endDeferGeometryChanges().
//  04/04/96 dgm    Added centerWindow().
//  04/25/96 dgm    Added archiveChildrenNotRealized flag and other
//                  geometry related archiver changes.
//  05/21/96 evc    Adjustment to ::resized() to pass resize event
//                  to the "componentThis" if it (exists).  Set
//                  archiveChildrenNotRealized to in realizeChildren when
//                  appropriate.
//  06/05/96 dgm    Call childRemoved() appropriately.
//  06/19/96 evc    Default constructed VComponentWindows in getFrom
//                  need an addref.
//  08/06/96 pkt    added 'beingDestroyed'.
//  09/11/96 dgm    Added setSystemFont bit.
//  09/17/96 dgm    Added setFont(VFont *, boolean).
//  09/18/96 dgm    Fixed memory leak -- background brush from archive.
//  09/25/96 dgm    Changed deferred window sizing/positioning to not
//                  nest after discovering very obscure problems;
//                  apparently it is safe only to defer a number of
//                  sibling windows, this sort of makes sense but is
//                  not documented.  Also, amazingly, StyleHidden was
//                  not being respected (it was in VView though).
//  11/15/96 dgm    Added fix for default font behavior of VTreeListBox.
//  11/15/96 mlw    Added code from Sanjay Chopra for VComponent* Archiving
//                  differences from previous approach.
//	02/04/97 pkt	moved handling of default styles from 'init' to 'build'.
//  02/07/97 dgm    Added LostFocus, lostFocus(), mmouseX(), mouseWheel(),
//					ContextHelp, ContextMenu, contextHelp(), and contextMenu().
//	12/18/96 dgm	Added the following virtual methods: mmouseDn/Up/Dbl(),
//					showContextHelp(), showContextMenu().
//  01/10/97 dgm    Added mouseWheel().
//  01/19/97 dgm    Added dragSite and dropSite data-members.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
//	04/28/97 dgm	Added invalidateGeometry() and reframeWindow().
//	06/25/97 dgm	Fix for not processing givenFocus() appropriately
//					when a top-level window is shown (non-modally).
//  08/01/97 dgm    Added setTabIndex(), swapTabIndices(), and getTabIndex()
//					for SCR 5809.
//	06/27/97 dgm	Fixed update() to recursively update *non* top-level
//					children only.
//-----------------------------------------------------------------------

#include "accel.h"
#include "brush.h"
#include "cvassert.h"
#include "event.h"
#include "geoframe.h"
#include "geomgr.h"
#include "geoform.h"
#include "georow.h"
#include "geocol.h"
#include "iterator.h"
#include "location.h"
#include "notifier.h"
#include "rect.h"
#include "set.h"
#include "str.h"
#include "style.h"
#include "winclass.h"
#include "dialog.h"
#include "window.h"

#ifndef CV_NOVECTOREVENTS
#include "dictinry.h"
#include "intassoc.h"
struct VWindowEventManager {
    method        eventDictionary[VWindow::_EventCount_];
    VDictionary  *clientDictionary;
};
#endif

// Compound Document Objects
#ifndef CV_NOOLE
#include "cmpwin.h"
#include "itemwin.h"
#endif

#ifdef CV_OLE_DRAG_DROP
#include "dropsite.h"
#include "dragsite.h"
#endif

#ifndef CV_NOARCHIVER
#   include "archiver.h"
#   include "brushinf.h"
#endif

extern "C" {
#   include <string.h>
#   include <stdarg.h>
#   include <stdio.h>
}

extern VClass *VAppViewCls;
extern VClass *VPopupWindowCls;

// geometry test masks
const unsigned GeoLeft  = 0x01;
const unsigned GeoRight = 0x02;
const unsigned GeoTop   = 0x04;
const unsigned GeoBottom = 0x08;

const unsigned GeoAll = GeoLeft | GeoRight | GeoTop | GeoBottom;

static unsigned gtestmask[4] = { ~GeoTop, ~GeoBottom, ~GeoLeft, ~GeoRight };

static inline int vmax(const int &a, const int &b)
{
    return (a > b) ? a : b;
}

#define NOstyle (-1)

defineClass(VWindow, VDisplay)
//
// revision 0   original
// revision 1   added bTabStop
// revision 2   VStyle change.
// revision 3   Fonts, Compound Documents
// revision 4   VGeoFrame
// revision 5   Compound Document Archiving
// revision 6   Sync level for VComponent Archiving
//
//
defineArchiveRevision(VWindow, 7)

// global for archive sync level for Component*  MLW.
long currentWindowTag = 0;

// ----------------------------------------------------------------------------
//
static HDWP     deferGeometryChangeId		= 0;
static boolean  endingDeferGeometyChanges	= FALSE;
static boolean  usingGeoFrames				= TRUE;
static boolean  convertOldToGeoFrames		= FALSE;

// ----------------------------------------------------------------------------
//
#if defined(CV_NOOLE) && !defined(CV_NOARCHIVER)
struct VObjectRawMemory {
		    VObjectRawMemory()
			{ data = 0; size = 0; }
		    VObjectRawMemory(char *d, unsigned long s)
			{ data = d; size = s; }
		   ~VObjectRawMemory()
			{ delete [] data; }
    unsigned long   size;
    char           *data;
};
#endif

//----------------------------------------------------------------------------
VStyle& VWindow::getClassDefaultStyle(void)
{
    static VWinStyle    CV_NEARDATA classDefaultStyle;

    return classDefaultStyle;
}

//----------------------------------------------------------------------------
VStyle& VWindow::getDefaultStyle(void) const
{
    return getClassDefaultStyle();
}

//----------------------------------------------------------------------------
//  Construct a VWindow object within VWindow 'win', with frame 'frame'
//  and style 'style'. The units specified to the VFrame object can
//  be either integer coordinates in the parent window\'s space or
//  ratios (that is, percents) of the parent window\'s space.  For maximum
//  flexibility the units specified in the VFrame object can be mixed
//  floats or ints.
//
//  For example, it might be useful to use framing ratios
//  for position and width but have the height of the window be a
//  fixed size:
//
//      VWindow(VFrame(0.25F,0.1F,.5F,20), pwin, StyleDefault);
//
//  This constructor should be called in the initialization list of any
//  user derived class of 'VWindow'.
//
VWindow::VWindow(const VFrame& frame, VWindow *win, const VStyle& style) :
    winFont(0),
    hInstance(0),
    beingDeleted(FALSE),
	beingDestroyed(FALSE)
{
    init(win);
    winFrame = frame;
    build(style);
}

//----------------------------------------------------------------------------
//
VWindow::VWindow(const VString& wname,  const VFrame&   frame,
		 VWindow    *pwin, const VStyle& style) :
    winFont(0),
    hInstance(0),
    beingDeleted(FALSE),
	beingDestroyed(FALSE)
{
    init(pwin);
    winFrame = frame;
    build(style);
    setName(wname);
}

//----------------------------------------------------------------------------
//  Construct a VWindow with no parent or children and no presentation
//  window.
//
VWindow::VWindow() :
    winFont(0),
    hInstance(0),
    beingDeleted(FALSE),
	beingDestroyed(FALSE)
{
    init(0);
}

//===========================================================================
VWindow::VWindow(HWND hwnd) :
    winFont(0),
    hInstance(0),
    beingDeleted(FALSE),
	beingDestroyed(FALSE)
{
// This constructor makes a 'blank' VWindow, and then fills
// in the VWindows's hwnd with that supplied as an argument.  Its used to
// make a non-views windows into a VWindow.   The VWindow can, for
// example, be used as the parent of a real Views object.
// This is done if we want to parent a Views dialog with a
// non-Views Window.
    init(0);
    hWnd = hwnd;
}

//----------------------------------------------------------------------------
//  Return the GUI system device context for this VWindow.
//
HDC VWindow::getHndl()
{
    if (!hDC) {
	if (!(hDC = GetDC(hWnd))) {
	    return 0;
	}
    }
    refCount++;
    return hDC;
}

//----------------------------------------------------------------------------
//  Release the GUI system device context handle for this VWindow.
//
void VWindow::ungetHndl()
{
    if (hDC && !-- refCount) {
	ReleaseDC(hWnd, hDC);
	hDC = 0;
    }
}

//----------------------------------------------------------------------------
//  Default initialization for the VWindow class.
//
void VWindow::init(VWindow *win)
{
    geoFrame = 0;
    constrainedMinimumWidth = 0;
    constrainedMinimumHeight = 0;
    constrainedMaximumWidth = 0;
    constrainedMaximumHeight = 0;
    dockableWindowOwner = 0;

    setDefRatio();
    parent = win == 0 ? 0 : win->getClientAreaWindow();
    // !!! dss 8/7/95 uStyle = NOstyle;
    // style.setOtherGroup(style);
    hWnd = 0;
    fromRsc = 0;
    children = 0;
    pCursor = 0;
    pBackground = 0;
	backgroundBrushIsMine = 0;
    defaultForeground = TRUE;       //  use system default color
    pForeground = 0ul;                //  initialize to black
    curChildFocus = 0;
    defProc = 0;
    winName = 0;
    bTabStop = TRUE;
    componentWindow = 0;
    componentWindowIsRawMemory = 0;

#ifndef CV_NOARCHIVER
    archiveChildren = 0;
    childrenZorder = 0;
	oldAttachArchiveData = 0;
    hiddenFlag  = FALSE;
    enabledFlag = TRUE;
    archiveEnableFlag = TRUE;
#endif
    archiveChildrenNotRealized = 0;

    model = 0;
    helpId = 0;             // jmd 01/04/94

    redrawUponResize = notifier->isWindowRedrawUponResize();
    autoErased = notifier->isWindowAutoErased();

#ifndef CVS_LATER_DEFAULT
    style = setTransient + getDefaultStyle();
#endif

    pFontFromArchive = 0;
    winFont = 0;
	setDefaultFont = -1;

    isPaletteChanging = 0;
	allowFocus = 1;
    processingNextChildFocus = 0;
    processingPrevChildFocus = 0;

#ifdef CV_OLE_DRAG_DROP
	dragSite = 0;
	dropSite = 0;
#endif

#ifndef CV_NOVECTOREVENTS
	theEventManager = 0;
#endif
}

//----------------------------------------------------------------------------
//  Construct a presentation window for this VWindow. If successful,
//  return TRUE, otherwise return FALSE.
//
boolean VWindow::build(const VStyle& style)
{
    VWindow *this_parent;
    int parent_width, parent_height, x, y, w, h;

#ifndef CVS_LATER_DEFAULT
	// "Default" styles have been set into 'style' as transient.
    this->style += style;
#else
	// 'This->style' is not yet used.
	// The default style is transient (not archived).
	// The styles given by the style parameter are archived.
	// GetDefaultStyle() must be to the left of style in the expression below,
	// so that non-commutative operator+ will favor the style parameter.
	this->style = setTransient + getDefaultStyle() - setTransient + style;
#endif

    if (parent != 0) {
	this_parent = parent->getClientAreaWindow();
    }
    else {
	this_parent = 0;
    }

    if (this_parent != 0) {
	this_parent->addChild(this);
	this_parent->sizeOfImage(&parent_width, &parent_height);
    }
    else {
	setGeoManager();
	notifier->getScreenSize(&parent_width, &parent_height);
    }

    winFrame.get(this, VLocation(parent_width, parent_height),
		 &x, &y, &w, &h);

    boolean window_created = notifier->createWin(this, x, y, w, h);

    // What is this??  DSS added it ...

    if (this_parent != 0) {
	this_parent->addChild(this);
    }

    return window_created;
}

//----------------------------------------------------------------------------
//  Make this VWindow the parent of 'child'.
//
void VWindow::addChild(VWindow *child)
{
    if (child->parent == 0) {
		child->parent = this;
    }
    if (children == 0) {
		children = new VOrdCollect;
    }
    else if (children->findSame(child)) {
		return;
    }
    children->add(child);
#ifndef CV_NOOLE
	child->setParent(this);
	if (getComponentThis() != 0) {
		getComponentThis()->addChild(child);
	}
#endif
	if (usingGeoFrames) {
		if (!child->isTopLevel()) {
		    if ((geoFrame == 0) || !geoFrame->isManager()) {
				setGeoManager();
		    }
			geoManager->addChild(child);
		}
	}
    childAdded(child, children->indexOfSame(child));
}

// ----------------------------------------------------------------------------
//
void VWindow::insertChild(VWindow *child, int index)
{
    if (child->parent == 0) {
		child->parent = this;
    }
    if (children == 0) {
		children = new VOrdCollect;
    }
    else if (children->findSame(child)) {
		return;
    }
    children->insertAt(index, child);
#ifndef CV_NOOLE
	child->setParent(this);
	if (getComponentThis() != 0) {
		getComponentThis()->addChild(child); // ToDo?
	}
#endif
	if (usingGeoFrames) {
		if (!child->isTopLevel()) {
			if ((geoFrame == 0) || !geoFrame->isManager()) {
				setGeoManager();
			}
			geoManager->addChild(child); // ToDo!
		}
	}
    childAdded(child, index);
}

// ----------------------------------------------------------------------------
//
void VWindow::childAdded(VWindow *, int)
{
}

// ----------------------------------------------------------------------------
//
void VWindow::childRemoved(VWindow *, int)
{
}

// --------------------------------------------------------------------------
//
boolean VWindow::reparent(VWindow *new_parent, boolean reframe)
{
    if (new_parent == 0) {
	return FALSE;
    }

    if (new_parent == parent) {
	return TRUE;
    }

    // Remove any/all attachments from/to this window.

    if (parent != 0) {
	parent->attachWin(this, VAttach::Left,   VAttach::UnAttached);
	parent->attachWin(this, VAttach::Right,  VAttach::UnAttached);
	parent->attachWin(this, VAttach::Top,    VAttach::UnAttached);
	parent->attachWin(this, VAttach::Bottom, VAttach::UnAttached);
	parent->detachWin(this);
    }

    // Do the Views level reparenting.

    VWindow *old_parent = parent;

    if (old_parent != 0) {
	old_parent->removeChild(this);
	parent = 0;
    }
    new_parent->addChild(this);

    // Do the Windows level reparenting.

    HWND this_hwnd = hwnd();
    HWND new_parent_hwnd = new_parent->hwnd();

    if ((this_hwnd != 0) && (new_parent_hwnd != 0)) {
	SetParent(this_hwnd, new_parent_hwnd);
    }

    old_parent->childReparented(this, new_parent);
    new_parent->childParented(this, old_parent);

    if (reframe) {
	if (new_parent != 0) {
	    new_parent->frameChildren();
	}
	if (old_parent != 0) {
	    old_parent->frameChildren();
	}
    }

    return TRUE;
}

// --------------------------------------------------------------------------
//
void VWindow::childReparented(VWindow *child, VWindow *new_parent)
{
}

// --------------------------------------------------------------------------
//
void VWindow::childParented(VWindow *child, VWindow *old_parent)
{
}

//----------------------------------------------------------------------------
//  Bring this VWindow to the top of a stack of overlapping windows.
//  Used to uncover a VWindow object that is partially or completely covered
//  by any overlapping windows.
//
void VWindow::bringToTop()
{
    BringWindowToTop(hWnd);
}

//----------------------------------------------------------------------------
//  Place this VWindow on top the given window in the stacking order.
//
void VWindow::bringToTop(VWindow *w)
{
    if ((w == 0) || (w->hWnd == 0) || (this->hWnd == 0)) {
	return;
    }
    HWND insert_after_hwnd = GetWindow(w->hWnd, GW_HWNDPREV);
    if (insert_after_hwnd == 0) {
	insert_after_hwnd = HWND_TOP;
    }
    SetWindowPos(this->hWnd, insert_after_hwnd,
		 0, 0, 0, 0, SWP_NOMOVE |  SWP_NOSIZE);
}

//----------------------------------------------------------------------------
//  Erase the background of this VWindow.
//
void VWindow::erase()
{
    getHndl();
    SendMessage(hWnd, WM_ERASEBKGND, (WPARAM) hDC, (LPARAM) 0);
    ungetHndl();
}

//----------------------------------------------------------------------------
//
// This method is executed when the presentation space of this VWindow
// object is natively destroyed. The native window handle is not valid after
// this method ends.
//
void VWindow::destroyed()
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		getComponentThis()->destroyed();
	}
#endif
}

//----------------------------------------------------------------------------
//  Draw the frame ( the non-client area) of this VWindow.
//
void VWindow::frame()
{
    if (!isHidden()) {
	SendMessage(hWnd, WM_NCPAINT, (WPARAM) 0, (LPARAM) 0);
    }
}

//----------------------------------------------------------------------------
//
VWindow::~VWindow()
{
    beingDeleted = TRUE;
    VWindow::clean();
}

//----------------------------------------------------------------------------
//  Return the dimensions of the bounding rectangle of this VWindow
//  into '*w' and '*h' respectively.
//
void VWindow::sizeOfWin(int *w, int *h)
{
    RECT rect;
    GetWindowRect(hWnd, (LPRECT)&rect);
    *w = int(rect.right - rect.left);
    *h = int(rect.bottom - rect.top);
}

//----------------------------------------------------------------------------
//  Return, in screen pixel units, the upper-left corner, width, and height
//  of this VWindow relative to the upper-left corner of the screen, in
//  '*x', '*y', '*w' and '*h' respectively.
//
void VWindow::getAbs(int *x, int *y, int *w, int *h)
{
    RECT rect;
    if (hWnd) {
	GetWindowRect(hWnd, (LPRECT)&rect);
    }
    *x = int(rect.left);
    *y = int(rect.top);
    if (w) {
	*w = int(rect.right - rect.left);
    }
    if (h) {
	*h = int(rect.bottom - rect.top);
    }
}

//----------------------------------------------------------------------------
//  Return, in screen pixel units, the minimum width and height required for
//  ledgibility of this VWindow in '*mw' and '*mH' respectively.
//
void VWindow::getMin(int *mW, int *mH)
{
    VWindow *child;
    int tempW, tempH, i, nChildren;
    float fx, fy, fw, fh;
    int w, h, dw, dh;
    short msW, msH;

    getMinSize(msW, msH);
    *mW = msW;  *mH = msH;
    if (!children) {
	return;
    }
    sizeOfImage(&w, &h);
    getAbs(&dw, &dh);
    dw -= w;             /* calculate size of non-client area */
    dh -= h;

    nChildren = int(children->count());
    for (i = 0; i < nChildren; i++) {
	child = (VWindow *)children->idAt(i);
	if (child && !child->isA(VPopupWindowCls)) {
	    child->getMin(&tempW, &tempH);
	    child->getRatio(&fx, &fy, &fw, &fh);
	    child->getAbs(&w, &h);

	    if (fw > 0.0 && fx >= 0.0) {
		tempW = (int)(tempW / fw + 0.5);
	    }
	    if (fh > 0.0 && fy >= 0.0) {
		tempH = (int)(tempH / fh + 0.5);
	    }

	    tempW += dw; /* add back non-client size */
	    tempH += dh;

	    if (tempH > msH) {
		*mH = tempH;
	    }
	    if (tempW > msW) {
		*mW = tempW;
	    }
	}
    }
}

//----------------------------------------------------------------------------
//  Return the screen coordinates of the parent of this VWindow
//  (or the screen itself if it has no parent) in '*x' ,'*y', '*w', and
//  '*h' respectively.
//
void VWindow::getParentAbs(int *x, int *y, int *w, int *h)
{
    if (parent) {
	parent->getClientAbs(x, y, w, h);
    }
    else {
	if (x) {
	    *x = 0;
	}
	if (y) {
	    *y = 0;
	}
	notifier->getScreenSize(w, h);
    }
}

//----------------------------------------------------------------------------
//  Return the ratio of this VWindow relative to its parent\'s
//  client area in '*x', '*y', '*w', and '*h' respectively.
//
void VWindow::getRatio(float *x, float *y, float *w, float *h)
{
    int px = 0, py = 0, pw, ph;
    int ix, iy, iw, ih;
    getParentAbs(&px, &py, &pw, &ph);
    getAbs(&ix, &iy, &iw, &ih);
    *x = ((float)(ix - px)) /pw;
    *y = ((float)(iy - py)) /ph;
    *w = ((float)iw) /pw;
    *h = ((float)ih) /ph;
}

//----------------------------------------------------------------------------
//  Return, in screen pixel units, the position and size of this VWindow
//  relative to its parent into'*x', '*y', '*w', and '*h' respectively.
//
void VWindow::getRel(int *x, int *y, int *w, int *h)
{
    getAbs(x, y, w, h);
    if (parent && parent->hWnd) {
	POINT pt;
	pt.x = *x;
	pt.y = *y;

	ScreenToClient(parent->hWnd, (LPPOINT)&pt);

	*x = (int) pt.x;
	*y = (int) pt.y;
    }
}

//----------------------------------------------------------------------------
//  Return the width and height of this VWindow\'s client area in
//  screen pixel units into '*w' and '*h' respectively.
//
void VWindow::sizeOfImage(int *w, int *h)
{
    int x, y;
    getClientRel(&x, &y, w, h);
}

//----------------------------------------------------------------------------
//  Return the position and size of this VWindow\'s client area
//  relative to the screen into '*x', '*y', '*w' and '*h' respectively.
//
void VWindow::getClientAbs(int *x, int *y, int *w, int *h)
{
    getClientRel(x, y, w, h);
    mapToScreen(x, y);
}

//----------------------------------------------------------------------------
//  Return the position and size of this VWindow\'s client area relative
//  to its parent window into'*x', '*y', '*w' and '*h' respectively.  The
//  returned ('*x', '*y') position is always (0,0) unless there is no parent
//  window.  In that case, the position of the upper-left hand corner is
//  returned in screen coordinates relative to the upper-left corner of the
//  screen.
//
void VWindow::getClientRel(int *x, int *y, int *w, int *h)
{
    RECT  rect;
    if (hWnd == 0) {
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
    }
    else {
	GetClientRect(hWnd, (LPRECT)&rect);
    }
    if (x != 0) {
	*x = int(rect.left);
    }
    if (y != 0) {
	*y = int(rect.top);
    }
    if (w != 0) {
	*w = int(rect.right - rect.left);
    }
    if (h != 0) {
	*h = int(rect.bottom - rect.top);
    }
}

// ----------------------------------------------------------------------------
//
void VWindow::setFrame(VFrame& f)
{
    winFrame = f;
    updateGeometry();
}

// ----------------------------------------------------------------------------
//
void VWindow::setFrameUnits(VFrame::Units units)
{
    if (winFrame.getUnits() == units) {
	return;
    }
    int x, y, w, h;
    boolean x_ratio, y_ratio, w_ratio, h_ratio;
    winFrame.isRatio(x_ratio, y_ratio, w_ratio, h_ratio);
    getRel(&x, &y, &w, &h);
    if (units == VFrame::Font16) {
	int font_width, font_height;
	this->fontSize(&font_width, &font_height);
	x = int((float(x) / float(font_width)) * 16.0);
	w = int((float(w) / float(font_width)) * 16.0);
	y = int((float(y) / float(font_height)) * 16.0);
	h = int((float(h) / float(font_height)) * 16.0);
    }
    winFrame.set(x, y, w, h, winFrame.howFramed());
    winFrame.setUnits(units);
    setFrameRatios(x_ratio, y_ratio, w_ratio, h_ratio);
}

void VWindow::setFrameRatios(boolean x_ratio, boolean y_ratio,
			     boolean w_ratio, boolean h_ratio)
{
    boolean x_is_ratio, y_is_ratio, w_is_ratio, h_is_ratio;

    winFrame.isRatio (x_is_ratio, y_is_ratio, w_is_ratio, h_is_ratio);

    if ((x_is_ratio == x_ratio) &&
	(y_is_ratio == y_ratio) &&
	(w_is_ratio == w_ratio) &&
	(h_is_ratio == h_ratio)) {
	return;
    }

    int ix, iy, iw, ih;
    float fx, fy, fw, fh;
    VRatio rx, ry, rw, rh;

    getRel(&ix, &iy, &iw, &ih);
    getRatio(&fx, &fy, &fw, &fh);

    if (x_ratio) {
	rx = VRatio(fx);
    }
    else {
	rx = VRatio(ix);
    }
    if (y_ratio) {
	ry = VRatio(fy);
    }
    else {
	ry = VRatio(iy);
    }
    if (w_ratio) {
	rw = VRatio(fw);
    }
    else {
	rw = VRatio(iw);
    }
    if (h_ratio) {
	rh = VRatio(fh);
    }
    else {
	rh = VRatio(ih);
    }

    winFrame.set(rx, ry, rw, rh, CornerDim);
    updateGeometry();
}

//----------------------------------------------------------------------------
//  Return TRUE if this VWindow is enabled (receives mouse and keyboard
//  events), otherwise return FALSE.
//
boolean VWindow::isEnabled()
{
    return IsWindowEnabled(hWnd);
}


//----------------------------------------------------------------------------
//  Return TRUE if this VWindow is hidden (not displayed on the screen),
//  otherwise return FALSE.
//
boolean VWindow::isHidden()
{
    return !IsWindowVisible(hWnd);
}

//----------------------------------------------------------------------------
//  Return TRUE if this VWindow is the parent VWindow of 'win', otherwise
//  return FALSE.
//
boolean VWindow::isParentOf(VWindow *win)
{
    if (!children) {
	return FALSE;
    }
    return children->findSame(win) ? TRUE: FALSE;
}

//----------------------------------------------------------------------------
//
// returns TRUE if i am a Top Level window. Sub-classes define top level
// by overriding this method.
//
boolean VWindow::isTopLevel() const
{
    return FALSE;
}

//----------------------------------------------------------------------------
// returns the Top Level window. Returns argument if it is a top level window
//
VWindow *VWindow::getTopLevelParent()
{
    VWindow     *parent = this;

    // climb the parent chain while i'm not top level and i have a parent
    // to find the top level parent
    for (;
	 !parent->isTopLevel() && parent->parent;
	 parent = parent->parent) {
	;       // NO CODE HERE
	}

    // all windows should be parented to some top level window
    cv_assert(0 != parent);

    return parent;
}

//----------------------------------------------------------------------------
// returns the AppView Level parent window to this window.
// Return this if this is an AppView level window.
//
VAppView *VWindow::getAppLevelParent()
{
    VWindow     *win;

    // climb the parent chain while i'm not top level and i have a parent.
    //
    // NOTES:
    //  1) It is possible to encounter a window with no parent that returns
    //      FALSE for "isTopLevel()", due to programmer choices when
    //      sub-classing, so always check to see if a parent exists while
    //      climbing the parent list.
    //  2) When climbing the parent list, most windows won't be
    //      VAppViews. It is much faster to check "isTopLevel()" than to
    //      check for the class in each loop iteration.


    for (win = getTopLevelParent();
	 !win->isA(VAppViewCls) && win->parent;
	 win = win->getTopLevelParent()) {
	win = win->parent;
    }

    // either we've got an AppView or we don't have a parent

     // VAppViews should have no parent.
    cv_assert(0 == win->parent);

    return (VAppView *) win;
}

//----------------------------------------------------------------------------
//  Resize this VWindow so that all of its child windows are legible
//  and their contents are readable on the screen.  If this VWindow is
//  already legible, then don\'t resize.
//
boolean VWindow::makeLegible()
{
    int w, h, minW, minH;

    getAbs(&w, &h);
    getMin(&minW, &minH);

    if (w < minW) {
	w = minW;
    }
    if (h < minH) {
	h = minH;
    }

    size(w, h);
    return TRUE;
}

//----------------------------------------------------------------------------
//  Map the given ('*x', '*y') coordinates of the this VWindow\'s client
//  area onto screen coordinates.  Return the converted values
//  through ('*x', '*y').
//
void VWindow::mapToScreen(int *x, int *y)
{
    POINT p;
    p.x = *x;
    p.y = *y;

    ClientToScreen(hWnd, (LPPOINT)&p);

    *x = (int) p.x;
    *y = (int) p.y;
}

//----------------------------------------------------------------------------
//  Map the given ('*x', '*y') screen coordinates onto this VWindow\'s
//  client area.  The translated point will be relative to this VWindow\'s
//  upper-left corner.  Return the translated values through the
//  given ('*x', '*y') pointers.
//
void VWindow::mapToWin(int *x, int *y)
{
    POINT p;
    p.x = *x;
    p.y = *y;

    ScreenToClient(hWnd, (LPPOINT)&p);

    *x = (int) p.x;
    *y = (int) p.y;
}

//----------------------------------------------------------------------------
//  Return the current coordinates of this VWindow into 'r'.
//
void VWindow::getWindowRect(VRectangle &r)
{
    int x, y, w, h;
    winFrame.get(this, dimension(), &x, &y, &w, &h);
    r.set(RectOpts(CornerDim), x, y, w, h);
}

//----------------------------------------------------------------------------
//  Perform the actual move operation on the GUI system window handle.
//  If 'b' is TRUE, then the window is repainted after it is moved.
//
//  PRIVATE
//
void VWindow::moveWin(boolean b)
{
    if (!hWnd) {
	return;
    }

    int x, y, w, h;
    VRectangle r;
    getWindowRect(r);
    r.get(CornerDim, &x, &y, &w, &h);
    MoveWindow(hWnd, x, y, w, h, b);
}

//----------------------------------------------------------------------------
//  Move and resize this VWindow to the coordinates of 'r'. If 'update'
//  is TRUE, then an exposure event (that is, a paint() call) is generated
//  on this VWindow.  Otherwise, if 'update' is FALSE, no exposure event is
//  produced.  The default value for 'update' is set to TRUE.  However, a
//  value of FALSE for 'update' can be used to prevent multiple repainting
//  when several child windows are being moved.
//
void VWindow::move(VRectangle &r, boolean update)
{
    winFrame = r;
    updateGeometry();
    moveWin(update);
}

//----------------------------------------------------------------------------
//  Translate this VWindow by ('x', 'y').
//
void VWindow::translate(VRatio x, VRatio y)
{
    winFrame.xlate(dimension(), x, y);
    updateGeometry();
    moveWin();
}

//----------------------------------------------------------------------------
//  Move this VWindow to a new ('x', 'y') position.  If 'update' is
//  TRUE, then an exposure event (that is, a paint() call) is generated on
//  this VWindow.  Otherwise, if 'update' is FALSE, no exposure event is
//  produced.  The default value for 'update' is set to TRUE.  However,
//  a value of FALSE for 'update' can be used to prevent multiple
//  repainting when several child windows are being moved.
//
void VWindow::move(VRatio x, VRatio y, boolean update)
{
    winFrame.move(dimension(), x, y);
    updateGeometry();
    moveWin(update);
}

//----------------------------------------------------------------------------
//  Resize this VWindow to width 'w' and and height 'h'.  Note: 'int'
//  and 'float' constants are automatically converted to 'VRatio' objects by
//  the compiler.
//
void VWindow::resize(VRatio w, VRatio h)
{
    // This zero width/height check is a sort of hack to make sure that
    // a zero specified here is not interpreted as "get-preferred-size"
    // as a zero size in VFrame normally is (perhaps that was a mistake
    // in retrospect; perhaps we should have added VFrame::PreferredWidth
    // and VFrame::PreferredHeight enumerators).  Look for a similar hack
    // in VWindow::move().

    int zero_width = 0, zero_height = 0;;
    if (!w.isRatio()) {
	int wi = int(w);
	if (wi == 0) {
	    zero_width = 1;
	}
    }
    if (!h.isRatio()) {
	int hi = int(h);
	if (hi == 0) {
	    zero_height = 1;
	}
    }
    if (zero_width) {
	if (zero_height) {
	    VRatio w(0.0F), h(0.0F);
	    winFrame.resize(dimension(), w, h);
	}
	else {
	    VRatio w(0.0F);
	    winFrame.resize(dimension(), w, h);
	}
    }
    else if (zero_height) {
	VRatio h(0.0F);
	winFrame.resize(dimension(), w, h);
    }
    else {
	winFrame.resize(dimension(), w, h);
    }
    updateGeometry();
    moveWin();
}

//----------------------------------------------------------------------------
//  Move this VWindow to a new ('x', 'y') position and resize it to
//  'w' and 'h' respectively.  If 'update' is TRUE, then an exposure event
//  (that is, a paint() call) is generated on this VWindow.  Otherwise, if
//  'update' is FALSE, no exposure event is produced.  The default value for
//  'update' is set to TRUE.  However, a value of FALSE for 'update' can be
//  used to prevent multiple repainting when several child windows are being
//  moved.
//
void VWindow::move(VRatio x, VRatio y , VRatio w, VRatio h, boolean update)
{
    winFrame.move(dimension(), x, y);

    int zero_width  = !w.isRatio() && (int(w) == 0);
    int zero_height = !h.isRatio() && (int(h) == 0);

    if (zero_width) {
	if (zero_height) {
	    VRatio w(0.0F), h(0.0F);
	    winFrame.resize(dimension(), w, h);
	}
	else {
	    VRatio w(0.0F);
	    winFrame.resize(dimension(), w, h);
	}
    }
    else if (zero_height) {
	VRatio h(0.0F);
	winFrame.resize(dimension(), w, h);
    }
    else {
	winFrame.resize(dimension(), w, h);
    }

    updateGeometry();
    moveWin(update);
}

//----------------------------------------------------------------------------
//  Called when an exposure event on this VWindow has occured.
//  A VRectangle object describing the exposed area can be obtained from
//  the notifier through the exposedRect() method of VNotifier.
//
boolean VWindow::paint()
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->paint()) {
			return TRUE;
		}
	}
#endif
    return FALSE;
}

//----------------------------------------------------------------------------
//  Called when the system is being shutdown.  Gives this VWindow a
//  chance to do last minute housekeeping and/or abort the shutdown.  Return
//  FALSE to allow shutdown, otherwise return TRUE to abort shutdown.
//
boolean VWindow::quit()
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->quit()) {
			return TRUE;
		}
	}
#endif
    return FALSE;
}

//----------------------------------------------------------------------------
//  Called when this VWindow\'s width and height has
//  changed to 'w' and 'h' respectively.  This message is
//  automatically sent after the user resizes this VWindow, or after
//  this VWindow has received the messages size or
//  move.   The default response is to propogate the 'size' change
//  to all the children of this VWindow.
//
boolean VWindow::resized(int w, int h)
{
	if (archiveChildrenNotRealized) {
#ifndef CV_NOOLE
		if (getComponentThis() != 0) {
			if (getComponentThis()->resized(w, h)) {
				return TRUE;
			}
		}
#endif
		return TRUE;
	}
	if (geoFrame != 0) {
		if (geoFrame->isManager()) {
			geoManager->resized(w, h);
		}
	}
	else {
		frameChildren(w, h);
	}
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->resized(w, h)) {
			return TRUE;
		}
	}
#endif
	return TRUE;
}

//----------------------------------------------------------------------------
//  Set this VWindow to be a bordered window.  NOTE: This
//  method is obsolete.
//
void VWindow::setBorder()
{
    setBorder(TRUE);
}

//----------------------------------------------------------------------------
void VWindow::setBorder(boolean b)
{
    long s;

    if (b) {
	if (style.contains(StyleBorder)) {
	    return;
	}
	style += StyleBorder;

	// Note: Windows does not set border reliably on the fly,
	// better to recreate the window
	s = GetWindowLong(hWnd, GWL_STYLE) | WS_BORDER;
#if defined(CV_WIN32)
		// add client edge style
	if (has3dEffects(V_WIN3D_EFFECTS)) {
		    long exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	    exStyle |= WS_EX_CLIENTEDGE;
		    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
		s &= ~WS_BORDER;
	}
#endif
    }
    else {
	if (!style.contains(StyleBorder)) {
	    return;
	}

	style -= StyleBorder;

	s = GetWindowLong(hWnd, GWL_STYLE) & ~WS_BORDER;
#if defined(CV_WIN32)
		// remove client edge style
	if (has3dEffects(V_WIN3D_EFFECTS)) {
		    long exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	    exStyle &= ~WS_EX_CLIENTEDGE;
		    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
	}
#endif
    }

#if 1
    SetWindowLong(hWnd, GWL_STYLE, s);

    int wx, wy;

    sizeOfWin(&wx, &wy);
    size(wx-1, wy-1);
    size(wx, wy);
#endif
   // frame();
}
//----------------------------------------------------------------------------

boolean VWindow::hasBorder() const
{
    return style.contains(StyleBorder) ? TRUE : FALSE;

//  jmd
//  long s = GetWindowLong(hWnd, GWL_STYLE);
//  if (s & WS_BORDER) {
//      return TRUE;
//  }
//  else {
//      return FALSE;
//  }
}

//----------------------------------------------------------------------------
//  Set the default framing ratios in this VWindow.
//
void VWindow::setDefRatio()
{
    winFrame.set(0, 0, 0, 0);
    updateGeometry();
}

//----------------------------------------------------------------------------
//  Set the framing ratio of this VWindow to ('x', 'y') position, ('w', 'h')
//  width, and height respectively and update the window.  Note: 'int' and
//  'float' constants are automatically converted to 'VRatio' objects by
//  the compiler.
//
void VWindow::setRatio(VRatio x, VRatio y, VRatio w, VRatio h)
{
    winFrame.set(x, y, w, h);
    updateGeometry();
    moveWin();
}

//----------------------------------------------------------------------------
//  PRIVATE: Enable this VWindow if the parameter is TRUE, else disable it
//
void VWindow::enableBool(boolean b)
{
    if (hWnd != 0) {
		if (b) {
			if (!IsWindowEnabled(hWnd)) {
			EnableWindow(hWnd, 1);
			}
		}
		else {
			if (IsWindowEnabled(hWnd)) {
				boolean was_visible = IsWindowVisible(hWnd);
			EnableWindow(hWnd, 0);
				if (was_visible) {
			shedFocus();
				}
			}
		}
    }
}

//----------------------------------------------------------------------------
//  PRIVATE: Show this VWindow if the parameter 'b' is TRUE, otherwise
//  hide it
//
void VWindow::showBool(boolean b, boolean update)
{
	if (hWnd != 0) {
		if (b) {
			if (!IsWindowVisible(hWnd)) {
			    ShowWindow(hWnd, SW_SHOWNORMAL);
			    if (update && IsWindowVisible(hWnd)) {
					UpdateWindow(hWnd);
			    }
			}
			if (isTopLevel() &&
				(((curChildFocus == 0) &&
				  (getFocusCandidate(100) != 0)) ||
				 (notifier->getFocus() != this))) {
				givenFocus();
			}
		}
		else {
			boolean was_visible = IsWindowVisible(hWnd);
			ShowWindow(hWnd, SW_HIDE);
			if (was_visible) {
				shedFocus();
			}
		}
	}
}

//----------------------------------------------------------------------------
//  Set the width and height for this VWindow to 'w' and 'h' respectively.
//
void VWindow::size(int w, int h)
{
    int i, x, y;
    if (isA(VPopupWindowCls)) {
	getAbs(&x, &y, &i, &i);
    }
    else {
	getRel(&x, &y, &i, &i);
    }
    move(x, y, w, h);
}

// --------------------------------------------------------------------------
// Called when this VWindow should take the keyboard focus for itself.
// If this VWindow or one of its descendents (within the same top-level
// window) are eligible candidates to receive the focus (i.e. enabled, not
// hidden, etc.), then actually set the focus to this VWindow (or, as an
// optimization, to the appropriate descendent), and return TRUE.  Otherwise,
// if this VWindow nor any descendents are eligible to receive the focus,
// then simply return FALSE.  This method may be overridden to return FALSE
// if the keyboard focus is not wanted.
//
boolean VWindow::takeFocus()
{
	VWindow *focus_window = getFocusCandidate();
	if (focus_window != 0) {
		if (focus_window->hasFocus()) {
			return TRUE;
		}
		else {
		return notifier->setFocus(focus_window);
		}
	}
    else {
	return FALSE;
    }
}

// --------------------------------------------------------------------------
// Called when this VWindow has been given the keyboard focus.  The default
// behavior is to accept the focus and pass it on to an eligible descendent
// (within the same top-level window).  Subclasses may override this method
// and pass the focus onto any desired child window.
//
boolean VWindow::givenFocus()
{
	VWindow *focus_window = getFocusCandidate();
	if (focus_window != 0) {
		if ((parent != 0) && !isTopLevel()) {
			parent->setChildFocus(this);
		}
		focus_window->setChildFocus(0);
		if (focus_window != this) {
			if (!focus_window->takeFocus()) {
				if (focus_window->parent != 0) {
					focus_window->parent->nextChildFocus(focus_window);
				}
			}
		}
	}
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->givenFocus()) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

// --------------------------------------------------------------------------
// Called when this VWindow has just lost the keyboard focus.
//
boolean VWindow::clearFocus()
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->clearFocus()) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

// --------------------------------------------------------------------------
// Called internally when this VWindow is about to lose the focus due to
// a user action, i.e. clicking on or TAB-ing to another focus-eligible
// VWindow.  The default action is to simply return FALSE which means that
// it's okay for this VWindow to lose focus; a return value of TRUE will
// prevent this VWindow from losing focus.  If the given VWindow argument
// is not null then the focus is being lost due to a mouseDn() on that
// VWindow, otherwise the focus is being lost due to keyboard travesal.
// Deriving classes may override this method to perform validation,
// returning TRUE if the validation fails.  The VWindow::LosingFocus event
// vector may be used in the same fashion except that currently, due to the
// insufficiency of the event vector mechanism, the VWindow argument will
// not be specified.
//
boolean VWindow::losingFocus(VWindow *)
{
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::lostFocus()
{
	return FALSE;
}

// --------------------------------------------------------------------------
// Set the child of this VWindow which should have the focus when this
// VWindow is requested to take or is given the focus, to the given VWindow.
//
void VWindow::setChildFocus(VWindow *child)
{
	curChildFocus = child;
	if ((child != 0) && (parent != 0) && !isTopLevel()) {
		parent->setChildFocus(this);
	}
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		getComponentThis()->setChildFocus(child);
	}
#endif
}

// --------------------------------------------------------------------------
// If this VWindow object has the keyboard focus, then move the focus to the
// next window in the focus ordering.
//
void VWindow::shedFocus()
{
    if (beingDeleted || beingDestroyed) {
		return;
	}
	VWindow *focus_window = containsFocus();
	if (focus_window == 0) {
		return;
	}
#ifndef CV_NOVECTOREVENTS
	if (!focus_window->vectorEvent(VWindow::ClearFocus)) {
		focus_window->clearFocus();
	}
#else
	focus_window->clearFocus();
#endif
	if (focus_window->parent != 0) {
		focus_window->parent->setChildFocus(0);
	}
	for (VWindow *w = focus_window ; 1 ; w = w->parent) {
		if (w->parent == 0) {
			break;
		}
		else if (w->parent->takeFocus()) {
			return;
		}
	}
	notifier->setFocus(0);
}

// --------------------------------------------------------------------------
// Called when this VWindow should pass the keyboard focus to its next
// sibling.  This call is normally generated when the user presses the
// TAB key.
//
boolean VWindow::nextFocus()
{
    if ((parent != 0) && !isTopLevel()) {
		return parent->nextChildFocus(this);
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
// Called when this VWindow should pass the keyboard focus to its previous
// sibling.  This event is normally generated when the user presses the
// SHIFT-TAB key.
//
boolean VWindow::prevFocus()
{
    if ((parent != 0) && !isTopLevel()) {
		return parent->prevChildFocus(this);
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
// Pass the keyboard focus to a child window of this VWindow following
// the given VWindow.
//
boolean VWindow::nextChildFocus(VWindow *this_child)
{
	if (processingNextChildFocus) {
		return FALSE;
	}

	processingNextChildFocus = 1;

	VWindow *focus = notifier->getFocus();

	for (VWindow *parent = this, *child = this_child ; 1 ; ) {
		if ((child = parent->getNextChildFocusCandidate(child)) == 0) {
			break;
		}
		if (focus != 0) {
	        if (VWindow::checkLosingFocus()) {
				break;
			}
			focus = 0;
		}
		if (child->takeFocus()) {
			processingNextChildFocus = 0;
			return TRUE;
		}
		if (child == this_child) {
			break;
		}
		parent = child->parent;
	}

	processingNextChildFocus = 0;
    return FALSE;
}

// --------------------------------------------------------------------------
//  Pass the keyboard focus to a child window of this VWindow preceding
//  the child 'win'.
//
boolean VWindow::prevChildFocus(VWindow *this_child)
{
	if (processingPrevChildFocus) {
		return FALSE;
	}

	processingPrevChildFocus = 1;

	VWindow *focus = notifier->getFocus();

	for (VWindow *parent = this, *child = this_child ; 1 ; ) {
		if ((child = parent->getPrevChildFocusCandidate(child)) == 0) {
			break;
		}
		if (focus != 0) {
	        if (VWindow::checkLosingFocus()) {
				break;
			}
			focus = 0;
		}
		if (child->takeFocus()) {
			processingPrevChildFocus = 0;
			return TRUE;
		}
		if (child == this_child) {
			break;
		}
		parent = child->parent;
	}

	processingPrevChildFocus = 0;
    return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::controlNextFocus()
{
	if ((parent != 0) && !isTopLevel()) {
		return parent->controlNextFocus();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VWindow::controlPrevFocus()
{
	if ((parent != 0) && !isTopLevel()) {
		return parent->controlPrevFocus();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VWindow::controlNextChildFocus(VWindow *child)
{
	return nextChildFocus(child);
}

// --------------------------------------------------------------------------
//
boolean VWindow::controlPrevChildFocus(VWindow *child)
{
	return prevChildFocus(child);
}

// --------------------------------------------------------------------------
//
VWindow *VWindow::getNextChildFocusCandidate(VWindow *this_child) const
{
	if (children == 0) {
		return 0;
	}

	int count = children->count();

	if (count <= 0) {
		return 0;
	}

	int start_index = this_child == 0 ?
					  -1 : children->indexOfSame(this_child);

	for (int index = start_index + 1 ; 1 ; index++) {
		if (index >= count) {
			if (isTopLevel()) {
				index = 0;
			}
			else if (parent != 0) {
				return (VWindow *)parent->
								  getNextChildFocusCandidate((VWindow *)this);
			}
			else {
				break;
			}
		}
		VWindow *child = (VWindow *)children->idAt(index);
		if (child == 0) {
			break;
		}
		if (!child->isTopLevel()) {
			child = child->getFocusCandidate(1);
			if (child != 0) {
				return child;
			}
		}
		if (index == start_index) {
			break;
		}
	}

	return 0;
}

// --------------------------------------------------------------------------
//
VWindow *VWindow::getPrevChildFocusCandidate(VWindow *this_child) const
{
	if (children == 0) {
		return 0;
	}

	int count = children->count();

	if (count <= 0) {
		return 0;
	}

	int start_index = this_child == 0 ?
					  count : children->indexOfSame(this_child);

	for (int index = start_index - 1 ; 1 ; index--) {
		if (index < 0) {
			if (isTopLevel()) {
				index = count - 1;
			}
			else if (parent != 0) {
				return (VWindow *)parent->
								  getPrevChildFocusCandidate((VWindow *)this);
			}
			else {
				break;
			}
		}
		VWindow *child = (VWindow *)children->idAt(index);
		if (child == 0) {
			break;
		}
		if (!child->isTopLevel()) {
			child = child->getFocusCandidate(-1);
			if (child != 0) {
				return child;
			}
		}
		if (index == start_index) {
			break;
		}
	}

	return 0;
}

// --------------------------------------------------------------------------
// If this window or one of its descendent windows (within the same top-level
// window) are eligible (i.e. not hidden, not disabled, not top-level, and
// getFocusCandidate()) to receive the focus, then return a pointer to this
// window or the descendent window as is appropriate, otherwise return null.
// Note that if a (parent) window has an eligible descendent, then that
// descendent will be chosen regardless of the setting of the allowFocus
// flag on the parent window itself; if the parent window does *not* have
// any eligible descendents (but *does* have non top-level children), the
// allowFocus flag on the parent window controls whether or not focus is
// allowed on itself.  For a (leaf) window which has no children or only
// top-level children, the allowFocus flag simply controls whether or not
// the window may receive focus.
//
VWindow *VWindow::getFocusCandidate(int mode) const
{
	if (!((VWindow *)this)->isEnabled()) {
		return 0;
	}

	boolean consider_hidden;

	if (mode == 100) {
		mode = 0;
		consider_hidden = TRUE;
	}
	else if (mode == 101) {
		mode = 1;
		consider_hidden = TRUE;
	}
	else if (mode == -101) {
		mode = -1;
		consider_hidden = TRUE;
	}
	else {
		consider_hidden = FALSE;
	}

	if (((VWindow *)this)->isHidden()) {
		if (consider_hidden) {
			if (style.contains(StyleHidden)) {
				return 0;
			}
		}
		else {
			return 0;
		}
	}

	if (children != 0) {

		boolean explicit_traversal              = mode ==  0;
		boolean foreward_keyboard_traversal     = mode ==  1;
		boolean backward_keyboard_traversal     = mode == -1;
		boolean found_non_top_level_child       = FALSE;

		if (explicit_traversal && (curChildFocus != 0)) {
			VWindow *focus_window = curChildFocus->getFocusCandidate(mode);
			if (focus_window != 0) {
				return focus_window;
			}
		}
		if (backward_keyboard_traversal) {
			REVERSEDO (*((VWindow *)this)->children, VWindow, child)
				found_non_top_level_child = TRUE;
				if (!child->isTopLevel()) {
					VWindow *focus_window = child->getFocusCandidate(mode);
					if (focus_window != 0) {
						return focus_window;
					}
				}
			END
		}
		else {
			DO (*children, VWindow, child)
				if (!child->isTopLevel()) {
					found_non_top_level_child = TRUE;
					VWindow *focus_window = child->getFocusCandidate(mode);
					if (focus_window != 0) {
						return focus_window;
					}
				}
			END
		}

		// Here, there are no eligible descendents.
	}

	if (allowFocus && ((VWindow *)this)->isTabStop()) {
		return (VWindow *)this;
	}

	return 0;
}

// --------------------------------------------------------------------------
// If this VWindow or one of its descendents (within the same top-level
// window) have the keyboard focus, then return the a poniter to the
// VWindow which has the focus, otherwise return null.
//
VWindow *VWindow::containsFocus() const
{
	if ((hWnd == 0) || !IsWindowVisible(hWnd)) {
		return 0;
	}

	VWindow *focus_window = notifier->getFocus();

	if (focus_window == this) {
		return focus_window;
	}
	else if ((focus_window != 0) && !focus_window->isTopLevel()) {
		for (VWindow *w = focus_window->parent ; w != 0 ; ) {
			if (w == this) {
				return focus_window;
			}
			else if (w->isTopLevel()) {
				break;
			}
			else {
				w = w->parent;
			}
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
// Return TRUE if this VWindow has the keyboard focus, otherwise return FALSE.
//
boolean VWindow::hasFocus() const
{
	if ((hWnd == 0) || !IsWindowVisible(hWnd)) {
		return 0;
	}
	return notifier->getFocus() == this;
}

// --------------------------------------------------------------------------
//
void VWindow::setTabStop(boolean enabled)
{
    bTabStop = enabled;
}

// --------------------------------------------------------------------------
//
boolean VWindow::isTabStop() const
{
    return bTabStop;
}

// --------------------------------------------------------------------------
//
void VWindow::setTabIndex(VWindow *child, int index)
{
	if ((children != 0) && (child != 0)) {
		int nchildren = children->count();
		if (nchildren > 0) {
			int current_index = children->indexOf(child);
			if (current_index != index) {
				if ((index < 0) || (index >= nchildren)) {
					index = nchildren - 1;
				}
				children->insertAt(index, children->removeAt(current_index));
			}
		}
	}
}
// --------------------------------------------------------------------------

//
void VWindow::swapTabIndices(VWindow *child1, VWindow *child2)
{
	if ((children != 0) &&
		(child1 != 0) && (child2 != 0) && (child1 != child2)) {
		int nchildren = children->count();
		if (nchildren > 0) {
			int index1 = children->indexOf(child1);
			int index2 = children->indexOf(child2);
			if (index1 > index2) {
				int tmp_index = index1;
				VWindow *tmp_child = child1;
				index1 = index2;
				child1 = child2;
				index2 = tmp_index;
				child2 = tmp_child;
			}
			children->removeSame(child1);
			children->removeSame(child2);
			children->insertAt(index1, child2);
			children->insertAt(index2, child1);
		}
	}
}

// --------------------------------------------------------------------------
//
int VWindow::getTabIndex(VWindow *child) const
{
	return children != 0 ? children->indexOf(child) : -1;
}

// --------------------------------------------------------------------------
//
void VWindow::setTabIndex(int index)
{
	if (parent != 0) {
		parent->setTabIndex(this, index);
	}
}

// --------------------------------------------------------------------------
//
int VWindow::getTabIndex() const
{
	return parent != 0 ? parent->getTabIndex((VWindow *)this) : -1;
}

// --------------------------------------------------------------------------
//
void VWindow::disableFocus(boolean b)
{
	if (b) {
		if (!allowFocus) {
			return;
		}
		allowFocus = 0;
		if (containsFocus()) {
			shedFocus();
		}
	}
	else {
		if (allowFocus) {
			return;
		}
		allowFocus = 1;
	}
}

// --------------------------------------------------------------------------
//
boolean VWindow::checkLosingFocus(VWindow *focus_requester)
{
	VWindow *focus_owner = notifier->getFocus();

	if (focus_owner == 0) {
		return FALSE;
	}
	else if (focus_requester != 0) {
		if (focus_owner == focus_requester) {
			return FALSE;
		}
		else {
			VWindow *real_focus_requester =
						focus_requester->getFocusCandidate();
			if (focus_owner == real_focus_requester) {
				return FALSE;
			}
			else if (real_focus_requester != 0) {
				focus_requester = real_focus_requester;
			}
		}
	}

#ifndef CV_NOVECTOREVENTS
	if (focus_owner->vectorEvent(VWindow::LosingFocus)) {
		return TRUE;
	}
	else
#endif

	return focus_owner->losingFocus(focus_requester);
}

// --------------------------------------------------------------------------
//
boolean VWindow::key(int virtual_key, char ascii_key)
{
	switch (virtual_key) {

		case K_Tab:
			return nextFocus();

		case K_Tab | K_Mod_Shift:
			return prevFocus();

	    case K_Tab | K_Mod_Control:
			return controlNextFocus();

	    case K_Tab | K_Mod_Shift | K_Mod_Control:
			return controlPrevFocus();

		case K_Return:
			return enterHit();

		case K_Escape:
			return escapeHit();
	}

	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::propagateKey(int virtual_key, char ascii_key)
{
	if ((parent != 0) && !isTopLevel()) {
		return parent->key(virtual_key, ascii_key);
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
// Called when the ENTER key has been hit while this VWindow has the keyboard
// focus.  Informs the parent window by calling enterHit() on it.  Typically
// overriden by subclasses for specialized behavior.
//
boolean VWindow::enterHit()
{
	if ((parent != 0) && !isTopLevel()) {
		return parent->enterHit();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
// Called when the ESCAPE key has been hit while this VWindow has the
// keyboard focus. Inform the parent window by calling enterHit() on it.
// Typically overriden by subclasses for specialized behavior.
//
boolean VWindow::escapeHit()
{
	if ((parent != 0) && !isTopLevel()) {
		return parent->escapeHit();
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VWindow::keyUp(int, char)
{
    return FALSE;
}

//----------------------------------------------------------------------------
//  Make this VWindow visible on the display screen. If 'update' is
//  TRUE, then update the display immediately (that is, cause the paint()
//  event to be sent). Otheriwse, defer the client area display until the
//  update() method is called. If 'update' is not spcified, its default
//  value is TRUE.
//
void VWindow::show(boolean update)
{
    showBool(TRUE, update);
}

//----------------------------------------------------------------------------
//  Force an exposure event for this VWindow and therefore, cause it to
//  redisplay.  Propogates to all child windows, no matter how many layers
//  deep they are nested.
//
boolean VWindow::update()
{
	if (hWnd != 0) {
		InvalidateRect(hWnd, LPRECT(0), BOOL(1));
		UpdateWindow(hWnd);
		if (children != 0) {
			DO(children, VWindow, child)
				if (!child->isTopLevel()) {
					child->update();
				}
			END
		}
	}
	return TRUE;
}

//----------------------------------------------------------------------------
//  Called if this VWindow has just been moved to a position given by
//  ('x','y').
//
boolean VWindow::moved(int x, int y)
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->moved(x, y)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called when a mouse button double-click occured within this VWindow
//  with the ('x','y') coordinates of the mouse.  This event follows a
//  mouseDn event.
//
boolean VWindow::mouseDbl(int x, int y)
{
	if (dockableWindowOwner != 0) {
		mapToScreen(&x, &y);
		dockableWindowOwner->mapToWin(&x, &y);
		return dockableWindowOwner->mouseDbl(x, y);
	}
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->mouseDbl(x, y)) {
			return TRUE;
		}
	}
#endif
    return FALSE;
}

//----------------------------------------------------------------------------
//  Called when a mouse button has been pressed down
//  within this VWindow with the ('x','y') coordinates of the mouse.
//
boolean VWindow::mouseDn(int x, int y)
{
	if (dockableWindowOwner != 0) {
		mapToScreen(&x, &y);
		dockableWindowOwner->mapToWin(&x, &y);
		return dockableWindowOwner->mouseDn(x, y);
	}
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->mouseDn(x, y)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called when the mouse has just entered this VWindow.  If 'bStat'
//  is TRUE, then the mouse button is down.
//
boolean VWindow::mouseEnter(int b)
{

#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->mouseEnter(b)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called when the mouse has just left this VWindow.
//  If 'b' is TRUE then, the mouse button is down.
//
boolean VWindow::mouseExit(int  b)
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->mouseExit(b)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called if mouse tracking is enabled through the VNotifier.  The
//  current mouse position is ('x', 'y') and the mouse button status is 'b'.
//  If 'b' is TRUE, then the mouse button is down.
//
boolean VWindow::mouseMv(int x, int y, int b)
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->mouseMv(x, y, b)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called if the mouse the button has been released
//  within this VWindow; the mouse position is given by ('x','y').
//
boolean VWindow::mouseUp(int x, int y)
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->mouseUp(x, y)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::mouseWheel(int, int, int, int)
{
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called if a right mouse button double-click occurred
//  within this VWindow at the given position ('x','y'). This event follows a
//  rmouseDn event.  NOTE: Not all mice have right buttons, supporting this
//  event may not be portable to other computer platforms.
//
boolean VWindow::rmouseDbl(int x, int y)
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->rmouseDbl(x, y)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called if a right mouse button press occured within this VWindow
//  at the given position ('x','y').  NOTE: Not all mice have right buttons,
//  supporting this event may not be portable to other computer platforms.
//
boolean VWindow::rmouseDn(int x, int y)
{
	if (dockableWindowOwner != 0) {
		mapToScreen(&x, &y);
		dockableWindowOwner->mapToWin(&x, &y);
		return dockableWindowOwner->rmouseDn(x, y);
	}
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->rmouseDn(x, y)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

//----------------------------------------------------------------------------
//  Called if the RIGHT mouse button was released within this VWindow
//  at the given position ('x','y').  NOTE: Not all mice have right buttons,
//  supporting this event may not be portable to other computer platforms.
//
boolean VWindow::rmouseUp(int x, int y)
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->rmouseUp(x, y)) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::mmouseDn(int, int)
{
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::mmouseUp(int, int)
{
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::mmouseDbl(int, int)
{
	return FALSE;
}

//----------------------------------------------------------------------------
//  Force an exposure event of the rectangular area 'rect' of this
//  VWindow.  If 'immediate' is TRUE (the default value if not specified), the
//  exposure event (that is, paint()) is generated immediately.  Otherwise,
//  if 'immediate' is FALSE, the exposure event is deferred until the
//  update() method is called on this VWindow. Deferring the exposure event
//  is useful when various member functions are changing different parts of a
//  window and you don't want the display repainted for each of those changes.
//  This function automatically propogates to the first layer of child windows.
//
void VWindow::update(VRectangle *rect, boolean immediate)
{
  if (hWnd) {
     if (rect) {

#if defined(CV_WIN32)
	RECT wRect;
	wRect.left      = rect->left;
	wRect.top       = rect->top;
	wRect.right     = rect->right;
	wRect.bottom    = rect->bottom;

	InvalidateRect(hWnd, &wRect, (BOOL)1);
#else
	InvalidateRect(hWnd, (LPRECT)&rect->left, (BOOL)1);
#endif
    }
    else {
	InvalidateRect(hWnd, (LPRECT)0, (BOOL)1);
    }
    if (immediate) {
	UpdateWindow(hWnd);
    }
  }
}

//----------------------------------------------------------------------------
//  WARNING: this method is not-portable, it is here for the use of
//  experienced WINDOWS programmers only.
//
//  All MS WINDOWS messages above WM_USER will be sent to this event method.
//  VControl classes do not receive this event.
//
//  n is a Windows message value
//  w is the wParam of the message
//  l is the lParam of the message
//
//  All MS Windows or OS/2 PM events above WM_USER are sent here
//  (including DDE events). The MS Windows or OS/2 PM message number is 'n',
//  the wParam is 'w', and the lParam is 'l'.  Return FALSE if the event is
//  not consumed, otherwise return TRUE.
//  WARNING:  This event method is NOT portable.
//
boolean VWindow::user(UINT n, WPARAM w, LPARAM l)
{
/*
    char buffer[80];
    sprintf(buffer, "USER #%d, W=%X, l=%lX", n,w,l);
    print(buffer);
*/
    return FALSE;
}

//----------------------------------------------------------------------------
//  Write the contents of VString object 's' into this VWindow\'s
//  client area at position ('x', 'y').
//
void VWindow::wrtText(VString *s, int x, int y)
{
    if (s) {
	wrtText(s->gets(), x, y);
    }
}

//----------------------------------------------------------------------------
//  Write C string 's' into this VWindow\'s client area at position
//  ('x', 'y').
//
void VWindow::wrtText(const char *s, int x, int y)
{
    VColor tb, tc;

    getHndl();
    if (pBackground) {
	tb = pBackground->background();
    }
    else {
	tb = GetSysColor(COLOR_WINDOW);
    }

    tb = SetBkColor(hDC, tb);
    if (defaultForeground) {
	tc = SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT));
    }
    else {
	tc = SetTextColor(hDC, pForeground);
    }
    TextOut(hDC, x, y, (LPSTR)s, strlen(s));
    SetBkColor(hDC, tb);
    SetTextColor(hDC, tc);
    ungetHndl();
}

//----------------------------------------------------------------------------
//  Called when the user has initiated some action to close this
//  VWindow.  By returning TRUE this implementation has no effect on the
//  dispositon of this VWindow.  Subclasses should reimplement this method
//  to achieve different results. Note, a return value of FLASE to the
//  VNotifier will cause this VWindow to be destroyed.
//
boolean VWindow::close()
{
#ifndef CV_NOOLE
	if (getComponentThis() != 0) {
		if (getComponentThis()->close()) {
			return TRUE;
		}
	}
#endif
	return TRUE;
}

//----------------------------------------------------------------------------
//  Scroll this VWindow by shifting the client area by 'dx' units in
//  the x direction and by 'dy' units in the y direction.  Move the client area
//  to the right if 'dx' is positive or to the left if 'dx' is negative.
//  Likewise, move the client area down if 'dy' is positive or up if 'dy' is
//  negative.
//
void VWindow::scroll(int dx, int dy)
{
    ScrollWindow(hWnd, dx, dy, (LPRECT)0, (LPRECT)0);
}

//----------------------------------------------------------------------------
//  Remove the child 'win' from this VWindow\'s collection of child
//  windows.
//
boolean VWindow::removeChild(VWindow *win)
{
	if (children == 0) {
		return FALSE;
	}
	if ((geoFrame != 0) && geoFrame->isManager()) {
		geoManager->removeChild(win);
	}
	return children->removeSame(win) ? TRUE: FALSE;
}

//----------------------------------------------------------------------------
//  Hash on the GUI system handle of this VWindow.
//
HashKey VWindow::hash() const
{
    return long(hWnd) & HASH_KEY_MASK;
}

//----------------------------------------------------------------------------
// Compare on the GUI system handle of this VWindow.
// Note: derived window classes should not override this.
// Two VWindows are equal if they refer to the same GUI object--jmd
//
//
boolean VWindow::isEqual(VObject *obj) const
{
    if (!obj->isA(VWindowCls)) {
	return FALSE;
    }
    else {
	return (hWnd == ((VWindow *) obj)->hWnd) ? TRUE : FALSE;
    }
}

//----------------------------------------------------------------------------
//  Return the size of the client area of this VWindow\'s parent
//  (or the screen if there is no parent) as a VLocation.
//
VLocation VWindow::dimension()
{
    int w, h;
    if (parent) {
	parent->sizeOfImage(&w, &h);
    }
    else {
	notifier->getScreenSize(&w, &h);
    }
    return VLocation(w, h);
}

//----------------------------------------------------------------------------
//  Set the frame of this VWindow to 'frame'. If 'update' is TRUE,
//  then an exposure event (that is, a paint() call) is generated on this
//  VWindow.  Otherwise, if 'update' is FALSE, no exposure event is produced.
//  The default value for 'update' is set to TRUE.  However, a value of FALSE
//  for 'update' can be used to prevent multiple repainting when several child
//  windows are being moved.
//
void VWindow::move(const VFrame &frame, boolean update)
{
    winFrame = frame;
    updateGeometry();
    moveWin(update);
}

//----------------------------------------------------------------------------
//  Build the GUI system window presentation space associated with this
//  VWindow using frame 'frame', parent 'win' and style 'style'. This method
//  is used on VWindow objects constructed with their NULL constructor or from
//  within constructors of subclasses.
//
void VWindow::operator()(const VFrame &frame, VWindow *win, const VStyle& style)
{
    winFrame = frame;
    parent = win == 0 ? 0 :  win->getClientAreaWindow();
    build(style);
}

//----------------------------------------------------------------------------
//
void VWindow::operator()(const VFrame &frame, VWindow *win, const VStyle& style,
			 const VString& wname)
{
    winFrame = frame;
    parent = win == 0 ? 0 :  win->getClientAreaWindow();
    build(style);
    setName(wname);
}

//----------------------------------------------------------------------------
//  Called when this VWindow\'s background is about to be erased. If
//  a background VBrush is set ( via the setBackground(VBrush *) method),
//  then that VBrush object will be used to paint the background of this
//  VWindow.  Otherwise, the defualt system color will be used. This method
//  can be overridden by subclasses to provide for custom window backgrounds.
//
boolean VWindow::erased()
{
    if ((pBackground == 0) || (pBackground->hbrush == 0) ||
		(hWnd == 0) && (hDC == 0)) {
	return FALSE;
    }
	if ((hWnd != 0) && (hDC != 0)) {
	    RECT r;
		GetClientRect(hWnd, LPRECT(&r));
		FillRect(hDC, &r, pBackground->hbrush);
    }
	return TRUE;
}

//----------------------------------------------------------------------------
//  Add the width and height of the border of this VWindow to
//  '*w' and '*h', respectively.
//
void VWindow::stdSize(const VStyle& style, int *w, int *h)
{
    if (style.contains(StyleBorder)) {
		*w += GetSystemMetrics(SM_CXBORDER) * 2;
		*h += GetSystemMetrics(SM_CYBORDER) * 2;
    }
}

//----------------------------------------------------------------------------
//  Deallocate this VWindow and all of its child windows.
//
void VWindow::clean()
{
    // Hide the cascade of our children's destruction.

    hide();

    // Child management.

    if (children != 0) {
		REVERSEDO(children, VWindow, child)
		    delete child;
		END
		delete children;
		children = 0;
    }

    // Parent management.

    if ((parent != 0) && (parent->children != 0)) {
		parent->children->removeSame(this);
		// remove the focus from myself.
		if (parent->curChildFocus == this) {
		    if (this == notifier->focus) {
				// fixes a rare bug
				notifier->focus =0;
		    }
		    parent->curChildFocus = 0;
		}
		if (parent->isBeingDeleted()) {
			VGeoForm *geoform = parent->getGeoForm();
			if (geoform != 0) {
				geoform->detachChild(geoFrame);
			}
		}
		parent->childRemoved(this, 0);
    }

    // Compound Document Interface.

#ifndef CV_NOOLE
    if (componentWindow) {
//      delete componentWindow;
	componentWindow->destroy();
//      componentWindow->Release();
	// the release should have deleted the component object, which should have
	// set the componentWindow pointer to zero.
// rhw 4/12/96: Someone commented out the Release above but did not comment out
// the cv_assert below, so I took care of it.
//      cv_assert(componentWindow == 0);
    }
#elif !defined(CV_NOARCHIVER)
    if ((componentWindow != 0) && componentWindowIsRawMemory) {
		delete (VObjectRawMemory *)componentWindow;
    }
#endif

    // Self management.

    parent = 0;
    notifier->destroyWin(this);

    // Remove window name.

    delete winName;
	winName = 0;

    // Delete resources.

	delete geoFrame;
	geoFrame = 0;

    if ((pFontFromArchive != 0) && !pFontFromArchive->inUse()){
		delete pFontFromArchive;
		// recover the font (if any that was realized from the archive.
		pFontFromArchive  = 0;
    }

#ifndef CV_NOARCHIVER
    if (childrenZorder != 0) {
    	delete childrenZorder;
    	childrenZorder = 0;
	}
	if (oldAttachArchiveData != 0) {
		delete oldAttachArchiveData;
		oldAttachArchiveData = 0;
	}
#endif

#ifndef CV_NOVECTOREVENTS
	if (theEventManager != 0) {
		delete (VWindowEventManager *)theEventManager;
	}
#endif

	if (backgroundBrushIsMine) {
		if (pBackground != 0) {
			delete pBackground;
			pBackground = 0;
		}
		backgroundBrushIsMine = 0;
	}

#ifdef CV_OLE_DRAG_DROP
	if (dropSite != 0) {
		dropSite->unregisterWindow(this);
	}
	if (dragSite != 0) {
		dragSite->unregisterWindow(this);
	}
#endif

	if (getPalette() != 0) {
		notifier->unregisterPaletteWindow(this);
	}

}

//----------------------------------------------------------------------------
//  Return the width and height of this VWindow in screen pixel units in
//  '*w' and '*h' respectively.
//
void VWindow::getAbs(int *w, int *h)
{
    int x, y;
    getAbs(&x, &y, w, h);
}

//----------------------------------------------------------------------------
//  Called for all MS Windows events with VEvent object 'ev'.
//  The MS Windows event parameters are public variables of VEvent object
//  'ev' and they are obtained directly:
//
//      ev.msg
//      ev.wParam
//      ev.lParam
//
//  This method is called only if the VNotifier is enabled to do so by
//  VNotifier::enableWinEvents(TRUE).  This method is called by the
//  VNotifier before the MS VWindow event is dispatched to any of the
//  other virtual functions of VWindow objects.  Therefore, events such
//  as WM_PAINT can be grabbed and processed before they are dispatched
//  via the paint() method.
//
//  You should return:
//
//      1       to indicate the event was processed by this method and that
//              VNotifier should not dispatch the event and not call
//              the default window procedure
//      0       to indicate that the event was not processed by this method
//              and that the VNotifier should proceed with normal processing
//     -1       to indicate that the event was not processed by this
//              method and that the VNotifier should call the default window
//              procedure.
//
//  If the event is processed by this method (you return 1), then you can
//  pass an additional value (a long) back to the sender of this event
//  through the given VEvent object by ev.returnValue.
//
//  WARNING:  This event method may NOT be portable.
//
long VWindow::winDispatch(VEvent &ev)
{
    return 0;
}

//----------------------------------------------------------------------------
//  Retrieve GUI system specific information about this VWindow.
//
void VWindow::getInfo(VWinInfo &info)
{
    info.wClass = notifier->CTwindow();
    info.style = WS_CHILD | WS_CLIPCHILDREN;
	if (!style.contains(StyleHidden)) {
	    info.style |= WS_VISIBLE;
	}
    info.exStyle = 0;

    if (style.contains(StyleBorder)) {
	static int CV_NEARDATA windows95 = LOBYTE(LOWORD(GetVersion())) >= 4;
	if (windows95) {
	    info.exStyle |= WS_EX_CLIENTEDGE;
	}
	else {
	    info.style |= WS_BORDER;
	}
    }
}

// --------------------------------------------------------------------------
//
// For objects that don't sub class this method, return the color of the
// dialog.
//
UINT VWindow::getCtlColor()
{
#if defined(CV_WIN32)
    return WM_CTLCOLORDLG;
#else
    return CTLCOLOR_DLG;
#endif
}

//----------------------------------------------------------------------------
//  Build the GUI system window associated with this VWindow object
//  with default style (that is, StyleDefault), with frame 'frame' and
//  parent 'win'. This method is used on VWindow objects constructed with
//  their NULL constructor or from within constructors of subclasses.
//
void VWindow::operator()(const VFrame &frame, VWindow *win)
{
    (*this)(frame, win, StyleDefault);
}

//----------------------------------------------------------------------------
//  Set this VWindow\'s background VBrush to 'b'. If 'all' is TRUE,
//  then propagate the background VBrush 'b' to all child windows (the
//  default for 'all' is FALSE).
//
//  Note that VBrush 'b' is not owned by this VWindow and, hence, it
//  is not destroyed when this VWindow is destroyed.
//
void VWindow::setBackground(VBrush *b, long all)
{
	if (backgroundBrushIsMine) {
		if (pBackground != 0) {
			delete pBackground;
		}
		backgroundBrushIsMine = 0;
	}
    pBackground = b;
    if (all && children) {
	children->doPerform(methodOf(VWindow, setBackground), (long)b, all);
    }
}

//----------------------------------------------------------------------------
//  Set this VWindow\'s foreground color to the VColor value 'color'.
//  If 'all' is TRUE, then propagate the foreground color 'color' to all child
//  windows (the default for 'all' is FALSE).
//
void VWindow::setForeground(VColor color, long all)
{
    pForeground = color;
    defaultForeground = FALSE;
    if (all && children) {
	children->doPerform(methodOf(VWindow, setForeground), color, all);
    }
}

// --------------------------------------------------------------------------
//
void VWindow::setDefaultForeground(boolean b)
{
	defaultForeground = b;
}

//----------------------------------------------------------------------------
//  Set this VWindow\'s VMouseCursor to 'c'. If 'all' is TRUE, then
//  propagate the VMouseCursor 'c' to all child windows (the default for
//  'all' is FALSE).
//
//  Note that the VMouseCursor 'c' is not owned by this VWindow and,
//  hence, it is not destroyed when this VWindow is destroyed.
//
void VWindow::setCursor(VMouseCursor *c, long all)
{
    if (((pCursor = c) != 0) && !all) {
	c->attachTo(this);
	if( !isHidden() ) {         // Don't set cursor for hidden window
	    SetCursor(c->getHndl());
	}
    }
    if (all && (children != 0)) {
	children->doPerform(methodOf(VWindow, setCursor), (long)c, all);
    }
}

//----------------------------------------------------------------------------
//  Return the type of this VWindow.
//
//  The window type may be one of the following:
//
//      WinView
//      WinControl
//      WinMdiApp
//      WinMdiClient
//      WinMdiChild
//      WinWindow
//      WinPseudo
//
enum WinType VWindow::winType()
{
    return WinWindow;
}

//----------------------------------------------------------------------------
//  Private.
//  Sets the name of this VWindow object.
//  VWindow users should use the constructor to create a named window.
//
void VWindow::setName(const VString& wname)
{
    if (wname != (const char *)"") {
	if (winName == 0) {
	    winName = new VString(wname);
	}
	else {
	    winName->puts(wname.gets());
	}
    }
    else if (winName != 0) {
	delete winName;
	winName = 0;
    }
}

//----------------------------------------------------------------------------
//  Searches for the window with the given window-name starting at this window
//  and continuing recursively through all of the children of this window; the
//  name of this window is not included in the search.  If the given window-
//  class is not null, then the type of an window found with the given window-
//  name is checked against the given window-class; if it does not match then
//  null will be returned.
//
//  Note:   search is breadth first.
//
VWindow *VWindow::findWindow(const VString& wname, VClass *cls) const
{
    if (wname.size() == 0) {
	//  null name argument, null name string, empty name string
	return 0;
    }

    if (children == 0) {
	//  no children
	return 0;
    }

    //  first, check our immediate children

    VString *childName;
    DO (*children, VWindow, child)
	childName = child->getName();
	if ((childName == 0) || childName->size() == 0) {
	    continue;
	}
	else {
	    //  !!! pat virtual VObject::isEqual() should take const !!!
	    VString dumb(wname.gets());
	    if (childName->isEqual(&dumb) && (cls == 0 || child->isA(cls))) {
		return child;
	    }
	}
    END

    //  then, ask our children check theirs

    VWindow *hit = 0;
    DO (*children, VWindow, child)
	if ((hit = child->findWindow(wname, cls)) != 0) {
	    return hit;
	}
    END

    return 0;
}

// ---------------------------------------------------------------------------
//
void VWindow::centerWindow(VWindow *window)
{
    int x, y, w, h, wx, wy, ww, wh, sw, sh;

    sizeOfWin(&w, &h);
    notifier->getScreenSize(&sw, &sh);

    if (window == 0) {
	wx = 0;
	wy = 0;
	ww = sw;
	wh = sh;
    }
    else {
	window->getAbs(&wx, &wy, &ww, &wh);
    }

    x = wx + (ww - w) / 2;
    y = wy + (wh - h) / 2;

    if (x < 0) { x = 0; } else if (x + w > sw) { x = sw - w; }
    if (y < 0) { y = 0; } else if (y + h > sh) { y = sh - h; }

    setGeometryPosition(x, y);
}

// ---------------------------------------------------------------------------
// Frame/size this VWindow to it preferred size if the given boolean argument
// is FALSE (this is the default), otherwise frame/size this VWindow to its
// minimum size.
//
void VWindow::frameWindow(boolean minimum)
{
	if ((geoFrame != 0) && geoFrame->isManager()) {
		geoManager->frame(minimum);
	}
}

// ---------------------------------------------------------------------------
//
static void invalidateGeometryOfChildren(VGeoManager *geomanager)
{
	if (geomanager != 0) {
		geomanager->setForceFrameChildren(TRUE);
		VOrdCollect *children = geomanager->getChildren();
		if (children != 0) {
			DO (*children, VGeoFrame, child)
				if (child->isManager()) {
					invalidateGeometryOfChildren((VGeoManager *)child);
				}
				child->update();
			END
		}
	}
}

// ---------------------------------------------------------------------------
//
static void invalidateGeometryFromHereDown(VGeoFrame *geoframe)
{
	if (geoframe != 0) {
		if (geoframe->isManager()) {
			invalidateGeometryOfChildren((VGeoManager *)geoframe);
		}
		geoframe->update();
	}
}

// ---------------------------------------------------------------------------
//
void VWindow::reframeWindow()
{
	invalidateGeometryFromHereDown(geoFrame);

	int current_w, current_h, preferred_w, preferred_h, new_w, new_h;
	sizeOfWin(&current_w, &current_h);
	determinePreferredSize(&preferred_w, &preferred_h);
	new_w = current_w > preferred_w ? current_w : preferred_w;
	new_h = current_h > preferred_h ? current_h : preferred_h;
	if ((current_w != new_w) || (current_h != new_h)) {
		setGeometrySize(new_w, new_h);
	}
	else if ((geoFrame != 0) && geoFrame->isManager()) {
		geoManager->frameChildren();
	}
}

// ---------------------------------------------------------------------------
//
void VWindow::reframeChildren()
{
	if (geoFrame != 0) {
		invalidateGeometryFromHereDown(geoFrame);
		if (geoFrame->isManager()) {
			geoManager->frameChildren();
		}
	}
}

// ---------------------------------------------------------------------------
//
void VWindow::invalidateGeometry(boolean propagate_up)
{
	if (geoFrame != 0) {
		if (geoFrame->isManager()) {
			((VGeoManager *)geoFrame)->setForceFrameChildren(TRUE);
		}
		if (propagate_up) {
			geoFrame->update(-1);
		}
		else {
			geoFrame->update();
		}
	}
}

// ---------------------------------------------------------------------------
// Resize this VWindow such that the cilent area will
// be set to the given width and height.
//
void VWindow::resizeClientArea(int w, int h)
{
    if (hWnd == 0) {
		return;
    }
    int sbw, sbh;
    getSystemBorderSize(&sbw, &sbh);
    setGeometrySize(w + sbw, h + sbh);
}

// ---------------------------------------------------------------------------
//
void VWindow::getSystemBorderSize(int *width, int *height) const
{

    static int CV_NEARDATA windows95 = LOBYTE(LOWORD(GetVersion())) >= 4;
    long win_ex_style, win_style;
    int w, h;

    if (hWnd == 0) {
	if (style.contains(StyleSizable)) {
	    win_style = WS_THICKFRAME;
	    win_ex_style = 0;
	}
	else if (style.contains(StyleModalBorder)) {
	    win_ex_style = WS_EX_DLGMODALFRAME;
	    win_style = 0;
	}
	else if (style.contains(StyleBorder)) {
	    if (windows95 && style.contains(Style3D)) {
		win_ex_style = WS_EX_CLIENTEDGE;
		win_style = 0;
	    }
	    else {
		win_style = WS_BORDER;
		win_ex_style = 0;
	    }
	}
	if (isTopLevel() && style.contains(StyleCaption)) {
	    win_style = WS_CAPTION;
	    win_ex_style = 0;
	}
    }
    else {
	win_ex_style = GetWindowLong(hWnd, GWL_EXSTYLE);
	win_style = GetWindowLong(hWnd, GWL_STYLE);
    }
    if (win_style & WS_THICKFRAME) {
	if (windows95) {
	    w = GetSystemMetrics(SM_CXSIZEFRAME) * 2;
	    h = GetSystemMetrics(SM_CYSIZEFRAME) * 2;
	}
	else {
	    w = GetSystemMetrics(SM_CXFRAME) * 2;
	    h = GetSystemMetrics(SM_CYFRAME) * 2;
	}
    }
    else if (win_ex_style & WS_EX_DLGMODALFRAME) {
	w = GetSystemMetrics(SM_CXBORDER) * 2;
	h = GetSystemMetrics(SM_CYBORDER) * 2;
	if (windows95) {
	    w += GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	    h += GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
	}
	else {
	    w += GetSystemMetrics(SM_CXDLGFRAME) * 2;
	    h += GetSystemMetrics(SM_CYDLGFRAME) * 2;
	}
    }
    else if (win_style & WS_DLGFRAME) {
	w = GetSystemMetrics(SM_CXBORDER) * 2;
	h = GetSystemMetrics(SM_CYBORDER) * 2;
    }
    else if (windows95) {
	if (win_ex_style & WS_EX_CLIENTEDGE) {
	    //
	    // 3D sunken border, normally 2 pixels thick.
	    //
	    w = GetSystemMetrics(SM_CXEDGE) * 2;
	    h = GetSystemMetrics(SM_CYEDGE) * 2;
	}
	else if (win_ex_style & WS_EX_WINDOWEDGE) {
	    //
	    // 3D raised border, normally 2 pixels thick.
	    //
	    w = GetSystemMetrics(SM_CXEDGE) * 2;
	    h = GetSystemMetrics(SM_CYEDGE) * 2;
	}
	else if (win_ex_style & WS_EX_STATICEDGE) {
	    //
	    // 3D sunken border, normally 1 pixel thick.
	    //
	    w = GetSystemMetrics(SM_CXEDGE) * 2;
	    h = GetSystemMetrics(SM_CYEDGE) * 2;
	}
	else if (win_style & WS_BORDER) {
	    w = GetSystemMetrics(SM_CXBORDER) * 2;
	    h = GetSystemMetrics(SM_CYBORDER) * 2;
	}
	else {
	    w = h = 0;
	}
    }
    else if (win_style & WS_BORDER) {
	//
	// Flat border, normally 1 pixel thick.
	//
	w = GetSystemMetrics(SM_CXBORDER) * 2;
	h = GetSystemMetrics(SM_CYBORDER) * 2;
    }
    else {
	w = h = 0;
    }
    if (isTopLevel() && (win_style & WS_CAPTION)) {
	h += GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYBORDER);
    }
    if (width  != 0) { *width  = w; }
    if (height != 0) { *height = h; }
}

// ----------------------------------------------------------------------------
//
void VWindow::getInternalBorderGeometry(int *left, int *right,
					int *top, int *bottom) const
{
    if (left   != 0) { *left   = 0; }
    if (right  != 0) { *right  = 0; }
    if (top    != 0) { *top    = 0; }
    if (bottom != 0) { *bottom = 0; }
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the preferred size of this VWindow.
// The size returned includes any non-client area surrounding the window.
// Either or both of the given (w, h) pointers may be null.
//
void VWindow::determinePreferredSize(int *w, int *h) const
{
	if (geoFrame != 0) {
		geoFrame->determinePreferredSize(w, h);
	}
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the preferred size of this VWindow, assuming
// that its width and/or height is constrained by the given values.
// The size returned includes any non-client area surrounding the window.
// Either or both of the given (w, h) pointers may be null.
//
void VWindow::determineConstrainedSize(int *w, int *h,
									   int constrained_w,
								       int constrained_h) const
{
	if (geoFrame != 0) {
		geoFrame->determineConstrainedSize(w, h, constrained_w, constrained_h);
	}
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the minimum size of this VWindow.
// The size returned includes any non-client area surrounding the window.
// Either or both of the given (w, h) pointers may be null.
//
void VWindow::determineMinimumSize(int *w, int *h) const
{
	if (geoFrame != 0) {
		geoFrame->determineMinimumSize(w, h);
	}
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the maximum size of this VWindow.
// The size returned includes any non-client area surrounding the window.
// Either or both of the given (w, h) pointers may be null.
//
void VWindow::determineMaximumSize(int *w, int *h) const
{
    getMaximumSize(w, h);
}

// ---------------------------------------------------------------------------
// Constrain the minimum size of this top-level window to its minimum size
// as reported by getMinimumSize().
//
void VWindow::constrainMinimumSize()
{
    int w, h;
    determineMinimumSize(&w, &h);
    constrainedMinimumWidth = w;
    constrainedMinimumHeight = h;
}

// ---------------------------------------------------------------------------
// Constrain the minimum size of this top-level window to the given size.
//
void VWindow::constrainMinimumSize(int w, int h)
{
    constrainedMinimumWidth = w;
    constrainedMinimumHeight = h;
}

// ---------------------------------------------------------------------------
// Constrain the maximum size of this top-level window to its maximum size
// as reported by getMaximumSize().
//
void VWindow::constrainMaximumSize()
{
    int w, h;
    getMaximumSize(&w, &h);
    constrainedMaximumWidth = w;
    constrainedMaximumHeight = h;
}

// ---------------------------------------------------------------------------
// Constrain the maximum size of this top-level window to the given size.
//
void VWindow::constrainMaximumSize(int w, int h)
{
    constrainedMaximumWidth = w;
    constrainedMaximumHeight = h;
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the preferred size of this VWindow.
// The size returned refers only to the client area of the window.
// Either or both of the given (w, h) pointers may be null.
//
// This method is meant to be overridden appropriately by
// derived classes and should not in general be called directly.
//
void VWindow::getPreferredSize(int *w, int *h) const
{
    if (w != 0) { *w = 16; }
    if (h != 0) { *h = 16; }
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the preferred size of this VWindow, assuming
// that its width and/or height is constrained by the given values.
// If this method is implemented, it should return TRUE, otherwise FALSE.
// The size returned refers only to the client area of the window.
// Either or both of the given (w, h) pointers may be null.
//
// This method is meant to be overridden appropriately by
// derived classes and should not in general be called directly.
//
boolean VWindow::getConstrainedSize(int *, int *,
				    int constrained_w,
				    int constrained_h) const
{
    return FALSE;
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the minimum size of this VWindow.
// The size returned refers only to the client area of the window.
// Either or both of the given (w, h) pointers may be null.
//
// This method is meant to be overridden appropriately by
// derived classes and should not in general be called directly.
//
void VWindow::getMinimumSize(int *w, int *h) const
{
    getPreferredSize(w, h);
}

// ----------------------------------------------------------------------------
// Return in (*w, *h) the maximum size of this VWindow.
// The size returned refers only to the client area of the window.
// Either or both of the given (w, h) pointers may be null.
//
// This method is meant to be overridden appropriately by
// derived classes and should not in general be called directly.
//
void VWindow::getMaximumSize(int *w, int *h) const
{
    notifier->getScreenSize(w, h);
}

// ---------------------------------------------------------------------------
//
VGeoManager *VWindow::setGeoManager()
{
	if ((geoFrame == 0) || !geoFrame->isManager()) {
		return setGeoForm();
	}
	else {
		return geoManager;
	}
}

// ---------------------------------------------------------------------------
//
VGeoForm *VWindow::setGeoForm()
{
	if ((geoFrame == 0) || !geoFrame->isFormManager()) {
	    return (VGeoForm *)setGeoManager(new VGeoForm);
	}
	else {
	    return (VGeoForm *)geoManager;
	}
}

// ---------------------------------------------------------------------------
//
VGeoRow *VWindow::setGeoRow()
{
	if ((geoFrame == 0) || !geoFrame->isRowManager()) {
	    return (VGeoRow *)setGeoManager(new VGeoRow);
	}
	else {
	    return (VGeoRow *)geoManager;
	}
}

// ---------------------------------------------------------------------------
//
VGeoRowOrColumn *VWindow::setGeoRowOrColumn()
{
	if ((geoFrame == 0) || !geoFrame->isRowOrColumnManager()) {
	    return (VGeoRowOrColumn *)setGeoManager(new VGeoRowOrColumn);
	}
	else {
	    return (VGeoRowOrColumn *)geoManager;
	}
}

// ---------------------------------------------------------------------------
//
VGeoColumn *VWindow::setGeoColumn()
{
	if ((geoFrame == 0) || !geoFrame->isColumnManager()) {
	    return (VGeoColumn *)setGeoManager(new VGeoColumn);
	}
	else {
	    return (VGeoColumn *)geoManager;
	}
}

// ---------------------------------------------------------------------------
//
VGeoManager *VWindow::setGeoManager(VGeoManager *geomanager)
{
    if (geomanager == 0) {
		return 0;
    }
	if (geomanager->associateWindow(this)) {
	    return geomanager;
	}
	else {
	    return 0;
	}
}

// ----------------------------------------------------------------------------
//
VGeoManager *VWindow::getGeoManager() const
{
    if ((geoFrame != 0) && geoFrame->isManager()) {
	return geoManager;
    }
    else {
	return 0;
    }
}

// ----------------------------------------------------------------------------
//
VGeoForm *VWindow::getGeoForm() const
{
    if ((geoFrame != 0) && geoFrame->isFormManager()) {
		return (VGeoForm *)geoManager;
    }
    else {
		return 0;
    }
}

// ----------------------------------------------------------------------------
//
VGeoRow *VWindow::getGeoRow() const
{
    if ((geoFrame != 0) && geoFrame->isRowManager()) {
	return (VGeoRow *)geoManager;
    }
    else {
	return 0;
    }
}

// ----------------------------------------------------------------------------
//
VGeoColumn *VWindow::getGeoColumn() const
{
    if ((geoFrame != 0) && geoFrame->isColumnManager()) {
	return (VGeoColumn *)geoManager;
    }
    else {
	return 0;
    }
}

// ----------------------------------------------------------------------------
//
VGeoRowOrColumn *VWindow::getGeoRowOrColumn() const
{
    if ((geoFrame != 0) && geoFrame->isRowOrColumnManager()) {
	return (VGeoRowOrColumn *)geoManager;
    }
    else {
	return 0;
    }
}

// ----------------------------------------------------------------------------
//
int VWindow::getGeoIndex() const
{
    return geoFrame == 0 ? 0 : geoFrame->getIndex();
}

// ---------------------------------------------------------------------------
//
void VWindow::setDontFrame(boolean b)
{
	VGeoFrame *geoframe = getGeoFrame();
	if (geoframe != 0) {
		geoframe->setDontFrame(b);
	}
}

// ---------------------------------------------------------------------------
//
boolean VWindow::isDontFrame() const
{
	VGeoFrame *geoframe = getGeoFrame();
	if (geoframe != 0) {
		return geoframe->isDontFrame();
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
//
void VWindow::updateGeometry(int i)
{
	if (geoFrame != 0) {
		geoFrame->update(i);
	}
}

// ---------------------------------------------------------------------------
// set the size/position of the physical (presentation) window
// without changing any logical data
//
void VWindow::setPhysical(const VRectangle& r)
{
	int x, y, w, h;
	((VRectangle&)r).get(CornerDim, &x, &y, &w, &h);
	if (hWnd != 0) {
		setGeometry(x, y, w, h);
	}
}

// ---------------------------------------------------------------------------
VRectangle VWindow::getPhysical() const
{
	int x, y, w, h;
	((VWindow *)this)->getAbs(&x, &y, &w, &h);
	return VRectangle(CornerDim, x, y, w, h);
}

// ---------------------------------------------------------------------------
//
void VWindow::setGeometry(int x, int y, int w, int h)
{
    if (hWnd != 0) {
	if (deferGeometryChangeId != 0) {
	    deferGeometryChangeId =
		DeferWindowPos(deferGeometryChangeId, hWnd, 0,
							   x, y, w, h, SWP_NOACTIVATE | SWP_NOZORDER);
			if (deferGeometryChangeId != 0) {
				return;
			}
	}
		MoveWindow(hWnd, x, y, w, h, TRUE);
    }
}

// ---------------------------------------------------------------------------
//
void VWindow::setGeometryPosition(int x, int y)
{
    if (hWnd != 0) {
	if (deferGeometryChangeId != 0) {
	    deferGeometryChangeId =
				DeferWindowPos(deferGeometryChangeId, hWnd, 0,
							   x, y, 0, 0,
							   SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
			if (deferGeometryChangeId != 0) {
				return;
			}
	}
		SetWindowPos(hWnd, 0, x, y, 0, 0,
					 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    }
}

// ---------------------------------------------------------------------------
//
void VWindow::setGeometryOffset(int xoffset, int yoffset)
{
    if (hWnd != 0) {
	RECT r; POINT p;
	GetWindowRect(hWnd, LPRECT(&r));
	p.x = int(r.left);
	p.y = int(r.top);
	if ((parent != 0) && (parent->hWnd != 0)) {
	    ScreenToClient(parent->hWnd, LPPOINT(&p));
	}
	SetWindowPos(hWnd, 0, p.x + xoffset, p.y + yoffset, 0, 0,
		     SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    }
}

// ---------------------------------------------------------------------------
//
void VWindow::setGeometrySize(int w, int h)
{
    if (hWnd != 0) {
	if (deferGeometryChangeId != 0) {
	    deferGeometryChangeId =
				DeferWindowPos(deferGeometryChangeId, hWnd, 0,
							   0, 0, w, h,
							   SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
			if (deferGeometryChangeId != 0) {
				return;
			}
	}
		SetWindowPos(hWnd, 0, 0, 0, w, h,
					 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
    }
}


// ---------------------------------------------------------------------------
//
void VWindow::beginDeferGeometryChanges()
{
	if (!endingDeferGeometyChanges) {
		endDeferGeometryChanges();
		deferGeometryChangeId = BeginDeferWindowPos(32);
	}
}

// ---------------------------------------------------------------------------
//
void VWindow::endDeferGeometryChanges()
{
	if (deferGeometryChangeId != 0) {
		HDWP defer_geometry_change_id = deferGeometryChangeId;
		deferGeometryChangeId = 0;
		endingDeferGeometyChanges = TRUE;
		EndDeferWindowPos(defer_geometry_change_id);
		endingDeferGeometyChanges = FALSE;
	}
}

// ---------------------------------------------------------------------------
//
void VWindow::flushDeferredGeometryChanges()
{
	endDeferGeometryChanges();
}

// ---------------------------------------------------------------------------
//
void VWindow::attachWin(VWindow *child,
			VAttach::Side side, VAttach::Type type,
			int offset, VWindow *sibling)
{
	VGeoForm::oldAttachWin(this, child, side, type, offset, sibling);
}

// ---------------------------------------------------------------------------
//
void VWindow::setAttach(VAttach::Side side, VAttach::Type type,
			int offset, VWindow *sibling)
{
	VGeoForm::oldSetAttach(this, side, type, offset, sibling);
}

// ---------------------------------------------------------------------------
//
VAttach *VWindow::getAttach() const
{
	return VGeoForm::oldGetAttach(this);
}

// ---------------------------------------------------------------------------
// Return the minimum width and height for this window in 'w' and
// 'h' respectively. Overloaded in sub-classes to return different sizes.
//
void VWindow::getMinSize(short &w, short &h)
{
    w = GetSystemMetrics(SM_CXMIN);
    h = GetSystemMetrics(SM_CYMIN);
}

// ---------------------------------------------------------------------------
// Return the preferred width and height for this window in 'w' and
// 'h' respectively. Overloaded in sub-classes to return different sizes.
//
void VWindow::getPrefSize(short &w, short &h)
{
    w = 10;     //!!!jmd
    h = 10;
}

// ---------------------------------------------------------------------------
// Remove all attachment references to 'win'
//
void VWindow::detachWin(VWindow *win)
{
	VGeoForm::oldDetachWin(this, win);
}

// ---------------------------------------------------------------------------
//  Resize all child windows of this VWindow in proportion to the new
//  width and height: 'neww' and 'newh' respectively.
//  Returns FALSE for bad geometry
//
boolean VWindow::frameChildren(int neww, int newh, boolean test)
{
	if (geoFrame != 0) {
		if (geoFrame->isManager()) {
			if (test) {
				return geoManager->frameChildrenOk(neww, newh);
			}
			else {
				geoManager->resized(neww, newh);
				return TRUE;
			}
		}
		else {
			return TRUE;
		}
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
// VWindow::clipToChildren
// --------------------------------------------------------------------------

boolean VWindow::clipToChildren () const
{
    long s = GetWindowLong(hWnd, GWL_STYLE);

    if ((s & WS_CLIPCHILDREN) != 0) {
	return TRUE;
    }
    else {
	return FALSE;
    }
}

void VWindow::clipToChildren (boolean clip_to_children)
{
    long s = GetWindowLong(hWnd, GWL_STYLE);

    if (clip_to_children) {
	if ((s & WS_CLIPCHILDREN) == 0) {
	    s |= WS_CLIPCHILDREN;
	    SetWindowLong(hWnd, GWL_STYLE, s);
	}
    }
    else if ((s & WS_CLIPCHILDREN) != 0) {
	s &= ~WS_CLIPCHILDREN;
	SetWindowLong(hWnd, GWL_STYLE, s);
    }
}

// --------------------------------------------------------------------------
// VWindow::clipToSiblings
// --------------------------------------------------------------------------

boolean VWindow::clipToSiblings () const
{
    long s = GetWindowLong(hWnd, GWL_STYLE);

    if ((s & WS_CLIPSIBLINGS) != 0) {
	return TRUE;
    }
    else {
	return FALSE;
    }
}

void VWindow::clipToSiblings (boolean clip_to_children)
{
    long s = GetWindowLong(hWnd, GWL_STYLE);

    if (clip_to_children) {
	if ((s & WS_CLIPSIBLINGS) == 0) {
	    s |= WS_CLIPSIBLINGS;
	    SetWindowLong(hWnd, GWL_STYLE, s);
	}
    }
    else if ((s & WS_CLIPSIBLINGS) != 0) {
	s &= ~WS_CLIPSIBLINGS;
	SetWindowLong(hWnd, GWL_STYLE, s);
    }
}

// --------------------------------------------------------------------------
// VWindow::obscured
//
// Return non-zero iff this window is currently visible but is currently
// partially or completely obscured by any other visible windows on
// the screen, or is partially or completely off the edge of the screen,
// or (in the case of a child window) is partially or completely off the
// edge any of its parent windows (ancestors).  Otherwise, return zero.
// In addition, if this window is *completely* obscured, then a negative
// number is returned otherwise if it is only partially obscured, then
// a positive number is returned.
// --------------------------------------------------------------------------

int VWindow::obscured () const
{
    if ((hWnd == 0) || !IsWindowVisible (hWnd))
	return (0);

    HDC     hdc;
    RECT    this_r, clip_r;
    int     clipbox_status;

    if ((hdc = GetWindowDC (hWnd)) == 0) {
	return 0;
    }

    clipbox_status = GetClipBox (hdc, &clip_r);
    ReleaseDC (hWnd, hdc);
    if (clipbox_status == NULLREGION) {
	return -1;
    }
    else if (clipbox_status == COMPLEXREGION) {
	return 1;
    }
    else if (clipbox_status != ERROR) {
	GetWindowRect (hWnd, &this_r);
	if ((clip_r.top == 0) &&
	    (clip_r.left == 0) &&
	    (clip_r.bottom == (this_r.bottom - this_r.top)) &&
	    (clip_r.right == (this_r.right - this_r.left))) {
	    return 0;
	}
	return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------
//
void VWindow::setPalette(HPALETTE hpalette)
{
	VDisplay::setPalette(hpalette);
	if (hpalette == 0) {
		notifier->unregisterPaletteWindow(this);
	}
	else {
		notifier->registerPaletteWindow(this);
	}
}

//----------------------------------------------------------------------------
//
boolean VWindow::paletteChanged(boolean foreground)
{
	return notifier->realizePalette(this, foreground) > 0;
}

// ---------------------------------------------------------------------------
void VWindow::setHelpId(long idno)
{
    helpId = idno;
}

// ---------------------------------------------------------------------------
// Return the window's help idno.
// if 0, return the parent window help idno
//
long VWindow::getHelpId(boolean tryParent) const
{
    if (helpId == 0 && tryParent && parent != 0) {
	return parent->getHelpId(TRUE);
    }

    return helpId;
}

// --------------------------------------------------------------------------
//
boolean VWindow::contextHelp()
{
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VWindow::contextMenu()
{
	return FALSE;
}

//--------------------------------------------------------------------------
//
// Returns true is this VWindow has 3D effects active.
//
boolean VWindow::has3dEffects() const
{
    return style.contains(Style3D) &&
	   (notifier->threeDEffectsAvailable() != V_NO_3D_EFFECTS);

}

//--------------------------------------------------------------------------
//
// Returns true is this VWindow has the specified 3D effect active.
//
boolean VWindow::has3dEffects(enum V_3D_EFFECTS effect) const
{
    return style.contains(Style3D) &&
	   notifier->threeDEffectsAvailable(effect);

}

//----------------------------------------------------------------------------
// -------------------- Font Access

//----------------------------------------------------------------------------
VFont * VWindow::getFont()  const
{
    return winFont;
}

//----------------------------------------------------------------------------
void VWindow::fontSize(int *w, int *h)
{
	VFont *font = getFont();
    if (font != 0) {
	*w = font->width();
	*h = font->height();
    }
    else {
	VDisplay::fontSize(w, h);
    }
}


//----------------------------------------------------------------------------
void VWindow::setFont(VFont *f)
{
    // The interesting stuff happens in VControl or other
    // derived classes
    winFont = f;
}

//----------------------------------------------------------------------------
void VWindow::setFont(VFont *f, boolean propagate_down)
{
	if ((propagate_down) && (children != 0)) {
		DO (*children, VWindow, child)
			child->setFont(f, TRUE);
			setFont(f);
		END
	}
	else {
		setFont(f);
	}
}

// --------------------------------------------------------------------------
//
void VWindow::registerDropSite(VDropSite *drop_site)
{
#ifdef CV_OLE_DRAG_DROP
	if (drop_site == 0) {
		if (dropSite != 0) {
			dropSite->unregisterWindow(this);
		}
	}
	else {
		if (dropSite != 0) {
			dropSite->unregisterWindow(this);
		}
		drop_site->registerWindow(this);
	}
#endif
}

// --------------------------------------------------------------------------
//
void VWindow::registerDragSite(VDragSite *drag_site)
{
#ifdef CV_OLE_DRAG_DROP
	if (drag_site == 0) {
		if (dragSite != 0) {
			dragSite->unregisterWindow(this);
		}
	}
	else {
		dragSite = drag_site;
		dragSite->registerWindow(this);
	}
#endif
}

// ===========================================================================

#ifndef CV_NOARCHIVER

//----------------------------------------------------------------------------
//
void VWindow::setArchiveEnable(boolean b)
{
    archiveEnableFlag = b;
	if (geoFrame != 0) {
		geoFrame->setDontArchive(!b);
	}
}

//----------------------------------------------------------------------------
//
void VWindow::putTo(VArchiver& a)
{
    VWindow::putTo(a, TRUE);
}

//----------------------------------------------------------------------------
//  Archive this VWindow object and, if most derived, its children.
//
//  Type:           What:
//      VString         window name
//      VFrame          window frame
//      short           user specified style
//      boolean         enabled flag
//      boolean         hidden flag
//      boolean         automatic sizing    under attachment flag
//      boolean         automatic framing   under attachment flag
//      int             minimum size width value
//      int             minimum size height value
//
//                      For each of the 4 sides of the window:
//
//      VAttach::Type   attachment type     for a side of the window
//                      (VAttach::UnAttached, if no attachment data)
//
//      int             attachment offset   for a side of the window
//                      (0, if no attachment data)
//
//
void VWindow::putTo(VArchiver& a, boolean most_derived)
{
    extern VClass *VTreeListBoxCls;

    VDisplay::putTo(a);

    a << VWindowArchiveTag;

    //  establish the collection of children which want to be archived
    //  for use by children which need sibling indices under attachment
    //  or by parent objects of derived classes which need child indices

    if (archiveChildren != 0) {
		delete archiveChildren;
    }
    if (children != 0) {
		archiveChildren = new VOrdCollect;
		DO (*children, VWindow, child)
		    if (child->getArchiveEnable()) {
				archiveChildren->add(child);
		    }
		END
    }

    if (winName == 0) {
		static unsigned long k = 0; VString s;
		s.prints("%s_%d", iam()->nameOf(), ++k);
		setName(s);
    }

    a.putObject(winName);                       //  name
    a.putObject(&winFrame);                     //  frame

    if ((parent != 0) && parent->isA(VDialogCls) &&
       (parent->getFont() == getFont())) {
		//
		// If the parent of this VWindow is a dialog,
		// and that dialog has the same font this window has, we
		// assure that the VDialog will set this
		// font when the parent (the dialog) and its children
		// (this font) are retrived from the archive.
		// Therefore the control's font is archived as 0.
		// If this were not done, the VWindow would
		// instance a font just like the parent dialog, which is
		// not desireable because we want to share
		// like fonts, not duplicate them.
		//
		VFontInfo fontinfo(0); // Font info for the Window's VFont
		a.putObject(&fontinfo, TRUE);
	}
	else {
		VFontInfo fontinfo(getFont());
		a.putObject(&fontinfo, TRUE);
		// Font info for the Window's VFont
    }


    a.putObject(&style);                            //  style

    a << isEnabled();                           //  enabled/disabled

    if ((isA(VControlCls)) ||
	(isA(VTreeListBoxCls))
#ifndef CV_NOOLE
	|| (isA(VItemWindowCls)))
#else
	)
#endif
	{
		a << FALSE;
    }
    else {
		a << isHidden();                        //  hidden/visible
    }

    if (VWindowTagRev >= 1) {
		a << isTabStop();                       //  tab stop
    }

    a << pForeground.getRed();                  //  foreground color
    a << pForeground.getGreen();
    a << pForeground.getBlue();
    a << defaultForeground;                     //  use default color?

    VBrushInfo brushinfo(getBackground());      //  background brush
    a.putObject(&brushinfo, TRUE);

	a << long(0xdeadbeaf);
	a.putObject(geoFrame);

#ifndef CV_NOOLE
    // rhw 4/12/96: I needed to change the way components were archived (i.e.
    // archive pointer to component rather than the component itself).  Old
    // code had a chicken-and-egg problem when testing to see if component
    // needed to be archived or restored.  Now, we simply archive or restore
    // what ever is returned by getComponentThis().  If it points to an
    // object, the object get persisted.  If its NULL then that is what is
    // archived and restored which has the correct semantics.
    if (VWindowTagRev >= 6) {
		VComponentWindow *cw = getComponentThis();
		if (cw == 0) {
		    a << (unsigned long)0;
		}
		else {
		    a.putObjectWithCount(getComponentThis());
		}
    }
    else if (VWindowTagRev >= 5) {
		a.putObject(getComponentThis());
    }
    else if (VWindowTagRev >= 3) {
		if (getComponentThis()) {
		    getComponentThis()->putTo(a);
		}
    }
#else
    //
    // No-OLE mode.
    //
    if (VWindowTagRev >= 6) {
		if ((componentWindow != 0) && componentWindowIsRawMemory) {
		    VObjectRawMemory *cw = (VObjectRawMemory *)componentWindow;
		    a << cw->size;
		    a.write(cw->data, cw->size);
		}
		else {
		    a << (unsigned long)0;
		}
    }
#endif

    if (most_derived) {
		VWindow::putChildrenTo(a);	//  children of this window
    }
}

//----------------------------------------------------------------------------
//
void VWindow::getFrom(VArchiver& a)
{
    archiveChildrenNotRealized = 1;

    static int CV_NEARDATA level = 0; level++;

    VDisplay::getFrom(a);

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VWindowArchiveTag)) {
		a.abort(archiveTagAbortCheck(tag, VWindowArchiveTag));
		level--;
		return;
    }

    VString name;                               //  name
    a.getObject(&name);
    setName(name);

    a.getObject(&winFrame);                     //  frame

    if (getArchiveTagRev(tag) >= 3) {
	    // pick up the font information
		VFontInfo finfo;                        // font
		a.getObject(&finfo);
		// if pointSize ==0, fontinfo was unitialized
		if (finfo.pointSize() != 0){
		    if (finfo.stockObject()) {
			    // create the STOCKOBJECT font
			    winFont = new VFont(finfo.stockObject(), 1);
		    }
			else {
				// create the regular font
				winFont = new VFont(finfo);
		    }
		}
		else {
			winFont = 0;
		}
    }

	pFontFromArchive = winFont;
    // we remember what we erad out of the archive so we can delete it.

    if (getArchiveTagRev(tag) >= 2) {
		VWinStyle oldStyle;
		a.getObject(&oldStyle);
		// add the persistent styles to the archived styles
		style += oldStyle;
    }
    else {
		short oldStyle;
		a >> oldStyle;
		// convert and add the old style bits to VWinStyle Objects
		VImplicitStyle::convertOldToNew(oldStyle, *this);
	}

	boolean b;
    a >> b; enabledFlag = b; //  enabled/disabled
    a >> b; hiddenFlag = b;  //  hidden/visible

	if (getArchiveTagRev(tag) > 0) {
		boolean b;
		a >> b; bTabStop = b;
	}

    short rgb[3];                               //  foreground color
    a >> rgb[0];
    a >> rgb[1];
    a >> rgb[2];
    a >> b; defaultForeground = b;              //  use default color?
    pForeground = VColor(rgb[0], rgb[1], rgb[2]);

    VBrushInfo brushinfo;                       //  background brush
    a.getObject(&brushinfo);

    // realize the brushinfo
	if (!brushinfo.isDefault()) {
		pBackground = new VBrush(brushinfo.foreground());
		backgroundBrushIsMine = 1;
	}
	else {
		pBackground = 0;
	}


    if (getArchiveTagRev(tag) >= 4) {
		//
		// Here, this archive could contain new or old geometry.
		//
		long use_geo_frames;
		a >> use_geo_frames;
		if (use_geo_frames) {
		    //
		    // Here, the archive contains new geometry.
		    //
		    if (!usingGeoFrames) {
				//
				// Error; we are old geometry mode, but we are
				// apparently reading an archive with new geometry.
				//
				a.abort(VArchiver::ClassSpecific,
					"New geometry in archive but old geometry active.\n"
					"Use VWindow::useGeoFrames(TRUE).");
		    }
		    else {
				//
				// Here, the archive contains new geometry,
				// and we are in new geometry mode; good.
				//
				VGeoFrame *geoframe = (VGeoFrame *)a.getObject(0, this);
				if (geoframe != 0) {
				    geoframe->associateWindow(this);
				}
		    }
		    convertOldToGeoFrames = FALSE;
		}
		else {
		    //
		    // Here, the archive contains old geometry.
		    // If we are in new geometry mode (usingGeoFrames) we
		    // will need to convert to the new geometry when we are done.
		    //
			boolean autoSize;
			boolean autoFrame;
		    a >> autoSize;                      //  geometry
	    	a >> autoFrame;
	    	int minSizePlaceHolder = 0;         //  minimum size spec
		    a >> minSizePlaceHolder;
		    a >> minSizePlaceHolder;
		    getAttachFrom(a);
		    convertOldToGeoFrames = usingGeoFrames;
		}
    }
    else {
		//
		// Here, this archive could only contain old geometry.
		// If we are in new geometry mode (usingGeoFrames) we
		// will need to convert to the new geomtry when we are done.
		//
		boolean autoSize;
		boolean autoFrame;
		a >> autoSize;				//  geometry
		a >> autoFrame;
		int minSizePlaceHolder = 0;	//  minimum size spec
		a >> minSizePlaceHolder;
		a >> minSizePlaceHolder;
		getAttachFrom(a);
		convertOldToGeoFrames = usingGeoFrames;
	}

	if (getArchiveTagRev(tag) == 5) {
		return;
	}

#ifndef CV_NOOLE
    // rhw 4/12/96: I needed to change the way components were archived (i.e.
    // archive pointer to component rather than the component itself).  Old
    // code had a chicken-and-egg problem when testing to see if component
    // needed to be archived or restored.  Now, we simply archive or restore
    // what ever is returned by getComponentThis().  If it points to an
    // object, the object get persisted.  If its NULL then that is what is
    // archived and restored which has the correct semantics.
	currentWindowTag=tag;
	if (getArchiveTagRev(tag) >= 6) {
		if (getComponentThis() != 0) {
		    a.getObjectWithCount(getComponentThis(), this);
		}
		else {
		    VComponentWindow *cw =
		    (VComponentWindow *)a.getObjectWithCount(0, this);
		    setComponentThis(cw);
		    // these "default constructed" VComponentWindows"
		    // seem to need an addref to prevent an assert in ~CRUnknown()
		    // upon deletion
		    if (cw){
			cw->AddRef();
		    }
		}
    }
    else if (getArchiveTagRev(tag) >= 5) {
		if (getComponentThis()) {
		    a.getObject(getComponentThis(), this);
		}
		else {
		    setComponentThis((VComponentWindow*) a.getObject(0, this));
		}
    }
    else if (getArchiveTagRev(tag) >= 3) {
		if (getComponentThis()) {
		    getComponentThis()->getFrom(a);
		}
    }
#else
    if (getArchiveTagRev(tag) >= 6) {
		char *data; unsigned long size;
		data = (char *)a.getObjectMemoryWithCount(&size);
		if ((data != 0) && (size != 0)) {
		    componentWindow =
			(VComponentWindow *)new VObjectRawMemory(data, size);
		    componentWindowIsRawMemory = 1;
		}
    }
#endif

    if ((--level == 0) && usingGeoFrames) {
		//
		// ### dgm, 12/4/95: This is not correct!
		//
		// VGeoFrame::getFromDone(this);
		//
    }
}

//----------------------------------------------------------------------------
//
void VWindow::getFrom(VArchiver& a, VObject *parent, boolean do_realize)
{
    VWindow::getFrom(a);                         //  retrieve object data
    if (do_realize) {
		VWindow::realize((VWindow *)parent);     //  build presentation
    }
    if (parent != 0) {
		VWindow::getChildrenFrom(a, do_realize); //  retrieve children
    }
}

//----------------------------------------------------------------------------
//
void VWindow::getAttachFrom(VArchiver& a)
{
    char c;
    a >> c;
    if (c != 'A') {
		return; // no VAttach data
    }

	if (oldAttachArchiveData == 0) {
		oldAttachArchiveData = new VOldAttachData;
	}

    int attach_type;
    int attach_offset;

    for (int side = 0; side < 4; side++) {
		a >> attach_type;
		a >> attach_offset;
		a >> oldAttachArchiveData->attachSibling[side];
		oldAttachArchiveData->attach.setAttach
			(VAttach::Side(side), VAttach::Type(attach_type), attach_offset);
	}
}

//----------------------------------------------------------------------------
//  Archive the number of children.
//  If there are children, then archive each one of them.
//
void VWindow::putChildrenTo(VArchiver& a)
{
    if ((archiveChildren == 0) || (archiveChildren->count() <= 0)) {
		a << 0;             //  no current child with focus
		a << 0;             //  0 children
    }
    else {
		//  archive the index of the child with focus
		if (curChildFocus != 0) {
		    a << (archiveChildrenOf()->indexOfSame(curChildFocus) + 1);
		}
		else {
		    a << 0;
		}
		int nchildren = int(archiveChildren->count());
		a << int(archiveChildren->count());
		//
		// Archiver the Z-order of the children; this is essentially an array
		// of integers arranged in top-to-bottom stacking-order; the integers
		// are indices into the "children" VOrdCollect object (which will
		// be archived out after this) which is in front-to-back tab-order.
		// The Z-order is not maintained within Views, so we need to get
		// it directly from MS-Windows.
		//
		int     nchildren_in_zorder = 0;
		HWND    child_hwnd = 0;
		if ((hWnd == 0) || ((child_hwnd = GetWindow(hWnd, GW_CHILD)) == 0)) {
		    //
		    // This apparent nonesense is just in case the children aren't
		    // really children of this window, they're just fake, i.e. they
		    // have (temporarily) been reparented to a different window than
		    // their real parent.  This is in fact done by the dialog-editor.
		    //
		    DO (children, VWindow, child)
			if ((child != 0) && child->isA(VWindowCls) &&
			    (child->hWnd != 0)) {
			    child_hwnd = GetWindow(child->hWnd, GW_HWNDFIRST);
			    if (child_hwnd != 0) {
					break;
			    }
			}
		    END
		}
		for ( ; child_hwnd != 0 ;
			child_hwnd = GetWindow(child_hwnd, GW_HWNDNEXT)) {
		    VObject *child = notifier->getThis(child_hwnd);
		    if ((child == 0) || !child->isA(VWindowCls)) {
				continue;
		    }
		    int child_index = int(children->indexOfSame(child));
		    if ((child_index < 0) || (child_index >= nchildren)) {
				continue;
		    }
		    a << child_index;
		    if (++nchildren_in_zorder == nchildren) {
				//
				// If for some reason the children Z-order list
				// wants to be longer than the actual number of
				// children, then cut it short.
				//
				break;
		    }
		}
		if (nchildren_in_zorder < nchildren) {
		    //
		    // If for some reason the children Z-order list was
		    // shorter than the actual number of children, then
		    // fill out the extra space with -1's.
		    //
		    for (int i = nchildren - nchildren_in_zorder ; i > 0 ; i--) {
				a << -1;
		    }
		}
	
		// archive the children which want to be archived
	
		DO (*archiveChildren, VWindow, child)
		    a.putObject(child);
		END

		delete archiveChildren;
		archiveChildren = 0;
    }
}

//----------------------------------------------------------------------------
void VWindow::getChildrenFrom(VArchiver& a, boolean do_realize)
{
    int  iFocus;
    a >> iFocus;        //  child with focus

    int  nChildren;
    a >> nChildren;     //  number of children

    // Get the Z-order of the children; this is essentially an array of
    // integers arranged in top-to-bottom stacking-order; the integers
    // are indices into the "children" VOrdCollect object (which will
    // be read in after this) which is in front-to-back tab-order.

    if (nChildren > 0) {
		childrenZorder = new int[nChildren];
		if (childrenZorder != 0) {
		    int child_index;
		    for (int i = 0 ; i < nChildren ; i++) {
				a >> child_index;
				childrenZorder[i] = child_index;
		    }
		}
    }

    // Retrieve the children; do not realize them at first;
    // this will be done afterwards (if do_realize) in another
    // loop (in realizeChildren()) so that they can be created
    // in the proper Z-order.

    while (nChildren-- > 0) {
		VWindow *child = (VWindow *)(a.getObject(0, this, FALSE));
		if (child != 0) {
		    if ((child->geoFrame != 0) &&
				(child->geoFrame->getParent() != 0) &&
				(child->geoFrame->getParent() != geoManager)) {
				boolean save_using_geo_frames = usingGeoFrames;
				usingGeoFrames = FALSE;
				this->addChild(child);
				usingGeoFrames = save_using_geo_frames;
		    }
		    else {
				this->addChild(child);
		    }
		}
		else {
		    a.abort(VArchiver::ClassSpecific,
			    "VWindow::getFrom():  Unable to retrieve child.");
		}
    }

    if ((children == 0) || (children->count() <= 0)) {
		return;
    }
    if (nChildren > children->count()) {
		nChildren = int(children->count());
    }

    // If desired, realize the children, in the proper stacking-order.

    if (do_realize) {
		realizeChildren();
    }

    if (iFocus > 0) {
		curChildFocus = (VWindow *)(childrenOf()->idAt(iFocus - 1));
    }

    // Set attachments

    if (!usingGeoFrames) {
		if (children == 0) {
		    return;
		}
		VAttach *attachp;
		VWindow *sibling;
		boolean prioritize = FALSE;
		DO (*children, VWindow, child)
			if (child->oldAttachArchiveData == 0) {
				continue;
			}
			attachp = &child->oldAttachArchiveData->attach;
		    integer iSib;
		    for (int side = 0; side < 4; side++) {
				prioritize = (attachp->getAttach(VAttach::Side(side)) !=
							  VAttach::UnAttached) ? TRUE : prioritize;
				if ((iSib = child->oldAttachArchiveData->
								   attachSibling[side]) > 0) {
					sibling = (VWindow *)children->idAt(iSib - 1);
					attachWin(child, VAttach::Side(side),
							  attachp->getAttach(VAttach::Side(side)),
							  attachp->getOffset(VAttach::Side(side)),
							  sibling);
				}
			}
		END
		this->frameChildren();
	}
	else if (convertOldToGeoFrames) {
		convertOldToGeoFrames = FALSE;
		VGeoForm::oldToNewArchiveConversion(this);
		frameChildren();
	}
}

//----------------------------------------------------------------------------
void VWindow::realize(VWindow *pwin)
{
    boolean save_using_geo_frames = usingGeoFrames;

    if (archiveChildrenNotRealized &&
		(geoFrame != 0) && (geoFrame->getParent() != 0) &&
		(geoFrame->getParent() != parent->geoManager)) {
		//
		// This hack is so that the addChild() which will happen as a
		// result of the operator() call below does not move this VWindow
		// from its VWindow-less VGeoManager parent, which this VWindow
		// apparently has, to the VGeoManager of its VWindow parent.
		//
		usingGeoFrames = FALSE;
    }

    if (pwin != 0) {
		(*this)(winFrame, pwin, style, winName == 0 ? 0 : winName->gets());
    }
    else if (this->isA(VAppViewCls)) {
		(*this)(winFrame, 0, style, winName == 0 ? 0 : winName->gets());
    }

    usingGeoFrames = save_using_geo_frames;

    enableBool(enabledFlag);
    showBool(!hiddenFlag, FALSE);

    realizeChildren();

    setFont(winFont);
}

//----------------------------------------------------------------------------
void VWindow::realizeChildren(VWindow *reparent,
						      VObject *upon_realize_callback_client,
						      method upon_realize_callback_method_method,
						      void *upon_realize_callback_data)
{
    if (this->children == 0) {
		archiveChildrenNotRealized = 0;
		return;
    }

    int nchildren = int(this->children->count());

    if (nchildren <= 0) {
		archiveChildrenNotRealized = 0;
		return;
    }

    boolean  do_reparenting;
    VWindow *parent_of_children;
    void    (VObject::*upon_realize_callback_method)(VWindow *, void *);

    if (reparent == 0) {
		parent_of_children = this;
		do_reparenting = FALSE;
    }
    else if (reparent == this) {
		parent_of_children = this;
		do_reparenting = FALSE;
    }
    else {
		parent_of_children = reparent;
		upon_realize_callback_method =
		    (void (VObject::*)(VWindow *, void *))
			upon_realize_callback_method_method;
		do_reparenting = TRUE;
    }

	if (do_reparenting) {
		if ((geoFrame != 0) && geoFrame->isManager()) {
			VGeoManager *new_manager = parent_of_children->setGeoManager();
			if (new_manager != 0) {
				geoManager->moveChildren(new_manager, TRUE);
			}
		}
	}

    if (childrenZorder != 0) {
		for (int i = 0 ; i < nchildren ; i++) {
		    int child_index = childrenZorder[i];
		    if ((child_index < 0) || (child_index >= nchildren)) {
				continue;
		    }
		    VWindow *child = (VWindow *)(this->children->idAt(child_index));
		    if ((child == 0) || !child->isA(VWindowCls)) {
				continue;
		    }
		    if (do_reparenting) {
				child->setParent(parent_of_children);
				child->realize(parent_of_children);
				if (upon_realize_callback_client != 0) {
				    (upon_realize_callback_client->*
				     upon_realize_callback_method)
					(child, upon_realize_callback_data);
				}
		    }
		    else {
				child->realize(parent_of_children);
		    }
		}
		delete childrenZorder;
		childrenZorder = 0;
		if (do_reparenting) {
		    delete this->children;
		    this->children = 0;
		    if (!usingGeoFrames) {
				// jmd 04/10/94 force geometry recalc...
		    }
			else if (parent_of_children->geoFrame != 0) {
				parent_of_children->geoFrame->update();
			}
		}
    }
    else {
		DO (*this->children, VWindow, child)
		    child->realize(this);
		END
		if (do_reparenting) {
		    parent_of_children->children = this->children;
		    this->children = 0;
		    if (!usingGeoFrames) {
				// jmd 04/10/94 force geometry recalc...
		    }
			else if (parent_of_children->geoFrame != 0) {
				parent_of_children->geoFrame->update();
			}
		}
    }

    // See to it that the font that belonged to the original
    // parent is transfered to the new parent.
    // We do this here (in RealizeChildren)
    // because setFont for a VDialog requires
    // that the children be already realized.

    parent_of_children->setFont(getFont());

    archiveChildrenNotRealized = 0;
}

// --------------------------------------------------------------------------
// Load any resources needed by this window or its children
//
boolean VWindow::loadResources(VRscArchiver& a)
{
    if (children != 0) {
		DO (children, VWindow, child)
		    if (!child->loadResources(a)) {
				return FALSE;
		    }
		END
    }
    return TRUE;
}

// --------------------------------------------------------------------------
// Replace 'original' with 'replacement'
// Both are assumed to be children of this window
// Assumes that replacement has no attachments
// Deletes 'original'
//
void VWindow::replaceChild(VWindow *original, VWindow *replacement)
{
    if (children == 0 || original == 0 || replacement == 0) {
		return;
    }

    // swap Z ordering in future version

	HWND original_hwnd = original->hwnd();
	HWND replacement_hwnd = replacement->hwnd();
	if ((original_hwnd != 0) && (replacement_hwnd != 0)) {
		SetWindowPos(replacement_hwnd, original_hwnd, 0, 0, 0, 0,
					 SWP_NOACTIVATE | SWP_NOMOVE |
					 SWP_NOSIZE | SWP_NOREDRAW);
	}

    // swap positions in the child list
    int oi = int(children->indexOf(original));
    int ri = int(children->indexOf(replacement));
    children->replaceAt(oi, replacement);
    children->replaceAt(ri, original);

    // set position of replacement using original
    VFrame frame;
    original->getFrame(frame);
    replacement->move(frame);

    // set border, name, and other data
    replacement->setBorder(original->hasBorder());

    replacement->enableBool(original->isEnabled());

    if (original->getName()) {
		replacement->setName(*original->getName());
    }

    // replacement->setBackground(original->getBackground()); (add this later)

#if 0
    if (!usingGeoFrames) {
		// get attachment data from original
		if (original->getAttach() != 0) {
		    replacement->attach = original->getAttach();
		    // replace references to original
		    VWindow      *child;
		    int           nchildren = int(children->count());
		    int           offset;
		    register int  i, j;
		    VAttach::Type type;
		    for (i = 0; i < nchildren; i++) {
				if ((child = (VWindow *)children->idAt(i)) != 0 &&
				     child->attach != 0) {
				    for (j = 0; j < 4; j++) {
						type = child->attach->getAttach(VAttach::Side(j));
						offset = child->attach->getOffset(VAttach::Side(j));
						if ((type == VAttach::Sibling ||
						     type == VAttach::SiblingSame) &&
						     child->attach->getSibling
							 	(VAttach::Side(j)) == original) {
						     child->attach->setAttach
							 	(VAttach::Side(j), type, offset, replacement);
						}
				    }
				}
		    }
		    original->attach = 0;
		}
		delete orginal;
		return;
    }
#endif

	if ((geoFrame != 0) && geoFrame->isManager()) {
	    geoManager->replaceChild
			(original->geoFrame, replacement->geoFrame);
	}
	delete original->geoFrame;
	original->geoFrame = 0;
    delete original;
}


#endif // !CV_NOARCHIVER

//--------------------------------
VComponentWindow* VWindow::findComponentWindow(const VString& name, VClass *cls)
{
#ifndef CV_NOOLE
 VWindow* pWin = findWindow(name,VItemWindowCls);
 if (pWin && pWin->getComponentThis()) {
    if (pWin->getComponentThis()->isA(cls)) {
      return pWin->getComponentThis();
    }
 }
#endif

 return 0;
}

#ifndef CV_NOVECTOREVENTS
void VWindow::setEventVector(VWindow::Event event, VObject *o, method m)
{
	if (theEventManager == 0) {
		theEventManager = new VWindowEventManager;
	((VWindowEventManager *)theEventManager)->clientDictionary = 0;
	}
    if (((VWindowEventManager *)theEventManager)->clientDictionary == 0) {
	((VWindowEventManager *)theEventManager)->clientDictionary =
			new VDictionary(VIntAssocCls);
    }
    ((VWindowEventManager *)theEventManager)->eventDictionary[event] = m;
    ((VWindowEventManager *)theEventManager)->clientDictionary->
		removeKey((VObject *)(long)event);
    ((VWindowEventManager *)theEventManager)->clientDictionary->
		atKeyPut((VObject *)(long)event, o);
}

//---------------------------------------------------
boolean VWindow::vectorEvent(int event)
{
    if ((theEventManager == 0) ||
		(((VWindowEventManager *)theEventManager)->clientDictionary == 0) ||
		 !((VWindowEventManager *)theEventManager)->clientDictionary->
			includesKey((VObject*)event)) {
		return FALSE;
    }
    VObject *client = ((VWindowEventManager *)theEventManager)->
					  clientDictionary->getValueAtKey((VObject *)event);
    method mthd = ((VWindowEventManager *)theEventManager)->
					eventDictionary[event];
    if ((mthd == 0) || (client == 0)) {
	return FALSE;
    }
    return client->perform(mthd, long(this));
}
#endif

