// $Header:   Q:/views/win/vcs/OLEDEFS.H_v   1.11   Mar 04 1997 14:13:18   gregm  $

//	oledefs.h
//
//  Views OLE Definitions
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
//	01/23/94 dss	Initial release.
//	11/16/94 dss	Defined _OT, LPOLESTR for CV_WIN16.
//	12/05/95 dss	define _T for non-windows. Always include tchar for
//					windows.
//	01/09/96 dss	VComponent name change.
//	02/06/96 dss	Received a whole lotta code from pre.h.
//					undef the enable/disable macro from borland's include files.
//	---------------------------------------------------------------------------

#ifndef OLEDEFS_H
#define OLEDEFS_H

#if defined(__OLE_H) || defined(_OLE2_H_)
//	#error "Must include oledefs.h before ole2.h."
#endif

// this has to come before windows.h on borland and msvc20
// it not used in 16 bit windows.
#if defined(__BCPLUSPLUS__) && !defined(INC_OLE2)
#	define INC_OLE2
#endif

#include "defs.h"		// for windows.h, among others

#include <ole2.h>

// OLE User Interface, OleStdXxx functions

#if defined(__BCPLUSPLUS__)
 #if defined(CV_WIN16)
	#include <olestd.h>
	#include <oleguid.h>
 #else
	#include <bolestd.h>
// the following is for droptarg.cpp
	#define OleStdGetDropEffect(grfKeyState)    \
	   ( (grfKeyState & MK_CONTROL) ?          \
	      ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_LINK : DROPEFFECT_COPY ) :  \
	      ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_MOVE : 0 ) )
STDAPI_(void) XformSizeInHimetricToPixels(HDC, LPSIZEL, LPSIZEL);
STDAPI_(int)        XformWidthInHimetricToPixels(HDC, int);
STDAPI_(int)        XformWidthInPixelsToHimetric(HDC, int);
STDAPI_(int)        XformHeightInHimetricToPixels(HDC, int);
STDAPI_(int)        XformHeightInPixelsToHimetric(HDC, int);


 #endif
	#include <ole2ui.h>
#elif defined(CV_WIN16)
	// These are 16 bit OLE
	#include <olestd.h>
	#include <ole2ui.h>
	#include <oleguid.h>
#elif defined(CV_WIN32)
	// These are 32 bit OLE
	#include <oledlg.h>
	#include "volestd.h"		// this is IAF, so no angle brackets
#else
	#error("must define CV_WIN16 or CV_WIN32")
#endif

#include <ole2ver.h>

#if defined(CV_WIN16)
	#include <ole2dbg.h>
#endif

// OLE Automation
#if defined(CV_WIN16)
	#include <variant.h>
	#include <dispatch.h>
#elif defined(CV_WIN32)
	#if defined(_MSC_VER)
		#include <oaidl.h>
	#endif
	#include <oleauto.h>
#endif

#include <olectl.h>

// borland olestd calls dos.h which defines these
// WARNING: put all OLE includes (from the compiler include dir) BEFORE this line!
#if defined(__BCPLUSPLUS__)
	#undef enable
	#undef disable
#endif

#if defined(CV_WIN32) && defined(__BCPLUSPLUS__)
	// 32 bit borland doesn't like nameless unions.
	#define LIGet32Hi(li)	((li).u.HighPart)
	#define LIGet32Lo(li)	((li).u.LowPart)
	
	#define ULIGet32Hi(li)	((li).u.HighPart)
	#define ULIGet32Lo(li)	((li).u.LowPart)
#else	// Microsoft
	#define LIGet32Hi(li)	((li).HighPart)
	#define LIGet32Lo(li)	((li).LowPart)
	
	#define ULIGet32Hi(li)	((li).HighPart)
	#define ULIGet32Lo(li)	((li).LowPart)
#endif

#include "object.h"

#if defined(CV_WIN32)
	// OLE is UNICODE in 32 bits.
	#include "vwchar.h"
#endif

// MS defines NOERROR and a literal 0, which is an int. The problem is HRESULT's
// are pointers and SCODE are longs and the OLEResult class uses both, so to 
// disambiguate the promotion from int to longs or pointers to the OLEResult
// class, we cast to to an HRESULT (kinda like NULL).
#if defined(CV_WIN16)
	#undef NOERROR
	#define NOERROR HRESULT(0)
#endif

#if defined(_MSC_VER)
	#include <tchar.h>
#else
	#define _tcscmp     strcmp
	#define _tcscpy     strcpy
	#define _tcslen     strlen
#endif

#if !defined(_T)
	#if defined(UNICODE)
		#define _T(x) L##x
	#else
		#define _T(x) x
	#endif
#endif

#if defined(CV_WIN32)
	#define _OT(x) L##x
#else
	#define _OT(x) x
