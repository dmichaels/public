// $Header:   Q:/views/common/vcs/PATHSTR.CPV   1.28   Mar 24 1997 16:12:50   scottt  $

//	pathstr.cpp
//
//	VPathString implementation
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
//	09/09/92 jmd	made WIN31 auto define MS_WINDOWS
//	09/10/92 jmd	made MSC ifdefs use >= instead of ==
//	10/20/92 jmd	removed inline from addName, fixed ifdefs in findNext/First
//	05/11/93 pkt	reinstated derivation from VString.
//	05/13/93 pkt	fixed getPath, to avoid stack overflow.
//	06/21/93 pkt	fixed find, to ignore resolved directory.
//	07/08/93 pkt	fixed access.
//	07/31/93 pkt	eliminated #ifdef macintosh.
//	07/31/93 jmd	added consts
//	08/14/93 jmd	made the remaining MSC ifdef use >= instead of ==
//	09/01/93 dgm	M8 changes.
//	09/26/93 jmd	preened, added test to nameOf
//	10/10/93 jmd	SC6 changes.
//	10/12/93 pat	updated notifier header guard for not Windows
//	10/12/93 pat	asParent() no longer duplicates drive specification;
//					returns FALSE for path string whose directory is a root.
//	10/12/93 pat	buildPath() now works correctly when it cannot get default
//					file
//	10/12/93 pat	expand() now works correctly with drive specification.
//	10/12/93 pat	bumped capacity of buffer for ProfileString from 300 to 600.
//	10/18/93 dgm	UNIX changes (from atb).
//	10/20/92 jmd	removed inline from addName, fixed ifdefs in findNext/First
//	11/02/93 pkt	changed puts for const argument & integer return.
//	11/12/93 pkt	removed call to free().
//	11/12/93 pkt	changed id to VObject *.
//	11/12/93 pkt	removed self.
//	11/12/93 pat	moved local variable in directoryOf() into ifdef where used.
//	11/12/93 pat	declared local helper functions static.
//	11/14/93 pkt	made string parameter of getSearchPath const.
//	11/14/93 pkt	made setCreator etc. const member functions.
//	11/14/93 pkt	made operator char* a const member function.
//	12/10/93 dgm	added declaration of rename() (for SPARC).
//	12/27/93 pkt	changed for Symantec compiler.
//	01/06/94 pkt	fixed infinite loops.
//	01/17/94 pkt	preened NULL.
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	merged with Macintosh!
//	01/17/94 dgm	cfront C++ changes
//	01/26/94 dgm	modified VPathString::file() to handle pathnames
//					starting with a slash (under UNIX).
//	02/10/94 jmd	made buffer dynamically allocate space in find and do_list
//	02/24/94 dgm	old C preprocessor changes
//	03/24/94 pat	auto defined CV_WINDOWS under CV_WIN32
//	03/24/94 pat	quick&dirty change to okDrive and selectDrive for CV_WIN32
//	03/24/94 pat	CV_WIN32 changes:  added to findFirst, findNext, findDone.
//	04/10/94 pat	fixed interpretation of _access() return value
//	04/19/94 pat	fixed findFirst() under CV_WIN32.
//	07/07/94 jmd	SC changes
//	07/14/94 jld	added dependency on defs.h to define CV_WINDOWS
//	08/22/94 dgm	Minor changes for SGI port.
//	08/31/94 dgm	Minor UnixWare/MetaWare-C++ related changes.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	09/15/94 dgm	Minor OSF-1/DEC-C++ changes.
//	09/22/94 dgm	Initialize/check findBuf::fblock appropriately.
//	10/20/94 dgm	Minor Linux/GNU-C++ changes.
//	11/02/94 jmd	removed optional derivation from VObject
//	01/20/95 evc    MBCS support
//	01/27/95 pkt	MBCS improvements
//	02/17/95 evc    added GetFileAttributes (MBCS safe) to isDirectory & isFile
//	03/09/95 evc	fixed multibyte case of 'dolist'.
//	03/09/95 pkt	defined 'VPathString::VPathString(const VString&)'.
//	04/11/95 pkt	updated for VWideChar.
//	04/11/95 pkt	updated for VWideCharAux.
//	04/12/95 pkt	removed #define of SINGLE_BYTE_CHAR_SET.
//	04/18/95 pkt	added functions with VString parameters, e.g. 'dotOf'.
//	04/19/95 pkt	fixed 'getPath' to concatenate 'unresolved' correctly.
//	02/16/95 jld	Win32 versions of isFile() and isDirectory() for 3.0.6
//	03/30/95 jld	renamed environ() environVar() to avoid conflict
// 	05/30/95 pkt	merged internationalization with 3.0.6.
//	06/14/95 pkt	fixed merge bug in 'dolist'.
//	07/18/95 dgm	Miscellaneous const char * changes.
//	08/04/95 dgm	Added support to duplicate() to optionally preserve
//					the modification time of the source file;
//					see the setFileModifyTime() helper.
//	08/09/95 dgm	Fixed minor goofs from previous rev.
//	08/15/95 pkt	removed extra #include "pathstr.h" which sneaked in.
//	08/15/95 pkt	fixed 'isFile' and 'isDirectory' not to call
//					GetFileAttributesW which fails under Windows 95.
//	08/15/95 pkt	fixed to pick up <sys/utime.h> instead of <utime.h>
//					in case of MSVC.
//	08/17/95 pkt	removed file-scope wcstok function.
//	08/17/95 pkt	changed _wstat to avoid tripping up if WCHAR_H_EXISTS
//					doesn't get defined.
//	09/05/95 dgm	Added first cut at createDirectory()/removeDirectory().
//	09/06/95 pkt	removed OFI artifacts.
//	09/11/95 pkt	added wide character defines for Unix.
//	09/11/95 pkt	updated to compile on Unix.
//	10/30/95 dss	made VResult compatible on Windows.
//	11/01/95 dgm	Removed inline from function containing loops
//					to suppress compiler warnings.
// 	11/07/95 tlf	Updated for Motif.
//	11/08/95 pkt	fixed 'createDirectory' to return the correct string when
//					creating a directory at the root.
//	11/29/95 tlf	updated for HPUX.
//	12/11/95 dgm	Added stdlib.h/errno.h includes for Borland.
//	05/20/96 pkt	fixed wide character code in 'expand' to not crash.
//	07/11/96 pkt	fixed 'dolist' to work reliably in double-byte locales.
//	11/06/96 pkt	revised to work with no notifier.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"

#if defined(CV_WINDOWS) && !defined(CV_NO_NOTIFIER)
#	include "notifier.h"
#endif
#include "file.h"
#include "iterator.h"
#include "pathstr.h"

#ifdef HP_INC
#	include <stdlib.h>
#endif

#ifdef CENTERLINE
#include <sysent.h>
#endif

