// $Header:   Q:/views/win/vcs/defs.h_v   1.23   Mar 25 1997 18:14:28   gregm  $ 

//	defs.h
//
//	defs.h definition file
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	09/10/92 jmd	made MSC and BC ifdefs use >= instead of ==
//	04/09/93 pmcm	changed id to VObject *
//	08/09/93 pmcm	preened; added constPointer, constLongptr, constIntptr
//	08/12/93 pmcm	added CPPV_HUGE
//	09/02/93 pmcm	added NO_AUTO_EXPORT for use under IN_DLL BORLAND regarding
//					class _export/huge and defined it for DLL builds
//	10/10/93 jmd	SC6 changes.
//	11/11/93 pmcm 	added M80 definition
//	02/04/94 pmcm	added ARCHIVER definition and CV_NOARCHIVER off switch
//	03/05/94 pmcm	removed CPPV_HUGE ifdef
//	03/05/94 pmcm	ifdef'd away far keyword in typedefs for CV_WIN32
//	03/24/94 pmcm	auto defined CV_WINDOWS under CV_WIN32
//	03/09/94 pmcm	CV_WIN32/STRICT: updated handle typedef
//	04/04/94 pmcm	CV_WIN32: added DLL support under m80
//	04/25/94 pmcm	made lstrncmp() extern "C++", not "C"
//	04/25/94 pmcm	CV_WIN32: exported lstrncmp() under DLL
//	07/13/94 jld	moved define for CV_NEARDATA here
//	07/14/94 jld	removed Windows vers2 stuff
//	08/23/94 jld	changed typedef boolean to match Microsoft
//	08/31/94 dss	changed typedef boolean back to int.
//	11/04/94 dss	let handle definition default to HANDLE. This covers
//						STRICT and WIN32. define STRICT, here.
//	12/01/94 jld	changed define for CV_DLL_MODIFIER for Borland with Win32
//	02/13/95 jld	added define for WIN32 for MSVC
//	02/13/95 jld	masked MS typedef of boolean when loading <windows.h>
//					to avoid conflict
//	09/01/95 dss	VStyle changes. Added CV_DLL_LINKAGE.
//	09/20/95 dss	Added CV_DLL_LINKAGE, CV_DLL_IMPORT for borland and symantec
//	10/30/95 dss	Define OLE2ANSI before including windows.h.
//	11/09/95 dss	take out the OLE2ANSI definition.
//	11/15/95 dgm	#if defined(CV_NOOLE) then #define CV_NOOLEARCHIVER.
//					#if defined(__BCPLUSPLUS__) && !defined(CV_NOOLE)
//					then #define INC_OLE.
//	11/16/95 dss	don't re-define CV_NOOLEARCHIVER if it is defined.
//	12/15/95 dss	CV_DLL_IMPORT is defined only for USE_DLL on Borland 32 bit.
//  02/20/96 evc    Undefed FAR for 16 bit static libraries
//  02/20/96 evc    Undefed FAR for ll but 16 bit IN_DLL borland case.
//	02/21/96 evc	took out "NEVER_EVER"
//  02/21/96 evc    No need to undef FAR
//	07/24/96 glm	Added CV_DLL_MODIFIER __import for Borland USE_DLL
//	01/19/97 dgm	Drag-and-Drop support.
//	--------------------------------------------------------------------------

#ifndef VDEFS_H
#define VDEFS_H

#ifndef NOSTRICT
#	ifndef STRICT
#		define STRICT
#	endif
#endif

// For backward compatability
#ifdef WIN31
#	define CV_WIN16
#endif

#if !defined(CV_WINDOWS) && (defined(CV_WIN16) || defined(CV_WIN32))
#	define CV_WINDOWS
#endif

#if defined(IN_DLL) || defined(USE_DLL)
#	define DLL
#endif

#if defined(CV_NOOLE)
#	if !defined(CV_NOOLEARCHIVER)
#		define CV_NOOLEARCHIVER
#	endif
#elif defined(__BCPLUSPLUS__) && !defined(INC_OLE2)
#	define INC_OLE2
#endif

#if defined(CV_OLE_DRAG_DROP) && defined(__BCPLUSPLUS__) && !defined(INC_OLE2)
#	define INC_OLE2
#endif

//	--------------------------------------------------------------------------
//	Microsoft C/C++ specific
//	--------------------------------------------------------------------------

#if defined(_MSC_VER)

#	if defined(CV_WIN32)

#		ifndef WIN32
#			define WIN32
#		endif

#		if defined(IN_DLL) || defined(USE_DLL)
#			define CV_DLL_EXPORT __declspec( dllexport )
#			define CV_DLL_IMPORT __declspec( dllimport )
#		else
#			define CV_DLL_EXPORT
#			define CV_DLL_IMPORT
#		endif

// Used when the declaration is used in the dll and by the dll client.
#		if defined(IN_DLL)
#			define CV_DLL_LINKAGE __declspec( dllexport )
#		elif defined(USE_DLL)
#			define CV_DLL_LINKAGE __declspec( dllimport)
#		else 
#			define CV_DLL_LINKAGE
#		endif

#		if defined(DLL) && defined(CV_IN_NATIVE_DLL)
#			define CLASS class __declspec(dllexport)
#		endif

#	else
#		define CV_DLL_EXPORT __export
#		define CV_DLL_IMPORT
#		if defined(IN_DLL)
# 			define CV_DLL_MODIFIER __export
#		endif
#		define CV_DLL_LINKAGE
#	endif

#endif

//	--------------------------------------------------------------------------
//	Borland C++ specific
//	--------------------------------------------------------------------------

#if defined(__BCPLUSPLUS__)

