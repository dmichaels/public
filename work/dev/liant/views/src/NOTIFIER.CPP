/*
$Header:   Q:/views/win/vcs/notifier.cpv   1.107   18 Mar 1997 16:50:40   davidmi  $
*/
//  notifier.cpp
//
//  VNotifier implementation
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
//  07/21/92 jmd    preened
//  03/25/93 jmd    added inStart flag
//  03/26/93 jmd    added support for menuHighlighted()
//  04/09/93 pat    changed id to VObject *
//  04/28/93 pat    added findWindow()
//  06/28/93 dgm    added yieldPaint(), paint(), and postPaint().
//  07/09/93 jmd    added closeWindow
//  07/19/93 jmd    minor preening,
//                  made sure that mouseExit is called when window is destroyed
//  07/20/93 jmd    preened (deja vu)
//  07/23/93 pat    changed findWindow() name arg to const VString&
//  07/23/93 pat    added palette support
//  07/28/93 dgm    applied a couple pat fixes for the ghost lists.
//  07/31/93 jmd    modified menu prompt logic, added mdi client mouse logic
//  08/02/93 jmd    added menu select callback
//  08/07/93 jmd    made getClient work if name is already in list
//  08/09/93 jmd    removed use of free
//  08/09/93 jmd    added test for empty strings to getClient
//  08/12/93 jmd    made bstat TRUE for rbutton for mouseMv event
//  08/09/93 dgm    added support for VTransparentWindow.
//  08/12/93 dgm    added dialogEditorCreateMode().
//  08/23/93 pat    added realizePalette() and maxColorUpdates and used 'em.
//  08/25/93 dgm    updated recreateWin() to respect Z-order and to handle
//                  combo-box resizing and to maintain disabled state.
//                  added dialogEditorRecreateMode().
//  09/02/93 pat    per dgm, fixed realizePalette() return value.
//  09/07/93 dgm    fix for non-modal behavior of modal-dialogs.
//  09/13/93 dgm    M8 changes; minor.
//  09/19/93 dgm    at long last, a fix for the tab-eating combo-boxes!
//  10/01/93 dgm    fixed modal border problem (#if 0'd out debugging code).
//  10/06/93 dgm    fix in recreateWin().
//  10/07/93 dgm    minor fix in default case in cvmessage(). (jmd).
//  10/10/93 jmd    enabled code for using global heap for edit controls
//  10/11/93 pat    initialized ghostClientList archive data
//  10/11/93 jmd    tests border before delete in recreateWindow
//  11/12/93 pat    declared _WndProcInit(), MdiChildProcInit() static.
//  12/27/93 jmd    added test for dlgClassName size
//  01/04/94 jmd    added help routines
//  01/14/94 jmd    fixed mdi client help
//  01/19/94 dgm    fixed dialog accelerators (broken on 9/7/93);
//                  also, now correctly disallow traversing to the next
//                  top-level window via Alt+F6 when a modal dialog is up.
//  01/31/94 dgm    fix for intermittant crash when closing main MDI window
//                  when a number of MDI child windows are still up.
//  02/03/94 dgm    added virtual keyUp() support.
//  02/07/94 dgm    fix for VMdiViews not respecting StyleHidden.
//  02/08/94 jmd    moved code inside ifdef
//  02/09/94 jmd    converted to use long scrollbar values
//  02/18/94 pat    freed global memory used by edit controls.
//  02/26/94 jmd    added initHelp, tests for hHelpCursor, fixed ifdef {
//  02/26/94 jmd    changed global memory scheme
//  03/03/94 pat    removed externs for window procedures.
//  03/04/94 pat    CV_WIN32:  GetClassLong(); GetWindowLong().
//  03/05/94 pat    STRICT:  
//                  HANDLE to HBRUSH in cvmessage()
//                  cast to HGDIOBJ for DeleteObject();
//                  cast to HBRUSH in WNDCLASS hbrBackground assignment;
//                  used VMouseCursor::getHndl(), not its protected hpCursor;
//                  HANDLE to HGLOBAL and HANDLE to HINSTANCE in global 
//                  edit line creation;
//  03/08/94 pat    CV_WIN32:  
//                  dealt with message re-packing under WM_VSCROLL, 
//                  WM_HSCROLL, WM_COMMAND, WM_CTLCOLOR, WM_MDIACTIVATE
//  03/08/94 pat    STRICT:
//                  updated mainwin() signature;
//                  cast control id to HMENU in CreateWindow() and 
//                  CreateWindowEx() calls;
//                  casts of window procedure params under WM_MDIACTIVATE
//  03/24/94 pat    removed hard-coded lParam value checks under WM_COMMAND
//  03/24/94 pat    CV_WIN32: updated SetWindowsHookEx() w/GetCurrentProcessId()
//  03/26/94 pat    CV_WIN32: added explicit int casts on coordinate 
//                  assignments from POINT, DRAWITEMSTRUCT, and PAINTSTRUCT;
//                  updated WM_MENUSELECT handling.
//  03/26/94 pat    CV_WIN32: 
//                  cast appropriately for SendMessage();
//                  classStyle parameter in *ProcInit()s is now UINT, not WORD;
//                  added casts to properly sign extend mouse coordinates.
//  04/06/94 pat    CV_WIN32: 
//                  does not use global shared memory trick for edit controls.
//  04/07/94 jmd    added NCMOUSEMOVE support to complete MouseExit handling
//  04/25/94 pat    exported lstrncmp(), gave external C++ not C linkage
//  06/28/94 jmd    added getExposedRectangle members
//  07/14/94 jld    replaced NULL with 0
//  08/02/94 dss    Ctl3d support. Only install the help filter ONCE.
//  08/15/94 dgm    Remove old postpaint support.
//  08/22/94 dgm    Fixed STRICT typo recreateWindow().
//  08/31/94 dss    Ctl3d non-dialog support.
//  09/08/94 jld    set buttonToTrack variable on mouse button down
//  09/28/94 jld    setAccel() overwrites existing accel key bindings
//  10/03/94 jld    made ~VNotifier() delete stuff
//  09/08/94 jld    set buttonToTrack variable on mouse button down
//  09/28/94 jld    setAccel() overwrites existing accel key bindings
//  10/03/94 jld    made ~VNotifier() delete stuff
//  10/11/94 dss    initialized VDialog::hwnd() for native dialog creation.
//                  Added Dialog "this" atom, Dialog "Wnd Proc" atom.
//                  Added MsgFilter, CBT hook procs.
//                  Removed data members: tick, modal, inStart.
//                  Added help support. Help is disassociated from appView 
//                  windows.
//                  Added window independent timer support. Timers formerly 
//                  depended the appView window.
//                  Changed modality: was enforced by event loop. Now natively 
//                  enforced by disabling the non-modal windows.
//                  startModal(), endModal(): modified for modal native dialogs.
//                  MsgFilterHook(): corrected bug where context sensitive help 
//                  didn't work in menus.
//  11/03/94 dgm    Moved wnId->fromRsc check in cvmessage() to the top of the
//                  routine to prevent a possible referencing wnId after it 
//  11/04/94 dss    multi appview support. STRICT conformance of some (HANDLE)
//                  types.
//  11/11/94 dgm    Changed messageLoop() to return when the window for which
//           dss    we are modal for is no longer on the modal stack.
//  11/18/94 dss    cvmessage(): WM_SETCURSOR: cast HTERROR to WORD. signed/
//                  unsigned promotion was wrong. 
//  12/19 94 evc    Moved file static ATOM variables into notifier class data.
//                  This fixes problem with multiple instances of Views 
//                  application that use common dialog boxes and Views DLLs. 
//  01/31/95 evc    dlgClassName changed to VString for MBCS safty
//  02/06/95 jld    added test for focus != 0 in processing WM_QUERYNEWPALETTE
//  03/07/95 jld    removed #ifdef STRICT
//  03/16/95 dss    cvmessage(): case WM_COMMAND: restore code for native
//                  accelerators.
//                  cvmessage(): case WM_DESTROY: call VWindow::destroyed()
//                  and related functions. Set hWnd to zero.
//  06/08/95 jld    changed doKey() to allow suspension of accelerator keys
//  06/23/95 dss    Removed support for VTransparentWindow. VTransparentWindow
//                  uses standard getInfo() to convey the correct style.
//                  Replaced most CreateWindow() calls with CreateWindowEx for
//                  simplicity.
//  07/25/95 evc    Added Header
//  07/26/95 evc    Changed SetWindowsHooksEX calls to be per thread for WIN32
//                  Fixes problems with Ctrl3D  
//  08/02/95 dss    Changed ctl3d functions to use the Views default if they
//                  fail. They could fail if they are used under Windows 95.
//  08/03/95 jld    removed help support not needed with new VHelp class
//  08/09/95 jld    put back MsgFilterHook
//  09/13/95 evc    Fixed MdiChildProcInit so it does not check for 
//                  onlyInstance.  FIxes bug where 2 MDI apps are brought up,
//                  and the first one is then closed.  AT that point the second
//                  app cannot create child MDI window(s).
//  09/14/95 dss    VStyle changes. Runtime detection of CTL3D & Windows 95
//					3D effects. VView::(de)activate.
//  09/28/95 dgm    Set buttonToTrack *before* calls to mouseDn/rmouseDn;
//                  this fixes a popup-menu mouse tracking problem.
//                  Fixed WM_INITMENUPOPUP to call VPopupMenu::selected() for
//                  popup-menus (and any sub-popup-menus) which are not part
//                  of a main menu.
//  09/29/95 dgm    Added setWindowRedrawUponResize(),
//                  isWindowRedrawUponResize(), setWindowAutoErased(), and
//                  isWindowAutoErased();  disableAutoRedraw() is obsolete.
//  09/29/95 dgm    Added WM_GETMINMAXINFO for minimum size constraint;
//                  set via VWindow::constrainMinimumSize(int w, int h).
//  10/08/95 evc	Initialized a coiupleof members to 0 in ::init(),
//                  most notably modalInfo.
//  10/05/95 dss    Protect Ctl3d functions from executing even if compiled
//					w/CV_CTL3D because we may be running Win95. Fixed WIN32
//					caption corruption but.
//  10/18/95 dgm    Added support for VWindow::constrainMaximumSize().
//                  Added activate()/deactivate() notification
//                  for VUtilityWindow.
//  10/19/95 evc    Fixed focus event handling for comboboxes
//  10/19/95 dgm    Added support for VWindow::systemColorChanged()
//                  and VWindow::systemFontChanged().
//  10/19/95 dgm    Fix getVirKey() return typo.
//  10/27/95 dgm    Turned static data into data members.
//  10/31/95 dss    Compound Document support.
//  11/02/95 evc    Added bringAppToTop
//  11/02/95 dgm    Changed call to VUtilityWindow::activate()/deactivate().
//  12/04/95 dgm    Fix in WM_NCMOUSEMOVE (wParam is not same as WM_MOUSEMOVE).
//  12/04/95 pkt    Added 'const' to char* parameter of createDialog.
//  12/05/95 dgm    Added call to VColorScheme::systemColorChanged().
//  12/12/95 dgm    Fixed WM_INITMENUPOP to call VPopupMenu::selected() for
//                  VPopupMenus which are not within a main VAppView menu-bar.
//  01/02/96 dss    multiple VMdiAppView handling.
//  01/25/96 krb    Added set/getPainterMode() and removed
//                  dialogEditorCreateMode().
//  01/29/96 dgm    Changes in WM_CTLCOLOR to make a control
//                  use its parent's background color.
//	02/08/96 evc	Added code to WM_NCLBUTTONDOWN to force any dropped-down
//                  comboboxes to pull up when we click the titlebar.
//                  Also subclasses the combobox to trap "comboLBox"
//	02/21/96 evc	Got rid of reference to VMemory::memhndl
//  02/22/96 pkt    In createWin for MDI child, worked around MS shortcomings
//                  when creating a child that needs to be maximized or hidden.
//  02/24/96 pkt    Straightened up the 02/22 fix so that (1) extra system
//                  menus and zoom buttons do not get created in the MDI menu
//                  bar, and (2) hidden MDI children do not get shown
//                  inadvertently.
//  02/24/96 pkt    More straightening, because in one case extra system menus
//                  still appeared.
//  04/19/96 dgm    Hack to suppress the blinking of VScrollBars which are
//                  not associated with a VView.
//  05/21/06 glm    changed tmpWin to *tmpWin
//  05/14/96 dgm    Fixed mouse events so that they go to disabled children;
//                  see mapToChild().
//  06/18/96 evc    WHen getting the hwnd of the editline
//                  of a combobox, we use GetWindowFromPoint.  Point
//					of 0,0 misses the editline under win95.  Make it 6,6
//	08/06/96 pkt	changed to set VWindow::beingDestroyed flag.
//	08/09/96 glm	mapToChild check for enabled/visible for NT
//	08/23/96 dgm	Set bit in VScrollBar during SB_THUMBTRACK callback.
//					More mapToChild() changes.
//	08/25/96 dgm	Changed window classes to never set CS_HREDRAW/CS_VREDRAW,
//					rather invalidate the entire window in WM_SIZE iff the
//					redraw-upon-resize flag is set.  Setting the window class
//					styles does not seem to work for nested windows.
//	09/10/96 dgm	Fixed resource leak in destructor (tmpWin); added null
//					handle checks to some Windows API calls; fixed
//					SetBrushOrg() to mod 8 its argument (all BoundsChecker).
//	09/14/96 dgm	Fixed resource (menu) leak in deleteAppView() and erroneous
//					resource (menu) reference in WM_MENUSELECT (BoundsChecker).
//	09/16/96 dgm	Fixed two modality problems -- one in which a system dialog
//					like VYesNo was popped up from a modal dialog and was
//					causing its parent modal dialog to go non-modal when the
//					VYesNo was dismisses; the other was causing the main
//					VAppView to be permanently disabled when a modal dialog
//					was dismised using the system box (WM_CLOSE).
//	09/26/96 dgm	Fixed key() and keyUp() to handle ALT sequences.
//	10/15/96 dgm	Reworked WM_CTLCOLOR handling a bit.
//	12/13/96 dgm	MDI/hClient related fixes in WM_ACTIVATE and createWin().
//	01/02/97 dgm	Temporary hack in createWindow() to intrepret an x,y value
//					of -4999 to mean the system default location for MDI
//					children; for all other window types it is changed to 0.
//	01/02/97 dgm	Fix in deleteAppView() for MDI child/menu crash.
//	01/09/97 dgm	Added hooks for IntelliMouse support (see imouse.h).
//	01/19/97 dgm	Added drag-and-drop support.
//	01/25/97 dgm	Fixed memory leak (remove accelerators in destructor).
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
// ----------------------------------------------------------------------------

//
// Notes to the Microsoft programmer about Ctl3d:
//
// Since Views creates dialogs at runtime, and not using resources, an 
// alternative method, other than the Microsoft recoommended one, is
// necessary to hook into the Ctl3d dll.
//
// 1) VControls are subclassed individually using Ctl3dSubclassCtl() when
//      they are created.
// 2) VWindows that have 3D effects have their background painted, manually,
//      (as opposed to having it done by ctl3d.dll, automatically) with the
//      brush returned from Ctl3dCtlColorEx().
// 3)   The frame is painted, manually, by Ctl3dDlgFramePaint().
// 4) Native MS Windows dialogs (FileOpen, MessageBox, etc.) are Ctl3d 
//      sub-classed automatically using Ctl3dAutoSubclass(). Therefore, there 
//      is no way, in Views, to override the 3d effects, as there is with 
//      VControls and VPopupWindows (using VWindow::hasCtl3dEffects() and 
//      VWindow::isTopLevelWindow()).
//
// Notes:
// 1)   What is true for the above classes is also true of their sub-classes.
// 2)   VControl's automatically render themselves with 3D effects if their 
//      top level parent has 3D effects enabled. Currently, only the VDialog 
//      class has 3D effects enabled, by default. To cause 3D effects of 
//      VControls on any top level VWindows that are not VDialogs, sub-class 
//      VWindow::hasCtl3dEffects() in the top level window class to enable 
//      3D rendering of the VControls.
//
// References: Microsoft Development Library, Technical Articles:
//             Windows (16-bit) Window Manager, "Adding 3-D Effects to Controls"
//
// - dss 09/06/94
//

extern "C" {
#   include <string.h>
#   include <ctype.h>
}

#include "accel.h"
#include "appview.h"
#include "control.h"
#include "dialog.h"
#include "iterator.h"
#include "mdiappvw.h"
#include "mdiview.h"
#include "stdmdiap.h"
#include "memry.h"
#include "menu.h"
#include "menuitem.h"
#include "popupmen.h"
#include "rect.h"
#include "scrollbr.h"
#include "editbox.h"
#include "stack.h"
#include "str.h"
#include "window.h"
#include "event.h"
#include "brush.h"
#include "mouscrsr.h"
#include "transwin.h"
#include "combobox.h"
#include "cvassert.h"
#include "notifier.h"
#include "pathstr.h"        // for help file
#include "intassoc.h"
#include "shdctrl.h"
#include "utilwin.h"
#include "clrscm.h"

#ifdef CV_OLE_DRAG_DROP
#include "dragsite.h"
#include "dropsite.h"
#include "dndimp.h"
#endif

#ifdef CV_IMOUSE
#include "imouse.h"
#endif

// These files have to be included after defs.h which optionally define
// MS Windows defines that affect window data types. defs.h is included,
// implicitly by one or more of the above views headers.
extern "C" {
#   include "windows.h"
#ifdef CV_CTL3D
#   include "ctl3d.h"
#endif
}

extern "C" {
#   include <dde.h>
}

// ----------------------------------------------------------------------------
//
defineClass(VNotifier, VGlobal)

// ----------------------------------------------------------------------------
//
extern VClass *VPopupMenuCls;
extern VClass *VBitMapCls;

