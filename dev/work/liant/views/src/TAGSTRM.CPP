// $Header:   Q:/views/common/vcs/TAGSTRM.CPV   1.15   Mar 24 1997 16:13:40   scottt  $ 

//	tagstrm.cpp
//
//	VTagStream class implementation [Common]
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
//	05/17/93 pmcm	merged MOTIF and MS-Windows versions
//	08/09/93 jmd	removed free
//	11/01/93 pkt	added const; removed COPYINIT.
//	12/15/93 jmd	added test for empty string in putTag, NIL to 0
//	12/15/93 jmd	added putTag(VStream)
//	01/04/94 jmd	preened, made copy constructor take a const arg
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	01/27/94 pkt	fixed #ifdef surrounding destructor.
//	03/24/94 pkt	changed \n for Symantec C++ for Mac.
//	03/24/94 pkt	fixed 'doTags' to accept CR-LF on Mac.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	03/10/95 evc	Internationalized
//	03/31/95 pkt	updated for revised positioning functions.
//	04/11/95 pkt	updated for VWideChar.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	combined international changes into common version.
//	07/18/95 dgm	Minor VWideChar change in getFrom();
//	08/11/95 pkt	updated for CSet.
//	08/18/95 pkt	tweaked 'getFrom' to eliminate MSVC warning.
//	09/06/95 pkt	removed OFI code.
//	11/29/95 tlf	updated for HPUX.
//	03/28/96 pkt	fixed so that ungetTag works within a VFile object.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "tagstrm.h"
#include "dictinry.h"
#include "archiver.h"

// ---------------------------------------------------------------------------
//
defineClass(VTagStream, VToFromStream)

// ---------------------------------------------------------------------------
//
#define MAXtagLength 100
#define INCRtagLength 100

// ---------------------------------------------------------------------------
// Construct a VTagStream of default VStream size, with '$'
// as the tag delimitor.
//
VTagStream::VTagStream()
{
	VTagStream::init();
}

// ---------------------------------------------------------------------------
// Construct a VTagStream from VTagStream ts.
//
VTagStream::VTagStream(const VTagStream& ts)
{
	init();
	copyFrom((VTagStream *)(&ts));
}

// ---------------------------------------------------------------------------
//
VTagStream& VTagStream::operator=(const VTagStream &ts)
{
	if (&ts != this) {
		reset();
		init();
		copyFrom((VTagStream *)(&ts));
	}
	return *this;
}

// ---------------------------------------------------------------------------
// Return the next tag encountered or a null pointer if 'end-of-stream' was
// reached.  If the FROM and TO VStreams are different, then copy all of the
// characters preceeding the tag and the tag itself into the TO VStream.
//
boolean VTagStream::getTag(VString& s)
{
	VWideChar c;
	boolean foundFlag;
    
	int t = 0;
	while (TRUE) {
		fromLastAt = getFromPos();
		toLastAt = getToPos();
		t = getch();
		if (t <= 0 || t == beginTag) {
			break;
		}
	}
	foundFlag = (t >= 0);

	lastTagPos = (int)getCharPos();

	if (!foundFlag) {
		tagStat = TagEOS;
		return FALSE;
	}

	s.reset();
 	int i;
 	c = (VWideChar) -1;	 
	for (i = 0 ; getch(c) && (c != endTag) ; i++) {
		s+=c; 
		c = (VWideChar) -1;
	}

	if (c ==(VWideChar) -1) { // End of stream in middle of tag
		tagStat = TagIncomplete;
		return FALSE;
	}
	else {
		tagStat = TagOk;
	}

 	return TRUE;
}