extern "C" {
#ifndef HP_INC
#	include <stdlib.h>
#endif

#ifdef SCO
//typedef long off_t;
//typedef short dev_t;
//typedef unsigned short ushort;
#	ifndef _SYS_TYPES_H
#	include <sys/types.h>
#	endif
#else	// SCO
#	if !defined(SUN) && !defined(V_MACINTOSH) && !defined(CPPV_AIX6000) && \
	   !defined(CPPV_HPUX) && !defined(CV_IRIX) && !defined(CV_SVR4) && \
	   !defined(__GNUC__) && !defined(__osf__) && !defined(CV_UNIXWARE)
#		ifdef __VMS
#			include <unixio.h>
#		else
#			include <io.h>
#		endif
#	endif
#endif // !SCO

#include <stdio.h>
#include <string.h>
#include <ctype.h>  
#include <limits.h>
#include <locale.h> // added for _wstat
#ifdef MSDOS
#	include <time.h>
#	include <direct.h>
#	if defined(__SC__)
#		undef MSDOS
#	endif
#	include <dos.h>
#	if defined(__SC__)
#		define MSDOS
#	endif
#	include <sys/stat.h>
#	if defined(_MSC_VER)
#		include <sys/utime.h>
#	else
#		include <utime.h>
#	endif
#else
#	include <utime.h>
#endif

#ifdef __BCPLUSPLUS__
#	include <dir.h>
#endif



#if defined(CPPV_AIX6000) || defined(CV_SVR4)
#	include <unistd.h>		/* contains prototypes of access(), chdir(), */
							/* unlink(), and getcwd() */
#	include <time.h>		/* contains prototype of time() */
#endif		// CPPV_AIX6000

#if (defined(UNIX) || defined(__VMS) || defined(__GNUC__)) && \
	!defined(CPPV_AIX6000) && !defined(CV_SVR4) && !defined(__osf__) && \
	!defined(CV_UNIXWARE)
#	if !defined(__VMS)
#		if defined(sparc) || defined(__GNUC__)
#			include <unistd.h>
#		else
#			include <sys/unistd.h>
#		endif
#	endif
#	if defined(HP) || defined(CV_UNIXWARE)
#		include <sys/dirent.h>
#		include <sys/time.h>
#	else
#		include <time.h>
#	endif // HP
#endif // UNIX

#if defined(MOTIF)
#include <errno.h>
#endif

#if defined(__osf__)
#	include <time.h>
#endif

#if _MSC_VER >= 700
#	include <sys/types.h>
#endif


#include <sys/stat.h>


#if defined(__ultrix) || defined(__VMS) || defined(GNU) || defined(sparc)
#	if !defined(__LPI_CXX__) && !defined(SUN) && !defined(SCO) && \
	   !defined(CV_IRIX) && !defined(__GNUC__) && !defined(__osf__) && \
	   !defined(CV_UNIXWARE)
		extern "C" int rename(char *, char *);
#	endif
#endif

#if defined(SCO) || defined(__VMS) || defined(SOLARIS) || \
	defined(CV_IRIX) || defined(__osf__) || defined(CV_UNIXWARE)
#if !defined(SOLARIS)
	int	access(char *, int);
	int	chdir(char *);
	int	unlink(char *);
#endif
#	if !defined(SOLARIS) && !defined(__LPI__) && \
	   !defined(CV_IRIX) && !defined(CV_UNIXWARE)
#		if !defined(__osf__)
			int		stat(char *, struct stat *);
#		endif
		char   *getcwd(char *, int);
#	endif
#	if defined(__VMS)
		int		unlink(char *) { return 0; }
#	endif
#endif

} // extern "C"

#ifdef MSDOS
#ifndef WCHAR_H_EXISTS
// not in standard library
static int local_wstat(const wchar_t * path, struct stat * buffer);
#else
# define local_wstat _wstat;
#endif      
 

#	define WCHARdrive CV_WCHAR_CONST(':')
#	define WSTRwildAll L"*.*"
#	define WcvPATH_SEP CV_WCHAR_CONST('\\')
#	define WisDelim(c) ((c) == WcvPATH_SEP || (c) == WCHARdrive)  

#	define SEMICOLON_CHAR ';' 
#	define SEMICOLON_WCHAR  CV_WCHAR_CONST(';')

#	define DELIMS ";"   // Environment filepath delimiter(s) for MSDOS

#	define _access ::access
#	define CHARdrive ':'
#	define STRwildAll "*.*"
#	define cvPATH_SEP '\\'
#	define isDelim(c) ((c) == cvPATH_SEP || (c) == CHARdrive)
#	define DELIMS ";"   // Environment filepath delimiter(s) for MSDOS      
#	define WDELIMS L";"   // double byte version
#endif // MSDOS

#ifdef __SC__
#	undef unlink
#endif

#if defined(UNIX) || defined(__VMS)
#ifndef WCHAR_H_EXISTS
// not in standard library
static int local_wstat(const wchar_t * path, struct stat * buffer);
#else
# define local_wstat _wstat;
#endif      
 

#	define WSTRwildAll L"*"
#	define WcvPATH_SEP CV_WCHAR_CONST('/')
#	define WisDelim(c) ((c) == WcvPATH_SEP)  

#	define SEMICOLON_CHAR ';' 
#	define SEMICOLON_WCHAR  CV_WCHAR_CONST (';')

#	define STRwildAll "*"
#	define cvPATH_SEP '/'
#	define isDelim(c) ((c) == cvPATH_SEP)
#	define _access ::access
#	define DELIMS ":"   // Environment filepath delimiter(s) for UNIX

#ifndef CPPV_HPUX
#	define WDELIMS L":"   // double byte version
#else
static VWideChar WDELIMS[2] =
		{ VWideChar('L'), VWideChar('\0') };
#endif

#endif

#define CHARdot '.'
#define CHARstar '*'
#define STRstar "*"

#define WCHARdot CV_WCHAR_CONST('.')
#define WCHARstar CV_WCHAR_CONST('*')
#define WSTRstar L"*"


#define S_IEXIST 0

// ---------------------------------------------------------------------------
//

#if !defined(UNIX)
#	if defined(CV_WIN32)
		typedef struct {
			WIN32_FIND_DATA fileData;
			HANDLE			hSearch;
		} findBuf;
#	else
#		ifdef __BCPLUSPLUS__
			typedef	struct ffblk findBuf;
#		endif
#		if defined(__SC__) || _MSC_VER >= 700 || defined(__HIGHC__)
			typedef	struct find_t findBuf;
#		endif
#	endif
#endif

// ---------------------------------------------------------------------------
//	Prototypes of local helper functions.
//
#ifdef MSDOS
	static boolean isBase(char *s);
	static boolean okDrive(char c);  
	
	static void    selectDrive(char c);
#endif

static boolean isWild(char *s);
static boolean findNext(findBuf& fblk, char *name);
static void    findDone(findBuf &fblk);
static void    copyFile(VFile *from, VFile *to, integer size);
static boolean setFileModifyTime(VFile *target, const VFile *source = 0);

static boolean findFirst(findBuf& fblk, char *filter, char *name);


// ---------------------------------------------------------------------------
//
static void dummyUse(void *)
{
}

defineClass(VPathString, VString)


// Construct a VPathString with a null name.
//
VPathString::VPathString()
{
#if 0
OutputDebugString("in VPathStr - locale is " );
	char * x = 
setlocale( LC_ALL, 0 );
OutputDebugString(x);
OutputDebugString("\n");
#endif
	initialize(); 
}

// ---------------------------------------------------------------------------
// Construct a VPathString object with the name s.
//
VPathString::VPathString(const char *s)
{
#if 0
OutputDebugString("in VPathStr - locale is ");
char * x = 
setlocale( LC_ALL, 0 );
OutputDebugString(x);
OutputDebugString("\n");
#endif
	initialize();
	if (s) {
		puts(s);
	}
}


// ---------------------------------------------------------------------------
// Construct a VPathString object with the name s.
//
VPathString::VPathString(const VString& s)
{
	initialize();
	puts(s.gets());
}

// ---------------------------------------------------------------------------
// Construct a VPathString object from VPathString object p.
//
VPathString::VPathString(const VPathString& p)
{
#if 0
OutputDebugString("in VPathStr - locale is ");
char * x = 
setlocale( LC_ALL, 0 );
OutputDebugString(x);
OutputDebugString("\n");
#endif
	initialize();
	*this = p;
}

// ---------------------------------------------------------------------------
//
static char *tail(char *p)
{
	if (!*p) {
		return p;
	}
	while (*p) {
		p++;
	}
	return p - 1;
} 

#ifdef CV_WCHAR
static VWideChar *tail(VWideChar *p)
{
	if (!*p) {
		return p;
	}
	while (*p) {
		p++;
	}
	return p - 1;
}
#endif

// ---------------------------------------------------------------------------
// Add a path separator character (if one is not already there) and the
// name name to the end of this VPathString.
//
VPathString &VPathString::addName(const char *name) 

{ 
	if SINGLE_BYTE_CHAR_SET { 
		char * p = (char *)  this->gets();
	
		p = tail(p);
		if (*p != cvPATH_SEP) {
			*this += cvPATH_SEP;
		}
		*this += name;
		return *this;  
	} else {  // MBCS safe 
		VWideChar * Wp = (VWideChar *) this->getwcs(); 
		Wp = tail(Wp);  
#if 1
		if (*Wp != WcvPATH_SEP) {
			*this += WcvPATH_SEP;
		}
		*this += name;
		return *this;  
#else
		if (*Wp != WcvPATH_SEP) {
			//*this += WcvPATH_SEP;
			VWideChar * temp;
			temp = (VWideChar*) this->getwcs();
			VWideCharAux::wcscat(	temp, L"\\");
			VString Name(name);
			VWideCharAux::wcscat(temp, Name.getwcs() );
			this->puts(temp);
			return *this;
		}


#endif
		*this += name;
		return *this;  
	}	
} 

