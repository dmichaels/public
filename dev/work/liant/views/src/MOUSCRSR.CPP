// $Header:   Q:/views/win/vcs/mouscrsr.cpv   1.9   Mar 04 1997 17:55:26   gregm  $ 

//	mouscrsr.cpp
//
//	VMousreCursor implementation
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
//	10/27/92 jmd	added new Size mouse cursors
//	08/09/93 jmd	removed free
//	08/12/93 pat	added defineClass
//	03/25/94 pat	STRICT: changed HANDLE cast to HINSTANCE in LoadCursor().
//	03/26/94 pat	CV_WIN32: added explicit int cast on POINT long members.
//	07/28/95 jld	added new cursor types: AppStarting, Icon, No, Help
//	08/10/95 dgm	Added #ifdef around IDC_APPSTARTING, IDC_NO, & IDC_ICON.
//	04/30/97 dgm	IDC_SIZE is now obsolete.
// ----------------------------------------------------------------------------

#include "mouscrsr.h"
#include "notifier.h"

defineClass(VMouseCursor, VObject)

// ---------------------------------------------------------------------------
// Construct an VMouseCursor object with the default 'arrow' shape.
//
VMouseCursor::VMouseCursor() : cursWin(0), idcCursor(TRUE)
{
	hpCursor = LoadCursor((HINSTANCE) 0, IDC_ARROW);
}

VMouseCursor::~VMouseCursor()
{
	if( !idcCursor ){
		DestroyCursor( hpCursor );
	}
}

// ---------------------------------------------------------------------------
// Construct a VMouseCursor object from the resource specified by the
// C string 'cursorName'.
//
VMouseCursor::VMouseCursor(char *cursorName) : cursWin(0), idcCursor(TRUE)
{
	hpCursor = LoadCursor(notifier->inst(), cursorName);
}

// ---------------------------------------------------------------------------
// Construct a VMouseCursor with shape of 'type', where 'type' is one of
// the enum 'CursorShape's
//
VMouseCursor::VMouseCursor(enum CursorShape type) : cursWin(0), idcCursor(TRUE)
{
	switch (type) {
	case UpArrow:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_UPARROW);
		break;
	case Ibeam:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_IBEAM);
		break;
	case FourArrow:
#if defined(CV_WIN32)
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_SIZEALL);
#else
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_SIZE);
#endif
		break;
	case Cross:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_CROSS);
		break;
	default:
	case Arrow:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_ARROW);
		break;
	case HourGlass:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_WAIT);
		break;
	case SizeNWSE:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_SIZENWSE);
		break;
	case SizeNESW:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_SIZENESW);
		break;
	case SizeWE:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_SIZEWE);
		break;
	case SizeNS:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_SIZENS);
		break;
#if defined(CV_WIN32)
	case AppStarting:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_APPSTARTING);
		break;
	case No:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_NO);
		break;
	case Icon:
		hpCursor = LoadCursor((HINSTANCE) 0, IDC_ICON);
		break;
#endif
	case Help:
#if defined(CV_WIN32)
		if (LOBYTE(LOWORD(GetVersion())) >= 4) {
			hpCursor = LoadCursor((HINSTANCE) 0, IDC_HELP);
		}
		else
#else
		{
			idcCursor = FALSE;
			// Create our own 32x32 monochrome help cursor.
			// Where did these masks come from? you might ask -- don't
			const unsigned long helpAndBits[] = {
				 0xFFFFFF1F, 0xFFFFFF0F, 0xFF0FF007, 0xFF07E003, 0xFF03C001, 0xFF018000, 0xFF000000, 0xFF800300,
				 0xFFC00700, 0xFFC00700, 0xFF800700, 0xFF010700, 0xFF030600, 0xFF077C00, 0xFF0F7800, 0xFF1F3810,
				 0xFF1F3838, 0xFF1F18F8, 0xFF1F18FC, 0xFF1F08FC, 0xFFFF0FFE, 0xFFFF0FFE, 0xFF1FF8FF, 0xFF1FF8FF,
				 0xFF1FF8FF, 0xFF1FF8FF, 0xFF1FF8FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
			};
			
			const unsigned long helpXorBits[] = {
				 0x00000000, 0x00000040, 0x00000060, 0x00000070, 0x00000078, 0x0000007C, 0x0000007E, 0x0000007F,
				 0x0000007F, 0x0000007F, 0x0000007F, 0x0000F07F, 0x0000007E, 0x00000066, 0x00000047, 0x00000003,
				 0x00008003, 0x00008001, 0x0000C001, 0x0000C000, 0x0000E000, 0x00000000, 0x00000000, 0x00000000,
				 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
			};
			
			hpCursor = CreateCursor( notifier->inst(), 0, 0, 32, 32,
						helpAndBits, helpXorBits );
		}
#endif
		break;
	}
}

// ---------------------------------------------------------------------------
// Return the current (x, y) position of this VMouseCursor into '*x'
// and '*y', respectively.  If this VMouseCursor is attached to a VWindow, 
// then return the the cursor position in window client area coordinates. 
// Otherwise, return the position in screen coordinates.  
// NOTE: Screen coordinates can be mapped onto a VWindow object\'s local
// coordinate space by using the mapToWin(int *, int *) member of the
// VWindow class.
//
void VMouseCursor::getCursorPos(int *x, int *y)
{
	POINT p;
	GetCursorPos((LPPOINT)&p);

	//	pat--under CV_WIN32 POINT coordinates are longs, we are truncating!	

	*x = (int) p.x;	
	*y = (int) p.y;

	if (cursWin) {
		cursWin->mapToWin(x, y);
	}
}

// ---------------------------------------------------------------------------
// Set this VMouseCursor position to ('x','y'), where 'x' and 'y' are
// assumed to be in window client coordinates if this VMouseCursor is attached
// to a VWindow object.  However, if this VMouseCursor is not attached to a
// VWindow object, then 'x' and 'y' are assumed to be in screen coordinates.
//
void VMouseCursor::setCursorPos(int x, int y)
{
	if (cursWin)  {
		cursWin->mapToScreen(&x, &y);
	}
	SetCursorPos(x, y);
}



