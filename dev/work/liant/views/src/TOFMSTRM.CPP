// $Header:   Q:/views/common/vcs/TOFMSTRM.CPV   1.14   Mar 24 1997 16:13:48   scottt  $ 

//	tofmstrm.cpp
//
//	VToFromStream class implementation [Common]
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
//	04/07/93 pmcm	changed id to VObject *
//	08/09/93 jmd	removed free
//	08/14/93 jmd	made write() and putch() take const arg
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	01/13/94 dgm	merged and preened
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	03/08/95 evc	Added wide character members for internationalization
//	04/10/95 pkt	fixed 'peekch(whar_t)' and 'getch(wchar_t)' to be
//					defined even if CV_WCHAR is not defined.
//	04/11/95 pkt	updated for VWideChar.
//	05/19/95 pkt	selected Motif as common source.
//	05/26/95 pkt	combined international changes into common version.
//	06/07/95 pkt	fixed #ifdef ARCHIVER to use CV_NOARCHIVER.
//	07/18/95 dgm	Changed ungetch() parameter for compatibility with base
//					class version.
//	08/17/95 pkt	fixed 'getch(const VWideChar&)' to return LEOF as
//					appropriate.
//	08/21/95 pkt	re-fixed 'getch(const VWideChar&)'.
//	09/06/95 pkt	removed OFI code.
//	04/02/96 pkt	fixed 'getch(const VWideChar&)' to return FALSE only if
//					EOF of from stream encountered.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "tofmstrm.h"

// ---------------------------------------------------------------------------
//
defineClass(VToFromStream, VStream)

// ---------------------------------------------------------------------------
// Construct a VToFromStream with the default size of a VStream.
//
VToFromStream::VToFromStream()
{
	VToFromStream::reset();
}

// ---------------------------------------------------------------------------
// Construct a VToFromStream with the default size of a VStream with
// from as the FROM VStream and to as the TO VStream.
//
VToFromStream::VToFromStream(VStream *this_from, VStream *this_to)
{
	scan(this_from, this_to);
}

// ---------------------------------------------------------------------------
// If from is non-zero, then arrange for characters to be read from
// from, otherwise read from this VToFromStream.  If to is non-zero, 
// then arrange for characters to be written to to, otherwise write to
// this VToFromStream. 
//
void VToFromStream::scan(VStream *this_from, VStream *this_to)
{
	this->from = (this_from == this) ? 0 : this_from;
	this->to = (this_to == this) ? 0 : this_to;
}

// ---------------------------------------------------------------------------
// Return the next character from the FROM VStream. If end-of-stream
// is reached, return a -1.  If the TO VStream is different than the FROM
// VStream, then put the character into it.
//
int VToFromStream::getch() 
{ 
	int c;
	if (from != 0) {
		c = from->getch();
	}
	else {
		c = VStream::getch();
	}
	if (c >= 0 && from != to) {
		putch((char) c);
	}
	return c;
}

// ---------------------------------------------------------------------------
// Put c back on the FROM and TO VStreams.
//
void VToFromStream::ungetch(char c) 
{ 
	if (from != 0) {
		from->ungetch(c); 
	}
	else {
		VStream::ungetch(c);
	}
	if (to != 0) {
		to->ungetch(c); 
	}
	else if (from != to) {
		VStream::ungetch(c); 
	}
}

// ---------------------------------------------------------------------------
// Read upto n characters from the TO VStream into s.  If the TO
// VStream is different than the FROM VStream, then write these characters to
// it. Return the number of characters read.
//
integer VToFromStream::read(char *s, integer n) 
{
	integer i;
	if (from != 0) {
		i = from->read(s, n);
	}
	else {
		i = VStream::read(s, n);
	}
	if (i > 0 && to != from) {
		i = write(s, i);
	}
	return i;
}

// ---------------------------------------------------------------------------
// Write c into the TO VStream and return TRUE if the operation
// is successful, otherwise return FALSE.
//
boolean VToFromStream::putch(const char c) 
{ 
	return (to != 0) ? to->putch(c) : VStream::putch(c); 
}

// ---------------------------------------------------------------------------
// Return the next character from the FROM VStream without advancing
// its current position.
//
int VToFromStream::peekch() 
{ 
	return (from != 0) ? from->peekch() : VStream::peekch();
}

