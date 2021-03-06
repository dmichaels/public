// $Header:   Q:/views/win/vcs/window.h_v   1.64   22 Nov 1996 19:10:54   patrickw  $

//  window.h
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
//  07/17/92 jmd    preened
//  09/09/92 jmd    changed 'all' arg from boolean to int to
//                  fix type promotion bug
//  09/17/92 jmd    added names
//  04/09/93 pat    changed id to VObject *
//  04/27/93 pat    preened
//  04/27/93 pat    added VWindow() and operator() that take names
//  04/27/93 pat    added getFrom(), putTo(), realize(), findWindow();
//                  added reference to VArchiver
//  05/07/93 pat    made setName() private
//  05/20/93 jmd    added geometry management
//  06/15/93 pat    added support for archiving geometry management;
//                  added enabledFlag, hiddenFlag, and attachSibling for
//                  use during retrieval from archive
//  06/28/93 dgm    changed getWindowRect() from protected to public.
//                  added isBeingDeleted() and beingDeleted.
//                  added clipToChildren() and clipToSiblings().
//                  added obscured(), enablePostPaint(), postPaint(), and
//                  VPostPaint declaration (see postpain.h for definition).
//  06/30/93 dgm    added setFrameRatios().
//  07/15/93 pat    added getArchiveEnable(), setArchiveEnable(), and
//                  archiveEnableFlag, archiveChildren.
//  07/19/93 jmd    added frameChildren test mode, detachWin
//  07/19/93 jmd    removed free(), made VFrame const
//  07/20/93 jmd    changed rgbColor to VColor
//  07/21/93 pat    added forward declaration of VControl
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    added final default boolean argument to getChildrenFrom().
//  07/27/93 pat    changed findWindow() name arg to const VString&
//  07/28/93 dgm    made setName() public.
//  07/31/93 jmd    made count, indexOf, hash, etc. const
//  08/10/93 dgm    befriended VbAttachDialog (temporary).
//  08/11/93 dgm    added getAttach() and befriended DesignerWindow (temporary).
//  08/16/93 jmd    made name const VString, removed setName(VString *)
//  08/18/93 dgm    added setFrame() and setFrameUnits(); removed setRatioX...
//  08/20/93 dgm    added hasBorder() and setBorder(boolean).
//  08/23/93 dgm    moved setFrameUnits() to window.cpp.
//  08/23/93 pat    removed class from friend declarations; removed VWindow
//                  friend; added incomplete declarations for VComboBox.
//  08/23/93 pat    added paletteChanged() and updateColorCount members.
//  09/23/93 pat    added protected defaultForeground member for archival
//  10/05/93 dgm    added bringToTop(VWindow *).
//  10/06/93 jmd    added replaceChild()
//  10/06/93 dgm    moved VView::model and VView::setModel() here,
//                  made setModel() virtual, and added getModel(). (jmd).
//  10/08/93 jmd    made get getPhysical not inline
//  10/12/93 jmd    preening, added saveDialog prototype
//  01/04/94 jmd    added help routines
//  02/03/94 dgm    added virtual keyUp().
//  02/26/94 jmd    added private hInstance member for VNotifier use
//  03/08/94 pat    STRICT: ifdef'd WNDPROC, FARPROC differences
//  03/14/94 pat    CV_WIN32: changed type of user() message param to UINT
//  08/09/94 dgm    Befriended VMdiAppView (uStyle in VMdiAppView archiving).
//  08/15/94 dgm    Removed old postpaint support.
//  08/12/94 dss    Ctl3d.
//  08/17/94 dss    Tab Stops.
//  11/03/94 dss    Added escapeHit().
//  03/16/95 dss    added destroyed().
//  03/27/95 jld    removed #ifdef ARCHIVE_ZORDER, always on
//  06/08/95 jld    changed helpId from int to long
//  06/08/95 jld    added accelKeysOff()/On() member functions
//  05/30/95 jld    added shedFocus() member function
//  08/23/95 dss    changed hasCtl3dEffects() to has3dEffects() to account
//                  for Windows 95 3d effects.
//  09/01/95 dss    VStyle changes.
//  09/28/95 dgm    Added setWindowRedrawUponResize(),
//                  isWindowRedrawUponResize(),
//                  setAutoErased(), and isAutoErased().
//  09/28/95 dgm    Moved V_3D_EFFECTS declaration from notifier.h to here..h.
//  09/29/95 dgm    VGeoFrame changes.
//  10/02/95 dgm    More VGeoFrame changes.
//  10/04/95 evc    Added $HEADER$
//  10/12/95 dss    Added default style functionality.
//  10/18/95 dgm    Added copy(VWindow *new_parent)
//                  and copyNonArchiveData(const VWindow *) --> protected.
//                  Added miscellaneous geometry management related functions.
//  10/19/95 dgm    Added systemColorChanged() & systemFontChanged().
//  10/19/95 dgm    Unvirtualized constrain{Minimum,Maximum}Size().
//  10/20/95 dgm    Added reparent().
//  11/01/95 dgm    Added getClientWindow(); virtual-ized getParent().
//                  Removed setGeoFrame(); added set/getGeoManager(),
//                  setGeoForm(), setGeoRow(), and setGeoColumn().
//                  Added childParented() and childReparented() virtual
//                  methods for dynamic reparenting notification.
//                  Added dockableWindowOwner; befriended VDockableWindow.
//  11/02/95 dgm    Changed getNonClientAreaSize() to getNonClientArea().
//  11/10/95 dgm    Removed getNonClientArea(); added getSystemBorderSize().
//                  Changed getClientWindow() to getClientAreaWindow().
//                  Added getInternalBorderGeometry().
//  11/13/95 dgm    Added setDontFrame() and isDontFrame().
//  11/16/95 dgm    Added insertChild(), childAdded(), and childRemoved().
//  11/20/95 dgm    Added getGeoIndex().
//  11/28/95 dgm    Added systemMetricsChanged().
//  11/30/95 dgm    Added updateGeometry().  Move setFrame() to window.cpp.
//  12/05/95 dgm    Made updateGeometry() virtual.
//  12/06/95 dgm    Moved setArchiveEnable() to window.cpp.
//  12/07/95 dgm    Moved setArchiveEnable() to window.cpp.
//  12/12/95 dgm    Changed changeGeometryXXX() to setGeometryXXX().
//  01/09/96 dss    VComponent name change.
//  01/29/96 dgm    Changed isTopLevel() to be a const function.
//  02/01/96 evc    added const to "getDefaultForeground() const"
//  04/04/96 dgm    Added centerWindow().
//  04/19/96 glm    removed v1.1 code/preened
//  04/25/96 dgm    Added minor archiveChildrenNotRealized flag.
//  08/06/96 pkt    Added beingDestroyed and isBeingDestroyed.
//  09/11/96 dgm    Added setSystemFont bit.
//  09/17/96 dgm    Added setFont(VFont *, boolean).
//  09/18/96 dgm    Fixed memory leak -- background brush from archive.
//  10/03/96 pkt    Reverted inadvertent change of protected members to public.
//  11/15/96 mlw    Added additional Archive revision level to accomodate new
//                  archiving of VComponent* from Sanjay Chopra.
//  02/07/97 dgm    Added LostFocus, lostFocus(), mmouseX(), mouseWheel(),
//					ContextHelp, ContextMenu, contextHelp(), and contextMenu().
//  08/01/97 dgm    Added setTabIndex(), swapTabIndices(), and getTabIndex()
//					for SCR 5809.
//  12/06/96 dgm    Added mmouseXXX().
//  01/10/97 dgm    Added mouseWheel().
//  01/21/97 dgm    Pruned.
// ----------------------------------------------------------------------------