// ----------------------------------------------------------------------------
//
static VWindow *captureWindow = 0;
static short VWinVersionMajor = LOBYTE(LOWORD(GetVersion()));

// ----------------------------------------------------------------------------
//
#if defined(CV_WIN32)
#define getDialogThis(hWnd) \
    ((VWindow *) GetProp( hWnd, (LPCSTR) notifier->aViewsThis))
#else
#define getDialogThis(hWnd) \
    ((VWindow *) MAKELONG( (UINT) GetProp( hWnd, (LPCSTR) notifier->aViewsThisLow),\
                                      GetProp( hWnd, (LPCSTR) notifier->aViewsThisHigh)))
#endif

#if defined(CV_WIN32)
#define getDialogWndProc(hWnd) \
    ((WNDPROC) GetProp( hWnd, (LPCSTR) notifier->aViewsWndProc))
#else
#define getDialogWndProc(hWnd) \
    ((WNDPROC) MAKELONG( (UINT) GetProp( hWnd, (LPCSTR) notifier->aViewsWndProcLow),\
                                      GetProp( hWnd, (LPCSTR) notifier->aViewsWndProcHigh)))
#endif

#define isDialogSubclassed(hWnd) (0 != getDialogWndProc(hWnd))

// ----------------------------------------------------------------------------
//
extern int cvmain(int, char **);

// ----------------------------------------------------------------------------
//
static void     _WndProcInit(char *classname, int isMdi, UINT classStyle);
static boolean  MdiChildProcInit(char *name, UINT classStyle);

// ----------------------------------------------------------------------------
//
#if 0

#define NOTRACKMOUSEEVENT

#ifndef NOTRACKMOUSEEVENT

#ifndef WM_NCMOUSEHOVER
#define WM_NCMOUSEHOVER                 0x02A0
#define WM_MOUSEHOVER                   0x02A1
#define WM_NCMOUSELEAVE                 0x02A2
#define WM_MOUSELEAVE                   0x02A3
#endif

#ifndef TME_HOVER
#define TME_HOVER       0x00000001
#define TME_LEAVE       0x00000002
#define TME_NONCLIENT   0x00000010
#define TME_QUERY       0x40000000
#define TME_CANCEL      0x80000000
#define HOVER_DEFAULT   0xFFFFFFFF
typedef struct tagTRACKMOUSEEVENT {
    DWORD cbSize;
    DWORD dwFlags;
    HWND  hwndTrack;
    DWORD dwHoverTime;
} TRACKMOUSEEVENT, *LPTRACKMOUSEEVENT;
#endif

__declspec(dllimport) BOOL __stdcall _TrackMouseEvent(LPTRACKMOUSEEVENT lpEventTrack);

#endif

#endif

//----------------------------------------------------------------------------
boolean MdiChildProcInit(char *name, UINT classStyle)
{
    WNDCLASS    wc;

    if (!GetClassInfo(notifier->inst(), (LPSTR)name, (LPWNDCLASS)&wc)) 
    {

        // Register the MDI child class

        wc.style            =  classStyle;
        wc.lpfnWndProc      = (WNDPROC) notifier->pCTalkMDIChildProc;
        wc.cbClsExtra       = 0;
        wc.cbWndExtra       = 0;
        wc.hInstance        = notifier->inst();
        wc.hIcon            = LoadIcon(wc.hInstance, name);
        wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground    = (HBRUSH) (COLOR_WINDOW + 1);
        wc.lpszMenuName     = 0;
        wc.lpszClassName    = name;

        RegisterClass(&wc);
        return TRUE;
    }
    return TRUE;
}

//----------------------------------------------------------------------------
void _WndProcInit(char *classname, int isMdi, UINT classStyle)
{
    WNDCLASS wc;

    if (!notifier->onlyInstance() || 
        GetClassInfo(notifier->inst(), (LPSTR)classname, (LPWNDCLASS)&wc)) {

        return;
    }

    //  Register the application class (MDI or SDI)

    wc.lpszClassName        = (LPSTR)classname;
    wc.hInstance            = notifier->inst();
    if (isMdi) {
        wc.lpfnWndProc      = (WNDPROC) notifier->pCTalkMDIFrameProc;
    }
    else {
        wc.lpfnWndProc      = (WNDPROC) notifier->pCTalkWndProc;
    }
    wc.style                = classStyle;
    wc.hbrBackground        = (HBRUSH) (COLOR_WINDOW + 1); 
    wc.hCursor              = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon                = LoadIcon(0, IDI_APPLICATION);
    wc.lpszMenuName         = (LPSTR) 0;
    wc.cbClsExtra           = 0;
    wc.cbWndExtra           = 0;

    if (!RegisterClass((LPWNDCLASS) &wc)) {
        print("MS Windows class %s already registered.", classname);
    }
}

//----------------------------------------------------------------------------
int VNotifier::getVirtKey(short key)
{
    if (GetKeyState(VK_LBUTTON) & 0x8000) {
        key |= K_Mod_Button;
    }
    if (key != K_Shift && GetKeyState(K_Shift) & 0x8000) {
        key |= K_Mod_Shift;
    }
    if (key != K_Meta1 && GetKeyState(K_Meta1) & 0x8000) {
        // Control
        key |= K_Mod_Meta1;
    }
    if (key != K_Meta2 && GetKeyState(K_Meta2) & 0x8000) {
        // Alt
        key |= K_Mod_Meta2;
    }
    if (key != K_Capslock && GetKeyState(K_Capslock) & 0x8000) {
        key |= K_Mod_Capslock;
    }
    return key;
}

// ----------------------------------------------------------------------------
//
static VPopupMenu *findPopupMenuHandle(VPopupMenu *popupmenu, HMENU hmenu)
{
    DO (popupmenu->mitems(), VObject, object)
        if (object->isA(VPopupMenuCls)) {
            if (((VPopupMenu *)object)->hmenu() == hmenu) {
                return (VPopupMenu *)object;
            }
            else {
                VPopupMenu *pm = findPopupMenuHandle
                                    ((VPopupMenu *)object, hmenu);
                if (pm != 0) {
                    return pm;
                }
            }
        }
    END
    return 0;
}

// ----------------------------------------------------------------------------
//
static void callSystemColorChangedOnChildren(VWindow *window)
{
    VOrdCollect *children = window->childrenOf();
    if (children != 0) {
        DO (*children, VWindow, child)
            child->systemColorChanged();
            callSystemColorChangedOnChildren(child);
        END
    }
}

// ----------------------------------------------------------------------------
//
static void callSystemFontChangedOnChildren(VWindow *window)
{
    VOrdCollect *children = window->childrenOf();
    if (children != 0) {
        DO (*children, VWindow, child)
            callSystemFontChangedOnChildren(child);
            child->systemFontChanged();
        END
    }
}

// ----------------------------------------------------------------------------
//
void VNotifier::systemColorsChanged(VWindow *window)
{
	VColorScheme::systemColorsChanged();
	if (window == 0) {
		DO(appViews.created, VAppView, appview)
			callSystemColorChangedOnChildren(appview);
			appview->systemColorChanged();
		END
	}
	else {
		callSystemColorChangedOnChildren(window);
		window->systemColorChanged();
	}
}

// ----------------------------------------------------------------------------
//
void VNotifier::systemFontsChanged(VWindow *window)
{
	VFont::systemFontsChanged();
	if (window == 0) {
		DO(appViews.created, VAppView, appview)
			callSystemFontChangedOnChildren(appview);
			appview->systemFontChanged();
		END
	}
	else {
		callSystemFontChangedOnChildren(window);
		window->systemFontChanged();
	}
}

// ----------------------------------------------------------------------------
//
void VNotifier::systemMetricsChanged(VWindow *)
{
}

