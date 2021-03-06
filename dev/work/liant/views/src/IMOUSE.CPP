//	imouse.cpp -- [UNDER DEVELOPMENT]
//
//	VIntelliMouse implementation
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
//	12/06/96 dgm	Original.  For a complete description and SDK, see:
//					http://www.microsoft.com/products/hardware/
//											 intellimouse/sdkhome.htm.
// --------------------------------------------------------------------------

#include "imouse.h"
#include "window.h"
#include "notifier.h"

// --------------------------------------------------------------------------
//
#ifndef CV_NOIMOUSE
#	ifndef CV_WIN32
#		define CV_NOIMOUSE
#	endif
#endif

// --------------------------------------------------------------------------
//
#ifndef CV_NOIMOUSE
#	include "zmouse.h"
#endif

// --------------------------------------------------------------------------
//
unsigned int VIntelliMouse::wheelMessageId = 0;

// --------------------------------------------------------------------------
//
#ifndef CV_NOIMOUSE

	static int	imouseInitialized		= 0;
	static int	imousePresent			= 0;
	static int	imouseScrollLines		= 0;
	static int	imouseBuiltinSupport	= 0;

#	ifndef SPI_GETWHEELSCROLLLINES
#		define SPI_GETWHEELSCROLLLINES	104
#	endif

#endif

// --------------------------------------------------------------------------
//
void VIntelliMouse::initialize()
{

#	ifndef CV_NOIMOUSE

		imouseInitialized = 1;

		// Windows 95 and Windows NT 3.51 support IntelliMouse indirectly
		// by registering the MSH_MOUSEWHEEL message and communicating with
		// the MSWHEEL system module; future versions of Window 95 and
		// Windows NT will include direct builtin support for IntelliMouse.

		OSVERSIONINFO osversion;
		memset(&osversion, 0, sizeof(OSVERSIONINFO));
		osversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osversion);

		if ((osversion.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) ||
			((osversion.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
			 (osversion.dwMajorVersion < 4))) {
			imouseBuiltinSupport = 0;
		}
		else if ((osversion.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
				 (osversion.dwMajorVersion >= 4)) {
			imouseBuiltinSupport = 1;
		}
		else {
			imouseBuiltinSupport = 0;
		}

		// Check for the presence of IntelliMouse, register the
		// MSH_MOUSEWHEEL message if necessary, and determine
		// the number of scroll lines per wheel rotation.

		imousePresent = 0;
		imouseScrollLines = 3;
		wheelMessageId = 0;

		if (imouseBuiltinSupport) {
#			ifdef SM_MOUSEWHEELPRESENT
				imousePresent = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
#			endif
			if (imousePresent) {
				SystemParametersInfo
					(SPI_GETWHEELSCROLLLINES, 0, &imouseScrollLines, 0);
			}
		}
		else {
			UINT msh_wheelsupport_msg =
				 RegisterWindowMessage(MSH_WHEELSUPPORT);
			if (msh_wheelsupport_msg != 0) {
				HWND hwnd = FindWindow(MSH_WHEELMODULE_CLASS,
									   MSH_WHEELMODULE_TITLE);
				if (hwnd != 0) {
					imousePresent =
						SendMessage(hwnd, msh_wheelsupport_msg, 0, 0);
					if (imousePresent) {
				 		wheelMessageId =
							RegisterWindowMessage(MSH_MOUSEWHEEL);
						UINT msh_scroll_lines_msg =
							 RegisterWindowMessage(MSH_SCROLL_LINES);
						if (msh_scroll_lines_msg != 0) {
							imouseScrollLines =
								SendMessage(hwnd,
											msh_scroll_lines_msg, 0, 0);
						}
					}
				}
			}
		}

#	endif

}

// --------------------------------------------------------------------------
//
void VIntelliMouse::systemSettingsChanged()
{
	initialize();
}

// --------------------------------------------------------------------------
//
boolean VIntelliMouse::isPresent()
{

#	ifndef CV_NOIMOUSE

		if (!imouseInitialized) {
			initialize();
		}
		return imousePresent;

#	else

		return FALSE;

#	endif

}

// --------------------------------------------------------------------------
//
int VIntelliMouse::getScrollLinesPerWheelRotation()
{

#	ifndef CV_NOIMOUSE

		if (!imouseInitialized) {
			initialize();
		}
		return imouseScrollLines;

#	else

		return 0;

#	endif

}

// --------------------------------------------------------------------------
//
boolean	VIntelliMouse::handleWheelMessage(WPARAM wparam, LPARAM lparam)
{

#	ifndef CV_NOIMOUSE

		int x = LOWORD(lparam), y = HIWORD(lparam), delta;

		if (imouseBuiltinSupport) {
			delta = HIWORD(wparam);
		}
		else {
			delta = wparam;
		}

		delta = (delta / WHEEL_DELTA) * imouseScrollLines;

		for (VWindow *w = notifier->getFocus() ; w != 0 ;
												 w = w->getParent()) {
			if (w->mouseWheel(delta, x, y, 0)) {
				return TRUE;
			}
			else if (w->isTopLevel()) {
				break;
			}
		}

#	endif

		return FALSE;
}
