// $Header:   Q:/views/common/vcs/FILE.CPV   1.22   Mar 24 1997 16:08:52   scottt  $

//	file.cpp
//
//	VFile class implementation [Common]
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	04/01/93 pat	preened
//	04/06/93 pat	added UNIX/MOTIF stuff
//	04/07/93 pat	changed id to VObject *
//	07/06/93 pat	preened.
//	07/07/93 pkt	Removed non-Mac code.
//	07/12/93 dgm	added breakAt() so as not to hide the base class one.
//	07/19/93 jmd	made size() const
//  08/09/93 jmd	removed free
//	08/11/93 pat	made write() and putch() take const arg
//	08/23/93 pat	removed fill() from write(); fixed comments.
//	09/17/93 dgm    (jmd) changed VFile(char *) to VFile(const VString&).
//	10/10/93 jmd	SC6 changes.
//	10/19/93 dgm	UNIX changes.
//	10/29/93 bkd	cleaned up top of this file, changed all _lXXX to
//			 		_sys_XXX.
//	12/27/93 pkt	changed for Symantec compiler.
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	01/19/94 bkd	added CPPV_HPUX ifdef to prevent including <io.h>
//	01/30/94 dgm	simplified ++ expression in getch() to
//					avoid apparent Borland C++ 4.0 compiler bug.
//	02/02/94 bkd	AIX/6000 ifdefs
//	02/24/94 dgm	old C preprocessor changes
//	03/24/94 pat	CV_WIN32:
//					auto defined CV_WINDOWS;
//					defined _sys_lseek as _llseek
//	03/30/94 pkt	removed 'enum' from signature of 'open'.
//	03/30/94 pkt	added const to VFile(char *) constructor.
//	04/02/94 pkt	changed 'open' to use O_BINARY with Symantec for Mac.
//	04/13/94 pat	CV_WIN32: used OpenFile() call under Create mode
//	07/08/94 pkt	changed 'open' to set file type to 'TEXT' with Symantec
//					for Mac.
//	07/14/94 jld	added dependency on defs.h to define CV_WINDOWS
//	08/17/94 dgm	Changed CREATE_FLAGS to respect umask.
//	08/22/94 dgm	Minor changes for SGI port.
//	08/27/94 pkt	updated for Metrowerks C++.
//	08/31/94 dgm	Minor UnixWare/MetaWare-C++ related changes.
//	09/15/94 dgm	Minor OSF-1/DEC-C++ changes.
//	09/22/94 jld	made _sys_XXX defines the same for all Windows
//	09/21/94 jld	using SEEK_ constants instead of hard code
//	09/22/94 jld	added stdDevice flag to be used in close()
//	10/20/94 dss	Corrected open flags per platform. Use binary file I/O
//					on MSDOS (not MSW). Was using text for MS compiler.
//	10/20/94 dgm	Minor Linux/GNU-C++ changes.
//	12/16/94 pkt	added 'setType' and 'getType'; updated for 'type'.
//	12/16/94 pkt	fixed 'open' to set file type only when creating file.
//	01/09/95 pkt	unconditional changes for Metrowerks CW5.
//	03/01/95 pkt	merged.
//	03/08/95 pkt	revised for international strings.
//	03/20/95 pkt	added wide character versions of 'read', 'write', etc.
//	03/24/95 pkt	minor fixes.
//	03/30/95 pkt	updated to use 'isAsByte' rather than 'asByteFlag'.
//	04/06/95 pkt	fixed 'fill' to leave 'position' correct.
//	04/11/95 pkt	updated for VWideChar.
//	04/12/95 pkt	updated to call 'getCurrent'.
//	04/21/95 pkt	replaced MB_XXX with CV_MB_XXX.
//	06/01/95 pkt	merged international changes for source control.
//	06/30/95 pkt	preened OS/2 version prior to merge with common code.
//	06/30/95 pkt	merged OS/2 support into common code.
//	07/14/95 pkt	updated PM portions for internationalization.
//	07/14/95 pkt	fixed 6/30 merge: TURBO not defined by Borland on OS/2.
//	08/15/95 pkt	fixed 'read' and 'write' for wide characters without
//					explicit buffers.
//	09/06/95 pkt	removed OFI code.
//	10/30/95 dss	made VResult compatible on Windows.
//	11/07/95 tlf	updated for Motif.
//	11/09/95 tlf	updated for AIX.
//	11/29/95 tlf	updated for HPUX.
//	12/11/95 dgm	Added stdlib.h/errno.h includes for Borland.
//	03/28/96 pkt	added 'getPos', and 'setPos'.
//	05/02/96 pkt	fixed 'write(const VWideChar *, integer)' to not switch
//					the file into MBCS mode.
//	05/16/96 pkt	added read(VString &, filepos_t).
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

// The _sys_XXX macros refer to Unix-system-call-like routines.
// These may exist in some environments as _lXXX routines, and
// in others as XXX.

#include "defs.h"

#if defined(CV_WIN32)
#	define	OPEN_READONLY	OF_READ
#	define	OPEN_WRITEONLY	OF_WRITE
#	define	OPEN_READWRITE	OF_READWRITE
#elif defined(CV_WINDOWS)
#	define	OPEN_READONLY	READ
#	define	OPEN_WRITEONLY	WRITE
#	define	OPEN_READWRITE	READ_WRITE
#elif defined(MSDOS)
#	define	USE_O_BINARY
#	define	OPEN_READONLY	(O_RDONLY | O_BINARY)
#	define	OPEN_WRITEONLY	(O_WRONLY | O_BINARY)
#	define	OPEN_READWRITE	(O_RDWR | O_BINARY)
#elif defined(PM)
#	define	OPEN_READONLY	(OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE)
#	define	OPEN_WRITEONLY	(OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE | OPEN_FLAGS_WRITE_THROUGH)
#	define	OPEN_READWRITE	(OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE | OPEN_FLAGS_WRITE_THROUGH)
#else
# if defined(V_MACINTOSH)
#	if defined(__SC__) || defined(__MWERKS__)
		// pkt - Prevent swapping of CR and LF.
