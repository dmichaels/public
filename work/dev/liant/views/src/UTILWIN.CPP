// $Header:   Q:/views/common/vcs/UTILWIN.CPV   1.36   Mar 24 1997 16:14:02   scottt  $

//	utilwin.cpp
//
//	VUtilityWindow implementation [Common]
//
//  Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	10/18/95 dgm	Original.
//	10/19/95 dgm	Updated.
//	11/01/95 dgm	Updated.
//	11/01/95 dgm	Changed activate()/deactivate() signature.
//	11/02/95 dgm	Changed getNonClientAreaSize() to getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	11/15/95 dgm	Updated.
//	11/29/95 dgm	Fixed brush deletion problem.
//	12/05/95 dgm	Changed to use VColorScheme::systemXXX().
//	12/07/95 dgm	Added setOpaqueResizing/Moving() support.
//	12/08/95 dgm	Added setDisableDecorations() support.
//	12/08/95 dgm	Motif changes.
//	12/12/95 dgm	Changed changeGeometryXXX() to setGeometryXXX().
//	02/07/96 dgm	Added NoClose.
//  02/19/96 evc    Made the defaultStyle CV_NEARDATA
//  03/04/96 dgm    Minor fixes in mouseMv() and StyleNoCloseBox.
//  03/20/96 dgm    Fixed setTitle() to update immediately, and
//					enhanced painting code to not flash much in this case.
//	03/21/96 dgm	Archiver support.
//	04/17/96 dgm	Minor changes to suppress warnings.
//	04/19/96 dgm	Minor changes initialize the opaque-move/resize bits.
//	04/29/96 dgm	More archiving changes.
//	05/13/96 dgm	For know, till we get more time, make Motif paint
//					like Windows 95; better than nothing (I think).
//	05/14/96 dgm	Minor fix in setCloseBehavior().
//	05/20/96 dgm	Minor fixes.
//	09/12/96 dgm	Added clipToChildren() calls.
//	09/13/96 dgm	Fixed memory leaks (theTitle & utilityWindows).
//	09/16/96 pkt	updated for WebShop.
//	09/17/96 pkt	updated some more for WebShop.
//
//	02/04/97 pkt	removed handling of style defaults from 'init'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "utilwin.h"
#include "port.h"
#include "brush.h"
#include "pen.h"
#include "color.h"
#include "clrscm.h"
#include "font.h"
#ifndef CV_NO_MOUSE_EVENTS
#include "mouscrsr.h"
#include "rband.h"
#endif
#include "str.h"
#include "rect.h"
#include "ordcllct.h"
#include "notifier.h"
#include "popupmen.h"
#include "menuitem.h"
#include "menudata.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#include "cllbckls.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VUtilityWindow, VPopupWindow)
defineArchiveRevision(VUtilityWindow, 1)

// --------------------------------------------------------------------------
//
static long					objectCount						= 0;

static short				defaultBorderThicknessX			= 0;
static short				defaultBorderThicknessY			= 0;
static short				defaultSizableBorderThicknessX	= 0;
static short				defaultSizableBorderThicknessY	= 0;
static short				defaultResizeCornerLength		= 0;
static short				defaultCaptionHeight			= 0;
static short				defaultCloseBoxWidth			= 0;
static short				defaultCloseBoxHeight			= 0;
static short				defaultHalfCaptionHeight		= 0;
static short				defaultHalfCloseBoxWidth		= 0;
static short				defaultHalfCloseBoxHeight		= 0;
static short				defaultCloseBoxSpacingX			= 0;
static short				defaultCloseBoxSpacingY			= 0;
static short				defaultTitleSpacing				= 0;
static boolean				closeBoxRightJustified			= FALSE;

static VFont			   *halfCaptionFont					= 0;
static VFont			   *captionFont						= 0;

static VPen				   *activeFlatBackgroundPen			= 0;
static VPen				   *activeFlatBorderPen				= 0;
static VBrush			   *activeBackgroundBrush			= 0;
static VPen				   *activeForegroundPen				= 0;
static VPen				   *inactiveFlatBackgroundPen		= 0;
static VPen				   *inactiveFlatBorderPen			= 0;
static VBrush			   *inactiveBackgroundBrush			= 0;
static VPen				   *inactiveForegroundPen			= 0;
static VPen				   *flatBackgroundPen				= 0;
static VBrush			   *shadedBackgroundBrush			= 0;
static VPen				   *shadedForegroundPen				= 0;
static VPen				   *shadedHighlightPen				= 0;
static VPen				   *shadedShadowPen					= 0;
static VPen				   *shadedDarkHighlightPen			= 0;
static VPen				   *shadedDarkShadowPen				= 0;

static VMouseCursor		   *leftCursor						= 0;
static VMouseCursor		   *rightCursor						= 0;
static VMouseCursor		   *topCursor						= 0;
static VMouseCursor		   *bottomCursor					= 0;
static VMouseCursor		   *topLeftCursor					= 0;
static VMouseCursor		   *topRightCursor					= 0;
static VMouseCursor		   *bottomLeftCursor				= 0;
static VMouseCursor		   *bottomRightCursor				= 0;

static VPort			   *thePort							= 0;
static VRectangle		   *theRectangle					= 0;

static int					lastRubberBandX					= 0;
static int					lastRubberBandY					= 0;
static int					lastRubberBandW					= 0;
static int					lastRubberBandH					= 0;

static int					windows95						= 0;
static int					styleWindows95					= 0;
static int					styleWindows31					= 0;
static int					styleOs2						= 0;
static int					styleMotif						= 0;
static short				styleThreeDee					= 0;

static VOrdCollect		   *utilityWindows					= 0;
static VUtilityWindow::
	   ConfigurationMenu   *configurationMenu				= 0;

