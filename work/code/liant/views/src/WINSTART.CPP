/*
$Header:   Q:/views/win/vcs/winstart.cpv   1.19   07 Nov 1996 11:06:20   davidmi  $ 
*/
//	winstart.cpp
//
//	Windows start procedure
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
//	Revision History:
//	-----------------
//	09/07/93 dgm	fix for non-modal behavior of modal-dialogs.
//	09/19/93 dgm	at long last, a fix for the tab-eating combo-boxes!
//
//	01/04/94 jmd	added help routines (message filter)
//	01/30/94 dgm	simplified ++ expression in WinMain() to
//					avoid apparent Borland C++ 4.0 compiler bug.
//  03/03/94 pat	CV_WIN32: updated CTalkWndProc declaration
//	03/08/94 pat	STRICT: updated instance parameter type in WinMain.
//	08/04/94 dgm	Fixed argv/argc processing.
//	08/02/94 dss	Ctl3d support.
//	09/05/94 dss	Ctl3dCtlWndProc.
//	10/11/94 dss	Added window independent timer support. Made argv[0] point
//			 		to module execution path. Added CBTHook. Removed
//					CTalkHelpFilter() (replaced with VNotifier::MsgFilterHook).
//					CV_CTL3D only: removed auto subclassing. This is done in the
//					CBTHook and gives user ability to override ctl3d on common
//					dialogs. STRICT (FARPROC) conformance.
//
//	01/17/95 jld	replaced lpfnTimerProc binding under Win32 that had been
//					lost and removed STRICT ifdef. Library is always compiled
//					STRICT.
//	07/26/95  evc	Fixed win32 SetWindowsHook calls to be per therad (instead
//					of per process.  Also broke out initialization and breakdown
//					code that surrounds cvmain into separate procedures
//					(initViews and endViews)
//	08/03/95 jld	removed help support not needed with new VHelp class
//	09/01/95 dss	Runtime detection of CTL3D. VStyles support.
//	09/20/95 dss	Added vStyleSelectionSet.
//	10/12/95 jld	added styles for notebook and treelistviewboxthing
//  11/19/95 evc    fixed combobox's editControl's window procedure to
//					handle focus and keydown events correctly
//	12/06/95 dss   setExclusive(): cast away const on arg.
//  02/07/96 evc    Got rid of usage of MakeProcInstance and replaced it
// 					with CV_DLL_EXPORT decoration for window procs and the like.
// 					Simplifies the code, and lets us not have to
// 					delete the thunks in endviews(). Deleting thunks 
//					can cause crashes if we have global window objects 
//					destroyed afterward.  Adding CV_DLL_EXPORT (_export) 
// 					insures that EXPDEF records are emmitted by linker. 
// 					It also insures we generate prolog/epilog code for 
//					protected-mode functions which makes thunking unnecessary.
//  02/08/96 evc 	Added a subclass of combobox to track the "comboLBox" 
//					Renamed the the combobox's edit control procedure.
//	02/08/95 evc	ComboBoxProc's CV_DLL_EXPORT in wrong place
//  02/61/96 evc 	Removed forced dropdown to get ComboLBox
//	06/18/96 evc    reenabled the arrow keys for a combobox with editline
//					under win NT.  Also forced the up or down arrow key
//					to drop down the list part.
//	01/09/97 dgm	Added hooks for IntelliMouse support (see imouse.h).
//	01/19/97 dgm	Added OLE initialization for drag-and-drop support.
//	06/26/97 dgm	Fix in CTalkComboEditProc for enterHit() being called for
//					a VComboBox when it's dropped down; this should not happen,
//					the drop-down should just be dropped-up.  Also, hitting
//					ESC now causes the drop-down to be dropped-up.
// ----------------------------------------------------------------------------

#include <string.h>
#include <ctype.h>
#include <memory.h>

#include "notifier.h"
#include "control.h"
#include "window.h"
#if 1
#	include "combobox.h"
#endif
#include "memry.h"
#include "styles.h"

#ifdef CV_CTL3D
#include "ctl3d.h"
#endif

#ifndef CV_NOIMOUSE
#include "imouse.h"
#endif

#ifdef CV_OLE_DRAG_DROP
#define boolean ms_boolean
#include <ole2.h>
#undef boolean
#endif

extern int cvmain(int argc, char **argv);

// Forward declarations
void initViews (HINSTANCE 	hInstance, HINSTANCE 	hPrevInstance);
int  endViews();

#define MAXargs 20
#define MAXmesgQueue 50