#endif

#if defined(CV_WIN16)
typedef TCHAR FAR*			LPTSTR;
typedef const TCHAR FAR*	LPCTSTR;
#endif

#if defined(CV_WIN16)
typedef char      OLECHAR;
typedef LPSTR     LPOLESTR;
typedef LPCSTR    LPCOLESTR;
#endif

typedef const POINT FAR *LPCPOINT;
typedef const POINTL FAR *LPCPOINTL;
typedef const SIZE FAR *LPCSIZE;
typedef const SIZEL FAR *LPCSIZEL;
typedef const RECT FAR *LPCRECT;
typedef const RECTL FAR *LPCRECTL;

CLASS VObject;
typedef VObject FAR *LPVOBJECT;
typedef VObject FAR& REFVOBJECT;

CLASS VWindow;
typedef VWindow FAR *LPVWINDOW;
typedef VWindow FAR& REFVWINDOW;

CLASS VView;
typedef VView FAR *LPVVIEW;
typedef VView FAR& REFVVIEW;

CLASS VAppView;
typedef VAppView FAR *LPVAPPVIEW;
typedef VAppView FAR& REFVAPPVIEW;

CLASS VMdiAppView;
typedef VMdiAppView FAR *LPVMDIAPPVIEW;
typedef VMdiAppView FAR& REFVMDIAPPVIEW;

CLASS VStdMdiAppView;
typedef VStdMdiAppView FAR *LPVSTDMDIAPPVIEW;
typedef VStdMdiAppView FAR& REFVSTDMDIAPPVIEW;

CLASS VComponentItem;
typedef VComponentItem FAR *LPVCOMPONENTITEM;
typedef VComponentItem FAR& REFVCOMPONENTITEM;

CLASS VComponentObject;
typedef VComponentObject FAR *LPVCOMPONENTOBJECT;
typedef VComponentObject FAR& REFVCOMPONENTOBJECT;

CLASS VComponentWindow;
typedef VComponentWindow FAR *LPVCOMPONENTWINDOW;
typedef VComponentWindow FAR& REFVCOMPONENTWINDOW;

CLASS VComponentView;
typedef VComponentView FAR *LPVCOMPONENTVIEW;
typedef VComponentView FAR& REFVCOMPONENTVIEW;

CLASS VComponentAppView;
typedef VComponentAppView FAR *LPVCOMPONENTAPPVIEW;
typedef VComponentAppView FAR& REFVCOMPONENTAPPVIEW;

CLASS VComponentStorage;
typedef VComponentStorage FAR *LPVCOMPONENTSTORAGE;
typedef VComponentStorage FAR& REFVCOMPONENTSTORAGE;

CLASS VCDTransferAgent;
typedef VCDTransferAgent FAR *LPVCOMPONENTTRANSFERAGENT;
typedef VCDTransferAgent FAR& REFVCOMPONENTTRANSFERAGENT;

CLASS CRUnknown;
typedef CRUnknown			FAR* LPCRUNKNOWN;
typedef CRUnknown			FAR& REFCRUNKNOWN;

CLASS CROleObject;
typedef CROleObject			FAR* LPCROLEOBJECT;
typedef CROleObject			FAR& REFCROLEOBJECT;

CLASS CRStorage;
typedef CRStorage			FAR* LPCRSTORAGE;
typedef CRStorage			FAR& REFCRSTORAGE;

CLASS CRSubStorage;
typedef CRSubStorage		FAR* LPCRSUBSTORAGE;
typedef CRSubStorage		FAR& REFCRSUBSTORAGE;

CLASS CRRootStorage;
typedef CRRootStorage		FAR* LPCRROOTSTORAGE;
typedef CRRootStorage		FAR& REFCRROOTSTORAGE;

CLASS CRDataObject;
typedef CRDataObject		FAR* LPCRDATAOBJECT;
typedef CRDataObject		FAR& REFCRDATAOBJECT;

typedef STATSTG FAR *LPSTATSTG;

#define MULTI_INHERIT_HACK(this) (LPVOBJECT(long(this)))

#define NUM_ELEMENTS_IN(x) (sizeof(x)/sizeof(x[0]))

#define CONTAIN_INTERFACE(intf, access)	CONTAIN_##intf##_INTERFACE()
#define INHERIT_INTERFACE(intf, access)	INHERIT_##intf##_INTERFACE()

// equivilances for xxx_ACCESS() macros. Theses are private and not supposed to
// be used.
typedef LPUNKNOWN			LPUnknown;
typedef LPOLEOBJECT			LPOleObject;
typedef LPSTORAGE			LPStorage;
typedef LPDATAOBJECT		LPDataObject;

#if defined(CV_WIN16)
	typedef char CHAR;
#endif

#endif // _OLEDEFS_H_
