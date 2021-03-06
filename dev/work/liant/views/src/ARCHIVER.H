// $Header:   Q:/views/common/vcs/ARCHIVER.H_V   1.14   Mar 24 1997 16:07:58   scottt  $ 

//	archiver.h
//
//	VArchiver class interface [Common]
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
//	04/02/93 pmcm	preened
//	04/07/93 pmcm	an archiver no longer is-a VFile; rather, it has-a VFile
//	04/07/93 pmcm	removed protected putObj(boolean, VObject *)
//	04/07/93 pmcm	changed id to VObject *
//	04/28/93 pmcm	added getObject() with VObject *data argument
//	06/11/93 pmcm	added reference to VPathString class;
//					changed getFileName() to getPathString().
//	07/09/93 pmcm	changed char * params to const VString&;
//					changed getPathString() to file();
//					changed VFile * member to VFile.
//	07/22/93 dgm	added final default boolean argument to getObject()
//					to optionally suppress realization of the (GUI) object.
//	08/09/93 jmd	removed free
//  08/10/93 pmcm	added protected interface to classSet and instSet
//	08/10/93 pmcm	added virtual read(), write(), putch(), getch();
//					setLineTerminator(), getLineTerminator, putLine(),
//					and getTrimLine() for smaller derivation.
//	08/12/93 pmcm 	made char * data args to ouput routines const.
//	08/19/93 pmcm 	added abort stuff
//	09/16/93 dgm 	changed enumerator values of VArchiver::AccessType to
//					coincide with the corresponding global enumerators in
//					file.h.
//	09/22/93 pmcm	included archive tag header
//	11/01/93 pkt	interchanged some inline definitions to prevent cfront
//					diagnostics about used before inlined.
//	01/13/94 dgm	merged and preened
//	02/02/94 pmcm	added enum AbortReason
//  02/09/94 pmcm	remove abortBit, added _abortReason and abortMsgBit;
//					added message argument to abort()
//	08/08/94 jmd	added begin/endWrite
//	08/30/94 jmd	added MANYOBJECTS support for 16bit version
//	03/06/95 jld	added #ifndef CV_NOARCHIVER wrapper
//	04/12/95 pkt	made 'setCurrentClassName' parameter const.
//	05/19/95 pkt	selected Windows as common source.
//	05/26/95 pkt	combined international changes into common version.
//  08/11/95 evc	added function makeAscii
//	11/08/95 dss	added {put,get}Self(), objAt().
//	05/08/96 dgm	Added putObjectWithCount(), getObjectWithCount(),
//					getObjectMemoryWithCount(), skipObjectWithCount().
//	07/30/96 pkt	added WriteFailure as new AbortReason.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
//	01/24/97 dgm	Made getFile() public (for VBitMap).
// ---------------------------------------------------------------------------

#ifndef VARCHIVER_H
#define VARCHIVER_H

// ---------------------------------------------------------------------------
//
#include "file.h"

#if defined(CV_WIN32) || !defined(CV_WINDOWS)
//  only needed for 16 bit os
#	undef CV_ARCHIVE_MANYOBJECTS
#endif

#ifdef CV_ARCHIVE_MANYOBJECTS
#	include "dictinry.h"
#	include "ordcllct.h"
#	include "intassoc.h"
#endif

// ---------------------------------------------------------------------------
//
CLASS VCollection;
CLASS VContainer;
CLASS VPathString;

// ---------------------------------------------------------------------------
//	The VArchiver class implements object persistance.  Its member
//	functions allow for the storage and retrieval of the data comprising an 
//	object to and from an VArchiver file.  In order to be archivable, a class 
//	must be a VObject and override the putTo() and getFrom() virtual 
//	functions.  Respond to putTo() by shifting each member variable 
//	into the VArchiver object.  A member which is an instance of some 
//	class is archived through the putObject() nethod.  Respond to getFrom() 
//	by right-shifting from the VArchiver object into the member variable or 
//	by invoking getObject().
//	
//	Archival can be ASCII or BINARY.  With ASCII archival, the data
//	comprising an object is written in a machine independant form. With 
//	BINARY	archival the data is written in the most expediant form.