extern "C" {

static int doAnyway(unsigned msg, VWindow *w);

static short VWinVersionMajor = LOBYTE(LOWORD(GetVersion()));

static boolean weAreAllDone = FALSE;

#if defined(CV_CTL3D)
//----------------------------------------------------------------------------
//
// Description: This Window Proc should be found on sub-classed VControls.
// Ctl3d expects that all controls parent's are dialogs. Views allows
// Controls to have children. Normally, controls send a WM_CTLCOLOR message
// to their parent, allowing the parent to override the control's default color.
// This functionality is found in the popup, dialog, or overlapped window
// class. In the case of a control with a control parent, there is no code,
// in a control, to choose a color for its child, so this window proc, keeps
// passing, the the message to its parent, which will be a dialog or other top
// level window, which will process the WM_CTLCOLOR message. Recursion stops
// because eventually the message will get to a non-VControl class window
// (a top level window).
//
// By passing the message to the dialog parent, it will be processed by
// Views, first (CTalkWndProc), allowing the message to be overriden at the
// Views level.
//
LRESULT CV_DLL_EXPORT CALLBACK Ctl3dCtlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (weAreAllDone) {
		return 0;
	}
	if (WM_CTLCOLOR == msg) {
		HWND	hParent;

		// we should always have a parent, because the window proc is only
		// placed on controls, which never are parented to the desktop.
#ifdef CV_WIN32
		hParent = (HWND) GetWindowLong( hWnd, GWL_HWNDPARENT);
#else
		hParent = (HWND) GetWindowWord( hWnd, GWW_HWNDPARENT);
#endif

		cv_assert( NULL != hParent);
		return SendMessage( hParent, msg, wParam, lParam);
    }

	// pass all other message to (Views) CTalkWndProc
	cv_assert(0 != notifier->lpfnWindowProc);
	return CallWindowProc(notifier->lpfnWindowProc, hWnd, msg, wParam, lParam);

} // Ctl3dCtlWndProc
#endif

