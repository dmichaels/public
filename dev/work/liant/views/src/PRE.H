// $Header:   Q:/views/win/vcs/PRE.H_v   1.8   Mar 04 1997 14:13:38   gregm  $

// Used for precompiled headers
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
//	11/09/95 dss	take out the OLE2ANSI definition.
//	01/09/96 dss	VComponent name change.
//	01/30/96 dss	volestd.h in qutoes (not angle brackets) so it will be
//					found in CWD.
//	02/06/96 dss	Moved a whole lotta code to oledefs.h.
//	---------------------------------------------------------------------------

#ifndef PRE_H
#define PRE_H

#if !defined( STRICT)
	#define STRICT
#endif

#if defined(WIN31) && !defined(CV_WIN16)
	#define CV_WIN16
#endif

#include <stddef.h>
#include <string.h>

#define USE_CUNKNOWN

// Views Components Types
#include "oledefs.h"

// 49E572C0-EE61-101B-A32D-080017088174
DEFINE_GUID(IID_ViewsObject, 0x49E572C0L, 0xEE61, 0x101B, 0xA3, 0x2D, 0x08, 0x00, 0x17, 0x08, 0x81, 0x74);

// Views GUI
#include "object.h"

#include "archiver.h"
#include "file.h"
#include "cvassert.h"
#include "menuitem.h"
#include "mdiappvw.h"
#include "mdiview.h"
#include "notifier.h"
#include "pathstr.h"
#include "report.h"
#include "result.h"
#include "rscarch.h"
#include "statusbr.h"
#include "stack.h"
#include "stdmdiap.h"
#include "view.h"
#include "window.h"
#include "yesno.h"

// Views Primitive Types
#include "opoint.h"
#include "winrect.h"

// Less Primitive but depends on debug
#include "debug.h"
#include "olerslt.h"

// Views OLE
#include "unknown.h"		// most primitive OLE class
#include "owindow.h"
#include "lockbyte.h"
#include "oipsite.h"
#include "oipuiwin.h"
#include "oipframe.h"
#include "oipappvw.h"
#include "oclsite.h"
#include "oobject.h"
#include "advsink.h"
#include "droptarg.h"
#include "dropsrc.h"
#include "xferagnt.h"
#include "rstorag.h"
#include "rootstrg.h"		// depends on rstorag
#include "substrg.h"        // depends on rstorag
#include "storag.h"

// Component Includes
#include "cmpstrg.h"
#include "cmpobj.h"
#include "cmpwin.h"
#include "cmpview.h"
#include "cmpappvw.h"

#include "itemwin.h"
#include "cmpitem.h"

#include "voleui.h"

#endif
