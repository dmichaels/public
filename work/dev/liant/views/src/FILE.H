// $Header:   Q:/views/common/vcs/FILE.H_V   1.17   Mar 24 1997 16:08:54   scottt  $

//	file.h
//
//	VFile interface [Common]
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
//	11/04/92 jmd	preened
//	05/12/93 pmcm	preened, made common between Motif and MS-Windows versions.
//	05/20/93 jmd	preened some more
//	07/09/93 pmcm	changed VString& param to file() to const
//	07/12/93 dgm	added breakAt() so as not to hide the base class one.
//	07/19/93 jmd	made size() const
//	08/09/93 jmd	removed free
//	08/11/93 pmcm	made write() and putch() take const arg
//	08/31/93 mrp    added the functions exists() and remove() to correct
//					change in product from 1.13 to 2.0. (modified by jmd)
//	09/17/93 dgm    (jmd) changed VFile(char*) to VFile(const VString&).
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	03/30/94 pkt	added const to VFile(char *) constructor.
//	09/22/94 jld	added stdDevice for use by close()
//	12/16/94 pkt	added 'setType', 'getType', and 'type'.
//	02/06/95 jld	added const to file()
//	03/01/95 pkt	merged.
//	03/20/95 pkt	added wide character versions of 'read', 'write', etc.
//	04/11/95 pkt	updated for VWideChar.
//	04/11/95 pkt	changed 'file' char* parameter to const.
//	06/01/95 pkt	merged international changes for source control.
//	06/30/95 pkt	merged OS/2 support.
//	07/06/95 pkt	inserted missing declaration of 'valid'.
//	09/06/95 pkt	removed OFI code.
//	10/30/95 dss	made VResult compatible on Windows.
//	11/02/95 dgm	#if 0'd operator=() since VResult has none;
//					Borland C++ complains; dss will need to look at this.
//	01/29/96 pkt	made 'mode()' public and const.
//	03/28/96 pkt	added 'getPos', and 'setPos'.
//	05/16/96 pkt	added read(VString &, filepos_t).
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VFILE_H
#define VFILE_H

// ---------------------------------------------------------------------------
//
#include "strm.h"
#include "pathstr.h"

// ---------------------------------------------------------------------------
//
enum fileMode { Create 		= 0, 	
				ReadOnly 	= 02, 	
				WriteOnly 	= 04, 
				Update 		= 06, 
				PrinterDev 	= 07, 
				Aux 		= 010, 
				StdIn 		= 011, 
				StdOut 		= 012, 
				StdError 	= 013, 
				Temporary 	= 014 
};

typedef integer	filepos_t;

// ---------------------------------------------------------------------------
//
#define DIRTY 010	// Set if contents of VStream have been modified
	
CLASS VFileResult : public VResult
{
public:

					// need explict op=() otherwise different VResult
					// derived classes will use explicit op=(?) 
					// (this class defines op=(int)). op=(int) only 
					//  copies the result value, not the result object.
#if 0
	VFileResult&	operator= (const VResult& r)
					{VResult::operator=(r); return *this;}
#endif
		
	// ***************************
	//
	// errno behavior		
	
					// makes this class act like errno (int)
	VFileResult&	operator=(int i) 
					{setResult(long(i), cioResultReport); return *this;}
					
					// makes this class act like errno (int)
					operator int() {return int(*(long*)&resultCode);}
};
	
// ---------------------------------------------------------------------------
//	The VFile class provides buffered access to system files.  
//	A VFile object can be associated with any file, closed, 
//	and reopened on a different file.  VFile objects can be 
//	constructed, or opened, with a file name which can be provided 
//	as a C string, a VString, or a VPathString.  
//	 Using  a VPathString object is the prefered way since 
//	VPathString's provide extensive capability for directory and 
//	path manipulations.