// ---------------------------------------------------------------------------
// Put str into the TO VStream.  The op parameter can be one or
// more of the following: ReplaceTag, WithTag, AppendNewLine, or
// ReplaceOnce or'ed together. If ReplaceTag is given, then just str is
// copied;  if WithTag is given, then the current tag delimitors are placed
// about str.  If AppendNewLine is given, then a line termination is written
// at the end.  If ReplaceOnce is given, then the replacement occurs once.
//
void VTagStream::putTag(unsigned op, const VString *str)
{
	if (str == 0) {
		return;
	}
	//const char* s = str->gets(); 
	const VWideChar * s = str->getwcs();
	if ((s == 0) || (*s == CV_WCHAR_CONST('\0'))) {		// jmd 12/15/93 added \0 test
		return;
	}
	integer i = str->getCharSize();
	if (op & WithTag) {
		putch(beginTag);
	}
	while (i-- > 0) {
		putch(*s++);
	}

	if (op & WithTag) {
		putch(endTag);
	}
	if (op & AppendNewLine) {
		if (to) {
			to->putln((char *) 0);
		}
		else {
			putln((char *)0);
		}
	}
} 

// takes a VString  reference instead of a pointer  
void VTagStream::putTag(unsigned op, const VString& str ) 
{  
	putTag(op, &str);
}



// ---------------------------------------------------------------------------
// (implementation detail)
// Same as putTag(unsigned, VString *) except that strm is a VStream
// jmd 12/15/93
//
void VTagStream::putStreamTag(unsigned op, VStream *strm)
{
	if (strm == 0) {
		return;
	}

	if (op & WithTag) {
		putch(beginTag);
	}

	int ch;
	strm->at(0L);

	while ((ch = strm->getch()) != -1) {
		putch((char) ch);
	}

	if (op & WithTag) {
		putch(endTag);
	}

	if (op & AppendNewLine) {
		if (to) {
			to->putln((char *)0);
		}
		else {
			putln((char *)0);
		}
	}
}

// ---------------------------------------------------------------------------
// Same as putTag(unsigned, VString *) except that s is a NULL
// terminated C string.   Left in for backward compatability 
// after initialization
//
void VTagStream::putTag(unsigned op, const char *s)  
{  
	VString t(s);   
	putTag ( op,t);
}

// ---------------------------------------------------------------------------
// Return the FROM and TO VStreams to the position of the first delimitor.
//
void VTagStream::ungetTag()
{
	setToPos(toLastAt);
	setFromPos(fromLastAt);
}

// ---------------------------------------------------------------------------
// Scan the entire to VStream performing op on the encountered tags. OP may be
// one or more of the following: ReplaceTag, AppendTag, PrependTag, 
// DeleteTag, StripTags, WithTag, AppendNewLine, or StripNewLine
// or'ed together.  The key VString object of each assocation in d is
// construed as a tag.  If ReplaceTag is given, then replace each key tag
// with its associated VString value.  If AppendTag is given, then the
// original tag is left undisturbed and the value is added after the original
// tag.  If PrependTag is given, then the original tag is left undisturbed and
// value is placed before it.  If DeleteTag is given, then the tag is
// deleted with no replacement.  If StripTags is given, then tags which do not
// match and key in d are deleted.  If WithTag is given, then place the
// current delimiters about value when it is inserted in the VTagStream. 
// If AppendNewLine is given, then a line terminator is appended to the
// inserted string.  If StripNewLine is given, then if there is a line
// terminator following the original tag, then it is deleted. If ReplaceOnce
// is given, then each key is replaced once and then removed from d.
//
void VTagStream::doTags(unsigned op, VDictionary *d)
{
	VString value;
	VString *s;
	VStream *ts = 0;
	VStream *last = to;
	int c;
	unsigned newLine = NoTag;
	integer start;

	if (op & (AppendTag | PrependTag)) {
		op = (unsigned)(op | ReplaceTag);
	}
	if (from == to) {
		to = ts = new VStream;
	}
	beginScan(from, to);
	while (getTag(value)) {
		if ((s = (VString *)d->getValueAtKey(&value)) != 0) {
			if (to) {
				to->setPos(start = toLastAt);
			}
			else {
				setPos(start = toLastAt);
			}
			if (op & (AppendTag | PrependTag | StripNewLine)) {
				// strip off new-line following tag
				c = peekch();
				newLine = NoTag;
				if (c == '\x0D' || c == '\x0A') {
					getch();
					if (c == '\x0D' && peekch() == '\x0A') {
						getch();
					}
					if (!(op & StripNewLine)) {
						newLine = AppendNewLine;
					}
				}
			}
			if (to) {
				to->setPos(start);
			}
			else {
				setPos(start);
			}
			if (op & AppendTag) {
				putTag((unsigned)(newLine | WithTag), &value);
			}
			if (op & ReplaceTag) {
				if (s->isA(VStreamCls)) {
					// jmd 12/15/93
					putStreamTag(op, (VStream *) s);
				}
				else {
					putTag(op, s);
				}
			}
			if (op & PrependTag) {
				putTag((unsigned)(newLine | WithTag), &value);
			}
			if (op & ReplaceOnce) {
				d->removeKey(&value);
			}
		}
		else if (op & StripTags) {
			if (to) {
				to->setPos(start = toLastAt);
			}
			else {
				setPos(start = toLastAt);
			}
		}
	}
	if (to) {
		to->truncAt();
	}
	else {
		truncAt();
	}
	beginScan(from, last);
	if (ts) {
		VStream *v = last ? last : (VStream *)this;
		v->setCharPos(0);
		v->setLength(0);
		v->puts(ts->gets());
		delete ts;			// jmd (was free)
	}
	beginScan(from, last);
}

