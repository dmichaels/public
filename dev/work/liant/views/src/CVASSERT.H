// $Header:   Q:/views/win/vcs/cvassert.h_v   1.7   Mar 04 1997 16:32:08   gregm  $ 

//	cvassert.h
//
//	cv_assert interface
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
//	12/05/95 dgm	Added #include "defs.h" for CV_DLL_LINKAGE definition.
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// The cv_assert() macro can be used check (assert) certain run-time
// conditions and to notify the user when the assertion is not true.
//
// If the CV_ASSERT macro is not defined, then the cv_assert()
// macro will be ignored.
//
// The notification can be made either to the standard error output
// or to a popped-up modal dialog based on whether or not the macros
// CV_ASSERT_STDERR or CV_ASSERT_DIALOG is defined.  The default behavior
// depends on the system; currently, under every platform but UNIX, the
// notification will be to a popped-up modal dialog under; under UNIX,
// the notification will be by default to the standard error output.
// --------------------------------------------------------------------------

#ifndef VASSERT_H
#define VASSERT_H

#if !defined(CV_ASSERT)

#	define cv_assert(e)

#else

#	include "defs.h"

#	if defined(CV_ASSERT_STDERR)
#		define _cv_assert _cv_assert_stderr
#	elif defined(CV_ASSERT_DIALOG)
#		define _cv_assert _cv_assert_dialog
#	elif defined(UNIX)
#		define _cv_assert _cv_assert_stderr
#	else
#		define _cv_assert _cv_assert_dialog
#	endif

#	if !defined(CVP_OLD_CPP)

#		define cv_assert(e) (void)((e)||(_cv_assert(#e,__FILE__,__LINE__),0))

#	else

#		define cv_assert(e) (void)((e)||(_cv_assert("e",__FILE__,__LINE__),0))

#	endif

#if defined(CV_ASSERT_STDERR) || defined(UNIX)
	extern void _cv_assert_stderr(const char *, const char *, int);
#endif

	extern void CV_DLL_LINKAGE _cv_assert_dialog(const char *, const char *, int);

#endif

#endif // VASSERT_H