CLASS VFile : public VStream {
public:

	// Constructors & Destructor
		   			VFile();	
						VFile(const VString& fname); 
						VFile(const char *); 
						VFile(fileMode m); 
				   	~VFile();

	VClass			*iam();

	// Opening and closing

	boolean 			open(fileMode m);
	boolean 			open(fileMode m, const char *fname);
	boolean			openIt(int m);		// note: m is not fileMode,
										// it is one of the access flags
										// for the system open() routine
	virtual void 	close();

	// Reading and Writing 
	filepos_t		read(char *b, filepos_t n);
#ifdef CV_WCHAR
	filepos_t		read(VWideChar *, filepos_t);
#endif
	filepos_t		read(VString &s, filepos_t n);
	filepos_t		write(const char *b, filepos_t n);
	filepos_t		write(const VString&);
#ifdef CV_WCHAR
	filepos_t		write(const VWideChar *, filepos_t);
#endif
	int				getch(); 
	boolean			getch(VWideChar &);
	void				ungetch(char c);
#ifdef CV_WCHAR
	void				ungetch(VWideChar);
#endif
	boolean 			putch(const char c);
#ifdef CV_WCHAR
	boolean			putch(const VWideChar);
#endif

	// File Position 
	filepos_t 		at();
	filepos_t		getPos() const;
	filepos_t 		at(filepos_t pos); 
	filepos_t		setPos(filepos_t n);

	// Interrogation 
	boolean 			eof();
	boolean 			empty();
	VPathString& 	file();
	boolean 			isOpen();
	filepos_t		size() const;
	long				modifyTime();
 	fileMode	 		mode() const;

	// Truncation and Splitting 
	boolean 			truncAt(filepos_t n);
	boolean 			truncAt();
	VObject			*breakAt(filepos_t i);
	VObject			*breakAt();

	// Substrings of the VFile name 
	char   		   *driveOf();	
	char   		   *directoryOf();	  	// for backwards compatibility
	char   		   *nameOf();			// for backwards compatibility
	char   		   *suffixOf();			// for backwards compatibility

	// Setting VFile name and Environment
	void				suffix(char *s); 	// for backwards compatibility
	void				environ(char *s);	// for backwards compatibility
	void				file(const char *s);
	void				file(const VString& s);

	// Operations on Files 
	boolean 			fill();
	boolean 			flush();

	// File system operations (dispatched to VPathString)
	boolean			exists();
	boolean			remove();

	// Initialization
	void			cleanFile();

	VFileResult&	getResult();

protected:
	void				init();
	boolean 			dirty();
	void				dirty(boolean b);
	void				mode(int m);
	long				actualSize();

	VPathString		fileName; 		// current file name	
#if defined(PM)
	HFILE				device;			// file system descriptor
#else	
	short 		   device;	  		// file system descriptor
#endif
	boolean			valid;			// device descriptor is usable
	filepos_t 	   fsize;	   	// length of the file
	filepos_t 	   position; 		// actual file position
	filepos_t 	   offset;	  		// file offset to start of internal buffer
	VString    	   *env;	   		// current environment string
	VString 	   	temp;	   		// used by nameOf(), directoryOf()...
	boolean			stdDevice;		// stdin, stdout, stderr, stdaux or stdprn
	
	VFileResult		theResult;

#if defined(PM)
	unsigned short	attr;
	unsigned short	openFlags;
#endif

};

extern VClass *VFileCls;

// ---------------------------------------------------------------------------
//
inline boolean VFile::dirty() 
{
	return flag(DIRTY); 
}

// ---------------------------------------------------------------------------
//
inline fileMode VFile::mode() const 
{
	return fileMode(flags & Update);
}

// ---------------------------------------------------------------------------
//
inline void VFile::mode(int m) 
{
	flags &= ~Update;
	flags |= m; 
}

// ---------------------------------------------------------------------------
//
inline void VFile::dirty(boolean b) 
{
	flag(DIRTY, b); 
}

// ---------------------------------------------------------------------------
// Set the filename of this VFile to 's'.
//
inline void VFile::file(const char *s) 
{
	fileName.puts(s); 
}

// ---------------------------------------------------------------------------
// Set the name of this VFile to 's'.
//
inline void VFile::file(const VString &s) 
{
	fileName = s; 
}

// ---------------------------------------------------------------------------
// Return the name of this file.
//
inline VPathString& VFile::file() 
{
	return fileName; 
}

// ---------------------------------------------------------------------------
// Return TRUE if this VFile is open, otherwise return FALSE.
//
inline boolean VFile::isOpen() 
{
	return valid; 
}

// ---------------------------------------------------------------------------
//
inline boolean VFile::exists()
{
	return fileName.exists();
}

// ---------------------------------------------------------------------------
//
inline boolean VFile::remove()
{
	return fileName.remove();
}

// ---------------------------------------------------------------------------
//
inline VFileResult& VFile::getResult() 
{
	return theResult; 
}

#endif // VFILE_H