// ---------------------------------------------------------------------------
// Perform op on this VTagStream. The op may be one or both
// of the following: StripNewLine or StripTags or'ed together.
//
void VTagStream::doTag(unsigned op)
{
	doTag(op, 0, 0);
}

// ---------------------------------------------------------------------------
// Perform op on this VTagStream using tag as both the key
// and value.  The op may be one or more of the following: DeleteTag, 
// StripNewLine, or StripTags or'ed together.
//
void VTagStream::doTag(unsigned op, VString *tag)
{
	doTag(op, tag, tag);
}

// ---------------------------------------------------------------------------
// Perform op on this VTagStream using a key and value. 
//
void VTagStream::doTag(unsigned op, VString *key, VString *value)
{
	VDictionary d;
	d.atKeyPut(key, value);
	doTags(op, &d);
}

// ---------------------------------------------------------------------------
// Reset this VTagStream as per VToFromStream::reset(), and set
// the tag delimitors to their default values.
//
void VTagStream::reset() 
{ 
	VToFromStream::reset(); 
	tags('$', '$'); 
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Write this VTagStream into the VArchiver 'a'.
//
void VTagStream::putTo(VArchiver& a)
{
	a << (unsigned short)beginTag;
	a << (unsigned short)endTag;
	VToFromStream::putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VTagStream from the VArchiver 'a'.
//
void VTagStream::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	beginTag = (VWideChar)tag;
	a >> tag;
	endTag = (VWideChar)tag;
	toLastAt = fromLastAt = 0;
	VToFromStream::getFrom(a);
}

// ----------------------------------------------------------------------------
//
void VTagStream::getFrom(VArchiver& a,
						 VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// A VTagStream can copy from another VStream.
//
boolean VTagStream::copyFrom(VObject *obj)
{
	if (!VToFromStream::copyFrom(obj)) {
		return FALSE;
	}
	if (!obj->isA(VTagStreamCls)) {
		return TRUE;
	}
	VTagStream *ts = (VTagStream *)obj;
	beginTag = ts->beginTag;
	endTag = ts->endTag;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return one of the following status flags based on the last getTag()
// operation for this VTagStream:
//		TagOk			Tag was processed with no errors.
//		TagIncomplete	The end stream was found before
//						the ending the last tag was found.
//		TagEOS			The end of this VTagStream was encountered
//						before a Tag was found.
//	lastTag will be set to the starting position in this VTagStream of the
//	last Tag processed or partially processed.  A position of -1 indicates
//	that no tags have been proccessed.
//
TagStatus VTagStream::getStatus(int& lastTag)
{
	lastTag = lastTagPos;
	return tagStat;
}
