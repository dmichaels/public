// $Header:   Q:/views/win/vcs/notifier.h_v   1.34   Mar 04 1997 16:32:44   gregm  $ 

//  notifier.h
//
//	VNotifier interface
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  07/21/92 jmd    preened
//  09/09/92 jmd    made the notifier object CV_NEARDATA
//  09/09/92 jmd    made CV_WIN16 auto define CV_WINDOWS
//
//  04/09/93 pat    changed id to VObject *
//  04/28/93 pat    added findWindow()
//  05/20/93 jmd    added inStart
//  06/28/93 dgm    added yieldPaint(), pendingPaint,
//                  postPaint(), and paint().
//  07/09/93 jmd    added closeWindow
//  07/20/93 jmd    preened (deja vu)
//  07/23/93 pat    changed findWindow() name arg to const VString&
//  07/23/93 pat    added palette support
//  07/27/93 pat    added getClientName(), getClient(),
//                  getGhostClientList(), and setGhostClientList().
//  07/31/93 jmd    modified menu prompt logic
//  08/01/93 jmd    added mouseMoveIgnored
//  08/02/93 jmd    added menu select callback
//  08/12/93 dgm    added dialogEditorCreateMode().
//  08/23/93 pat    added realizePalette(), get/setMaxUpdateColors().
//  08/31/93 dgm    added dialogEditorRecreateMode().
//  09/02/93 pat    added realizePalette() return value; made overloaded
//                  realizePalette() not default on RealizePaletteReason
//  09/07/93 dgm    made VNotifier::modal public (for winstart to get at).
//                  added VNotifier::isModalWindow() to help fix
//                  for non-modal behavior of modal-dialogs.
//  10/10/93 jmd    SC6 changes.
//  12/27/93 jmd    increased dlgClassName size to 48
//
//  01/04/94 jmd    added help routines
//  01/14/94 jmd    added msgIgnored, made some booleans into bit fields
//  02/26/94 jmd    added initHelp
//  03/08/94 pat    STRICT:
//                  HANDLE replaced by HINSTANCE in hInst, inst();
//                  HBRUSH for bkBrush; HACCEL for hAccel;
//                  HCURSOR for harrowCurs and hpHelpCursor;
//                  HWND for hApplVw;
//                  ifdef'd FARPROC, WNDPROC;
//                  updated mainwin() signature;
//                  curshWnd is now HWND not HANDLE;
//  03/09/94 pat    preened.
//  03/24/94 pat    CV_WIN32:
//                  CV_NEARDATA is whitespace for CV_WIN32.
//                  auto defined CV_WINDOWS;
//                  removed wndproc typedef
//  03/26/94 pat    CV_WIN32: 'last' member is WPARAM, not WORD;
//                  cast 'last' to int in exitCode().
//  06/28/94 jmd    added getExposedRectangle members
//  07/13/94 jld    added dependency on defs.h and removed defines
//  08/15/94 dgm    Removed old postpaint support.
//  09/05/94 dss    Ctl3dCtlWndProc.
//  10/11/94 dss    Added help functions, window independent timer support,
//                  closeApp(), CBTHook, common dialog support, modality
//                  support.
//                  Replaced old help filter with generic MsgFilterHook.
//  11/03/94 dss    multi appview support. Changed closeApp() to
//                  quitApplication().
//  12/19/94 evc    Moved file static ATOM variables into notifier class data.
//                  This fixes problem with multiple instances of Views
//                  application that use common dialog boxes and Views DLLs.
//
//  01/31/95 evc    dlgClassName changed to VString for MBCS safty
//  06/08/95 jld    added dontKey, accelKeysOff/On() for suspending accelerator keys
//
//	06/19/95 evc	added declarations to help manage dynamic notifier
//                  creation in DLL clients.  This is special cased with
//                  CV_NOSTATICNOTIFIER. Used for creating Views objects from
//                  non-views programs.
//  08/03/95 jld    removed help support not needed with new VHelp class
//  09/01/95 dss    Runtime detection of CTL3D. Runtime detection of CTL3D & Windows 95
//                      3D effects.
//  09/29/95 dgm    Moved V_3D_EFFECTS declaration from here to window.h.
//  09/29/95 dgm    Added setWindowRedrawUponResize(),
//                  isWindowRedrawUponResize(), setAutoErased(), and
//                  isAutoErased(); disableAutoRedraw() is obsolete.
//  10/19/95 evc    Added GetVirtualKey as static function
//  10/20/95 evc    wrong return type in getVirtualKey
//  10/23/95 evc    Added bringAppToTop
//	10/27/95 dgm	Turned static data into data members.
//  11/02/95 evc    Moved bringToTop to cpp file
//	12/04/95 pkt	Added 'const' to char* parameter of createDialog.
//  01/25/96 krb    Added set/getPainterMode() and removed dialogEditorCreateMode().
// 02/08/96  evc    added lpfnSaveComboProc & lpfnComboProc 
//	05/21/06 glm	changed tmpWin to *tmpWin

