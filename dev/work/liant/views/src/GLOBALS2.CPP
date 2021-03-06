// $Header:   Q:/views/common/vcs/GLOBALS2.CPV   1.12   Mar 24 1997 16:09:20   scottt  $

//  globals2.cpp [Common]
//
//  This file contains global objects:
//
//      - dll: exported in def file
//      - static: linked in.
//
// Note that the objects in this file are for exprot outside the dll.
// This differs from globals.cpp, where those objects are not
// exported out of the dll.
//
//	Allegris Foundation 1.1.00
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
//  12/05/95 dss    Initial revision.
//  12/07/95 dss    Added oleResultReport.
//  12/08/95 dgm    Removed FAR; fixed for Motif.
//  02/19/96 evc    Made global statics near for 16 bit static case
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "result.h"


#if defined(CV_MOTIF) || (defined PM)

VResultReport       CV_DLL_EXPORT nullResultReport;
CIOResultReport     CV_DLL_EXPORT cioResultReport;
MSWIOResultReport   CV_DLL_EXPORT mswioResultReport;

#else
#if defined (CV_WIN16) && !defined (DLL)
	VResultReport       CV_NEARDATA nullResultReport;
	CIOResultReport     CV_NEARDATA cioResultReport;
	MSWIOResultReport   CV_NEARDATA mswioResultReport;
#else
	VResultReport       CV_DLL_EXPORT nullResultReport;
	CIOResultReport     CV_DLL_EXPORT cioResultReport;
	MSWIOResultReport   CV_DLL_EXPORT mswioResultReport;
#endif

#if !defined(CV_NOOLE)
    #if defined(CV_DEBUG) 
        #include "debug.h"
#if defined (CV_WIN16) && !defined (DLL)
        VDebugStream    CV_NEARDATA	   cdbg; 
#else
        VDebugStream    CV_DLL_EXPORT  cdbg; 
#endif
    #endif

    #include "olerslt.h"

	#if defined (CV_WIN16) && !defined (DLL)
		OLEResultReport     CV_NEARDATA  oleResultReport;
	#else
		OLEResultReport     CV_DLL_EXPORT oleResultReport;
	#endif
#endif

#endif