#		define USE_O_BINARY
#	endif
#	if defined(__MWERKS__)
#		define FAKE_CREAT
#	endif
# endif
# if defined(USE_O_BINARY)
#	define	OPEN_READONLY	(O_RDONLY | O_BINARY)
#	define	OPEN_WRITEONLY	(O_WRONLY | O_BINARY)
#	define	OPEN_READWRITE	(O_RDWR | O_BINARY)
# else
#	define	OPEN_READONLY	O_RDONLY
#	define	OPEN_WRITEONLY	O_WRONLY
#	define	OPEN_READWRITE	O_RDWR
# endif
#endif

#if defined(CV_WINDOWS)
#	define CREATE_FLAGS (0)	//	read/write permission under _lcreat
#else
#   define CREATE_FLAGS (S_IRUSR | S_IWUSR | \
					     S_IRGRP | S_IWGRP | \
					     S_IROTH | S_IWOTH)
#endif

#if !defined(UNIX)

#	ifdef CV_WINDOWS
#		include "notifier.h"
#		include "winclass.h"
#	endif

#	include "file.h"

extern "C" {
#	include <ctype.h>
#	include <stdio.h>
#	include <string.h>
#	if defined(PM)
#		include <time.h>
#	else
#		include <io.h>
#		include <sys/types.h>
#		include <sys/stat.h>
#	endif
#	include <errno.h>
}

#	if defined(CV_WINDOWS)

#		define _sys_lseek	_llseek
#		define _sys_creat	_lcreat
#		define _sys_read	_lread
#		define _sys_write	_lwrite
#		define _sys_close	_lclose
#		define _sys_open	_lopen
#		define SETRESULT(d) theResult.setResult(d, mswioResultReport)
#	elif defined(PM)
		// VWSPORT (djs) - Need to determine "_sys" defines
#		define SETRESULT(d) theResult.setResult(d, mswioResultReport)
#	else
#		if	defined(MSDOS)
			// don't use creat(). as it defaults to O_TEXT mode.
			// O_CREAT | O_TRUNC give same effect as creat().
#			define _sys_creat(fn, flags) ::open(fn, OPEN_READWRITE | O_CREAT | O_TRUNC, flags)
#		else
#			define _sys_creat	creat
#		endif
#		define _sys_lseek	lseek
#		define _sys_read	::read
#		define _sys_write	::write
#		define _sys_close	::close
#		define _sys_open	::open
#		define SETRESULT(d) ((VResult&)theResult = (d))
#	endif

#	ifdef V_MACINTOSH
#		ifdef __MWERKS__
#			include <unix.h>
#		endif
#	elif defined(PM)
#	else
#		include <fcntl.h>
#	endif

//============================================================================
#else	//	UNIX

// The mess that follows is all about getting protypes for
//  creat, lseek, read, write, close and open, and to get definitions
//  for the constants: S_IREAD, S_IWRITE, O_RDONLY, O_WRONLY, O_RDWR.

#	define _sys_creat		creat
#	define _sys_lseek		lseek

#	define _sys_read		::read
#	define _sys_write		::write
#	define _sys_close		::close
#	define _sys_open		::open

// #	define SETRESULT(d) ((VResult&)theResult = (d))
#	define SETRESULT(d) (theResult = (d))

extern "C" {

#	include <ctype.h>
#	include <stdio.h>
#	include <string.h>
#	include <errno.h>

#	if !defined(SYSV) && !defined(SUN) && !defined(__ultrix) && \
	   !defined(__VMS) && !defined(CPPV_HPUX) && !defined(CPPV_AIX6000) && \
	   !defined(CV_IRIX) && !defined(__GNUC__) && !defined(__osf__) && \
	   !defined(CV_UNIXWARE)
#		include <io.h>
#	endif

#	ifndef __SYS_TYPES_H
#		include <sys/types.h>	/* necessary for sys/stat.h below */
#	endif

#	include <sys/stat.h>		/* for S_IREAD, S_IWRITE only */

//!!!bkd - 10/29/93 unistd.h doesn't seem to be needed because we aren't
//  using the SEEK_ constants in the code (though we should be).
//

//#	ifdef SUN
//#		include <unistd.h>
//#	else
//#		ifdef __ultrix
//#			include <unistd.h>
//#		else
//#			include <sys/unistd.h>
//#		endif
//#	endif

#	include <fcntl.h>		/* for O_RDONLY, O_WRONLY, O_RDWR */

} //	end of external C junk for UNIX

#	ifdef MOTIF
#		include "notifier.h"
#	endif

#	include "winclass.h"
#	include "file.h"

//!!!bkd - 10/29/93 We need a better piece of logic to determine that
//  the system provided no prototypes for the magic 6 functions.
//
//#	if !defined (SVR4) && (defined (SCO) || defined (SYSV))
//extern "C" {
//int close(int);
//long lseek(int, long, int);
//int read(int, char *, unsigned);
//int write(int, char *, unsigned);
//}
//#	endif
//
//#	if (!defined (SVR4) || defined (CII)) && !defined(SUN)
//extern "C" {
//int creat(char*, int);
//int open(char*, int, ...);
//}
//#	endif

extern "C"{
#if !defined(__LPI_CXX__) && !defined(CENTERLINE) && !defined(SOLARIS) && \
	!defined(CPPV_HPUX) && !defined(CPPV_AIX6000) && !defined(CV_OSCC) && \
	!defined(SUN) && !defined(SCO) && !defined(CV_IRIX) && \
	!defined(CV_SVR4) && !defined(__GNUC__) && !defined(__osf__)
extern int	creat (char *, int);
extern int	open  (char *, int);
#endif
#if ( !defined(CENTERLINE) || XtSpecificationRelease > 4 )  && !defined(CPPV_AIX6000)
extern int	close (int);
extern int	read  (int, char *, unsigned);
extern int	write (int, char *, unsigned);
#endif
extern long	lseek (int, long, int);
}