// ----------------------------------------------------------------------------

#ifndef VNOTIFIER_H
#define VNOTIFIER_H

// 3d effects enumerations.
#if 0
enum V_3D_EFFECTS { V_NO_3D_EFFECTS, 
                    V_CTL3D_EFFECTS,    // ctl3d.dll
                    V_WIN3D_EFFECTS};   // native windows 3d
#endif

#include "defs.h"

#include "appview.h"
#include "collectn.h"
#include "containr.h"
#include "dictinry.h"
#include "global.h"
#include "rect.h"
#include "set.h"
#include "str.h"
#include "timer.h"
#include "winclass.h"
#include "window.h"

enum LoopReason { LoopMain, LoopModal, LoopYield};

#define maxKeys 50
#define BN_USERDRAW 11 // user drawn button
#define LN_USERDRAW 20 	  // user drawn list/combo
#define LN_MEASUREITEM 21 // user drawn list/combo

// The VNotifier class is the centralized controller class of
// the C++/Views MVC paradigm.  Only one instance of the VNotifier is
// defined: 'notifier'.  As controller, the VNotifier provides functionality
// in two areas: (1) event trapping and dispatching to window objects through
// well-defined message protocols; and (2) services for windows and views to
// control event sequencing (modal dialogs), cursor styles, and keyboard/mouse
// redirection.  The VNotifier handles events such as mouse events, timer
// events, keyboard focus changes, keyboard inputs, window exposures, window
// size changes, and window movements.
//

CLASS VPopupMenu;
CLASS VMenuItem;
CLASS VMenu;
CLASS VControl;
CLASS VStack;
CLASS VDialog;
CLASS VAppView;
CLASS VOrdCollect;
CLASS VNotifier;

#   ifdef DLL
#       ifdef IN_DLL
            extern VNotifier CV_NEARDATA notifier;
#       endif

#       ifdef USE_DLL

// Certain custom client configurations can 
// refer to the notifier thru a pointer (rather than
// with a notifier object created staticly and defined
// in globals.h).  In those cases pNotifier is defined 
// instead and initialized in winstart via new. 

// This is primarily used for creating Views dialogs 
// from a non-views application, where a DLL is built to
// create notifier objects on the fly for use by a non-views 
// client app(s).

            #ifdef CV_NOSTATICNOTIFIER
                extern VNotifier * pNotifier;
                #define notifier (*pNotifier) 
            #else 
                extern VNotifier CV_NEARDATA notifier;
            #endif

#       endif
#else // !DLL
// regular non-dll case
extern VNotifier CV_NEARDATA notifier;
#endif

extern VClass *VNotifierCls;
extern char *CTWindow;

#define GLOBALnotifier 2

