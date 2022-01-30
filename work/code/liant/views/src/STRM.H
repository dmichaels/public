// $Header:   Q:/views/common/vcs/STRM.H_V   1.21   Mar 24 1997 16:13:32   scottt  $ 

//	strm.h
//
//	VStream class interface [Common]
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
//	10/20/92 jmd	added fixes to operator>>(VString&) and (char *p)
//	11/04/92 jmd	preened
//	04/01/93 pmcm	per jgl 8/20/92, fixes to operator>>(VString&) for OFI
//	04/01/93 pmcm	added const to gets() per motif 2.1  source
//	04/01/93 pmcm 	preened -- no foolin'
//	04/09/93 pmcm	changed id to VObject *
//	07/19/93 jmd	made size() const
//	07/21/93 jmd	made puts and concat take a const char*
//					removed free, added const to some members
//	08/11/93 pmcm	made atPut(), write() and putch() take const arg
//	08/20/93 pmcm	made helpers format(), scan() protected member functions.
//	09/02/93 dgm	M8 changes; changed operator>>(_fmtstr&)
//					to take (const _fmtstr&).
//	10/18/93 dgm	UNIX changes; changed operator=(const VString&) inline
//					definition to have correct signature.
//	11/01/93 pkt	interchanged some inline definitions to prevent cfront
//					diagnostics about used before inlined.
//	11/12/93 pkt	removed self.
//	11/17/93 pkt	moved concat(const char*), concat(VString *), and size
//					back to strm.cpp.
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	10/18/94 dss	Added operator>>(unsigned int&) & operator>>(unsigned long&).
//	02/01/95 pkt	added currentEle, lengthEle, getCurrentEle, getLengthEle.
//	02/15/95 pkt	added wide character versions of member functions.
//	03/07/95 pkt	added overloaded variants taking VString& arguments.
//	03/20/95 pkt	added overrides of VString manipulation functions.
//	03/24/95 pkt	small fixes to VString manipulation functions.
//	03/30/95 pkt	fixed asByteString to be public.
//	04/11/95 pkt	updated for VWideChar.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	combined international changes into common version.
//	06/27/95 pkt	replaced operator>> for unsigned int and unsigned long,
//					which were omitted 5/19/95.
//	09/06/95 pkt	removed OFI code.
//	09/06/95 pkt	worked around Metrowerks bug.
//	09/14/95 pkt	changed 'size' to return size in bytes of this stream
//					as a multibyte string.
//	12/06/95 pkt	changed operator<< to use 'format' less.
//	12/12/95 pkt	added variants of 'puts'. 
//	01/10/96 pkt	removed new 'puts' variants; redundant with 'write'.
//	03/28/96 pkt	added 'getPos', and 'setPos'.
//	01/24/97 pkt	added 'VStream(const VString&)' constructor.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSTREAM_H
#define VSTREAM_H

// ---------------------------------------------------------------------------
//
#include "str.h"

// ---------------------------------------------------------------------------
//
#define EXPAND 	01
#ifndef EOF
#define EOF 	(-1)
#endif

#ifndef LEOF
#define LEOF 	(VWideChar)(-1)
#endif
// ---------------------------------------------------------------------------
//
struct _fmtstr {
	char *fmt;
	void *data;
};