#ifndef VWINDOW_H
#define VWINDOW_H

// ----------------------------------------------------------------------------
//
#include "attach.h"
#include "display.h"
#include "frame.h"
#include "mouscrsr.h"
#include "ordcllct.h"
#include "wininfo.h"
#include "implstyl.h"
#include "styles.h"
#include "winstyle.h"
#include "font.h"
#include "fontinfo.h"

// ----------------------------------------------------------------------------
//
#define getArea getClientRel	// 1.1 compatibility

// ----------------------------------------------------------------------------
//
extern long currentWindowTag;	// archive sync for Component

// ----------------------------------------------------------------------------
//
CLASS VAppView;
CLASS VBrush;
CLASS VComponentWindow;
CLASS VComboBox;
CLASS VControl;
CLASS VDialog;
CLASS VDockableWindow;
CLASS VDragSite;
CLASS VDropSite;
CLASS VEvent;
CLASS VFont;
CLASS VGeoColumn;
CLASS VGeoForm;
CLASS VGeoFrame;
CLASS VGeoManager;
CLASS VGeoRow;
CLASS VGeoRowOrColumn;
CLASS VMdiAppView;
CLASS VNotifier;
CLASS VStatusBar;
CLASS VString;
CLASS VView;

CLASS DesignerView;   // obsolete
CLASS DesignerWindow; // obsolete
CLASS VbAttachDialog; // obsolete