CLASS VNotifier : public VGlobal {

public:

    friend VWindow;
    friend VMenu;
    friend VPopupMenu;
    friend VMenuItem;
    friend VDialog;
    friend VControl;
    friend VAppView;
            
                VNotifier();
#ifdef DLL
                VNotifier(boolean dll);
#endif          
                ~VNotifier();

    VClass          *iam();

    WNDPROC         lpfnWindowProc;
    WNDPROC         lpfnMDIChildProc;
    WNDPROC         lpfnMDIFrameProc;
                    
    WNDPROC         lpfnControlProc;

	// these hold the
	// address of the editline owned by the combobox
    WNDPROC         lpfnSaveComboEditProc;
    WNDPROC         lpfnComboEditProc;

	// These have the address of the combobox proc.
	// (used for getting comboLBox part of a combobox)
    WNDPROC         lpfnSaveComboProc;
    WNDPROC         lpfnComboProc;


    DLGPROC         lpfnDialogProc;
    DLGPROC         lpfnDialogWndProc;
    WNDPROC         lpfnCtl3dCtlWndProc;
    TIMERPROC       lpfnTimerProc;

    WNDPROC         pCTalkWndProc;
    WNDPROC         pCTalkMDIFrameProc;
    WNDPROC         pCTalkMDIChildProc;
    WNDPROC         pCtl3dCtlWndProc;

    HINSTANCE       hPrevInst;
    HINSTANCE       hInst;
    long                msgReturn;
    integer         cnt;
    int             idx;        
    char           *pCTwindow;

    VString         dlgClassName;  // MBCS safe - evc

    boolean             setThis(VWindow *);
    VWindow         *getThis(HWND h);
    boolean             unsetThis(VWindow *);

    boolean             measureItem(long);
    boolean             drawItem(long);
    boolean             scrollItem(HWND, int, WPARAM, LPARAM, VWindow *);
    int             getExitCode();
    void                setExitCode(int exitCode);

    char        *DLGwindow();
    int             getMenuId();

    friend VWindow *getThis(HWND, char);
    void                mainwin(HINSTANCE hi, HINSTANCE hprevi, char *s);

    HWND                client();
    void                popupMenu(VMenu *menu);
    DRAWITEMSTRUCT      *userDraw();
    void				*userEvent();
    HWND                dlg();

    WNDPROC             proc();

    long                cvmessage(  HWND        hWnd, 
                                UINT        msg, 
                                WPARAM      wParam, 
                                LPARAM      lParam, 
                                VWindow *&  win);

    void                setWindowRedrawUponResize(boolean);
    boolean             isWindowRedrawUponResize() const;
    void                setWindowAutoErased(boolean);
    boolean             isWindowAutoErased() const;
    void                disableAutoRedraw(); // obsolete

    boolean             createDialog(VWindow *, const char *);
    boolean             createWin(VWindow *, int, int, int, int);
    boolean             recreateWin(VWindow *);
    void                destroyWin(VWindow *);

    void                appViewDeleted(VAppView *);
    void                appViewConstructed(VAppView *);

    boolean             keyStatus(int);
    boolean             doKey(int);

    VRectangle          *exposedRect(); 

    void                getScreenSize(int *w, int *h);

    VWindow             *getFocus();
    boolean             setFocus(VWindow *);

    void                mouseTracking(boolean tf);
    boolean             buttonStatus();
    boolean             rbuttonStatus();
    void                getCursorPos(int *cx, int *cy);
    void                setCursorPos(int cx, int cy);
    void                showCursor(boolean tf);
    VWindow             *whereIsMouse();

    void                releaseCapture();
    void                captureMouseFor(VWindow *win);

    void                setAccel(int key, method mthd, VObject *client);
    void                unsetAccel(int key);
    void                setAccelKeys(int *keys, method *mthds, VObject *clients);
    boolean             useAccelResource(char *rsc);
    void                accelKeysOff();
    void                accelKeysOn();

    void                setCursor(enum CursorShape);
    void                restoreCursor();

    void                enableWinEvents(boolean b);
    void                start();
    void                yield();
    void                yieldPaint(VWindow *win = 0);
    boolean             pendingPaint(VWindow *win = 0);

    void                startModal(VWindow *win);
    void                endModal(boolean hideWin = TRUE);

    boolean             isModalWindow(VWindow *);
    boolean             isModalWindowActive();

    void                beginWait();
    void                beginWaitFor(VWindow *win);
    void                endWait();

    void                uponMenuSelect(VObject *client, method mthd);

    HINSTANCE       inst();
    HINSTANCE       prevInst();

    boolean         onlyInstance();
    char            *CTwindow();
    VNotifier      *operator->();

    boolean             closeWindow(VWindow *win);
    long                mdimessage(HWND, unsigned, WPARAM, LPARAM, VWindow *&);

    VWindow        *findWindow(const VString& name, VClass *cls = 0);

    boolean         quitApplication(int appResult = 0);

    integer         numAppViews();

	void			systemColorsChanged(VWindow * = 0);
	void			systemFontsChanged(VWindow * = 0);
	void			systemMetricsChanged(VWindow * = 0);

#ifndef CV_NOARCHIVER
    VOrdCollect    *getGhostClientList();
    void                setGhostClientList(VOrdCollect *list);
    VString     *getClientName(const VObject* client, integer index = -1L);
    VObject        *getClient(const VString& name, integer *index = 0);
#endif

    void                setPainterMode(boolean);
    boolean				getPainterMode();

    int				realizePalette(VWindow *, boolean foreground = FALSE);
    void			registerPaletteWindow(VWindow *);
    void			unregisterPaletteWindow(VWindow *);
#if 0
    int             getMaxColorUpdates();
    void                setMaxColorUpdates(int n);
#endif

    boolean         getExposedRectangle(VRectangle& rect);
    boolean         getExposedRectangle(int *left, int *top, int *right, int *bottom);

    static              int getVirtKey(short);

    // Windows Hooks
    static LRESULT CALLBACK 
    MsgFilterHook(int nCode, WPARAM wParam, LPARAM lParam);

    HOOKPROC            lpfnMsgFilterHook;          // filter proc address
    HHOOK               hMsgFilterHook;     // hook handle

    static LRESULT CALLBACK CBTHook(int nCode, WPARAM wParam, LPARAM lParam);
    HOOKPROC            lpfnCBTHook;            // filter proc address
    HHOOK               hCBTHook;               // hook handle

    static LRESULT CALLBACK CommDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    WNDPROC         lpfnCommDlgProc;

    // timer stuff
    UINT                startTimer(VTimer *timer, UINT timeout);
    void                stopTimer(UINT timerId);
    void                timerExpired(UINT systemTimerId);

// used for common dialog procedure subclassing 

#if defined(CV_WIN32)
    ATOM    aViewsWndProc;
#else
    ATOM    aViewsWndProcLow;
    ATOM    aViewsWndProcHigh;
#endif

#if defined(CV_WIN32)
    ATOM aViewsThis;
#else
    ATOM aViewsThisLow;
    ATOM aViewsThisHigh;
#endif

// 3d
    boolean         threeDEffectsAvailable(void);
    int             threeDEffectsAvailable(enum V_3D_EFFECTS);
    boolean         ctl3dIsActive;

// beep support
    enum    BeepType { Note = MB_ICONASTERISK, Warning = MB_ICONEXCLAMATION, Error = MB_ICONHAND };
    void    beep(BeepType);

    void    setTopLevelWindowEnablement(VWindow *modalWin, boolean enablement);


#if defined CV_WIN32
    static void bringAppToTop();
#endif

protected:

    boolean         winEvents;
    VSet           *accelerators;
    VSet           *currAccels;
    boolean         dontKey;
    HWND                curshWnd;
    HCURSOR         harrowCurs;
    VMouseCursor    *winCursor;

    HWND                hClient;

    void                deleteAppView(VAppView *);

    VView           *dlgWin;    // pointer to current resource based dialog.

    HWND                hDlgWnd;    // MS VWindow handle to current resource based dialog.
    HWND                hDlgRsc;
    short               xlat;
    short               key;
    HBRUSH          bkBrush;    // deflt bckgrnd brush for/controls,if needed
    PAINTSTRUCT     ps;
    DRAWITEMSTRUCT *ds;
    void		   *es;
    VRectangle      expose;
    boolean             exposed;

protected:

    VWindow         *cursWndId;
    boolean         mouseTrk;
    VWindow         *focus;
    HACCEL          hAccel;
    short               waitState;
    short               keyCount;
    ushort          controlId;
    ushort          menuId;
    VWindow         *windowOf(HWND);
    VDictionary    *timers;
    short               nTimers;
    VStack          *modals;    
    VDictionary    *modalInfo;  
    VMenu           *popup;
    VWindow         *tmpWin;
    short           tmpWinStyle;
    VSet            *winset;
    ushort          Id;

    // each collection tracks the number of appViews in its particular
    // state
    struct tagAppViews {
        VOrdCollect constructed;    // object construction (VAppView::VAppView)
        VOrdCollect created;            // native creation (CreateWindow)
    } appViews;

    boolean         classesInitialized;

    VObject         *menuSelClient;
    method          menuSelMethod;

    void                messageLoop(enum LoopReason);
    void                init();

private:
    unsigned            msgIgnored :1;
    unsigned            isPainterMode :1;

public:
	VOrdCollect		   *paletteWindows;
	int					paletteChangedJustReceived;

private:
    unsigned            buttonToTrack;

    int             appResult;      // application return code

    boolean         inStart;

	boolean			windowRedrawUponResize;
	boolean			windowAutoErased;

    static BOOL CALLBACK enableTopLevelWindows(HWND hwndTopLevel, LPARAM lModal);
    static BOOL CALLBACK disableTopLevelWindows(HWND hwndTopLevel, LPARAM lModal);

#ifndef CV_NOARCHIVER
    VOrdCollect    *ghostClientList;
#endif
};