// ---------------------------------------------------------------------------
//
static boolean mapToChild(VWindow *&window, HWND& hwnd, int& x, int& y)
{
    if (captureWindow != 0) {
        return FALSE;
    }

    POINT p;
    p.x = x;
    p.y = y;
    HWND child_hwnd = ChildWindowFromPoint(hwnd, p);
    if ((child_hwnd != 0) &&
    	(child_hwnd != hwnd) &&
		IsWindowVisible(child_hwnd)) {
        VWindow *child = notifier->getThis(child_hwnd);
        if (child != 0) {
            ClientToScreen(hwnd, &p);
            ScreenToClient(child_hwnd, &p);
            x = p.x;
            y = p.y;
            hwnd = child_hwnd;
            window = child;
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
// Construct a notifier instance. This constructor is only used only once
// to construct the notifier object
//
VNotifier::VNotifier()
{
    int x = 888;
    init();
}

#ifdef DLL
//----------------------------------------------------------------------------
// Construct a VNotifier object for use with DLLs.
//
VNotifier::VNotifier(boolean dll) :
    VGlobal(GLOBALnotifier)
{
    int x = 999;
    if (dll) {
        init();
    }
}
#endif

//----------------------------------------------------------------------------
void VNotifier::init()
{
#if defined(CV_WIN32)
    aViewsThis = GlobalAddAtom("C++/Views This");
    aViewsWndProc = GlobalAddAtom("C++/Views WndProc");
#else
    aViewsThisLow = GlobalAddAtom("C++/Views This Low");
    aViewsThisHigh = GlobalAddAtom("C++/Views This High");

    aViewsWndProcLow = GlobalAddAtom("C++/Views WndProc Low");
    aViewsWndProcHigh = GlobalAddAtom("C++/Views WndProc High");
#endif
    hPrevInst = 0;
    dlgWin = 0;
    hDlgWnd = 0;
    hDlgRsc = 0;
    nTimers = 0;
    hAccel = 0;

    currAccels = accelerators = 0;
    dontKey = FALSE;
    Id = 0;
    cnt = 0;
    idx = 0;
    winCursor = 0;
    curshWnd = 0;

    harrowCurs = LoadCursor(0, IDC_ARROW);
    hInst = 0;
    xlat = FALSE;
    key = 0;
    menuId = controlId = 10;

    cursWndId = 0;
    focus = 0;
    timers = 0;
    waitState = FALSE;
    keyCount = 0;
    exposed = FALSE;
    winEvents = FALSE;
    modals = 0;
    modalInfo = 0;
    winset = new VSet(200);
    popup = 0;

    bkBrush = 0;
    classesInitialized = FALSE;

    tmpWin = 0;

    // init MS Win class styles
    msgReturn = 0;

    //dlgClassName[0] = '\0'; // evc 

    mouseTracking(FALSE);

    msgIgnored = FALSE;
    menuSelClient = 0;
    menuSelMethod = NIL_METHOD;

    isPainterMode = FALSE;

    appResult = 0;

    lpfnMsgFilterHook = 0;
    hMsgFilterHook = 0;

    lpfnCBTHook = 0;
    hCBTHook = 0;

    lpfnCommDlgProc = 0;

    inStart = FALSE;

    ctl3dIsActive = 0;

    windowRedrawUponResize = TRUE;
    windowAutoErased = FALSE;

#ifndef CV_NOARCHIVER
    ghostClientList = 0;
#endif

	paletteChangedJustReceived = 0;
	paletteWindows = 0;
}

//----------------------------------------------------------------------------
VNotifier::~VNotifier()
{

    // there should be no appViews with presentations spaces
    cv_assert(appViews.created.count() == 0);

    // delete the appviews that weren't deleted by the programmer
    //
    // MUST USE REVERSEDO! 'delete' causes the object in the collection to
    // be removed causing the data in the colection to move up a notch for 
    // every delete. If DO is used then, as we increment forward, the object 
    // data is moving backwards, causing every other object to be deleted, 
    // instead.
    REVERSEDO(appViews.constructed, VAppView, appView)
        delete appView; // NOTE: its delete, here, not deleteAppView()!
    END

    if (bkBrush != 0) {
        DeleteObject((HGDIOBJ) bkBrush);
        bkBrush = 0;
    }

    if (tmpWin) {
        tmpWin->hWnd = 0;
        delete tmpWin;
    }

    if (0 != timers) {
        cv_assert(0 == timers->count());
        delete timers;
        timers = 0;
    }

    if (0 != modalInfo){
        delete modalInfo;
    }

	if (accelerators != 0) {
		accelerators->freeContents();
		delete accelerators;
	}

    // NOTE: unhook the windows hooks BEFORE destroying any windows:
    // prevents GP faults.

    // disconnect message filter
    if (0 != hMsgFilterHook) {
        UnhookWindowsHookEx(hMsgFilterHook);
        hMsgFilterHook = 0;
    }

    if (0 != hCBTHook) {
        UnhookWindowsHookEx(hCBTHook);
        hCBTHook = 0;
    }

#if defined(CV_WIN32)
     GlobalDeleteAtom(aViewsThis);
     aViewsThis = 0;

     GlobalDeleteAtom(aViewsWndProc);
     aViewsWndProc = 0;
#else

     GlobalDeleteAtom(aViewsThisLow);
     aViewsThisLow = 0;
     GlobalDeleteAtom(aViewsThisHigh);
     aViewsThisHigh = 0;

     GlobalDeleteAtom(aViewsWndProcLow);
     notifier->aViewsWndProcLow = 0;
     GlobalDeleteAtom(aViewsWndProcHigh);
     aViewsWndProcHigh = 0;
#endif

	if (paletteWindows != 0) {
		delete paletteWindows;
	}
}

//----------------------------------------------------------------------------
boolean VNotifier::setThis(VWindow *win)
{
    if (!winset) {
        winset = new VSet(200);
    }
    return winset->add((VObject *)win) ? TRUE : FALSE;
}

//----------------------------------------------------------------------------
boolean VNotifier::unsetThis(VWindow *win)
{
    return winset->remove((VObject *)win) ? TRUE : FALSE;
}

//----------------------------------------------------------------------------
// Start the VNotifier\'s event dispatching loop for an application.
//
void VNotifier::start()
{
    if (appViews.created.count() <= 0) {
		return;
	}
    inStart = TRUE;
    messageLoop(LoopMain);
    inStart = FALSE;
}

//----------------------------------------------------------------------------
void VNotifier::messageLoop(enum LoopReason loopingFor)
{
    MSG msg, msg2;

    VWindow *loopingForThisModalWindow;

    if (loopingFor == LoopModal) {
        cv_assert(modals != 0);
        loopingForThisModalWindow = (VWindow *)modals->top();
        cv_assert(loopingForThisModalWindow != 0);
    }

    while (1) {

        // if the modal window we're looping for isn't modal, anymore,
        // get out of this message loop.
        if (loopingFor == LoopModal && 
            ((modals == 0) || !modals->findSame(loopingForThisModalWindow))) {
            break;
        }
        
        if (loopingFor == LoopYield) {
            if (!PeekMessage((LPMSG)&msg, (HWND)0, 0, 0, PM_REMOVE)) {
                break;
            }
        }
        else if (!GetMessage((LPMSG)&msg, 0, 0, 0))  {
            break;
        }
        
        if (hDlgWnd && IsDialogMessage(hDlgWnd, (LPMSG)&msg)) {
            continue;
        }
        
        if ((hClient != 0) &&
            TranslateMDISysAccel(hClient, &msg)) {
            continue;
        }
        if (hAccel && currAccels &&
            TranslateAccelerator(msg.hwnd, hAccel, (LPMSG)&msg)) {
            continue;
        }

        if ((msg.message == WM_KEYDOWN) || (msg.message == WM_SYSKEYDOWN)) {
            int key = getVirtKey(msg.wParam);
            if (isModalWindowActive() &&
                ((key & (K_Mod_Alt | K_F6)) == (K_Mod_Alt | K_F6))) {
                //
                // Here, there's a modal dialog up and the user tried
                // to traverse to the next top-level window in the
                // application via Alt+F6 --> disallow it and beep!
                // dgm, 1/19/94.
                //
                MessageBeep(MB_OK);
                continue;
            }
            // currAccels == 0 no longer means that no accels are present
            // can now exist on windows
            // if (currAccels != 0) {
               if (doKey(key)) {
                  continue;
               }
            // }
        }

        if (xlat = TranslateMessage((LPMSG)&msg)) {
            xlat = PeekMessage((LPMSG)&msg2, msg.hwnd, WM_CHAR,
                WM_CHAR, PM_NOREMOVE | PM_NOYIELD);
        }

        // Windows does not let us get events in the
        // MDI client window, so we've got to test for them
        // in this less than optimal fashion.
        // If the msgIgnored is TRUE after DispatchMessage
        // then we did not proccess the message

        msgIgnored = TRUE;

        DispatchMessage((LPMSG)&msg);

        if (msgIgnored) {
            switch (msg.message) {
            case WM_MOUSEMOVE:
                // mouse moved in MDI client
                if (hClient != curshWnd) {
                    if (curshWnd) {
#ifndef CV_NOVECTOREVENTS
					    if (!cursWndId->vectorEvent(VWindow::MouseExit))
#endif
                        cursWndId->mouseExit(0);        // bstatus is wrong!
                    }
                    curshWnd = 0;
                    cursWndId = 0;
                }
                break;
            }
            msgIgnored = FALSE;
        }
    }
}

//----------------------------------------------------------------------------
boolean VNotifier::measureItem(long lParam)
{
    // because this is called during creation,
    // must use tmpWin->parent which is set in createWin
    VControl* cw = (VControl*) tmpWin->parent;
    if (cw != 0) {
        int result;
        LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT) lParam;
        if (lpmis->CtlType == ODT_COMBOBOX ||
            lpmis->CtlType == ODT_LISTBOX) {
            result = cw->notify(LN_MEASUREITEM);
            lpmis->itemHeight = result;
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
boolean VNotifier::drawItem(long lParam)
{
    int wasDone;
    VControl *cw;
    ds = (DRAWITEMSTRUCT *)lParam;
    if (ds->CtlType != ODT_BUTTON && 
        ds->CtlType != ODT_COMBOBOX &&
        ds->CtlType != ODT_LISTBOX) {
        return FALSE;
    }
    if (!(cw = (VControl *)getThis(ds->hwndItem))) {
        return FALSE;
    }
    if (!cw->refCount++) {
        cw->hDC = ds->hDC;
    }
    exposed = TRUE;

    expose.set( Corners, 
                int(ds->rcItem.left), 
                int(ds->rcItem.top),
                int(ds->rcItem.right), 
                int(ds->rcItem.bottom));

    if (ds->CtlType == ODT_BUTTON) {
        wasDone =  cw->notify(BN_USERDRAW);
    }
    else {
        wasDone =  cw->notify(LN_USERDRAW);
    }
    if (!-- cw->refCount) {
        cw->hDC = 0;
    }
    exposed = FALSE;
    return wasDone;
}

//----------------------------------------------------------------------------
boolean VNotifier::scrollItem(  HWND        hWnd, 
                                int         msg, 
                                WPARAM      wParam, 
                                LPARAM      lParam, 
                                VWindow    *wnId)
{
#if defined(CV_WIN32)
    int         nScrollCode     = (int)         LOWORD(wParam);
    short int   nPos            = (short int)   HIWORD(wParam);
    HWND        hwndScrollBar   = (HWND)        lParam;
#else
    int         nScrollCode     = (int)         wParam;
    short int   nPos            = (short int)   LOWORD(lParam);
    HWND        hwndScrollBar   = (HWND)        HIWORD(lParam);
#endif
    if (hwndScrollBar != 0)  {
#if defined(CV_WIN32)
        wnId = (VWindow *) getThis(GetDlgItem(hWnd, 
                               (int) GetWindowLong(hwndScrollBar, GWL_ID)));
#else
        wnId = (VWindow *) getThis(GetDlgItem(hWnd, 
                               (int) GetWindowWord(hwndScrollBar, GWW_ID)));
#endif
    }
    else {
        if (!wnId) {
            return FALSE;
        }
        if (wnId->isA(VViewCls)) {
            if (msg == WM_VSCROLL) {
                wnId =  (VWindow *)((VView *)wnId)->vScroll;
            }
            else {
                wnId =  (VWindow *)((VView *)wnId)->hScroll;
            }
        }
    }
    if (!wnId || !wnId->isA(VScrollBarCls)) {
        return FALSE;
    }
    VScrollBar *sb = (VScrollBar *)wnId;

    switch (nScrollCode) {
    case SB_LINEUP:
        sb->lineUp();
        break;
    case SB_LINEDOWN:
        sb->lineDown();
        break;
    case SB_PAGEUP:
        sb->pageUp();
        break;
    case SB_PAGEDOWN:
        sb->pageDown();
        break;
    case SB_THUMBTRACK:
		sb->dragging = 1;
        sb->track(sb->scaleValue(nPos));
		sb->dragging = 0;
        break;
    case SB_THUMBPOSITION:
        sb->endScroll(sb->scaleValue(nPos));
        break;
    case SB_ENDSCROLL:
        sb->endScroll(sb->getPos());
        break;
    }
    return TRUE;
}

//----------------------------------------------------------------------------
// Handler for MS Windows messages
//
long VNotifier::cvmessage(  HWND        hWnd, 
                            UINT        msg, 
                            WPARAM      wParam, 
                            LPARAM      lParam, 
                            VWindow *&  wnId)
{

#	define WM_LOSTFOCUS	(WM_USER+1234)

	VWindow *saveWnId;

    if (!tmpWin){
        tmpWin=new VWindow();
        tmpWin->hWnd = 0;
        tmpWin->parent = 0;
        tmpWin->disableFocus();
    }

    int  wasDone = 0;
    msgReturn = 0;
    msgIgnored = FALSE;
    if (msg == WM_CREATE){
        if (!winset) {
            winset = new VSet(200);
        }
    }

    if (msg == WM_CREATE && (tmpWin->parent != 0)) {
        VWindow *win = tmpWin->parent;
        VWindow *pwin = win->parent;
        win->hWnd = hWnd;
        setThis(win);
        if (pwin) {
            pwin->addChild(win);
        }
        win->parent = pwin;
        return wasDone;
    }

    if (!(wnId = getThis(hWnd)) && msg != WM_TIMER) {
        return wasDone;
    }

    // !!!dgm 11/3/94: Moved the check for wnId->fromRsc from the bottom
    // of this routine to here to avoid the possibility of erroneously
    // referencing wnId after it has been deleted; specifically, this
    // was tracked down when a bitmap-button click in a tool-bar caused
    // the tool-bar to be deleted.

    boolean wnIdFromRsc = wnId->fromRsc;

    if (winEvents) {
        VEvent ev(msg, wParam, lParam);
        long ret;
        if (ret = wnId->winDispatch(ev)) {
            if (ret == -1) {
                ret = 0;
            }
            msgReturn = ev.returnValue;
            return ret;
        }
    }


   	if ((wnId->setDefaultFont >= 0) && (wnId->getFont() == 0)) {
   		int fontid = wnId->setDefaultFont;
   		wnId->setDefaultFont = -1;
		SendMessage(hWnd, WM_SETFONT,
						  WPARAM(VFont::system(fontid).getHndl()),
						  LPARAM(1));
	}

    switch (msg) {
    case WM_ACTIVATEAPP:

        // if the app is activating with a modal window open, bring all the
        // top level windows to the top, preserving stacking order.
        //
        // NOTE: this is basically for the multiple appView case, but has no
        // ill side effects for single appview or mdi case.
        //
        // NOTE: multiple appviews may no be in consecutive sequence. They
        // will be after this message is processed.
        //
        // NOTE: this message is sent, by windows, to all the top level windows
        // of an app when it becomes activated or deactivated. Therefore, for
        // each message, each top level window only has to worry about getting 
        // itself next to it sibling, higher up the stacking order
        //
        // NOTE: the reason we process top level windows, and not appViews, is 
        // that there may be popup or dialog windows open. With this method, we
        // ensure we catch all top level windows, not just appViews.

        if ((wParam != 0) && isModalWindowActive()) {
            HWND sibling;

            // find this window's sibling higher up in the stacking order
            for (sibling = GetWindow( hWnd, GW_HWNDPREV); 
                  sibling != NULL && 
                  (GetWindowTask(sibling) != GetWindowTask(hWnd) ||
                   !IsWindowVisible(sibling));
                  sibling = GetWindow(sibling, GW_HWNDPREV))
                // no code goes here!!!
                ;

            if (sibling) {
                // put it behind its sibling
                SetWindowPos(hWnd, sibling,
                    0, 0, 0, 0,  // x, y, width, height
                    SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
            }
        }
        break;

    case WM_ACTIVATE:
        // if the window is a top level window (aren't WM_ACTIVATE's only
        // sent to top level windows?) and its not being deactivated,
        // then set the dlg handle to the window handle

        if (wnId->isTopLevel()) {
            if (LOWORD(wParam) != WA_INACTIVE) {
                if (wnId->fromRsc) {
                    hDlgWnd = hWnd;
                }
                dlgWin = (VView *)wnId;
                if (wnId->isA(VUtilityWindowCls)) {
                    ((VUtilityWindow *)wnId)->activate(0);      // ToDo
                }
            }
            else {
                dlgWin = 0;
                hDlgWnd = 0;
                if (wnId->isA(VUtilityWindowCls)) {
                    ((VUtilityWindow *)wnId)->deactivate(0);    // ToDo
                }
            }
        }
        if (wnId->isA(VViewCls)) {
#if defined(CV_WIN32)
//          if (hWnd == (HWND) lParam) {
            if (WA_INACTIVE != LOWORD(wParam)) {
                ((VView*)wnId)->activate((VView*)getThis((HWND) lParam));
				if (wnId->isA(VMdiAppViewCls)) {
					hClient = ((VMdiAppView *)wnId)->clientOf()->hwnd();
				}
            }
            else {
                ((VView*)wnId)->deactivate((VView*)getThis((HWND) lParam));
            }
#else
            if (WA_INACTIVE != wParam) {
				((VView *)wnId)->
					activate((VView *)getThis((HWND) LOWORD(lParam)));
				if (wnId->isA(VMdiAppViewCls)) {
					hClient = ((VMdiAppView *)wnId)->clientOf()->hwnd();
				}
            }
            else {
				((VView *)wnId)->
					deactivate((VView *)getThis((HWND) LOWORD(lParam)));
            }
#endif
        } // if a view class
        break;

    case WM_INITDIALOG:
        return FALSE;

    case WM_CUT:
        wnId = wnId;
        break;

    case WM_MOUSEMOVE:
    {
			int cursorSet = 0;
            int x = int(short(LOWORD(lParam)));
            int y = int(short(HIWORD(lParam)));
			saveWnId = wnId;
			if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
            if (winCursor) {
                SetCursor(winCursor->getHndl());
				cursorSet = 1;
            }
            else if (wnId->pCursor) {
                SetCursor(wnId->pCursor->getHndl());
				cursorSet = 1;
            }
#if 0 // sorry but this is wrong... should let def proc take care of this 
      // default view class registration not sets cursor
            else {
#ifdef CV_WIN32
                if (!GetClassLong(hWnd, GCL_HCURSOR))
#else
                if (!GetClassWord(hWnd, GCW_HCURSOR))
#endif
                {
                    SetCursor(harrowCurs);
                }
            }
#endif

            int bFlag = wParam & MK_LBUTTON || wParam & MK_RBUTTON;
            if (hWnd != curshWnd) {
                if (curshWnd != 0) {
#ifndef CV_NOVECTOREVENTS
				    if (!cursWndId->vectorEvent(VWindow::MouseExit))
#endif
                    cursWndId->mouseExit(bFlag);
                }
                curshWnd = hWnd;
                cursWndId = wnId;
#ifndef CV_NOVECTOREVENTS
			    if (!wnId->vectorEvent(VWindow::MouseEnter))
#endif

#if 0

#ifndef NOTRACKMOUSEEVENT
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				tme.dwHoverTime = 0;
				_TrackMouseEvent(&tme);
#endif

#endif
                wnId->mouseEnter(bFlag);
            }
            if (mouseTrk) {
#ifndef CV_NOVECTOREVENTS
			    if (wnId->vectorEvent(VWindow::MouseMv)) {
					wasDone = TRUE;
				}
				else {
                	wasDone = wnId->mouseMv(x, y, bFlag);
				}
#else
                wasDone = wnId->mouseMv(x, y, bFlag);
#endif
            }
            if (cursorSet) {
				wasDone = TRUE;
			}
			wnId = saveWnId;
            break;
        }

    case WM_NCMOUSEMOVE:
    {
        // jmd 4/7/94 make sure that mouseExit is called
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
        if (hClient != curshWnd) {
            int bFlag = buttonStatus() || rbuttonStatus();
            if (hWnd != curshWnd) {
                if (curshWnd) {
#ifndef CV_NOVECTOREVENTS
				    if (!cursWndId->vectorEvent(VWindow::MouseExit))
#endif
                    cursWndId->mouseExit(bFlag);
                }
                curshWnd = 0;
                cursWndId = 0;
            }
        }
		wnId = saveWnId;
        break;
    }

    case WM_NCLBUTTONDOWN:
        if (!wasDone) {
            // fix the ComboLBox staying down problem
            VWindow *hit = 0;
            DO (winset, VWindow, hit)
                if  (hit->isA(VComboBoxCls)){
                    SendMessage(hit->hwnd(), CB_SHOWDROPDOWN, FALSE, 0);
                }
            END
        }

        break;

    case WM_LBUTTONDOWN:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
        if (wnId != focus) {
			if (VWindow::checkLosingFocus(wnId)) {
				wasDone = TRUE;
				break;
			}
			wnId->takeFocus();
		}
        if (!wasDone) {
            buttonToTrack = TPM_LEFTBUTTON;
#ifndef CV_NOVECTOREVENTS
		    if (wnId->vectorEvent(VWindow::MouseDn)) {
				wasDone = TRUE;
			}
			else {
            	wasDone = wnId->mouseDn(x, y);
			}
#else
            wasDone = wnId->mouseDn(x, y);
#endif
#ifdef CV_OLE_DRAG_DROP
			if (wnId->dragSite != 0) {
				VObjectCourier *object_courier =
					wnId->dragSite->
						dragStart(wnId, int(short(LOWORD(lParam))),
										int(short(HIWORD(lParam))));
				if (object_courier != 0) {
					wnId->dragSite->dragLoop(object_courier);
				}
			}
#endif
        }
		wnId = saveWnId;
        break;
    }

    case WM_RBUTTONDOWN:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
        if (wnId->isA(VEditBoxCls) && VWindow::checkLosingFocus(wnId)) {
			wasDone = TRUE;
			break;
        }
        buttonToTrack = TPM_RIGHTBUTTON;
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::RMouseDn)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->rmouseDn(x, y);
		}
#else
        wasDone = wnId->rmouseDn(x, y);
#endif
		wnId = saveWnId;
        break;
    }

    case WM_MBUTTONDOWN:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
        wasDone = wnId->mmouseDn(x, y);
		wnId = saveWnId;
        break;
    }

    case WM_LBUTTONUP:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::MouseUp)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->mouseUp(x, y);
		}
#else
        wasDone = wnId->mouseUp(x, y);
#endif
		wnId = saveWnId;
        break;
    }

    case WM_RBUTTONUP:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::RMouseUp)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->rmouseUp(x, y);
		}
#else
        wasDone = wnId->rmouseUp(x, y);
#endif
		wnId = saveWnId;
        break;
    }

    case WM_MBUTTONUP:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
        wasDone = wnId->mmouseUp(x, y);
		wnId = saveWnId;
        break;
    }

    case WM_LBUTTONDBLCLK:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::MouseDbl)) {
			wasDone = TRUE;
		}
		else {
			wasDone = wnId->mouseDbl(x, y);
		}
#else
		wasDone = wnId->mouseDbl(x, y);
#endif
		wnId = saveWnId;
        break;
    }

    case WM_RBUTTONDBLCLK:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::RMouseDbl)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->rmouseDbl(x, y);
		}
#else
        wasDone = wnId->rmouseDbl(x, y);
#endif
		wnId = saveWnId;
        break;
    }

    case WM_MBUTTONDBLCLK:
    {
        int x = int(short(LOWORD(lParam)));
        int y = int(short(HIWORD(lParam)));
		saveWnId = wnId;
		if (wnId->children != 0) { mapToChild(wnId, hWnd, x, y); }
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::MMouseDbl)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->mmouseDbl(x, y);
		}
#else
        wasDone = wnId->mmouseDbl(x, y);
#endif
		wnId = saveWnId;
        break;
    }

    case WM_CLOSE:
        if (wnId->isA(VAppViewCls)) {
#ifndef CV_NOVECTOREVENTS
		    if (wnId->vectorEvent(VWindow::Close)) {
				wasDone = TRUE;
			}
			else {
            	wasDone = ((VView *)wnId)->close();
			}
#else
            wasDone = ((VView *)wnId)->close();
#endif
            if (!wasDone) {
                deleteAppView((VAppView *)wnId);
                wasDone = TRUE;
            }
        }

        // else plain ol' view, so call close()
        else if (wnId->isA(VViewCls) && ((VView *)wnId)->isTopLevel()) {
#ifndef CV_NOVECTOREVENTS
		    if (wnId->vectorEvent(VWindow::Close)) {
				wasDone = TRUE;
			}
			else {
            	wasDone =  ((VView *)wnId)->close();
			}
#else
            wasDone =  ((VView *)wnId)->close();
#endif
            if (!wasDone) {
				if ((modals != 0) && (modals->top() == wnId)) {
					modals->pop();
					VWindow *top = (VWindow *)modals->top();
					if (top != 0) {
						HWND hwnd = top->hwnd();
						if ((hwnd != 0) && !IsWindowEnabled(hwnd)) {
		            		EnableWindow(hwnd, TRUE);
						}
					}
					else {
						setTopLevelWindowEnablement(0, TRUE);
					}
				}
                delete wnId;
                wasDone = TRUE;
            }
        }
		else if ((modals != 0) && (modals->top() == wnId)) {
			modals->pop();
			VWindow *top = (VWindow *)modals->top();
			if (top != 0) {
				HWND hwnd = top->hwnd();
				if ((hwnd != 0) && !IsWindowEnabled(hwnd)) {
            		EnableWindow(hwnd, TRUE);
				}
			}
			else {
				setTopLevelWindowEnablement(0, TRUE);
			}
		}

        break;

    case WM_INITMENU:
        if (wnId->isA(VAppViewCls)) {
            wasDone = ((VAppView *)wnId)->menuSelected(0);
        }
        break;

    case WM_INITMENUPOPUP:
        if (wnId->isA(VAppViewCls)) {
            VPopupMenu *mp = (VPopupMenu *)
                             ((VAppView *)wnId)->menuOf(HMENU(wParam));
            if (mp != 0) {
                wasDone = ((VAppView *)wnId)->menuSelected(mp);
                break;
            }
        }
        if ((popup != 0) && popup->isA(VPopupMenuCls)) {
            if (((VPopupMenu *)popup)->hMenu == HMENU(wParam)) {
                ((VPopupMenu *)popup)->selected();
            }
            else {
                VPopupMenu *pm = findPopupMenuHandle
                                    ((VPopupMenu *)popup, HMENU(wParam));
                if (pm != 0) {
                    pm->selected();
                    wasDone = 1;
                }
            }
        }
        break;

    case WM_MENUSELECT:
        {
#if defined(CV_WIN32)
            UINT    uItem       = (UINT) LOWORD(wParam);    // id of selection
            UINT    fuFlags     = (UINT) HIWORD(wParam);    // menu flags
            HMENU   hMenu       = (HMENU) lParam;           // menu w/selectn
			if ((fuFlags == 0xFFFF) && (hMenu == 0)) {
				//
				// Windows has close the menu.
				//
				if (wnId->isA(VAppViewCls)) {
					((VAppView *)wnId)->displayPrompt(0);
				}
				break;
			}
#else
            UINT    uItem       = (UINT) wParam;            // id of selection
                                                            // or popup handle
            UINT    fuFlags     = (UINT) LOWORD(lParam);    // menu flags
            HMENU   hMenu       = (HMENU) HIWORD(lParam);   // menu w/selectn   
#endif
            if (wnId->isA(VAppViewCls)) {
                VString     *msgstr = 0;
                VMenuItem   *mitm = 0;
                VPopupMenu  *mp = 0;

                if (fuFlags & MF_POPUP) {
                    // send popup message
#if defined(CV_WIN32)
                    mp = (VPopupMenu *)((VAppView *)wnId)->menuOf(
                                                    GetSubMenu(hMenu, uItem));
#else
                    mp = (VPopupMenu *)((VAppView *)wnId)->menuOf((HMENU)uItem);
#endif
                    if (mp != 0) {
                        msgstr = &(mp->getPrompt());
                    }
                }
                else {
                    // send menu item message
                    if (fuFlags & MF_SEPARATOR) {
                        msgstr = 0;
                    }
                    else if (fuFlags & MF_SYSMENU) {
                        msgstr = 0;
                    }
                    else {
                        mitm = ((VAppView *)wnId)->itemOf(uItem);
                        if (mitm != 0) {
                            msgstr = &(mitm->getPrompt());
                        }
                    }
                }

                ((VAppView *)wnId)->displayPrompt(msgstr);

                if (menuSelClient != 0 && menuSelMethod != NIL_METHOD) {
                    (menuSelClient)->perform(menuSelMethod, (long) mp, 
                                                            (long) mitm);
                }
            }

#if 0   // !!!jmd
            if (wnId->isA(VAppViewCls)) {
                VMenuItem *mitm = 0;
                VPopupMenu *mp = 0;

                VMenuItem   empty("", 0, NIL_METHOD);
                VMenuItem   sys("", 0, NIL_METHOD);

                if (fuFlags & MF_POPUP) {
                    // send popup message
#   if defined(CV_WIN32)
                    mp = (VPopupMenu *)((VAppView *)wnId)->menuOf(
                                                    GetSubMenu(hMenu, uItem));
#   else
                    mp = (VPopupMenu *)((VAppView *)wnId)->menuOf((HMENU)uItem);
#   endif
                }
                else {
                    // send menu item message
                    if (fuFlags & MF_SEPARATOR) {
                        mitm = &empty;
                    }
                    else if (fuFlags & MF_SYSMENU) {
                        mitm = &sys;
                    }
                    else {
                        mitm = ((VAppView *)wnId)->itemOf(uItem);
                    }
                }

                ((VAppView *)wnId)->menuHighlighted(mp, mitm);
            }
#endif //   0   !!!jmd
        }
        break;  //  end of WM_MENUSELECT handling

