// $Header:   Q:/views/common/vcs/VWCHAR.H_V   1.10   Mar 24 1997 16:14:34   scottt  $

//	vwchar.h
//
//	Interface to substitute wchar.h functions [Common]
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
//	Revision History:
//	-----------------
//	02/14/95 pkt	originated.
//	03/17/95 pkt	added 'iswspace'.
//	04/04/95 evc	added iswalpha
//	04/04/95 evc	added iswpunct and  iswcntrl
//	04/11/95 pkt	updated for VWideChar.
//	04/11/95 pkt	added VWideCharAux.
//	04/12/95 pkt	worked around 'iswxxx' macros.
//	04/14/95 pkt	added 'setupByteTypeTable' for Mac.
//	04/21/95 pkt	added CV_MB_LEN_MAX and CV_MB_CUR_MAX.
//	04/21/95 pkt	moved definition here, from str.h, of CV_WCHAR,
//					SINGLE_BYTE_CHAR_SET, and CV_MB_HAS_STATE.
//	04/21/95 pkt	disabled special setting of SINGLE_BYTE_CHAR_SET;
//					no longer in test mode.
//	08/17/95 pkt	added standard-conforming version of 'wcstok'.
//	08/30/95 pkt	added 'swscanf', 'strcoll', 'tolower', 'toupper',
//					'towlower', 'towupper', 'wcstol', 'wcstoul', and 'wcstod'.
//	08/31/95 pkt	added 'checkLocale'.
//	09/05/95 pkt	made VWideCharAux::strcoll call ::mbccoll.
//	09/06/95 pkt	updated Mac discriminator to V_MACINTOSH.
//	09/11/95 pkt	added #include of <ctype.h>.
//	09/18/95 pkt	added 'mbslen' functions.
//	10/03/95 pkt	picked up 'wctoa' and 'atowc' from VString.
//	11/29/95 tlf	updated for HPUX.
//	12/04/96 pkt	changed to compile on SunOS.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VWCHAR_H
#define VWCHAR_H

#include "defs.h"

// CV_WCHAR is defined if VWideChar is distinct from char.
#define CV_WCHAR

#if defined(V_MACINTOSH) || defined(PM) || defined(CV_WIN16)  
  // The standard C library does not support locale-dependent string behavior.
# define CV_SYNTHESIZE_WCHAR
#endif

#if defined(CPPV_HPUX)
#define CV_WCHAR_CONST(const) 	(VWideChar)const
#else
#define CV_WCHAR_CONST(const)  L ## const
#endif

#include <stdlib.h> // for wchar_t
#ifdef WCHAR_H_EXISTS
# include <wchar.h> // for wint_t
#endif

// Define typedef name VWideChar (duplicate of str.h).
#ifndef CV_SYNTHESIZE_WCHAR
// Use the wchar_t supplied by the compiler/OS.
typedef wchar_t VWideChar;
# define CV_MB_CUR_MAX (VWideCharAux::checkLocale(), MB_CUR_MAX)
# define CV_MB_LEN_MAX MB_LEN_MAX
#else
// Avoid the wchar_t supplied by the compiler.
# if defined(V_MACINTOSH)  
#  define CV_OWN_VWIDECHAR
typedef unsigned short VWideChar;
# else
// The wchar_t supplied by the OS is OK, though the implementation is not.
typedef wchar_t VWideChar;
# endif
# define CV_MB_CUR_MAX (SINGLE_BYTE_CHAR_SET ? 1 : 2)
# define CV_MB_LEN_MAX 2
#endif
#if defined(WCHAR_H_EXISTS) && !defined(CV_OWN_VWIDECHAR)
typedef wint_t VWideInt;
#else
typedef signed long VWideInt;
#endif

#define CV_SELECT_TCHAR (TCHAR(0))
#if 0
#define     SINGLE_BYTE_CHAR_SET (0)     // exercise the changes
#elif !defined(CV_SYNTHESIZE_WCHAR)
# if !defined(CV_SUNOS)
#  define    SINGLE_BYTE_CHAR_SET (CV_MB_CUR_MAX == 1)
# else
#  define    SINGLE_BYTE_CHAR_SET (1)
# endif 
#else
# define    SINGLE_BYTE_CHAR_SET (VWideCharAux::isSingleByteCharSet())
#endif  
#define		CV_MB_HAS_STATE (VWideCharAux::wctomb(0, 0) != 0)