#if 0
//----------------------------------------------------------------------------
//
inline int VNotifier::getMaxColorUpdates()
{
    return maxColorUpdates;
}

//----------------------------------------------------------------------------
//
inline void VNotifier::setMaxColorUpdates(int n)
{
    maxColorUpdates = n;
}
#endif

//----------------------------------------------------------------------------
// Return the instance handle for this application (MS Windows and
// OS/2 PM only).
//
inline HINSTANCE VNotifier::inst()
{
    return hInst;
}

//----------------------------------------------------------------------------
// Return the prev instance handle for this application (MS Windows
// only).
//
inline HINSTANCE VNotifier::prevInst()
{
    return hPrevInst;
}

//----------------------------------------------------------------------------
// Return TRUE if this is the only instance of this application running
// This method is defined for MS Windows and OS/2 PM only.
//
inline boolean VNotifier::onlyInstance()
{
    return hPrevInst ? FALSE : TRUE;
}

//----------------------------------------------------------------------------
inline VWindow *VNotifier::getThis(HWND h)
{
	if (!tmpWin){
		tmpWin=new VWindow();
		tmpWin->hWnd = 0;
		tmpWin->parent = 0;
	}

    tmpWin->hWnd = h;
    return (VWindow *)winset->find((VObject *)tmpWin);
}

