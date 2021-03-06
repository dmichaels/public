// $Header:   Q:/views/common/vcs/ARCHIVER.CPV   1.24   Mar 24 1997 16:07:56   scottt  $ 

//	archiver.cpp
//
//	VArchiver implementation [Common]
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
//	04/01/93 pmcm	preened
//	04/06/93 pmcm	changed %g to %lg in ::operator<<(double d)
//	04/07/93 pmcm	an archiver no longer is-a VFile; rather, it has-a VFile
//	04/07/93 pmcm	changed id to VObject *
//	04/28/93 pmcm	added getObject() with additional VObject *data argument.
//	05/04/93 pmcm	fixed get(char)/put(char) to retrieve/store '\\' 
//					in ascii mode as octal notation sequence, not byte value.
//	07/09/93 pmcm	changed char * params to const VString&;
//					changed getPathString() to file().
//	07/22/93 dgm	added final default boolean argument to getObject/From()
//					to optionally suppress realization of the (GUI) object.
//	07/30/93 pkt	eliminated #ifdef macintosh.
//	08/09/93 jmd	removed free
//	08/10/93 pmcm	added protected interface to classSet and instSet;
//					added ascii(FALSE) to init();
//					consolidated file i/o in read(), write(), putch(), 
//					getch(), getLine(), and getTrimLine().
//	08/12/93 pmcm 	made char * data args to ouput routines const.
//	08/19/93 pmcm 	added abort stuff
//	09/01/93 dgm	M8 changes.
//	10/10/93 jmd	SC6 changes.
//	10/18/93 dgm	UNIX changes.
//	11/10/93 pkt	fixed broken code in get(char *, integer).
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened NULL.
//	02/04/94 pmcm	added _currClsName; added abort message.
//	06/06/94 jld	remove VMemory::memory from get()
//	08/08/94 jmd	added begin/endWrite
//	08/05/94 dgm	Changes for VAppView archiving support.
//	08/30/94 jmd	added MANYOBJECTS support for 16bit versions
//	09/30/94 jld	get(char *& s) allocates memory iff s!=0
//	11/17/94 dgm	Fix in get(char *& s) for reading null strings (set s = 0).
//	11/29/94 dgm	Added VAppView check.
//	12/16/94 pkt	improved dgm's 11/17 fix to read null string into the
//					supplied buffer.
//	03/06/95 jld	added #ifndef CV_NOARCHIVER wrapper
//	03/27/95 jld	removed antique #ifdef CPPV21_LONGPTRPUT code
//	04/11/95 pkt	updated for internationalization.
//	04/12/95 pkt	merged Mac & NT.
//	04/14/95 pkt	fixed for VDIRECTNEWDELETE.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	fixed getObject for VAppViews (broken in 5/19 merge).
//	05/26/95 pkt	combined international changes into common version.
//	07/13/95 pkt	fixed 5/26 merge problem: VAppViewCls not declared.
//  08/11/95 evc    speeded up put(const char *, int)  for ascii archives
//	11/08/95 dss	added {put,get}Self(), objAt().
//	11/10/95 dgm	Minor change for Borland C++ in archSelf().
//	12/22/95 tlf	Minor fixes for calls to getTrimLine.
//	04/04/96 pkt	changed 'getObject' to check for abort before returning.
//	05/08/96 dgm	Added putObjectWithCount(), getObjectWithCount(),
//					getObjectMemoryWithCount(), skipObjectWithCount().
//	06/18/96 evc	Replaced  ULONG_MAX with 1111111111  to fix
//					odd problem.
//	07/30/96 pkt	changed %lg to %.15g in ::put(double d).
//	07/30/96 pkt	added check for I/O error to set abort flag.
//	08/20/96 pkt	added Win16 support for containers larger than 64K.
//	08/21/96 pkt	fixed yesterday's change.
//	11/06/96 pkt	revised to work with no notifier.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "archiver.h"
#include "cvassert.h"
#include "dictinry.h"
#include "ordcllct.h"
#include "intassoc.h"
#if defined(CV_WINDOWS)
# include "memry.h"
#endif

extern "C" {
#	include <ctype.h>
#	include <stdio.h>
#	include <string.h>
}

// ---------------------------------------------------------------------------
//
defineClass(VArchiver, VObject)

#ifndef CV_NO_NOTIFIER
// ---------------------------------------------------------------------------
//
extern VClass *VAppViewCls;
#endif