// ---------------------------------------------------------------------------
//
VPathString &VPathString::addName(const VString& name) {  
	return addName(name.gets());
}

#ifdef MSDOS
// ---------------------------------------------------------------------------
// Return TRUE if s is of the form <drive>:\.
// This function is unique to MS DOS and OS/2 platforms
// and it may not be portable.
//
static boolean isBase(char*s)
{  
// s could be multi-character

	if (!*s || !s[1] || !s[2]) {
		return FALSE;
	} 
if SINGLE_BYTE_CHAR_SET { 
	if (s[1] == CHARdrive && s[2] == cvPATH_SEP && !s[3]) {
		return TRUE;
	}
	return FALSE;
} else { 
	VString tempstr(s);
	const VWideChar *wide_str = tempstr.getwcs(); 
	if (wide_str[1] == WCHARdrive && 
			wide_str[2] == WcvPATH_SEP && 
			!wide_str[3]) {
		return TRUE;
	}
	return FALSE;
}

	

}
#endif // MSDOS

// ---------------------------------------------------------------------------
// Return TRUE if s contains a '*' character.
//
static boolean isWild(char *s)
{    
	if SINGLE_BYTE_CHAR_SET  {
		while (*s) {
			if (*s == CHARstar) {
				return TRUE;
			}
		else {
				s++;
			}
		}
		return FALSE; 
	} else { 
		VString tempstr(s);
		const VWideChar *wide_str = tempstr.getwcs(); 
		while (*wide_str) {
			if (*wide_str == WCHARstar){
				return TRUE;
			}else {
				wide_str++;
			}
		}
		return FALSE; 
	}	
}

#ifdef MSDOS

// ---------------------------------------------------------------------------
// Return TRUE if c is a valid drive.
// This function is unique to MS DOS and OS/2 platforms and it may not be portable.
//
static boolean okDrive(char c)
{ 
// drive letters are always single byte
#if defined(CV_WIN32)
	DWORD logicalDrivesBitField = GetLogicalDrives();
	DWORD driveMask = ((DWORD) 1) << c - 'A';
	return (logicalDrivesBitField & driveMask) ? TRUE : FALSE;
#else
	union REGS regs;
	struct SREGS sregs;
	regs.h.ah = 0x1C;
	regs.h.dl = c - 'A'+1;
	segread(&sregs);
	intdosx(&regs, &regs, &sregs);
	if (regs.h.al == 0xFF) {
		return FALSE;
	}
#endif
	return TRUE;
}

// ---------------------------------------------------------------------------
// Changes the current drive to c.
// This function is unique to MS DOS and OS/2 platforms
// and it may not be portable.
//
static void selectDrive(char c)
{
#if defined(CV_WIN32)
	char s[3];
	s[0] = c;
	s[1] = ':';
	s[2] = '\0';
	SetCurrentDirectory(s);
#else
	union REGS regs;
	struct SREGS sregs;
	regs.h.ah = 0x0E;
	regs.h.dl = c - 'A';
	segread(&sregs);
	intdosx(&regs, &regs, &sregs);
#endif
}

#endif // MSDOS


// ---------------------------------------------------------------------------
// PRIVATE: Build path from its constituent parts
//
void VPathString::buildPath(	VString&	path,
								const char *drive,
								const char *directory,
								const char *name)
{
	VString t;
	VString temp;

	//	first comes the drive specification

#ifdef MSDOS
	if (drive == DEFAULT) {
		driveOf(temp);
	}
	else if (drive) {
		//temp += drive; 
		temp = drive ;     // temp assigned a drive letter
		temp += CHARdrive; // add the drive delimitor (:)
	}
#else
	dummyUse(&path);
	dummyUse(&drive);
#endif

	//	then the base directory specification

	if (directory == DEFAULT) {
#ifdef MSDOS
		baseOf(t);
#else
		directoryOf(t);
#endif
		temp += t;
	}
	else if (directory) {
		temp += directory;
	}

	if SINGLE_BYTE_CHAR_SET  {	
		//	put a path separator before appending the file name
	
		char *p = tail((char *) temp.gets());
	
		VString defName;
		boolean haveDefName = (name == DEFAULT && nameOf(defName)) ? TRUE :  FALSE;
	
		if (haveDefName || (name != 0 && name != DEFAULT)) {
			if (*p && (*p != cvPATH_SEP)) {
				temp += cvPATH_SEP;
			}
	
			if (haveDefName) {
				temp += defName;
			}
			else {
				temp += name;
			}
		}
	
		path = temp;
	} else { 
		//	put a path separator before appending the file name
	
		VWideChar *p = tail((VWideChar *) temp.getwcs());
	
		VString defName;
		boolean haveDefName = (name == DEFAULT && nameOf(defName)) ? TRUE :  FALSE;
	
		if (haveDefName || (name != 0 && name != DEFAULT)) {
			if (*p && (*p != WcvPATH_SEP)) {
				temp += WcvPATH_SEP;
			}
	
			if (haveDefName) {
				temp += defName;
			}
			else {
				temp += name;
			}
		}
		path = temp;
	}	
}


// ---------------------------------------------------------------------------
// PRIVATE: Return the value of the system environment variable f in s.

boolean VPathString::environVar(const char *f, VString &s)
{
	char *p;
	if (!f) {
		return FALSE;
	}

#if defined(CV_WINDOWS) && !defined(CV_NO_NOTIFIER)
	VPathString iniFileName;
	int n;

	s.expandTo(600);
	//
	// Search the notifier->CTwindow() global string for the ini filename.
	// notifier->CTwindow() will be of the form "iniFileName,applicationName"
	// or "applicationName". If notifier->CTwindow() is of the form
	// "applicationName", the the .ini file is assumed to be win.ini
	//
	// Setup p to point to notifier->CTwindow():
	//

	p = notifier->CTwindow();
	while ((*p != '\0') && (*p != ',')) {
		iniFileName += *p++;
	}

	if (*p == '\0') {
		//	CTwindow has the form "applicationName", use win.ini
		if (!(n = GetProfileString(notifier->CTwindow(), f, (LPSTR)"",
			(char*) s.gets(), 600))) {

			return FALSE;
		}
	}
	else {
		//	CTwindow has the form "iniFileName,applicationName", 
		//	use iniFileName.ini

		if (iniFileName.dotOf() == (char *)0) {
			iniFileName += ".ini";
		}
		p++;
		if (!(n = GetPrivateProfileString(p, f, (LPSTR)"", ( char*)s.gets(), 300, (char *)iniFileName.gets()))) {
			return FALSE;
		}
	}
	s.expandTo(n);

#else
	if ((p = getenv(f)) == 0) {
		return FALSE;
	}
	s = p;
#endif

	return TRUE;
}

// ---------------------------------------------------------------------------
// PRIVATE: Return the value of the system environment variable f in s.
//
boolean VPathString::environVar(const VString& f, VString& s)
{
	return environVar(f.gets(), s);
}

// ---------------------------------------------------------------------------
// PRIVATE: Return the dot (dot plus suffix) portion of this VPathString.
//
char *VPathString::dotOf()
{ 
	if SINGLE_BYTE_CHAR_SET {   
		char *p = (char *)gets();
		if (!*p) {
			return 0;
		}
		char *q = tail(p);
		while (q > p && *q != CHARdot) {
			if (isDelim(*q)) {
				return 0;
			}
			q--;
		}
		if (p == q) {
			return 0;
		}
		if (q[-1] == CHARdot) {
			return 0;
		}
		return q;    
	} else {   // MBCS safe  
	// scan forward till we get to the last dot.  Then scan forward or
	// a path separator ('\').  If no '\' then the dot
	// is in the filename and we return its position.  All
	// scanning is MBCS safe.

	    char * last_dot_pos = (char *) -1;
	    const char *p = gets();
		VWideCharAux::mbtowc(0, 0, MB_LEN_MAX); // Initialize the multibyte state.
		for (;;) {  
			VWideChar wideone;
			int ch_size = VWideCharAux::mbtowc(&wideone, p, MB_LEN_MAX);
			if (ch_size <= 0) {
				break;
			} 
			if (wideone == WCHARdot) { 
				last_dot_pos = (char *) p;
			}
			p += ch_size;
		}  
		
	    if ( last_dot_pos == (char *)-1){
			return 0; // not found  
	    } else { 
	    	// at this point we might not be inside the file name. 
	    	// (consider \e.d\i.s\filename)
	    	// But we would be if we encounter no '\'. 
	    	p = last_dot_pos;
		    for (;;) {  
				VWideChar wideone;
				int ch_size = VWideCharAux::mbtowc(&wideone, p, MB_LEN_MAX);
				if (ch_size <= 0) {
					break;
				} 
				if (wideone == cvPATH_SEP) { 
					return 0;
				}
				p += ch_size;
			}  
	    	return   last_dot_pos;
	    } 
	}
}

