// $Header:   Q:/views/common/vcs/TOFMSTRM.H_V   1.16   Mar 24 1997 16:13:48   scottt  $ 

//	tofmstrm.h
//
//	VToFromStream class interface [Common]
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
//	04/09/93 pmcm	changed id to VObject *
//	05/17/93 pmcm	merged MOTIF and MS Windwos versions
//	08/09/93 jmd	removed free
//	08/14/93 jmd	made write() and putch() take const arg
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	03/08/95 evc	Added wide character member functions for
//					internationalization
//	03/31/95 pkt	Added character positioning member functions.
//	04/04/95 pkt	fixed 'peekch(whar_t)' and 'getch(wchar_t)' to be
//					declared even if CV_WCHAR is not defined.
//	04/11/95 pkt	updated for VWideChar.
//  05/19/95 pkt    selected Motif as common source.
//  05/25/95 dgm    inverted #if check from ARCHIVER to CV_NOARCHIVER.
//	05/26/95 pkt	combined international changes into common version.
//	07/18/95 dgm	Changed ungetch() parameter for compatibility with base
//					class version.
//	09/06/95 pkt	removed OFI code.
//	01/29/96 pkt	added 'getToStream' and 'getFromStream'.
//	03/28/96 pkt	added 'getToPos', 'getFromPos', 'setToPos', 'setFromPos'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTOFMSTRM_H
#define VTOFMSTRM_H

// ---------------------------------------------------------------------------
//
#include "strm.h"

// ---------------------------------------------------------------------------
// The VToFromStream class defines a scan from a FROM VStream
// to a TO VStream.  By default, a VToFromStream object scans from
// itself to itself through the beginScan() method.  However,  
// scan(VStream *, VStream *) may be called to set other VStream objects
// as sources and destinations.
//
CLASS VToFromStream : public VStream {
public:
	// Constructors
					VToFromStream();
					VToFromStream(const VToFromStream&);
					VToFromStream(VStream *, VStream *);
	VClass		   *iam();

	// Positioning
	integer			toAt();
	integer			getToCharPos();
	integer			getToPos();
	integer			toAt(integer n);
	integer			setToCharPos(integer n);
	integer			setToPos(integer n);
	integer			fromAt();
	integer			getFromCharPos();
	integer			getFromPos();
	integer			fromAt(integer n);
	integer			setFromCharPos(integer n);
	integer			setFromPos(integer n);
	void			begin();

	// Reading and Writing
	integer			read(char *, integer);
	integer			write(const char *s, integer n);
	int				getch();
	boolean  		getch(VWideChar& c);
	void			ungetch(char);
	boolean			putch(const char c);
	int				peekch(); 
	boolean   		peekch(VWideChar& c);
#ifdef CV_WCHAR  
	void  			ungetch(VWideChar c); 
	boolean  		putch( const VWideChar c);
	integer  		read(VWideChar *s, integer n);
	integer  		write(const VWideChar *s, integer n);    
#endif

	// Scanning
	void			scan();
	void			scan(VStream *from, VStream *to);
	void			beginScan(VStream *from, VStream *to);
	void			beginScan();

	// Initialization
	boolean			copyFrom(VObject *);
	VToFromStream&	operator=(VString&);
	VToFromStream&	operator=(const VToFromStream&);
	void			reset();

	// Interrogation
	VStream		   *getFromStream();
	const VStream  *getFromStream() const;
	VStream		   *getToStream();
	const VStream  *getToStream() const;

#ifndef CV_NOARCHIVER
	// Archiving
	void			putTo(VArchiver &);
	void			getFrom(VArchiver &);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:
	void			init();

	VStream		   *from;	// from which characters are to be got
	VStream		   *to;		// to which characters are to be put

};

extern VClass *VToFromStreamCls;

// ---------------------------------------------------------------------------
//
inline void VToFromStream::init()
{
	to = from = 0;
}

// ---------------------------------------------------------------------------
// Return the position of the TO VStream, measured in bytes.
//
inline integer VToFromStream::toAt()
{
	return to ? to->at() : at();
}