// ---------------------------------------------------------------------------
//
static boolean translateAccess(int access, fileMode *fmode); // helper

// ---------------------------------------------------------------------------
//	Construct an archiver that is not bound to any file.
//
//	The memoryArchiveOk argument defaults to TRUE.  It is provided for 
//	derived classes that do not support in-memory archives.  Such classes
//	can use the VArchiver default constructor without creating a VFile.
//
VArchiver::VArchiver(boolean memoryArchiveOk)
			: 	instSet(0),
				classSet(0),
				instIndex(0L),
				_currClsName(""),
				classIndex(0L)
{
	init("", memoryArchiveOk);
}

// ---------------------------------------------------------------------------
//	Construct an archiver that is bound to the given file.
//
//	The memoryArchiveOk argument defaults to TRUE.  It is provided for 
//	derived classes that do not support in-memory archives.  Such classes
//	can use the VArchiver default constructor without creating a VFile.
//
//	You may open the file later, with VFile::open(enum fileMode m).  
//	Bind the archiver to another file with VFile::close(), 
//	VFile::open(enum fileMode m, const VString& fname).
//
VArchiver::VArchiver(const VString& fname, boolean memoryArchiveOk) 
			: 	instSet(0),
				classSet(0),
				instIndex(0L),
				_currClsName(""),
				classIndex(0L)
{ 
	init(fname, memoryArchiveOk);
}

// ---------------------------------------------------------------------------
//	Close the archive file before destroying this archiver.
//
VArchiver::~VArchiver()
{
	if (isOpen()) {
		close();
	}
}

// ---------------------------------------------------------------------------
//	Common initializer for the constructors.
//
void VArchiver::init(const VString& fname, boolean memoryArchiveOk) 
{
	closeSets();

	ascii(FALSE);
   	isOpen(FALSE);
   	isMemoryArchive(FALSE);
   	abort(NoAbort);
	displayAbortMessage(TRUE);

	if (fname.size() == 0 && memoryArchiveOk) {
		//	open an archive up in memory  

	   	isOpen(TRUE);
   		isMemoryArchive(TRUE);
	}
	else {
		archiveFile.file(fname);
	}

	//	archive is always expandable
 	archiveFile.expandable(TRUE);

#ifdef CV_ARCHIVE_MANYOBJECTS
	setsAsDict = FALSE;
#endif

	objFilePosition = -1;
}

// ---------------------------------------------------------------------------
//	Close the archive file
//
void VArchiver::close()
{
	closeSets();
	archiveFile.close();
   	isOpen(FALSE);
	isMemoryArchive(FALSE);
	abort(NoAbort);
}

// ---------------------------------------------------------------------------
//	Free the class and instance sets
//
void VArchiver::closeSets()
{
	if (instSet)  {

#ifdef CV_ARCHIVE_MANYOBJECTS
		instSet->freeContents();
		delete instSet;
#else
		delete instSet;
#endif
		instSet = 0;
	}
	instIndex = 0L;
	if (classSet)  {
		delete classSet;	// jmd (was free)
		classSet = 0L;
	}
	classIndex = 0L;
}

// ---------------------------------------------------------------------------
//	Rewind the archive to the beginning
//
void VArchiver::rewind()
{
	closeSets();
	if (isOpen()) {
	 	archiveFile.at(0);
	}
}

// ---------------------------------------------------------------------------
//	Open the archive file to which this archiver is bound with the given 
//	mode of access.
//
//	The access mode should be one of VArchiver::AccessType
//
//	Return TRUE, if able to open the archive; FALSE, if not.
//
boolean VArchiver::open(int access)
{
	if (isOpen()) {
		close();
	}
	enum fileMode fmode;

	if (translateAccess(access, &fmode)) {
		isOpen(archiveFile.open(fmode));
	}
	return isOpen();
}

// ---------------------------------------------------------------------------
//	Bind the archiver to the named file and open the file with the given 
//	mode of access.
//				 
//	The access mode should be one of VArchiver::AccessType
//
//	Return TRUE, if able to open the archive; FALSE, if not.
//
boolean VArchiver::open(int access, const VString& fname)
{
	if (isOpen()) {
		close();
	}
	enum fileMode fmode;

	if (translateAccess(access, &fmode)) {
		isOpen(archiveFile.open(fmode, fname.gets()));
	}
	return isOpen();
}

