// $Header:   Q:/views/common/vcs/PATHSTR.H_V   1.17   Mar 24 1997 16:12:52   scottt  $

//	pathstr.h
//
//	VPathString interface [Common]
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
//	05/11/93 pkt	reinstated derivation from VString.
//	05/17/93 pat	merged MOTIF and MS-Windows versions
//	05/19/93 pkt	ensured that all variants of gets are callable for
//					VPathStrings.
//	07/31/93 jmd	added consts, removed free()
//	09/26/93 jmd	preened
//	10/10/93 jmd	SC6 changes.
//	10/18/93 dgm	UNIX changes (from atb).
//	11/01/93 pkt	made gets const.
//	11/02/93 pkt	changed puts for const argument & integer return.
//	11/12/93 pkt	removed self.
//	11/14/93 pkt	made string parameter of getSearchPath const.
//	11/14/93 pkt	made setCreator etc. const member functions.
//	11/14/93 pkt	made operator char* a const member function.
//	11/15/93 pkt	made string parameter of setPath const.
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	08/12/94 pkt	added overrides of 'concat'.
//	08/12/94 pkt	eliminated #ifdef VPATHISNOSTRING.
//	08/26/94 pkt	tidied comments.
//	09/03/94 pkt	added VPathString(const FSSpec&);
//	09/06/94 pkt	added 'getMacCurDir' and 'setMacCurDir'.
//	01/15/95 pkt	added internationalization support.
//	01/27/95 pkt	merged Macintosh variant with the other.
//	01/30/95 pkt	changed 'directory' to take a const char *.
//	02/01/95 pkt	added 'operator=(const VString&)'.
//	02/06/95 jld	neutralized msvc 'environ' macro.
//	04/11/95 pkt	updated for VWideChar.
//	04/18/95 pkt	changed 'dotOf(VString&)' to return boolean.
//	03/30/95 jld	renamed environ() environVar() to avoid conflict
//	05/30/95 pkt	merged internationalization with 3.0.6.
//	07/18/95 dgm	Miscellaneous const char * changes.
//	08/04/95 dgm	Added optional final boolean parameter to duplicate()
//					to allow the user to specify that the original file
//					modification time should be preserved.
//	08/14/95 pkt	changed operator=(const char*) to be inline in OS/2.
//	09/05/95 dgm	Added createDirectory() and removeDirectory().
//	09/06/95 pkt	removed OFI code.
//	10/30/95 dss	made VResult compatible on Windows.
//	11/02/95 dgm	#if 0'd operator=() since VResult has none;
//					Borland C++ complains; dss will need to look at this.
//	06/06/96 djs	Added 'directoryContents' methods and supporting enum
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VPATHSTR_H
#define VPATHSTR_H

// ---------------------------------------------------------------------------
//
#include "str.h"
#include "ordcllct.h"
#include "result.h"

#ifdef environ		  // added for CRT DLL build with msvc 2.0
#undef environ
#endif

// ---------------------------------------------------------------------------
//
CLASS VFileSelect;

// ---------------------------------------------------------------------------
//
#ifdef UNIX
// C++/Views Directory selection defines used in dir method
#	define listFiles          0x01
#	define listDrives         0x02
#	define listSubDirectories 0x04
#	define listAll            0x07
#endif

#ifdef PM
// List types for 'directoryContents' method
#define DirOfFiles          	0x01
#define DirOfSubDirectories 	0x02
#define DirOfFilesAndDirs   	0x03

#endif

// ---------------------------------------------------------------------------
//
enum accessMode {	accExist	=  0,
#if defined(PM)
					accRead		= 02,
					accWrite	= 04,
#else
					accRead		= 04,
					accWrite	= 02,
#endif
					accUpdate	= 06
};

// ---------------------------------------------------------------------------
//
#ifdef UNIX
#	define MAX_PATH 255
#	define MAX_FILE 255
#elif defined(PM)
#	define MAX_PATH CCHMAXPATH
#	define MAX_FILE CCHMAXPATHCOMP
#elif defined(CV_WIN32)
#	define MAX_FILE MAX_PATH
#else
#	define MAX_PATH 200
#	define MAX_FILE 20
#endif

// ---------------------------------------------------------------------------
//
#	define DEFAULT ((const char *)-1L)

#ifdef MSDOS
#	define cvPATH_SEP '\\'
#endif

#if defined(UNIX) || defined(__VMS)
#	define cvPATH_SEP '/'
#endif

#ifdef SCO
#	undef remove
//	!!!pat per MOTIF header	#	define remove remove
#endif

CLASS VPathStringResult : public VResult
{
public:

						// need explict op=() otherwise different VResult
						// derived classes will use explicit op=(?) 
						// (this class defines op=(int)). op=(int) only 
						//  copies the result value, not the result object.
#if 0
	VPathStringResult&	operator= (const VResult& r)
						{VResult::operator=(r); return *this;}
#endif
		
	// ***************************
	//
	// errno behavior		
	
						// makes this class act like errno (int)
	VPathStringResult&	operator=(int i) 
						{setResult(long(i), cioResultReport); return *this;}
					
						// makes this class act like errno (int)
						operator int() {return int(*(long*)&resultCode);}
};

// ---------------------------------------------------------------------------
//	VPathString represents a platform independant file and/or directory
//	name. It provides methods for: (a) checking for the existance of and access
//	permissions of files; (b) for obtaining or setting the current working
//	directory; and (c) for manipulating component parts of a file and/or
//	directory name.