CLASS VArchiver : public VObject {

public:

	typedef filepos_t (VArchiver::*AtMethodSetType)(filepos_t);
	typedef filepos_t (VArchiver::*AtMethodGetType)(void);
	typedef void (*ArchSelf)(VObject&, VArchiver&, AtMethodGetType, AtMethodSetType);

	enum AccessType 	{ 	Create		= ::Create,
						  	ReadOnly	= ::ReadOnly,
						  	WriteOnly	= ::WriteOnly,
						  	Update		= ::Update,
						  	Temporary	= ::Temporary
						};

	enum AbortReason	{ 	NoAbort,
							UnknownClassName,
							BadTag,
							NewArchiveObject,
							OldArchiveObject,
							ClientCallbackMismatch,
						  	ClassSpecific,
							WriteFailure
						};

#ifdef CV_ARCHIVE_MANYOBJECTS
	// maximum size of each instance set
	enum				{ SetSize = 4000 };
#endif

	//	Constructors/Destructor

						VArchiver(	boolean memoryArchiveOk = TRUE);

						VArchiver(	const VString& fname, 
									boolean memoryArchiveOk = TRUE);

						~VArchiver();

	VClass 	   		   *iam();

	//	Opening and closing the archive

	virtual boolean 	open(int access);
	virtual boolean 	open(int access, const VString& fname);
	virtual void 		close();

	//	Positioning

	virtual void		rewind();

	//	VObject archival and retrieval

	VArchiver& 			operator << (VObject *obj);
	VArchiver& 			operator >> (VObject *& obj);

	virtual boolean 	putObject(VObject *obj, boolean temp = FALSE);
	virtual VObject    *getObject(VObject *obj);
	virtual VObject    *getObject(VObject *obj, VObject *data,
								  boolean do_realize = TRUE);

	virtual boolean 	putSelf(VObject&, ArchSelf);
	virtual VObject    *getSelf(VObject&, ArchSelf);

	boolean			 	putObjectWithCount(VObject *,
										   boolean = FALSE);
	VObject			   *getObjectWithCount(VObject *,
										   VObject * = 0,
										   boolean = FALSE,
										   unsigned long * = 0);
	char			   *getObjectMemoryWithCount(unsigned long * = 0);
	unsigned long		skipObjectWithCount(VObject * = 0);

	//	Data archival and retrieval

	virtual void	put(char c);
	virtual void 	get(char& c);
	virtual void	put(const char *s);
	virtual void 	get(char *& s);
	virtual void 	put(const char *buf, integer n);
	virtual void	get(char *buf, integer n);
	virtual void	put(long l);
	virtual void	get(long& l);
	virtual void 	put(double d);
	virtual void 	get(double& d);
	virtual void 	put(VContainer *container, integer n);
	virtual void 	get(VContainer *container, integer n);

	virtual char 	getch();
	virtual boolean	putch(const char c);
 	virtual integer	read(char *buf, integer n);
	virtual integer	write(const char *buf, integer n);
	virtual integer	getTrimLine(char *buf, integer maxBytes);
	virtual integer	putLine(const char *buf);

	virtual void	beginWrite();
	virtual void	endWrite();

	VArchiver&  	operator << (short i);
	VArchiver&  	operator >> (short& i);
	VArchiver&  	operator << (unsigned short i);
	VArchiver&  	operator >> (unsigned short& i);
	VArchiver&  	operator << (int i);
	VArchiver&  	operator >> (int& i);
	VArchiver&  	operator << (unsigned i);
	VArchiver&  	operator >> (unsigned& i);
	VArchiver&  	operator << (long l);
	VArchiver&  	operator >> (long& l);
	VArchiver&  	operator << (unsigned long l);
	VArchiver&  	operator >> (unsigned long& l);
	VArchiver&  	operator << (float f);
	VArchiver&  	operator >> (float& f);
	VArchiver&  	operator << (double d);
	VArchiver&  	operator >> (double& d);
	VArchiver&  	operator << (char c);
	VArchiver&  	operator >> (char& c);
	VArchiver&  	operator << (const char *s);
	VArchiver&  	operator >> (char *& s);

	//	Miscellany

	VPathString& 	file();
	boolean 		isOpen();
	void 			ascii(boolean b);	//	set ASCII/BINARY mode
	boolean 		ascii();			//	TRUE, if ASCII; FALSE, if BINARY

	boolean			getMemoryArchiveBuffer(	const char *& buf, 
											unsigned long& len);

	void	 		abort(AbortReason r, const char *msg = 0);
	boolean 		isAbort() const;
	AbortReason		getAbortReason();
	void			displayAbortMessage(boolean torf);
	boolean			displayAbortMessage() const;

protected:

	void 			init(const VString& fname, boolean memoryArchiveOk);
public:
	VFile&			getFile();
protected:
	boolean			openSets(boolean asDictionaries);
	void			closeSets();
	void			isOpen(boolean b);
	boolean			isMemoryArchive();
	void			isMemoryArchive(boolean b);

	integer 	   	indexOf(VObject *obj);
	integer 	   	indexOf(VClass *cls);
	VObject 	   *instanceAt(integer i);
	VClass	 	   *classAt(integer i);

	virtual filepos_t objAt(filepos_t);
	virtual filepos_t objAt(void);

	boolean			addInstance(VObject *obj);
	boolean		    addClass(VClass *cls);
	boolean			addIndexedInstance(VObject *obj);
	boolean			addIndexedClass(VClass *cls);

	void			setCurrentClassName(const char *clsName);
	const VString& 	getCurrentClassName() const;

	int 			makeAscii(const char * , int , VStream & );

	filepos_t		objFilePosition;	// relative to file
	filepos_t		objCurrentPosition;	// relative to object

private:

	VFile			archiveFile;	
	unsigned short	openBit		: 1;
	unsigned short	asciiBit 	: 1;
	unsigned short	inMemoryBit	: 1;
	unsigned short	abortMsgBit	: 1;

#ifdef CV_ARCHIVE_MANYOBJECTS
	unsigned short	setsAsDict	: 1;
#endif

	AbortReason		_abortReason;
	VString 		_currClsName;

	VCollection    *classSet; 	//	VClasses encountered
#ifdef CV_ARCHIVE_MANYOBJECTS
	VOrdCollect    *instSet;	//	set of sets of VObjects encountered
#else
	VCollection    *instSet;	//	VObjects encountered
#endif
	integer 		classIndex;	//	index of next VClass in the class set
	integer			instIndex;	//	index of next object in the instance set

	integer 	    indxOf(VCollection *collect, VObject *obj);
	VObject 	   *objAt(VCollection *collect, integer i);
	boolean 	    addObj(VCollection *collect, VObject *obj);

	boolean		    addIndexedObj(VCollection *collect, VObject *obj, 
									integer& i);

};

