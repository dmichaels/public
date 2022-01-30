// $Header:   Q:/views/common/vcs/rband.cpv   1.19   02 Apr 1997 12:21:26   GREGM  $
//	rband.cpp
//
//	VRubberBand implementation [Common]
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
//	11/01/95 dgm	Original.
//	11/03/95 dgm	Added support for key-up notification.
//	12/07/95 dgm	Added disableDrawing() support.
//	12/08/95 dgm	Added setWindow(); Motif changes; integrated PM changes.
//	12/14/95 dgm	Fixed mouse-y parameter for move-callback for OS/2.
//	03/06/96 dgm	Added undraw().
//	05/23/96 dgm	Added moveLine().
//	08/25/96 dgm	Made getCurrent() non-inline - trouble with 16-bit DLL.
//	09/13/96 dgm	Fixed resource leak (BoundsChecker).
//	03/27/97 djs	Modified macro getNextEvent and supporting code
//					for OS/2 - #ifdef'd
// --------------------------------------------------------------------------

#include "rband.h"
#include "rect.h"
#include "mouscrsr.h"
#include "wintypes.h"
#include "notifier.h"

// --------------------------------------------------------------------------
//
#undef	abs 
#define	abs(x)	((x) < 0 ? -(x) : (x))

// --------------------------------------------------------------------------
// N.B. Assumption! Only one VRubberBand can possibly be active at a time.
//
VRubberBand		   *VRubberBand::currentRubberBand	= 0;
static int			rectangle_drawn					= 0;
static int			undraw_thickness				= 0;
static int			rubber_x						= 0;
static int			rubber_y						= 0;
static int			rubber_w						= 0;
static int			rubber_h						= 0;

// =========================================================================
// OSF/Motif
//
#if defined(CV_MOTIF)

#include "xdefs.h"
#include <X11/cursorfont.h>

// --------------------------------------------------------------------------
//
#undef	WindowingSystemHasSeparateMouseUpEvents

#define UpArrowKey					XK_Up
#define DownArrowKey				XK_Down
#define LeftArrowKey				XK_Left
#define RightArrowKey				XK_Right
#define EscapeKey					XK_Escape
#define ReturnKey					XK_Return

#define LeftMouseButton				Button1
#define RightMouseButton			Button3

#define MouseMoveEvent				MotionNotify
#define KeyDownEvent				KeyPress
#define KeyUpEvent					KeyRelease
#define MouseButtonUpEvent			ButtonRelease

#define KeyType						KeySym
#define EventType					XEvent
#define getNextEvent(e)				(XNextEvent(xdisplay, &e), e.type)
#define getTypeOfEvent(e)			(e.type)

#define mapMouseMoveEvent(e,X,Y)	if (e.xany.window != xdrawable) {		\
										Drawable child_window;				\
										XTranslateCoordinates				\
										(xdisplay,		  					\
										 e.xany.window, xdrawable,			\
										 e.xmotion.x, e.xmotion.y,			\
										 &X, &Y, &child_window);			\
									}										\
									else {									\
										X = event.xmotion.x;				\
										Y = event.xmotion.y;				\
									}

#define mapKeyEvent(e,K,C,S)		{ K = XKeycodeToKeysym					\
									      (xdisplay,						\
											e.xkey.keycode, 0);				\
									  C = e.xkey.state & ControlMask;		\
									  S = e.xkey.state & ShiftMask;			\
									}
					