#if defined(CV_WIN32)
    case WM_NOTIFY:
        {
            LPNMHDR ptrNMHDR = (LPNMHDR) lParam;
            HWND    hwndCtl  = ptrNMHDR->hwndFrom;
            int		wNotifyCode = ptrNMHDR->code;
            if (hwndCtl != 0) {
                //  control command
                VControl *cw;
                if ((cw = (VControl *) getThis(hwndCtl)) && 
                    cw->isA(VControlCls)) {

                    // fill up event structure
                    es = (void *) lParam;

                    // notify control
                    wasDone = cw->notify(wNotifyCode);
                }
                break;
            }
        }
        break;
#endif

    case WM_COMMAND:
        {
#if defined(CV_WIN32)
            WORD    wID         = LOWORD(wParam);           
            WORD    wNotifyCode = HIWORD(wParam);
            HWND    hwndCtl     = (HWND) lParam;
#else
            WORD    wID         = (WORD) wParam;            
            WORD    wNotifyCode = HIWORD(lParam);
            HWND    hwndCtl     = (HWND) LOWORD(lParam);
#endif
#if 1
            // if the command is from an accelerator
            if (wNotifyCode == 1 && hwndCtl == 0) {
                if (doKey(wID)) {
                    break;
                }
            }
#endif
            if (hwndCtl != 0) {
                //  control command

                VControl *cw;
                if ((cw = (VControl *) getThis(hwndCtl)) && 
                    cw->isA(VControlCls)) {

                    wasDone = cw->notify(wNotifyCode);
                }
                break;
            }
            
            //  menu command

            switch (wNotifyCode) {
            default:
                break;

            case 0: // VMenu selection
            case 1: // VMenu selection via accelerator
                {
                    VMenuItem *mitm = 0;
                    if (wnId->isA(VAppViewCls)) {
                        if (mitm = ((VAppView *)wnId)->itemOf(wID)) {
                            wasDone =  mitm->selected();
                        }
                    }
                    if (!mitm && popup) {
                        if (mitm = popup->itemOf(wID)) {
                            wasDone =  mitm->selected();
                        }
                    }
                    if (hDlgWnd) {
                        HWND hd;
                        if (hd = GetDlgItem(hDlgWnd, wID)) {
                            VControl *cw;
                            if (cw = (VControl *)getThis(hd)) {
                                wasDone = cw->notify(0);
                            }
                        }
                    }
                }
                break;
            }   //  end of menu command notification code switch
        }   //  end of block for WM_COMMAND 
        break;

    case WM_DESTROY:
        if (hWnd == curshWnd) {
            // jmd-7/19/93
            if (cursWndId != 0) {
#ifndef CV_NOVECTOREVENTS
				if (!cursWndId->vectorEvent(VWindow::MouseExit))
#endif
                cursWndId->mouseExit(buttonStatus());
            }
            curshWnd = 0;
            cursWndId = 0;
        }

        // If the last AppView is being destroyed, post a quit to terminate
        // the message loop. We don't have to check the class of winId, 
        // because only VAppViews are in this collection and the return
        // will tell us if an VAppView was found.
        if (0 != appViews.created.removeSame(wnId)) {
            if (0 == appViews.created.count()) {
                PostQuitMessage(appResult);
            }
        }

        wnId->destroyed();
        unsetThis(wnId);
        wnId->hWnd = 0;
        break;

    case WM_MEASUREITEM:
        wasDone = measureItem(lParam);
        if (wasDone)
            msgReturn = TRUE;
        break;

    case WM_DRAWITEM:
        wasDone = drawItem(lParam);
        break;

    case WM_SYSCOLORCHANGE:
    {
        if (bkBrush) {
            DeleteObject((HGDIOBJ) bkBrush);
            bkBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        }
#ifdef CV_CTL3D
        if (threeDEffectsAvailable(V_CTL3D_EFFECTS)) {
            Ctl3dColorChange();
        }
#endif
		systemColorsChanged(wnId);
        break;
    }

    case WM_WININICHANGE:
    case WM_FONTCHANGE:
    {
		systemFontsChanged(wnId);
        break;
    }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
#ifdef CV_WIN32
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORMSGBOX:
    case WM_CTLCOLORSCROLLBAR:
    case WM_CTLCOLORSTATIC: {
#else   
    case WM_CTLCOLOR: {
#endif
		HWND		child_hwnd;
		HDC			child_hdc;
		VWindow	   *child_window;
		HBRUSH		hbrush;

#ifdef CV_WIN32
		child_hwnd	= HWND(lParam);
		child_hdc	= HDC(wParam);
#else   
		child_hwnd	= HWND(LOWORD(lParam));
		child_hdc	= HDC(wParam);
#endif
		if ((child_window = getThis(child_hwnd)) == 0) {
			break;
		}

		VColor&  child_foreground_color = child_window->pForeground;
		VBrush  *child_background_brush = child_window->pBackground;
		boolean	 child_has_explicit_foreground =
					!child_window->defaultForeground;
		boolean  child_has_explicit_background =
					child_background_brush != 0;

		// Set up the foreground.

		if (child_has_explicit_foreground) {
			SetTextColor(child_hdc, COLORREF(child_foreground_color));
		}

		// Set up the background.

		if (child_has_explicit_background) {
			hbrush = child_background_brush->getHndl();
			SetBkColor(child_hdc,
					   COLORREF(child_background_brush->background()));
			wasDone = 1;
		}
		else {
#ifdef CV_CTL3D
			//
			// Let CTL3D set the DC (wParam) first: it sets default
			// foreground and background colors. If background is
			// defined then let that override CTL3D. If we get here,
			// then the background is not defined so default to CTL3D.
			// CTL3D will set up defaults for foreground, background,
			// and text color, so let it do that first. Then, let
			// Views override the foreground and text colors.
			//
			if (child_window->has3dEffects(V_CTL3D_EFFECTS)) {
				//
				// Otherwise, no foreground or background specified,
				// so let CTL3D exercise its dominance.
				//
				hbrush = Ctl3dCtlColorEx(msg, wParam, lParam);
				if (child_has_explicit_foreground) {
					//
					// If CTL3D fails to yield a brush,
					// then default to the Views definition.
					//
					if (hbrush == 0) {
						if (bkBrush == 0) {
							bkBrush =
								CreateSolidBrush(GetSysColor(COLOR_WINDOW));
						}
						SetBkColor(child_hdc, GetSysColor(COLOR_WINDOW));
						hbrush = bkBrush;
					}
					wasDone = 1;
				}
				else {
					//
					// Return TRUE if CTL3D returned a brush.
					// It is important to return FALSE, if no
					// brush is returned to allow windows to take
					// the default action by calling DefWindowProc.
					//
					wasDone = hbrush != 0;
					goto WmCtlColorFinish;
				}
			}
#endif
			//
			// Here, we do not have an explicitly set (Views) background.
			// We will do nothing and take the default system background
			// color *unless* this is a static type control, in which
			// case we will change its background to be the same as its
			// parent's background, *or* unless this is a dialog in which
			// case will use the default system shaded background color.
			//
			boolean child_is_static, child_is_dlg;
			boolean child_is_edit, child_is_btn;
#ifdef CV_WIN32
			child_is_static	= msg == WM_CTLCOLORSTATIC;
			child_is_dlg	= msg == WM_CTLCOLORDLG;
			child_is_edit	= msg == WM_CTLCOLOREDIT;
			child_is_btn	= msg == WM_CTLCOLORBTN;
#else
			child_is_static	= HIWORD(lParam) == CTLCOLOR_STATIC;
			child_is_dlg	= HIWORD(lParam) == CTLCOLOR_DLG;
			child_is_edit	= HIWORD(lParam) == CTLCOLOR_EDIT;
			child_is_btn	= HIWORD(lParam) == CTLCOLOR_BTN;
#endif
			if (child_is_static) {
				WmCtlColorChildIsStatic:
				VWindow *parent_window = wnId;
				while (1) {
					VBrush *parent_background_brush =
							parent_window->getBackground();
					if (parent_background_brush != 0) {
						SetBkColor(child_hdc,
								   COLORREF(parent_background_brush->
								   			background()));
						hbrush = parent_background_brush->getHndl();
						wasDone = 1;
						break;
					}
					else {
						UINT ctlcolor = parent_window->getCtlColor();
						boolean parent_is_static, parent_is_dlg;
#ifdef CV_WIN32
						parent_is_static = ctlcolor == WM_CTLCOLORSTATIC;
						parent_is_dlg = ctlcolor == WM_CTLCOLORDLG;
#else
						parent_is_dlg = ctlcolor == CTLCOLOR_STATIC;
						parent_is_dlg = ctlcolor == CTLCOLOR_DLG;
#endif
						if (parent_is_static) {
							if (parent_window->isTopLevel()) {
								break;
							}
							else {
								parent_window = parent_window->getParent();
								if (parent_window == 0) {
									break;
								}
							}
						}
						else {
							if (parent_is_dlg) {
								if (wnId->has3dEffects()) {
									SetBkColor
										(child_hdc,
										 COLORREF(VColor::
										 		  system(VColorScheme::
												  		 ShadedBackground)));
									hbrush = VBrush::
											 system(VColorScheme::
											 		ShadedBackground).
													getHndl();
									wasDone = 1;
									break;
								}
							}
							break;
						}
					}
				}
			}
			else if (child_is_dlg) {
				if (wnId->has3dEffects()) {
					SetBkColor(child_hdc,
							   COLORREF(VColor::
							   			system(VColorScheme::
											   ShadedBackground)));
					hbrush = VBrush::system(VColorScheme::
											ShadedBackground).getHndl();
					wasDone = 1;
				}
			}
			else if (child_is_edit) {
				if (!IsWindowEnabled(child_hwnd)) {
					goto WmCtlColorChildIsStatic;
				}
				else if (((VEditBox *)child_window)->isReadOnly()) {
					goto WmCtlColorChildIsStatic;
				}
			}
			else if (child_is_btn) {
#ifdef CV_WIN32
				if (child_window->getCtlColor() == WM_CTLCOLORSTATIC)
#else
				if (child_window->getCtlColor() == CTLCOLOR_STATIC)
#endif
				{
					goto WmCtlColorChildIsStatic;
				}
				else {
					SetBkColor(child_hdc,
							   COLORREF(VColor::
							   			system(VColorScheme::
											   ShadedBackground)));
					hbrush = VBrush::system(VColorScheme::
											ShadedBackground).getHndl();
					wasDone = 1;
				}
			}
		}

	WmCtlColorFinish:

		if (wasDone) {
			msgReturn = long(hbrush);
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
    case WM_ERASEBKGND:
		if (!wnId->isBeingDeleted()) {
            if (wnId->refCount++ == 0) {
                wnId->hDC = HDC(wParam);
            }
            if (!wnId->autoErased) {
                wasDone = wnId->erased();
            }
			//
            // If Views didn't handle background erasing,
			// then defer to 3d effects.
			//
            if (!wasDone && wnId->has3dEffects()) {
                HBRUSH hbrush = 0;
                HWND   parent_hwnd;
				//
                // If I'm a top level window, then
				// I get to determine my background color,
                // otherwise my parent determines my background color.
				//
				UINT ctlcolor = wnId->getCtlColor();
				if (ctlcolor != 0xFFFF) {
	                if (wnId->isTopLevel()) {
	                    parent_hwnd = hWnd;
	                }
	                else {
#if defined(CV_WIN32)
	                    parent_hwnd = HWND(GetWindowLong
										   (hWnd, GWL_HWNDPARENT));
#else
	                    parent_hwnd = HWND(GetWindowWord
										   (hWnd, GWW_HWNDPARENT));
#endif
	                    cv_assert(0 != parent_hwnd);
					}
#if defined(CV_WIN32)
					hbrush = HBRUSH(SendMessage(parent_hwnd, ctlcolor,
												wParam, LPARAM(hWnd)));
#else
					hbrush = HBRUSH(SendMessage(parent_hwnd, WM_CTLCOLOR,
												wParam,
												MAKELPARAM(hWnd, ctlcolor)));
#endif
					if (hbrush != 0) {
						RECT r;
						GetClientRect(hWnd, &r); r.right++; r.bottom++;
						FillRect(HDC(wParam), &r, hbrush);
						wasDone = TRUE;
						msgReturn = 1;
					}
				}
			}

			if (--wnId->refCount == 0) {
				wnId->hDC = 0;
			}
		}
		break;

#if defined(CV_CTL3D)
    case WM_NCPAINT:
    case WM_NCACTIVATE: {
        VWinInfo        winInfo;
        wnId->getInfo(winInfo);

        // Ctl3d will draw the frame, improperly, for windows that don't have 
        // WS_EX_DLGMODALFRAME (thick modal frame), so only allow ctl3d to 
        // draw if this condition is met.
        
        if (wnId->has3dEffects(V_CTL3D_EFFECTS) && (winInfo.exStyle & WS_EX_DLGMODALFRAME)) {
            msgReturn = Ctl3dDlgFramePaint(hWnd, msg, wParam, lParam);
            return TRUE;
        }
        break;
    }
#endif

    case WM_PAINT:
        if (wnId->fromRsc) {
            break;
        }

        // int i;
        // HANDLE h = 0;
        // if (wnId->refCount) {
        //  i = SaveDC(wnId->hDC);
        //  h = wnId->hDC;
        // }

        wnId->hDC = BeginPaint(hWnd, (LPPAINTSTRUCT)&ps);

        wnId->refCount++;
        exposed = TRUE;

#ifndef CV_NOVECTOREVENTS
    	if (wnId->vectorEvent(VWindow::Paint)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->paint();
		}
#else
        wasDone = wnId->paint();
#endif

        if (!wasDone) {
            InvalidateRect(hWnd, (LPRECT)&ps.rcPaint, 0);
        }

        EndPaint(hWnd, (LPPAINTSTRUCT)&ps);
        //wnId->hDC = 0;
        //wnId->refCount--;
        if (!-- wnId->refCount) {
            wnId->hDC = 0;
        }
        // if (wnId->refCount)
        //  RestoreDC(h,i);
        // wnId->hDC = h;
        exposed = FALSE;
        break;

    case WM_QUERYENDSESSION:
        if (wnId->isA(VAppViewCls)) {
#ifndef CV_NOVECTOREVENTS
	    	if (wnId->vectorEvent(VWindow::Quit)) {
				return TRUE;
			}
#endif
            return ((VAppView *)wnId)->quit();
        }
        break;

    case WM_SETFOCUS:
    // If the control is a combobox then
    // we only set focus if the window that lost focus 
    // is NOT the editControl part of the combo box.
    // That edit control will be a child of the
    // combobox.
    	// if (!IsWindowEnabled(hWnd) || !IsWindowVisible(hWnd)) {
		if (wnId->beingDeleted || wnId->beingDestroyed) {
	        focus = wnId;
			break;
		}
        else if (wnId->isA(VComboBoxCls)) {
            // wParam is the handle of the window loosing focus
            if (IsChild(wnId->hwnd(), (HWND) wParam)){
                break;
            }
        }
        else if (wnId->isA(VScrollBarCls) &&
                 !((VScrollBar *)wnId)->isPartOfView() &&
                 (!((VScrollBar *)wnId)->isTabStop() ||
                  (((VScrollBar *)wnId)->getPageSize() > 0))) {
            wasDone = 1;
            break;
        }
        focus = wnId;
#ifndef CV_NOVECTOREVENTS
    	if (wnId->vectorEvent(VWindow::GivenFocus)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->givenFocus();
		}
#else
        wasDone = wnId->givenFocus();
#endif
        break;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
    case WM_KILLFOCUS:
	{
    	// if (!IsWindowEnabled(hWnd) || !IsWindowVisible(hWnd)) {
		if (wnId->beingDeleted || wnId->beingDestroyed) {
	        focus = 0;
			break;
		}
        else if (wnId->isA(VComboBoxCls)) {
            // wParam is the handle of the window loosing focus
            if (IsChild(wnId->hwnd(), (HWND) wParam)){
                break;
            }
        } 
		PostMessage(wnId->hwnd(), WM_LOSTFOCUS, WPARAM(0), LPARAM(0));
        focus = 0;
#ifndef CV_NOVECTOREVENTS
    	if (wnId->vectorEvent(VWindow::ClearFocus)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->clearFocus();
		}
#else
        wasDone = wnId->clearFocus();
#endif
        break;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
	case WM_LOSTFOCUS:
	{
#ifndef CV_NOVECTOREVENTS
    	if (wnId->vectorEvent(VWindow::LostFocus)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->lostFocus();
		}
#else
        wasDone = wnId->lostFocus();
#endif
        break;
	}

    case WM_SYSKEYDOWN:
		wasDone = wnId->key(K_Mod_Alt, char(~K_Mod_Alt & wParam));
		break;

    case WM_SYSKEYUP:
		wasDone = wnId->keyUp(K_Mod_Alt, char(~K_Mod_Alt & wParam));
		break;

    case WM_KEYDOWN:
        if (!wasDone) {
            key = getVirtKey(wParam);
            if (!xlat) {
                wasDone = wnId->key(key, (char)0);
            }
        }
        break;

    case WM_KEYUP:
        key = getVirtKey( wParam );
        wasDone = wnId->keyUp( key, (char) 0 );
        break;

    case WM_SETCURSOR:

        // if no active window, then there's nothing to do, so leave.
        if (GetActiveWindow() == 0) {
            break;
        }
             
#if defined(CV_WIN32)
        DWORD dwProcessId;
        (void) GetWindowThreadProcessId(GetActiveWindow(), &dwProcessId);
#endif

        // if the click occurred on our window and we're modal and we're not 
        // the currently running task, then bring the modal window to top to
        // trigger a WM_ACTIVATEAPP. Deactivated windows that are
        // not parent's of the modal window won't be brought to the top,
        // by windows. We need this for the multiple appView case.
        if (LOWORD(lParam) == WORD(HTERROR) && 
            HIWORD(lParam) == WORD(WM_LBUTTONDOWN) && 
            isModalWindowActive() &&
#if defined(CV_WIN32)
            GetCurrentProcessId() != dwProcessId
#else
            GetCurrentTask() != GetWindowTask(GetActiveWindow())
#endif
            ) {
            // find the window that was active when this app was last active
            // and bring it to the front. This works for non-Views modal
            // windows, like setup on the print dialog. All ancestors of the
            // modal window will follow to the front. This will generate
            // WM_ACTIVATEAPP;s and the other appViews can take action, then.
            //
            // NOTE: this is not the same as GetActiveWindow() which will
            // get the currently active window, which is not in this task.
            // We know that the current task is not us because of the above 
            // "if".
            //
            // NOTE: zero or one last active popup must exist if we were modal.
            // If one then its a modal dialog. If zero, then one of the
            // top level windows is modal.

            HWND hActiveWindow = 0;

            DO(appViews.created, VAppView, appView)
                HWND hwnd;
                hwnd = GetLastActivePopup(appView->hwnd());
                if (hwnd != appView->hwnd()) {
                   hActiveWindow = hwnd;
                    break;
                }
            END

            // no active popup windows (dialogs), then the top level window 
            // on the modal stack is the active window.
            if (0 == hActiveWindow) {
                hActiveWindow = ((VWindow *)modals->top())->hwnd();
            }

            BringWindowToTop(hActiveWindow);

            // prevents the beeping when a mouse down occurs on a disabled
            // window. We don't want to beep on the mouse down that brings
            // the app to the front.
            wasDone = TRUE;
        }
		if (winCursor != 0) {
			SetCursor(winCursor->getHndl());
			wasDone = TRUE;
		}
		else if ((wnId->pCursor != 0) && (LOWORD(lParam) == WORD(HTCLIENT))) {
			//
			// Already set in WM_MOUSEMOVE.
			//
			wasDone = TRUE;
		}
        break;

    case WM_CHAR:
        wasDone = wnId->key(key, (char)wParam);
        xlat = key = 0;
        break;

    case WM_SIZE:
        if (IsIconic(hWnd)) {
            VAppView *appView = (VAppView *)getThis(hWnd);
            if ((appView != 0) && (appViews.created.findSame(appView) != 0)) {
                wasDone = appView->iconized(LOWORD(lParam) , HIWORD(lParam));
            }
        }
        else {
#ifndef CV_NOVECTOREVENTS
			if (wnId->vectorEvent(VWindow::Resized)) {
				wasDone = TRUE;
			}
			else {
            	wasDone =  wnId->resized(LOWORD(lParam), HIWORD(lParam));
			}
#else
            wasDone =  wnId->resized(LOWORD(lParam), HIWORD(lParam));
#endif
			if (wnId->isRedrawUponResize()) {
				InvalidateRect(hWnd, NULL, TRUE);
			}
        }
        break;

    case WM_MOVE:
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::Moved)) {
			wasDone = TRUE;
		}
		else {
        	wasDone = wnId->moved(LOWORD(lParam) , HIWORD(lParam));
		}
#else
        wasDone = wnId->moved(LOWORD(lParam) , HIWORD(lParam));
#endif
        break;

    case WM_VSCROLL:
    case WM_HSCROLL:
        wasDone = scrollItem(hWnd, msg, wParam, lParam, wnId);
        break;

    case WM_QUERYNEWPALETTE:
	{
		if (paletteWindows != 0) {
			VWindow *window = 0;
			if (focus != 0) {
				window = (VWindow *)paletteWindows->findSame(focus);
			}
			if (window == 0) {
				window = (VWindow *)paletteWindows->idAt(0);
			}
			if (window != 0) {
				window->isPaletteChanging = 1;
				window->paletteChanged(TRUE);
				window->isPaletteChanging = 0;
			}
		}
		break;
	}

    case WM_PALETTECHANGED:
	{
		if (paletteWindows != 0) {
			DO (*paletteWindows, VWindow, window)
				if (!window->isPaletteChanging) {
					window->isPaletteChanging = 1;
					window->paletteChanged(FALSE);
					window->isPaletteChanging = 0;
				}
			END
		}
		break;
	}

    case WM_GETMINMAXINFO:
    {
        if (wnId->constrainedMinimumWidth > 0) {
            ((MINMAXINFO *)lParam)->ptMinTrackSize.x =
                wnId->constrainedMinimumWidth;
        }
        if (wnId->constrainedMinimumHeight > 0) {
            ((MINMAXINFO *)lParam)->ptMinTrackSize.y =
                wnId->constrainedMinimumHeight;
        }
        if (wnId->constrainedMaximumWidth > 0) {
            ((MINMAXINFO *)lParam)->ptMaxTrackSize.x =
                wnId->constrainedMaximumWidth;
        }
        if (wnId->constrainedMaximumHeight > 0) {
            ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
                wnId->constrainedMaximumHeight;
        }
        break;
    }

#if defined(WM_HELP)

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
    case WM_HELP:
	{
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::ContextHelp)) {
			wasDone = TRUE;
			break;
		}
#endif
		wasDone = wnId->contextHelp();
		break;
	}