// ----------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER
CLASS VArchiver;
CLASS VRscArchiver;
extern boolean saveDialog(VDialog *, const VString&, VRscArchiver *);
#endif

// ----------------------------------------------------------------------------
//
enum V_3D_EFFECTS {	V_NO_3D_EFFECTS,	// Flat
					V_CTL3D_EFFECTS,    // CTL3D.DLL
					V_WIN3D_EFFECTS		// Native Windows 3D
};

enum WinType {		WinView,
					WinControl,
					WinMdiApp,
					WinMdiClient,
					WinMdiChild,
					WinWindow,
					WinPseudo
};

#ifndef CV_NOVECTOREVENTS
#define WinMouseDbl     VWindow::MouseDbl
#define WinMouseDn      VWindow::MouseDn
#define WinMouseEnter   VWindow::MouseEnter
#define WinMouseExit    VWindow::MouseExit
#define WinMouseMv      VWindow::MouseMv
#define WinMouseUp      VWindow::MouseUp
#define WinRMouseDbl    VWindow::RMouseDbl
#define WinRMouseDn     VWindow::RMouseDn
#define WinRMouseUp     VWindow::RMouseUp
#define WinClose        VWindow::Close
#define WinQuit         VWindow::Quit
#define WinMoved        VWindow::Moved
#define WinResized      VWindow::Resized
#define WinLoseFocus    VWindow::ClearFocus
#define WinTakeFocus    VWindow::GivenFocus
#endif

// ----------------------------------------------------------------------------
// The VWindow class is an abstract super class that defines attributes common
// to all windows: sizing, moving, event protocols, creating, destroying,
// managing of children, etc.  A VWindow object is usually bound to a
// presentation space which is the visual representation of the VWindow
// object in the underlying GUI system.