// ---------------------------------------------------------------------------
// Return the position of the TO VStream, measured in characters.
//
inline integer VToFromStream::getToCharPos()
{
	return to ? to->getCharPos() : getCharPos();
}

// ---------------------------------------------------------------------------
// Return the position of the TO VStream.
//
inline integer VToFromStream::getToPos()
{
	return to ? to->getPos() : getPos();
}

// ---------------------------------------------------------------------------
// Position the TO VStream to byte n and return n.
//
inline integer VToFromStream::toAt(integer n)
{
	return to ? to->at(n) : at(n);
}

// ---------------------------------------------------------------------------
// Position the TO VStream to character n and return n.
//
inline integer VToFromStream::setToCharPos(integer n)
{
	return to ? to->setCharPos(n) : setCharPos(n);
}

// ---------------------------------------------------------------------------
// Position the TO VStream to n and return n.
//
inline integer VToFromStream::setToPos(integer n)
{
	return to ? to->setPos(n) : setPos(n);
}

// ---------------------------------------------------------------------------
// Return the position of the FROM VStream, measured in bytes.
//
inline integer VToFromStream::fromAt()
{
	return from ? from->at() : at();
}

// ---------------------------------------------------------------------------
// Return the position of the FROM VStream, measured in characters.
//
inline integer VToFromStream::getFromCharPos()
{
	return from ? from->getCharPos() : getCharPos();
}

// ---------------------------------------------------------------------------
// Return the position of the FROM VStream.
//
inline integer VToFromStream::getFromPos()
{
	return from ? from->getPos() : getPos();
}

// ---------------------------------------------------------------------------
// Position the FROM VStream to byte n and return n.
//
inline integer VToFromStream::fromAt(integer n)
{
	return from ? from->at(n) : at(n);
}

// ---------------------------------------------------------------------------
// Position the FROM VStream to character n and return n.
//
inline integer VToFromStream::setFromCharPos(integer n)
{
	return from ? from->setCharPos(n) : setCharPos(n);
}

// ---------------------------------------------------------------------------
// Position the FROM VStream to n and return n.
//
inline integer VToFromStream::setFromPos(integer n)
{
	return from ? from->setPos(n) : setPos(n);
}

// ---------------------------------------------------------------------------
// Position the FROM and TO VStreams to their beginning.
//
inline void VToFromStream::begin()
{
	fromAt(0L);
	toAt(0L);
}

// ---------------------------------------------------------------------------
// Get and put characters from/to this VToFromStream.
//
inline void VToFromStream::scan()
{
	scan(0, 0);
}

// ---------------------------------------------------------------------------
// Get characters from the VStream 'from' and put the characters to the
// 'to' VStream.  If 'from' or 'to' are null, then this VToFromStream is
// used.  Reposition both to their beginning.
//
inline void VToFromStream::beginScan(VStream *this_from, VStream *this_to)
{
	scan(this_from, this_to);
	begin();
}

// ---------------------------------------------------------------------------
// Get and put characters from/to this VToFromStream and reposition
// it to the beginning.
//
inline void VToFromStream::beginScan()
{
	beginScan(0, 0);
}

// ---------------------------------------------------------------------------
// Copy the contents of VString object s to this VToFromStream.
//
inline VToFromStream& VToFromStream::operator=(VString& s)
{
	copyFrom(&s);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStream *VToFromStream::getFromStream()
{
	return (from != 0) ? from : this;
}

// ---------------------------------------------------------------------------
//
inline const VStream *VToFromStream::getFromStream() const
{
	if (from != 0) {
		return from;
	}
	else {
		return this;
	}
}

// ---------------------------------------------------------------------------
//
inline VStream *VToFromStream::getToStream()
{
	return (to != 0) ? to : this;
}

// ---------------------------------------------------------------------------
//
inline const VStream *VToFromStream::getToStream() const
{
	if (to != 0) {
		return to;
	}
	else {
		return this;
	}
}

#endif // VTOFMSTRM_H