// --------------------------------------------------------------------------
//
static X_Display   *xdisplay	= 0;
static Drawable		xdrawable	= 0;
static GC			xgc			= 0;

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::initializeWindow(int *w, int *h)
{
	if (xdisplay != 0) {
		if (w != 0) { *w = 0; }
		if (h != 0) { *w = 0; }
		return;
	}
	if (theWindow != 0) {
		HWND hwnd = theWindow->hwnd();
		if (hwnd != 0) {
			xdisplay = XtDisplay(hwnd);
			xdrawable = XtWindow(hwnd);
		}
		else {
			xdisplay = notifier->defaultDisplay();
			xdrawable = notifier->defaultRootWindow();
		}
		int window_w, window_h;
		theWindow->sizeOfImage(&window_w, &window_h);
		if (w != 0) {*w = window_w; }
		if (h != 0) {*h = window_h; }
	}
	else {
		xdisplay = notifier->defaultDisplay();
		xdrawable = notifier->defaultRootWindow();
		static int screen_w = 0, screen_h = 0;
		if (screen_w == 0) { notifier->getScreenSize(&screen_w, &screen_h); }
		if (w != 0) {*w = screen_w; }
		if (h != 0) {*h = screen_h; }
	}
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::deinitializeWindow()
{
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::createRubberBand()
{
	XGCValues gcv;
	gcv.foreground	   = BlackPixel(xdisplay, DefaultScreen(xdisplay)) ^
						 WhitePixel(xdisplay, DefaultScreen(xdisplay));
	gcv.function	   = GXxor;
	if (thickness > 1) {
		gcv.line_width = thickness;
	}
	else {
		gcv.line_width = 0;
	}
	gcv.cap_style	   = CapNotLast;
	gcv.join_style	   = JoinMiter;
	gcv.subwindow_mode = IncludeInferiors;
	xgc				   = XCreateGC(xdisplay, xdrawable,
								   GCForeground |
								   GCFunction |
								   GCLineWidth |
								   GCCapStyle |
								   GCJoinStyle |
								   GCSubwindowMode,
								   &gcv);
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::destroyRubberBand()
{
	if ((xdisplay != 0) && (xgc != 0)) {
		XFreeGC(xdisplay, xgc);
		XFlush(xdisplay);
	}
	xgc = 0;
	xdrawable = 0;
	xdisplay = 0;
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::drawRectangle(int x, int y, int w, int h)
{
	if (undraw_thickness > 0) {
		XGCValues gcv;
		gcv.line_width = undraw_thickness;
		XChangeGC(xdisplay, xgc, GCLineWidth, &gcv);
		XDrawRectangle(xdisplay, xdrawable, xgc, x, y, w, h);
		gcv.line_width = thickness;
		XChangeGC(xdisplay, xgc, GCLineWidth, &gcv);
		undraw_thickness = 0;
	}
	else {
		XDrawRectangle(xdisplay, xdrawable, xgc, x, y, w, h);
	}
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::drawLine(int x1, int y1, int x2, int y2)
{
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::captureMouse(VWindow *window)
{
	if (window == 0) {
		window = theWindow;
	}
	Cursor cursor = None;
	if (window != 0) {
		VMouseCursor *mouse_cursor = window->getCursor();
		if (mouse_cursor != 0) {
			cursor = mouse_cursor->getHndl();
		}
	}
	if ((xdisplay != 0) && (xdrawable != 0)) {
		XGrabPointer(xdisplay, xdrawable, False,
				     unsigned(PointerMotionMask |
							  ButtonPressMask |
							  ButtonReleaseMask),
				     GrabModeAsync, GrabModeAsync, None,
				     cursor, unsigned(CurrentTime));
	}
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::uncaptureMouse()
{
	if (xdisplay != 0) {
		XUngrabPointer(xdisplay, CurrentTime);
	}
}

// --------------------------------------------------------------------------
// OSF/Motif
//
void VRubberBand::setThickness(int t)
{
	if (t < 0) {
		t = 1;
	}
	if (rectangle_drawn) {
		undraw_thickness = thickness;
	}
	else if ((xdisplay != 0) && (xgc != 0)) {
		XGCValues gcv;
		gcv.line_width = thickness;
		XChangeGC(xdisplay, xgc, GCLineWidth, &gcv);
	}
	thickness = t;
}

// =========================================================================
// MS/Windows
//
#elif defined(CV_WINDOWS)

#include "windows.h"

// --------------------------------------------------------------------------
//
#define	WindowingSystemHasSeparateMouseUpEvents

#define UpArrowKey					VK_UP
#define DownArrowKey				VK_DOWN
#define LeftArrowKey				VK_LEFT
#define RightArrowKey				VK_RIGHT
#define EscapeKey					VK_ESCAPE
#define ReturnKey					VK_RETURN

#define MouseMoveEvent				WM_MOUSEMOVE
#define KeyDownEvent				WM_KEYDOWN
#define KeyUpEvent					WM_KEYUP
#define MouseButtonUpEvent			WM_LBUTTONUP
#define LeftMouseButtonUpEvent		WM_LBUTTONUP
#define RightMouseButtonUpEvent		WM_RBUTTONUP

#define KeyType						int
#define EventType					MSG
#define getNextEvent(m)				(GetMessage(&m, 0, 0, 0),				\
									 TranslateMessage(&m), m.message)
#define getTypeOfEvent(m)			(m.message)

#if defined(CV_WIN32)
#define mapMouseMoveEvent(m,X,Y)	{ POINTS ps; POINT p;					\
									  ps = MAKEPOINTS(m.lParam);			\
									  if (m.hwnd == hwnd) {					\
										  X = ps.x; Y = ps.y;				\
									  }										\
									  else {								\
										  p.x = ps.x; p.y = ps.y;			\
										  ClientToScreen					\
										  	(m.hwnd, LPPOINT(&p));			\
										  if (hwnd != 0) {					\
											  ScreenToClient				\
											  	(hwnd, LPPOINT(&p));		\
										  }									\
										  X = int(p.x); Y = int(p.y);		\
									  }										\
									}
#else
#define mapMouseMoveEvent(m,X,Y)	{ POINT ps; POINT p;					\
									  ps = MAKEPOINT(m.lParam);				\
									  if (m.hwnd == hwnd) {					\
										  X = ps.x; Y = ps.y;				\
									  }										\
									  else {								\
										  p.x = ps.x; p.y = ps.y;			\
										  ClientToScreen					\
										  	(m.hwnd, LPPOINT(&p));			\
										  if (hwnd != 0) {					\
											  ScreenToClient				\
											  	(hwnd, LPPOINT(&p));		\
										  }									\
										  X = int(p.x); Y = int(p.y);		\
									  }										\
									}
#endif

#define mapKeyEvent(m,K,C,S)		{ K = m.wParam;							\
									  C = GetKeyState(VK_CONTROL) &	0x8000;	\
									  S = GetKeyState(VK_SHIFT) & 0x8000;	\
									}

// --------------------------------------------------------------------------
//
static HDC		hdc					= 0;
static HBRUSH	hbrush				= 0;
static HRGN		hrgn				= 0;
static HWND		hwnd				= 0;
static boolean	hwnd_clipchildren	= FALSE;

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::initializeWindow(int *w, int *h)
{
	if (theWindow != 0) {
		hwnd = theWindow->hwnd();
		if (hwnd != 0) {
			long s = GetWindowLong(hwnd, GWL_STYLE);
			hwnd_clipchildren = (s & WS_CLIPCHILDREN) != 0;
			if (hwnd_clipchildren) {
	            s &= ~WS_CLIPCHILDREN;
	            SetWindowLong(hwnd, GWL_STYLE, s);
			}
		}
		else {
			hwnd_clipchildren = 0;
		}
		hdc = theWindow->getHndl();
		int window_w, window_h;
		theWindow->sizeOfImage(&window_w, &window_h);
		if (w != 0) {*w = window_w; }
		if (h != 0) {*h = window_h; }
	}
	else {
		hwnd = 0;
		hwnd_clipchildren = 0;
		hdc = CreateDC("DISPLAY", NULL, NULL, NULL);
		static int screen_w = 0, screen_h = 0;
		if (screen_w == 0) { notifier->getScreenSize(&screen_w, &screen_h); }
		if (w != 0) {*w = screen_w; }
		if (h != 0) {*h = screen_h; }
	}
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::deinitializeWindow()
{
	if (hwnd_clipchildren) {
		long s = GetWindowLong(hwnd, GWL_STYLE);
		s |= WS_CLIPCHILDREN;
		SetWindowLong(hwnd, GWL_STYLE, s);
		hwnd_clipchildren = FALSE;
	}
	hwnd	= 0;
	hdc		= 0;
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::createRubberBand()
{
	static unsigned char stipple_bitmap_bits[] = { 0xAA, 0,
												   0x55, 0,
												   0xAA, 0,
												   0x55, 0,
												   0xAA, 0,
												   0x55, 0,
												   0xAA, 0,
												   0x55, 0 };

	HBITMAP hbitmap = CreateBitmap(8, 8, 1, 1, stipple_bitmap_bits);
	hbrush = CreatePatternBrush(hbitmap);
	DeleteObject(hbitmap);
	SetROP2(hdc, R2_XORPEN);
	SelectObject(hdc, hbrush);
	hrgn = CreateRectRgn(0, 0, 0, 0);
}					

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::destroyRubberBand()
{
	if (hbrush != 0) {
		DeleteObject(HGDIOBJ(hbrush));
		hbrush = 0;
	}
	if (hrgn != 0) {
		DeleteObject(HGDIOBJ(hrgn));
		hrgn = 0;
	}
	if (theWindow != 0) {
		theWindow->ungetHndl();
	}
	else {
		DeleteDC(hdc);
	}
	hdc = 0;
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::drawRectangle(int x, int y, int w, int h)
{
	SetRectRgn(hrgn, x, y, x + w, y + h);
	if (undraw_thickness > 0) {
		FrameRgn(hdc, hrgn, hbrush, undraw_thickness, undraw_thickness);
		undraw_thickness = 0;
	}
	else {
		FrameRgn(hdc, hrgn, hbrush, thickness, thickness);
	}
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::drawLine(int x1, int y1, int x2, int y2)
{
	if ((x1 != x2) && (y1 != y2)) {
		//
		// Non-horizontal non-vertical line.
		//
		if (undraw_thickness > 0) {
		}
		else {
		}
		LineTo(hdc, x1, y1);
		MoveToEx(hdc, x2, y2, 0);
	}
	else if ((undraw_thickness == 1) || (thickness == 1)) {
    	// RECT rect;
		// DrawFocusRect(hdc, &rect);
	}
	else {
		int t;
		if (undraw_thickness > 0) {
			t = undraw_thickness;
			undraw_thickness = 0;
		}
		else {
			t = thickness;
		}
		if (x1 == x2) {
			y1 -= 1;
			y2 += 2;
			x2 += t;
		}
		else {
			x1 -= 1;
			x2 += 2;
			y2 += t;
		}
		Rectangle(hdc, x1, y1, x2 + 1, y2 + 1);
	}
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::captureMouse(VWindow *window)
{
	if (window == 0) {
		window = theWindow;
	}
	if (window != 0) {
		HWND capture_hwnd = window->hwnd();
    	SetCapture(capture_hwnd);
	}
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::uncaptureMouse()
{
    ReleaseCapture();
}

// --------------------------------------------------------------------------
// MS/Windows
//
void VRubberBand::setThickness(int t)
{
	if (t < 0) {
		t = 1;
	}
	if (rectangle_drawn) {
		undraw_thickness = thickness;
	}
	thickness = t;
}

// =========================================================================
// IBM OS/2
//
#elif defined(PM)

// --------------------------------------------------------------------------
// We will handle the OS/2 backward y-coordinate issue at the lowest level,
// when a mouse event is received and when a rubber-band is drawn.
//
//     Foundation to OS/2: y = window-height - y
//     OS/2 to Foundation: y = window-height - y
//
#define	WindowingSystemHasSeparateMouseUpEvents

#define UpArrowKey					VK_UP
#define DownArrowKey				VK_DOWN
#define LeftArrowKey				VK_LEFT
#define RightArrowKey				VK_RIGHT
#define EscapeKey					VK_ESC
#define ReturnKey					VK_ENTER

#define MouseMoveEvent				WM_MOUSEMOVE
#define KeyEvent					WM_CHAR
#define MouseButtonUpEvent			WM_BUTTON1UP
#define LeftMouseButtonUpEvent		WM_BUTTON1UP
#define RightMouseButtonUpEvent		WM_BUTTON2UP

#define KeyType						int
#define EventType					QMSG

#if 0
#define getNextEvent(m)				(WinGetMsg(notifier->getHab(),			\
											   &m, 0, 0, 0),				\
									 WinDispatchMsg(notifier->getHab(),		\
									 				&m), m.msg)
#else
#define getNextEvent(m)				(WinGetMsg(notifier->getHab(),			\
											   &m, 0, 0, 0),				\
									 WinDispatchMsg(notifier->getHab(),		\
									 				&m),					\
									 ((m.msg != WM_CHAR) ?					\
									  	m.msg :								\
										((CHARMSG(&m)->fs & KC_KEYUP) ?		\
											KeyUpEvent : KeyDownEvent)))
#endif

#define getTypeOfEvent(m)			(m.msg)

#define mapMouseMoveEvent(m,X,Y)	{ POINTL p;								\
									  p.x = SHORT1FROMMP(m.mp1);			\
									  p.y = SHORT2FROMMP(m.mp1);			\
									  WinMapWindowPoints(m.hwnd,			\
									  					 HWND_DESKTOP,		\
														 &p, 1);			\
									  X = p.x; Y = windowHeight - p.y;		\
									}

#define mapKeyEvent(m,K,C,S)		{ K = SHORT2FROMMP(m.mp2);				\
									  C = WinGetKeyState					\
									  	  (HWND_DESKTOP, VK_CTRL)			\
										   == 0x8000;						\
									  S = WinGetKeyState					\
									  	  (HWND_DESKTOP, VK_SHIFT)			\
										   == 0x8000;						\
									}

// --------------------------------------------------------------------------
//
static HPS		hps				= 0;
static HRGN		hrgn			= 0;
static LONG		savedMixMode	= 0;
static int		windowHeight	= 0; // For backward y-coordinate handling

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::initializeWindow(int *w, int *h)
{
	if (theWindow != 0) {
		int window_w, window_h;
		theWindow->sizeOfImage(&window_w, &window_h);
		if (w != 0) {*w = window_w; }
		if (h != 0) {*h = window_h; }
		windowHeight = window_h;
	}
	else {
		static int screen_w = 0, screen_h = 0;
		if (screen_w == 0) { notifier->getScreenSize(&screen_w, &screen_h); }
		if (w != 0) {*w = screen_w; }
		if (h != 0) {*h = screen_h; }
		windowHeight = screen_h;
	}
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::deinitializeWindow()
{
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::createRubberBand()
{
	RECTL rect;
	rect.xLeft = rect.yBottom = rect.xRight = rect.yTop = 0;
	if (theWindow != 0) {
		hps = theWindow->getHndl();
	}
	else {
		hps = WinGetScreenPS(HWND_DESKTOP);
	}
	savedMixMode = GpiQueryMix(hps);
	GpiSetMix(hps, FM_NOTXORSRC);
	hrgn = GpiCreateRegion(hps, 1, &rect);
	cv_assert(hrgn != 0);
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::destroyRubberBand()
{
	GpiSetMix(hps, savedMixMode); // Restore the MIX MODE
	GpiDestroyRegion(hps, hrgn);
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::drawRectangle(int x, int y, int w, int h)
{
	RECTL rect;
	rect.xLeft = x;
	rect.yBottom = windowHeight - (y + h);
	rect.xRight = x + w;
	rect.yTop = windowHeight - y;
	GpiSetRegion(hps, hrgn, 1, &rect);
	SIZEL frameThickness;
	if (undraw_thickness > 0) {
		frameThickness.cx = frameThickness.cy = undraw_thickness;
		undraw_thickness = 0;
	}
	else {
		frameThickness.cx = frameThickness.cy = thickness;
	}
	GpiFrameRegion(hps, hrgn, &frameThickness);
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::drawLine(int x1, int y1, int x2, int y2)
{
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::captureMouse(VWindow *window)
{
	if (window == 0) {
		window = theWindow;
	}
	notifier->captureMouseFor(window);
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::uncaptureMouse()
{
	notifier->releaseCapture();
}

// --------------------------------------------------------------------------
// OS/2
//
void VRubberBand::setThickness(int t)
{
	if (t < 0) {
		t = 1;
	}
	if (rectangle_drawn) {
		undraw_thickness = thickness;
	}
	thickness = t;
}

#endif

// --------------------------------------------------------------------------
//
void VRubberBand::setWindow(VWindow *window)
{
	if (currentRubberBand == 0) {
		theWindow = window;
	}
}

// --------------------------------------------------------------------------
//
VRubberBand::VRubberBand(VWindow *window)
{
	theWindow				= window;
	clientObject			= 0;
	uponBeginResizeMethod	= 0;
	uponResizeMethod		= 0;
	uponBeginMoveMethod		= 0;
	uponMoveMethod			= 0;
	minimumWidth			= 1;
	minimumHeight			= 1;
	confinedToWindow		= 0;
	thickness				= 2;
	hysteresisX				= 0;
	hysteresisY				= 0;
	drawingDisabled			= 0;
	doNotMoveX				= 0;
	doNotMoveY				= 0;
	doLine					= 0;
}

// --------------------------------------------------------------------------
//
VRubberBand::~VRubberBand()
{
}

// --------------------------------------------------------------------------
//
VRubberBand *VRubberBand::getCurrent()
{
	return currentRubberBand;
}

// --------------------------------------------------------------------------
//
void VRubberBand::uponBeginResize(VObject *o, method m)
{
	clientObject = o;
	uponBeginResizeMethod = (boolean (VObject::*) ())m;
}

// --------------------------------------------------------------------------
//
void VRubberBand::uponResize(VObject *o, method m)
{
	clientObject = o;
	uponResizeMethod = (boolean (VObject::*)(int, int, int&, int&, int))m;
}

// --------------------------------------------------------------------------
//
void VRubberBand::uponBeginMove(VObject *o, method m)
{
	clientObject = o;
	uponBeginMoveMethod = (boolean (VObject::*)())m;
}

// --------------------------------------------------------------------------
//
void VRubberBand::uponMove(VObject *o, method m)
{
	clientObject = o;
	uponMoveMethod = (boolean (VObject::*)
							  (int&, int&, int&, int&, int, int))m;
}

// --------------------------------------------------------------------------
//
boolean VRubberBand::beginResizeCallback()
{
	if ((clientObject != 0) && (uponBeginResizeMethod != 0)) {
		return (clientObject->*uponBeginResizeMethod)();
	}
	else {
		return TRUE;
	}
}

// --------------------------------------------------------------------------
//
boolean VRubberBand::resizeCallback(int x, int y, int& w, int& h, int mode)
{
	if ((clientObject != 0) && (uponResizeMethod != 0)) {
		return (clientObject->*uponResizeMethod)(x, y, w, h, mode) &&
			   (doLine || ((w > 0) && (h > 0)));
	}
	else {
		return TRUE;
	}
}

// --------------------------------------------------------------------------
//
boolean VRubberBand::beginMoveCallback()
{
	if ((clientObject != 0) && (uponBeginMoveMethod != 0)) {
		return (clientObject->*uponBeginMoveMethod)();
	}
	else {
		return TRUE;
	}
} 

// --------------------------------------------------------------------------
//
boolean VRubberBand::moveCallback(int& x, int& y, int& w, int& h,
								  int mx, int my)
{
	if ((clientObject != 0) && (uponMoveMethod != 0)) {
		return (clientObject->*uponMoveMethod)(x, y, w, h, mx, my) &&
			   (doLine || ((w > 0) && (h > 0)));
	}
	else {
		return TRUE;
	}
} 

// --------------------------------------------------------------------------
//
void VRubberBand::disableDrawing(boolean b)
{
	drawingDisabled = b;
	undraw();
}

// --------------------------------------------------------------------------
//
void VRubberBand::undraw()
{
	if (rectangle_drawn) {
		if (doLine) {
			drawLine(rubber_x, rubber_y, rubber_w, rubber_h);
		}
		else {
			drawRectangle(rubber_x, rubber_y, rubber_w, rubber_h);
		}
		rectangle_drawn = 0;
	}
}

// --------------------------------------------------------------------------
//
void VRubberBand::draw(int x, int y, int w, int h)
{
	if (drawingDisabled) {
		return;
	}
	if (doLine) {
		if (rectangle_drawn) {
			drawLine(rubber_x, rubber_y, rubber_w, rubber_h);
		}
		else {
			rectangle_drawn = 1;
		}
		drawLine(x, y, w, h);
	}
	else {
		if (rectangle_drawn) {
			drawRectangle(rubber_x, rubber_y, rubber_w, rubber_h);
		}
		else {
			rectangle_drawn = 1;
		}
		drawRectangle(x, y, w, h);
	}
	rubber_x = x; rubber_w = w;
	rubber_y = y; rubber_h = h;
}

// --------------------------------------------------------------------------
//
int VRubberBand::resize(int mode, int& x, int& y, int& w, int& h,
								  int& mx, int& my, VWindow *window)
{
	currentRubberBand = this;

	int last_x = x, last_y = y, last_w = w, last_h = h;
	int offset_x, offset_y;
	int window_w, window_h, status, cancelled = 0;
	EventType event;

	switch (mode) {
	case ResizeLeft:
		offset_x = mx - x;
		break;
	case ResizeRight:
		offset_x = mx - (x + w);
		break;
	case ResizeTop:
		offset_y = my - y;
		break;
	case ResizeBottom:
		offset_y = my - (y + h);
		break;
	case ResizeTopLeft:
		offset_x = mx - x;
		offset_y = my - y;
		break;
	case ResizeTopRight:
		offset_x = mx - (x + w);
		offset_y = my - y;
		break;
	case ResizeBottomLeft:
		offset_x = mx - x;
		offset_y = my - (y + h);
		break;
	case ResizeBottomRight:
		offset_x = mx - (x + w);
		offset_y = my - (y + h);
		break;
	default:
		return NeverStarted;
	}

	initializeWindow(&window_w, &window_h);
	captureMouse(window);

	if (!beginResizeCallback() ||
		!resizeCallback(last_x, last_y, last_w, last_h, mode)) {
		goto Done;
	}

	createRubberBand();
	draw(last_x, last_y, last_w, last_h);

	while (1) {
		switch (getNextEvent(event)) {
		case MouseMoveEvent:
		{
			int new_x, new_y, new_w, new_h;
			mapMouseMoveEvent(event, mx, my);
			new_x = mx - offset_x; 
			new_y = my - offset_y; 
			switch (mode) {
			case ResizeLeft:
				if ((new_w = last_x + last_w - new_x) < minimumWidth) {
					new_w = minimumWidth;
				}
				new_x = last_x + last_w - new_w;
				new_y = last_y;
				new_w = new_w;
				new_h = last_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				new_x = last_x + last_w - new_w;
				break;
			case ResizeRight:
				if ((new_w = new_x - last_x) < minimumWidth) {
					new_w = minimumWidth;
				}
				new_x = last_x;
				new_y = last_y;
				new_w = new_w;
				new_h = last_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				break;
			case ResizeTop:
				if ((new_h = last_y + last_h - new_y) < minimumHeight) {
					new_h = minimumHeight;
				}
				new_x = last_x;
				new_y = last_y + last_h - new_h;
				new_w = last_w;
				new_h = new_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				new_y = last_y + last_h - new_h;
				break;
			case ResizeBottom:
				if ((new_h = new_y - last_y) < minimumHeight) {
					new_h = minimumHeight;
				}
				new_x = last_x;
				new_y = last_y;
				new_w = last_w;
				new_h = new_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				break;
			
			case ResizeTopLeft:
				if ((new_w = last_x + last_w - new_x) < minimumWidth) {
					new_w = minimumWidth;
				}
				new_x = last_x + last_w - new_w;
				new_w = new_w;
				if ((new_h = last_y + last_h - new_y) < minimumHeight) {
					new_h = minimumHeight;
				}
				new_y = last_y + last_h - new_h;
				new_h = new_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				new_x = last_x + last_w - new_w;
				new_y = last_y + last_h - new_h;
				break;
			case ResizeTopRight:
				if ((new_w = new_x - last_x) < minimumWidth) {
					new_w = minimumWidth;
				}
				new_x = last_x;
				new_w = new_w;
				if ((new_h = last_y + last_h - new_y) < minimumHeight) {
					new_h = minimumHeight;
				}
				new_y = last_y + last_h - new_h;
				new_h = new_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				new_y = last_y + last_h - new_h;
				break;
			case ResizeBottomLeft:
				if ((new_w = last_x + last_w - new_x) < minimumWidth) {
					new_w = minimumWidth;
				}
				new_x = last_x + last_w - new_w;
				new_w = new_w;
				if ((new_h = new_y - last_y) < minimumHeight) {
					new_h = minimumHeight;
				}
				new_y = last_y;
				new_h = new_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				new_x = last_x + last_w - new_w;
				break;
			case ResizeBottomRight:
				if ((new_w = new_x - last_x) < minimumWidth) {
					new_w = minimumWidth;
				}
				new_x = last_x;
				new_w = new_w;
				if ((new_h = new_y - last_y) < minimumHeight) {
					new_h = minimumHeight;
				}
				new_y = last_y;
				new_h = new_h;
				if (!resizeCallback(new_x, new_y, new_w, new_h, mode)) {
					continue;
				}
				break;
			}
			if ((new_x != last_x) || (new_y != last_y) ||
				(new_w != last_w) || (new_h != last_h)) {
				if ((new_x >= window_w - 2) || (new_x + new_w <= 2) ||
					(new_y >= window_h - 2) || (new_y + new_h <= 2)) {
					continue;
				}
				if (confinedToWindow &&
					((new_x < 0) || (new_x + new_w > window_w) ||
					 (new_y < 0) || (new_y + new_h > window_h))) {
					continue;
				}
				last_x = new_x; last_y = new_y;
				last_w = new_w; last_h = new_h;
				draw(last_x, last_y, last_w, last_h);
			}
			break;
		}
		case MouseButtonUpEvent:
		{
			goto Done;
		}

		case KeyDownEvent:
		{
			KeyType	key;
			int	control, shift;
			mapKeyEvent(event, key, control, shift);
			switch (key) {
			case EscapeKey:
				cancelled = 1;
				goto Done;
			case ReturnKey:
				goto Done;
			}
			break;
		}
		}
	}

Done:
	uncaptureMouse();
	undraw();
	destroyRubberBand();
	deinitializeWindow();
	if (cancelled) {
		status = Cancelled;
	}
	else if ((x == last_x) && (y == last_y) &&
			 (w == last_w) && (h == last_h)) {
		status = Unchanged;
	}
	else {
		x = last_x; y = last_y;
		w = last_w; h = last_h;
		status = Changed;
	}
	currentRubberBand = 0;
	return status;
}

// --------------------------------------------------------------------------
// Move the rubberband starting with the given (x,y,w,h) position/size; the
// position is relative to the the window this VRubberBand was created with,
// or to the screen if that was null -- this is the window to which the
// drawing will be done.  The (mx,my) arguments represent the current
// position of the mouse, also relative to the window this VRubberBand was
// createed with. The values passed to the callbacks are also *all* relative
// to the window this VRubberBand was created with.  The given window
// argument merely specifies the window to use for capturing the mouse.
//
int VRubberBand::move(int& x, int& y, int& w, int& h,
					  int& mx, int& my, VWindow *window)
{
	currentRubberBand = this;

	int last_x = x, last_y = y, last_w = w, last_h = h;
	int window_w, window_h, original_mx = mx, original_my = my;
	int offset_x, offset_y;
	int begun, status, cancelled = 0;
	EventType event;

	initializeWindow(&window_w, &window_h);
	captureMouse(window);

	offset_x = mx - x;
	offset_y = my - y;

	if ((hysteresisX == 0) && (hysteresisY == 0)) {
		if (!beginMoveCallback() ||
			!moveCallback(last_x, last_y, last_w, last_h, mx, my)) {
			goto Done;
		}
		createRubberBand();
		draw(last_x, last_y, last_w, last_h);
		begun = 1;
	}
	else {
		begun = 0;
	}

	while (1) {
		switch (getNextEvent(event)) {
		case MouseMoveEvent:
		{
			int new_x, new_y, new_w, new_h;
			mapMouseMoveEvent(event, mx, my);
			if (begun) {
				if (doNotMoveX) { mx = original_mx; }
				if (doNotMoveY) { my = original_my; }
			}
		HandleMouseMoveEvent:
			if (!begun) {
				if ((abs(mx - original_mx) > hysteresisX) ||
					(abs(my - original_my) > hysteresisY)) {
					if (!beginMoveCallback()) {
						goto Done;
					}
					new_x = mx - offset_x; 
					new_y = my - offset_y; 
					new_w = last_w;
					new_h = last_h;
					if (!moveCallback(new_x, new_y,
									  new_w, new_h, mx, my)) {
						continue;
					}
					createRubberBand();
					last_x = new_x; last_y = new_y;
					last_w = new_w; last_h = new_h;
					draw(last_x, last_y, last_w, last_h);
					begun = 1;
				}
				continue;
			}
			new_x = mx - offset_x; 
			new_y = my - offset_y; 
			new_w = last_w;
			new_h = last_h;
			if (!moveCallback(new_x, new_y, new_w, new_h, mx, my)) {
				continue;
			}
			if ((new_x != last_x) || (new_y != last_y) ||
				(new_w != last_w) || (new_h != last_h)) {
				if (confinedToWindow) {
					if (new_x + new_w > window_w) {
						new_x = window_w - w;
					}
					if (new_y + new_h > window_h) {
						new_y = window_h - new_h;
					}
					if (new_x < 0) {
						new_x = 0;
					}
					if (new_y < 0) {
						new_y = 0;
					}
				}
				last_x = new_x; last_y = new_y;
				last_w = new_w; last_h = new_h;
				draw(last_x, last_y, last_w, last_h);
			}
			break;
		}
		case MouseButtonUpEvent:
		{
			goto Done;
		}

		case KeyDownEvent:
		{
			KeyType	key;
			int	control, shift;
			mapKeyEvent(event, key, control, shift);
			switch (key) {
			case EscapeKey:
				cancelled = 1;
				goto Done;
			case ReturnKey:
				goto Done;
			}
			if (control || shift) {
				goto HandleMouseMoveEvent;
			}
			break;
		}
		case KeyUpEvent:
		{
			KeyType key;
			int	control, shift;
			mapKeyEvent(event, key, control, shift);
			goto HandleMouseMoveEvent;
		}
		}
	}

Done:
	uncaptureMouse();
	if (begun) {
		undraw();
		if (cancelled) {
			status = Cancelled;
		}
		else if ((x == last_x) && (y == last_y) &&
				 (w == last_w) && (h == last_h)) {
			status = Unchanged;
		}
		else {
			x = last_x; w = last_w;
			y = last_y; h = last_h;
			status = Changed;
		}
	}
	else {
		status = NeverStarted;
	}
	destroyRubberBand();
	deinitializeWindow();
	currentRubberBand = 0;
	return status;
}

// --------------------------------------------------------------------------
//
int VRubberBand::resizeLine(int& x1, int& y1, int& x2, int& y2,
							int& mx, int& my, VWindow *window)
{
	return NeverStarted;
}

// --------------------------------------------------------------------------
//
int VRubberBand::moveLine(int& x1, int& y1, int& x2, int& y2,
						  int& mx, int& my, VWindow *window)
{
	currentRubberBand = this;
	doLine = 1;

	int last_x1 = x1, last_y1 = y1, last_x2 = x2, last_y2 = y2;
	int original_mx = mx, original_my = my;
	int dx = x2 - x1, dy = y2 - y1;
	int window_w, window_h, offset_x, offset_y;
	int begun, status, cancelled = 0;
	EventType event;

	initializeWindow(&window_w, &window_h);
	captureMouse(window);

	if ((hysteresisX == 0) && (hysteresisY == 0)) {
		offset_x = mx - last_x1;
		offset_y = my - last_y1;
		if (!beginMoveCallback() ||
			!moveCallback(last_x1, last_y1, last_x2, last_y2, mx, my)) {
			goto Done;
		}
		createRubberBand();
		draw(last_x1, last_y1, last_x2, last_y2);
		begun = 1;
	}
	else {
		begun = 0;
	}

	while (1) {
		switch (getNextEvent(event)) {
		case MouseMoveEvent:
		{
			int new_x1, new_y1, new_x2, new_y2;
			mapMouseMoveEvent(event, mx, my);
			if (begun) {
				if (doNotMoveX) { mx = original_mx; }
				if (doNotMoveY) { my = original_my; }
			}
		HandleMouseMoveEvent:
			if (!begun) {
				if ((abs(mx - original_mx) > hysteresisX) ||
					(abs(my - original_my) > hysteresisY)) {
					if (!beginMoveCallback() ||
						!moveCallback(last_x1, last_y1,
									  last_x2, last_y2, mx, my)) {
						continue;
					}
					offset_x = mx - last_x1;
					offset_y = my - last_y1;
					createRubberBand();
					draw(last_x1, last_y1, last_x2, last_y2);
					begun = 1;
				}
				continue;
			}
			new_x1 = mx - offset_x; 
			new_y1 = my - offset_y; 
			new_x2 = new_x1 + dx;
			new_y2 = new_y1 + dy;
			if (!moveCallback(new_x1, new_y1, new_x2, new_y2, mx, my)) {
				continue;
			}
			if ((new_x1 != last_x1) || (new_y1 != last_y1) ||
				(new_x2 != last_x2) || (new_y2 != last_y2)) {
				if (confinedToWindow) {
					if (new_x2 > window_w)	{ new_x2 = window_w; }
					if (new_y2 > window_h)	{ new_y2 = window_h; }
					if (new_x1 < 0)			{ new_x1 = 0; }
					if (new_y1 < 0)			{ new_y1 = 0; }
				}
				last_x1 = new_x1; last_y1 = new_y1;
				last_x2 = new_x2; last_y2 = new_y2;
				draw(last_x1, last_y1, last_x2, last_y2);
			}
			break;
		}
		case MouseButtonUpEvent:
		{
			goto Done;
		}

		case KeyDownEvent:
		{
			KeyType	key;
			int	control, shift;
			mapKeyEvent(event, key, control, shift);
			switch (key) {
			case EscapeKey:
				cancelled = 1;
				goto Done;
			case ReturnKey:
				goto Done;
			}
			if (control || shift) {
				goto HandleMouseMoveEvent;
			}
			break;
		}
		case KeyUpEvent:
		{
			KeyType key;
			int	control, shift;
			mapKeyEvent(event, key, control, shift);
			goto HandleMouseMoveEvent;
		}
		}
	}

Done:
	uncaptureMouse();
	if (begun) {
		undraw();
		destroyRubberBand();
		if (cancelled) {
			status = Cancelled;
		}
		else if ((x1 == last_x1) && (y1 == last_y1) &&
				 (x2 == last_x2) && (y2 == last_y2)) {
			status = Unchanged;
		}
		else {
			x1 = last_x1; x2 = last_x2;
			y1 = last_y1; y2 = last_y2;
			status = Changed;
		}
	}
	else {
		status = NeverStarted;
	}
	deinitializeWindow();
	doLine = 0;
	currentRubberBand = 0;
	return status;
}