CLASS VWindow : public VDisplay {

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Public Interface
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:

	// Event Vectoring

#	ifndef CV_NOVECTOREVENTS
		enum Event {	MouseDn,
						MouseUp,
						MouseDbl,
						MouseMv,
						MouseEnter,
						MouseExit,
						RMouseDn,
						RMouseUp,
						RMouseDbl,
						MMouseDn,
						MMouseUp,
						MMouseDbl,
						Key,
						KeyUp,
						Close,
						Quit,
						Moved,
						Resized,
						GivenFocus,
						ClearFocus,
						LosingFocus,
						LostFocus,
						Paint,
						Erased,
						ContextHelp,
						ContextMenu,
						_EventCount_ };
#	endif

	// Construction, Instantiation, Identity, Destruction, ...

						VWindow();
						VWindow(HWND);
						VWindow(const VFrame&, VWindow *,
								const VStyle& = StyleDefault);
						VWindow(const VString&, const VFrame&, VWindow *,
								const VStyle& = StyleDefault);
					   ~VWindow();
	VClass			   *iam();

	void				operator()(const VFrame&, VWindow *);
	void				operator()(const VFrame&, VWindow *, const VStyle&);
	void				operator()(const VFrame&, VWindow *, const VStyle&,
								   const VString&);

	void				clean();

	// Name

	VString			   *getName();
	void				setName(const VString&);

	// Styles

    static VStyle&		getClassDefaultStyle();
    virtual VStyle&		getDefaultStyle() const;
    VWinStyle&			getStyle();

	// Drawing Text

	void				wrtText(VString *, int, int);
	void				wrtText(const char *, int, int);

    // Mouse

	virtual boolean		mouseDbl(int, int);
	virtual boolean		mouseDn(int, int);
	virtual boolean		mouseEnter(int);
	virtual boolean		mouseExit(int);
	virtual boolean		mouseMv(int, int y, int);
	virtual boolean		mouseUp(int, int);
	virtual boolean		rmouseDbl(int, int);
	virtual boolean		rmouseDn(int, int);
	virtual boolean		rmouseUp(int, int);
	virtual boolean		mmouseDn(int, int);
	virtual boolean		mmouseUp(int, int);
	virtual boolean		mmouseDbl(int, int);
	virtual boolean		mouseWheel(int delta, int x, int y, int flags);

	// Keyboard

	virtual boolean		key(int virtual_key, char ascii_key);
	virtual boolean		keyUp(int, char);
	virtual boolean		enterHit();
	virtual boolean		escapeHit();
	boolean				propagateKey(int virtual_key, char ascii_key);

	// Keyboard/Focus

	virtual boolean		takeFocus();
	virtual boolean		givenFocus();
	virtual boolean		clearFocus();
	virtual boolean		losingFocus(VWindow * = 0);
	virtual boolean		lostFocus();

	virtual boolean		nextFocus();
	virtual boolean		prevFocus();
	virtual boolean		nextChildFocus(VWindow *);
	virtual boolean		prevChildFocus(VWindow *);
	virtual boolean		controlNextFocus();
	virtual boolean		controlPrevFocus();
	virtual boolean		controlNextChildFocus(VWindow *);
	virtual boolean		controlPrevChildFocus(VWindow *);

	virtual void		setChildFocus(VWindow *);
	virtual VWindow	   *getChildFocus();

	void				enableFocus(boolean = TRUE);
	void				disableFocus(boolean = TRUE);
	boolean				isFocusEnabled() const;
	boolean				isFocusDisabled() const;

	virtual void		setTabStop(boolean);
	virtual boolean		isTabStop() const;

	void				setTabIndex(int);
	void				setTabIndex(VWindow *child, int);
	void				swapTabIndices(VWindow *child1, VWindow *child2);
	int					getTabIndex() const;
	int					getTabIndex(VWindow *child) const;

	boolean				hasFocus() const;
	VWindow			   *containsFocus() const;

	virtual VWindow	   *getFocusCandidate(int = 0) const;
	virtual VWindow	   *getNextChildFocusCandidate(VWindow * = 0) const;
	virtual VWindow	   *getPrevChildFocusCandidate(VWindow * = 0) const;

	static boolean		checkLosingFocus(VWindow * = 0);

	// Update

	virtual void		update(VRectangle *, boolean = TRUE);
	virtual boolean		update();
	virtual boolean		paint();
	virtual boolean		erased();
	virtual void		destroyed();
	virtual boolean		moved(int, int);
	virtual boolean		resized(int, int);
	virtual boolean		close();
	virtual boolean		quit();
	void				erase();

	// Hidden, Disable

	void                hide();
	void                show(boolean update = TRUE);
	virtual boolean     isHidden();

	void                disable();
	void                enable();
	boolean             isEnabled();

	virtual void		showBool(boolean, boolean = TRUE);
	virtual void		enableBool(boolean);

	// Border

	void				setBorder();
	void				setBorder(boolean);
	boolean				hasBorder() const;

	// Z-Order

	void				bringToTop();
	void				bringToTop(VWindow *);
	void				setDefRatio();

	// Hashing

	HashKey				hash() const;
	boolean				isEqual(VObject *) const;

	// Type

	virtual WinType		winType();

	// Model

	virtual void		setModel(VObject *);
	VObject			   *getModel() const;

	// Foreground/Background

	void				setForeground(VColor, long = 0L);
	void				setDefaultForeground(boolean = TRUE);
	VColor				getForeground();
	boolean				getDefaultForeground() const;
	void				setBackground(VBrush *, long = 0L);
	VBrush			   *getBackground();

	// Cursor

	VMouseCursor	   *getCursor();
	void				setCursor(VMouseCursor *, long = 0L);

	// Font

	virtual VFont	   *getFont() const;
	virtual void		setFont(VFont *);
	void				setFont(VFont *, boolean);
	virtual void		fontSize(int *, int *);

	// Help

	void				setHelpId(long);
	long				getHelpId(boolean = FALSE) const;

	virtual boolean		contextHelp();
	virtual boolean		contextMenu();

	// Parent

	boolean				isParentOf(VWindow *);
	virtual VWindow	   *getParent();
	void				setParent(VWindow *);
	boolean				reparent(VWindow *, boolean = TRUE);
	VWindow			   *getTopLevelParent();
	VAppView		   *getAppLevelParent();
	virtual boolean		isTopLevel() const;

	// Children

	VOrdCollect		   *childrenOf();
	virtual void		addChild(VWindow *);
	virtual void        insertChild(VWindow *, int);
	boolean				removeChild(VWindow *);

	virtual void		childAdded(VWindow *, int);
	virtual void		childRemoved(VWindow *, int);
	virtual void		childReparented(VWindow *, VWindow *);
	virtual void		childParented(VWindow *, VWindow *);

	// Client Area

	virtual VWindow	   *getClientAreaWindow() const;

	// Non-Client Area Size

    static void			stdSize(const VStyle&, int *, int *);

	// New Geometry Management

	VGeoManager		   *setGeoManager(VGeoManager *);
	VGeoManager		   *setGeoManager();
	VGeoForm		   *setGeoForm();
	VGeoRow			   *setGeoRow();
	VGeoRowOrColumn	   *setGeoRowOrColumn();
	VGeoColumn		   *setGeoColumn();
	VGeoManager		   *getGeoManager() const;
	VGeoForm		   *getGeoForm() const;
	VGeoRow			   *getGeoRow() const;
	VGeoColumn		   *getGeoColumn() const;
	VGeoRowOrColumn	   *getGeoRowOrColumn() const;
	VGeoFrame		   *getGeoFrame() const;
	int					getGeoIndex() const;

	virtual void		frameWindow(boolean minimum = FALSE);
	void				reframeWindow();
	void				reframeChildren();
	void				centerWindow(VWindow * = 0);

	void				determinePreferredSize(int *w, int *h) const;
	void				determineConstrainedSize(int *, int *,
												 int, int) const;
	void				determineMinimumSize(int *w, int *h) const;
	void				determineMaximumSize(int *w, int *h) const;

	void				constrainMinimumSize();
	void				constrainMinimumSize(int w, int h);
	void				constrainMaximumSize();
	void				constrainMaximumSize(int w, int h);

	virtual void		getSystemBorderSize(int *, int *) const;
	virtual void		getInternalBorderGeometry(int *, int *,
												  int *, int *) const;

	void				setDontFrame(boolean);
	boolean				isDontFrame() const;

	void				invalidateGeometry(boolean propagate_up = TRUE);
	virtual void		updateGeometry(int = 0);

	virtual void		resizeClientArea(int w, int h);

	virtual void		getPreferredSize(int *w, int *h) const;
	virtual boolean		getConstrainedSize(int *w, int *h, int, int) const;
	virtual void		getMinimumSize(int *w, int *h) const;
	virtual void		getMaximumSize(int *w, int *h) const;

	virtual void		setGeometry(int, int, int, int);
	virtual void		setGeometrySize(int, int);
	virtual void		setGeometryPosition(int, int);
	virtual void		setGeometryOffset(int, int);

	static void			beginDeferGeometryChanges();
	static void			endDeferGeometryChanges();
	static void			flushDeferredGeometryChanges();

    // Old Geometry Management

	virtual boolean		frameChildren(int = -1, int = -1, boolean = FALSE);
	void				frame();
	void				getMin(int *, int *);
	boolean				makeLegible();
	virtual void		getMinSize(short&, short&);
	virtual void		getPrefSize(short&, short&);
	void				setAutoFrame(boolean);
	void				setAutoSize(boolean);
	void				attachWin(VWindow *, VAttach::Side,
											 VAttach::Type,
											 int = 0, VWindow * = 0);
	VAttach			   *getAttach() const;

	void				detachWin(VWindow *win);
	int					getPriority();
	void				setPriority(int p);
	unsigned			getPtest();
	void				setPtest(unsigned p);
	void				setAttach(VAttach::Side,
								  VAttach::Type, int , VWindow *);
	VRectangle			getPhysical() const;
	void				setPhysical(const VRectangle&);

	struct VOldAttachData {
		VAttach			attach;
    	integer			attachSibling[4];
	};

	// Query Size, Position, Frame

	void				sizeOfImage(int *, int *);
	void				sizeOfWin(int *, int *);
	void				getAbs(int *w, int *h);
	void				getAbs(int *x, int *y, int *w, int *h);
	void				getRatio(float *x, float *y, float *w, float *h);
	void				getRel(int *x, int *y, int *w = 0, int *h = 0);
	void				getParentAbs(int *x, int *y, int *w, int *h);
	void				getClientRel(int *x, int *y, int *w = 0, int *h = 0);
	void				getClientAbs(int *x, int *y, int *w = 0, int *h = 0);
	void				getWindowRect(VRectangle&);
	void				getFrame(VFrame& f);
	VLocation			dimension();

	// Set Frame

	void				setRatio(VRatio, VRatio, VRatio, VRatio);
	void				setFrame(VFrame&);
	void				setFrameRatios(boolean, boolean, boolean, boolean);
	void				setFrameUnits(VFrame::Units);
	void				move(VRectangle&, boolean = TRUE);
	void				move(const VFrame&, boolean = TRUE);
	void				move(VRatio, VRatio, boolean = TRUE);
	void				move(VRatio, VRatio, VRatio , VRatio , boolean = TRUE);
	void				resize(VRatio, VRatio h);
	void				size(int, int);
	void				translate(VRatio, VRatio);

	// Coordinate Mapping

	void				mapToScreen(int *, int *);
	void				mapToWin(int *, int *);

	// Redraw Attributes

    void				setRedrawUponResize(boolean);
    boolean				isRedrawUponResize() const;
    void				setAutoErased(boolean);
    boolean				isAutoErased() const;

	// Scroll

	void				scroll(int, int);

	// Drag & Drop

	void				registerDropSite(VDropSite *);
	void				registerDragSite(VDragSite *);

	// System Attributes

	virtual void		systemColorChanged();
	virtual void		systemFontChanged();
	virtual void		systemMetricsChanged();
	virtual boolean		paletteChanged(boolean = -1);
	virtual void		setPalette(HPALETTE);

	// Native GUI

	virtual boolean		build(const VStyle& style);
	HWND				hwnd();
	virtual boolean		user(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual long		winDispatch(VEvent& e);
	virtual HDC			getHndl();
	virtual void		ungetHndl();

	// OLE

	VComponentWindow   *getComponentThis();
	void				setComponentThis(VComponentWindow *);
	VComponentWindow   *findComponentWindow(const VString&, VClass *);

	// Archiving

	void				setArchiveEnable(boolean);
	boolean				getArchiveEnable();

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a);
	void				getFrom(VArchiver&);
	void				getFrom(VArchiver&, VObject *, boolean = TRUE);
	virtual void		realize(VWindow *);
	void				realizeChildren(VWindow * = 0,
										VObject * = 0, method = 0,
										void * = 0);
	virtual boolean		loadResources(VRscArchiver&);
	void				replaceChild(VWindow *, VWindow *);
	friend boolean		saveDialog(VDialog *, const VString&, VRscArchiver *);
#endif

	VWindow			   *findWindow(const VString&, VClass * = 0) const;

	// Event Vectoring

#ifndef CV_NOVECTOREVENTS
	virtual void		setEventVector(Event event, VObject *, method);
	void				onEvent(int event, VObject *, method);
	boolean				vectorEvent(int event);
#endif

	// Internal

public:
	void				clipToChildren(boolean);
	void				clipToSiblings(boolean);
	boolean				clipToChildren() const;
	boolean				clipToSiblings() const;
	WNDPROC				defaultProc();
protected:
    void				init(VWindow *pwin);
    virtual void		getInfo(VWinInfo& info);
    virtual UINT		getCtlColor();
    void				moveWin(boolean b = TRUE);
	virtual boolean		has3dEffects() const;
	virtual boolean		has3dEffects(V_3D_EFFECTS) const;
	boolean				isBeingDeleted() const;
	boolean				isBeingDestroyed() const;
	int					obscured() const;
	void				shedFocus();
#ifndef CV_NOARCHIVER
	void				putTo(VArchiver&, boolean);
	void				putChildrenTo(VArchiver&);
	void				getChildrenFrom(VArchiver&, boolean = TRUE);
	VOrdCollect		   *archiveChildrenOf();
	void				getAttachFrom(VArchiver&);
#endif

	// Data-members

protected:
	VString			   *winName;
    union {
		VGeoManager	   *geoManager;
		VGeoFrame	   *geoFrame;
    };
	VObject			   *model;
	VWindow			   *parent;
	VOrdCollect		   *children;
#ifndef CV_NOARCHIVER
	VOrdCollect		   *archiveChildren;
	VOldAttachData	   *oldAttachArchiveData;
	int				   *childrenZorder;
#endif
	VComponentWindow   *componentWindow;
	VWindow			   *dockableWindowOwner;
#ifndef CV_NOVECTOREVENTS
	void			   *theEventManager;
#endif
	VWindow			   *curChildFocus;
#ifdef CV_OLE_DRAG_DROP
	VDragSite		   *dragSite;
	VDropSite		   *dropSite;
#endif
	VFont			   *winFont;
	VFont			   *pFontFromArchive;
	VMouseCursor	   *pCursor;
	VBrush			   *pBackground;
	VColor				pForeground;
	VFrame				winFrame;
	VWinStyle			style;
	HWND				hWnd;
	HINSTANCE			hInstance;
	WNDPROC				defProc;
	int					setDefaultFont;
	long				helpId;
	short				constrainedMinimumWidth;
	short				constrainedMinimumHeight;
	short				constrainedMaximumWidth;
	short				constrainedMaximumHeight;
	boolean				beingDeleted : 1;
	boolean				beingDestroyed : 1;
	boolean				componentWindowIsRawMemory : 1;
	boolean				backgroundBrushIsMine : 1;
	boolean				isPaletteChanging : 1;
	boolean				allowFocus : 1;
	boolean				defaultForeground : 1;
	boolean				processingNextChildFocus : 1;
	boolean				processingPrevChildFocus : 1;
	short				fromRsc : 1;
	boolean				bTabStop : 1;
	boolean				redrawUponResize : 1;
	boolean				autoErased : 1;
	boolean				archiveChildrenNotRealized : 1;
#ifndef CV_NOARCHIVER
	boolean				enabledFlag : 1;
	boolean				hiddenFlag : 1;
	boolean				archiveEnableFlag : 1;
#endif

	// Friends

private:
    friend				VComboBox;
    friend				VControl;
    friend				VDialog;
    friend				VDockableWindow;
    friend				VDragSite;
    friend				VDropSite;
    friend				VGeoForm;
    friend				VGeoFrame;
    friend				VGeoManager;
    friend				VMdiAppView;
    friend				VNotifier;
    friend				VStatusBar;
    friend				VView;
    friend				DesignerView;   // obsolete
    friend				DesignerWindow; // obsolete
    friend				VbAttachDialog; // obsolete
};