// ---------------------------------------------------------------------------
// Window procedure for a C++/Views SDI application window class.
//
LRESULT CV_DLL_EXPORT CALLBACK CTalkWndProc(	HWND 		hWnd, 
								UINT	 	msg, 
								WPARAM		wParam, 
								LPARAM 		lParam)
{
	if (weAreAllDone) {
		return(long)DefWindowProc(hWnd, msg, wParam, lParam);
	}
	VWindow *wnId = 0;
	if (!(notifier->cvmessage(hWnd, msg, wParam, lParam, wnId))) {
		if (wnId && wnId->defaultProc()) {

			return(CallWindowProc(((VControl *) wnId)->defaultProc(), 
												hWnd, msg, wParam, lParam));
		}
		return(long)DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return(notifier->msgReturn);
}

// ---------------------------------------------------------------------------
// Window procedure for a C++/Views MDI application window class.
//
LRESULT CV_DLL_EXPORT CALLBACK CTalkMDIFrameProc(	HWND 		hWnd, 
									UINT	 	msg, 
									WPARAM		wParam, 
									LPARAM 		lParam)
{
	if (weAreAllDone) {
		return(long)DefFrameProc(hWnd, notifier->client(), msg, wParam, lParam);
	}
	VWindow *wnId = 0;
	if (!(notifier->cvmessage(hWnd, msg, wParam, lParam, wnId))) {
		return(long)DefFrameProc(hWnd, notifier->client(), msg, wParam, lParam);
	}
	return(notifier->msgReturn);
}

// ---------------------------------------------------------------------------
//	Window procedure for a C++/Views MDI child window class
//
LRESULT CV_DLL_EXPORT CALLBACK CTalkMDIChildProc(	HWND 		hWnd, 
									UINT	 	msg, 
									WPARAM		wParam, 
									LPARAM 		lParam)
{
	if (weAreAllDone) {
		return(long)DefMDIChildProc(hWnd, msg, wParam, lParam);
	}
	VWindow *wnId = 0;
	if (!((notifier->mdimessage(hWnd, msg, wParam, lParam, wnId)) || 
		(notifier->cvmessage(hWnd, msg, wParam, lParam, wnId))) ||
		doAnyway(msg, wnId)) {

		return(long)DefMDIChildProc(hWnd, msg, wParam, lParam);
	}
	return(notifier->msgReturn);
}

// ---------------------------------------------------------------------------
BOOL CV_DLL_EXPORT CALLBACK CTalkDlgProc(	HWND 		hWnd, 
			 				UINT	 	msg, 		
			 				WPARAM 		wParam, 
			 				LPARAM 		lParam)
{
	if (weAreAllDone) {
		return TRUE;
	}
	VWindow *wnId = 0;
	notifier->cvmessage(hWnd, msg, wParam, lParam, wnId);
	return BOOL(notifier->msgReturn);
}

// ---------------------------------------------------------------------------
int doAnyway(unsigned msg, VWindow *w)
{

	//
	// If there's a modal dialog active, and we're not it, don't process
	// the message.
	//
	if (notifier->isModalWindowActive() && !notifier->isModalWindow(w)) {
		//
		// Here, the window in which this event occurred,
		// is not the modal dialog window or a child thereof,
		// so, do not process this message.
		//
		return FALSE;
	}

	switch (msg) {
	case WM_CHILDACTIVATE:
	case WM_GETMINMAXINFO:
	case WM_MENUCHAR:	//	note, parameter packing differ from Win16 to Win32
	case WM_MOVE:
	case WM_MENUSELECT:	//	note, parameter packing differ from Win16 to Win32
	case WM_SETFOCUS:
	case WM_SIZE:
	case WM_SYSCOMMAND:
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//  This is the subclass for the combobox.
// 
LRESULT CV_DLL_EXPORT CALLBACK ComboBoxProc(	HWND 	hWnd,
									UINT 	msg,
								  	WPARAM 	wParam, 
									LPARAM 	lParam)
{
// This is to find the comboLBox handle which is the part of a combobox
// that is owned by the desktop.

	if (weAreAllDone) {
		return 0;
	}

	VComboBox *w;
	#ifdef CV_WIN16
	if (  WM_CTLCOLOR  == msg)  // combo controls are to be painted.
	#else
	if ( WM_CTLCOLORLISTBOX == msg) // 32 bits has new message.
	#endif
	{
		if ((w =(VComboBox*) notifier->getThis(hWnd)) != 0) {
			#if 0
			// causes cosmetic difficulties.  
			if (! w->getComboLboxHwnd () ) {
				// force a paint of the listbox
				SendMessage(hWnd, CB_SHOWDROPDOWN, TRUE, 0);
				PostMessage(hWnd, CB_SHOWDROPDOWN, FALSE, 0);
			}
			#endif
		#ifdef CV_WIN16
			if (CTLCOLOR_LISTBOX == HIWORD(lParam))
		#endif
			{
				#ifdef CV_WIN16
					w->setComboLBoxHwnd((HWND)LOWORD(lParam));
				#else
					w->setComboLBoxHwnd ((HWND)lParam);
				#endif
			}
		}
	}

	// force the list to drop down when a user hist up or down arrow
	if ( WM_KEYDOWN  == msg){
		if ((VK_DOWN == wParam) || (VK_UP== wParam)){
		 VWindow * w;
			//if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(hWnd)) != 0) {
					if (!((VComboBox *)	w)->isDroppedDown()){
					((VComboBox *)	w)->showDropDown(TRUE);
					}
				}
			//}
		}
		else if (wParam == VK_RETURN) {
			VComboBox *combobox = (VComboBox *)notifier->getThis(hWnd);
			if (combobox != 0) {
				if (combobox->isDroppedDown()) {
					combobox->showDropDown(FALSE);
					combobox->selectItem(combobox->selectedItem());
    				MSG msg;
    				PeekMessage(&msg, hWnd, WM_CHAR, WM_CHAR,
								PM_REMOVE | PM_NOYIELD);
					return 0;
				}
			}
		}
	}

	return CallWindowProc(notifier-> lpfnSaveComboProc,
						  hWnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
// This is the subclass window proc for the editline  owned by
// a combobox
LRESULT CV_DLL_EXPORT CALLBACK CTalkComboEditProc(	HWND 	hWnd, 
									UINT 	msg,
								  	WPARAM 	wParam, 
									LPARAM 	lParam)
{
	if (weAreAllDone) {
		return 0;
	}
	HWND combobox_hwnd;
	VWindow *w;
	int shift;

	switch (msg) {
 
	case WM_SETFOCUS:
		if ((combobox_hwnd = GetParent(hWnd)) != 0) {
			if (combobox_hwnd != (HWND) wParam){
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					w->givenFocus();
				}
		  	}
		}  		  
		break;
	case WM_KILLFOCUS:
		if ((combobox_hwnd = GetParent(hWnd)) != 0) {
			if (combobox_hwnd != (HWND) wParam){
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					w->clearFocus();
				}
		  	}	 
			// if I'm not losing focus to the combobox part of 
			// the Combobox then the Views Combobox 
		  	// entity is really losing focus.
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_DOWN:
		case VK_UP:
			if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					if (!((VComboBox *)	w)->isDroppedDown()){
						((VComboBox *)	w)->showDropDown(TRUE);
					}
				}
			}

			break;
		case VK_TAB:
			shift = (GetKeyState(VK_SHIFT) & 0x8000);
			if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					if (shift) {
						if (w->prevFocus()) {
							return 0;
						}
					}
					else {
						if (w->nextFocus()) {
							return 0;
						}
					}
				}
			}
			break;
		case VK_RETURN:
			if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					if (((VComboBox *)w)->isDroppedDown()) {
						((VComboBox *)w)->showDropDown(FALSE);
						break;
					}
					else {
						w->enterHit();
						return 0;
					}
				}
			}
			break;
		case VK_ESCAPE:
			if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					if (((VComboBox *)w)->isDroppedDown()) {
						((VComboBox *)w)->showDropDown(FALSE);
						break;
					}
					else if ((w = w->getParent()) != 0) {
						w->close();
						return 0;
					}
				}
			}
			break;

	


		default:
		// fix combobox so it calls key()
			if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					int key = VNotifier::getVirtKey(wParam);
		 			if (w->key(key, (char)0)){
						return 0;
				 	} else {
						break;
					}

				}
			}
			break;
		}

		break;
	case WM_KEYUP:
		switch (wParam) {
		case VK_TAB:
			return 0;
		case VK_RETURN:
			return 0;
		case VK_ESCAPE:
			return 0;
		default:
			if ((combobox_hwnd = GetParent(hWnd)) != 0) {
				if ((w = notifier->getThis(combobox_hwnd)) != 0) {
					int key = VNotifier::getVirtKey(wParam);
					if (w->keyUp(key, (char)0)){
						return 0;
					} else {
						break;
					}
				
				}
			}
		}
		break;
	case WM_CHAR:
		switch (wParam) {
		case VK_TAB:
			return 0;
		case VK_RETURN:
			return 0;
		case VK_ESCAPE:
			return 0;
		}
		break;
	}
	return CallWindowProc(notifier->lpfnSaveComboEditProc,
						  hWnd, msg, wParam, lParam);
}