// ---------------------------------------------------------------------------
// Some needed functions which should be in wchar.h,
// but which are not provided on all systems. 
// vwchar.h and vwchar.cpp supply implementations.
//
class VWideCharAux {
public:
	static VWideChar   *wcscpy(VWideChar *to_in, const VWideChar *from_in);
	static size_t		wcslen(const VWideChar *p);
	static int			wcscmp(const VWideChar *s1, const VWideChar *s2);
	static int			wcsncmp(const VWideChar *s1, const VWideChar *s2, size_t n);
	static int			wcscoll(const VWideChar *s1, const VWideChar *s2);
	static VWideChar   *wcschr(const VWideChar *str, VWideChar c);
	static long			wcstol(const VWideChar *nptr, VWideChar **endptr,
							   int base);
	static unsigned long wcstoul(const VWideChar *nptr, VWideChar **endptr,
								int base);
	static double		wcstod(const VWideChar *nptr, VWideChar **endptr);
	static int 			(iswalpha)(VWideChar c);
	static int 			(iswalnum)(VWideChar c);
	static int			(iswdigit)(VWideChar digit);
	static int			(iswspace)(VWideChar sp);
	static VWideChar   *wcstok(VWideChar *s, const VWideChar *delims);
	static VWideChar   *wcstok(VWideChar *s, const VWideChar *delims, VWideChar **buffer_p);
	static int			(iswpunct)(VWideChar wc);  
	static int			(iswcntrl)(VWideChar wc);
	
	static int			(mblen)(const char *s, size_t n);
	static int			mbtowc(VWideChar *pwc, const char *s, size_t n);
	static int			wctomb(char *s, VWideChar wchar);
	static size_t		mbstowcs(VWideChar *pwcs, const char *s, size_t n);
	static size_t		wcstombs(char *s, const VWideChar *pwcs, size_t n);
	static int 			swscanf(const  VWideChar *, const  VWideChar *, ...);

	static size_t		mbslen(const char *mbstring);
	static size_t		mbslen(const char *mbstring, size_t n);
#define CV_HAS_SWSCANF

	// other locale-dependent string functions
	static int			strcoll(const char *s1, const char *s2);
	static int			(tolower)(int c);
	static int			(toupper)(int c);
	static VWideInt		towlower(VWideInt wc);
	static VWideInt		towupper(VWideInt wc);
	static boolean		atowc(VWideChar& wc, char c);
	static boolean		wctoa(char& c, const VWideChar& wc);
	
#ifdef CV_SYNTHESIZE_WCHAR
# if defined(PM) || defined(V_MACINTOSH)
	static void			setupByteTypeTable();
# endif
	static boolean		isSingleByteCharSet();
#else
	static void			checkLocale();
#endif
};

#ifndef CV_SYNTHESIZE_WCHAR

# include <stdlib.h>
# include <string.h>  
# include <ctype.h>

// Forward VWideCharAux's functions to the standard library functions.

// ---------------------------------------------------------------------------
// Stand-in for standard mblen.
//
inline int (VWideCharAux::mblen)(const char *s, size_t n)
{
	checkLocale();
	return (::mblen)(s, n);
}