#endif

#if defined(WM_CONTEXTMENU)

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
    case WM_CONTEXTMENU:
	{
#ifndef CV_NOVECTOREVENTS
	    if (wnId->vectorEvent(VWindow::ContextMenu)) {
			wasDone = TRUE;
			break;
		}
#endif
		wasDone = wnId->contextMenu();
		break;
	}

#endif

#if defined(CV_IMOUSE) && defined(WM_MOUSEWHEEL)

	case WM_MOUSEWHEEL:
		wasDone = VIntelliMouse::handleWheelMessage(wParam, lParam);
		break;

#endif

    default:
        if (msg >= WM_USER) {
#			if defined(CV_IMOUSE)
				if (msg == VIntelliMouse::wheelMessageId) {
					wasDone = VIntelliMouse::
							  handleWheelMessage(wParam, lParam);
				}
				else {
					wasDone = wnId->user(msg, wParam, lParam);
				}
#			else
				wasDone = wnId->user(msg, wParam, lParam);
#			endif
		}
		else if ((msg >= WM_DDE_FIRST) && (msg <= WM_DDE_LAST)) {
            wasDone = wnId->user(msg, wParam, lParam);
        }
        break;
    }

    if (wnIdFromRsc) {
        msgReturn = wasDone;
    }
    return wasDone;
}

//----------------------------------------------------------------------------
//
// Deletes an appView if it was created inside VNotifier::start(), else
// it just hides it.
//
void VNotifier::deleteAppView(VAppView *appView)
{
	if (appView->isA(VMdiAppViewCls)) {
		if (((VMdiAppView *)appView)->clientOf()->hwnd() == hClient) {
			hClient = 0;
		}
	}
    // if not created in start, then destroy the presentation space and
    // leave the object alone, else delete the object.
    if (!appView->wasCreatedInStart()) {
		if (appView->menu != 0) {
			appView->hide();
			appView->refreshMenu(FALSE);
			if (appView->isA(VStdMdiAppViewCls)) {
				VStdMdiAppView *stdmdiappview = (VStdMdiAppView *)appView;
				if ((stdmdiappview->menu == stdmdiappview->menuBar) &&
					 stdmdiappview->menuBarIsMine) {
					delete appView->menu;
					appView->menu = 0;
				}
			}
			else {
				delete appView->menu;
				appView->menu = 0;
			}
			if (appView->hWnd != 0) {
				SetMenu(appView->hWnd, 0);	
			}
		}
        destroyWin(appView);
    }
    else {
        delete appView;
    }
}

//----------------------------------------------------------------------------
// Destroy the GUI system presentation window associated with the VWindow
// object 'win'.
//
void VNotifier::destroyWin(VWindow *win)
{
    if (win->hWnd) {
        if (win->hWnd == curshWnd) {
            VAppView *appView = (VAppView *) appViews.created.findSame(win);

            if (appView && curshWnd != appView->hwnd()) {
                curshWnd = appView->hwnd();
                cursWndId = appView;
            }
            else {
                curshWnd = 0;
                cursWndId = 0;
            }
        }

        if (win == dlgWin) {
            hDlgWnd = 0;
            dlgWin = 0;
        }

#if !defined(CV_WIN32)
//  (pat--see comments in creatWin())

        HINSTANCE h = win->hInstance;
#endif

        if (IsWindow(win->hWnd) && win->hWnd != hClient) {
            win->hide();
            enum WinType type = win->winType();
            if (type == WinMdiChild) {
                //
                // This causes a crash in certain situations.
                // !!!dgm, 1/31/94.
                //
                // ToDo!!!
                // 1. Hide the MDI child.
                // 2. Send a WM_MDIRESTORE.
                // 3. Send a WM_MDIDESTROY.
                //
                // SendMessage(GetParent(win->hWnd),
                //          WM_MDIDESTROY, (WPARAM) win->hWnd, (LPARAM) 0);
                //
				boolean saved_flag = win->isBeingDestroyed();
				win->beingDestroyed = TRUE;
                PostMessage(win->hWnd, WM_CLOSE, 0, 0L);
				win->beingDestroyed = saved_flag;
                return;
            }
            else {  
                if (IsWindow(win->hWnd)){ // added by ed
                    DestroyWindow(win->hWnd); 
                }
            }
        }

#if !defined(CV_WIN32)
        if (h != 0) {
            // free global shared memory block
            GlobalFree(h); 
        }
#endif

        unsetThis(win);     // redundant: done on WM_DESTROY
        win->hWnd = 0;      // redundant: done on WM_DESTROY
    }
}

//----------------------------------------------------------------------------
boolean VNotifier::recreateWin(VWindow *win)
{
    int     x, y, w, h;
    char    text_buffer[2048], *text_buffer_ptr = 0;
    boolean is_control = (win->winType() == WinControl);
    boolean is_disabled = !win->isEnabled();

    win->getRel(&x, &y, &w, &h);
    if (win->isA(VComboBoxCls)) {
        h = ((VComboBox *)win)->getDropDownHeight();
    }

    if (is_control) {
        if (((VControl *)win)->getText(text_buffer, 2047) > 0) {
            text_buffer_ptr = text_buffer;
        }
    }

    // Determine the placement of this window in the Z-order
    // so that we can place the new window at the same position.

    HWND prev_hWnd = GetWindow(win->hWnd, GW_HWNDPREV);
    if (prev_hWnd == 0) {
        prev_hWnd = HWND_TOP;
    }

    // get window properties before we delet it
    boolean hasborder = win->hasBorder();

    int cntrlid = GetDlgCtrlID(win->hWnd);
    DestroyWindow(win->hWnd);
    unsetThis(win);     // redundant: done on WM_DESTROY
    VWinInfo winfo;
    win->getInfo(winfo);
    long style = winfo.style;

    if (hasborder) {
        style |= WS_BORDER;
    }

#if !defined(CV_WIN32)
    //  (pat--see createWin() for explanation)

    HINSTANCE hFudgeInst = hInst;

    if (strcmp(winfo.wClass, "edit") == 0) {

        long nBytes = 1024L;
        HGLOBAL aGhEditHeap = 
        GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE ,nBytes);

        if (aGhEditHeap != 0) {
            char *p = (char *) GlobalLock(aGhEditHeap);
            if (!p) {   //  could not lock it down to zero it, free it
                GlobalFree(aGhEditHeap);
            }
            else {      //  zero memory contents
                register char *pp;
                for (pp = p; nBytes > 0; nBytes--) {
                    *pp++ = 0;
                }
                GlobalUnlock(aGhEditHeap);
                hFudgeInst = (HINSTANCE) aGhEditHeap;
                win->hInstance = hFudgeInst;
            }
        }
        //
        //  else the edit class window uses the app's local heap
        //
    }

    win->hWnd = CreateWindow(   (LPCSTR) winfo.wClass, 
                                (LPCSTR) 0, 
                                (DWORD) style, 
                                x, y, w, h, 
                                win->parent->hWnd,
                                (HMENU) cntrlid, 
                                hFudgeInst, 
                                (LPVOID) 0);


#else
    win->hWnd = CreateWindow(   (LPCSTR) winfo.wClass,
                                (LPCSTR) 0, 
                                (DWORD) style, 
                                x, y, w, h, 
                                win->parent->hWnd,
                                (HMENU) cntrlid, 
                                hInst, 
                                (LPVOID) 0);

#endif          //  CV_WIN32

    if (is_control) {
        VControl *c = (VControl *)win;

        //  replace window procedure of control 
        //  (lpfnWindowProc points to CTalkWndProc --see winstart.cpp)

        if (strcmp(winfo.wClass, pCTwindow) != 0) {
#ifdef CV_CTL3D
            // if object has 3d effects and it was subclassed by ctl3d, 
            // then sub class with our clt3d
            if (win->has3dEffects(V_CTL3D_EFFECTS) && Ctl3dSubclassCtl(win->hWnd)) {
                cv_assert(lpfnCtl3dCtlWndProc != 0);
                c->defProc = (WNDPROC) 
                    SetWindowLong(win->hWnd, GWL_WNDPROC, (LONG)lpfnCtl3dCtlWndProc);
            }
            else
#endif
            {
            c->defProc = (WNDPROC)
                SetWindowLong(win->hWnd, GWL_WNDPROC, (LONG) lpfnWindowProc);
            }
        }

        if (c->isA(VComboBoxCls)) {
            if (c->style.contains(StyleComboEditDrop) || 
                 c->style.contains(StyleComboEdit)) {

                POINT pt; 
                pt.x = pt.y = 6;
                HWND combobox_edit_hwnd = ChildWindowFromPoint(win->hWnd, pt);

                //  replace window procedure of combo box's edit control 
                //  (lpfnComboEditProc points to CTalkComboEditProc--
                //  see winstart.cpp)
                if (lpfnSaveComboEditProc == 0 && combobox_edit_hwnd != 0) {
                  lpfnSaveComboEditProc = 
                     (WNDPROC)GetWindowLong
                              (combobox_edit_hwnd, GWL_WNDPROC);
                }
                SetWindowLong(combobox_edit_hwnd,
                              GWL_WNDPROC, (LONG) lpfnComboEditProc);
            }
            // subclass the combobox to retrive the comboLbox listbox
            // owned by the desktop. Its class is comboLBox.
            lpfnSaveComboProc = (WNDPROC)GetWindowLong(win->hWnd, GWL_WNDPROC);
            SetWindowLong(win->hWnd, GWL_WNDPROC, (LONG)lpfnComboProc);
        }
    }

    SetWindowPos(win->hWnd, prev_hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    if (is_disabled) {
        win->disable();
    }
    setThis(win);
    if (text_buffer_ptr != 0) {
        ((VControl *)win)->putText(text_buffer_ptr);
    }

    return TRUE;
}

