// $Header:   Q:/views/common/vcs/TAGSTRM.H_V   1.13   Mar 24 1997 16:13:42   scottt  $ 

//	tagstrm.h
//
//	VTagStream class interface [Common]
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
//	05/17/93 pmcm	merged MOTIF and MS-Windows versions
//	08/09/93 jmd	removed free
//	09/26/93 jmd	preened (a little)
//	11/01/93 pkt	added const.
//	11/01/93 pkt	preened.
//	11/03/93 pkt	added cast to copyFrom call.
//  12/15/93 jmd	added putTagStream()
//  01/04/94 jmd	preened, made copy constructor take a const arg
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	03/10/95 evc    internationalization
//	03/30/95 pkt	updated for revised positioning.
//	04/11/95 pkt	updated for VWideChar.
//	04/11/95 pkt	moved call to inline 'tags' after inline definition.
//	05/19/95 pkt	selected Windows as common source.
//	05/26/95 pkt	combined international changes into common version.
//	09/06/95 pkt	removed OFI code.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTAGSTRM_H
#define VTAGSTRM_H

// ---------------------------------------------------------------------------
//
#include "tofmstrm.h"

// ---------------------------------------------------------------------------
//
CLASS VClass;
CLASS VDictionary;
CLASS VObject;
CLASS VString;
CLASS VStream;

// ---------------------------------------------------------------------------
//
#define MAX_TAG_VALUE	200

#define NoTag             0
#define AppendTag        01
#define PrependTag       02
#define DeleteTag        04
#define WithTag         010
#define StripTags       020
#define AppendNewLine   040
#define StripNewLine   0100
#define ReplaceTag     0200
#define ReplaceOnce    0400

// ---------------------------------------------------------------------------
//
enum TagStatus { TagOk, TagIncomplete, TagEOS }; 

// ---------------------------------------------------------------------------
// The VTagStream class allows flexable placement and replacement of
// tags within VStream objects.  A tag is one or more characters delimited
// with two characters: beginTag and endTag.  Any two ASCII characters may
// be selected to delimit a tag, though $ is used by default.  A VString
// object 'value', is used to hold the last scanned tag string.
//
CLASS VTagStream : public VToFromStream {
public:
	// Constructors & Destructor
					VTagStream();
					VTagStream(const VTagStream&);

	VClass		   *iam();

	// Getting and Putting Tags
	boolean			getTag(VString&);
	void			ungetTag();
	void			ungetFrom();
	void			ungetTo();
	void			putTag(unsigned, const VString *);
	void			putTag(unsigned, const char *); 
	void			putTag(unsigned, const VString& );

	TagStatus		getStatus(int&);
	virtual void	putStreamTag(unsigned, VStream *strm);

	// Operations on Entire VTagStream.
	void			doTags(unsigned, VDictionary *);
	void			doTag(unsigned, VString *, VString *);
	void			doTag(unsigned, VString *);
	void			doTag(unsigned);

	// Tag Characters
#ifdef CV_WCHAR
	void			tags(char b, char e);
#endif
	void			tags(VWideChar b, VWideChar e);

	// Initialization.
	void			reset();

	// Copying
	boolean			copyFrom(VObject *);
	VTagStream&		operator=(const VString& s);
	VTagStream&		operator=(const VTagStream&);
#ifndef CV_NOARCHIVER
	// Archival
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:
	void			init();

public:
	integer			toLastAt;	// TO VStream position of first tag char
	integer			fromLastAt;	// FROM VStream position of first tag char
								// measured in characters

protected:
	VWideChar		beginTag;	// The characters used as tag delimitors
	VWideChar		endTag;
	int				lastTagPos;	// The position of the last tag
	TagStatus		tagStat;	// The tag status, one of:
								// TagOk, TagIncomplete, or TagEOS
};

extern VClass *VTagStreamCls;

// ---------------------------------------------------------------------------
// Set the position of the FROM VStream to the start of the last tag.
//
inline void VTagStream::ungetFrom()
{
	setFromCharPos(fromLastAt);
}

// ---------------------------------------------------------------------------
// Set the position of the TO VStream to the start of the last tag.
//
inline void VTagStream::ungetTo()
{
	setToCharPos(toLastAt);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Set the tag delimiters to b and e, respectively.
//
inline void VTagStream::tags(char b, char e)
{
	VString::atowc(beginTag, b);  
	VString::atowc(endTag, e);
}
#endif

// ---------------------------------------------------------------------------
// Set the tag delimiters to b and e, respectively.
//
inline void VTagStream::tags(VWideChar b, VWideChar e)
{
	beginTag = b;
	endTag = e;
}

// ---------------------------------------------------------------------------
//
inline void VTagStream::init()
{

	tags('$','$');
	tagStat = TagOk;
	lastTagPos = -1;
}

// ---------------------------------------------------------------------------
// Copy the contents of VString object s to this VTagStream.
//
inline VTagStream& VTagStream::operator=(const VString& s)
{
	copyFrom((VObject *)&s);
	return *this;
}

#endif // VTAGSTRM_H