#endif	//	UNIX

// ===========================================================================

const int FileBufferSize = 1024;

///	helper function
static void copyFile(VFile *from, VFile *to, integer size);

defineClass(VFile, VStream)

// ---------------------------------------------------------------------------
//	Construct a VFile which is not bound to any file.
//
VFile::VFile()
{
	init();
}

// ---------------------------------------------------------------------------
//	Construct a VFile on file 'fname'.  'fname' can be a C string, 
//	VString, or a VPathString.  The name of the file is maintained by 
//	VFile objects as a VPathString object, which can be accessed through 
//	the file() method to perform directory and path operations.
//
VFile::VFile(const VString& fname)
{
	init();
	fileName = fname;
}

// ---------------------------------------------------------------------------
// The name of the file is maintained by VFile objects as a VPathString
// object, which can be accessed through the file() method to perform
// directory and path operations.
// N.B. This constructor exists as a work-around to a cfront compiler bug.
//
VFile::VFile(const char *str)
{
	init();
	fileName.puts(str);
}

// ---------------------------------------------------------------------------
//	Construct a VFile which is open on one of several special files.
//	'mode' is one of the following:  
//
//		Stdin
//		Stdout
//		Auxilary
//		PrinterDev
//		Temporary		(file is created in the system temporary directory)
//
VFile::VFile(fileMode mode)
{
	char *f = 0;

	init();

	switch (mode) {

	case PrinterDev:
#if defined(CV_WINDOWS) || defined(PM)
		f = 0;
#else
		mode = WriteOnly;
		f = "/dev/printer";
#endif
		break;

	case Aux:
#if defined(CV_WINDOWS) || defined(PM)
		f = 0;
#else
		mode = Update;
		f = "/dev/aux";
#endif
		break;
	}	//	end of mode switch

	(void) open(mode, f);
}

// ---------------------------------------------------------------------------
//
void VFile::init()
{
	valid		= FALSE;
	fsize 		= -1L; 		//	length of the file
	position 	= 0;		//	position of the physical file 'device',
							//	used to avoid unnecessary seeks
	offset 		= 0; 		// 	file offset to start of internal buffer,
							//	difference between at() and 'currentEle'
	env 		= 0;		//	current environment string
	stdDevice	= FALSE;	//  not stdin, stdout, etc.
	// asByteString(TRUE);
	expandable(FALSE);		
	expandTo(FileBufferSize);
}

// ---------------------------------------------------------------------------
//
VFile::~VFile() 
{
	cleanFile(); 
	if (env != 0) {
		delete env;		// jmd (was free)
	}
}

// ---------------------------------------------------------------------------
//	Flush and close this VFile.  Free any auxiliary memory used by it.
//
void VFile::cleanFile()
{
	close();
}

// ---------------------------------------------------------------------------
//	Flush and close this VFile.
//
void VFile::close()
{
	if (!valid) {
		//	no file system descriptor, nothing to close
		return;
	}

	flush();
	if (!stdDevice) {
#if defined(PM)
		DosClose(device);
#else
		int result = _sys_close(device);
		if (result < 0) {
			SETRESULT(errno);
		}
#endif
	}
	valid = FALSE;
}

// ---------------------------------------------------------------------------
//	Open this VFile on a file named 'fname' using mode 'm'.  
//	'mode' is one of the following:
//	
//		Create

//		ReadOnly
//		WriteOnly
//		Update  
//
//	An existing VPathString object can be provided for the 'fname' parameter 
//	since VPathString's can be treated as C strings through the (char *) cast
//	operator.  This allows for file name and directory manipulation prior to
//	opening the file.
//
boolean VFile::open(fileMode m, const char *fname) 
{ 
	fileName = fname;
	return open(m);
}