// ---------------------------------------------------------------------------
void CV_DLL_EXPORT CALLBACK ViewsTimerProc(HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime)
{
	if (weAreAllDone) {
		return;
	}
	notifier->timerExpired(idTimer);
}

//---------------------------------------------------------------------------
// MS Windows entry point
//
int PASCAL WinMain(	HINSTANCE 	hInstance, 
					HINSTANCE 	hPrevInstance, 
					LPSTR 		lpCmdLine, 
					int 		nCmdShow)
{
	for (int size = MAXmesgQueue; size > 0; size -= 4) {
		if (SetMessageQueue(size)) {
			break;
		}
	}

	//	We use CTWindow for the class name when we register the application.
	//	CTWindow is defined by the application (typically, in the file which 
	//	contains cvmain()) and, by convention, is the name of the application
	//	file containing cvmain().

	char	*av[MAXargs];
	int	ac;
	char	szFileName[_MAX_PATH];
    
	GetModuleFileName(hInstance, szFileName, sizeof(szFileName));
	av[0] = szFileName;
	ac	  = 1;

	if (lpCmdLine != 0) {
		int inQuote = 0;
		char* q = lpCmdLine;
		for (char *p = lpCmdLine; (*p != '\0') && (ac < MAXargs-1); p++) {
			if (isspace(*p) && !inQuote) {
				// terminate previous argument
				if (ac > 1) {
					*q++ = '\0';
				}
				// advance to next non-space character
				for (p++; (isspace(*p) || *p == '\"') && !inQuote; p++) {
					if (*p == '\"')
						inQuote = !inQuote;
				}
				// stop if end of command line
				if (*p == '\0') {
					break;
				}
				else {
					// start new argument
					av[ac] = q;
					ac++;
					// copy character
					*q++ = *p;
				}
			}
			// skip "'s
			else if (*p == '\"') {
				inQuote = !inQuote;
			}
			else {
				if (ac == 1) {
					// start new argument
					av[ac] = q;
					ac++;
				}
				// copy character
				*q++ = *p;
			}
		}
		// terminate previous argument
		if (ac > 1) {
			*q = '\0';
		}
		av[ac] = 0;
	}

	initViews (hInstance, hPrevInstance); // finish initializing our notifier
	cvmain(ac, av);
	int result = endViews(); 
	weAreAllDone = TRUE;
	return result;
}
} // extern "C"