// ---------------------------------------------------------------------------
//	Retrieve an object from the archive.
//
//	If the object holds other objects, they will be loaded as well.  
//	If obj is 0, then a new object of the type being retrieved 
//	will be created; otherwise, obj will be assumed to be an object that 
//	is already constructed to contain the retrieved object data.
//
//	Returns a pointer to the object retrieved; 0, if unable to retrieve
//	the object.
//
VObject *VArchiver::getObject(VObject *obj)
{
	return getObject(obj, 0);
}

// ---------------------------------------------------------------------------
//	Retrieve an object (and its constituent objects) from the archive.
//
//	If obj is 0, then a new object of the type being retrieved 
//	will be created; otherwise, obj will be assumed to be an object that 
//	is already constructed to contain the retrieved object data.
//
//	If data is non-0, then data is passed to the objects 
//	getFrom() method.
//	
//	Returns a pointer to the object retrieved; 0, if unable to retrieve
//	the object.
//
VObject *VArchiver::getObject(VObject *obj, VObject *data, boolean do_realize)
{
	if (!isOpen() || isAbort()) {
	 	return 0;
	}

	VClass *cls = 0;
	char p[51];
	int i;

	openSets(FALSE);

	do  {
		switch (getch()) {

		case '0':			//	null object
			return 0;

		case 'A':			//	the following object is in ASCII form
			ascii(TRUE);
			break;

		case 'B':			//	the following object is in BINARY form
			ascii(FALSE);
			break;

		case 'O':			//	index to a previous object definition
			*this >> i;
			return instanceAt(i);

		case 'c':			//	class definition (name of the meta-class)
			getTrimLine(p, 50);
			setCurrentClassName(p);
			if ((cls = VClass::of(p)) == 0) {
				abort(UnknownClassName);
				return 0;
			}
			(void) addClass(cls);	
			break;

		case 'C':			//	index to a previous class definition
			*this >> i;
			if ((cls = classAt(i)) == 0) {
				error("VArchiver::getFrom():  Bad class index.");
			}
			setCurrentClassName(cls->nameOf());
			break;

		default:
			return 0;

		}	//	end of character switch

	} while (cls == 0);

 	//	object definition

	char c = getch();
	if (c == 'o' || c == 't') {

		if (obj == 0) {	
			obj = (VObject *)cls->construct();
		}
		if (obj != 0) {
			if (c != 't') {
				(void) addInstance(obj);
			}

			if (data != 0) {
				obj->getFrom(*this, data, do_realize);
			}
#ifndef CV_NO_NOTIFIER
			else if (obj->isA(VAppViewCls)) {
				obj->getFrom(*this, 0, do_realize);
			}
#endif
			else {
				obj->getFrom(*this);
			}
			if (isAbort()) {
				// Assume 'obj' is corrupt; don't return it!
				instSet->remove(obj);
				delete obj;
				obj = 0;
			}
		}
	}
	setCurrentClassName(0);
	return obj;
}