// ---------------------------------------------------------------------------
//	A VStream is an array of bytes of length length.  A VStream
//	has a current character, and an unsigned flags member variable
//	whose bits are generally available for use by derived classes.
//	Only the low-order bit is reserved by VStream.  A VStream may be
//	expandable or non-expandable.
//
CLASS VStream : public VString {
public:
	// Constructors
						VStream();	
			   		   	VStream(integer size);
						VStream(const VString&);
					   	VStream(const VStream&);

	VClass	   		   *iam();

	VStream&	 	   	operator>>(const _fmtstr&);

	// Interrogation and Modification
	integer 			size() const; // number of elements in this VStream
	integer 			getCharSize() const; // number of characters in this VStream
	void				setLength(unsigned n);
	void				setData(pointer p, integer n);
	void				flag(int flag, boolean b);
	boolean				flag(int flag);
	void				expandable(boolean b);
	boolean 			expandable();

	// Reading and Writing Contents 
	virtual integer		atGet(integer, char *, integer);
#ifdef CV_WCHAR
	virtual integer		atGet(integer, VWideChar *, integer);
#endif
	virtual integer		atPut(integer, const char *,integer);
#ifdef CV_WCHAR
	virtual integer		atPut(integer, const VWideChar *,integer);
#endif
	virtual integer		read(char *, integer);
#ifdef CV_WCHAR
	virtual integer		read(VWideChar *, integer);
#endif
	virtual integer		write(const char *, integer);
	integer				write(const VString&);
#ifdef CV_WCHAR
	virtual integer		write(const VWideChar *, integer);
#endif
	virtual int			peekch();
	virtual boolean		peekch(VWideChar &);
	virtual int			getch();
	virtual boolean		getch(VWideChar &);
	virtual boolean		putch(const char);
#ifdef CV_WCHAR
	virtual boolean		putch(const VWideChar);
#endif
	virtual void		ungetch(char);
#ifdef CV_WCHAR
	virtual void		ungetch(VWideChar);
#endif
	integer				getln(char *, integer); 	
#ifdef CV_WCHAR
	integer				getln(VWideChar *, integer);
#endif
	integer				gettrim(char *, integer);
#ifdef CV_WCHAR
	integer				gettrim(VWideChar *, integer);
#endif
	virtual integer		gets(char *, integer);
#ifdef CV_WCHAR
	virtual integer		gets(VWideChar *s, integer n);
#endif
	virtual CV_GETS_CONST char *gets() const;
	virtual const VWideChar *getwcs() const;
	virtual char	   *gets(char *) const; 
	integer				puts(const char *); 
#ifdef CV_WCHAR
	virtual integer		puts(const VWideChar *s);
#endif
	integer				puts(VString *); 
	integer				puts(const VString& s);
	integer				putln(const char *);
	integer				putln(const VString& s);
#ifdef CV_WCHAR
	integer 			putln(const VWideChar *s);
#endif

	//	The methods concat(), operator=(),	operator<<(), and operator>>()
	//	each advance the current position in the stream by the number of 
	//	characters they concatenate, copy, insert, or extract.

 	virtual	integer  	concat(VString *s);
	integer				concat(const VString& s);
 	virtual	integer  	concat(const char *s);
#ifdef CV_WCHAR
	virtual	integer		concat(const VWideChar *s);
#endif

 	VStream& 			operator=(const VStream&);
 	VStream& 			operator=(const VString& s);

 	VStream& 			operator=(char c);
 	VStream& 			operator=(const char *s);
#ifdef CV_WCHAR
 	VStream& 			operator=(const VWideChar *s);
#endif

	VStream& 			operator<<(char c);
	VStream& 			operator>>(char& c);

	VStream& 			operator<<(const char *s);
	VStream& 			operator>>(char *s);

	VStream& 			operator<<(const VString& s);
	VStream& 			operator>>(VString& s);

	VStream& 			operator<<(int i);
	VStream& 			operator>>(int& i);
	VStream&			operator>>(unsigned int& i);

	VStream& 			operator<<(long l);
	VStream& 			operator>>(long& l);
	VStream&			operator>>(unsigned long& l);

	VStream& 			operator<<(float f);
	VStream& 			operator>>(float& f);

	VStream&	 		operator<<(double d);
	VStream& 			operator>>(double& d);

	//	Copying
 	boolean				copyFrom(VObject *o);

	//	Current position
	virtual integer		at(integer n); 
	virtual integer		setCharPos(integer n);
	virtual integer		setPos(integer n);
	virtual integer		at();
	virtual	integer		getCharPos() const;
	virtual integer		getPos() const;
	virtual boolean		eof();
	virtual boolean		empty();
	virtual boolean		isEqual(VObject *o) const;
	virtual HashKey		hash() const;
	virtual void		asByteString(boolean) const;

	//	Breaking and truncating
	virtual VObject    *breakAt(integer n);
	virtual VObject    *breakAt();
	virtual boolean 	truncAt(integer n);
	virtual boolean 	truncAt();

	//	Scanning
	boolean				skipTo(char c);
#ifdef CV_WCHAR
	boolean				skipTo(VWideChar c);
#endif
	boolean				scanToMatching(char beg, char end);

	//	Initialization
	virtual void		reset();
	virtual void		expandTo(integer n);
	void				expand();
	
	//	Manipulation (overrides of VString functions)
	virtual boolean		truncAtCharPos(integer idx);
	virtual boolean		padTo(integer idx, char pad = ' ');
	virtual boolean		deleteAtCharPos(integer idx, integer count = 1);
	virtual void		insertAtCharPos(const VString& ins, integer idx);
	void				insertAtCharPos(const char *ins, integer idx = 0, integer inssize = 0, char pad = ' ');
	void				insertAtCharPos(const char c, integer idx, char pad = ' ');
#ifdef CV_WCHAR
	virtual boolean		padTo(integer idx, VWideChar pad);
	virtual void		insertAtCharPos(const VWideChar *ins, integer idx, integer inssize, VWideChar pad);
	void				insertAtCharPos(const VWideChar c, integer idx, VWideChar pad);
#endif

#ifndef CV_NOARCHIVER
	//	Archiving
	void				putTo(VArchiver& a);
	void				getFrom(VArchiver& a);
	virtual void	 	getFrom(VArchiver& a, VObject *data,
								boolean do_realize = TRUE);
#endif

protected:
#ifndef __MWERKS__
	_fmtstr				scan(char *, ...);
	VString				format(char *, ...);
#else
	_fmtstr				scan(char * ...);
	VString				format(char * ...);
#endif

	void				init();
	virtual void		asWideString(boolean) const;
	
	integer				getLengthEle() const;
	integer				getCurrentEle() const;
	integer				getCurrent() const;
	integer				getCount() const;
	integer				toElePosition(integer) const;
	integer				fromElePosition(integer, short *) const;

	// integer 			nEle; 		// The number of slots in a VString (inherited)
	integer 			length; 	// The number of characters in the VStream
	integer 			lengthEle; 	// The VStream's characters, measured in bytes or wchar_ts. 
	integer 			currentEle;	// Current position in bytes or wchar_ts.
	integer				current;	// Current character position within the VStream contents, in characters.
	short				byteOfCharacter; // Byte offset of currentEle within current character.
	short				bytesOfPrev;// Number of bytes in preceding character, or 0.
	integer				eleCount;	// Count of elements handled by atGet, atPut.
	integer				charCount;	// Count of characters handled just now
	ushort				flags; 		// EXPAND (expandable) plus any subclass flags

};