extern VClass *VWindowCls;

// ----------------------------------------------------------------------------
//
inline VString *VWindow::getName()
{
    return winName;
}

inline WNDPROC VWindow::defaultProc()
{
    return defProc;
}

inline VWindow *VWindow::getChildFocus()
{
    return curChildFocus;
}

inline void VWindow::hide()
{
    showBool(FALSE);
}

inline void VWindow::disable()
{
    enableBool(FALSE);
}

inline void VWindow::enable()
{
    enableBool(TRUE);
}

inline VColor VWindow::getForeground()
{
    return pForeground;
}

inline VBrush *VWindow::getBackground()
{
    return pBackground;
}

inline VMouseCursor *VWindow::getCursor()
{
    return pCursor;
}

inline void VWindow::getFrame(VFrame& f)
{
    f = winFrame;
}

inline VWindow *VWindow::getParent()
{
    return parent;
}

inline void VWindow::setParent(VWindow *pwin)
{
    parent = pwin;
}

inline VComponentWindow *VWindow::getComponentThis()
{
    return componentWindowIsRawMemory ? 0 : componentWindow;
}

inline void VWindow::setComponentThis(VComponentWindow *componentWindow)
{
    this->componentWindow = componentWindow;
}

inline VOrdCollect *VWindow::childrenOf()
{
    return children;
}