// --------------------------------------------------------------------------
//
VUtilityWindow::VUtilityWindow()
	: VPopupWindow()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VUtilityWindow::VUtilityWindow(const VFrame& frame, VWindow *parentwin,
							   const VStyle& style)
	: VPopupWindow()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VUtilityWindow::VUtilityWindow(VWindow *parentwin, const VStyle& style)
	: VPopupWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VUtilityWindow::VUtilityWindow(const VString& name,
							   const VFrame& frame, VWindow *parentwin,
							   const VStyle& style)
	: VPopupWindow()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VUtilityWindow::VUtilityWindow(const VString& name,
							   VWindow *parentwin, const VStyle& style)
	: VPopupWindow()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::operator()(const VFrame &frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::operator()(const VFrame &frame, VWindow *parentwin,
							    const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::operator()(const VFrame &frame, VWindow *parentwin,
							    const VStyle& style, const VString& name)
{
	VPopupWindow::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VUtilityWindow::~VUtilityWindow()
{
	if (theTitle != 0) {
		delete theTitle;
	}
	if (utilityWindows != 0) {
		utilityWindows->removeSame(this);
	}
	deleteStaticData();
}

// --------------------------------------------------------------------------
//
VStyle& VUtilityWindow::getClassDefaultStyle(void)
{
	static VWinStyle CV_NEARDATA default_style
								 (VPopupWindow::getClassDefaultStyle() +
								  setTransient +
								  StyleCaption +
								  StyleCloseBox +
								  StyleSizable);
	return default_style;
}

// --------------------------------------------------------------------------
//
VStyle& VUtilityWindow::getDefaultStyle(void) const
{
	return getClassDefaultStyle();
}

#if defined(CV_WINDOWS)

// --------------------------------------------------------------------------
//
void VUtilityWindow::getInfo(VWinInfo& wi)
{
	VPopupWindow::getInfo(wi);
#ifndef CV_REMOTE_GUI
	wi.style &= ~(WS_BORDER |
				  WS_THICKFRAME |
				  WS_CAPTION |
				  WS_SYSMENU |
				  WS_MINIMIZEBOX |
				  WS_MAXIMIZEBOX);
#endif
}

#elif defined(CV_MOTIF)

void VUtilityWindow::getInfo(VWinInfo& wi)
{
	VPopupWindow::getInfo(wi);
	if (wi.sizingWid != 0) {
		wi.sizingWid = XtParent(wi.sizingWid);
	}
}

#elif defined(PM)

void VUtilityWindow::getInfo(VWinInfo& wi)
{
	VPopupWindow::getInfo(wi);
	wi.frameFlag &= ~(FCF_BORDER |
				  FCF_SIZEBORDER |
				  FCF_TITLEBAR |
				  FCF_SYSMENU |
				  FCF_MINBUTTON |
				  FCF_MAXBUTTON);
}

#endif

// --------------------------------------------------------------------------
//
void VUtilityWindow::setTitle(const VString& title)
{
	if (theTitle == 0) {
		theTitle = new VString(title);
	}
	else {
		*theTitle = title;
	}
	paintTitle();
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::getTitle(VString& title) const
{
	if (theTitle != 0) {
		title = *theTitle;
	}
	else {
		title = "";
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setCloseBehavior(int cb)
{
	int draw_close_box = drawCloseBox;
	if (cb == 0) {
		deleteOnClose = 0;
		hideOnClose = 0;
	}
	else {
		if (cb & HideOnClose) {
			deleteOnClose = 0;
			hideOnClose = 1;
		}
		else if (cb & DeleteOnClose) {
			deleteOnClose = 1;
			hideOnClose = 0;
		}
		if (cb & NoClose) {
			drawCloseBox = 0;
		}
	}
	if (draw_close_box != drawCloseBox) {
		paint();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setMovingStyle(int ms)
{
	if (ms & NoMoving) {
		noMoving = 1;
	}
	else {
		noMoving = 0;
		if (ms & ClientAreaMoving) {
			clientAreaMoving = 1;
		}
		else {
			clientAreaMoving = 0;
		}
		if (ms & TitleBarMoving) {
			titleBarMoving = 1;
		}
		else {
			titleBarMoving = 0;
		}
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setResizingStyle(int rs)
{
	if (rs == NoResizing) {
		noResizing = 1;
		preferredResizing = 0;
	}
	else if (rs == PreferredResizing) {
		noResizing = 0;
		preferredResizing = 1;
	}
	else if (rs == NormalResizing) {
		noResizing = 0;
		preferredResizing = 0;
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setOpaqueResizing(boolean b)
{
#ifndef CV_REMOTE_GUI
	if (VRubberBand::getCurrent() == 0) {
		opaqueResizingExplicitlySet	= 1;
	}
#endif
	opaqueResizing = b;
#ifndef CV_REMOTE_GUI
	if (VRubberBand::getCurrent() != 0) {
		VRubberBand::getCurrent()->disableDrawing(opaqueResizing);
	}
#endif
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setOpaqueMoving(boolean b)
{
	opaqueMoving = b;
#ifndef CV_REMOTE_GUI
	if (VRubberBand::getCurrent() != 0) {
		VRubberBand::getCurrent()->disableDrawing(opaqueMoving);
	}
	else {
		opaqueMovingExplicitlySet = 1;
	}
#endif
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponBeginMoving(VObject *o, method m)
{
	clientObject = o;
	beginMovingMethod = (boolean (VObject::*)())m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponMoving(VObject *o, method m)
{
	clientObject = o;
	movingMethod = (boolean (VObject::*)
							(int&, int&, int&, int&, int, int))m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponEndMoving(VObject *o, method m)
{
	clientObject = o;
	endMovingMethod = (boolean (VObject::*)
							   (int, int, int, int, int, int, int&))m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponBeginResizing(VObject *o, method m)
{
	clientObject = o;
	beginResizingMethod = (boolean (VObject::*)())m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponResizing(VObject *o, method m)
{
	clientObject = o;
	resizingMethod = (boolean (VObject::*)(int, int, int&, int&, int))m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponEndResizing(VObject *o, method m)
{
	clientObject = o;
	endResizingMethod = (boolean (VObject::*)
								 (int, int, int, int, int, int&))m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponMouseDbl(VObject *o, method m)
{
	clientObject = o;
	mouseDblMethod = (boolean (VObject::*)(int, int))m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponRmouseDn(VObject *o, method m)
{
	clientObject = o;
	rmouseDnMethod = (boolean (VObject::*)(int, int))m;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::uponClose(VObject *o, method m)
{
	clientObject = o;
	closeMethod = (void (VObject::*)(VWindow *))m;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::beginMoving()
{
	if ((clientObject != 0) && (beginMovingMethod != 0)) {
		return (clientObject->*beginMovingMethod)();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::moving(int& x, int& y, int& w, int& h,
							   int mx, int my)
{
	if ((clientObject != 0) && (movingMethod != 0)) {
		return (clientObject->*movingMethod)(x, y, w, h, mx, my);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::endMoving(int x, int y, int w, int h,
								  int mx, int my, int& status)
{
	if ((clientObject != 0) && (endMovingMethod != 0)) {
		return (clientObject->*endMovingMethod)(x, y, w, h, mx, my, status);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::beginResizing()
{
	if ((clientObject != 0) && (beginResizingMethod != 0)) {
		return (clientObject->*beginResizingMethod)();
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::resizing(int x, int y,
								 int& w, int& h, int resize_area)
{
	if ((clientObject != 0) && (resizingMethod != 0)) {
		return (clientObject->*resizingMethod)(x, y, w, h, resize_area);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::endResizing(int x, int y, int w, int h,
									int resize_area, int& status)
{
	if ((clientObject != 0) && (endResizingMethod != 0)) {
		return (clientObject->*endResizingMethod)
				(x, y, w, h, resize_area, status);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::close()
{
	if ((clientObject != 0) && (closeMethod != 0)) {
		(clientObject->*closeMethod)(this);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::resizeClientArea(int w, int h)
{
	int border_w, border_h;
	getSystemBorderSize(&border_w, &border_h);
	setGeometrySize(w + border_w, h + border_h);
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::stdSize(const VStyle& style, int *w, int *h)
{
	int ncw = 0, nch = 0;
	if (style.contains(StyleCaption)) {
		nch += defaultHalfCaptionHeight;
	}
	if (style.contains(StyleSizable) ||
		style.contains(StyleModalBorder) ||
		style.contains(StyleBorder)) {
		ncw += defaultBorderThicknessX * 2;
		nch += defaultBorderThicknessY * 2;
	}
	if (w != 0) { *w += ncw; }
	if (h != 0) { *h += nch; }
}

// ---------------------------------------------------------------------------
//
void VUtilityWindow::getSystemBorderSize(int *width, int *height) const
{
	if (width  != 0) { *width  = borderThicknessX * 2; }
	if (height != 0) { *height = borderThicknessY * 2 + getTitleBarHeight(); }
}

// ----------------------------------------------------------------------------
//
void VUtilityWindow::getInternalBorderGeometry(int *left, int *right,
											   int *top, int *bottom) const
{
VPopupWindow::getInternalBorderGeometry(left, right, top, bottom);
return;
	if (left   != 0) { *left   = borderThicknessX; }
	if (right  != 0) { *right  = borderThicknessX; }
	if (top    != 0) { *top    = borderThicknessY + getTitleBarHeight(); }
	if (bottom != 0) { *bottom = borderThicknessY; }
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::getMinimumSize(int *w, int *h) const
{
	if (w != 0) {
		*w = minimumWidth;
	}
	if (h != 0) {
		*h = minimumHeight;
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::initialize()
{
	if (utilityWindows == 0) {
		utilityWindows = new VOrdCollect;
	}
	utilityWindows->add(this);

#ifndef CVS_LATER_DEFAULT
    style						= setTransient + getDefaultStyle();
#endif
	clientWindow				= 0;
	theTitle					= 0;
	clientObject				= 0;
	beginResizingMethod			= 0;
	resizingMethod				= 0;
	endResizingMethod			= 0;
	beginMovingMethod			= 0;
	movingMethod				= 0;
	endMovingMethod				= 0;
	mouseDblMethod				= 0;
	rmouseDnMethod				= 0;
	closeMethod					= 0;
	drawTitleBar				= 0;
	drawCloseBox				= 0;
	drawBorder					= 0;
	clientAreaMoving			= 0;
	titleBarMoving				= 0;
	noMoving					= 0;
	noResizing					= 0;
	preferredResizing			= 0;
	borderThicknessX			= 0;
	borderThicknessY			= 0;
	titleBarHeight				= 0;
	closeBoxWidth				= 0;
	closeBoxHeight				= 0;
	closeBoxSpacingX			= 0;
	closeBoxSpacingY			= 0;
	titleSpacing				= 0;
	resizeCornerLength			= 0;
	minimumWidth				= 0;
	minimumHeight				= 0;
	maximumWidth				= 0;
	maximumHeight				= 0;
	currentCursor				= 0;
	hasFocus					= 0;
	mouseOverhead				= 0;
	confinedToScreen			= 0;
	closeBoxArmed				= 0;
	closeBoxPressed				= 0;
	deleteOnClose				= 0;
	hideOnClose					= 1;
	smallTitleBar				= 1;
	decorationsDisabled			= 0;
	configurationMenuDisabled	= 0;
	opaqueResizingExplicitlySet	= 0;
	opaqueMovingExplicitlySet	= 0;

	initializeOpaqueMoveResize();

#ifndef CV_NOARCHIVER

	clientObjectIndex			= -1;
	beginResizingMethodIndex	= -1;
	resizingMethodIndex			= -1;
	endResizingMethodIndex		= -1;
	beginMovingMethodIndex		= -1;
	movingMethodIndex			= -1;
	endMovingMethodIndex		= -1;
	mouseDblMethodIndex			= -1;
	rmouseDnMethodIndex			= -1;
	closeMethodIndex			= -1;

#endif

	static int first_time = 1;
	if (first_time) {
		first_time = 0;
#if defined(CV_WINDOWS)
		if (LOBYTE(LOWORD(GetVersion())) >= 4) {
			windows95		= 1;
			styleWindows95	= 1;
		}
		else {
			styleWindows31	= 1;
		}
#elif defined(PM)
		styleOs2			= 1;
#elif defined(CV_MOTIF)
		styleMotif			= 1;
#endif
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setupMetrics()
{
	borderThicknessX	= drawBorder ? defaultBorderThicknessX : 0;
	borderThicknessY	= drawBorder ? defaultBorderThicknessY : 0;
	titleBarHeight		= drawTitleBar ? (smallTitleBar ?
										  defaultHalfCaptionHeight :
										  defaultCaptionHeight) : 0;
	if (drawCloseBox) {
		closeBoxWidth	= smallTitleBar ? defaultHalfCloseBoxWidth :
										  defaultCloseBoxWidth;
		closeBoxHeight	= smallTitleBar ? defaultHalfCloseBoxHeight :
										  defaultCloseBoxHeight;
	}
	else {
		closeBoxWidth	= 0;
		closeBoxHeight	= 0;
	}
	closeBoxSpacingX	= defaultCloseBoxSpacingX;
	closeBoxSpacingY	= defaultCloseBoxSpacingY;
	titleSpacing		= drawTitleBar ? defaultTitleSpacing : 0;
	resizeCornerLength	= defaultResizeCornerLength;
	minimumWidth		= borderThicknessX * 2 +
						  closeBoxWidth + closeBoxSpacingX * 2 + 1;
	minimumHeight		= titleBarHeight + borderThicknessY * 2 + 1;

	if (styleWindows95) {
		setBackground(shadedBackgroundBrush);
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::construct()
{
	clipToChildren(TRUE);

	constructStaticData();

	drawTitleBar		= style.contains(StyleCaption);
	drawCloseBox		= style.contains(StyleCloseBox);
	drawBorder			= style.contains(StyleSizable) ||
						  style.contains(StyleModalBorder) ||
						  style.contains(StyleBorder);
	clientAreaMoving	= 1;
	titleBarMoving		= 1;

	setupMetrics();
	setGeoManager(new Manager);
	clientWindow = newClientWindow(this);
	clientWindow->clipToChildren(TRUE);
}

// --------------------------------------------------------------------------
//
VWindow *VUtilityWindow::newClientWindow(VWindow *parentwin)
{
	return new ClientWindow(parentwin);
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::initializeOpaqueMoveResize()
{
	if (opaqueResizingExplicitlySet && opaqueMovingExplicitlySet) {
		return;
	}
#if defined(CV_WIN32) && defined(SPI_GETDRAGFULLWINDOWS)
	BOOL b;
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &b, 0);
	if (!opaqueMovingExplicitlySet) { opaqueMoving = b; }
	if (!opaqueResizingExplicitlySet) { opaqueResizing = b; }
#else
	if (!opaqueMovingExplicitlySet) { opaqueMoving = 0; }
	if (!opaqueResizingExplicitlySet) { opaqueResizing = 0; }
#endif
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::constructStaticData()
{
	if (objectCount++ > 0) {
		return;
	}

	// Calculate the default border and caption sizes.

#if defined(CV_WINDOWS)

	if (styleWindows95) {

		styleThreeDee = 1;

		defaultBorderThicknessX		= GetSystemMetrics(SM_CXFRAME);
		defaultBorderThicknessY		= GetSystemMetrics(SM_CYFRAME);
		defaultCaptionHeight		= GetSystemMetrics(SM_CYCAPTION) -
									  GetSystemMetrics(SM_CYBORDER) * 2;
#if defined(CV_WIN32)
		if (windows95) {
			defaultHalfCaptionHeight = GetSystemMetrics(SM_CYSMCAPTION);
		}
		else {
			defaultHalfCaptionHeight = 16;
		}
#else
		defaultHalfCaptionHeight	= 16;
#endif
		defaultCloseBoxWidth		= defaultCaptionHeight - 3;
		defaultCloseBoxHeight		= defaultCaptionHeight - 4;
		defaultHalfCloseBoxWidth	= defaultHalfCaptionHeight - 4;
		defaultHalfCloseBoxHeight	= defaultHalfCaptionHeight - 4;
		defaultCloseBoxSpacingX		= 2;
		defaultCloseBoxSpacingY		= 2;
		defaultTitleSpacing			= 2;
		defaultResizeCornerLength	= defaultCaptionHeight +
									  defaultBorderThicknessY + 2;
		closeBoxRightJustified		= TRUE;
	}
	else {

		styleThreeDee = 0;

		defaultBorderThicknessX		= GetSystemMetrics(SM_CXFRAME);
		defaultBorderThicknessY		= GetSystemMetrics(SM_CYFRAME);
		defaultCaptionHeight		= GetSystemMetrics(SM_CYCAPTION) -
									  GetSystemMetrics(SM_CYBORDER) * 2;
		if (windows95) {
			defaultHalfCaptionHeight = defaultCaptionHeight / 2;
		}
		else {
			defaultHalfCaptionHeight = defaultCaptionHeight / 2 + 2;
		}
		defaultCloseBoxWidth		= defaultCaptionHeight;
		defaultCloseBoxHeight		= defaultCaptionHeight;
		defaultHalfCloseBoxWidth	= defaultHalfCaptionHeight;
		defaultHalfCloseBoxHeight	= defaultHalfCaptionHeight;
		defaultCloseBoxSpacingX		= 0;
		defaultCloseBoxSpacingY		= 0;
		defaultTitleSpacing			= 2;
		defaultResizeCornerLength	= defaultCaptionHeight +
									  defaultBorderThicknessY + 2;
		closeBoxRightJustified		= FALSE;
	}

#elif defined(CV_MOTIF)

	// ### Motif

	styleThreeDee = 1;

	defaultBorderThicknessX			= 4;
	defaultBorderThicknessY			= 4;
	defaultCaptionHeight			= 20;
	defaultHalfCaptionHeight		= 17;
	defaultCloseBoxWidth			= defaultCaptionHeight - 4;
	defaultCloseBoxHeight			= defaultCloseBoxWidth;
	defaultHalfCloseBoxWidth		= defaultHalfCaptionHeight - 3;
	defaultHalfCloseBoxHeight		= defaultHalfCaptionHeight - 4;
	defaultCloseBoxSpacingX			= 2;
	defaultCloseBoxSpacingY			= 2;
	defaultTitleSpacing				= 2;
	defaultResizeCornerLength		= defaultCaptionHeight +
									  defaultBorderThicknessY + 2;
	closeBoxRightJustified			= TRUE;

#elif defined(PM)

	// ### OS/2

	styleThreeDee = 1;

	defaultBorderThicknessX			= 4;
	defaultBorderThicknessY			= 4;
	defaultCaptionHeight			= 20;
	defaultHalfCaptionHeight		= 20;
	defaultCloseBoxWidth			= defaultCaptionHeight - 4;
	defaultCloseBoxHeight			= defaultCloseBoxWidth;
	defaultHalfCloseBoxWidth		= defaultHalfCaptionHeight - 3;
	defaultHalfCloseBoxHeight		= defaultHalfCaptionHeight - 4;
	defaultCloseBoxSpacingX			= 2;
	defaultCloseBoxSpacingY			= 2;
	defaultTitleSpacing				= 2;
	defaultResizeCornerLength		= defaultCaptionHeight +
									  defaultBorderThicknessY + 2;
	closeBoxRightJustified			= TRUE;

#else

#error "Not implemented: VUtilityWindow::constructStaticData()!"

#endif

	// Create the mouse cursors we'll be needing.

#if defined(CV_REMOTE_GUI)
#elif defined(CV_MOTIF)
	leftCursor				= new VMouseCursor(SizeW);
	rightCursor				= new VMouseCursor(SizeE);
	topCursor				= new VMouseCursor(SizeN);
	bottomCursor			= new VMouseCursor(SizeS);
	topLeftCursor			= new VMouseCursor(SizeNW);
	topRightCursor			= new VMouseCursor(SizeNE);
	bottomLeftCursor		= new VMouseCursor(SizeSW);
	bottomRightCursor		= new VMouseCursor(SizeSE);
#else
	leftCursor				= new VMouseCursor(SizeWE);
	rightCursor				= new VMouseCursor(SizeWE);
	topCursor				= new VMouseCursor(SizeNS);
	bottomCursor			= new VMouseCursor(SizeNS);
	topLeftCursor			= new VMouseCursor(SizeNWSE);
	topRightCursor			= new VMouseCursor(SizeNESW);
	bottomLeftCursor		= new VMouseCursor(SizeNESW);
	bottomRightCursor		= new VMouseCursor(SizeNWSE);
#endif

	// Create the fonts we'll be needing.

	constructStaticFonts();

	// Create the appropriately colored pens and brushes.

	constructStaticColors();

	// Create objects which we'd really only like to create once.

	thePort					= new VPort;
	theRectangle			= new VRectangle;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::deleteStaticData(boolean delete_everything)
{
	if (--objectCount > 0) {
		return;
	}

	delete captionFont;			captionFont			= 0;
	delete halfCaptionFont;		halfCaptionFont		= 0;
#ifndef CV_REMOTE_GUI
	delete leftCursor;			leftCursor			= 0;
	delete rightCursor;			rightCursor			= 0;
	delete topCursor;			topCursor			= 0;
	delete bottomCursor;		bottomCursor		= 0;
	delete topLeftCursor;		topLeftCursor		= 0;
	delete topRightCursor;		topRightCursor		= 0;
	delete bottomLeftCursor;	bottomLeftCursor	= 0;
	delete bottomRightCursor;	bottomRightCursor	= 0;
#endif
	delete thePort;				thePort				= 0;
	delete theRectangle;		theRectangle		= 0;

	if (delete_everything) {
		delete configurationMenu;	configurationMenu	= 0;
		delete utilityWindows;		utilityWindows		= 0;
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::constructStaticColors()
{
	activeFlatBackgroundPen =
		&VColorScheme::systemPen(VColorScheme::TopActiveFlatBackground);
	activeFlatBorderPen =
		&VColorScheme::systemPen(VColorScheme::TopActiveFlatBorder);
	activeBackgroundBrush =
		&VColorScheme::systemBrush(VColorScheme::TopActiveCaptionBackground);
	activeForegroundPen =
		&VColorScheme::systemPen(VColorScheme::TopActiveCaptionForeground);

	inactiveFlatBackgroundPen =
		&VColorScheme::systemPen(VColorScheme::TopInactiveFlatBackground);
	inactiveFlatBorderPen =
		&VColorScheme::systemPen(VColorScheme::TopInactiveFlatBorder);
	inactiveBackgroundBrush =
		&VColorScheme::systemBrush(VColorScheme::TopInactiveCaptionBackground);
	inactiveForegroundPen =
		&VColorScheme::systemPen(VColorScheme::TopInactiveCaptionForeground);

	flatBackgroundPen =
		&VColorScheme::systemPen(VColorScheme::FlatBackground);
	shadedBackgroundBrush =
		&VColorScheme::systemBrush(VColorScheme::ShadedBackground);
	shadedForegroundPen =
		&VColorScheme::systemPen(VColorScheme::ShadedForeground);
	shadedHighlightPen =
		&VColorScheme::systemPen(VColorScheme::ShadedHighlight);
	shadedShadowPen =
		&VColorScheme::systemPen(VColorScheme::ShadedShadow);
	shadedDarkHighlightPen =
		&VColorScheme::systemPen(VColorScheme::ShadedDarkHighlight);
	shadedDarkShadowPen =
		&VColorScheme::systemPen(VColorScheme::ShadedDarkShadow);
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::constructStaticFonts()
{
	captionFont			= new VFont;

	if (styleThreeDee || !windows95) {
		halfCaptionFont	= new VFont;
	}
	else {
		halfCaptionFont	= new VFont("Small Fonts", 6);
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::deleteStaticFonts()
{
	delete captionFont;			captionFont			= 0;
	delete halfCaptionFont;		halfCaptionFont		= 0;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setDisableDecorations(boolean b)
{
	if (b) {
		if (decorationsDisabled) {
			return;
		}
		decorationsDisabled = 1;
	}
	else {
		if (!decorationsDisabled) {
			return;
		}
		decorationsDisabled = 0;
	}
	frameChildren();
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::supportsStyleWindows95()
{
#if defined(CV_WINDOWS)
	return TRUE;
#else
	return FALSE;
#endif
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::supportsStyleWindows31()
{
#if defined(CV_WINDOWS)
	return TRUE;
#else
	return FALSE;
#endif
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::supportsStyleOs2()
{
#if defined(PM)
	return TRUE;
#else
	return FALSE;
#endif
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::supportsStyleMotif()
{
#if defined(CV_MOTIF)
	return TRUE;
#else
	return FALSE;
#endif
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::isStyleWindows95()
{
	return styleWindows95;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::isStyleWindows31()
{
	return styleWindows31;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::isStyleOs2()
{
	return styleOs2;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::isStyleMotif()
{
	return styleMotif;
}

// --------------------------------------------------------------------------
//
char VUtilityWindow::getBorderStyle() const
{
	if		(styleWindows95)	{ return 1; }
	else if	(styleWindows31)	{ return 2; }
	else if	(styleOs2)			{ return 3; }
	else if	(styleMotif)		{ return 4; }
	else						{ return 0; }
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setStyleWindows95()
{
	if (!styleWindows95) {
		styleWindows95	= 1;
		styleWindows31	= 0;
		styleOs2		= 0;
		styleMotif		= 0;
		setNewStyle();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setStyleWindows31()
{
	if (!styleWindows31) {
		styleWindows95	= 0;
		styleWindows31	= 1;
		styleOs2		= 0;
		styleMotif		= 0;
		setNewStyle();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setStyleOs2()
{
	if (!styleWindows31) {
		styleWindows95	= 0;
		styleWindows31	= 0;
		styleOs2		= 1;
		styleMotif		= 0;
		setNewStyle();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setStyleMotif()
{
	if (!styleMotif) {
		styleWindows95	= 0;
		styleWindows31	= 0;
		styleOs2		= 0;
		styleMotif		= 1;
		setNewStyle();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::setNewStyle()
{
	VOrdCollect *hidden = 0;
	if (utilityWindows != 0) {
		DO (*utilityWindows, VUtilityWindow, utility_window)
			if (utility_window->isHidden()) {
				if (hidden == 0) {
					hidden = new VOrdCollect;
				}
				hidden->add(utility_window);
			}
			utility_window->hide();
		END
	}
	long object_count = objectCount;
	objectCount = 1;
	VUtilityWindow::deleteStaticData(FALSE);
	VUtilityWindow::constructStaticData();
	objectCount = object_count;
	if (utilityWindows != 0) {
		DO (*utilityWindows, VUtilityWindow, utility_window)
			int oldncw = utility_window->borderThicknessX * 2;
			int oldnch = utility_window->borderThicknessY * 2 +
						 utility_window->getTitleBarHeight();
			utility_window->setupMetrics();
			int newncw = utility_window->borderThicknessX * 2;
			int newnch = utility_window->borderThicknessY * 2 +
						 utility_window->getTitleBarHeight();
			if ((newncw != oldncw) || (newnch != oldnch)) {
				int w, h;
				utility_window->sizeOfWin(&w, &h);
				utility_window->setGeometrySize
					(w + (newncw - oldncw), h + (newnch - oldnch));
			}
			if ((hidden == 0) || !hidden->findSame(utility_window)) {
				utility_window->show();
			}
		END
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::systemFontChanged()
{
	deleteStaticFonts();
	constructStaticFonts();
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::erased()
{
	return VPopupWindow::erased();
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::paint()
{
	// Get the current window size.

	int w, h;
	sizeOfImage(&w, &h);
	if ((w == 0) || (h == 0)) {
		return TRUE;
	}

	// Open the port.

	if ((thePort == 0) || !thePort->openOn(this)) {
		return FALSE;
	}

	// Paint the decorations.

	paintBorder(w, h);
	if (!decorationsDisabled) {
		paintTitleBar(w, h);
	}

	// Close the port.

	thePort->close();

	return TRUE;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::paintTitleBar(int w, int h)
{
	paintTitle(w, h);
	paintCloseBox(w, h);
}

#if defined(CV_WINDOWS) || defined(PM) || defined(CV_MOTIF)

// --------------------------------------------------------------------------
//
void VUtilityWindow::paintBorder(int w, int h)
{
	if (!drawBorder) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	if (styleThreeDee) {

		thePort->usePen(shadedDarkHighlightPen);
		thePort->moveTo(0, 0);
		thePort->lineTo(w - 1, 0);
		thePort->moveTo(0, 1);
		thePort->lineTo(0, h - 2);

		thePort->usePen(shadedHighlightPen);
		thePort->moveTo(1, 1);
		thePort->lineTo(w - 2, 1);
		thePort->moveTo(1, 2);
		thePort->lineTo(1, h - 2);

		thePort->usePen(shadedDarkShadowPen);
		thePort->moveTo(w - 1, 0);
		thePort->lineTo(w - 1, h);
		thePort->moveTo(0, h - 1);
		thePort->lineTo(w - 1, h - 1);

		thePort->usePen(shadedShadowPen);
		thePort->moveTo(w - 2, 1);
		thePort->lineTo(w - 2, h - 1);
		thePort->moveTo(1, h - 2);
		thePort->lineTo(w - 2, h - 2);
	}
	else {
		if (hasFocus) {
			thePort->usePen(activeFlatBorderPen);
		}
		else {
			thePort->usePen(inactiveFlatBorderPen);
		}
		theRectangle->set(CornerDim, 0, 0, w, h);
		thePort->frameRegion(theRectangle);
		theRectangle->set(CornerDim,
						  borderThicknessX - 1,
						  borderThicknessY - 1,
						  w - borderThicknessX * 2 + 2,
						  h - borderThicknessY * 2 + 2);
		thePort->frameRegion(theRectangle);
		thePort->moveTo(borderThicknessX,
						borderThicknessY + titleBarHeight);
		thePort->lineTo(w - borderThicknessX,
						borderThicknessY + titleBarHeight);
		if (hasFocus) {
			activeFlatBackgroundPen->width(borderThicknessX - 2);
			thePort->usePen(activeFlatBackgroundPen);
		}
		else {
			inactiveFlatBackgroundPen->width(borderThicknessX - 2);
			thePort->usePen(inactiveFlatBackgroundPen);
		}
		theRectangle->set(CornerDim,
						  borderThicknessX / 2,
						  borderThicknessY / 2,
						  w - borderThicknessX + 1,
						  h - borderThicknessY + 1);
		thePort->frameRegion(theRectangle);
	}

	if (close_port) {
		thePort->close();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::paintTitle(int w, int h)
{
	if (!drawTitleBar) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	VRegion *clipregion;

	if (drawCloseBox) {
		if (closeBoxRightJustified) {
			theRectangle->set(CornerDim,
							  w - borderThicknessX -
							  	  closeBoxSpacingX - closeBoxWidth - 1,
							  borderThicknessY + closeBoxSpacingY,
							  closeBoxWidth + 1,
							  closeBoxHeight);
			VRectangle r(CornerDim, 0, 0, w, h);
			clipregion = r.combine(theRectangle, RgnDifference);
		}
		else {
			theRectangle->set(CornerDim,
							  borderThicknessX + closeBoxSpacingY,
							  borderThicknessY + closeBoxSpacingY,
							  closeBoxWidth + 1,
							  closeBoxHeight);
			VRectangle r(CornerDim, 0, 0, w, h);
			clipregion = r.combine(theRectangle, RgnDifference);
		}
		thePort->setClipRgn(clipregion);
	}

	theRectangle->set(CornerDim,
					  borderThicknessX, borderThicknessY,
					  w - borderThicknessX * 2, titleBarHeight);
	if (hasFocus) {
		thePort->useBrush(activeBackgroundBrush);
	}
	else {
		thePort->useBrush(inactiveBackgroundBrush);
	}
	thePort->fillRegion(theRectangle, FALSE);

	if (theTitle != 0) {
		if (hasFocus) {
			thePort->usePen(activeForegroundPen);
		}
		else {
			thePort->usePen(inactiveForegroundPen);
		}
		if (smallTitleBar) {
			thePort->useFont(halfCaptionFont);
		}
		else {
			thePort->useFont(captionFont);
		}
		const char *title_s = theTitle->gets();
		int title_w, rect_w, title_x, dummy;
		if (closeBoxRightJustified) {
			title_x = borderThicknessX + titleSpacing;
		}
		else {
			title_x = borderThicknessX +
					  closeBoxWidth + closeBoxSpacingX+ titleSpacing;
		}
		thePort->textSize(title_s, &title_w, &dummy);
		rect_w = w - borderThicknessX * 2 -
					 closeBoxWidth - closeBoxSpacingX * 2 -
					 titleSpacing * 2;
		int wrttext_style;
		if (styleThreeDee) {
			wrttext_style = SingleLine | JustifyVcenter;
		}
		else {
			wrttext_style = SingleLine | JustifyVcenter | JustifyCenter;
		}
		if (title_w > rect_w) {
			static const char etc[] = "..."; int etc_w;
			thePort->textSize(etc, &etc_w, &dummy);
			rect_w -= etc_w + 2;
			theRectangle->set(CornerDim,
							  title_x,
							  borderThicknessY,
							  rect_w, titleBarHeight);
			thePort->wrtText(title_s, theRectangle, wrttext_style);
			theRectangle->set(CornerDim,
							  title_x + rect_w,
							  borderThicknessY,
							  etc_w, titleBarHeight);
			thePort->wrtText(etc, theRectangle, wrttext_style);

		}
		else {
			theRectangle->set(CornerDim,
							  title_x, borderThicknessY,
							  rect_w, titleBarHeight);
			thePort->wrtText(title_s, theRectangle, wrttext_style);
		}
	}

	if (drawCloseBox) {
		thePort->setClipRgn(0);
		delete clipregion;
	}

	if (close_port) {
		thePort->close();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::paintCloseBox(int w, int h)
{
	if (!drawCloseBox) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Paint the close-box border shading.

	if (styleThreeDee) {

		if (closeBoxPressed) {
			thePort->usePen(shadedDarkShadowPen);
		}
		else {
			thePort->usePen(shadedHighlightPen);
		}
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth - 1,
						borderThicknessY + closeBoxSpacingY);
		thePort->lineTo(w - borderThicknessX - closeBoxSpacingX - 1,
						borderThicknessY + closeBoxSpacingY);
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth - 1,
						borderThicknessY + closeBoxSpacingY + 1);
		thePort->lineTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth - 1,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 1);
	
		if (closeBoxPressed) {
			thePort->usePen(shadedHighlightPen);
		}
		else {
			thePort->usePen(shadedDarkShadowPen);
		}
		thePort->moveTo(w - borderThicknessX - closeBoxSpacingX - 1,
						borderThicknessY + closeBoxSpacingY);
		thePort->lineTo(w - borderThicknessX - closeBoxSpacingX - 1,
						borderThicknessY + closeBoxSpacingY + closeBoxHeight);
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth - 1,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 1);
		thePort->lineTo(w - borderThicknessX - closeBoxSpacingX - 1,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 1);
	
		if (closeBoxPressed) {
			thePort->usePen(shadedShadowPen);
		}
		else {
			thePort->usePen(shadedDarkHighlightPen);
		}
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth,
						borderThicknessY + closeBoxSpacingY + 1);
		thePort->lineTo(w - borderThicknessX - closeBoxSpacingX - 2,
						borderThicknessY + closeBoxSpacingY + 1);
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth,
						borderThicknessY + closeBoxSpacingY + 2);
		thePort->lineTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 2);
	
		if (closeBoxPressed) {
			thePort->usePen(shadedDarkHighlightPen);
		}
		else {
			thePort->usePen(shadedShadowPen);
		}
		thePort->moveTo(w - borderThicknessX - closeBoxSpacingX - 2,
						borderThicknessY + closeBoxSpacingY + 1);
		thePort->lineTo(w - borderThicknessX - closeBoxSpacingX - 2,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 1);
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX - closeBoxWidth,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 2);
		thePort->lineTo(w - borderThicknessX - closeBoxSpacingX - 2,
						borderThicknessY +
						closeBoxSpacingY + closeBoxHeight - 2);
	
		// Paint the close-box background.
	
		thePort->useBrush(shadedBackgroundBrush);
		theRectangle->set(CornerDim,
						  w - borderThicknessX -
						  	  closeBoxSpacingX - closeBoxWidth + 1,
						  borderThicknessY + 4,
						  closeBoxWidth - 3,
						  closeBoxHeight - 4);
		thePort->fillRegion(theRectangle, FALSE);
	
		// Paint the close-box ``X''.
	
	
		int offset;

		if (closeBoxPressed) {
			offset = 1;
		}
		else {
			offset = 0;
		}

		thePort->usePen(shadedForegroundPen);

		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX -
							closeBoxWidth + 3 + offset,
						borderThicknessY +
						closeBoxSpacingY + 3 + offset);
		thePort->lineTo(w - borderThicknessX -
							closeBoxSpacingX - 4 + offset,
						borderThicknessY +
						closeBoxSpacingY +
						closeBoxHeight - 4 + offset);
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX -
							closeBoxWidth + 3 + offset,
						borderThicknessY +
						closeBoxSpacingY +
						closeBoxHeight - 5 + offset);
		thePort->lineTo(w - borderThicknessX -
							closeBoxSpacingX - 4 + offset,
						borderThicknessY +
						closeBoxSpacingY + 2 + offset);
	
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX -
							closeBoxWidth + 4 + offset,
							borderThicknessY +
							closeBoxSpacingY + 3 + offset);
		thePort->lineTo(w - borderThicknessX -
							closeBoxSpacingX - 5 + offset,
						borderThicknessY +
						closeBoxSpacingY +
						closeBoxHeight - 4 + offset);
		thePort->moveTo(w - borderThicknessX -
							closeBoxSpacingX -
							closeBoxWidth + 4 + offset,
						borderThicknessY +
						closeBoxSpacingY +
						closeBoxHeight - 5 + offset);
		thePort->lineTo(w - borderThicknessX -
							closeBoxSpacingX - 5 + offset,
						borderThicknessY +
						closeBoxSpacingY + 2 + offset);
	}
	else {
		thePort->useBrush(shadedBackgroundBrush);
		theRectangle->set(CornerDim,
						  borderThicknessX,
						  borderThicknessY,
						  closeBoxWidth + 1,
						  closeBoxHeight);
		thePort->fillRegion(theRectangle, FALSE);
		if (hasFocus) {
			thePort->usePen(activeFlatBorderPen);
		}
		else {
			thePort->usePen(inactiveFlatBorderPen);
		}
		thePort->moveTo(borderThicknessX + closeBoxWidth + 1,
						borderThicknessY);
		thePort->lineTo(borderThicknessX + closeBoxWidth + 1,
						borderThicknessY + titleBarHeight);
		theRectangle->set(CornerDim,
						  borderThicknessX + 2,
						  borderThicknessY + closeBoxHeight / 2 - 1,
						  closeBoxWidth - 3, 3);
		thePort->frameRegion(theRectangle);
		thePort->usePen(flatBackgroundPen);
		thePort->moveTo(borderThicknessX + 3,
						borderThicknessY + closeBoxHeight / 2);
		thePort->lineTo(borderThicknessX + closeBoxWidth - 2,
						borderThicknessY + closeBoxHeight / 2);
		thePort->usePen(shadedShadowPen);
		thePort->moveTo(borderThicknessX + 3,
						borderThicknessY + closeBoxHeight / 2 + 2);
		thePort->lineTo(borderThicknessX + closeBoxWidth,
						borderThicknessY + closeBoxHeight / 2 + 2);
		thePort->moveTo(borderThicknessX + closeBoxWidth - 1,
						borderThicknessY + closeBoxHeight / 2);
		thePort->lineTo(borderThicknessX + closeBoxWidth - 1,
						borderThicknessY + closeBoxHeight / 2 + 2);

		if (closeBoxPressed) {
			theRectangle->set(CornerDim,
							  borderThicknessX, borderThicknessY,
							  closeBoxWidth + 1, closeBoxHeight);
			thePort->invertRegion(theRectangle);
		}
	}

	if (close_port) {
		thePort->close();
	}
}

#elif defined(CV_MOTIF)

// ### Motif

void VUtilityWindow::paintBorder(int w, int h)
{
	if (!drawBorder) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Do the real work!!!

	if (close_port) {
		thePort->close();
	}
}

void VUtilityWindow::paintTitle(int w, int h)
{
	if (!drawTitleBar) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Do the real work!!!

	if (close_port) {
		thePort->close();
	}
}

void VUtilityWindow::paintCloseBox(int w, int h)
{
	if (!drawCloseBox) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Do the real work!!!

	if (close_port) {
		thePort->close();
	}
}

#elif defined(PM_DAVE)

// ### OS/2

void VUtilityWindow::paintBorder(int w, int h)
{
	if (!drawBorder) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Do the real work!!!

	if (close_port) {
		thePort->close();
	}
}

void VUtilityWindow::paintTitle(int w, int h)
{
	if (!drawTitleBar) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Do the real work!!!

	if (close_port) {
		thePort->close();
	}
}

void VUtilityWindow::paintCloseBox(int w, int h)
{
	if (!drawCloseBox) {
		return;
	}

	boolean close_port;

	if (w == 0) {
		if ((thePort == 0) || !thePort->openOn(this)) {
			return;
		}
		sizeOfImage(&w, &h);
		close_port = TRUE;
	}
	else {
		close_port = FALSE;
	}

	// Do the real work!!!

	if (close_port) {
		thePort->close();
	}
}

#else

#error "Not implemented: VUtilityeWindow::paintXXX()!"

#endif

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::activate(VView *)
{
	noteFocus(TRUE);
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::deactivate(VView *)
{
	noteFocus(FALSE);
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::givenFocus()
{
	noteFocus(TRUE);
	return VPopupWindow::givenFocus();
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::clearFocus()
{
//	noteFocus(FALSE);
	return VPopupWindow::clearFocus();
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::key(int i, char c)
{
	return VPopupWindow::key(i, c);
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::mouseEnter(int)
{
	if (!isEnabled()) {
		return FALSE;
	}
	mouseOverhead = 1;
	if (!closeBoxArmed) {
		notifier->mouseTracking(TRUE);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::mouseExit(int)
{
	if (!isEnabled()) {
		return FALSE;
	}
	mouseOverhead = 0;
	if (closeBoxArmed) {
		if (closeBoxPressed) {
			closeBoxPressed = 0;
			paintCloseBox();
		}
	}
	else {
		notifier->mouseTracking(FALSE);
		if (currentCursor != 0) {
			setCursor(currentCursor = 0);
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::mouseDn(int x, int y)
{
	if (!isEnabled()) {
		return FALSE;
	}

	noteFocus(TRUE);

	int hit, this_x, this_y, this_w, this_h;

	getAbs(&this_x, &this_y ,&this_w, &this_h);

	switch (hit = hitTest(x, y, TRUE)) {
	case TitleBar:
		if (titleBarMoving) {
			mapToScreen(&x, &y);
			moveWithRubberBand(this_x, this_y, this_w, this_h, x, y);
		}
		break;
	case ClientArea:
		if (clientAreaMoving) {
			mapToScreen(&x, &y);
			moveWithRubberBand(this_x, this_y, this_w, this_h, x, y);
		}
		break;
#ifndef CV_REMOTE_GUI
	case Left:
	case Right:
	case Top:
	case Bottom:
	case TopLeft:
	case TopRight:
	case BottomLeft:
	case BottomRight:
		mapToScreen(&x, &y);
		resizeWithRubberBand(this_x, this_y, this_w, this_h, x, y, hit);
		break;
#endif
	case CloseBox:
		armCloseBox(TRUE);
		break;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::mouseUp(int x, int y)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if (closeBoxArmed) {
		armCloseBox(FALSE);
		if (hitTest(x, y) == CloseBox) {
			activateCloseBox();
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::mouseDbl(int x, int y)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if ((clientObject != 0) && (mouseDblMethod != 0)) {
		return (clientObject->*mouseDblMethod)(x, y);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::rmouseDn(int x, int y)
{
	if (!isEnabled()) {
		return FALSE;
	}
	if ((clientObject != 0) && (rmouseDnMethod != 0)) {
		return (clientObject->*rmouseDnMethod)(x, y);
	}
	else if (!configurationMenuDisabled && !closeBoxArmed) {
		if (configurationMenu == 0) {
			configurationMenu = new ConfigurationMenu;
		}
		configurationMenu->popup(x, y, this);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::mouseMv(int x, int y, int)
{
	if (!isEnabled()) {
		return FALSE;
	}
#ifndef CV_REMOTE_GUI
	if (VRubberBand::getCurrent() != 0) {
		return TRUE;
	}
#endif
	if (closeBoxArmed) {
		if (hitTest(x, y) == CloseBox) {
			if (!closeBoxPressed) {
				closeBoxPressed = 1;
				paintCloseBox();
			}
		}
		else if (closeBoxPressed) {
			closeBoxPressed = 0;
			paintCloseBox();
		}
	}
	else {
		hitTest(x, y, TRUE);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::noteFocus(boolean b)
{
	if (b) {
		if (!hasFocus) {
			hasFocus = 1;
			paintTitleBar();
		}
	}
	else if (hasFocus) {
		hasFocus = 0;
		paintTitleBar();
	}
}

// --------------------------------------------------------------------------
//
int VUtilityWindow::hitTest(int x, int y, boolean set_cursor)
{
#ifndef CV_NO_MOUSE_EVENTS
	int w, h;

	sizeOfImage(&w, &h);

	if (y < borderThicknessY) {
		if (!preferredResizing) {
			if (x <= resizeCornerLength) {
				if (set_cursor && (currentCursor != topLeftCursor)) {
					setCursor(currentCursor = topLeftCursor);
				}
				return TopLeft;
			}
			else if (x >= w - resizeCornerLength) {
				if (set_cursor && (currentCursor != topRightCursor)) {
					setCursor(currentCursor = topRightCursor);
				}
				return TopRight;
			}
		}
		if (set_cursor && (currentCursor != topCursor)) {
			setCursor(currentCursor = topCursor);
		}
		return Top;
	}
	else if (y >= h - borderThicknessY) {
		if (!preferredResizing) {
			if (x <= resizeCornerLength) {
				if (set_cursor && (currentCursor != bottomLeftCursor)) {
					setCursor(currentCursor = bottomLeftCursor);
				}
				return BottomLeft;
			}
			else if (x >= w - resizeCornerLength) {
				if (set_cursor && (currentCursor != bottomRightCursor)) {
					setCursor(currentCursor = bottomRightCursor);
				}
				return BottomRight;
			}
		}
		if (set_cursor && (currentCursor != bottomCursor)) {
			setCursor(currentCursor = bottomCursor);
		}
		return Bottom;
	}
	else if (x <= borderThicknessX) {
		if (!preferredResizing) {
			if (y <= resizeCornerLength) {
				if (set_cursor && (currentCursor != topLeftCursor)) {
					setCursor(currentCursor = topLeftCursor);
				}
				return TopLeft;
			}
			else if (y >= h - resizeCornerLength) {
				if (set_cursor && (currentCursor != bottomLeftCursor)) {
					setCursor(currentCursor = bottomLeftCursor);
				}
				return BottomLeft;
			}
		}
		if (set_cursor && (currentCursor != leftCursor)) {
			setCursor(currentCursor = leftCursor);
		}
		return Left;
	}
	else if (x >= w - borderThicknessX) {
		if (!preferredResizing) {
			if (y <= resizeCornerLength) {
				if (set_cursor && (currentCursor != topRightCursor)) {
					setCursor(currentCursor = topRightCursor);
				}
				return TopRight;
			}
			else if (y >= h - resizeCornerLength) {
				if (set_cursor && (currentCursor != bottomRightCursor)) {
					setCursor(currentCursor = bottomRightCursor);
				}
				return BottomRight;
			}
		}
		if (set_cursor && (currentCursor != rightCursor)) {
			setCursor(currentCursor = rightCursor);
		}
		return Right;
	}
	else if (drawCloseBox) {
		if (closeBoxRightJustified) {
			if ((x >= w - borderThicknessX -
						  closeBoxSpacingX - closeBoxWidth) &&
				(x <= w - borderThicknessX - closeBoxSpacingX) &&
				(y >= borderThicknessY + closeBoxSpacingY) &&
				(y <= borderThicknessY +
					  closeBoxSpacingY + closeBoxHeight)) {
				if (set_cursor && (currentCursor != 0)) {
					setCursor(currentCursor = 0);
				}
				return CloseBox;
			}
		}
		else if ((x <= borderThicknessX + closeBoxWidth) &&
				 (y <= borderThicknessY + closeBoxHeight)) {
			if (set_cursor && (currentCursor != 0)) {
				setCursor(currentCursor = 0);
			}
			return CloseBox;
		}
	}
	if (set_cursor && (currentCursor != 0)) {
		setCursor(currentCursor = 0);
	}
#endif
	return TitleBar;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::armCloseBox(boolean b)
{
	if (b) {
		if (!closeBoxArmed) {
			bringToTop();
			notifier->yieldPaint(this);
			notifier->captureMouseFor(this);
			notifier->mouseTracking(TRUE);
			closeBoxArmed = 1;
			closeBoxPressed = 1;
			paintCloseBox();
		}
	}
	else if (closeBoxArmed) {
		if (!mouseOverhead) {
			notifier->mouseTracking(FALSE);
		}
		notifier->releaseCapture();
		closeBoxArmed = 0;
		closeBoxPressed = 0;
		paintCloseBox();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::activateCloseBox()
{
	close();
	if (deleteOnClose) {
		delete this;
	}
	else if (hideOnClose) {
		hide();
	}
}

// --------------------------------------------------------------------------
//
int VUtilityWindow::moveWithRubberBand(int& x, int& y, int& w, int& h,
									   int& mx, int& my)
{
#ifndef CV_REMOTE_GUI
	if (noMoving || !beginMoving() || !moving(x, y, w, h, mx, my)) {
		return VRubberBand::NeverStarted;
	}

	int this_x, this_y, this_w, this_h;

	bringToTop();
	notifier->yieldPaint(this);
	VRubberBand rubberband;
	rubberband.confineToWindow(confinedToScreen);
	rubberband.uponMove(this, methodOf(VUtilityWindow,
									   moveRubberBandCallback));
	initializeOpaqueMoveResize();
	if (opaqueMoving) {
		lastRubberBandX = x;
		lastRubberBandY = y;
		lastRubberBandW = w;
		lastRubberBandH = h;
		getAbs(&this_x, &this_y ,&this_w, &this_h);
		rubberband.disableDrawing(TRUE);
	}
	int status = rubberband.move(x, y, w, h, mx, my, this);
	if (endMoving(x, y, w, h, mx, my, status) && (status > 0)) {
		setGeometryPosition(x, y);
	}
	else if (opaqueMoving) {
		setGeometry(this_x, this_y, this_w, this_h);
	}

	return status;
#else
	return 0;
#endif
}

// --------------------------------------------------------------------------
//
int VUtilityWindow::resizeWithRubberBand(int& x, int& y, int& w, int& h,
										 int& mx, int& my, int resize_area)
{
#ifndef CV_REMOTE_GUI
	if (noResizing || !beginResizing() ||
					  !resizing(x, y, w, h, resize_area)) {
		return VRubberBand::NeverStarted;
	}

	int this_x, this_y, this_w, this_h;

	bringToTop();
	notifier->yieldPaint(this);
	VRubberBand rubberband;
	rubberband.confineToWindow(confinedToScreen);
	rubberband.uponResize(this, methodOf(VUtilityWindow,
										 resizeRubberBandCallback));
	initializeOpaqueMoveResize();
	if (opaqueResizing) {
		lastRubberBandX = x;
		lastRubberBandY = y;
		lastRubberBandW = w;
		lastRubberBandH = h;
		getAbs(&this_x, &this_y ,&this_w, &this_h);
		rubberband.disableDrawing(TRUE);
	}
	int status = rubberband.resize(resize_area, x, y, w, h, mx, my, this);
	if (endResizing(x, y, w, h, resize_area, status) && (status > 0)) {
		if (preferredResizing) {
			int preferred_w, preferred_h;
			determineConstrainedSize(&preferred_w, &preferred_h, w, 0);
			setGeometry(x, y, preferred_w, preferred_h);
		}
		else {
			setGeometry(x, y, w, h);
		}
	}
	else if (opaqueResizing) {
		setGeometry(this_x, this_y, this_w, this_h);
	}

	return status;
#else
	return 0;
#endif
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::moveRubberBandCallback(int& x, int& y,
											   int& w, int& h,
											   int mx, int my)
{
	boolean ok = moving(x, y, w, h, mx, my);

	if (opaqueMoving && ok) {
		if ((x != lastRubberBandX) || (y != lastRubberBandY) ||
			(w != lastRubberBandW) || (h != lastRubberBandH)) {
			if ((w > 0) && (h > 0)) {
				setGeometry(x, y, w, h);
			}
			else {
				setGeometryPosition(x, y);
			}
			notifier->yieldPaint();
			lastRubberBandX = x;
			lastRubberBandY = y;
			lastRubberBandW = w;
			lastRubberBandH = h;
		}
	}

	return ok;
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::resizeRubberBandCallback(int x, int y,
												 int& w, int& h,
												 int mode)
{
	boolean changed = TRUE;

	if (preferredResizing) {
#ifndef CV_REMOTE_GUI
		int preferred_w, preferred_h;
		switch (mode) {
		case VRubberBand::ResizeLeft:
		case VRubberBand::ResizeRight:
			determineConstrainedSize(&preferred_w, &preferred_h, w, 0);
			changed = w != preferred_w || h != preferred_h;
			w = preferred_w;
			h = preferred_h;
			break;
		case VRubberBand::ResizeTop:
		case VRubberBand::ResizeBottom:
			changed = w != preferred_w || h != preferred_h;
			determineConstrainedSize(&preferred_w, &preferred_h, 0, h);
			w = preferred_w;
			h = preferred_h;
			break;
		}
#endif
	}
	else {
		if (w < minimumWidth) {
			w = minimumWidth;
		}
		if (h < minimumHeight) {
			h = minimumHeight;
		}
	}

	boolean ok = resizing(x, y, w, h, mode);

	if (opaqueResizing && ok) {
		if ((x != lastRubberBandX) || (y != lastRubberBandY) ||
			(w != lastRubberBandW) || (h != lastRubberBandH)) {
#ifndef CV_REMOTE_GUI
			switch (mode) {
				case VRubberBand::ResizeLeft:
					x = lastRubberBandX + lastRubberBandW - w;
					break;
				case VRubberBand::ResizeTop:
					y = lastRubberBandY + lastRubberBandH - h;
					break;
				case VRubberBand::ResizeTopLeft:
					x = lastRubberBandX + lastRubberBandW - w;
					y = lastRubberBandY + lastRubberBandH - h;
					break;
				case VRubberBand::ResizeTopRight:
					y = lastRubberBandY + lastRubberBandH - h;
					break;
				case VRubberBand::ResizeBottomLeft:
					x = lastRubberBandX + lastRubberBandW - w;
					break;
			}
#endif
			setGeometry(x, y, w, h);
			notifier->yieldPaint();
			lastRubberBandX = x;
			lastRubberBandY = y;
			lastRubberBandW = w;
			lastRubberBandH = h;
		}
	}

	return ok;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::Manager::getPreferredSize(int *w, int *h)
{
	VUtilityWindow *utility_window = (VUtilityWindow *)getWindow();
	if (utility_window != 0) {
		VWindow *client_window = utility_window->getClientAreaWindow();
		if (client_window != 0) {
			VGeoManager *client_manager = client_window->getGeoManager();
			if (client_manager != 0) {
				client_manager->getPreferredSize(w, h);
				addNonClientAreaSize(w, h);
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::Manager::getMinimumSize(int *w, int *h)
{
	VUtilityWindow *utility_window = (VUtilityWindow *)getWindow();
	if (utility_window != 0) {
		VWindow *client_window = utility_window->getClientAreaWindow();
		if (client_window != 0) {
			VGeoManager *client_manager = client_window->getGeoManager();
			if (client_manager != 0) {
				client_manager->getMinimumSize(w, h);
				addNonClientAreaSize(w, h);
			}
		}
	}
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::Manager::getConstrainedSize(int *w, int *h,
													int constrained_w,
													int constrained_h)
{
	VUtilityWindow *utility_window = (VUtilityWindow *)getWindow();
	if (utility_window != 0) {
		VWindow *client_window = utility_window->getClientAreaWindow();
		if (client_window != 0) {
			VGeoManager *client_manager = client_window->getGeoManager();
			if (client_manager != 0) {
				if (constrained_w > 0) {
					constrained_w -= (utility_window->borderThicknessX * 2);
					if (constrained_w <= 0) { constrained_w = 1; }
				}
				if (constrained_h > 0) {
					constrained_h -= (utility_window->borderThicknessX * 2 +
									  utility_window->getTitleBarHeight());
					if (constrained_h <= 0) { constrained_h = 1; }
				}
				if (client_manager->getConstrainedSize
					(w, h, constrained_w, constrained_h)) {
					addNonClientAreaSize(w, h);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::Manager::frameChildren(int w, int h)
{
	VUtilityWindow *utility_window = (VUtilityWindow *)getWindow();
	if (utility_window != 0) {
		VWindow *client_window = utility_window->getClientAreaWindow();
		if (client_window != 0) {
			client_window->setGeometry
				(utility_window->borderThicknessX,
				 utility_window->borderThicknessY +
				 utility_window->getTitleBarHeight(),
				 w - (utility_window->borderThicknessX * 2),
				 h - (utility_window->borderThicknessY * 2 +
				 	  utility_window->getTitleBarHeight()));
		}
	}
}

// --------------------------------------------------------------------------
//
VUtilityWindow::ClientWindow::ClientWindow(VWindow *parentwin)
	: VWindow(VFrame(), parentwin, StyleNoBorder)
{
	setBackground(&VColorScheme::systemBrush(VColorScheme::ShadedBackground));
}

// --------------------------------------------------------------------------
//
VUtilityWindow::ClientWindow::~ClientWindow()
{
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::ClientWindow::mouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	mapToScreen(&mx, &my);
	VUtilityWindow *utility_window = (VUtilityWindow *)getParent();
	utility_window->mapToWin(&mx, &my);
	return utility_window->mouseDn(mx, my);
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::ClientWindow::mouseDbl(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	mapToScreen(&mx, &my);
	VUtilityWindow *utility_window = (VUtilityWindow *)getParent();
	utility_window->mapToWin(&mx, &my);
	return utility_window->mouseDbl(mx, my);
}

// --------------------------------------------------------------------------
//
boolean VUtilityWindow::ClientWindow::rmouseDn(int mx, int my)
{
	if (!isEnabled()) {
		return FALSE;
	}
	mapToScreen(&mx, &my);
	VUtilityWindow *utility_window = (VUtilityWindow *)getParent();
	utility_window->mapToWin(&mx, &my);
	return utility_window->rmouseDn(mx, my);
}

// --------------------------------------------------------------------------
//
VUtilityWindow::ConfigurationMenu::ConfigurationMenu()
	: VPopupMenu(VMenuData("", "",
						   methodOf(ConfigurationMenu, poppedUp), this))
{
	utilityWindow = 0;

	addMenuItem(VMenuData
		("&Move", "",
		 methodOf(ConfigurationMenu, setMove), this, 1));
	addMenuItem(VMenuData
		("&Size", "",
		 methodOf(ConfigurationMenu, setSize), this, 2));
	addMenuItem(VMenuData
		("&Close", "",
		 methodOf(ConfigurationMenu, setClose), this, 9));
	addMenuItem(VMenuData());

	VPopupMenu *popup1 = new VPopupMenu
		(VMenuData("S&tyle", "",
		 methodOf(ConfigurationMenu, poppedUpStyle), this));
	popup1->addMenuItem(VMenuData
		("MS/Windows &95", "",
		 methodOf(ConfigurationMenu, setStyle), this, 1));
	popup1->addMenuItem(VMenuData
		("MS/Windows &3.1", "",
		 methodOf(ConfigurationMenu, setStyle), this, 2));
	popup1->addMenuItem(VMenuData
		("&OS/2", "",
		 methodOf(ConfigurationMenu, setStyle), this, 3));
	popup1->addMenuItem(VMenuData
		("OSF/&Motif", "",
		 methodOf(ConfigurationMenu, setStyle), this, 4));
	addMenuItem(VMenuData(popup1));

	VPopupMenu *popup2 = new VPopupMenu
		(VMenuData("&Opaque", "",
		 methodOf(ConfigurationMenu, poppedUpOpaque), this));
	popup2->addMenuItem(VMenuData
		("Mo&ve/Size", "",
		 methodOf(ConfigurationMenu, setOpaque), this, 1));
	popup2->addMenuItem(VMenuData
		("&Move", "",
		 methodOf(ConfigurationMenu, setOpaque), this, 2));
	popup2->addMenuItem(VMenuData
		("&Size", "",
		 methodOf(ConfigurationMenu, setOpaque), this, 3));
	popup1->addMenuItem(VMenuData());
	popup1->addMenuItem(VMenuData(popup2));
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::popup(int x, int y,
											  VUtilityWindow *utility)
{
	if (utility != 0) {
		utilityWindow = utility;
		utilityWindow->mapToScreen(&x, &y);
		popupAt(x, y);
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::poppedUp(VPopupMenu *pm)
{
	if ((pm != 0) && (utilityWindow != 0)) {
		pm->enableMenuItem(1, FALSE);
		pm->enableMenuItem(2, FALSE);
		pm->enableMenuItem(9, !(utilityWindow->getCloseBehavior() & NoClose));
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::poppedUpOpaque(VPopupMenu *pm)
{
	if ((pm != 0) && (utilityWindow != 0)) {
		pm->checkMenuItem(1, utilityWindow->isOpaqueMoving() &&
							 utilityWindow->isOpaqueResizing());
		pm->checkMenuItem(2, utilityWindow->isOpaqueMoving());
		pm->checkMenuItem(3, utilityWindow->isOpaqueResizing());
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::poppedUpStyle(VPopupMenu *pm)
{
	if (pm != 0) {
		pm->enableMenuItem(1, VUtilityWindow::supportsStyleWindows95());
		pm->enableMenuItem(2, VUtilityWindow::supportsStyleWindows31());
		pm->enableMenuItem(3, VUtilityWindow::supportsStyleOs2());
		pm->enableMenuItem(4, VUtilityWindow::supportsStyleMotif());
		pm->checkMenuItem(1, VUtilityWindow::isStyleWindows95());
		pm->checkMenuItem(2, VUtilityWindow::isStyleWindows31());
		pm->checkMenuItem(3, VUtilityWindow::isStyleOs2());
		pm->checkMenuItem(4, VUtilityWindow::isStyleMotif());
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::setMove(VMenuItem *)
{
	// Not yet implemented.
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::setSize(VMenuItem *)
{
	// Not yet implemented.
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::setClose(VMenuItem *)
{
	if (utilityWindow != 0) {
		utilityWindow->activateCloseBox();
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::setOpaque(VMenuItem *mi)
{
	if ((mi != 0) && (utilityWindow != 0)) {
		switch (mi->getIdNo()) {
		case 1:
			if (utilityWindow->isOpaqueMoving() &&
				utilityWindow->isOpaqueResizing()) {
				utilityWindow->setOpaqueMoving(FALSE);
				utilityWindow->setOpaqueResizing(FALSE);
			}
			else {
				utilityWindow->setOpaqueMoving(TRUE);
				utilityWindow->setOpaqueResizing(TRUE);
			}
			break;
		case 2:
			utilityWindow->setOpaqueMoving
				(!utilityWindow->isOpaqueMoving());
			break;
		case 3:
			utilityWindow->setOpaqueResizing
				(!utilityWindow->isOpaqueResizing());
			break;
		}
	}
}

// --------------------------------------------------------------------------
//
void VUtilityWindow::ConfigurationMenu::setStyle(VMenuItem *mi)
{
	if ((mi != 0) && (utilityWindow != 0)) {
		switch (mi->getIdNo()) {
		case 1:
			VUtilityWindow::setStyleWindows95();
			break;
		case 2:
			VUtilityWindow::setStyleWindows31();
			break;
		case 3:
			VUtilityWindow::setStyleOs2();
			break;
		case 4:
			VUtilityWindow::setStyleMotif();
			break;
		}
	}
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VUtilityWindow::putTo(VArchiver& a)
{
	VUtilityWindow::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VUtilityWindow::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VPopupWindow::putTo(a, FALSE);

	// Write the object version tag.

	a << VUtilityWindowArchiveTag;

	// Write any object specific data.

    a.putObject(notifier->
				getClientName(clientObject, clientObjectIndex), TRUE);

    const char *class_name = 0, *method_name = 0;
    VCallbackList::findCallback(method(beginResizingMethod),
								class_name, method_name,
								beginResizingMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(resizingMethod),
								class_name, method_name,
								resizingMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(endResizingMethod),
								class_name, method_name,
								endResizingMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(beginMovingMethod),
								class_name, method_name,
								beginMovingMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(movingMethod),
								class_name, method_name,
								movingMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(endMovingMethod),
								class_name, method_name,
								endMovingMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(mouseDblMethod),
								class_name, method_name,
								mouseDblMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(rmouseDnMethod),
								class_name, method_name,
								rmouseDnMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

    class_name = 0, method_name = 0;
    VCallbackList::findCallback(method(closeMethod),
								class_name, method_name,
								closeMethodIndex);
    a << (char *)class_name;
    a << (char *)method_name;

	if (theTitle == 0) {
		a << char(1);
	}
	else {
		a << char(1);
		a.putObject(theTitle);
	}

	a << char(drawTitleBar);
	a << char(drawCloseBox);
	a << char(drawBorder);
	a << char(clientAreaMoving);
	a << char(titleBarMoving);
	a << char(noMoving);
	a << char(noResizing);
	a << char(preferredResizing);
	a << char(deleteOnClose);
	a << char(hideOnClose);
	a << char(confinedToScreen);
	a << char(smallTitleBar);
	a << char(opaqueResizing);
	a << char(opaqueMoving);
	a << char(decorationsDisabled);
	a << char(configurationMenuDisabled);

	// Write any children.

	if (most_derived) {
		VWindow::putChildrenTo(a);
	}
}

// ---------------------------------------------------------------------------
//
void VUtilityWindow::getFrom(VArchiver& a)
{
	// Read the base object data.

	VPopupWindow::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VUtilityWindowArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VUtilityWindowArchiveTag));
		return;
	}

	// Read any object specific data.

    VString client_name;
    a.getObject(&client_name);
    clientObject = notifier->getClient(client_name, &clientObjectIndex);

    char method_name[256], *method_name_ptr = method_name;
    char class_name[256], *class_name_ptr = class_name;
	method m;

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&beginResizingMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponBeginResizing(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&resizingMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponResizing(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&endResizingMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponEndResizing(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&beginMovingMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponBeginMoving(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&movingMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponMoving(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&endMovingMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponEndMoving(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&mouseDblMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponMouseDbl(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&rmouseDnMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponRmouseDn(clientObject, m);
    }

    a >> class_name_ptr;
    a >> method_name_ptr;
    m = VCallbackList::findCallback(class_name_ptr, method_name_ptr,
									&closeMethodIndex);
    if ((clientObject != 0) && (m != 0)) {
        if (!clientObject->isA(VClass::of(class_name_ptr))) {
            a.abort(VArchiver::ClientCallbackMismatch); return;
        }
		uponClose(clientObject, m);
    }

	char c;
	a >> c;

	if (c != 0) {
		theTitle = (VString *)a.getObject(0);
	}

	a >> c; drawTitleBar				= c;
	a >> c; drawCloseBox				= c;
	a >> c; drawBorder					= c;
	a >> c; clientAreaMoving			= c;
	a >> c; titleBarMoving				= c;
	a >> c; noMoving					= c;
	a >> c; noResizing					= c;
	a >> c; preferredResizing			= c;
	a >> c; deleteOnClose				= c;
	a >> c; hideOnClose					= c;
	a >> c; confinedToScreen			= c;
	a >> c; smallTitleBar				= c;
	a >> c; opaqueResizing				= c;
	a >> c; opaqueMoving				= c;
	a >> c; decorationsDisabled			= c;
	a >> c; configurationMenuDisabled	= c;
}

// ---------------------------------------------------------------------------
//
void VUtilityWindow::getFrom(VArchiver& a,
							 VObject *parentwin, boolean do_realize)
{
	VUtilityWindow::getFrom(a);

	if (do_realize) {
		VUtilityWindow::realize((VWindow *)parentwin);
	}

	if (parentwin != 0) {
		VWindow::getChildrenFrom(a, do_realize);
	}
}

// ---------------------------------------------------------------------------
//
void VUtilityWindow::realize(VWindow *parentwin)
{
	VPopupWindow::realize(parentwin);
	constructStaticData();
	setupMetrics();
}

#endif // !CV_NOARCHIVER

