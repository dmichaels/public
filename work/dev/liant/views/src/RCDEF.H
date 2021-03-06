// $Header$

//	rcdef.h [Common]
//
//	Macros to define .rc file resources between MS/Windows, OSF/Motif, OS/2
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
//	04/01/96 dgm	Original.
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Instructions:
//
// o In your .rc file, #include "rcdef.h", and use the defineResource() macro
//   to declare your bitmap, cursor, and/or icon resources.  For example, in
//   main.rc do the following:
//
//		#include "rcdef.h"
//		defineResource(BITMAP, RcToolBarImage,   1, rc, stdim.bmp)
//		defineResource(CURSOR, RcMoveHandCursor, 2, rc, mvhand.cur)
//		defineResource(ICON,   RcMainAppIcon,    3, rc, mainapp.ico)
//
// o In exactly one .cpp module of your application, preferably your cvmain()
//   module, #define CV_RC_DEFINE, and then #include your .rc file.  For
//   example, in main.cpp do the following:
//
//		#define CV_RC_DEFINE
//		#include "main.rc"
//
// o In each .cpp module of your application which needs to use any of the
//   resources, #include your .rc file and simply use the name given to the
//   resource when loading it.  For example, in mainvw.cpp do the following:
//
//		#include "main.rc"
//		VBitMap      *toolBarImages  = new VBitMap(RcToolBarImage);
//		VMouseCursor *moveHandCursor = new VMouseCursor(RcMoveHandCursor);
//		VIcon        *mainAppIcon    = new VIcon(RcMainAppIcon);
//
// --------------------------------------------------------------------------

#ifndef CV_RCDEF_H
#define CV_RCDEF_H

// --------------------------------------------------------------------------
// This nonsense is so that the user of defineResource() can use macros as
// arguments and get them expanded value, as woule be expected.
//
#define _CvNxStringize_(s)		#s				// does not expand arguments
#define _CvNxTokenPaste_(a,b)	a##b			// does not expand arguments

#define _CvStringize_(s)		_CvNxStringize_(s)		// expands arguments
#define _CvTokenPaste_(a,b)		_CvNxTokenPaste_(a,b)	// expands arguments

// --------------------------------------------------------------------------
// MS/Windows
//
#if defined(CV_WINDOWS) || defined(CV_WIN16) || defined(CV_WIN32)

#	if defined(CV_RC_COMPILE)

#		define	defineLibraryResource(type, name, number, dir, file) \
					name type dir\\file

#		define	defineResource(type, name, number, dir, file) \
					name type dir\\file

#	elif defined(CV_RC_LIBRARY_DEFINE)

#		include "defs.h"

#		if (defined(__BCPLUSPLUS__) || defined(__SC__)) && defined(DLL)
#			define CV_EXPORT(t, n) t FAR CV_DLL_EXPORT n
#		elif defined(CV_WIN16)
#			if defined(DLL)
#				define CV_EXPORT(t, n) t FAR CV_DLL_EXPORT n
#			else
#				define CV_EXPORT(t, n) t CV_NEARDATA n
#			endif
#		elif defined(CV_WIN32)
#			if defined(DLL)
#				define CV_EXPORT(t, n) CV_DLL_EXPORT t n
#			else
#				define CV_EXPORT(t, n) t n
#			endif
#		else
#			define CV_EXPORT(t, n) t n
#		endif

#		define	defineLibraryResource(type, name, number, dir, file) \
					CV_EXPORT(char *, name) = _CvStringize_(name);

#		define	defineResource(type, name, number, dir, file) \
					char *name = _CvStringize_(name);

#	elif defined(CV_RC_DEFINE)

#		include "defs.h"

#		if (defined(__BCPLUSPLUS__) || defined(__SC__)) && defined(DLL)
#			define CV_EXPORT(t, n) t FAR CV_DLL_EXPORT n
#		elif defined(CV_WIN16)
#			if defined(DLL)
#				define CV_EXPORT(t, n) t FAR CV_DLL_EXPORT n
#			else
#				define CV_EXPORT(t, n) t CV_NEARDATA n
#			endif
#		elif defined(CV_WIN32)
#			if defined(DLL)
#				define CV_EXPORT(t, n) CV_DLL_EXPORT t n
#			else
#				define CV_EXPORT(t, n) t n
#			endif
#		else
#			define CV_EXPORT(t, n) t n
#		endif