// ---------------------------------------------------------------------------
//	Open this VFile with mode 'm' on its current file.
//
boolean VFile::open(fileMode m) 
{
	boolean needs_text_change = FALSE;
	boolean check_text_change = FALSE;
#if defined(V_MACINTOSH) && defined(USE_O_BINARY)
	// May need to explicitly change the file type to 'TEXT'.
	check_text_change = TRUE;
#endif

	if (valid) {
		close();
	}

	fsize = -1; 
	position = offset = 0; 
	expandable(FALSE);
	expandTo(FileBufferSize);
	reset();

	switch (m) {

	// -----------------------------------------------------------------------
	//	fileMode in		internal flags set to 			device
	//	-----------		--------------------------		------
	//	StdIn			ReadOnly						0
	//	StdOut			WriteOnly						1
	//	StdError		WriteOnly						2
	//	Aux	  			Update (read/write)				3
	//	PrinterDev		WriteOnly						4
	//	Temporary		Update (read/write)				system file handle
	//	Create			WriteOnly						system file handle
	//	ReadOnly		ReadOnly						system file handle
	//	WriteOnly		WriteOnly						system file handle
	//	Update			Update							system file handle
	//
	//	!!! pat
	//		In a non-MS Windows context:
	//
	//		VFile(Aux)			calls open(Update, "/dev/aux")
	//							which calls open(Update)
	//		VFile(PrinterDev) 	calls open(WriteOnly, "/dev/printer")
	//							which calls open(WriteOnly)
	//		
	//		if the user calls open(enum fileMode m, char *fname)
	//		directly, should it behave the same (i.e, change the mode and
	//		filename?).  what about calling open(enum fileMode m) directly?
	//		
	// -----------------------------------------------------------------------

	case StdIn:
		m = ReadOnly;
		device = 0;
		valid = TRUE;
		stdDevice = TRUE;
		break;
	case StdOut:
		m = WriteOnly;
		device = 1;
		valid = TRUE;
		stdDevice = TRUE;
		break;
	case StdError:
		m = WriteOnly;
		device = 2;
		valid = TRUE;
		stdDevice = TRUE;
		break;
	case Aux:
#if defined(PM)
		fileName = "AUX";
		attr = FILE_NORMAL;
		openFlags = FILE_OPEN;
		valid = openIt(OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYNONE);
		stdDevice = FALSE;
#else
		m = Update;
		device = 3;
		valid = TRUE;
		stdDevice = TRUE;
#endif
		break;
	case PrinterDev:
#if defined(PM)
		fileName = "PRN";
		attr = FILE_NORMAL;
		openFlags = FILE_OPEN;
		valid = openIt(OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE);
		stdDevice = FALSE;
#else
		m = WriteOnly;
		device = 4;
		valid = TRUE;
		stdDevice = TRUE;
#endif
		break;
	case Temporary:
		fileName.tempFile();
		m = Update;
#if defined(USE_O_BINARY) && !defined(FAKE_CREAT)
		_fmode = O_BINARY;
#endif
		needs_text_change = check_text_change && !fileName.exists();
#if defined(PM)
		attr = FILE_NORMAL;
		openFlags = FILE_CREATE;
		valid = openIt(OPEN_READWRITE);
#elif defined(V_MACINTOSH)
#  if defined(FAKE_CREAT) && defined(USE_O_BINARY)
		device = (int)_sys_creat((char *)fileName.gets(), O_CREAT | O_BINARY);
#  else
		device = (int)_sys_creat((char *)fileName.gets(), O_CREAT);
#  endif
		valid = (device >= 0);
#else
		device = (int)_sys_creat((char *)fileName.gets(), CREATE_FLAGS);
		valid = (device >= 0);
#endif
		stdDevice = FALSE;
		if (valid) {
			SETRESULT(errno);
		}
		break;

	case ReadOnly:
#if defined(PM)
		attr = FILE_READONLY;
		openFlags = FILE_OPEN;
#endif
		valid = openIt(OPEN_READONLY);
		stdDevice = FALSE;
		break;

	case WriteOnly:
		needs_text_change = check_text_change && !fileName.exists();
#if defined(PM)
		attr = FILE_NORMAL;
		openFlags = FILE_OPEN;
#endif
		valid = openIt(OPEN_WRITEONLY);
		stdDevice = FALSE;
		break;

	case Update:
		needs_text_change = check_text_change && !fileName.exists();
#if defined(PM)
		attr = FILE_NORMAL;
		openFlags = FILE_OPEN;
#endif
		valid = openIt(OPEN_READWRITE);
		stdDevice = FALSE;
		break;

	case Create:
		needs_text_change = check_text_change;
#if defined(USE_O_BINARY) && !defined(FAKE_CREAT)
		_fmode = O_BINARY;
#endif
#if defined(PM)
		attr = FILE_NORMAL;
		openFlags = FILE_CREATE | FILE_TRUNCATE;
		valid = openIt(OPEN_READWRITE);
#elif defined(V_MACINTOSH)
#  if defined(FAKE_CREAT) && defined(USE_O_BINARY)
		device = (int)_sys_creat((char *)fileName.gets(), O_CREAT | O_BINARY);
#  else
		device = (int)_sys_creat((char *)fileName.gets(), O_CREAT);
#  endif
		valid = (device >= 0);
#else
		device = (int)_sys_creat((char *)fileName.gets(), CREATE_FLAGS);
		valid = (device >= 0);
#endif
		m = WriteOnly;
		stdDevice = FALSE;
		if (valid) {
			SETRESULT(errno);
		}
		break;
	}	//	end of mode switch


	mode(m);
#if defined(V_MACINTOSH)
	if (device >= 0 && needs_text_change) {
		fileName.setType('TEXT');
	}
#endif
	return valid;
}

// ---------------------------------------------------------------------------
//	Open the current file with 'mode'. 
//	Tries 'fileName' first, then the 'env' environment string.
//
//	Note:	'mode' here is not enum fileMode, but system mode.
//
boolean VFile::openIt(int mode)
{
#if defined(PM)
	unsigned long action;

	if (DosOpen((PSZ)fileName.gets(), &device, &action, 0L, attr, openFlags, mode, 0L) == 0) {
		return TRUE;
	}
#else
	int d;
	if ((d = _sys_open((char *)fileName.gets(), mode)) >= 0) {
		device = d;
		return TRUE;
	}
#endif
	if (env == 0 || !fileName.find(*env)) {
		theResult = ENOENT;
		return FALSE;
	}
#if defined(PM)
	if (DosOpen((PSZ)fileName.gets(), &device, &action, 0L, attr, openFlags, mode, 0L) == 0) {
		return TRUE;
	}
#else
	if ((d = _sys_open((char *)fileName.gets(), mode)) >= 0) {
		device = d;
		return TRUE;
	}
#endif

	SETRESULT(errno);
	
	return FALSE;
}

// ---------------------------------------------------------------------------
//	Return the system time at which this VFile was last modified.
//
long VFile::modifyTime()
{
#if defined(PM)
	if (valid) {
		FILESTATUS fs;
		struct tm t;

		if (DosQueryFileInfo(device, FIL_STANDARD, (PBYTE)&fs, sizeof(fs)) != 0) {
			return(-1);
		}
   
		t.tm_sec  = fs.ftimeLastWrite.twosecs * 2;
		t.tm_min  = fs.ftimeLastWrite.minutes;
		t.tm_hour = fs.ftimeLastWrite.hours;
		t.tm_mday = fs.fdateLastWrite.day;
		t.tm_mon  = fs.fdateLastWrite.month;
		t.tm_year = fs.fdateLastWrite.year;
		return mktime(&t);
	}
#endif

	return fileName.modifyTime();
}

// ---------------------------------------------------------------------------
//	Returns the number of bytes in the system file to with which this 
//	VFile is associated.
//
integer VFile::actualSize()
{
	//	seek to the end of the file to find its size

#if defined(PM)
	DosChgFilePtr(device, 0, FILE_END, (unsigned long *)&position);
#else
	position = _sys_lseek(device, (long)0, SEEK_END);
#endif
	return position;
}

