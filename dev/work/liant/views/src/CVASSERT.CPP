// $Header:   Q:/views/win/vcs/cvassert.cpv   1.6   Mar 04 1997 17:54:52   gregm  $ 

//	cvassert.cpp
//
//	cv_assert implementation
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
//	07/14/94 dgm	Original.
//	07/18/94 ***	3.0.1 release for OSF/Motif (RS/6000 AIX).
//	10/20/94 dss	include GUI headers for CV_WINDOWS, only.
//	02/09/95 jld	added CV_DLL_EXPORT to fit platform based export
//					declarations
// ----------------------------------------------------------------------------

// --------------------------------------------------------------------------
//
#include <stdio.h>
#include "defs.h"

#if defined (CV_WINDOWS)
#	include "object.h"
#	include "notifier.h"
#endif

// --------------------------------------------------------------------------
//
#define CV_ASSERT_STRING "CvAssertion Failed: %s [%s, %d]"

#if defined(CV_ASSERT_STDERR) || defined(UNIX)
// --------------------------------------------------------------------------
//
void _cv_assert_stderr(const char *expr, const char *file, int line)
{
	fprintf(stderr, CV_ASSERT_STRING "\n", expr, file, line);
}
#endif

// --------------------------------------------------------------------------
//
void CV_DLL_LINKAGE _cv_assert_dialog(const char *expr, const char *file, int line)
{
	print(CV_ASSERT_STRING, expr, file, line);
}