//----------------------------------------------------------------------------
// Create the GUI system presentation window for VWindow object 'win'
// with dimensions 'x','y','w', and 'h'. This function should not be directly
// called.
//
boolean VNotifier::createWin(VWindow *win, int x, int y, int w, int h)
{
	extern VClass *VTreeListBoxCls;
    HWND hWnd, hPWnd = 0;
    long style;
    long exStyle;
    char altClassName[64];

    if (!tmpWin){
        tmpWin=new VWindow();
        tmpWin->hWnd = 0;
        tmpWin->parent = 0;
        tmpWin->disableFocus();
    }

    tmpWin->parent = win;
    if (!win) {
        return FALSE;
    }

    if (!winset) {
        winset = new VSet(200);
    }

    enum WinType type = win->winType();
    VWindow *pwin = win->parent;
    hWnd = win->hWnd;
    if (pwin) {
        hPWnd = pwin->hWnd;
    }

    if (!hWnd) {
        WORD cntrlid = 0 ;
        VWinInfo winfo;
        win->getInfo(winfo);
        style = winfo.style;
        exStyle = winfo.exStyle;
        char *classname = winfo.wClass;

        if (!hPrevInst && !classesInitialized) {
			// initialize only with known classnames
			// all other registration is the responsibility of
			// the user

			// register default window proc
			_WndProcInit(pCTwindow, FALSE, CS_DBLCLKS);

			// register default dialog proc
			_WndProcInit((char *)dlgClassName.gets(), FALSE,
                         CS_SAVEBITS | CS_DBLCLKS);

			// register default mdi app proc
			strcpy(altClassName, pCTwindow);
			altClassName[0] = '_';   // MDI VFrame class name
			_WndProcInit(altClassName, TRUE, CS_DBLCLKS);

			classesInitialized = TRUE;
		}

		switch (type) {
		case WinMdiApp:
            strcpy(altClassName, classname);
            altClassName[0] = '_';   // MDI VFrame class name
            classname = altClassName;
			break;

		case WinMdiClient:
            break;

		case WinMdiChild:
            if (!classname) {
                classname = "VMdiView";
            }
            MdiChildProcInit(classname, CS_DBLCLKS);
            break;

        case WinControl:
            cntrlid = Id++;
            break;

        default:
            if (style & WS_POPUP) {
                cntrlid = 0;
            }
            break;
        }

		if (type != WinMdiChild) {
			if (x == -4999) { x = 0; }
			if (y == -4999) { y = 0; }
		}

        switch (type) {
        case WinMdiClient: {
                CLIENTCREATESTRUCT ccs;

                ccs.hWindowMenu = popup->hMenu;
                ccs.idFirstChild = 0x4000;

                hClient = hWnd = CreateWindow(  "MDICLIENT", 
                                                (LPCSTR) 0, 
                                                (DWORD) style, 
                                                x, y, w, h, 
                                                hPWnd, 
                                                (HMENU) 0, 
                                                hInst, 
                                                (LPVOID) &ccs);
                ShowWindow (hClient, SW_SHOW);
            }
            break;

        case WinMdiChild: {
				HWND hclient;
				if (pwin != 0) {
					if (pwin->isA(VMdiAppViewCls)) {
						VWindow *client_window =
								 ((VMdiAppView *)pwin)->clientOf();
						if (client_window != 0) {
							if ((hclient = client_window->hwnd()) == 0) {
								hclient = hClient;
							}
						}
						else {
							hclient = hClient;
						}
					}
					else if (pwin->isA(VMdiClientCls)) {
						if ((hclient = pwin->hwnd()) == 0) {
							hclient = hClient;
						}
					}
					else {
						hclient = hClient;
					}
				}
				else {
					hclient = hClient;
				}
                // Determine the topmost sibling, if any.
                VMdiView *sibling =
						  ((VMdiClient *)getThis(hclient))->activeChild();
                boolean was_maximized = sibling != 0 && sibling->isMaximized();
                boolean hide = !(style & WS_VISIBLE);
                boolean no_draw = TRUE; // prevent flashing

                MDICREATESTRUCT ms;
                ms.szClass = classname;
                VString st;
                ((VMdiView *)win)->getTitle(st);
                ms.szTitle = (char *)st.gets();
                ms.hOwner = hInst;
                if ((w == 0) && (h == 0)) {
                    x = y = w = h = CW_USEDEFAULT;
                }
				else {
					if (x == -4999) {
						x = CW_USEDEFAULT;
					}
					if (y == -4999) {
						y = CW_USEDEFAULT;
					}
				}
                ms.x = x;
                ms.y = y;
                ms.cx = w;
                ms.cy = h;
                ms.style = style;
                ms.lParam = 0L;
                if (no_draw && (was_maximized || hide)) {
                    //
                    // This and the below is a workaround for
                    // a documented MS/Windows bug (Q70080); it was
                    // snatched from the Development Network CD-ROM.
                    // dgm, 2/7/94.
                    //
                    SendMessage(hclient, WM_SETREDRAW,
                                (WPARAM)FALSE, (LPARAM)0L);
                }
                if (was_maximized && sibling->hWnd != 0) {
                    // For MS/Windows bug Q99411.
                    ShowWindow(sibling->hWnd, SW_RESTORE);
                }
                hWnd = (HWND) SendMessage(hclient, WM_MDICREATE, (WPARAM) 0,    
                                            (LPARAM) (LPMDICREATESTRUCT) &ms);
                if (was_maximized) {
                    if (!hide && no_draw) {
                        SendMessage(hclient, WM_SETREDRAW,
                                    (WPARAM)TRUE, (LPARAM)0L);
                    }
                    if (hWnd != 0) {
                        SendMessage(hclient, WM_MDIMAXIMIZE,
											 WPARAM(hWnd), LPARAM(0L));
                    }
                }
                if (hide) { 
                    if (sibling != 0) {
                        sibling->bringToTop();
                    }
                    if (hWnd != 0) {
                        ShowWindow(hWnd, SW_HIDE);
                    }
                    if (no_draw) {
                        SendMessage(hclient, WM_SETREDRAW,
                                    WPARAM(TRUE), LPARAM(0L));

                        // The following does not update the non-client
						// area of the MDI children.
                        if (pwin != 0) {
                            pwin->update();
                        }
                    }
                }
            }
            break;

        case WinControl: {
#if !defined(CV_WIN32)
                //  02/18/94 pat--what is going on here?
                //
                //  VEditBox objects and objects of its derived classes
                //  (VEditLine and VTextEditor) sit on top of MS Windows
                //  edit controls.  By default, Windows edit controls
                //  use the application's local heap. In addition to 
                //  occupying space in the local heap, edit controls 
                //  are limited to use of a maximum of 32K of memory.
                //
                //  Here, we force edit controls to use the global heap.
                //  Edit controls using the global heap can use more than
                //  32K of memory and do not clog up the application's 
                //  local heap.  We do this by allocating a 1K block of 
                //  global, shared memory and passing this to CreateWindow()
                //  as the instance handle argument.  This trick is part 
                //  of the literature for Windows 3.1.  It may not hold
                //  in future releases.
                //
                //  Since global, shared memory used by an edit control 
                //  must be freed explicitly, we need a way to tell 
                //  destroyWin() about it.  VNotifier is permitted by 
                //  VWindow to look at the private hInstance member of
                //  that class. This member is only for the benefit of the
                //  notifier.  Edit controls that use the global heap in
                //  this fashion store the handle to the global block in
                //  hInstance member of VWindow.
                //
                //  This is hacky, but the alternatives are not any cleaner:
                //  keep a collection in the notifier of windows whose
                //  edit controls use the global heap; or, pull the instance
                //  handle from the internal Windows window structure
                //  (requires the selector value of the USER.DLL DGROUP).
                //  (actually this second alternative is how it now works)
                
                HANDLE hFudgeInst = hInst;

                if (strcmp(classname, "edit") == 0) {

                    long nBytes = 1024L;
                    HANDLE aGhEditHeap = 
                    GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE ,nBytes);

                    if (aGhEditHeap) {
                        char *p = (char *) GlobalLock(aGhEditHeap);

                        if (!p) {
                            //  could not lock it down to zero it, free it

                            GlobalFree(aGhEditHeap);
                        }
                        else {
                            //  zero memory contents

                            register char *pp;
                            for (pp = p; nBytes > 0; nBytes--) {
                                *pp++ = 0;
                            }

                            GlobalUnlock(aGhEditHeap);
                            hFudgeInst = aGhEditHeap;
                            win->hInstance = (HINSTANCE) hFudgeInst;
                        }
                    }
                    //
                    //  else the edit class window uses the app's local heap
                    //
                }
                
                hWnd = CreateWindowEx(  exStyle,
                                        (LPCSTR) classname, 
                                        (LPCSTR) 0, 
                                        (DWORD) style, 
                                        x, y, w, h, 
                                        hPWnd,
                                        (HMENU) cntrlid, 
                                        (HINSTANCE) hFudgeInst, 
                                        (LPVOID) 0);   
#else
                hWnd = CreateWindowEx(  exStyle,
                                        (LPCSTR) classname, 
                                        (LPCSTR) 0, 
                                        (DWORD) style, 
                                        x, y, w, h, 
                                        hPWnd,
                                        (HMENU) cntrlid, 
                                        hInst, 
                                        (LPVOID) 0);   
#endif          //  CV_WIN32

                if (hWnd != 0) {
                    VControl *c = (VControl *)win;

                // do not ctl3d sub class if the class is the CTalkWndProc class
                    if (strcmp(classname, pCTwindow) != 0) {
#ifdef CV_CTL3D
                        // if object has 3d effects and it was subclassed by ctl3d, 
                        // then sub class with our clt3d
                        if (win->has3dEffects(V_CTL3D_EFFECTS) && Ctl3dSubclassCtl(hWnd)) {
                            c->defProc = (WNDPROC) 
                                SetWindowLong(hWnd, GWL_WNDPROC, (LONG)lpfnCtl3dCtlWndProc);
                        }
                        
                        // else...
                        else
#endif
                        // sub class the native MSW control
                        {
                            c->defProc = (WNDPROC) 
                                SetWindowLong
                                    (hWnd, GWL_WNDPROC, (LONG)lpfnWindowProc);
                        }
                    }

                    if (c->isA(VComboBoxCls)) {
                        if (c->style.contains(StyleComboEditDrop) ||
                             c->style.contains(StyleComboEdit)) {
							// if you use a point of (1,1)
							// on win95 you miss the edit control.
							// Not so for winNT!!
                            POINT pt; pt.x = pt.y = 6;
                            HWND combobox_edit_hwnd;
                            combobox_edit_hwnd = ChildWindowFromPoint(hWnd, pt);
                            if (lpfnSaveComboEditProc == 0 && combobox_edit_hwnd != 0) {
                              lpfnSaveComboEditProc = 
                                  (WNDPROC)GetWindowLong
                                           (combobox_edit_hwnd, GWL_WNDPROC);
                            }
                            SetWindowLong(combobox_edit_hwnd,
                                          GWL_WNDPROC, (LONG)lpfnComboEditProc);
                        }

                    // subclass the combobox to retrive the comboLbox listbox
                    // owned by the desktop. Its class is comboLBox.
                        lpfnSaveComboProc = 
                        (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);
                        SetWindowLong(hWnd, GWL_WNDPROC, 
                        (LONG)lpfnComboProc);
                    }
					win->setDefaultFont = VFont::Default;
                } // if 0 != hWnd
            }
            break;

        default: {
                hWnd = CreateWindowEx(exStyle,
                                      (LPCSTR)classname,
                                      (LPCSTR)0, style,
                                      x, y, w, h,
                                      hPWnd,
                                      (HMENU)cntrlid,
                                      hInst,
                                      (LPVOID)0);

                if (win->isA(VAppViewCls)) {
                    appViews.created.add(win);
                }
				if (win->isA(VTreeListBoxCls)) {
					win->setDefaultFont = VFont::Icon;
				}

            } // default

            break;

        } //switch
    }
          
    tmpWin->parent = 0;

    if (hWnd != 0) {
        if (!win->hWnd || win->fromRsc) {
            win->hWnd = hWnd;
            setThis(win);
            if (pwin) {
                pwin->addChild(win);
            }
            win->parent = pwin;
        }
    } // if hWnd != 0

    return hWnd ? TRUE : FALSE;
}

//----------------------------------------------------------------------------
void VNotifier::appViewDeleted(VAppView *appView)
{
    appViews.constructed.removeSame(appView);
}

//----------------------------------------------------------------------------
// Called by all VAppView ctor's. Used when a VAppview is constructed,
// without regard to whether the presentation object was created.
void VNotifier::appViewConstructed(VAppView *appView)
{
    appViews.constructed.add(appView);
    appView->inStartCreation = inStart;
}

//----------------------------------------------------------------------------
// Register an array of callback methods 'sels' for the array
// of virtual key codes 'keys' for the client object 'client'.  The array
// of keys is terminated by the K_NULL key.
//
void VNotifier::setAccelKeys(int *keys, method *sels, VObject *client)
{
    int i = 0;
    while (keys[i]!= K_NULL) {
        setAccel(keys[i], sels[i], client);
        i++;
    }
}

//----------------------------------------------------------------------------
// Return a VRectangle containing the minimum rectangle enclosing the
// area exposed by the current exposure event.  The coordinates
// of the rectangle are in the local coordinates of the VWindow object that
// just received the exposure event.  If there is no active exposure event,
// then return 0.
//
VRectangle *VNotifier::exposedRect()
{
    return (exposed ?
            (new VRectangle(    Corners, 
                                int(ps.rcPaint.left), 
                                int(ps.rcPaint.top), 
                                int(ps.rcPaint.right), 
                                int(ps.rcPaint.bottom)))
            : 0);
}