// ---------------------------------------------------------------------------
//	Return the number of bytes in this VFile.
//
integer VFile::size() const
{
	if (fsize < 0 || nEle == 0) {
		if (!valid) {
			asWideString(FALSE);
			return (((VFile *)(this))->fsize = lengthEle);
		}
		((VFile *)(this))->flush();
		((VFile *)(this))->fsize = ((VFile *)(this))->actualSize();
	}
	return fsize;
}

// ---------------------------------------------------------------------------
//	Flush the internal buffer of this VFile into its associated file.
//
//	Assumes VFile pointer at offset+position.
//	at() is left unchanged.
//	Returns FALSE on i/o write else TRUE
//	Leaves VFile pointer at offset+position.
//	Leaves length and current alone if current < length else
//	if current == length then current = length = 0.
//
boolean VFile::flush()
{
#if defined(PM)
	unsigned long n;
#else
	integer n;
#endif

	if (!dirty() || lengthEle == 0 || nEle == 0 || !valid) {
		//	the internal buffer has not been modified, or is empty, 
		//	or of no size, or the file descriptor is invalid

		return TRUE;
	}

	if (position != offset) {
		//	position the file to receive the buffer

#if defined(PM)
		DosChgFilePtr(device, offset, FILE_BEGIN, (unsigned long *)&position);
#else
		position = _sys_lseek(device, offset, SEEK_SET);
#endif
		if (position != offset) {
			SETRESULT(errno);
			return FALSE;
		}
	}

	//	flush the data in the buffer to the file

	asWideString(FALSE);
#if defined(PM)
	DosWrite(device, getData(), lengthEle, &n);
#else
	n = (integer)_sys_write(device, getData(), (unsigned)lengthEle);
#endif
	if (n < 0) {
		SETRESULT(errno);
		return FALSE;
	}

	position += n;
	if (position > fsize) {
		fsize = position;	//	update running notion of file size
	}
	dirty(FALSE);

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Fill the internal buffer of this VFile from its associated
//	file starting at the current position.
//
//	Assumes VFile pointer at offset+position.
//	returns FALSE if i/o error on read else TRUE
//	If current == length then current <- 0.
//	Leaves length at nEle or less if eof reached.
//	Leaves VFile pointer at offset+position.
//
boolean VFile::fill()
{
#if defined(PM)
	unsigned long n;
#else
	integer n;
#endif
	if (nEle == 0 || !valid) {
		//	there is no buffer, or the file descriptor is invalid
		return TRUE;
	}

	asWideString(FALSE);
	// asByteString(TRUE);
	flush();
	if (offset != position) {
		//	position file to start of buffer

#if defined(PM)
		DosChgFilePtr(device, offset, FILE_BEGIN, (unsigned long *)&position);
#else
		position = _sys_lseek(device, offset, SEEK_SET);
#endif
		if (position != offset) {
			SETRESULT(errno);
			return FALSE;
		}
	}

	char *d = getData();
#if defined(PM)
	DosRead(device, d, nEle, &n);
#else
	n = _sys_read(device, d, (unsigned) nEle);
#endif
	if (n < 0) {
		SETRESULT(errno);
		return FALSE;
	}

	dirty(FALSE);
	lengthEle = n;
	short left_over;
	length = fromElePosition(lengthEle, &left_over);
	lengthEle -= left_over;
	position += n;
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Position the file as requested.  Return the actual 
//	position of the file after the attempt to position it.
//	The returned value will be less than the requested position, if the
//	requested position is greater than the number of characters in the file.
//
//	Returns -1 if i/o error else at().
//	Leaves VFile pointer at offset+position; position == 0.
//	Leaves at() at l, i.e. offset + current.
//
integer VFile::at(integer pos)
{
	asWideString(FALSE);
	if (nEle == 0) {
#if defined(PM)
		DosChgFilePtr(device, 0, FILE_CURRENT, (unsigned long *)&position);
		return position;
#else
		long result = _sys_lseek(device, 0L, SEEK_CUR);
		if (result < 0) {
			SETRESULT(errno);
		}
		return result;
#endif
	}
	if (fsize < 0) {
		(void) size();
	}
	if (pos < 0) {
		if ((pos = fsize - pos) < 0) {
			pos = 0;
		}
	}
	else if (pos > fsize) {
		pos = fsize;
	}

	if (!valid || (pos >= offset && pos < offset + lengthEle)) {
		//	file descriptor is invalid or the requested position 
		//	falls within buffer limits

		currentEle = (unsigned) (pos - offset);	
		current = fromElePosition(currentEle, &byteOfCharacter);
		bytesOfPrev = SINGLE_BYTE_CHAR_SET ? 1 : 0;
	}
	else {
		//	file descriptor is valid and the requested position 
		//	falls outside buffer limits

		flush();
		current = length = 0;
		currentEle = lengthEle = 0;
		byteOfCharacter = 0;
		bytesOfPrev = 0;
#if defined(PM)
		DosChgFilePtr(device, pos, FILE_BEGIN, (unsigned long *)&position);
#else
		position = _sys_lseek(device, pos, SEEK_SET);
#endif
		offset = position;
		if (offset < 0) {
			SETRESULT(errno);
			return -1L;
		}
	}
	return pos;
}

// ---------------------------------------------------------------------------
// Override VStream::getPos to use byte positioning rather than character,
// because character positioning in a file is difficult and/or inefficient.
//
filepos_t VFile::setPos(filepos_t pos)
{
	return at(pos);
}

// ---------------------------------------------------------------------------
//	Return the next character from this VFile and advance the file
//	position.
//
int VFile::getch()
{
	char c;

	asWideString(FALSE);
	if (nEle != 0 && currentEle < lengthEle) {
		c = VStream::getch();
	}
	else {
		if (read(&c, 1) != 1) {
			return -1;
		}
	}

	return (c & 0377);	//	character values 0 - 255 (0x00 to 0xFF)
}

// ---------------------------------------------------------------------------
//	Return the next character from this VFile as 'ch' and advance the file
//	position.  Return FALSE at end of file.
//
boolean VFile::getch(VWideChar &ch)
{
	if (nEle != 0 && currentEle < lengthEle) {
		return VStream::getch(ch);
	}
	else {
		if (read(&ch, 1) != 1) {
			return FALSE;
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Arrange to have 'c' returned as the next character returned by
//	getch() or read(). This method may be called only after a getch() or
//	a read() call.
//
void VFile::ungetch(char c)
{
	if (c == -1) {
		return;
	}

	if (nEle != 0 && currentEle != 0) {
		VStream::ungetch(c);
	}
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//	Arrange to have 'c' returned as the next character returned by
//	getch() or read(). This method may be called only after a getch() or
//	a read() call.
//
void VFile::ungetch(VWideChar c)
{
	if (c == (VWideChar)-1) {
		return;
	}

	if (nEle != 0 && currentEle != 0) {
		VStream::ungetch(c);
	}
}
#endif

// ---------------------------------------------------------------------------
//	Write character 'c' into this VFile and  advance the current position.
//
boolean VFile::putch(const char c)
{
	return (write(&c, 1) == 1) ? TRUE : FALSE;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//	Write character 'c' into this VFile and advance the current position.
//
boolean VFile::putch(const VWideChar c)
{
	return (write(&c, 1) == 1) ? TRUE : FALSE;
}
#endif

// ---------------------------------------------------------------------------
//	Read upto 'n' bytes from this VFile into the buffer 'b'
//	and return the number of bytes actually read.  The returned 
//	value may be less than 'n' if the end-of-file was reached first, 
//	or if the end of the buffer fell in the middle of a multibyte character.  
//	A value of zero is returned if the file-open mode is WriteOnly, and 
//	a -1 is returned if there is an error.
//
integer VFile::read(char *b, integer n)
{
	integer i = 0;
	integer iEle = 0;

	asWideString(FALSE);
	if (mode() == WriteOnly || n == 0) {
		eleCount = 0;
		charCount = 0;
		return 0;
	}
	if (nEle == 0)  {
		charCount = -1; // kinda inefficient to attempt ??
#if defined(PM)
		unsigned long bytesRead;

		if (DosRead(device, b, n, &bytesRead) == 0) {
			eleCount = bytesRead;
		}
		else {
			eleCount = -1;
		}
#else
		eleCount = (integer)_sys_read(device, b, (unsigned)n);
#endif
		if (eleCount < 0) {
			SETRESULT(errno);
		}
		return getCount();
	}
	while (iEle < n) {
		if (currentEle == lengthEle) {
			offset += lengthEle;
			current = length = 0;
			currentEle = lengthEle = 0;
			byteOfCharacter = 0;
			bytesOfPrev = 0;
			if (!fill()) {
				eleCount = iEle ? iEle : -1;
				charCount = i ? i : -1;
				return getCount();
			}
		}
		VStream::read(b, n - iEle);
		if (eleCount == 0)	{
			eleCount = iEle;
			charCount = i;
			return getCount();
		}
		b += eleCount; 
		iEle += eleCount;
		i += charCount;
	}
	eleCount = iEle;
	charCount = i;
	return getCount();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//	Read upto 'n' characters from this VFile into the buffer 'b'
//	and return the number of characters actually read.  The returned 
//	value may be less than 'n' if the end-of-file was reached first.  A 
//	value of zero is returned if the file-open mode is WriteOnly, and 
//	a -1 is returned if there is an error.
//
integer VFile::read(VWideChar *b, integer n)
{
	integer i = 0;
	integer iEle = 0;

	if (mode() == WriteOnly || n == 0) {
		return 0;
	}
	if (nEle == 0)  {
		// No persistent VStream buffer is available for processing
		// the bytes of the physical file.  Create one on the fly. 
		const int char_buf_max = 1024;
		char char_buf[char_buf_max];
		int buf_current = 0; // current position in char_buf
		int buf_length = 0;  // # of bytes in char_buf
		int bytes_tried = 0; // # of bytes examined in prev. conversion that failed
		int mb_max = CV_MB_CUR_MAX;
		while (iEle < n) {
			// Of the characters requested, some have not yet
			// been supplied.  Try to supply one more character,
			// and if that's not possible at least read another
			// byte from the physical file.
			if (buf_current + bytes_tried >= buf_length) {
				// need more in the buffer
				// Shift the buffer contents to the beginning of the buffer.
				memmove(char_buf, char_buf + buf_current, buf_length - buf_current);
				buf_length -= buf_current;
				buf_current = 0;
				
				// We mustn't read too much of the file, therefore our demand
				// is limited to the number of characters yet to be read.
				// However, if not enough bytes were available on the last
				// conversion attempt, we need to ask for more than that.
				// Furthermore, we mustn't ask for more than will fit into our buffer.
				char *where = char_buf + buf_length;
				int bytes_available = char_buf_max - buf_length;
				int bytes_can_demand = bytes_tried + (n - iEle);
				int bytes_to_read = bytes_available < bytes_can_demand ? bytes_available
																	   : bytes_can_demand;
				unsigned long bytes_read;
				boolean read_failed;
#if defined(PM)
				read_failed = DosRead(device, where, bytes_to_read, &bytes_read) != 0;
#else
				{
					integer result = (integer)_sys_read(device, where, (unsigned)bytes_to_read);
					read_failed = result < 0;
					bytes_read = result;
				} 
#endif
				if (read_failed) {
					// I/O error
					charCount = -1;
					eleCount = -1;
					SETRESULT(errno);
					return getCount();
				}
				if (buf_current + bytes_tried >= buf_length + (int)bytes_read) {
					// end of file
					charCount = iEle;
					eleCount = iEle;
					return getCount();
				}
				buf_length += bytes_read; 
			}
			bytes_tried = 0;
			int bytes_to_try = buf_length - buf_current;
			int mb_count = VWideCharAux::mbtowc(b, char_buf + buf_current, bytes_to_try);
			if (mb_count >= 0) {
				b += 1;
				iEle += 1;
				buf_current += mb_count;
			}
			else if (bytes_to_try >= mb_max) {
				// Input should, but does not, translate to multibyte characters.
				// Punt.
				*b = VWideChar('?');
				b += 1;
				iEle += 1;
				buf_current += 1;
			}
			else {
				bytes_tried = bytes_to_try;
			}
		}
		eleCount = iEle;
		charCount = iEle;
		return getCount();
	}
	while (iEle < n) {
		if (currentEle == lengthEle) {
			asWideString(FALSE);
			offset += lengthEle;
			current = length = 0;
			currentEle = lengthEle = 0;
			byteOfCharacter = 0;
			bytesOfPrev = 0;
			if (!fill()) {
				eleCount = iEle ? iEle : -1;
				charCount = i ? i : -1;
				return getCount();
			}
		}
		VStream::read(b, n - iEle);
		if (eleCount == 0)	{
			eleCount = iEle;
			charCount = i;
			return getCount();
		}
		b += eleCount; 
		iEle += eleCount;
		i += charCount;
	}
	eleCount = iEle;
	charCount = i;
	return getCount();
}
#endif

// ---------------------------------------------------------------------------
//
filepos_t VFile::read(VString &s, filepos_t n)
{
	char *buf = new char[n + 1];
	if (buf == 0) {
		return 0;
	}
	filepos_t len = read(buf, n);
	if (len >= 0) {
		len = s.write(buf, len);
	}
	delete [] buf;
	return len;
}

// ---------------------------------------------------------------------------
//	Write 'n' bytes into this VFile and return the actual number
//	of bytes written.  A value of zero is returned if the file-open 
//	mode is ReadOnly, and a -1 is returned if there is an error.
//
integer VFile::write(const char *b, integer n)
{
	if (mode() == ReadOnly || n == 0) {	// no access or nothing to write
		return 0;
	}

	if (nEle == 0)  {					//	no buffer
#if defined(PM)
		unsigned long numBytesWritten;

		if (DosWrite(device,(void*) b, n, &numBytesWritten) != 0) {
			eleCount = -1;
		}
		else {
			eleCount = numBytesWritten;
		}
#else
		eleCount = (integer)_sys_write(device, (char *)b, (unsigned)n);
#endif
		if (isAsByte()) {
			charCount = eleCount;
		}
		else {
			charCount = 0;
			for (;;) {
				int ch_size = VWideCharAux::mbtowc(0, b, CV_MB_LEN_MAX);
				if (ch_size <= 0) {
					break;
				}
				b += ch_size;
				charCount += 1;
			}
		}
		if (eleCount < 0) {
			SETRESULT(errno);
		}
		return getCount();
	}

	integer i = 0;
	integer iEle = 0;

	asWideString(FALSE);
	while (iEle < n) {
//		pat--commented out 08/23/93
//		if (mode() == Update && current == 0) {
//			fill();
//		}

		if ((currentEle == nEle) && (!expandable())) {
			//	flush full, non-expandable buffer

			if (!flush()) {
				charCount = i ? i : -1;
				eleCount = iEle ? iEle : -1;
				return getCount();
			}
			offset += lengthEle; 
			current = length = 0;
			currentEle = lengthEle = 0;
			byteOfCharacter = 0;
			bytesOfPrev = 0;
		}

		VStream::write(b, n - iEle);
		b += eleCount; 
		iEle += eleCount;
		i += charCount;
		dirty(TRUE);
	}

	if (offset + lengthEle > fsize || fsize < 0) {
		fsize = lengthEle + offset;
	}
	eleCount = iEle;
	charCount = i;
	return getCount();
}

// ---------------------------------------------------------------------------
// Make VStream::write available directly for VFile objects.
//
integer VFile::write(const VString& s)
{
	return VStream::write(s);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//	Write 'n' wide characters into this VFile and return the actual number
//	of bytes written.  A value of zero is returned if the file-open 
//	mode is ReadOnly, and a -1 is returned if there is an error.
//
integer VFile::write(const VWideChar *b, integer n)
{
	if (mode() == ReadOnly || n == 0) {	// no access or nothing to write
		return 0;
	}

	integer i = 0;
	integer iEle = 0;

	if (nEle == 0)  {
		// No persistent VStream buffer is available for accumulating the
		// bytes for delivery to the physical file.  Provide one here.
		const int byte_buf_max = 1024;
		char byte_buf[byte_buf_max];
		int byte_buf_current = 0;
		int mb_max = CV_MB_CUR_MAX;
		for (;;) {
			if (iEle == n || byte_buf_current + mb_max > byte_buf_max) {
				// Flush the buffer to the file.
				unsigned long bytes_written;
				boolean write_failed;
#if defined(PM)
				write_failed = DosWrite(device, (void*)byte_buf, byte_buf_current, &bytes_written) != 0;
#else
				integer write_result = (integer)_sys_write(device, byte_buf, (unsigned)byte_buf_current);
				write_failed = write_result < 0;
				bytes_written = write_result;
#endif
				if (write_failed || bytes_written < (unsigned long)byte_buf_current) {
					// failed to write the requested bytes
					eleCount = -1;
					charCount = -1;
					return getCount();
				}
				if (iEle == n) {
					// All characters have been converted, and the buffer has
					// been flushed.
					break;
				}
				byte_buf_current = 0;
			}
			// Convert a wide character to multibyte.
			int k = VWideCharAux::wctomb(byte_buf + byte_buf_current, b[iEle]);
			if (k < 0) {
				byte_buf[byte_buf_current] = '?';
				k = 1;
			}
			iEle += 1;
			i += k;
			byte_buf_current += k; 
		}
		eleCount = iEle;
		charCount = i;
		return getCount();
	}

	while (iEle < n) {
//		pat--commented out 08/23/93
//		if (mode() == Update && current == 0) {
//			fill();
//		}

		if ((currentEle == nEle) && (!expandable())) {
			//	flush full, non-expandable buffer

			if (!flush()) {
				charCount = i ? i : -1;
				eleCount = iEle ? iEle : -1;
				return getCount();
			}
			asWideString(FALSE);
			offset += lengthEle; 
			current = length = 0;
			currentEle = lengthEle = 0;
			byteOfCharacter = 0;
			bytesOfPrev = 0;
		}

		VStream::write(b, n - iEle);
		b += eleCount; 
		iEle += eleCount;
		i += charCount;
		dirty(TRUE);
	}

	if (SINGLE_BYTE_CHAR_SET) {
		if (offset + lengthEle > fsize || fsize < 0) {
			fsize = lengthEle + offset;
		}
	}
	else if (iEle != 0) {
		fsize = -1;
	}
	eleCount = iEle;
	charCount = i;
	return getCount();
}
#endif

// ---------------------------------------------------------------------------
//	Truncate this VFile at the current position.
//
boolean VFile::truncAt()
{
	asWideString(FALSE);
	if (actualSize() <= offset + lengthEle) {
		length = getCurrent();
		lengthEle = getCurrentEle();
		return TRUE;
	}
	return truncAt(currentEle + offset);
}

// ---------------------------------------------------------------------------
//	Truncate this VFile at 'n'. This may involve copying the current
//	file to a temporary file, and thus, it may not be used with any
//	special files.
//
boolean VFile::truncAt(integer n)
{
	VFile f;

	enum fileMode m = mode();

	VPathString p;
	asWideString(FALSE);
	if (actualSize() <= offset + lengthEle) {
		lengthEle = (unsigned)(n - offset);
		length = fromElePosition(lengthEle, 0);
		if (lengthEle < currentEle) {
			current = length;
			currentEle = lengthEle;
			byteOfCharacter = 0;
			bytesOfPrev = 0;
		}
		return TRUE;
	}
	close();
	if (!f.open(Temporary)) {
		return FALSE;
	}
	p = f.fileName;
	f.open(Create);
	open(ReadOnly);
	copyFile(this, &f, n);
	close();
	f.close();
	if (!p.move(fileName)) {
		return FALSE;
	}
	if (!open(m)) {
		return FALSE;
	}
	at(n);
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Return the current position of this VFile.
//
integer VFile::at() 
{ 
	//	the curent position in the file is given by the sum of the
	//	offset from the start of the file to the first character 
	//	in the internal buffer ('offset') and the offset from the start of
	//	the internal buffer to the current position the buffer ('current').

	asWideString(FALSE);
	return currentEle + offset; 
}

// ---------------------------------------------------------------------------
// Override VStream::getPos to use byte positioning rather than character,
// because character positioning in a file is difficult and/or inefficient.
//
filepos_t VFile::getPos() const
{
	return ((VFile *)this)->at();
}

// ---------------------------------------------------------------------------
//	Return TRUE if the current position of this VFile is at
//	the end-of-file, otherwise return FALSE.
//
boolean VFile::eof() 
{ 
	asWideString(FALSE);
	return size() == offset + currentEle ? TRUE : FALSE; 
}

// ---------------------------------------------------------------------------
//	Return TRUE if the length of the file is zero, otherwise return
//	FALSE.
//
boolean VFile::empty() 
{ 
	return size() == 0 ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
//	Returns a null pointer, as files are not allowed to be broken.
//
VObject *VFile::breakAt(integer i) 
{ 
	i = 0;
	return (VObject *)i; 
}

// ---------------------------------------------------------------------------
//
VObject *VFile::breakAt() 
{ 
	return VStream::breakAt();
} 

// ---------------------------------------------------------------------------
//	Return a pointer to the drive part of the 'fileName' of this VFile.
//	This function is unique to MS DOS and OS/2 platforms and it may not 
//	portable.
//
char *VFile::driveOf()
{
#if defined(MSDOS) || defined(V_MACINTOSH) || defined(__OS2__)
	fileName.driveOf(temp);
	return (char *) temp.gets();
#else 
	return "";
#endif
}

// ---------------------------------------------------------------------------
//	Return a pointer to the directory portion of the fileName of this
//	VFile.
//
char *VFile::directoryOf()
{
	fileName.directoryOf(temp);
	return (char *) temp.gets();
}

// ---------------------------------------------------------------------------
//	Return a pointer to the file name part of the fileName of this
//	VFile.
//
char *VFile::nameOf()
{
	fileName.nameOf(temp);
	return (char *) temp.gets();
}

// ---------------------------------------------------------------------------
//	Return a pointer to the suffix part of the fileName of this
//	VFile.  The suffix part of a VPathString is any part of the filename
//	following the dot "." character.
//
char *VFile::suffixOf()
{
	fileName.suffixOf(temp);
	return (char *)  temp.gets();
}

// ---------------------------------------------------------------------------
//	Set the suffix part of the fileName of this VFile to equal the
//	Null terminated C string 's'.  Replace the previous suffix or add 
//	a dot "." plus the suffix 's' if a suffix does not currently exist.  
//	If 's' is null, remove any existing suffix.
//
void VFile::suffix(char *s)
{
	fileName.suffix(s);
}

// ---------------------------------------------------------------------------
//
void VFile::environ(char *s)
{
	if (s == 0) {
		if (env != 0) {
			delete env;		// jmd (was free)
		}
		env = 0;
	}
	else {
		if (env == 0) {
			env = new VString(s);
		}
		else {
			*env = s;
		}
	}
}

// ===========================================================================
//	helper functions

// ---------------------------------------------------------------------------
//	Copy 'size' characters from 'from' to 'to'.
//
//	Note:  		offset + current == at()
//				offset ... offset + length -1 == file data
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
//	end of helper functions
// ===========================================================================