// ---------------------------------------------------------------------------
// PRIVATE: Return the dot (dot plus suffix) portion of this VPathString.
//
boolean VPathString::dotOf(VString& suffix) const
{
	const char *cp = ((VPathString *)this)->dotOf();
	boolean result = (cp != 0);
	if (result) {
		suffix = cp;
	}
	return result;
}

// ---------------------------------------------------------------------------
// Copy the directory portion of this VPathString into s.  Return
// TRUE if successful, otherwise return FALSE.
//
boolean VPathString::directoryOf(VString &s)
{
	VString d, dir;
	if (isDirectory()) {
 		s = *this;
	} else {

#  if defined(MSDOS)
		driveOf(d);
		baseOf(dir);
		s = d + dir;
#  elif defined(UNIX) || defined(__VMS)
		char p[MAX_STRING + 1];
		strcpy(p, (char *)this->gets());
#    if defined(__ultrix) || defined(__VMS)
		char *tmp = (char *) this->gets();
		strcpy(p, tmp);
#    else
		strcpy(p, this->gets());
#    endif 
 
// unix code   
if SINGLE_BYTE_CHAR_SET {   
		char *q = tail(p);
		while (q > p && *q != cvPATH_SEP) {
			q--;
		}
		if (q == p) {
			if (*q == cvPATH_SEP) {
				q++;
			}
			else {
				return FALSE;
			}
		}
		*q = 0;
		s = p; 
}else {// MBCS safe  
		VString tempstr (p);
	    VWideChar *Wp =(VWideChar *) tempstr.getwcs(); 
		VWideChar *Wq = tail(Wp);
		while (Wq > Wp && *Wq != WcvPATH_SEP) {
			Wq--;
		}
		if (Wq == Wp) {
			if (*Wq == WcvPATH_SEP) {
				Wq++;
			}
			else {
				return FALSE;
			}
		}
		*Wq = 0;
		s = Wp; 
}			
#endif 

	}
	return TRUE;
}

#if defined(MSDOS) 

// ---------------------------------------------------------------------------
// Copy the drive part of this VPathString into VString object s.
// Return FALSE if there isn't a drive specified, otherwise return TRUE.
// This function is unique to MS DOS, OS/2, and Macintosh platforms
// and it may not be portable.
//
boolean VPathString::driveOf(VString &s)
{
	VString temp(*this); 
	if SINGLE_BYTE_CHAR_SET { 	
		char *p = (char *) temp.gets();
	
		if (!*p) {
			return FALSE;
		}
		if (p[1] == CHARdrive) {
			p[2] = 0;
			s = temp;
			return TRUE;
		}
		return FALSE;   
	} else {  
		VWideChar * wide_str = (VWideChar *) temp.getwcs(); 
		if (!*wide_str) {
			return FALSE;
		}
		if (wide_str[1] == WCHARdrive) {
			wide_str[2] = 0;
			s = temp;
			return TRUE;
		}
		return FALSE;   
	}	
}

// ---------------------------------------------------------------------------
// Return the directory path (without the drive specification)
// part of this VPathString in VString object s and return TRUE.
// Return FALSE if the directory path is not set.
// This function is unique to MS DOS and OS/2 platforms
// and it may not be portable.
//
boolean VPathString::baseOf(VString &s)
{

	char temp[MAX_PATH], *p = temp;
	VWideChar Wtemp[MAX_PATH], *Wp = Wtemp;
 
if SINGLE_BYTE_CHAR_SET {  
//	char temp[MAX_PATH], *p = temp;
	gets(temp, MAX_PATH);
	if (!*p) {
		return FALSE;
	}
	if (p[1] == CHARdrive) {
		p += 2;
	} 
} else {   
//	VWideChar temp[MAX_PATH], *p = temp;
	gets(Wtemp, MAX_PATH); // gets() is overloaded for VWideChar
	if (!*Wp) {
		return FALSE;
	}
	if (Wp[1] == WCHARdrive) {
		Wp += 2;
	} 

}	

	if SINGLE_BYTE_CHAR_SET {  
		if (!isDirectory()) {
			char *q = tail(p);
			while (q > p && *q != cvPATH_SEP) {
				q--;
			}
			if (q == p) {
				if (*q == cvPATH_SEP) {   // returning "\"
					q++;
				}
				else {
					return FALSE;  // was a filename
				}
			}
			*q = 0;
		}
		s = p;
		return TRUE;  
	} else {
		if (!isDirectory()) {
				VWideChar *Wq = tail(Wp);
				while (Wq > Wp && *Wq != WcvPATH_SEP) {
					Wq--;
				}
				if (Wq == Wp) {
					if (*Wq == WcvPATH_SEP) {   // returning "\"
						Wq++;
					}
					else {
						return FALSE;  // was a filename
					}
				}
				*Wq = CV_WCHAR_CONST('\0');
			}
			s = Wp;
			return TRUE;    
	}	
}

#endif // MSDOS

// ---------------------------------------------------------------------------
// Return the suffix part of this VPathString in VString object s.
// Return TRUE if there is one, otherwise return FALSE.  The suffix part of
// a VPathString is any part of the filename following the dot "." character.
//
boolean VPathString::suffixOf(VString &s)
{

	char *p = dotOf(); 
 	if (p) {  
		if SINGLE_BYTE_CHAR_SET {
			s= ++p;
		} else { 
			VWideCharAux::mbtowc(0, 0, MB_LEN_MAX); // Initialize the multibyte state.
			VWideChar wideone;
			int ch_size = VWideCharAux::mbtowc(&wideone, p, MB_LEN_MAX);
			p += ch_size;
			s = p;    
		}	
		return TRUE;
	}
	s = "";
	return FALSE;
}