//----------------------------------------------------------------------------
// Fill a VRectangle with the minimum rectangle enclosing the
// area exposed by the current exposure event.  The coordinates
// of the rectangle are in the local coordinates of the VWindow object that
// received the exposure event.  If there is no active exposure event,
// then return FALSE.
//
boolean VNotifier::getExposedRectangle(VRectangle& rect)
{
    if (exposed) {
        rect.set(Corners, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

//----------------------------------------------------------------------------
// Return the minimum rectangle enclosing the
// area exposed by the current exposure event via the integers
// pointed to by top, left, bottom, and right.  The coordinates
// of the rectangle are in the local coordinates of the VWindow object that
// received the exposure event.  If there is no active exposure event,
// then return FALSE.
//
boolean VNotifier::getExposedRectangle(int *left, int *top, int *right, int *bottom)
{
    if (exposed) {
        *left = ps.rcPaint.left;
        *top = ps.rcPaint.top;
        *right = ps.rcPaint.right;
        *bottom = ps.rcPaint.bottom;
        return TRUE;
    }
    else {
        return FALSE;
    }
}

//----------------------------------------------------------------------------
// Set the mouse cursor to the hourglass style and capture all mouse
// events for the window currently containing the cursor.
//
void VNotifier::beginWait()
{
    setCursor(HourGlass);

    // jmd 02/26/94
    if (curshWnd != 0) {
        SetCapture(curshWnd);
    }
    waitState = TRUE;
}

//----------------------------------------------------------------------------
// Set the mouse cursor to the hourglass style and capture all mouse events
// for the VWindow 'win'.
//
void VNotifier::beginWaitFor(VWindow *win)
{
    setCursor(HourGlass);
    // jmd 02/26/94
    if (win->hWnd != 0) {
        SetCapture(win->hWnd);
    }
    waitState = TRUE;
}

//----------------------------------------------------------------------------
// Return the current (x, y) position of the mouse cursor into
// '*x','*y', respectively. The cursor position is returned in screen
// coordinates.  Note: screen coordinates can be mapped onto a VWindow
// object's local coordinate space by using the mapToWin(int *, int *)
// member of the VWindow class.
//
void VNotifier::getCursorPos(int *x, int *y)
{
    POINT p;
    GetCursorPos((LPPOINT)&p);

    //  pat--under CV_WIN32 POINT coordinates are longs, we are truncating! 

    *x = (int) p.x;
    *y = (int) p.y;
}

//----------------------------------------------------------------------------
// Return TRUE if the left mouse button is down or return FALSE if the
// button is up.
//
boolean VNotifier::buttonStatus()
{
    return (GetKeyState(VK_LBUTTON) < 0) ? TRUE : FALSE;
}

//----------------------------------------------------------------------------
// Capture all mouse events for the VWindow 'win'.
//
void VNotifier::captureMouseFor(VWindow *win)
{
    if (win != 0) {
        captureWindow = win;
        SetCapture(win->hWnd);
    }
}

//---------------------------------------------------------------------------- 
// Process the callback method for the virtual keycode 'key'.
//
boolean VNotifier::doKey(int key)
{
    // Accelerator keys are suspended
    if (dontKey) {
        return TRUE;
    }
     
    VAccelerator *ap = 0, a(key);
     
    if (dlgWin && !dlgWin->dontKey && dlgWin->accels) {
        // try dialog accels
        if (ap = (VAccelerator *)dlgWin->accels->find(&a)) {
            ap->call();
        }
        // next try global accels
        else if (accelerators
        && (ap = (VAccelerator *)accelerators->find(&a))) {
            ap->call();
        }
    }
    else if (accelerators) {
        // try global accels
        if (ap = (VAccelerator *)accelerators->find(&a)) {
            ap->call();
        }
    }
     
    return ap ? TRUE : FALSE;
}
 
//----------------------------------------------------------------------------
// Reset the mouse cursor to its previous style and release the
// mouse capture.
//
void VNotifier::endWait()
{
    restoreCursor();
    ReleaseCapture();
    waitState = FALSE;
}

//----------------------------------------------------------------------------
// Return the width and height (in pixels) of the display screen that the
// application is running on in '*w' and '*h', respectively.
//
void VNotifier::getScreenSize(int *w, int *h)
{
    static int screen_width = 0, screen_height = 0;
    if (screen_width == 0) {
        screen_width = GetSystemMetrics(SM_CXSCREEN);
        screen_height = GetSystemMetrics(SM_CYSCREEN);
    }
    if (w != 0) { *w = screen_width; }
    if (h != 0) { *h = screen_height; }
}

//----------------------------------------------------------------------------
// Return TRUE if the key 'key' is down, otherwise return FALSE if it is up.
//
boolean VNotifier::keyStatus(int key)
{
    if (GetKeyState(key & 0xFF) & 0x8000) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

//----------------------------------------------------------------------------
// End modal operating mode.  Events are handled normally. If 'hideWin'
// is TRUE (the default), then hide the current modal window before terminating
// modal operations.
//
void VNotifier::endModal(boolean hideWin)
{
    VWindow *oldModalWin;       // the window that's disappearing
    VWindow *newModalWin;       // the modal win that was modal previously

    if ((0 == modals) || (0 == modals->count())) {
        return;
    }

    // save the modal window object
    oldModalWin = (VWindow *)modals->pop();

    newModalWin = (VWindow *)modals->top();

    // if there's a modal window in the modal stack, re-enable it
    if (newModalWin) {
        newModalWin->show();
        newModalWin->enable();
    }

    // else no modal windows in modal stack, enable everything (assuming 
    // multiple VAppViews and their kids) and clean up modal stack
    else {
        setTopLevelWindowEnablement(0, TRUE);

        delete modals;  // jmd (was free)
        modals = 0;
    }

    // don't hide this window until its parent has been enabled, else
    // MS Windows will raise the first enabled window it can find.
    if (hideWin) {
        oldModalWin->hide();
    }

}

//----------------------------------------------------------------------------
// Make the VWindow 'win' modal.  Direct all keyboard and mouse events for
// the application to 'win' until the endModal() method is called.
//
void VNotifier::startModal(VWindow *win)
{
    if (modals == 0) {
        modals = new VStack;
    }

    modals->push(win);

	// after pushed onto stack, test for native handle
	// if hwnd = 0, then it is a native common dialog
	// and will be processed natively
    if ((win == 0) || (win->hwnd() == 0)) {
		return;
	}

#if 0
    if (win->isHidden()) {
        win->show();
#ifndef CV_NOVECTOREVENTS
    	if (!win->vectorEvent(VWindow::GivenFocus))
#endif
        win->givenFocus();
    }
    else {
		if (win->getParent()) {
        	setFocus(win->getParent());
		}
        setFocus(win);
    }
#else
    if (win->isHidden()) {
        win->show();
        win->takeFocus();
	}
#endif
    
    // disable all windows but the modal window
    setTopLevelWindowEnablement(win, FALSE);

    VSet *lastAccels = currAccels;

    // use the native message loop for native dialogs
    if (win->isA(VDialogCls) && ((VDialog *)win)->isNativeDialog()) {
        return;
    }

    cv_assert(win->isA(VViewCls));

    currAccels = ((VView *)modals->top())->accels;

    messageLoop(LoopModal);

    currAccels = lastAccels;
}

//----------------------------------------------------------------------------
boolean     VNotifier::isModalWindowActive()
{                           
    return (modals != 0) && (modals->count() != 0);
}

//----------------------------------------------------------------------------
boolean VNotifier::isModalWindow(VWindow *window)
{
    if ((modals == 0) || (modals->count() == 0) || (window == 0)) {
        return FALSE;
    }

    for (VWindow *w = window ; w != 0 ; w = w->getParent()) {
        if (w == modals->top()) {
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
//
#pragma warning (disable : 4759)
BOOL CALLBACK VNotifier::enableTopLevelWindows(HWND hwndTopLevel, LPARAM lModal)
{
	if (notifier->modals != 0) {
		VWindow *top = (VWindow *)notifier->modals->top();
		if (top != 0) {
			HWND hwnd = top->hwnd();
			if (hwnd != hwndTopLevel) {
				return TRUE;
			}
		}
	}

    // if the window isn't the modal window and is visible, enable it
    if ((hwndTopLevel != (HWND) lModal) && IsWindowVisible(hwndTopLevel)) {

        VWindow *w = notifier->getThis(hwndTopLevel);
        boolean enableTheWindow = TRUE;

        // check if we have info on window, before it was disabled
        if (0 != notifier->modalInfo) {

            // don't enable the window if it was disabled before it the modal
            // was raised.
            //
            // NOTE: there is no way to tell if a boolean FALSE, meaning disabled,
            // or a '(VObject *)', meaning not found, is return. This does not 
            // matter as, in either case, we don't want to enable the window.
            enableTheWindow = (boolean) 
                notifier->modalInfo->removeKey((VObject *) hwndTopLevel);

            if (notifier->modalInfo->count() == 0) {
                delete notifier->modalInfo;
                notifier->modalInfo = 0;
            }
        }

        if (enableTheWindow) {
            if (w) {
                // enable through Views so object knows of the event
                w->enable();
            }
            else {
                EnableWindow(hwndTopLevel, TRUE);
            }
        }
    }

    return TRUE;
}
#pragma warning (default : 4759)

//----------------------------------------------------------------------------
//
BOOL CALLBACK VNotifier::disableTopLevelWindows(HWND hwndTopLevel, LPARAM lModal)
{
    // if the window isn't the modal window and is visible, enable it
    if ((hwndTopLevel != (HWND) lModal) && IsWindowVisible(hwndTopLevel)) {
        
        if (0 == notifier->modalInfo) {
            notifier->modalInfo = new VDictionary(VIntAssocCls);
        }

        // save the enablement state
        notifier->modalInfo->atKeyPut((VObject *)hwndTopLevel, 
            (VObject *)IsWindowEnabled(hwndTopLevel));

        VWindow *w = notifier->getThis(hwndTopLevel);

        // disable through Views so object knows of the event
        if (w) {
            w->disable();
        }
        else {
            EnableWindow(hwndTopLevel, FALSE);
        }
    }

    return TRUE;
}

//----------------------------------------------------------------------------
//
// This functions enables/disables all top level windows except the
// modal window (passed). If modalWin is zero or modalWin->hwnd() is zero,
// the enablement applies to all windows.
//
void VNotifier::setTopLevelWindowEnablement(VWindow *modalWin, boolean enablement)
{
    FARPROC lpfnWndProc;

    // disable all the windows except the modal one
    if (FALSE == enablement) {
        lpfnWndProc = 
            MakeProcInstance((FARPROC)disableTopLevelWindows, inst());
    }
    // enable all the windows except the modal one
    else {
        lpfnWndProc = 
            MakeProcInstance((FARPROC)enableTopLevelWindows, inst());
    }

#if defined(CV_WIN32)
    EnumThreadWindows(GetCurrentThreadId(), (WNDENUMPROC) lpfnWndProc, 
        modalWin ? long(modalWin->hwnd()) : 0);
#else
    EnumTaskWindows(GetCurrentTask(), (WNDENUMPROC) lpfnWndProc, 
        MAKELPARAM(modalWin ? modalWin->hwnd() : 0, 0));
#endif

    FreeProcInstance(lpfnWndProc);

}

//----------------------------------------------------------------------------
// Return TRUE if the RIGHT mouse button is down or return FALSE if the
// button is up.  NOTE: Not all mice have right buttons, hence, supporting this
// event may not be portable to other computer platforms.
//
boolean VNotifier::rbuttonStatus()
{
    return (GetKeyState(VK_RBUTTON) < 0) ? TRUE : FALSE;
}

//----------------------------------------------------------------------------
// Set the keyboard focus to VWindow 'win'.
//
boolean VNotifier::setFocus(VWindow *win)
{
	if (win == focus) {
		return TRUE;
	}
	else if (win == 0) {
		SetFocus(0);
		return TRUE;
	}
	else if (!win->beingDeleted && !win->beingDestroyed) {
		HWND hwnd = win->hwnd();
		if ((hwnd != 0) && IsWindowEnabled(hwnd) && IsWindowVisible(hwnd)) {
			SetFocus(hwnd);
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
// Allow mouse events to be sent to other windows.
//
void VNotifier::releaseCapture()
{
    captureWindow = 0;
    ReleaseCapture();
}

//----------------------------------------------------------------------------
// Restore the mouse cursor style to the style used before the
// last change.
//
void VNotifier::restoreCursor()
{
    if (winCursor) {
        delete winCursor;
        winCursor = 0;
    }
}

//----------------------------------------------------------------------------
// Unregister a callback method for the virtual key code 'key'.
//
void VNotifier::unsetAccel(int key)
{
    VObject *obj;
    VAccelerator a(key);
    if (!accelerators || key == K_NULL) {
        return;
    }
    if (obj = accelerators->remove(&a)) {
        delete obj;     // jmd (was free)
    }
}

//----------------------------------------------------------------------------
// Register a callback method 'sel' for the virtual key code 'key'
// for the client object 'client'.
//
void VNotifier::setAccel(int key, method sel, VObject *client)
{
    if (key == K_NULL) {
        return;
    }
    if (!accelerators) {
        currAccels = accelerators = new VSet;
    }
    VObject *newAccel = (VObject *)new VAccelerator(key, client, sel);
    VObject *oldAccel;

    if ( oldAccel = accelerators->find( newAccel ) ) {
        accelerators->remove(oldAccel);
        delete oldAccel;        // jmd (was free)
    }
    accelerators->add( newAccel );
}

//----------------------------------------------------------------------------
// Set the mouse cursor to type 'type', where 'type' is one of the
// following enum 'CursorShape's:  Arrow, Cross,  FourArrow, Ibeam,
// UpArrow, or HourGlass.
//
void VNotifier::setCursor(enum CursorShape type)
{
    if (winCursor) {
        delete winCursor;
    }
    winCursor = new VMouseCursor(type);
    SetCursor(winCursor->getHndl());
}

//----------------------------------------------------------------------------
// Yield to VNotifier processing during disk and/or calculation
// intensive operations.
//
void VNotifier::yield()
{
    messageLoop(LoopYield);
}

//----------------------------------------------------------------------------
void VNotifier::yieldPaint(VWindow *w)
{
    HWND    hwnd = (w == 0) ? 0 : w->hWnd;
    MSG     msg;

    while (PeekMessage
           (&msg, hwnd, WM_PAINT, WM_ERASEBKGND, PM_REMOVE | PM_NOYIELD)) {
        if ((msg.message != WM_PAINT) && (msg.message != WM_ERASEBKGND)) {
            continue;
        }
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

//----------------------------------------------------------------------------
boolean VNotifier::pendingPaint(VWindow *w)
{
    HWND    hwnd = (w == 0) ? 0 : w->hWnd;
    MSG     msg;

    if (PeekMessage(&msg, hwnd, WM_PAINT, WM_PAINT, PM_NOREMOVE | PM_NOYIELD)) {
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------
// Enable or disable mouse tracking if 'b' is TRUE or FALSE,
// respectively.  If tracking is enabled, then 'mouseMv()' messages will be
// periodically sent to the VWindow currently containing the mouse.
//
void VNotifier::mouseTracking(boolean b)
{
    mouseTrk = b;
}

//----------------------------------------------------------------------------
int errz(char *txt)
{
    ReleaseCapture();
    return MessageBox((HWND)0, txt, "ERROR!", MB_OK);
}

/*
//----------------------------------------------------------------------------
int cppv_errz(char *txt)
{
    ReleaseCapture();
    if (MessageBox((HWND)0, txt, "DEBUG", MB_YESNO)==IDNO) {
        exit(0);
    }
    return IDYES;
}
*/

//----------------------------------------------------------------------------
#if defined(DLL)
CV_DLL_DECL int lstrncmp(char *c1, char *c2, int cnt)
#else
int lstrncmp(char *c1, char *c2, int cnt)
#endif
{
    while (cnt--) {
        if (*c1++ != *c2++) {
            return 1;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------
int lstrncmpi(LPSTR c1, LPSTR c2, int cnt)
{
    while (cnt--) {
        if ((*c1 == *c2) || (*c1 +0x20 == *c2) || (*c1 == *c2 +0x20)) {
            {c1++; c2++; }
        }
        else {
            return 1;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------
//
UINT VNotifier::startTimer(VTimer *timer, UINT timeout)
{
    UINT systemTimerId;

    if (!timers) {
        timers = new VDictionary(VIntAssocCls);
    }

    systemTimerId = SetTimer(0, 0, timeout, notifier->lpfnTimerProc);

    if (systemTimerId) {
        timers->atKeyPut((VObject *)systemTimerId, (VObject *)timer);
    }

    return systemTimerId;
}

//----------------------------------------------------------------------------
//
void VNotifier::stopTimer(UINT systemTimerId)
{
    KillTimer(0, systemTimerId);

    cv_assert(0 != timers);
    timers->removeKey((VObject *)systemTimerId);

    if (0 == timers->count()) {
        delete timers;
        timers = 0;
    }
}

//----------------------------------------------------------------------------
//
void VNotifier::timerExpired(UINT systemTimerId)
{
    VTimer *timer;

    if (timers && 
         (timer = (VTimer *) timers->getValueAtKey((VObject *)systemTimerId))) {
        timer->timerExpired();
    }
}

//----------------------------------------------------------------------------
// Create a MS Windows or OS/2 PM presentation dialog from resource 's'
// with parent 'win'.
//
boolean VNotifier::createDialog(VWindow *win, const char *s)
{
    HWND hPWnd = 0;

    if (!winset) {
        winset = new VSet(200);
    }
    if (win->parent) {
        hPWnd = win->parent->hwnd();
    }

    //  create Windows dialog whose window procedure is 
    //  CTalkDlgProc (see winstart.cpp)

    hDlgRsc = CreateDialog(hInst, (LPCSTR)s, hPWnd, lpfnDialogProc);

    if (!(win->hWnd = hDlgRsc)) {
        return FALSE;
    }
    setThis(win);
    if (win->parent) {
        win->parent->addChild(win);
    }
    return TRUE;
}

//----------------------------------------------------------------------------
// Use the MS Windows or OS/2 PM accelarator keys defined in the
// resource 's'
//
boolean VNotifier::useAccelResource(char *s)
{
    if (!(hAccel = LoadAccelerators(hInst, s))) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}

//----------------------------------------------------------------------------
// Lookup an instance of the VNotifier class and return its reference.
// If not being used with a DLL, then return 'this'.
//
VNotifier *VNotifier::operator->()
{
#ifdef DLL
    VNotifier *n;
    if (!(n = (VNotifier *)lookup(GLOBALnotifier))) {
        // something is WRONG because the VGlobal table
        // has no entry.  We could simply
        // new a notifier(1) here and return its this.
        // That's an idea worth persueing at least in the
        // Win32  case.
        return this;
    }
    return n;
#else
    return this;
#endif
}

//----------------------------------------------------------------------------
void VNotifier::mainwin(HINSTANCE hInstance, 
                        HINSTANCE hPrevInstance, 
                        char     *name)
{
    hInst           = hInstance;
    hPrevInst       = hPrevInstance;
    pCTwindow       = name;
    dlgClassName.puts( "." );
    dlgClassName.concat( pCTwindow );
}

//----------------------------------------------------------------------------
// Return the application name for this C++/Views application.
// (This is from the CTWindow defined in 'appname.cpp'.)
//
char *VNotifier::CTwindow()
{
    return pCTwindow;
}

//----------------------------------------------------------------------------
// Return the MS Windows registered class name for C++/Views
// VDialog objects.
//
char *VNotifier::DLGwindow()
{
    return (char *) dlgClassName.gets();
}

//----------------------------------------------------------------------------
unsigned mesgToType(enum MessageType type)
{
    switch (type) {
    case MesgInformation:
        return MB_ICONINFORMATION;
    case MesgQuestion:
        return MB_ICONQUESTION;
    case MesgAction:
        return MB_ICONSTOP;
    case MesgWarning:
        return MB_ICONEXCLAMATION;
    }
    return 0;
}

//----------------------------------------------------------------------------
// If 'b' is TRUE, then show the cursor, otherwise hide the cursor.
//
void VNotifier::showCursor(boolean b)
{
    ShowCursor(b);
}

//----------------------------------------------------------------------------
// Set the cursor position to ('x','y').
//
void VNotifier::setCursorPos(int x, int y)
{
    SetCursorPos(x, y);
}

//----------------------------------------------------------------------------
// Handler for special MDI child MS Windows messages
//
long VNotifier::mdimessage( HWND hWnd, 
                            unsigned msg, 
                            WPARAM wParam,  
                            LPARAM lParam, 
                            VWindow *&wnId)
{
    long wasDone = FALSE;
    msgReturn = 0;

    switch (msg) {

    case WM_MDIACTIVATE:
        if (wnId = getThis(hWnd)) {

        // Because this a MDI message we assume we are at least a VView class,
        // so we can cast with impunity.
#if defined(CV_WIN32)
            if (hWnd == (HWND) lParam) {
                ((VView *) wnId)->activate((VView *) 
                                            getThis((HWND) wParam));
            }
            else {
                ((VView *) wnId)->deactivate((VView *) 
                                            getThis((HWND) lParam));
            }
#else
            if (wParam != 0) {
                ((VView *)wnId)->activate((VView *) 
                                            getThis((HWND) HIWORD(lParam)));
            }
            else {
                ((VView *)wnId)->deactivate((VView *)
                                            getThis((HWND) LOWORD(lParam)));
            }
#endif  //  CV_WIN32

            wasDone = TRUE;
        }
        break;

    case WM_QUERYENDSESSION:
    case WM_CLOSE:
        if ((msg == WM_CLOSE) && isModalWindowActive() && !isModalWindow(wnId)) {
            return TRUE;
        }
        if (wnId = getThis(hWnd)) {
			if (!wnId->isBeingDestroyed()) {
#ifndef CV_NOVECTOREVENTS
			    if (wnId->vectorEvent(VWindow::Close)) {
					wasDone = TRUE;
				}
				else {
	            	wasDone = ((VMdiView *)wnId)->close();
				}
#else
            	wasDone = ((VMdiView *)wnId)->close();
#endif
			}
			if (!wasDone) {
				DefMDIChildProc(hWnd, msg, wParam, lParam);
				delete wnId;
				wasDone = TRUE;
			}
        }
        break;

    default:
        break;
    }
    return wasDone;
}

//----------------------------------------------------------------------------
//  Return a pointer to the VWindow object containing the mouse cursor.
//
VWindow *VNotifier::whereIsMouse()
{
    POINT p;
    GetCursorPos(&p);
    HWND h = WindowFromPoint(p);
    if (h != 0) {
        VWindow *window;
        ScreenToClient(h, LPPOINT(&p));
        HWND ch = ChildWindowFromPoint(h, p);
        if (ch != 0) {
            if ((window = getThis(ch)) != 0) {
                return window;
            }
        }
        if ((window = getThis(h)) != 0) {
            return window;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
//
void VNotifier::setWindowRedrawUponResize(boolean b)
{
    windowRedrawUponResize = b;
}

// ---------------------------------------------------------------------------
//
void VNotifier::setWindowAutoErased(boolean b)
{
    windowAutoErased = b;
}

// ---------------------------------------------------------------------------
//
boolean VNotifier::isWindowRedrawUponResize() const
{
    return windowRedrawUponResize;
}

// ---------------------------------------------------------------------------
//
boolean VNotifier::isWindowAutoErased() const
{
    return windowAutoErased;
}

//----------------------------------------------------------------------------
// Disable the automatic redrawing of the entire client are of VWindow
// objects whenever a movement or adjustment to the size changes the height or
// width of the client area.  NOTE:  To be effective, this method MUST be
// called before creating any VWindow (or derived) objects.
// This is obsolete; use setWindowRedrawUponResize().
//
void VNotifier::disableAutoRedraw()
{
}

//----------------------------------------------------------------------------
//  Searches the set of windows for the window with the given name and, if
//  specified, the given VWindow class.  Returns the first match,
//  if successful; 0, if not.
//
VWindow *VNotifier::findWindow(const VString& name, VClass *cls)
{
    if ((winset == 0) || name.size() == 0) {
        //  no set of windows, no name specified,
        //  null name string, or empty name string
        return 0;
    }
    //  check for the window in our window set
    DO (winset, VWindow, win)
        if (name.isEqual(win->getName())    &&
            ((cls == 0) || win->isA(cls)))      {

            return win;
        }

    END

    //  then have top-level windows check their children

    VWindow *hit = 0;
    DO (winset, VWindow, win)
        if ((win->parent == 0) && ((hit = win->findWindow(name, cls)) != 0)) {
            break;
        }
    END

    return hit;
}

// ---------------------------------------------------------------------------
boolean VNotifier::closeWindow(VWindow *win)
{
    if (win->hwnd()) {
        SendMessage(win->hwnd(), WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
        return FALSE;
    }
    else {
        delete win;
        return TRUE;
    }
}

//----------------------------------------------------------------------------
//
// This function returns TRUE if the app didn't close, FALSE if it did
//
boolean VNotifier::quitApplication(int appResult)
{
    this->appResult = appResult;

    DO(appViews.created, VAppView, appView)
        // poll all the open appviews. IF any of them return TRUE, abort
        // the close app.
#ifndef CV_NOVECTOREVENTS
	    if (appView->vectorEvent(VWindow::Quit)) {
	       	return TRUE;
		}
#endif
        if (appView->quit()) {
            return TRUE;
        }
    END

    // MUST USE REVERSEDO! 'delete' causes the object in the collection to
    // be removed causing the data in the colection to move up a notch for 
    // every delete. If DO is used then, as we increment forward, the object 
    // data is moving backwards, causing every other object to be deleted, 
    // instead.
    REVERSEDO(appViews.created, VAppView, appView)
        deleteAppView(appView);
    END

	// clear out modal stack if necessary
    if (modals != 0 && modals->count() != 0) {
        setTopLevelWindowEnablement(0, TRUE);

        delete modals;  // jmd (was free)
		modals = 0;
	}

    return FALSE;
}

// ----------------------------------------------------------------------------
//
void VNotifier::registerPaletteWindow(VWindow *window)
{
	if (window != 0) {
		if (paletteWindows == 0) {
			paletteWindows = new VOrdCollect;
		}
		if (!paletteWindows->findSame(window)) {
			paletteWindows->add(window);
		}
	}
}

// ----------------------------------------------------------------------------
//
void VNotifier::unregisterPaletteWindow(VWindow *window)
{
	if (window != 0) {
		if (paletteWindows != 0) {
			paletteWindows->removeSame(window);
		}
	}
}

// ----------------------------------------------------------------------------
//
int VNotifier::realizePalette(VWindow *window, boolean foreground)
{
	if (window == 0) {
		return 0;
	}

	HPALETTE hpalette = window->getPalette();
	if (hpalette == 0) {
		return 0;
	}

	HWND hwnd = window->hwnd();
	if (hwnd == 0) {
		return 0;
	}

	HDC hdc = window->getHndl();
	if (hdc == 0) {
		return 0;
	}

	HPALETTE old_hpalette = SelectPalette(hdc, hpalette, !foreground);
	int nmapped = RealizePalette(hdc);
	SelectPalette(hdc, old_hpalette, TRUE);
	RealizePalette(hdc);
	window->ungetHndl();

	if (nmapped > 0) {
		InvalidateRect(hwnd, 0, TRUE);
	}
	else if (foreground) {
		SendMessage(hwnd, WM_PALETTECHANGED, WPARAM(hwnd), LPARAM(NULL));
	}

	return nmapped;
}

//----------------------------------------------------------------------------
//
void VNotifier::setPainterMode(boolean b)
{
    isPainterMode = b;
}
boolean VNotifier::getPainterMode()
{
    return (boolean) isPainterMode;
}

// ===========================================================================
//
// Trap F1 key for help support from the menu bar and when menus are posted.
//
LRESULT CALLBACK VNotifier::MsgFilterHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    LPMSG lpmsg = (LPMSG)lParam;

#if 0
    // This is leftover from before VHelp class addition

    // if there's a key stroke in a menu, look for the help keystrokes
    if ((nCode == MSGF_MENU) && 
        (lpmsg->message == WM_KEYDOWN && lpmsg->wParam == VK_F1)) {

        // if Shift+F1, turn on help mode
        if (GetKeyState(VK_SHIFT) & 0x8000) {
            if (!notifier->isHelpModeActive() && notifier->isHelpAvailable()) {
                notifier->setHelpMode(TRUE);
                notifier->setHelpCursor();
            }
        }

        // else show help
        else {
            notifier->showHelp(notifier->currHelpId);
        }
    }
#endif

    return CallNextHookEx(notifier->hMsgFilterHook, nCode, wParam, lParam);
}

//----------------------------------------------------------------------------
//
// Remove the association between "this" with an MS Window handle.
//
static void unsetDialogThis(HWND hWnd)
{
    #if defined(CV_WIN32)
        RemoveProp( hWnd, (LPCSTR) notifier->aViewsThis);
    #else
        RemoveProp( hWnd, (LPCSTR) notifier->aViewsThisLow);
        RemoveProp( hWnd, (LPCSTR) notifier->aViewsThisHigh);
    #endif
}

//----------------------------------------------------------------------------
//
// Associate "this" with an MS Window handle so that "this" can be referenced
// from the hwnd.
//
static void setDialogThis(HWND hWnd, VDialog *dlg)
{
    #if defined(CV_WIN32)
        SetProp( hWnd, (LPCSTR) notifier->aViewsThis, (HANDLE) dlg);
    #else
        SetProp( hWnd, (LPCSTR) notifier->aViewsThisLow, (HANDLE) LOWORD(dlg));
        SetProp( hWnd, (LPCSTR) notifier->aViewsThisHigh, (HANDLE) HIWORD(dlg));
    #endif
}

//----------------------------------------------------------------------------
//
// Returns  V_CTL3D_EFFECTS if CTL3D effects available,
//          V_WIN3D_EFFECTS if Windows 95 3d effects available,
// else     V_NO_3D_EFFECTS is returned.
//
int VNotifier::threeDEffectsAvailable(void)
{
    if (ctl3dIsActive) {
        cv_assert(VWinVersionMajor <= 4);
        return V_CTL3D_EFFECTS;
    }
    else if (VWinVersionMajor >= 4) {
        cv_assert(!ctl3dIsActive);
        return V_WIN3D_EFFECTS;
    }
    else {
        return V_NO_3D_EFFECTS;
    }
} // threeDEffectsAvailable

//----------------------------------------------------------------------------
//
boolean VNotifier::threeDEffectsAvailable(enum V_3D_EFFECTS effect)
{
    switch (effect) {
    case V_CTL3D_EFFECTS:   return ctl3dIsActive;
    case V_WIN3D_EFFECTS:   return VWinVersionMajor >= 4;
    default:                return FALSE;
    }

}  // threeDEffectsAvailable

//----------------------------------------------------------------------------
//
// This window proc
//      - implements Ctl3d effects for Views and non-views dialogs.
//      - manages the "this" pointer as a window property.
//
LRESULT CALLBACK VNotifier::CommDlgProc(    HWND    hWnd, 
                                        UINT    msg,
                                        WPARAM  wParam, 
                                        LPARAM  lParam)
{
    WNDPROC lpfnWndProc = getDialogWndProc(hWnd);

    switch (msg) {

    case WM_INITDIALOG: {
        VDialog *dlg = (VDialog *) getDialogThis(hWnd);
        if (dlg){
            // Creates a winset for common dialogs
            if (!notifier->winset) {
                notifier->winset = new VSet(200);
            }
            notifier->winset->add(dlg);
            // disable all windows but the modal window
            notifier->setTopLevelWindowEnablement((VWindow *) dlg, FALSE);
        }

#if defined(CV_CTL3D)
        //
        // if this isn't a Views dialog or the VDialog is 3d effected,
        // turn on Ctl3D.
        //
        // NOTE: the "dlg == 0 ||" also protects us from dereferencing zero.
        // NOTE: the "dlg == 0 ||" has the same affect as Ctl3dAutoSubclass().
        //
        if (notifier->threeDEffectsAvailable(V_CTL3D_EFFECTS) && 
            (dlg == 0 || dlg->has3dEffects())) {
            Ctl3dSubclassDlgEx(hWnd, CTL3D_ALL);
        }
#endif
    }
    break;

    case WM_NCDESTROY: {
        VDialog *dlg = (VDialog *) getDialogThis(hWnd);


        // if this is a Views dlg
        if (dlg) {
            // added evc
            // disable all windows but the modal window
            notifier->setTopLevelWindowEnablement((VWindow *) dlg, TRUE);
            notifier->winset->removeSame(dlg);
            unsetDialogThis( hWnd);
        }

        #if defined(CV_WIN32)
            RemoveProp( hWnd, (LPCSTR) notifier->aViewsWndProc);
        #else
            RemoveProp( hWnd, (LPCSTR) notifier->aViewsWndProcLow);
            RemoveProp( hWnd, (LPCSTR) notifier->aViewsWndProcHigh);
        #endif
        
        // Since this is a native dialog, it has been destroyed when the OK
        // or Cancel button was hit, so tell Views about it.
        if (dlg) {
            dlg->hWnd = 0; 
        }
    }
    break;

    } // switch

    cv_assert(0 != lpfnWndProc);
    // cv_assert(!IsBadReadPtr(lpfnWndProc, sizeof(FARPROC)));
    return CallWindowProc(lpfnWndProc, hWnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------
//
// Sets the the new WndProc into the window of the Dialog and saves the old 
// one as a property on the window.
//
// Notes:
// 1) Currently, this function is used on MS Window Dialog and Views
//      VDialog classes. Properites take up room in the local heap of the USER 
//      segment. Its a good idea NOT to use properties to store the old WndProc
//      for EVERY child window in EVERY dialog, created.
//
static void subclassDialog(HWND hWnd)
{
    FARPROC lpfnWndProc = (FARPROC)
        SetWindowLong(hWnd, 
                          GWL_WNDPROC, 
                          (LONG) notifier->lpfnCommDlgProc);

    #if defined(CV_WIN32)
        SetProp( hWnd, (LPCSTR) notifier->aViewsWndProc, (HANDLE) lpfnWndProc);
    #else
        SetProp( hWnd, (LPCSTR) notifier->aViewsWndProcLow, (HANDLE) LOWORD(lpfnWndProc));
        SetProp( hWnd, (LPCSTR) notifier->aViewsWndProcHigh, (HANDLE) HIWORD(lpfnWndProc));
    #endif
}

//----------------------------------------------------------------------------
//
// Trap Native Modal Dialog (WC_DIALOG class) creation to set the HWND 
// into the VWindow (if its a presentation window of a Views object).
//
// Notes:
// 1) Currently, the way we tell if the MS window creation is for a Views 
//      Object is that we only look at windows created during Views Modality.
//      The prerequisite is to call VNotifier::startModal() before creating
//      the modal dialog.
//
LRESULT CALLBACK VNotifier::CBTHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    static HWND hWnd = 0;

    if (nCode == HCBT_CREATEWND) {
        LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;

        // if a modal dialog being created, set flag so next window created 
        // (presumably on this dialog) will trigger code affecting previous 
        // window.
        //
        // NOTE: Its dangerous to do anything to this window, at this time, 
        // because the window hasn't received a WM_CREATE yet. Instead,
        //  we save the hWnd and wait for the next window to be created, when we do
        // work on the saved window handle.
        //
        // NOTE: In Views, only modal WC_DIALOGs are VDialogs. We only known
        // when to associate the object and the hWnd during modality. Otherwise,
        // we couldn't tell the difference between View dialogs and users who 
        // create their own common dialogs, outside of views.
        //
        if (lpcs->lpszClass == WC_DIALOG) {
                hWnd = (HWND) wParam;
        }

        // if dialog window was created last pass throught this hook proc and
        // we have never sub-classed this window before (probably can
        // never happen but its a good sanity check)     
        // caste of hWnd 
        else if ((((UINT) hWnd) != 0) && !isDialogSubclassed(hWnd)) {

            subclassDialog(hWnd);

            // if its a Views modal dialog, set the hwnd into member data
            // and set "this" as a property on the hwnd.
            //
            // NOTE: testing the hwnd() of the modal window allows programmers
            // to raise native modal dialogs on top of views modal dialogs. The
            // window handle test essentially acts as a semaphore to prevent the
            // resetting of the hwnd of the Views modal dialog.

            if (notifier->isModalWindowActive() &&
                 ((VDialog *)notifier->modals->top())->hwnd() == 0
                ) {
                VDialog *modalWin = (VDialog *)notifier->modals->top();

                // >>> Start Critical Section!!! <<<
                // Don't create any windows in the critical section!

                modalWin->hWnd = hWnd;

                setDialogThis( hWnd, modalWin);

                //
                // >>> End Critical Section!!! <<<
                //

                // don't cv_assert() until after the above critical section or 
                // else infinite recursion can occur, due to the fact we are 
                // creating a window in cv_assert() in the window creation hook 
                // proc.

                cv_assert(modalWin->isA(VDialogCls));

            } // Views Dialog

            hWnd = 0;

        } // else if (hWnd != 0) and dialog sub classed

    } // is window being created?

    return CallNextHookEx(notifier->hCBTHook, nCode, wParam, lParam);
}

//----------------------------------------------------------------------------
//
void VNotifier::setExitCode( int exitCode)
{
    appResult = exitCode;
}

// ===========================================================================
//
// returns the number of appViews with presentation spaces. Because
// appViews are not deleted, but destroyed, in VNotifier::start(), 
// there could be appView objects with no presentation spaces.
integer VNotifier::numAppViews()
{
    return appViews.created.count();
}

// ===========================================================================
#ifndef CV_NOARCHIVER

//----------------------------------------------------------------------------
//
VString *VNotifier::getClientName(const VObject* client, integer index)
{
    if (!getPainterMode() || ghostClientList == 0) {
        if (client == 0 || !(((VObject *)client)->isA(VWindowCls))) {
            return 0;
        }
        VWindow *win = (VWindow *)client;
        return win->getName();
    }

    if (index != -1L) {
        VObject *obj = ghostClientList->idAt(index);
        if (obj != 0 && obj->isA(VStringCls)) {
            return (VString *)obj;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------
//  Returns the named client; 0, if not found.
//
VObject *VNotifier::getClient(const VString& name, integer *index)
{
    if (!getPainterMode() || ghostClientList == 0) {
        if (index != 0) {
            *index = -1L;
        }
        return findWindow(name);
    }

    // ignore empty strings
    if (name == (const char *)"") {
        *index = -1L;
        return 0;
    }

    VString client(name);
    VObject *obj = ghostClientList->find(&client);

    if (obj == 0) {
        // name not in ghost list
        if (index != 0) {
            VString *pClient = new VString(name);

            ghostClientList->add(pClient);
            *index = ghostClientList->indexOfSame(pClient);
        }
    }
    else if (index != 0) {
        // name is in ghost list
        *index = ghostClientList->indexOf(&client);
    }

    return obj;
}

//---------------------------------------------------------------------
#if defined (CV_WIN32)
// used below in ::bringAppToTop
static BOOL CALLBACK handleWindowEnumeration(HWND hwnd, LPARAM lParam) {
    HWND *pHwnd = (HWND *) lParam;
    if (GetWindowThreadProcessId(hwnd,NULL)==GetCurrentThreadId() && 
        IsWindowVisible(hwnd)) {
        *pHwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}
void VNotifier::bringAppToTop(){
// Get the window the OS marks as active within the thread that is 
// making the call and force it to the top.

// Useful for multithreaded applications where none of the widows 
// within one of the threads of the application 
// is the top-level window, and program want to 
// force that window to the top.
    HWND hwnd = NULL;
    EnumWindows(&handleWindowEnumeration,(LPARAM) &hwnd); 
    if (hwnd != NULL) {
        SetForegroundWindow(hwnd);
    }
}

#endif

#endif // ARCHIVER