// ---------------------------------------------------------------------------
// Write upto n bytes of s to the TO VStream.  Return the actual
// number of characters written.
//
integer VToFromStream::write(const char *s, integer n) 
{ 
	return (to != 0) ? to->write(s, n) : VStream::write(s, n);
}

// ---------------------------------------------------------------------------
// Set the contents of this VToFromStream to empty and call beginScan().
//
void VToFromStream::reset() 
{ 
	VStream::reset();
	beginScan(); 
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Write this VToFromStream into the VArchiver a. Just this object
// itself is written to the VArchiver, not the VStream objects it may refer to.
//
void VToFromStream::putTo(VArchiver& a)
{
	VStream::putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VToFromStream from the VArchiver a. Note: It is
// initialized to stream from and to itself.
//
void VToFromStream::getFrom(VArchiver& a)
{
	VStream::getFrom(a);
	to = from = 0;
}

// ----------------------------------------------------------------------------
//
void VToFromStream::getFrom(VArchiver& a,
						    VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// A VToFromStream can copy from a VStream object.
//
boolean VToFromStream::copyFrom(VObject *obj)
{
	if (!VStream::copyFrom(obj)) {
		return FALSE;
	}
	if (!obj->isA(VToFromStreamCls)) {
		return TRUE;
	}
	VToFromStream *t = (VToFromStream *)obj;
	to = t->to;
	from = t->from;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Construct a VToFromStream from the VToFromStream tfs.
//
VToFromStream::VToFromStream(const VToFromStream& tfs)
{
	init();
	copyFrom((VToFromStream *)(&tfs));
}

// ---------------------------------------------------------------------------
//
VToFromStream& VToFromStream::operator=(const VToFromStream& tfs)
{
	if (&tfs != this) {
		reset();
		init();
		copyFrom((VToFromStream *)(&tfs));
	}
	return *this;
}

// ---------------------------------------------------------------------------
// Return the next wide character from the FROM VStream. If end-of-stream
// is reached, return a 0.  If the TO VStream is different than the FROM
// VStream, then put the character into it.
//
boolean VToFromStream::getch(VWideChar& c) 
{  
	boolean rtval; 
	if (from != 0) {
		rtval = from->getch(c);
	}
	else {
		rtval = VStream::getch(c);
	}
	if (!rtval) {
		// c = LEOF; pkt 08/21/95 -- boolean return val indicates EOF
	}
	else if (from != to) {
		putch(c); // pkt 04/02/96; Vantive failure 214748
	}
	return rtval;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Put c back on the FROM and TO VStreams.
//
void VToFromStream::ungetch(VWideChar c) 
{ 
	if (from != 0) {
		from->ungetch(c); 
	}
	else {
		VStream::ungetch(c);
	}
	if (to != 0) {
		to->ungetch(c); 
	}
	else if (from != to) {
		VStream::ungetch(c); 
	}
}


// ---------------------------------------------------------------------------
// Write c into the TO VStream and return TRUE if the operation
// is successful, otherwise return FALSE.
//

boolean VToFromStream::putch(const VWideChar c) 
{ 
	return (to != 0) ? to->putch(c) : VStream::putch(c); 
}    
#endif

// ---------------------------------------------------------------------------
// Return the next wide character from the FROM VStream without advancing
// its current position.
//
boolean  VToFromStream::peekch(VWideChar &c) 
{    
boolean rval;    
	if (from != 0) { 
	    rval = from->peekch(c);
	}  else { 
		rval = VStream::peekch(c) ;
	}    
	if (!rval) c = LEOF;  // EOF
	return  rval;
	//return (from != 0) ? from->peekch(c) : VStream::peekch(c);
}  

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// 	Read upto n wide characters from the TO VStream into s. 
//  If the TO  VStream is different than the FROM VStream, 
//  then write these characters to  it. Return the number of
//	characters read.   

integer VToFromStream::read(VWideChar *s, integer n) 
{
	integer i;
	if (from != 0) {
		i = from->read(s, n);
	}
	else {
		i = VStream::read(s, n);
	}
	if (i > 0 && to != from) {
		i = write(s, i);
	}
	return i;
}   

// ---------------------------------------------------------------------------
// Write upto n wide characters in s to the TO VStream.  Return the actual
// number of characters written.
//
integer VToFromStream::write(const VWideChar *s, integer n) 
{ 
	return (to != 0) ? to->write(s, n) : VStream::write(s, n);
}   
#endif