#		if defined(CV_WIN16)
#			if defined(DLL)
#				define CV_IMPORT(t, n) extern t _far n
#			else
#				define CV_IMPORT(t, n) extern t CV_NEARDATA n
#			endif
#		elif defined(__BCPLUSPLUS__) && defined (CV_WIN32) && defined(DLL)
#			define CV_IMPORT(t, n) extern t CV_DLL_IMPORT n
#		elif defined(_MSC_VER) && defined(CV_WIN32)
#			define CV_IMPORT(t, n) extern CV_DLL_IMPORT t n
#		else
#			define CV_IMPORT(t, n) extern t n
#		endif

#		define	defineLibraryResource(type, name, number, dir, file) \
					CV_IMPORT(char *, name);

#		define	defineResource(type, name, number, dir, file) \
					char *name = _CvStringize_(name);
#	else

#		include "defs.h"
#
#		if defined(CV_WIN16)
#			if defined(DLL)
#				define CV_IMPORT(t, n) extern t _far n
#			else
#				define CV_IMPORT(t, n) extern t CV_NEARDATA n
#			endif
#		elif defined(__BCPLUSPLUS__) && defined (CV_WIN32) && defined(DLL)
#			define CV_IMPORT(t, n) extern t CV_DLL_IMPORT n
#		elif defined(_MSC_VER) && defined(CV_WIN32)
#			define CV_IMPORT(t, n) extern CV_DLL_IMPORT t n
#		else
#			define CV_IMPORT(t, n) extern t n
#		endif

#		define	defineLibraryResource(type, name, number, dir, file) \
					CV_IMPORT(char *, name);

#		define	defineResource(type, name, number, dir, file) \
					extern char *name;
#	endif

// --------------------------------------------------------------------------
// OS/2
//
#elif defined(CV_PM) || defined(PM)

#	if defined(CV_RC_COMPILE)
#		define	defineLibraryResource(type, name, number, dir, file) \
					type number dir\file
#		define	defineResource(type, name, number, dir, file) \
					type number dir\file
#	elif defined(CV_RC_DEFINE)
#		define	defineLibraryResource(type, name, number, dir, file) \
					char *name = _CvStringize_(number);
#		define	defineResource(type, name, number, dir, file) \
					char *name = _CvStringize_(number);
#	else
#		define	defineLibraryResource(type, name, number, dir, file) \
					extern char *name;
#		define	defineResource(type, name, number, dir, file) \
					extern char *name;
#	endif


// --------------------------------------------------------------------------
// OSF/Motif (UNIX)
//
#elif defined(CV_MOTIF) || defined(MOTIF)

#	if defined(CV_RC_COMPILE)
#		define	defineLibraryResource(type, name, number, dir, file)
#		define	defineResource(type, name, number, dir, file)
#	elif defined(CV_RC_DEFINE)
#		define	defineLibraryResource(type, name, number, dir, file) \
					char *name = _CvStringize_(dir) "/" _CvStringize_(file);
#		define	defineResource(type, name, number, dir, file) \
					char *name = _CvStringize_(dir) "/" _CvStringize_(file);
#	else
#		define	defineLibraryResource(type, name, number, dir, file) \
					extern char *name;
#		define	defineResource(type, name, number, dir, file) \
					extern char *name;
#	endif

#endif

// --------------------------------------------------------------------------
//
#define cvLibraryBitmap(name, id, dir, file)	defineLibraryResource \
												(BITMAP, name, id, dir, file)
#define cvLibraryCursor(name, id, dir, file)	defineLibraryResource \
												(CURSOR, name, id, dir, file)
#define cvLibraryIcon(name, id, dir, file)		defineLibraryResource \
												(ICON, name, id, dir, file)

#define cvDefineBitmap(name, id, dir, file)		defineResource \
												(BITMAP, name, id, dir, file)
#define cvDefineCursor(name, id, dir, file)		defineResource \
												(CURSOR, name, id, dir, file)
#define cvDefineIcon(name, id, dir, file)		defineResource \
												(ICON, name, id, dir, file)

#endif // CV_RCDEF_H