// ---------------------------------------------------------------------------
// Return the prefix part of this VPathString's name (or simply the
// name if no dot character is found) into VString object s.  Return TRUE
// if the name exists, otherwise return FALSE.
//
boolean VPathString::prefixOf(VString &s)
{
	VString n;

	if (!nameOf(n)) {
		return FALSE;
	}
#if 0 && (defined(__ultrix) || defined(__VMS))
	char   buf[MAX_STRING + 1], 
	*p;
	char *tmp = n;
	strcpy(buf, tmp);
//#else
//	strcpy(buf, n);
#endif 

#if 0   //pre-MBCS
	for (p = buf; *p != '\0'; p++) {
		if (*p == CHARdot) {
			*p = '\0';
			break;
		}
	}
	s = buf;   
#else 
	VWideChar * name = (VWideChar *)  n.getwcs();   
	while (*name != WCHARdot && *name != 0) { 
		name ++;
	}
	if (*name == WCHARdot) {
		*name = CV_WCHAR_CONST('\0') ;
	}  
	s = n;
#endif
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return TRUE if the name portion of this VPathString matches
// the named portion of the passed wildcarded VPathString p.  The only
// wildcard currently supported is the * character as a pathname prefix or
// a pathname suffix.  A null name or name suffix is equivalent to a * or
// a prefix.* pathname, respectively.
//
boolean VPathString::isMatch(VPathString &p)
{
	// Compare prefixes
	VString this_pre;
	VString p_pre;
	prefixOf(this_pre);
	if (!p.prefixOf(p_pre)) {
		p_pre = STRstar;
	} 
	// these comparisons are MBCS safe
	if ((p_pre != (const char *)STRstar) && p_pre != this_pre) {
		return FALSE;
	}
	else {
		// Compare suffixes
		VString this_suf;
		VString p_suf;
		suffixOf(this_suf);
		if (!p.suffixOf(p_suf)) {
			p_suf = STRstar;
		}

		return (p_suf == (const char *)STRstar) || (p_suf == this_suf);
	}
}

// ---------------------------------------------------------------------------
// Set the suffix part of this VPathString to equal the Null terminated
// C string s.  Replace the previous suffix of this VPathString or add a
// dot "." plus the suffix s if a suffix does not currently exist.  If s is
// NULL, remove any suffix from this VPathString.
//
void VPathString::suffix(const char *s)
{
 	char *p = dotOf();
	if (p) {
		*p = 0;
	}
	if (s) {
		*this += CHARdot;
		*this += s;
	}
 }

// ---------------------------------------------------------------------------
// Copy the file name part of this VPathString into VString object
// s.  Return FALSE if there is no file name part of this VPathString,
// otherwise return TRUE.
//
boolean VPathString::nameOf(VString &s)
{
	VString temp(*this);
	if SINGLE_BYTE_CHAR_SET  {	
		char *p = (char *) temp.gets();
		if (!*p) {
			return FALSE;
		}
		char *q = tail(p);
		while (q > p && !isDelim(*q)) {
			q--;
		}
		if (p != q || isDelim(*q)) {		// jmd 9/26/93
			q++;
		}
		s = q;
		return TRUE;
	} else {       // MBCS safe
		 VWideChar *p = (VWideChar *) temp.getwcs();  
		if (!*p) {
			return FALSE;
		}
		VWideChar *q = tail(p);
		while (q > p && !WisDelim(*q)) {
			q--;
		}
		if (p != q || WisDelim(*q)) {		// jmd 9/26/93
			q++;
		}
		s = q;
		return TRUE;
	}	
}

// ---------------------------------------------------------------------------
// Return TRUE if this VPathString specifies a file, otherwise return FALSE.
//
boolean VPathString::isFile() const
{ 
#if defined(CV_WIN32)

	if (isWild((char *)gets())) {
		return FALSE;
	}
	DWORD value = GetFileAttributes(gets());

	if(	value == (DWORD) -1 ) {
		return FALSE;
	}
	if( value & FILE_ATTRIBUTE_DIRECTORY ) {
		return FALSE;
	}
	else
		return TRUE;

#else
	if (isWild((char *)gets())) {
		return FALSE;
	}
	struct stat s;

	if (stat(gets(), &s)) {
		return FALSE;
	}
	return (s.st_mode & S_IFREG) ? TRUE : FALSE;

#endif
}

// ---------------------------------------------------------------------------
// Return TRUE if this VPathString specifies a directory,
// otherwise return FALSE.
//
boolean VPathString::isDirectory() const
{   
	if (isWild((char *)gets())) {
		return FALSE;
	}

#if  defined(CV_WIN32) 
 
	DWORD value = GetFileAttributes(gets());
	DWORD err_value = GetLastError();

	if(	value == (DWORD) -1 ) {
		return FALSE;
	}
	if( value & FILE_ATTRIBUTE_DIRECTORY ) {
		return TRUE;
	}
	else {
		return FALSE;
	}	
#else
	if (!SINGLE_BYTE_CHAR_SET) {
		// wstat, a wide character version of stat,
		//  will need to be implemented for MBCS code
		// page machines.
		struct stat s;
		if (local_wstat(getwcs(), &s)) {
			return FALSE;
		}
		return (s.st_mode & S_IFDIR) ? TRUE : FALSE;
	}
	else {
		struct stat s;
		if (stat(gets(), &s)) {
			return FALSE;
		}
		return (s.st_mode & S_IFDIR) ? TRUE : FALSE;
	}   			
#endif // CV_WIN32	
}

// ---------------------------------------------------------------------------
// Find the next name matching the VPathString 'filter'.
// Copy the name to the character array pointed to by 'name'.
// 'fblk' retains the state of this iteration between calls to findNext.
//
static boolean findNext(findBuf &fblk, char *name)
{

#if defined(UNIX) || defined(__VMS)

	VPathString	path;
	struct dirent *dir_ent;

	if (fblk.fblock == 0) {
		return FALSE;
	}

	do {
		if ((dir_ent = readdir(fblk.fblock)) == 0) {
			closedir(fblk.fblock);
			fblk.fblock = 0;
			return FALSE;
		}
		path = fblk.dir;
		path.addName(dir_ent->d_name);
	} while (!path.isMatch(fblk.name) || path.isDirectory());

	strcpy (name, dir_ent->d_name);
	return TRUE;


#elif defined(CV_WIN32) 

	if (FindNextFile(fblk.hSearch, &(fblk.fileData))) {
		strcpy(name, fblk.fileData.cFileName);
		return TRUE;
	}
	return FALSE;
	
#else // !(UNIX or VMS)

# ifdef __BCPLUSPLUS__
	if (findnext(&fblk)) {
		return FALSE;
	}
	strcpy(name, fblk.ff_name);
	return TRUE;
# else
	if (_dos_findnext(&fblk)) {
		return FALSE;
	}
	else {
		strcpy(name, fblk.name);
		return TRUE;
	}
# endif
#endif // !(UNIX or VMS)
}

// ---------------------------------------------------------------------------
// Find the first name matching the VPathString 'filter'.
// Copy the name to the character array pointed to by 'name'.
// Set up 'fblk' so that subsequent calls to findNext will find more matches.
//

static boolean findFirst(findBuf &fblk, char *filter, char *name)
{
#if defined(CV_WIN32)
	fblk.hSearch = FindFirstFile(filter, &(fblk.fileData));
	if (fblk.hSearch != INVALID_HANDLE_VALUE) {
		name[MAX_FILE] = '\0'; 
		strncpy(name, fblk.fileData.cFileName, MAX_FILE);
		return TRUE;
	}
	*name = '\0';
	return FALSE;
#endif

#ifdef UNIX
	VPathString	filter_p = filter;

	filter_p.directoryOf(fblk.dir);
	filter_p.nameOf(fblk.name);
	if ((fblk.fblock = opendir(fblk.dir)) != 0) {
		return findNext(fblk, name);
	}
	return FALSE;
#endif	//	UNIX

#if !defined(UNIX) && !defined(CV_WIN32)
#	ifdef __BCPLUSPLUS__
 		if (findfirst(filter, &fblk, FA_DIREC)) {
 			return FALSE;
 		}
 		strcpy(name, fblk.ff_name);
 		return TRUE;
#	else
 		if (_dos_findfirst(filter, _A_SUBDIR, &fblk)) {
 			return FALSE;
 		}
 		else {
 			strcpy(name, fblk.name);
 			return TRUE;
 		}
#	endif
#endif	//	!UNIX && !CV_WIN32
}


// ---------------------------------------------------------------------------
//
static void findDone(findBuf& fblk)
{
#if defined(UNIX) || defined(__VMS)
	if (fblk.fblock != 0) {
		closedir(fblk.fblock);
		fblk.fblock = 0;
	}
#elif defined(CV_WIN32)
	FindClose(fblk.hSearch);
#else
	// Avoid warnings.
	dummyUse(&fblk);
#endif
}

// ---------------------------------------------------------------------------
// PRIVATE: Body of a directory search
//	
// Place in collect all names matching this VPathString from the directories
// specified by the environment variable env (if defined).  If cwd is TRUE or
// env is null, then check the current directory.  If full is TRUE then
// the names are represented with VPathString objects, otherwise VString.
//
boolean VPathString::dolist(boolean full, VOrdCollect &collect,
							const char *env, boolean cwd)
{
	VString envstr(env != 0 ? env : "");
	VString &this_str = (VString &)(*this);
	return dolist(full, collect, env != 0 ? envstr : this_str /* trick value */, 
						         cwd || (env == 0));
}

// ---------------------------------------------------------------------------
// PRIVATE: Body of a directory search
//	
// Place in collect all names matching this VPathString from the directories
// specified by the environment variable env (if defined).  If cwd is TRUE or
// env is null, then check the current directory.  If full is TRUE then
// the names are represented with VPathString objects, otherwise VString.
//
// Note: If &env == this, that's a trick value to indicate absence
// of environment.
//
boolean VPathString::dolist(boolean full, VOrdCollect &collect,
							const VString& env, boolean cwd)
{
	VPathString	temp;
	VPathString	*ps;

	VPathString	prefix;			// diretory prefix to append filename to

	findBuf		fb;
#if defined(UNIX) || defined(__VMS)
	fb.fblock = 0;
#endif
	char		name[MAX_FILE + 1];
	char *		path; 
	VWideChar  *Wpath;


	VPathString	path_list;	// list of all paths including . and directoryOf()


	if (cwd) {	// Add current working directory
 		path_list.asCurrent();
	}

	if ((&env != this) && environVar(env, temp)) {
		path_list += DELIMS;
		path_list += temp;
	}


#ifdef MSDOS
	path_list.toLower();
#endif
if SINGLE_BYTE_CHAR_SET { 
	// jmd 02/10/94
	char *buffer = new char[path_list.size() + 1];
	if (buffer == 0) {
		return FALSE;
	}

	strcpy(buffer, path_list.gets());
	if ((path = strtok(buffer, DELIMS)) != 0) {
		do {
			temp = path;

			temp.addName(*this);
			if (findFirst(fb, (char *) temp.gets(), name)) {

				do {
#ifdef MSDOS
					strlwr(name);
#endif
					if (full) {
						ps = new VPathString(path);
						ps->addName(name);
						collect.add(ps);
					}
					else {
						collect.add(new VString(name));
					}
				} while (findNext(fb, name));
			}
			findDone(fb);
		} while ((path = strtok(0, DELIMS)) != 0);
	}

	delete[] buffer;
} else { // MBCS safe
	VWideChar *buffer = new VWideChar[path_list.getCharSize() + 1];
	if (buffer == 0) {
		return FALSE;
	}

	path_list.gets(buffer, path_list.getCharSize());
	if (Wpath = VWideCharAux::wcstok(buffer, WDELIMS)) {
		do {
			temp = Wpath;

			temp.addName(*this);
			if (findFirst(fb, (char *) temp.gets(), name)) {

				do {
#ifdef MSDOS         
					// need a wide vesion of this  
					VString ts(name);
					ts.toLower();  
					strcpy(name, (char *) ts.gets()); 
					//strlwr(name);
#endif
					if (full) {
						ps = new VPathString(Wpath);
						ps->addName(name);
						collect.add(ps);
					}
					else {
						collect.add(new VString(name));
					}
				} while (findNext(fb, name));
			}
			findDone(fb);
		} while (Wpath = VWideCharAux::wcstok(0, WDELIMS));
	}

	delete[] buffer;

}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Place in VOrdCollect 'collect' all VPathString objects matching this
// VPathString from the directories in the environment variable 'env' (if
// defined). If the current-working-directory flag cwd is TRUE or the
// environment variable 'env' is null, check only the current directory.  Before
// calling this function this VPathString should be set to the filename that is
// to be matched.  Wildcarded strings of the form "*.h", "filename.*", and "*.*"
// are supported.  (Full regular expression wildcarding is not supported at this
// time.)  If this VPathString is equal to a Null string "", then "*.*" will
// be used for matching by default.
//
boolean VPathString::list(VOrdCollect &collect, const char *env, boolean cwd)
{
	return dolist(TRUE, collect, env, cwd);
}

// ---------------------------------------------------------------------------
// Analogous to the above.
//
boolean VPathString::list(VOrdCollect &collect, const VString& env, boolean cwd)
{
	return dolist(TRUE, collect, (char *)env.gets(), cwd);
}

// ---------------------------------------------------------------------------
// Place in VOrdCollect collect the names (as VString objects) of all
// files matching this VPathString from the directories in the environment
// variable env (if defined). If the current-working-directory flag cwd is
// TRUE or the environment variable env is NIL, check only the current
// directory.  Before calling this function this VPathString should be set to
// the filename that is to be matched.  Wildcarded strings of the form "*.h",
// "filename.*", and "*.*" are supported.  (Full regular expression wildcarding
// is not supported at this time.)  If this VPathString is equal to a Null
// string "", then "*.*" will be used for matching by default.
//
boolean VPathString::listNames(VOrdCollect &collect,
							   const char *env, boolean cwd)
{
	return dolist(FALSE, collect, env, cwd);
}

// ---------------------------------------------------------------------------
// Analogous to the above.
//
boolean VPathString::listNames(VOrdCollect &collect, const VString& env, boolean cwd)
{
	return dolist(FALSE, collect, (char *)env.gets(), cwd);
}

// ---------------------------------------------------------------------------
// Set this VPathString to the first file matching this VPathString
// from the directories in the environment variable 'env' (if defined).  If the
// current-working-directory flag 'cwd' is TRUE or the environment variable name
// 'env' is null, check only the current directory.  Before calling this function
// this VPathString should be set to the filename that is to be matched.
// Wildcarded strings of the form "*.h", "filename.*", and "*.*" are supported.
// (Full regular expression wildcarding is not supported at this time.)  If this
// VPathString is equal to a Null string "", then "*.*" will be used for
// matching by default.
//
boolean VPathString::find(const char *env, boolean cwd)
{
		char         name[MAX_FILE + 1];
		findBuf		fb;
#if defined(UNIX) || defined(__VMS)
		fb.fblock = 0;
#endif
		char *       path;  
	
		VWideChar * Wpath;// wide character path
		VPathString path_list;
	
		VPathString   temp;
		VString myName(*this);
	
	
		int wild = isWild((char *) this->gets());
	
#ifdef UNIX
		//
		// If this path-string begins with a slash, then first just look
		// for it there, if not found, then try use the file-name part
		// and go through the normal procedure.
		//
		if (!!myName && (myName[0] == cvPATH_SEP)) {
			if (wild) {
				path = "";
				temp = path;
				temp.addName(myName);
				if (findFirst(fb, (char *)temp.gets(), name)) {
					temp.addName(name);
					*this = temp;
					findDone(fb);
					return TRUE;
				}
			}
			else if (this->exists()) {
				return TRUE;
			}
			this->nameOf(myName);
		}
#endif
	
		if (cwd) {             // Add current working directory
			path_list.asCurrent();
	
		}
		if (env && environVar(env, temp)) { // Add environ var to path
			path_list += DELIMS;
			path_list += temp;
		}
	
#ifdef MSDOS
		if (wild) {
			path_list.toLower();
			myName.toLower();
		}
#endif
	if SINGLE_BYTE_CHAR_SET { 
		
		char *buffer = new char[path_list.size() + 1];
		if (buffer == 0) {
			return FALSE;
		}
	
		strcpy(buffer, path_list.gets());
	  	path = strtok(buffer, DELIMS);
		while (path) {
			temp = path;
			if (wild) {
				temp.addName(myName);
				if (findFirst(fb, (char *)temp.gets(), name)) {
	
					temp.addName(name);
					*this = temp;
#ifdef MSDOS
					(*this).toLower();
#endif
					findDone(fb);
	
					delete[] buffer;
					return TRUE;
				} 
			}else {
				temp.addName(*this);
	
				if (temp.exists()) {
					*this = temp;
#ifdef MSDOS
					(*this).toLower();
#endif
					delete[] buffer;
					return TRUE;
				}
			}
			path = strtok(0, DELIMS); 
		}
	
		delete[] buffer; 
	
		return FALSE;  
	} else{ 
	//MBCS safe  
		// jmd 02/10/94
		VWideChar *buffer = new VWideChar [path_list.size() + 1];
		if (buffer == 0) {
			return FALSE;
		}
	    
	    VWideChar * X= (VWideChar *) path_list.getwcs();
	    int i;
	    for (i=0; i<=path_list.size(); i ++)  buffer[i] = X[i];
	    
		//strcpy(buffer, path_list.getwcs());
	    
	   
		Wpath = VWideCharAux::wcstok(buffer, WDELIMS);
		while (Wpath) {
			temp = Wpath;
			if (wild) {
				temp.addName(myName);
				if (findFirst(fb,(char *) temp.gets(), name)) {
	
					temp.addName(name);
					*this = temp;
	#ifdef MSDOS
					(*this).toLower();
	#endif
					findDone(fb);
	
					delete[] buffer;
				
					return TRUE;
				} 
			}
			else {
				temp.addName(*this);
	
				if (temp.exists()) {
					*this = temp;
#ifdef MSDOS
					(*this).toLower();
#endif
					delete[] buffer;
					
					return TRUE;
				}
			}
			Wpath = VWideCharAux::wcstok(0, WDELIMS); 
		}  // while Wpath
	
		delete[] buffer; 
	
		return FALSE; 
	
}	
}

//---------------------------------------------------------------------- 
// overloaded find that takes a VString reference
//
boolean VPathString::find(const VString& env, boolean cwd) {
	return find(env.gets(), cwd);
}

// ---------------------------------------------------------------------------
// Expand this VPathString to a full path using the current
// working directory.  Do nothing if this VPathString already contains
// a full path name.
//
void VPathString::expand()
{

	VPathString cwd;   // Current working directory
	VString     dir;   // Directory of this
	char       *s;    // Temp char pointer   
	VWideChar * Ws; 
	
	char 	curr[MAX_PATH]; 
	VWideChar  Wcurr[MAX_PATH];

#ifdef MSDOS
	if (baseOf(dir)) {
#else
	if (directoryOf(dir)) {
#endif  
		// should work for MBCS too.
		s = (char *) dir.gets();
		if (*s == cvPATH_SEP) {
			return;
		}
	}
	cwd.asCurrent();
	
	if SINGLE_BYTE_CHAR_SET {   
		s = (char *)cwd.gets();
		s = tail(s);
		if (*s != cvPATH_SEP) {
			cwd += cvPATH_SEP;
		}
		s =(char *) gets();
		cwd.gets(curr, MAX_PATH);
		char *p;
		for (p = curr; *p; p++){ ; }
		while (*s) {
			if (*s == cvPATH_SEP && s[1] == cvPATH_SEP) {
				s++;
				continue;
			}
			if (*s == CHARdot && s[1] == cvPATH_SEP) {
				s += 2;
				continue;
			}
			if (*s == CHARdot && s[1] == CHARdot) {
				s += 2;
				if (*s == cvPATH_SEP) {
					s++;
				}
				if (p > curr && p[-1] == cvPATH_SEP) {
					p--;
				}
				do p--; while (p > curr && !isDelim(p[-1]));
				continue;
			}
			*p++ = *s++;
		}
		*p = 0;
		*this = curr;
	} else {// MBCS  safe   
		Ws = (VWideChar *)cwd.getwcs();
		Ws = tail(Ws);
		if (*Ws != WcvPATH_SEP) {
			cwd += WcvPATH_SEP;
		}
		Ws =(VWideChar *) getwcs();
		cwd.gets(Wcurr, MAX_PATH);
		VWideChar *Wp;
		for (Wp = Wcurr; *Wp; Wp++){ ; }
		while (*Ws) {
			if (*Ws == WcvPATH_SEP && Ws[1] == WcvPATH_SEP) {
				Ws++;
				continue;
			}
			if (*Ws == WCHARdot && Ws[1] == WcvPATH_SEP) {
				Ws += 2;
				continue;
			}
			if (*Ws == WCHARdot && Ws[1] == WCHARdot) {
				Ws += 2;
				if (*Ws == WcvPATH_SEP) {
					Ws++;
				}
				if (Wp > Wcurr && Wp[-1] == WcvPATH_SEP) {
					Wp--;
				}
				do Wp--; while (Wp > Wcurr && !WisDelim(Wp[-1]));
				continue;
			}
			*Wp++ = *Ws++;
		}
		*Wp = 0;
		*this = VString(Wcurr);
	}// end OK with MBCS
}

// ---------------------------------------------------------------------------
// Check the file named in this VPathString to determine if it exists,
// and whether it can be read, written, or executed.  Return TRUE if the passed
// enum accessMode (one of accExist, accRead, accWrite, accUpdate)
// for this VPathString is allowed, otherwise return FALSE.
//
boolean VPathString::access(enum accessMode mode)
{

#ifdef MSDOS
	return _access(gets(), mode) != 0 ? FALSE : TRUE;
#else
	return _access((char *)gets(), mode) ? FALSE : TRUE;
#endif

}

// ---------------------------------------------------------------------------
// Return the system time that the file specified by this VPathString
// was last modified. Return zero if this VPathString does not represent
// a valid, accessable file.
//
long VPathString::modifyTime()
{
			
	struct stat statbuf;
	char *p = (char *) gets();
#ifdef MSDOS
	if (isWild(p) || isBase(p)) {
		return 0L;
	}
#endif
#ifdef UNIX
	if (isWild(p)) {
		return 0L;
	}
#endif
	if (stat(p, &statbuf)) {
		return 0L;
	}
	return statbuf.st_mtime;

}

// ---------------------------------------------------------------------------
// Return TRUE if this VPathString names an existing file, otherwise
// return FALSE.  No environment path checking is performed.
//
boolean VPathString::exists() const
{
	return _access((char *)gets(), S_IEXIST) ? FALSE : TRUE;

}

// ---------------------------------------------------------------------------
// Return as a VString object the idxth slash separated name in this
// VPathString .  A zero length VString is returned if index idx is out
// of range.
//
VString VPathString::operator[](int idx)
{
	VString temp; // temp string
	char * p; // pointer to start of returned string segment
	char * end_p; // pointer to end of returned string segment
 
    VWideChar *Wp  , *Wend_p;
    
	if (idx < 0) {
		return (char *) 0;
	}

	temp = *this;
	if SINGLE_BYTE_CHAR_SET { 
		for (p = (char *) temp.gets(); idx > 0 && *p != '\0'; p++) {
			if (*p == cvPATH_SEP) {
				idx--;
			}
		}
		if (*p == '\0') {
			return (char *) 0;
		}
		for (end_p = p; *end_p != cvPATH_SEP && *end_p != '\0'; end_p++) {
			;
		}
		*end_p = '\0';  
		return VString(p); 
	} else { // use wide characters
		for (Wp = (VWideChar *) temp.getwcs(); idx > 0 && *Wp != CV_WCHAR_CONST('\0'); Wp++) {
			if (*Wp == WcvPATH_SEP) {
				idx--;
			}
		}
		if (*Wp == CV_WCHAR_CONST('\0')) {
			return (VWideChar *) 0;
		}
		for (Wend_p = Wp; *Wend_p != WcvPATH_SEP && *Wend_p != CV_WCHAR_CONST('\0'); Wend_p++) {
			;
		}
		*Wend_p = CV_WCHAR_CONST('\0');   
		return VString(Wp);
	}	
}

// ---------------------------------------------------------------------------
// Set the contents of this VPathString to the current working directory.
//
boolean VPathString::asCurrent()
{

	char buf[MAX_PATH];
	getcwd(buf, MAX_PATH);
	*this = buf;
# ifdef MSDOS
	toLower();
# endif

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VPathString::createDirectory(const VString& directory_name,
									 VPathString& result_directory_name)
{
	if (!isDirectory() || !exists()) {
		return FALSE;
	}
	char *s = (char *)(directory_name.gets());
	if ((s == 0) || (s[0] == '\0')) {
		return FALSE;
	}
	boolean ok = TRUE;
	int status, insep = 0;
	VPathString cwd; cwd.asCurrent(); setCurrent();
	char *p, *q;
	for (p = s, q = s ; *q != '\0' ; q++) {
		if (*q == cvPATH_SEP) {
			if (insep) { continue; }
			insep = 1;
			if (p < q) {
				char c = *q; *q = '\0';
				status = _access(p, S_IEXIST);
				if (status != 0) {
#if defined(UNIX)
					status = mkdir(p, S_IRWXU | S_IRWXG | S_IRWXO);
#else
					status = mkdir(p);
#endif
					*q = c;
					if (status != 0) {
						ok = FALSE;
						goto Done;
					}
				}
				else { *q = c; }
				while (*(q + 1) == cvPATH_SEP) { q++; } insep = 0;
			}
		}
		else {
			insep = 0;
		}
	}
	if (p < q) {
		char c = *q; *q = '\0';
		status = _access(p, S_IEXIST);
		if (status != 0) {
#if defined(UNIX)
			status = mkdir(p, S_IRWXU | S_IRWXG | S_IRWXO);
#else
			status = mkdir(p);
#endif
			*q = c;
			if (status != 0) {
				ok = FALSE;
				goto Done;
			}
		}
		else { *q = c; }
	}
Done:
	cwd.setCurrent();
	if (ok) {
		result_directory_name = *this;
		result_directory_name.addName(directory_name);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
//
boolean VPathString::createDirectory(const VString& directory_name)
{
	VPathString dummy;
	return createDirectory(directory_name, dummy);
}

// ---------------------------------------------------------------------------
//
boolean VPathString::removeDirectory(boolean all)
{
	if (all) {
		//
		// ToDo!!!
		//
		return unlink((char *)gets()) == 0 ? TRUE : FALSE;
	}
	else {
		return unlink((char *)gets()) == 0 ? TRUE : FALSE;
	}
}

#ifdef MSDOS
// ---------------------------------------------------------------------------
// PRIVATE:	Sets the current drive to the one in this VPathString.
// This function is unique to MS DOS and OS/2 platforms
// and it may not be portable.
//
boolean VPathString::setDrive()
{
	VString d;
	if (!driveOf(d)) {
		return FALSE;
	}  
	
	// note that the drive should always be a single byte 
	// ascii character

	char c = *((char *)d.gets());
	if (islower(c)) {
		c = toupper(c);
	}
	if (!okDrive(c)) {
		return FALSE;
	}
	selectDrive(c);
	return TRUE;  
	
}
#endif

// ---------------------------------------------------------------------------
// Set the current working directory to this VPathString.
//
boolean VPathString::setCurrent()
{
	VString tmp;
	if (isDirectory()) {
		tmp = gets();
	}
	else {
		buildPath(tmp, DEFAULT, DEFAULT, 0);
	}
	chdir((char *)tmp.gets());
#ifdef MSDOS
	setDrive();
#endif

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Make this VPathString represent the parent directory of the directory that
//	it now specifies.  This method assumes that the path string refers to
//	a directory path and not a file--discards the filename, if present.
//
//	Return FALSE, 	and leave this path string object untouched
//					if this directory is a root or "" 
//					(i.e., there is no parent directory).
//
//	return TRUE, 	otherwise.
//
boolean VPathString::asParent()
{
	VString	temp;
	char* p, * q;
    VWideChar* Wp, *Wq;
    

# ifdef MSDOS
	baseOf(temp);
# else
	directoryOf(temp);
# endif
if SINGLE_BYTE_CHAR_SET  {
	p = (char *) temp.gets();
	if (*p == '\0' || (*p == cvPATH_SEP && *(p+1) == '\0')) {
		//	at root, (no base directory string or string is "\")
		return FALSE;
	}

	//	find the last path separator in the base directory string
	for (q = tail(p); *q != cvPATH_SEP; q--) { 
		; 
	}
	if (q == p && *p == cvPATH_SEP) {
		//	if the last path separator is the only one, keep it
		p[1] = '\0';
	}
	else {
		//	otherwise, trim the last path separator
		*q = '\0';
	}
	buildPath(*this, DEFAULT, p, 0);	//	default drive, parent dir, no name
} else {   // MBCS safe   

	Wp = (VWideChar *) temp.getwcs();
	if (*Wp == CV_WCHAR_CONST('\0') || (*Wp == WcvPATH_SEP && *(Wp+1) == CV_WCHAR_CONST('\0'))) {
		//	at root, (no base directory string or string is "\")
		return FALSE;
	}

	//	find the last path separator in the base directory string
	for (Wq = tail(Wp); *Wq != WcvPATH_SEP; Wq--) { 
		; 
	}
	if (Wq == Wp && *Wp == WcvPATH_SEP) {
		//	if the last path separator is the only one, keep it
		Wp[1] = CV_WCHAR_CONST('\0');
	}
	else {
		//	otherwise, trim the last path separator
		*Wq = CV_WCHAR_CONST('\0');
	}  
	VString t(Wp);
	// buildpath requires a MBCS string as its 3rd argument
	buildPath(*this, DEFAULT,(char *) t.gets(), 0);	//	default drive, parent dir, no name
}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Copy 'size' characters from 'from' to 'to'.
//
static void copyFile(VFile *from, VFile *to, integer size)
{
	char buffer[1024];
	integer n;
	from->at(0);
	to->at(0);
	while (size > 0) {
		n = (unsigned)((size > 1024) ? 1024 : size);
		from->read(buffer, n);
		to->write(buffer, n);
		size -= (long)n;
	}
}

// ---------------------------------------------------------------------------
//
static boolean setFileModifyTime(VFile *target, const VFile *source)
{
	if (target == 0) {
		return FALSE;
	}

#if defined(MSDOS) || defined(UNIX)

	struct stat st;
	struct utimbuf ut;

	if (source == 0) {
		time_t t = time(0);
		ut.actime = t;
		ut.modtime = t;
	}
	else if (stat(((VFile *)source)->nameOf(), &st) == 0) {
		ut.actime = st.st_atime;
		ut.modtime = st.st_mtime;
	}
	else {
		return FALSE;
	}

	return utime(target->nameOf(), &ut) == 0;

#else

	// !!! ToDo for OS/2 and Mac.

	return FALSE;

#endif

}

// ---------------------------------------------------------------------------
// Move the file named by this VPathString to VPathString 'p', and
// return TRUE if successful.  Return FALSE if the file could not be moved.
//
boolean VPathString::move(const VPathString& p) const
{
	unlink((char *)p.gets());

#if defined(__ultrix) || defined(__VMS)
	char *thistmp = *this;
	char *totmp = to;
	if (!rename(thistmp, totmp)) {
#else
	if (!rename((char *)this->gets(), (char *)p.gets())) {
#endif
		return TRUE;
	}
	else {
		// save the result. cast away the const.
		(VPathStringResult&)theResult = errno;
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
// Copy the file associated with this VPathString to the file specified
// by VPathString to.   If file to already exists, it will be overwritten.
// Return TRUE if the operation was successful, otherwise return FALSE.
//
boolean VPathString::duplicate(const VPathString& to,
							   boolean preserve_from_time)
{
	VFile fromFile(*this);
	VFile toFile(to);
	if (!fromFile.open(ReadOnly)) {
		theResult = fromFile.getResult();
		return FALSE;
	}
	if (!toFile.open(Create)) {
		theResult = toFile.getResult();
		return FALSE;
	}
	copyFile(&fromFile, &toFile, fromFile.size());
	if (preserve_from_time) {
		setFileModifyTime(&toFile, &fromFile);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Remove (that is, delete) the file named by this VPathString.  Return
// TRUE if the file was successfully deleted, otherwise return FALSE.
//
boolean VPathString::remove() const
{
	if (unlink((char *)gets()) == 0) {
		return TRUE;
	}
	else {
		// save the result. cast away the const.
		(VPathStringResult&)theResult = errno;
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
// Set a random temporary file name in this VPathString.
//
void VPathString::tempFile()
{
	time_t ntime;
	char b[20];
	static int i = 0;
	time(&ntime);
	sprintf(b, "%2.2d%2.2d.tmp", ntime % 100, i++ % 100);
	*this = b;
}

// ---------------------------------------------------------------------------
//
void VPathString::directory(const VString& s)
{
	directory(s.gets());
}

#if defined(MSDOS) || defined(PM)
// ---------------------------------------------------------------------------
//
void VPathString::drive(const VString& s)
{
	drive(s.gets());
}
#endif

// ---------------------------------------------------------------------------
//
void VPathString::name(const VString& s)
{
	name(s.gets());
}

// ---------------------------------------------------------------------------
//
void VPathString::suffix(const VString& s)
{
	suffix(s.gets());
}

#ifndef WCHAR_H_EXISTS
// ---------------------------------------------------------------------------
// Compilation environment does not support standard C amendment 1
// functions for wide characters.  See also vwchar.h and vwchar.cpp.
//

// ---------------------------------------------------------------------------
// 
static int local_wstat(const wchar_t * path, struct stat * buffer){

	// this needs to be revisited on hosts (like windows 16 bit) that
	// support MBCS but no wide characters
	VString p(path);
	return stat(p.gets(), buffer);
}

#endif