// ---------------------------------------------------------------------------
// initViews finishes initializing the notifier , and then sets
// a few Windows hooks.  For a Views View to execute
// properly within a thread, that thread must create single  
// a notifier object, and then execute initViews.  This is required for
// each thread of execution. (Notice that the windows hooks
// occur on a per-thread basis).  For Win16, there is, of cource
// only 1 "thread" of execution, whose code is largely 
// embodied in cvmain()
// 
void initViews (HINSTANCE 	hInstance, HINSTANCE 	hPrevInstance)
{

	//	Window procedure  for the SDI application window class 

	notifier->pCTalkWndProc 		= CTalkWndProc;

	//	Window procedurse for the MDI application window classes

	notifier->pCTalkMDIFrameProc 	= CTalkMDIFrameProc;
	notifier->pCTalkMDIChildProc 	= CTalkMDIChildProc;

	notifier->lpfnWindowProc 		= CTalkWndProc;
	notifier->lpfnMDIFrameProc 		= CTalkMDIFrameProc;
	notifier->lpfnMDIChildProc 		= CTalkMDIChildProc;

	notifier->lpfnDialogProc 		= CTalkDlgProc;

	notifier->lpfnComboEditProc 		= CTalkComboEditProc;
	notifier->lpfnSaveComboEditProc	= 0;

	// for comboLBox
	notifier->lpfnComboProc 		= ComboBoxProc;
	notifier->lpfnSaveComboProc		= 0;

	#if defined(CV_CTL3D)
	notifier->lpfnCtl3dCtlWndProc	= Ctl3dCtlWndProc;
	#else
	notifier->lpfnCtl3dCtlWndProc	= 0;
	#endif
	notifier->lpfnTimerProc			= ViewsTimerProc;

	// message filter for menu
	notifier->lpfnMsgFilterHook 	= VNotifier::MsgFilterHook;

	// message filter for dialog processing
	notifier->lpfnCBTHook 			= VNotifier::CBTHook;

	notifier->lpfnCommDlgProc 		= VNotifier::CommDlgProc;


	notifier->mainwin(hInstance, hPrevInstance, CTWindow);

#if defined(CV_WIN32)
		notifier->hCBTHook = SetWindowsHookEx(WH_CBT, 
										notifier->lpfnCBTHook,
										hInstance,
										GetCurrentThreadId());
										// WAS PROCESS
#else
		notifier->hCBTHook = SetWindowsHookEx(WH_CBT, 
										notifier->lpfnCBTHook,
										hInstance,
										GetCurrentTask());
#endif

#if defined(CV_CTL3D)
// Some versions of CTL3D put up a dialog box to indicate the wrong version of windows is
// being used. To stop this we have to detect the version ourselves.
cv_assert(VWinVersionMajor > 0);
if (VWinVersionMajor < 4) {
	notifier->ctl3dIsActive = Ctl3dRegister(notifier->inst());
}
#else
	notifier->ctl3dIsActive = FALSE;
#endif

#ifndef CV_NOIMOUSE
	VIntelliMouse::initialize();
#endif

	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleCloseBoxSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyle3DSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleSizeableSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleHorizontalSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleVerticalSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleMaximizeSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleMinimizeSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleMaximizedSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleComboSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleBorderSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleAutoToggleSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleDefaultButtonSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleAutoDefaultSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleJustificationSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleVisiblitySet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleWordWrapSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleSortedSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleCaptionSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleDialogSet);
#if defined(MOTIF)
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleAutoScrollSet);
#endif
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleEditSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleShadowSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleSizeRedrawSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleSelectionSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleTabPlacementSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleTabTypeSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleNotebookBindingSet);
	VExclusiveStyle::setExclusive((VExclusiveStyle**)vStyleTreeDisplaySet);

#if defined(CV_OLE_DRAG_DROP) && defined(CV_WIN32)
	SetMessageQueue(96);
	OleInitialize(NULL);
#endif

}


/* 
End Views is called after a thread (cv_main in the single thread case
exits.  It unhooks procs set in initViews. 
*/

int endViews()
{

#if defined(CV_OLE_DRAG_DROP) && defined(CV_WIN32)
	OleUninitialize();
#endif

#if defined(CV_CTL3D)
	if (notifier->ctl3dIsActive) {
		Ctl3dUnregister(notifier->inst());
	}
#endif

	return(notifier->getExitCode());
}