inline HWND VWindow::hwnd()
{
    return hWnd;
}

inline void VWindow::setPriority(int p)
{
}

inline int VWindow::getPriority()
{
    return 1;
}

inline void VWindow::setPtest(unsigned)
{
}

inline unsigned VWindow::getPtest()
{
	return 0;
}

inline void VWindow::setAutoSize(boolean tf)
{
}

inline void VWindow::setAutoFrame(boolean tf)
{
}

inline boolean VWindow::isBeingDeleted () const
{
    return beingDeleted;
}

inline boolean VWindow::isBeingDestroyed () const
{
    return beingDestroyed != 0;
}

inline void VWindow::setModel(VObject *m)
{
    model = m;
}

inline VObject *VWindow::getModel() const
{
    return model;
}

inline VWinStyle& VWindow::getStyle(void)
{
    return style;
}

inline void VWindow::setAutoErased(boolean b)
{
    autoErased = b;
}

inline boolean VWindow::isAutoErased() const
{
    return autoErased;
}

inline void VWindow::setRedrawUponResize(boolean b)
{
    redrawUponResize = b;
}

inline boolean VWindow::isRedrawUponResize() const
{
    return redrawUponResize;
}

inline VGeoFrame *VWindow::getGeoFrame() const
{
    return geoFrame;
}

inline void VWindow::systemColorChanged()
{
}

inline void VWindow::systemFontChanged()
{
}

inline void VWindow::systemMetricsChanged()
{
}

inline VWindow *VWindow::getClientAreaWindow() const
{
    return (VWindow *)this;
}

inline boolean VWindow::getDefaultForeground() const
{
    return defaultForeground;
}

inline boolean VWindow::isFocusEnabled() const
{
	return allowFocus;
}

inline void VWindow::enableFocus(boolean b)
{
	disableFocus(!b);
}

inline boolean VWindow::isFocusDisabled() const
{
	return !allowFocus;
}

#ifndef CV_NOARCHIVER

inline boolean VWindow::getArchiveEnable()
{
    return archiveEnableFlag;
}

inline VOrdCollect *VWindow::archiveChildrenOf()
{
    return archiveChildren;
}

#endif

#ifndef CV_NOVECTOREVENTS

inline void VWindow::onEvent(int event, VObject *o, method m)
{
	setEventVector(Event(event), o, m);
}

#endif

#endif // VWINDOW_H