// ---------------------------------------------------------------------------
//	Write the object obj to the archive file.  If the object
//	contains other objects, they are also written.  If temp is TRUE, 
//	then obj is assumed to be constructed on the stack and therefore
//	temporary. The default value for temp is FALSE.
//
//	the entry for a null object is:
//
//	1) 	'0'						the digit zero denotes a null object
//
//	the entry for a non-null object has 3 parts in this order:
//
//	1) 	'A' or 'B'				'A' for ascii, 'B' for binary archive data
//	2)	'c' class-name			lower-case letter 'c' denotes a class
//								name follows
//		or,
//
//		'C' class-index			upper-case letter 'C' denotes a class
//								by indexed reference to a previous class entry
//
//	3)	'o' object-definition	lower-case letter 'o' denotes an object
//								definition follows
//		or,
//		
//		't' object-definition	lower-case letter 't' denotes an object
//								definition follows but the object defined 
//								is a temporary object.  temporary objects 
//								are not entered in the instance set. 
//								subsequent archival of the same instance
//								does not benefit from instance indexing and
//								results in another complete object definition.
//								
//		or,
//	
//		'O' instance-index		upper-case letter 'O' denotes an object by
//								indexed reference to a previous object 
//								definition
//			
//	Note that when multiples of a particular non-temporary object or of a
//	class of objects are archived, only one definition of that object or 
//	name for that class of objects is actually stored.  The first 
//	occurence of a particular object in the archive is accompanied by 
//	its definition;	the first occurence of a class, by its class name.  
//	Subsequent occurences are denoted in the archive by an instance or 
//	class index to the first occurence.  The instance and class indices 
//	are actually sequence numbers corresponding to the order in which 
//	objects or classes are defined in the archive.  These indices rely on 
//	the fact that objects are always read from the archive sequentially, 
//	in the order in which they were written.
//
//	When multiples of a temporary object are archived, multiple definitions
//	are stored, one for each occurence of the object in the archive.
//	Temporary objects are not entered in the instance set.
//
boolean VArchiver::putObject(VObject *obj, boolean temp)
{
	if (!isOpen() || isAbort()) {
	 	return 0;
	}
	openSets(TRUE);

	if (obj == 0) {
		//	null objects are represented by the digit 0.
		putch('0');
		return TRUE;
	}
	beginWrite();		// jmd 08/08/94

	putch(ascii() ? 'A': 'B');

	integer i;
	VClass *cls;

	if (!temp) {
	 	temp = obj->isTemporaryObject();
	}

	if (!temp && (i = indexOf(obj)) > 0) {

		//	not the first occurence in the archive of a particular 
		//	non-temporary object.
		//	save only the instance index to the first occurence.

		putch('O');
		*this << i - 1L;
	}
	else {
		if (!temp)  {
			//	the first occurence in the archive of a particular 
			//	non-temporary object.  add it to the instance set.
			//
			(void) addIndexedInstance(obj);
		}

		//	class information

		cls = obj->getIsA();
		if ((i = indexOf(cls)) > 0L) {
			//	not the first occurence in the archive of a particular class.
			//	save only the class index to the first occurence.

			putch('C');
			*this << i - 1L;
		}
		else {
			//	first occurrence of a particular class in the archive.
			//	add it to the class set and save the name of the class.
			//
			(void) addIndexedClass(cls);

			putch('c');
			putLine(obj->clsName());
		}

		//	write object instance information

		putch(temp ? 't' : 'o');
		setCurrentClassName(obj->clsName());
		obj->putTo(*this);
		setCurrentClassName(0);
	} 

	endWrite();		// jmd 08/08/94

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Read a long integer from the archive.
//
void VArchiver::get(long& l)
{
	if (isOpen()) {
		if (!ascii()) {
			read((char *) &l, (unsigned) sizeof(long));
		}
		else {
			// char b[21];
			// getTrimLine(b, 20);
			char b[31];
			getTrimLine(b, 30);
			l = atol(b);
		}
	}
}

// ---------------------------------------------------------------------------
//	Read a double from the archive.
//
void VArchiver::get(double& d)
{
	if (isOpen()) {
		if (!ascii()) {
			read((char *) &d, (unsigned) sizeof(double));
		}
		else {
			char b[101];
			getTrimLine(b, 100);
			d = atof(b);
		}
	}
}

// ---------------------------------------------------------------------------
//	Read a character from the archive.
//
void VArchiver::get(char& c)
{
	if (isOpen()) {
		int i = getch();
			
		if (ascii() && ((char) i == '\\')) {
			//	a '\\' in ascii mode denotes either a non-printable 
			//	or the '\\' character itself, stored in octal notation

			i  = (getch() & 07) << 6;
			i += (getch() & 07) << 3;
			i += (getch() & 07);
		}
		c = i;
	}
}

// ---------------------------------------------------------------------------
//	Read a string from the archive.  The string is null-terminated.
//						
void VArchiver::get(char *& s)
{
	if (isOpen()) {
		int i;
		*this >> i;
		if (i != 0) {
			if (s == 0) {
				s = new char[i + 1];
			}
			read(s, i + 1);
		}
		else {
			if (s != 0) {
				s[0] = '\0';
			}
		}
	}
}

// ---------------------------------------------------------------------------
//	Read 'n' bytes into 'buf' from the archive file.  
//
void VArchiver::get(char *buf, integer n)
{
	if (isOpen() && n > 0L) {
		if (!ascii()) {
			read(buf, n);
			return;
		}

		char *bufp = buf;
		while(n-- > 0) {
			*this >> *bufp++;
		}
	}
}

// ---------------------------------------------------------------------------
//	Read 'n' elements into the `VContainer` 'c' from this archiver.
//
void VArchiver::get(VContainer *c, integer n)
{
	if (isOpen()) {
		if (c->size() < n) {
			c->expandTo(n);
		}
		pointer p = c->getData();
		integer n_bytes = n * c->sizeOfEle();
#ifdef CV_WIN16
		if (c->isHuge()) {
			// 'get' the bytes into a small enough buffer.
			hugePointer hp = hugePointer(p);

			while (n_bytes > 0) {
				// There remain 'n_bytes' bytes to write, starting at hp.
				const integer bufsize = 1024;
				char buf[bufsize];

				integer buf_count = n_bytes < bufsize ? n_bytes : bufsize;
				get(buf, buf_count);
				for (integer i = 0; i < buf_count; i += 1) {
					*hp = buf[i];
					hp += 1;
					n_bytes -= 1;
				}
			}
		}
		else {
			get(p, n_bytes);
		}
#else
		get(p, n_bytes);
#endif
	}
}

// ---------------------------------------------------------------------------
//	Write a long integer to the archive.
//
//	In ASCII mode, write the long integer to the archive as a string of
//	of digits, followed by a newline.  ASCII is portable.
//
//	In BINARY mode, write the long integer as a sequence of bytes.  
//	BINARY may not be portable due to the platform differences in regard 
//	to the number and order of bytes in a integer.
//
void VArchiver::put(long l)
{
	if (isOpen()) {
		if (!ascii()) {
			write((char *) &l, (unsigned) sizeof(long));
		}
		else {
			// char b[21];
			char b[31];
			sprintf(b, "%ld", l);
			putLine(b);
		}
	}
}

// ---------------------------------------------------------------------------
//	Write a double to the archive.
//
//	In ASCII mode, write a double to the archive as a string followed by a 
//	newline.  ASCII is portable.
//
//	In BINARY mode, write a double as a sequence of bytes.  
//	BINARY may not be portable due to the platform differences in regard 
//	to the internal representation of doubles.
//
void VArchiver::put(double d)
{
	if (isOpen()) {
		if (!ascii()) {
			write((char *) &d, (unsigned) sizeof(double));
		}
		else {
			char b[101];
			sprintf(b, "%.15g", d);
			putLine(b);
		}
	}
}

// ---------------------------------------------------------------------------
//	Write a character to the archive.
//
//	A printable character, under either ASCII or BINARY mode, is written to
//	the archive as a byte (all eight bits are significant).  A non-printable 
//	character under BINARY mode is also stored as a byte.
//
//	Under ASCII mode, a non-printable character, or the '\\' character
//	is stored as a sequence of characters denoting the value of the 
//	given character in octal notation.  For example, the value of 
//	the non-printable line-feed character (LF) in the ASCII character 
//	set is 10 (0x0A).  So, a LF is stored in the archive as a 
//	sequence of 4 characters: \012.
//	
void VArchiver::put(char c)
{
	if (isOpen()) {
		if (ascii() && (!isprint(c) || (c == '\\'))) {
			//	non-printables and '\\' in ascii mode, as octal notation

			putch('\\');
			putch(((c >> 6) & 07) + '0');
			putch(((c >> 3) & 07) + '0');
			putch((c & 07)+ '0');
		}
		else {
			//	printables, and non-printables in binary mode, as bytes

			putch(c);
		}
	}		  
}

// ---------------------------------------------------------------------------
//	Write a null-terminated C string to the archive.
//
//  null or empty strings are stored as
//		0
//	non-null strings are stored as 
//		the length of the string, followed by
//		the string (including its terminating '\0')
//
void VArchiver::put(const char *s)
{
	if (isOpen()) {
		int i = (s == 0) ? 0 : strlen(s);
		(*this) << i;
		if (i) {
			write(s, i + 1);
		}
	}
}

// ---------------------------------------------------------------------------
//	Writes the first 'n' bytes of 'buf' to the archive.
//
//	In BINARY mode, it writes the buffer to the archiver as a sequence 
//	of bytes.  If 'n' is zero, then nothing is written.
//
//	In ASCII mode, it writes all printable characters (excepting '\\') 
//	as bytes; for non-printables and the '\\' character, the value of
//	a character is stored in octal notation (see put(char c)).
//
void VArchiver::put(const char *buf, integer n)
{
	if (isOpen()) {
		if (!ascii()) {
			if (n > 0L) {
				write(buf, n);
			}
		}
		else {
			VStream S(n*2);
			int newlen = 
			makeAscii(buf, n, S);
			write(S.gets(), newlen);
		}
	}
}
	
// ---------------------------------------------------------------------------
//	Write the first 'n' elements of the `VContainer` 'c' to the archive.
//
void VArchiver::put(VContainer *c, integer n)
{
	integer n_bytes = n * c->sizeOfEle();
#ifdef CV_WIN16
	if (c->isHuge()) {
		// Buffer the bytes into an array that can be 'put'.
		hugePointer p = hugePointer(c->getData());

		while (n_bytes > 0) {
			// There remain 'n_bytes' bytes, starting at p.
			const integer bufsize = 1024;
			char buf[bufsize];

			integer buf_count = n_bytes < bufsize ? n_bytes : bufsize;
			for (integer i = 0; i < buf_count; i += 1) {
				buf[i] = *p;
				p += 1;
				n_bytes -= 1;
			}
			put(buf, buf_count);
		}
	}
	else {
		put(c->getData(), n_bytes);
	}
#else
	put(c->getData(), n_bytes);
#endif
}

// ---------------------------------------------------------------------------
boolean VArchiver::putSelf(VObject& obj, ArchSelf archSelf)
{
	objFilePosition = getFile().at();
	objCurrentPosition = 0;
	
	archSelf(obj, *this, &VArchiver::objAt, &VArchiver::objAt);
	
	return TRUE;
}

// ---------------------------------------------------------------------------
// rhw 4/15/96: Changed the name of the second parameter to be
// more reflective of what it does since I was going to put it
// to use
VObject *VArchiver::getSelf(VObject& obj, ArchSelf restoreSelf)
{
	objFilePosition = getFile().at();
	objCurrentPosition = 0;
// rhw 4/15/96: Commented out the assert since I gave it an implementation
//	cv_assert(0); // not implemented yet
	// rhw 4/15/96: Added this line of code to propagate the meaning of
	// method to object.  David left this out.
	restoreSelf(obj, *this, &VArchiver::objAt, &VArchiver::objAt);
	
// rhw 4/15/96: Commented out old return and added the correct one
//	return 0;
	return &obj;
}

// ---------------------------------------------------------------------------
filepos_t VArchiver::objAt(filepos_t pos)
{
	cv_assert(objFilePosition != -1);
	
	if (pos < 0) {
		return -1;
	}

	// position relative to file	
	getFile().at(pos + objFilePosition);
	
	// return position relative to object
	return pos;
}

// ---------------------------------------------------------------------------
//
// Objects are composed of items written with putTo().
//
// Returns the item file position by computing the difference of the current
// file position and the file position of the start of the object.
filepos_t VArchiver::objAt(void)
{
	cv_assert(objFilePosition != -1);
	
	// return position relative to object
	return getFile().at() - objFilePosition;
}

// ---------------------------------------------------------------------------
//	Set a pointer to the in-memory archive's buffer and tell the size of 
//	the buffer.  Buffer access is read-only.  
//
//	Returns FALSE, if the archive is not an in-memory archive (the pointer and
//	buffer length return parameter values are bogus, then); TRUE, otherwise.
//
//	Note:
//
//		This method accomodates code written for the old 
//		VArchiver.  Since the old VArchiver was derived from VFile, 
//		some old code (like	VDde::idToDat()) relied upon the ability 
//		to read the buffer of the VFile directly, with the getData() 
//		method.  It also relied upon the fact that the size of the 
//		buffer reported by VFile was really one less than the actual 
//		size.  This code does the same.
//
// 		We discourage the use of getMemoryArchiveBuffer() and 
//		recommend using get() and put(), instead.
//
boolean	VArchiver::getMemoryArchiveBuffer(const char *& buf, unsigned long& len)
{
	if (isMemoryArchive()) {
		buf = (char *) archiveFile.gets();
		len = (unsigned long) archiveFile.size();
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
void VArchiver::abort(AbortReason r, const char *msg) 
{
	_abortReason = r;
	if (_abortReason == NoAbort || !displayAbortMessage()) {
		return;
	}

	VString s(msg);

	if (s.size() == 0) {	//	set default message
		const char *clsName = getCurrentClassName().gets();
		switch (r) {
		default:
	  	case ClassSpecific:
			s.prints("%s: Archival error.", clsName);
			break;
		case UnknownClassName:
			s.prints("VArchiver: The class %s is unknown.", clsName);
			break;
		case BadTag:
			s.prints("%s::getFrom(): Archived object data not in expected position.", clsName);
			break;
		case NewArchiveObject:
			s.prints("%s::getFrom(): Old class code, cannot read new %s object from archive.", clsName, clsName);
			break;
		case OldArchiveObject:
			s.prints("%s::getFrom(): Old %s object in archive, not supported.", clsName, clsName);
			break;
		case ClientCallbackMismatch:
			s.prints("%s::getFrom(): Client does not contain specified callback.", clsName);
			break;
		case WriteFailure:
			s.prints("%s::putTo(): I/O error while writing object to archive.", clsName);
			break;
		}
	}
	error(s.gets());
}

// ---------------------------------------------------------------------------
//	This helper function is used internally to check access modes specified to
//	open(int) and open(int, char *).  It interprets the access mode as a
//	VArchiver::AccessType and converts its value to one of enum fileMode.
//
//	Returns TRUE and *fmode contains the translation, if the access mode is
//	a VArchiver::AccessType; FALSE and *fmode is bogus, if not.
//
static boolean translateAccess(int access, enum fileMode *fmode)
{
	switch ((VArchiver::AccessType) access) {

	case VArchiver::ReadOnly:
		*fmode = ::ReadOnly;
		break;
		
   	case VArchiver::Create:	
		*fmode = ::Create;
		break;

	case VArchiver::WriteOnly:
		*fmode = ::WriteOnly;
		break;

	case VArchiver::Update:
		*fmode = ::Update;
		break;

	case VArchiver::Temporary:
		*fmode = ::Temporary;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
integer VArchiver::indxOf(VCollection *collect, VObject *obj)
{
	if (collect == 0) {
		return -1L;
	}
	return (long) ((VDictionary *) collect)->getValueAtKey(obj);
}

// ---------------------------------------------------------------------------
//
VObject *VArchiver::objAt(VCollection *collect, integer i)
{
	if (collect == 0) {
		return 0;
	}
	return ((VOrdCollect *) collect)->idAt(i);
}

// ---------------------------------------------------------------------------
//
boolean VArchiver::addObj(VCollection *collect, VObject *obj)
{
	if (collect == 0) {
		return 0;
	}
	return ((VOrdCollect *) collect)->add(obj) == 0 ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VArchiver::addIndexedObj(VCollection *collect, VObject *obj, integer& i)
{
	if (collect == 0) {
		return FALSE;
	}
	return ((VDictionary *) collect)->atKeyPut(obj, ++i);
}

// ---------------------------------------------------------------------------
//
boolean	VArchiver::openSets(boolean asDictionaries)
{
	if (classSet == 0) {
		if (asDictionaries) {
			classSet	= new VDictionary(VIntAssocCls);
#ifdef CV_ARCHIVE_MANYOBJECTS
			// instSet is an VOrdCollect of VDictionary's
			instSet 	= new VOrdCollect(1);
			instSet->add(new VDictionary(VIntAssocCls));

			setsAsDict  = TRUE;
			instIndex = 0L;
#else
			instSet 	= new VDictionary(VIntAssocCls);
#endif
		}
		else {
			classSet 	= new VOrdCollect(50);
#ifdef CV_ARCHIVE_MANYOBJECTS
			// instSet is an VOrdCollect of VOrdCollect's
			instSet 	= new VOrdCollect(1);
			instSet->add(new VOrdCollect(200));
			setsAsDict  = FALSE;
			instIndex = 0L;
#else
			instSet 	= new VOrdCollect(200);
#endif
		}
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
integer VArchiver::write(const char *buf, integer n)
{
	if (!isAbort() && isOpen()) {
	 	integer count = archiveFile.write(buf, n);
		if (count < n) {
			abort(WriteFailure);
		}
		return count;
	}
	return -1L;
}

// ---------------------------------------------------------------------------
//
integer VArchiver::read(char *buf, integer n)
{
	if (!isAbort() && isOpen()) {
	 	return archiveFile.read(buf, n);
	}
	return -1L;
}

// ---------------------------------------------------------------------------
//
boolean VArchiver::putch(const char c)
{
	if (!isAbort() && isOpen()) {
	 	boolean ok = archiveFile.putch(c);
		if (!ok) {
			abort(WriteFailure);
		}
	 	return ok;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
char VArchiver::getch()
{
	if (!isAbort() && isOpen()) {
	 	return archiveFile.getch();
	}
	return -1;
}

// ---------------------------------------------------------------------------
//
integer	VArchiver::putLine(const char * buf)
{
	if (buf == 0) {
		return 0;
	}
	if (!isAbort() && isOpen()) {
		// Figure the number of chars that should be written.
		size_t len = strlen(buf);
		integer count = archiveFile.putln(buf);
		if (count < integer(len)) {
			abort(WriteFailure);
		}
	 	return count;
	}
	return -1L;
}

// ---------------------------------------------------------------------------
//
integer	VArchiver::getTrimLine(char *buf, integer maxBytes)
{
	if (!isAbort() && isOpen()) {
	 	return archiveFile.gettrim(buf, maxBytes);
	}
 	return -1L;
}

// ---------------------------------------------------------------------------
// Called at the start of a putObject operation
//
void VArchiver::beginWrite()
{
	;
}

// ---------------------------------------------------------------------------
// Called at the end of a putObject operation
//
void VArchiver::endWrite()
{
	;
}

// ---------------------------------------------------------------------------
// Construct a destination stream that contains a buffer with n characters
// converted into an Views "ascii" representation. Return the number of 
// characters in the destination.  
// Used by put(const char *, int) to speed up write of large buffers 
// to ascii archives.

int VArchiver::makeAscii(const char * buf, int n, VStream & dest){

	while(n-- > 0) {
		const char c = *buf++;
		if ((!isprint(c) || (c == '\\'))) {
		//  non-printables and '\\' in ascii mode, as octal notation
			dest.putch('\\');
			dest.putch( (char) (((c >> 6) & 07) + '0'));
			dest.putch( (char)(((c >> 3) & 07) + '0'));
			dest.putch((char) ((c & 07)+ '0'));
		} else {
			dest.putch(c);
		}
	}
	return dest.getCharSize();
}

#include "rscarch.h"
#include "blockfil.h"

// ---------------------------------------------------------------------------
//
boolean VArchiver::putObjectWithCount(VObject *obj, boolean temp)
{
	if (!isOpen() || isAbort()) {
		return FALSE;
	}
	unsigned long count_position, start_position, end_position;
	VRscArchiver *ra = isA(VRscArchiverCls) ? (VRscArchiver *)this : 0;
	if (ra != 0) {
		count_position = ra->getFile().getFile().at();
	}
	else {
		count_position = getFile().at();
	}
	*this << (unsigned long)1111111111;
	if (ra != 0) {
		start_position = ra->getFile().getFile().at();
	}
	else {
		start_position = getFile().at();
	}
	putObject(obj, temp);
	if (ra != 0) {
		end_position = ra->getFile().getFile().at();
	}
	else {
		end_position = getFile().at();
	}
	if (ra != 0) {
		ra->getFile().getFile().at(count_position);
	}
	else {
		getFile().at(count_position);
	}
	VString s; s.prints("%010d", end_position - start_position);
	integer sz = s.getCharSize();
	integer written = 0;
	if (ra != 0) {
		written = ra->getFile().getFile().write(s.gets(), sz);
		ra->getFile().getFile().at(end_position);
	}
	else {
		written = getFile().write(s.gets(), sz);
		getFile().at(end_position);
	}
	if (written != sz) {
		abort(WriteFailure);
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
VObject *VArchiver::getObjectWithCount(VObject *obj,
									   VObject *data, boolean do_realize,
									   unsigned long *count)
{
	unsigned long l;
	*this >> l;
	if (count != 0) {
		*count = l;
	}
	return l != 0 ? getObject(obj, data, do_realize) : 0;
}

// ---------------------------------------------------------------------------
//
char *VArchiver::getObjectMemoryWithCount(unsigned long *count)
{
	unsigned long l;
	*this >> l;
	if (count != 0) {
		*count = l;
	}
	if (l > 0) {
		char *buffer = new char [l];
		if (buffer != 0) {
			read(buffer, l);
			return buffer;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
//
unsigned long VArchiver::skipObjectWithCount(VObject *data)
{
	unsigned long l;
	*this >> l;
	if (l > 0) {
		VObject *object = getObject(0, data, FALSE);
		if (object != 0) {
			delete object;
		}
	}
	return l;
}
