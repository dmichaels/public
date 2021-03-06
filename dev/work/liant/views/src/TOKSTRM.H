// $Header:   Q:/views/common/vcs/TOKSTRM.H_V   1.11   Mar 24 1997 16:13:52   scottt  $ 

//	tokstrm.h
//
//	VTokenStream class interface [Common]
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
//	09/26/93 jmd	preened (a little)
//	10/18/93 dgm	UNIX changes.
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Mac.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	04/04/95 evc 	Added  isnum for wide characters
//	04/11/95 pkt	updated for VWideChar.
//	05/19/95 pkt	selected Windows version as common source.
//	05/26/95 pkt	combined international changes into common version.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTOKSTRM_H
#define VTOKSTRM_H

// ---------------------------------------------------------------------------
//
#include "tagstrm.h"
#include "tokens.h"

// ---------------------------------------------------------------------------
//
CLASS VTokens;

// ---------------------------------------------------------------------------
// The VTokenStream class defines a lexical scan from the FROM VStream
// to the TO VStream.  A VTokens object defines the tokens for all
// VTokenStream objects. 
//
CLASS VTokenStream : public VTagStream {
public:
	// Constructors & Destructor
					VTokenStream();
					VTokenStream(const VTokenStream&);
				   ~VTokenStream();

	VClass		   *iam();

	// Getting and Putting VTokens
	virtual int		getToken();
	virtual int		getToken(VString&);
	void			useTokens(VTokens *); 

	void			ungetToken();
	void			putToken(char *s);
	void			putToken(VString *s);

	// Initialization
	void			reset();

	// Copying

	boolean			copyFrom(VObject *);
	VTokenStream&	operator=(VString&);
	VTokenStream&	operator=(const VTokenStream&);

#ifndef CV_NOARCHIVER
	// Archiving
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:
	int				isnum(char *, int); 
	int				isnum(VWideChar *, int);
	void			init();

	//
	// tokens represents the VTokens for this VTokenStream.
	//
	// defTokens is set to TRUE to indicate that the tokens instance
	// variable is an internally constructed VTokens object and hence,
	// it will be destroyed when replaced or when this VTokenStream is
	// destroyed.  Otherwise, set to FALSE to indicate that the tokens
	// instance variable is provided by an external source and hence,
	// it should not be destroyed by this VTokenStream.
	//
	VTokens		   *tokens;
	boolean			defTokens;

};

extern VClass *VTokenStreamCls;

// ---------------------------------------------------------------------------
// Arrange for the current character of the "from" and "to" VStreams
// to be the first character of the last token obtained with getToken().
//
inline void VTokenStream::ungetToken()
{
	ungetTag();
}

// ---------------------------------------------------------------------------
// Put the Null terminated C string s out to the TO VStream.
//
inline void VTokenStream::putToken(char *s)
{
	putTag(NoTag, s);
}

// ---------------------------------------------------------------------------
// Put VString object s out to the TO VStream.
//
inline void VTokenStream::putToken(VString *s)
{
	putTag(NoTag, s);
}

// ---------------------------------------------------------------------------
//
inline VTokenStream& VTokenStream::operator=(VString& s)
{
	copyFrom(&s);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline void VTokenStream::init()
{
	VTokenStream::reset();
}

#endif // VTOKSTRM_H