// ---------------------------------------------------------------------------
// Stand-in for standard mbtowc.
//
inline int VWideCharAux::mbtowc(VWideChar *pwc, const char *s, size_t n)
{
	checkLocale();
	return ::mbtowc(pwc, s, n);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wctomb.
//
inline int VWideCharAux::wctomb(char *s, VWideChar wchar)
{
	checkLocale();
	return ::wctomb(s, wchar);
}

// ---------------------------------------------------------------------------
// Stand-in for standard mbstowcs.
//
inline size_t VWideCharAux::mbstowcs(VWideChar *pwcs, const char *s, size_t n)
{
	checkLocale();
	return ::mbstowcs(pwcs, s, n);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcstombs.
//
inline size_t VWideCharAux::wcstombs(char *s, const VWideChar *pwcs, size_t n)
{
	checkLocale();
	return ::wcstombs(s, pwcs, n);
}

#ifdef WCHAR_H_EXISTS
#if 0  
//  the multibyte functions should live in this file instead of stdlib.h
# include <wchar.h>
#endif          
//	but MSVC has them in stdlib.h at the moment

// If wchar.h exists then forward VWideCharAux's functions to the standard
// library functions.

// ---------------------------------------------------------------------------
// Stand-in for standard wcscpy.
//
inline VWideChar *VWideCharAux::wcscpy(VWideChar *to_in, const VWideChar *from_in)
{
	return ::wcscpy(to_in, from_in);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcslen.
//
inline size_t VWideCharAux::wcslen(const VWideChar *p)
{
	return ::wcslen(p);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcscmp.
//
inline int VWideCharAux::wcscmp(const VWideChar *s1, const VWideChar *s2)
{
	return ::wcscmp(s1, s2);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcsncmp.
//
inline int VWideCharAux::wcsncmp(const VWideChar *s1, const VWideChar *s2, size_t n)
{
	return ::wcsncmp(s1, s2, n);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcscoll.
//
inline int VWideCharAux::wcscoll(const VWideChar *s1, const VWideChar *s2)
{
	checkLocale();
	return ::wcscoll(s1, s2);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcschr.
//
inline VWideChar *VWideCharAux::wcschr(const VWideChar *str, VWideChar c)
{
	return ::wcschr(str, c);
}

// ---------------------------------------------------------------------------
// Stand-in for standard iswalpha.
//
inline int (VWideCharAux::iswalpha)(VWideChar c)
{
	checkLocale();
	return (::iswalpha)(c);
}

// ---------------------------------------------------------------------------
// Stand-in for standard iswalnum.
//
inline int (VWideCharAux::iswalnum)(VWideChar c)
{
	checkLocale();
	return (::iswalnum)(c);
}

// ---------------------------------------------------------------------------
// Stand-in for standard iswdigit.
//
inline int (VWideCharAux::iswdigit)(VWideChar digit)
{
	checkLocale();
	return (::iswdigit)(digit);
}

// ---------------------------------------------------------------------------
// Stand-in for standard iswspace.
//
inline int (VWideCharAux::iswspace)(VWideChar sp)
{
	checkLocale();
	return (::iswspace)(sp);
}

// ---------------------------------------------------------------------------
// Stand-in for MS-specific wcstok.
//
inline VWideChar *VWideCharAux::wcstok(VWideChar *s, const VWideChar *delims)
{
#ifdef MSDOS
	checkLocale();
	return ::wcstok(s, delims);
#else
	static VWideChar *buffer;
	return wcstok(s, delims, &buffer);
#endif
}

#ifndef MSDOS
// ---------------------------------------------------------------------------
// Stand-in for standard wcstok.
//
inline VWideChar *VWideCharAux::wcstok(VWideChar *s, const VWideChar *delims,
									   VWideChar **buffer_p)
{
	checkLocale();
	return ::wcstok(s, delims, buffer_p);
}
#endif

// ---------------------------------------------------------------------------
// Stand-in for standard iswpunct.
//
inline int (VWideCharAux::iswpunct)(VWideChar wc)
{
	checkLocale();
	return (::iswpunct)(wc);
}

// ---------------------------------------------------------------------------
// Stand-in for standard iswcntrl.
//
inline int (VWideCharAux::iswcntrl)(VWideChar wc)
{
	checkLocale();
	return (::iswcntrl)(wc);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcstol.
//
inline long VWideCharAux::wcstol(const VWideChar *nptr,
								 VWideChar **endptr,
								 int base)
{
	return ::wcstol(nptr, endptr, base);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcstoul.
//
inline unsigned long VWideCharAux::wcstoul(const VWideChar *nptr,
										   VWideChar **endptr,
										   int base)
{
	return ::wcstoul(nptr, endptr, base);
}

// ---------------------------------------------------------------------------
// Stand-in for standard wcstod.
//
inline double VWideCharAux::wcstod(const VWideChar *nptr,
								   VWideChar **endptr)
{
	return ::wcstod(nptr, endptr);
}

// ---------------------------------------------------------------------------
// Stand-in for standard towlower.
//
inline VWideInt VWideCharAux::towlower(VWideInt wc)
{
	checkLocale();
	return ::towlower(wc);
}

// ---------------------------------------------------------------------------
// Stand-in for standard towupper.
//
inline VWideInt VWideCharAux::towupper(VWideInt wc)
{
	checkLocale();
	return ::towupper(wc);
}

#endif /* WCHAR_H_EXISTS */

// ---------------------------------------------------------------------------
// Stand-in for standard strcoll.
//
inline int VWideCharAux::strcoll(const char *s1, const char *s2)
{
	checkLocale();
#ifdef MBCHAR_H_EXISTS
	// This must be better than strcoll, right?
	return ::mbccoll(s1, s2);
#else
	return ::strcoll(s1, s2);
#endif
}

// ---------------------------------------------------------------------------
// Stand-in for standard tolower.
//
inline int (VWideCharAux::tolower)(int c)
{
	return (::tolower)(c);
}

// ---------------------------------------------------------------------------
// Stand-in for standard toupper.
//
inline int (VWideCharAux::toupper)(int c)
{
	return (::toupper)(c);
}

#endif /* not synthesizing wide character functions */

#endif /* VWCHAR_H */