extern VClass *VArchiverCls;

// ---------------------------------------------------------------------------
//	(Protected)
//
inline void VArchiver::setCurrentClassName(const char *clsName)
{
 	_currClsName = clsName;
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline const VString& VArchiver::getCurrentClassName() const
{
	return _currClsName;
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline integer VArchiver::indexOf(VObject *obj)
{
#ifdef CV_ARCHIVE_MANYOBJECTS
	if (!setsAsDict || instSet == 0 || instSet->count() == 0) {
		return -1L;
	}

	// test all sets for object
	long ret;
	int	 setno;
	for (setno = 0; setno < instSet->count(); setno++) {
		VDictionary *d = (VDictionary *) (instSet->idAt(setno));

		if (d != 0) {
			ret = (long) d->getValueAtKey(obj);

			if (ret > 0L) {
				// found it
				return (long)(setno * SetSize) + ret;
			}
		}
	}

	return -1L;

#else
	return indxOf(instSet, obj);	
#endif
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline integer VArchiver::indexOf(VClass *cls)
{
	return indxOf(classSet, (VObject *) cls);	
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline VObject *VArchiver::instanceAt(integer i)
{
#ifdef CV_ARCHIVE_MANYOBJECTS
	if (setsAsDict || instSet == 0 || instSet->count() == 0) {
		return 0;
	}

	integer	setno = i / SetSize;
	integer idx	= i % SetSize;

	// find correct set
	VOrdCollect *oc = (VOrdCollect *) (instSet->idAt(setno));

	if (oc == 0) {
		return 0;
	}

	return oc->idAt(idx);

#else
	return objAt(instSet, i);	
#endif
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline VClass *VArchiver::classAt(integer i)
{
	return (VClass *) objAt(classSet, i);	
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline boolean VArchiver::addInstance(VObject *obj)
{
#ifdef CV_ARCHIVE_MANYOBJECTS
	if (setsAsDict || instSet == 0 || instSet->count() == 0) {
		return 0;
	}

	integer	setno = instIndex / SetSize;
	integer idx	= instIndex % SetSize;
	instIndex++;

	// find correct set
	VOrdCollect *oc = (VOrdCollect *) (instSet->idAt(setno));
	if (oc == 0) {
		oc = new VOrdCollect(200);
//		instSet->replaceAt(setno, oc);
		instSet->add(oc);
	}

	return (oc->add(obj) == 0) ? FALSE : TRUE;

#else
	return addObj(instSet, obj);
#endif
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline boolean VArchiver::addClass(VClass *cls)
{
	return addObj(classSet, (VObject *) cls);
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline boolean VArchiver::addIndexedInstance(VObject *obj)
{
#ifdef CV_ARCHIVE_MANYOBJECTS
	if (!setsAsDict || instSet == 0 || instSet->count() == 0) {
		return FALSE;
	}

	integer	setno = instIndex / SetSize;
	integer idx	= instIndex % SetSize;
	instIndex++;

	// find correct dictionary
	VDictionary *d = (VDictionary *) (instSet->idAt(setno));
	if (d == 0) {
		d = new VDictionary(VIntAssocCls);
//		instSet->replaceAt(setno, d);
		instSet->add(d);
	}

	return d->atKeyPut(obj, instIndex);

#else
	return addIndexedObj(instSet, obj, instIndex);
#endif
}

// ---------------------------------------------------------------------------
//	(Protected)
//
inline boolean VArchiver::addIndexedClass(VClass *cls)
{
	return addIndexedObj(classSet, (VObject *) cls, classIndex);
}

// ---------------------------------------------------------------------------
//
inline VPathString& VArchiver::file()
{
	return archiveFile.file();
}
	
// ---------------------------------------------------------------------------
//
inline VFile& VArchiver::getFile() 
{
	return archiveFile;
}
		
// ---------------------------------------------------------------------------
//
inline boolean VArchiver::isOpen() 
{
	return openBit == 0 ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
//
inline void VArchiver::isOpen(boolean b) 
{
	openBit = b ? 1 : 0;
}

// ---------------------------------------------------------------------------
//
inline void VArchiver::displayAbortMessage(boolean b)
{
	abortMsgBit = b ? 1 : 0;
}

// ---------------------------------------------------------------------------
//
inline boolean VArchiver::displayAbortMessage() const
{
	return abortMsgBit == 1;
}

// ---------------------------------------------------------------------------
//	Returns TRUE, if archiver was aborted; FALSE, if not.
//
inline boolean VArchiver::isAbort() const
{
	return _abortReason != VArchiver::NoAbort;
}

// ---------------------------------------------------------------------------
//	Returns and clear the abort code.
//
inline VArchiver::AbortReason VArchiver::getAbortReason()
{
	VArchiver::AbortReason r = _abortReason;
	_abortReason = VArchiver::NoAbort;
	return r;
}

// ---------------------------------------------------------------------------
//
inline boolean VArchiver::isMemoryArchive() 
{
	return inMemoryBit == 0 ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
//
inline void VArchiver::isMemoryArchive(boolean b) 
{
	inMemoryBit = b ? 1 : 0;
}

// ---------------------------------------------------------------------------
//
inline void VArchiver::ascii(boolean b) 
{ 
	asciiBit = b ? 1 : 0;
}

// ---------------------------------------------------------------------------
//
inline boolean VArchiver::ascii() 
{ 
	return asciiBit == 0 ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator << (VObject *obj) 
{ 
	putObject((VObject *) obj, TRUE); 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (VObject *& obj) 
{ 
	obj = getObject(obj); 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator << (double d) 
{ 
	put(d);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (double& d) 
{ 
	get(d);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator << (long l) 
{ 
	put(l);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (long& l) 
{ 
	get(l);
	return *this; 
}

// ---------------------------------------------------------------------------
//	Store a short integer as a long integer.
//
inline VArchiver& VArchiver::operator << (short i) 
{ 
	*this << (long) i; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (short& i) 
{ 
	long l; 
	*this >> l; 
	i = (short) l; 
	return *this; 
}

// ---------------------------------------------------------------------------
//	Store an unsigned short integer as a long integer.
//
inline VArchiver& VArchiver::operator << (unsigned short i) 
{ 
	*this << (long) i; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (unsigned short& i) 
{ 
	long l; 
	*this >> l; 
	i = (unsigned short) l; 
	return *this; 
}

// ---------------------------------------------------------------------------
//	Store an integer as a long integer.
//
inline VArchiver& VArchiver::operator << (int i) 
{ 
	*this << (long) i; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (int& i) 
{ 
	long l; 
	*this >> l; 
	i = (int) l; 
	return *this; 
}

// ---------------------------------------------------------------------------
//	Store an unsigned integer as a long integer.
//
inline VArchiver& VArchiver::operator << (unsigned i) 
{ 
	*this << (long) i; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (unsigned& i) 
{ 
	long l; 
	*this >> l; 
	i = (int) l; 
	return *this; 
}

// ---------------------------------------------------------------------------
//	Store an unsigned long integer as a long integer.
//
inline VArchiver& VArchiver::operator << (unsigned long l) 
{ 
	*this << (long) l; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (unsigned long& l) 
{ 
	long l2; 
	*this >> l2; 
	l = l2; 
	return *this; 
}

// ---------------------------------------------------------------------------
//	Store a float as a double.
//
inline VArchiver& VArchiver::operator << (float f) 
{ 
	*this << (double) f; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (float& f) 
{ 
	double d; 
	*this >> d; 
	f = (float) d; 
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator << (char c) 
{ 
	put(c);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (char& c) 
{ 									   
	get(c);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator << (const char *s) 
{ 
	put(s);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VArchiver& VArchiver::operator >> (char *& s) 
{ 
	get(s);
	return *this; 
}

#include "archvrtg.h"

#endif // VARCHIVER_H
