// $Header:   Q:/views/win/vcs/VOLEUI.H_v   1.7   Mar 04 1997 14:14:28   gregm  $

//  Views OLE2UI Definitions and Prototypes
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
//	Some definitions and functions did not migrate from WIN16 to
//	WIN32 so they are replicated here.
//
//	Revision History:
//	-----------------
//	05/31/95 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//  02/23/96 dss    removed inlcude of ole2.h.
//	---------------------------------------------------------------------------

#ifndef VOLEUI_H
#define VOLEUI_H

#if defined(CV_WIN32)

#if defined(__BCPLUSPLUS__) && !defined(INC_OLE2)
#	define INC_OLE2
#endif

#include "windows.h"
#include "object.h"

/*************************************************************************
 ** OLE OBJECT FEEDBACK EFFECTS
 *************************************************************************/

#define OLEUI_HANDLES_USEINVERSE    0x00000001L
#define OLEUI_HANDLES_NOBORDER      0x00000002L
#define OLEUI_HANDLES_INSIDE        0x00000004L
#define OLEUI_HANDLES_OUTSIDE       0x00000008L

#define OLEUI_SHADE_FULLRECT        1
#define OLEUI_SHADE_BORDERIN        2
#define OLEUI_SHADE_BORDEROUT       3

// STDAPI_(void) OleUIDrawShading(LPRECT lpRect, HDC hdc, DWORD dwFlags, UINT cWidth);
// STDAPI_(void) OleUIShowObject(LPCRECT lprc, HDC hdc, BOOL fIsLink);

#endif	// defined(WIN32)

void DrawItemHandles(VComponentItem& item, DWORD dwFlags, UINT cSize, BOOL fDraw);

#endif	// VOLEUI_H