#	define CV_DLL_EXPORT __export
#	if defined(CV_WIN32) && defined(USE_DLL)
#		define CV_DLL_IMPORT __import
#	else
#		define CV_DLL_IMPORT
#	endif
#	define CV_DLL_LINKAGE

#	if defined(IN_DLL)
#		if defined(NO_AUTO_EXPORT)
#			define CV_DLL_MODIFIER huge
#		else
#			define CV_DLL_MODIFIER _export
#		endif
#	elif defined(USE_DLL)
#		if !defined(CV_WIN32)
#  			define CV_DLL_MODIFIER huge
#		else
#			define CV_DLL_MODIFIER __import
#		endif
#	endif
 
#	if defined(DLL)
#		define CLASS class CV_DLL_MODIFIER
#	endif

#endif 	// Borland C++

//	--------------------------------------------------------------------------
//	Symantec C++ specific
//	--------------------------------------------------------------------------

#if defined(__SC__)

#	define CV_DLL_EXPORT _export
#	define CV_DLL_IMPORT
#	define CV_DLL_LINKAGE

#	if defined(IN_DLL)
#		if defined(NO_AUTO_EXPORT)
#			define CV_DLL_MODIFIER huge
#		else
#			define CV_DLL_MODIFIER _export
#		endif
#	elif defined(USE_DLL)
#		if !defined(CV_WIN32)
#  			define CV_DLL_MODIFIER huge
#		endif
#	endif
 
#	if defined(DLL)
// #		define CLASS class CV_DLL_MODIFIER
#		define CLASS class
#	endif
 
#endif 	// Symantec C++

//	--------------------------------------------------------------------------
//	
//	--------------------------------------------------------------------------

#if !defined(CLASS)
#	define CLASS class
#endif

#if !defined(CV_DLL_MODIFIER)
#	define CV_DLL_MODIFIER
#endif

#if defined(DLL) && !defined(CV_DLL_DECL)
#	define CV_DLL_DECL
#endif

CLASS VObject;

#define FREESELF delete this; return TRUE;

#ifdef __BCPLUSPLUS__
#pragma warn -inl // functions containing 'ident' not expanded inline
#pragma warn -hid // 'function' hides virtual function 'function2'
#pragma warn -pia // possible incorrect assignment
#pragma warn -ccc // condition always true/false
#pragma warn -aus // 'ident' assigned a value which is never used
#pragma warn -par // parameter 'ident' is never used
#pragma warn -rch // unreachable code
#endif

typedef long       	   *longptr;

//	this typedef is for backward compatibility
//	VObject * 	is preferred over id
//	VObject ** 	is preferred over idptr
//
typedef VObject *id;
typedef VObject    	 **idptr;

extern "C" {

#	include <io.h>
#	undef NULL
#if ATT
#	undef min
#	undef max
#endif

}

#ifdef null
#	undef null
#endif
#define null 0

#include <stdlib.h>

#ifdef CV_WINDOWS
#	undef MSDOS
#	define MSDOS
#endif

#ifdef MSDOS
#	define LINE_TERM "\r\n"
#else
#	define LINE_TERM "\n"
#endif

#if __BCPLUSPLUS__ >= 0x0300 || _MSC_VER >= 700 || defined(__SC__)
#	define CVP_NEWINCR
#endif

//	--------------------------------------------------------------------------
//	CV_WINDOWS specific
//	--------------------------------------------------------------------------

#ifdef CV_WINDOWS

// Avoid conflict with Microsoft typedef of boolean by defining over it 
// while loading the MS headers.
//
#define boolean MS_boolean
#include "windows.h"
#undef boolean

extern "C" {

typedef		HANDLE		handle;

#	if !defined(CV_WIN32)
		typedef 		char 	far    *pointer;	
		typedef 		long 	far    *longptr;
		typedef 		int  	far    *intptr;

		typedef const 	char 	far	   *constPointer;
		typedef const 	long 	far    *constLongptr;
		typedef const 	int  	far    *constIntptr;
#	else
		typedef 		char 		   *pointer;	
		typedef 		long 	       *longptr;
		typedef 		int  	       *intptr;

		typedef const 	char 		   *constPointer;
		typedef const 	long 	       *constLongptr;
		typedef const 	int  	       *constIntptr;
#	endif

}	//	end of extern "C" for CV_WINDOWS

#	if defined(CV_WIN32)
#		if defined(DLL)
			extern CV_DLL_DECL int lstrncmp(char *, char *, int); 	
#		else
			int lstrncmp(char *, char *, int); 	
#		endif
#	else
			int lstrncmp(char far *, char far *, int); 	
#	endif

#	if defined(CV_WIN32)
#		define CV_NEARDATA
#	else
#		define CV_NEARDATA		_near
#	endif


//	end of CV_WINDOWS section

#else 

//	--------------------------------------------------------------------------
//	non-CV_WINDOWS section
//	--------------------------------------------------------------------------

	typedef char   	   *handle;

	typedef char   	   *pointer;
	typedef long   	   *longptr;
	typedef int    	   *intptr;

	typedef const char *constPointer;
	typedef const long *constLongptr;
	typedef const int  *constIntptr;

#	define HANDLE short

#	define CV_NEARDATA		_near

#endif	//	end of non-CV_WINDOWS section

//	generic defs

#if defined(DLL)
extern CV_DLL_DECL void print(char *...); 
#else
extern void print(char *...); 
#endif

typedef long 				integer;
typedef int					boolean;
typedef unsigned short 		ushort;
typedef unsigned long 		ulong;

#define TRUE				1
#define FALSE				0
#define MAX_CLASSES			100
#define INCR_CLASSES		50
#define NIL  				0
#define NIL_ID 				((VObject *) 0)

enum Iterate { StartIterator, EndIterator, NextIterator, PrevIterator };


#endif // VDEFS_H