CLASS VPathString : public VString {
public:
	// Constructors
					VPathString();
					VPathString(const char *);
					VPathString(const VString&);
					VPathString(const VPathString&);

	VClass		   *iam();

	// FileSystem Operations
	boolean			isFile() const;
	boolean			isDirectory() const;
	boolean			access(accessMode);
	boolean			exists() const;
	boolean			list(VOrdCollect &, const char *env = 0,
						 boolean cwd = TRUE);
	boolean			list(VOrdCollect &, const VString& env, boolean cwd = TRUE);
	boolean			listNames(VOrdCollect &, const char *env = 0,
							  boolean cwd = TRUE);
	boolean			listNames(VOrdCollect &, const VString& env, boolean cwd = TRUE);
	boolean			find(const char *env = 0, boolean cwd = TRUE);
	boolean			find(const VString& env, boolean cwd = TRUE);
	void			expand(); 
	boolean			duplicate(const VPathString& p, boolean = FALSE);
	boolean			move(const VPathString& p) const;
	boolean			remove() const;

#ifdef PM
	// Directory listing methods
	boolean directoryContents(VOrdCollect &collect,
								const VString& mask,
								unsigned listType=DirOfFilesAndDirs);
	boolean directoryContentsNames(VOrdCollect &collect,
								const VString& mask,
								unsigned listType=DirOfFilesAndDirs);
	boolean doDirectoryContents(boolean full,
								VOrdCollect &collect,
								const VString& mask,
								unsigned listType=DirOfFilesAndDirs);

#endif

	// Operations on the VPathString
	void			tempFile();

#if defined(MSDOS) || defined(PM)
	boolean			driveOf(VString&);
	boolean			baseOf(VString&);
#endif

	boolean			directoryOf(VString&);
	boolean			nameOf(VString&);
	boolean			suffixOf(VString&);
	boolean			prefixOf(VString&);
	
	boolean			isMatch(VPathString&);

	void			suffix(const char *s);
	void			suffix(const VString& s);
	void			directory(const char *s);
	void			directory(const VString& s);
#if defined(MSDOS) || defined(PM)
	void			drive(const char *s);
	void			drive(const VString& s);
#endif
	void			name(const char *s);
	void			name(const VString& s);

	long			modifyTime();

	VString			operator[](int);
	boolean			asParent();
	VPathString&	addName(const char *name);
	VPathString&	addName(const VString& name);

	VPathString&	operator= (const char *s);
	VPathString&	operator= (const VString& s);
	VPathString&	operator= (const VPathString& ps);

	// Current Working Directory
	boolean			asCurrent(); 
	boolean			setCurrent();

	// Directory creation/deletion

	boolean			createDirectory(const VString&, VPathString&);
	boolean			createDirectory(const VString&);
	boolean			removeDirectory(boolean all = FALSE);

	VPathStringResult&	getResult();

#if defined(PM)
public:
#else
protected:
#endif
	boolean			environVar(const char *, VString&);
	boolean			environVar(const VString&, VString&);
#if defined(PM)
	boolean			putEnviron(char *f, VString& s);
#endif

protected:
	void			buildPath(VString&, const char *,
							  const char *, const char *);
	void			buildPath(VString&, const VString&,
							  const VString&, const VString&);
	boolean			dolist(boolean, VOrdCollect&, const char *, boolean);
	boolean			dolist(boolean, VOrdCollect&, const VString&, boolean);

	VPathStringResult	theResult;

	char				*dotOf();
	boolean			dotOf(VString&) const;

#if defined(MSDOS) || defined(PM)
	boolean			setDrive(); 
#endif

	void			initialize();

};

extern VClass *VPathStringCls;

// ---------------------------------------------------------------------------
//	This is a class used by the static methods 
//	VPathString::findFirst() and VPathString::findNext()
//
#ifdef UNIX

class findBuf {
public:
	DIR			   *fblock;
	VPathString		dir;
	VPathString		name;
	int				flags;
};

#endif // UNIX

// ---------------------------------------------------------------------------
//
inline void VPathString::initialize()
{
}

// ---------------------------------------------------------------------------
// Set the directory portion of the VPathString to the Null terminated
// C string s.
//
inline void VPathString::directory(const char *s)
{
	buildPath(*this, DEFAULT, s, DEFAULT);
}

#if defined(MSDOS) || defined(PM)
// ---------------------------------------------------------------------------
// Set the drive portion of this VPathString to the Null terminated
// C string s.
//
inline void VPathString::drive(const char *s)
{
	buildPath(*this, s, DEFAULT, DEFAULT);
}
#endif // MSDOS or PM

// ---------------------------------------------------------------------------
//
inline void VPathString::name(const char *s)
{
	buildPath(*this, DEFAULT, DEFAULT, s);
}

// ---------------------------------------------------------------------------
// Set the contents of this VPathString to the Null terminated
// C string s.
//
inline VPathString& VPathString::operator=(const char *s)
{
	puts(s);
	return *this;
}

// ---------------------------------------------------------------------------
// Set the contents of this VPathString equal to the contents of
// VPathString ps.
//
inline VPathString& VPathString::operator=(const VPathString &ps)
{
	puts(ps.gets());
	return *this;
}

// ---------------------------------------------------------------------------
// Set the contents of this VPathString equal to the contents of
// VString s.
//
inline VPathString& VPathString::operator=(const VString &s)
{
    puts(s.gets());
    return *this;
}  
 
// ---------------------------------------------------------------------------
//
inline VPathStringResult& VPathString::getResult() 
{
	return theResult; 
}

// ===========================================================================

#endif // VPATHSTR_H