extern VClass *VStreamCls;

// ---------------------------------------------------------------------------
//
inline void VStream::setLength(unsigned n) 
{
	length = n;
	lengthEle = toElePosition(length);
}

// ---------------------------------------------------------------------------
//
inline void VStream::setData(pointer p, integer n) 
{
	VString::setData(p); 
	nEle = n;
}

// ---------------------------------------------------------------------------
// Set or reset 'flag' if 'b' is TRUE or FALSE respectively.
//
inline void VStream::flag(int flag, boolean b) 
{
	if (b) {
		flags |= flag; 
	}
	else {
		flags &= ~flag;
	}
}

// ---------------------------------------------------------------------------
// Return TRUE if 'flag' is set, FALSE otherwise.
//
inline boolean VStream::flag(int flag) 
{
	return (flags & flag) ? TRUE : FALSE; 
}

// ---------------------------------------------------------------------------
// If 'b' is TRUE then this VStream is allowed to expand,
// otherwise it is not.
//
inline void VStream::expandable(boolean b) 
{
	flag(EXPAND, b); 
}

// ---------------------------------------------------------------------------
// Return TRUE if this VStream is allowed to expand, otherwise return FALSE.
//
inline boolean VStream::expandable() 
{
	return flag(EXPAND); 
}

// ---------------------------------------------------------------------------
//
inline integer VStream::size() const
{
	asWideString(FALSE);
	return lengthEle; 
}

