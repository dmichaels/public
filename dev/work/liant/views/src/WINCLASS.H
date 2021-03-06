// $Header:   Q:/views/win/vcs/winclass.h_v   1.15   Mar 04 1997 16:33:16   gregm  $ 

//	winclass.h
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
//	08/13/92 jmd	fixed ifdef's for BC3.1 compiler
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	03/19/93 pat	preened
//	03/23/93 jmd	made CV_WIN32 auto define CV_WINDOWS; excluded
//					re-definitions of LPARAM and WPARAM under CV_WIN32.
//	07/14/94 jld	added defs.h dependency for CV_WINDOWS
//	10/11/94 dss	Added include windowsx.h to get access to MSW message
//					crackers and WIN32 macros.
//	03/06/95 jld	removed ZORTECH ifdefs
//	07/13/95 dss	added WS_EX_CLIENTEDGE if not defined.
//					Protect windows.h with MS_boolean.
//	10/30/95 dss	added WS_EX_CONTEXTHELP if not defined. Define OLE2ANSI
//					before including windows.h.
//	11/02/95 dgm	Added WS_EX_WINDOWEDGE, WS_EX_STATICEDGE, SM_CYSMCAPTION,
//					SM_CXSMSIZE, SM_CYSMSIZE, SM_CXSIZE, and SM_CYSIZE.
//	11/09/95 dss	take out the OLE2ANSI definition.
//	11/14/95 dgm	Added SM_CXSIZE and SM_CYSIZE.
//	12/05/95 dgm	Added Windows 95 COLOR_xxx definitions.
//	01/29/96 dgm	More Windows 95 definitions.
//	01/31/96 dgm	Minor fix.
//	06/12/96 pkt	added VHANDLE, VHWND, and VMENU.
// ----------------------------------------------------------------------------

#ifndef winclass_h
#define winclass_h

#include "defs.h"

#ifdef CV_WINDOWS

#	define NOKANJI
#	define NOSOUND
#	undef LPTR

extern "C" {
// Avoid conflict with Microsoft typedef of boolean by defining over it 
// while loading the MS headers.
//
#	define boolean MS_boolean

#	include <windows.h>
#	include <windowsx.h>
#	undef boolean
}


#	undef min
#	undef max

#	ifndef ES_READONLY
#		define ES_READONLY	0x00000800L
#	endif

// Some versions of the 32 bit SDK don't define these
#	if defined(CV_WIN32)
#		if !defined(WS_EX_CONTEXTHELP)
#			define WS_EX_CONTEXTHELP	0x00000400L
#		endif
//#		if WINVER >= 0x0400
#			if !defined(COLOR_3DDKSHADOW)
#					define COLOR_3DDKSHADOW        21
#			endif
#			if !defined(COLOR_3DLIGHT)
#					define COLOR_3DLIGHT           22
#			endif
#			if !defined(COLOR_INFOTEXT)
#					define COLOR_INFOTEXT          23
#			endif
#			if !defined(COLOR_INFOBK)
#					define COLOR_INFOBK            24
#			endif
#			if !defined(COLOR_3DFACE)
#					define COLOR_3DFACE            COLOR_BTNFACE
#			endif
#			if !defined(COLOR_3DSHADOW)
#					define COLOR_3DSHADOW          COLOR_BTNSHADOW
#			endif
#			if !defined(COLOR_3DHIGHLIGHT)
#					define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#			endif
#			if !defined(COLOR_3DHILIGHT)
#					define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT
#			endif
#			if !defined(COLOR_BTNHILIGHT)
#					define COLOR_BTNHILIGHT        COLOR_BTNHIGHLIGHT
//#			endif
#		endif
#	endif

#	if !defined(WS_EX_CLIENTEDGE)
#		define WS_EX_CLIENTEDGE		0x00000200L
#	endif
#	if !defined(WS_EX_WINDOWEDGE)
#		define WS_EX_WINDOWEDGE		0x00000100L
#	endif
#	if !defined(WS_EX_STATICEDGE)
#		define WS_EX_STATICEDGE		0x00020000L
#	endif
#	if !defined(SM_CXFIXEDFRAME)
#		define SM_CXFIXEDFRAME		SM_CXDLGFRAME
#	endif
#	if !defined(SM_CYFIXEDFRAME)
#		define SM_CYFIXEDFRAME		SM_CYDLGFRAME
#	endif
#	if !defined(SM_CXSIZEFRAME)
#		define SM_CXSIZEFRAME		SM_CXFRAME
#	endif
#	if !defined(SM_CYSIZEFRAME)
#		define SM_CYSIZEFRAME		SM_CYFRAME
#	endif
#	if !defined(SM_CYSMCAPTION)
#		define SM_CYSMCAPTION		51
#	endif
#	if !defined(SM_CYEDGE)
#		define SM_CYEDGE			46
#	endif
#	if !defined(SM_CXSMSIZE)
#		define SM_CXSMSIZE			52
#	endif
#	if !defined(SM_CYSMSIZE)
#		define SM_CYSMSIZE			53
#	endif
#	if !defined(SM_CXSIZE)
#		define SM_CXSIZE			30
#	endif
#	if !defined(SM_CYSIZE)
#		define SM_CYSIZE			31
#	endif
#	if !defined(SM_CXEDGE)
#		define SM_CXEDGE			45
#	endif

#	if !defined(CV_WIN16) && !defined(CV_WIN32)
#		define WPARAM WORD
#		define LPARAM LONG
#	endif

#endif	//	!CV_WINDOWS

typedef HANDLE  VHANDLE; // VHANDLE used by common files
typedef HWND    VHWND;   // VHWND used by common files
typedef HMENU   VHMENU;  // VHMENU used by common files

#endif	//	!winclass_h