//----------------------------------------------------------------------------
inline int VNotifier::getExitCode()
{
    return (int) appResult;
}

//----------------------------------------------------------------------------
inline int VNotifier::getMenuId()
{
    return menuId++;
}

//----------------------------------------------------------------------------
inline HWND VNotifier::client()
{
    return hClient;
}

//----------------------------------------------------------------------------
inline void VNotifier::popupMenu(VMenu *menu)
{
    popup = menu;
}

//----------------------------------------------------------------------------
inline DRAWITEMSTRUCT *VNotifier::userDraw()
{
    return ds;
}

//----------------------------------------------------------------------------
inline void *VNotifier::userEvent()
{
    return es;
}

//----------------------------------------------------------------------------
inline WNDPROC VNotifier::proc()
{
    return lpfnControlProc;
}

//----------------------------------------------------------------------------
inline HWND VNotifier::dlg()
{
    return hDlgRsc;
}

//----------------------------------------------------------------------------
// Return a pointer to the VWindow object with the current focus.
//
inline VWindow *VNotifier::getFocus()
{
    return focus;
}

//----------------------------------------------------------------------------
// If 'b' is TRUE, enable the dispatch of GUI system event messages
// directly to VWindow objects via the winDispatch() virtual function.
//
inline void VNotifier::enableWinEvents(boolean b)
{
    winEvents = b;
}

//----------------------------------------------------------------------------
// Establish callback for menu bar highlighting (PRIVATE)
//
inline void VNotifier::uponMenuSelect(VObject *client, method mthd)
{
    if (client == 0 || mthd == NIL_METHOD) {
        menuSelClient = 0;
        menuSelMethod = NIL_METHOD;
    }
    else {
        menuSelClient = client;
        menuSelMethod = mthd;
    }
}

// ---------------------------------------------------------------------------
//
//
inline void VNotifier::accelKeysOn()
{
    dontKey = FALSE;
}

// ---------------------------------------------------------------------------
//
//
inline void VNotifier::accelKeysOff()
{
    dontKey = TRUE;
}

// ---------------------------------------------------------------------------
//
//
inline void VNotifier::beep(BeepType type)
{
    MessageBeep(type);
}

// ===========================================================================
#ifndef CV_NOARCHIVER

//----------------------------------------------------------------------------
//  Returns the list of ghost clients, if present; 0, if not.
//
//  Note that if you delete this collection of client names (VString s), then
//  you must use VOrdCollect::freeContents().  This is because getClient()
//  has the capacity to allocate new VStrings and add them to the 
//  collection.
//
inline VOrdCollect *VNotifier::getGhostClientList()
{
    return ghostClientList;
}

//----------------------------------------------------------------------------
//
inline void VNotifier::setGhostClientList(VOrdCollect *list)
{
    ghostClientList = list;
}


//----------------------------------------------------------------------------

#endif  //  ARCHIVER
// ===========================================================================

#endif // VNOTIFIER_H