// ---------------------------------------------------------------------------
//
inline integer VStream::getCharSize() const
{
	return length; 
}

// ---------------------------------------------------------------------------
// Prevent hiding the base class puts(const VString &).
//
inline integer VStream::puts(const VString& str)
{
	return VString::puts(str);
}

// ---------------------------------------------------------------------------
// Prevent hiding the base class putln(const VString&).
//
inline integer VStream::putln(const VString& str)
{
	return VString::putln(str);
}

// ---------------------------------------------------------------------------
// Prevent hiding the base class concat(const VString&.
//
inline integer VStream::concat(const VString& str)
{
	return VString::concat(str);
}

// ---------------------------------------------------------------------------
//
inline integer VStream::concat(VString *s) 
{
	return puts(s); 
}

// ---------------------------------------------------------------------------
//
inline integer VStream::concat(const char *s) 
{
	return puts(s); 
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Could not keep this inline, because it's virtual and MPW C++ was 
// making too many definitions.
//
inline integer VStream::concat(const VWideChar *s)
{
	return puts(s); 
}
#endif

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator=(const VString& s) 
{ 
	reset();
	puts(s.gets());
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator=(const char *s) 
{ 
	reset();
	puts(s);
	return *this; 
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator=(const VWideChar *s) 
{ 
	reset();
	puts(s);
	return *this; 
}
#endif

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator=(char c) 
{ 
	reset();
	putch(c);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator<<(char c) 
{ 
	putch(c);
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(char& c) 
{ 
	c = getch();
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator<<(const char *s) 
{
	VString vs = s;
	puts(&vs);  
	return *this; 
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator<<(const VString& s)
{
	return *this << s.gets();
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(char *s)
{
	return *this >> scan("%s",s);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(VString& s) 
{
	char p[MAX_STRING + 1];
	*this >> p;
	s = p;
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(int& i)
{
	return *this >> scan("%d", &i);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(unsigned int& i)
{
	return *this >> scan("%u", &i);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(long& l)
{
	return *this >> scan("%ld", &l);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(unsigned long& l)
{
	return *this >> scan("%lu", &l);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator<<(float f)
{
	return *this << format("%g ", f);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(float& f)
{
	return *this >> scan("%g", &f);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator<<(double d)
{
	return *this << format("%lg ", d);
}

// ---------------------------------------------------------------------------
//
inline VStream& VStream::operator>>(double& d)
{
	return *this >> scan("%lg", &d);
}

// ---------------------------------------------------------------------------
//
inline void VStream::init()
{
	current	= 0; 
	currentEle = 0;
	byteOfCharacter = 0;
	bytesOfPrev = 0;
	flags 	= 0; 
	length	= 0; 
	lengthEle = 0;
}

// ---------------------------------------------------------------------------
//
inline integer VStream::getLengthEle() const
{
	return lengthEle;
}

// ---------------------------------------------------------------------------
//
inline integer VStream::getCurrentEle() const
{
	return currentEle;
}

// ---------------------------------------------------------------------------
//
inline integer VStream::getCurrent() const
{
	if (current < 0) {
		((VStream *)this)->current = fromElePosition(currentEle, 0);
	}
	return current;
}

// ---------------------------------------------------------------------------
// Return the count from a function which has set eleCount and charCount
// to indicate how many elements and characters have been processed.
//
inline integer VStream::getCount() const
{
	return eleCount;
}

#endif // VSTREAM_H
