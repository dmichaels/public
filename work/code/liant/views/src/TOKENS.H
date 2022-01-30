// $Header:   Q:/views/common/vcs/TOKENS.H_V   1.16   Mar 24 1997 16:13:50   scottt  $ 

//	tokens.h
//
//	VTokens class interface [Common]
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
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	04/11/95 pkt	updated for VWideChar.
//	05/26/95 pkt	combined international changes into common version.
//	08/08/95 pkt	fixed 'isFirst' for char type that is signed by default.
//	11/02/95 dgm	Moved tokenOf() to tokens.cpp to suppress warnings
//					about loops within inline functions.
//	03/05/96 pkt	changed 'char *' to 'const char *' in 'add'.
//	04/30/96 pkt	restored 'tokenOf(unsigned, char*)'.
//	04/30/96 pkt	changed 'chars' back to char*.
//	04/30/96 pkt	added 'getPunctLimit'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTOKENS_H
#define VTOKENS_H

// ---------------------------------------------------------------------------
//
#include "str.h"
#include "dictinry.h"

extern "C" {
#include <string.h>
}

// ---------------------------------------------------------------------------
//
#define INT_OK			  01
#define FLOAT_OK		  02
#define DELIM_OK		  04
#define IDENT_OK		 010
#define CNTRL_OK		 020
#define SPACE_OK		 040

#define DELTA_TOKS		0100

#define EndTok			   0
#define IntTok			   1
#define FloatTok		   2
#define DelimTok		   3
#define IdentTok		   4
#define CntrlTok		   5
#define SpaceTok		  11

// ---------------------------------------------------------------------------
// The VTokens class defines the tokens to be scanned by VTokenStream.
// A token is zero or more contiguous characters in a VStream.  A token has a
// type and a value.  A token type may either be user-specified or a built-in
// integer value.  A token value is a string of characters which comprise the
// token.
//
// The following built-in token types are be recognized:
//
// IntTok		- possibly a sign followed by one or more contiguous digits, 
//				  not followed by a . or exponent;
// FloatTok		- a floating-point value, fixed point or exponential,
//				  that is possibly a sign followed by zero or more digits,
//				  possibly followed by a period (.) and zero or more digits, 
//				  possibly followed by a e or E, possibly a sign and one or
//				  more digits; 
// DelimTok		- one ASCII punctuation character;
// IdentTok		- one or more contiguous identifier characters which do not
//				  comprise an integer or float value; Identifier characters
//				   are alpha-numeric ASCII plus underscore (_);
// CntrlTok		- An ASCII control character which is not a space;
// SpaceTok		- An ASCII space character NL (\\n), CR (\\r), SPACE ( )
//				  and FF (\\f).
// 
// The reserved EndTok token matches no character upon end-of-stream. 
// 
// The type of a user-specified token is an integer value provided via an
// add() call to a VTokens object.  The value of a user-specified token is
// one or more characters subject to the following constraints.
// The value must be:
//		(a) one or more ASCII identifier characters;
//		(b) one ASCII control character; or
//		(c) one or more ASCII punctuation characters.
//
// User-specified tokens have precedence over built-in tokens. That is, 
// if a set of characters matches a user-specified token and a built-in
// token, the user-specified token is matched and returned.
//
// The ASCII NULL character is always ignored.
//
// VStream characters which do not match a defined token type, are ignored. 
//
CLASS VTokens : public VDictionary {
public:
	// Constructors & Destructor
					VTokens();
					VTokens(const VTokens&);
				   ~VTokens();

	VClass		   *iam();

	void			useTokens();

	boolean			isOk(int i);
	boolean			isFirst(char c);

	int				tokenOf(unsigned i, char *s);  
	int				tokenOf(unsigned i, VWideChar *ws);

	// Specifying Token Types
	boolean			add(int);
	boolean			add(int, int...);
	boolean			add(int, const char *);
	boolean			add(int, VString *);
	VObject		   *add(VObject *obj);

	// Removing Token Types
	boolean			remove(int );
	void			remove(int, int, ...);
	void			reset();
	void			setDefault();

	// Info
	int				getPunctLimit() const;

	// Copying
	boolean			copyFrom(VObject *);
	VTokens&		operator=(const VTokens&);

protected:
	void		clean();
	int			getKey(char *); 
	int			getKey(VWideChar *);
	void		init();

	ushort		ok;	 			// which built-in token types are recognized
	VString	   *charToks;		// single char tokens who are their own value
	char	   *chars;			// pointer to the contents of charToks expressed
								// as an array of char
	char		firstChar[128];	// boolean values used to speed up scanning
	int			punctLimit;		// longest all-punctuation token

};

extern VClass *VTokensCls;

// ---------------------------------------------------------------------------
//
inline boolean VTokens::isOk(int i)
{
	return (ok & i) ? TRUE: FALSE;
}

// ---------------------------------------------------------------------------
//
inline boolean VTokens::isFirst(char c)
{  
	if ((unsigned char)c > 127) {
		return FALSE;
	}
	return firstChar[c];
}

// ---------------------------------------------------------------------------
// Add 'obj' to this Token set and return it. 
//
inline VObject *VTokens::add(VObject *obj)
{
	return VSet::add(obj);
}

// ---------------------------------------------------------------------------
// Copy the contents of VTokens object obj to this VTokens.
//
inline VTokens& VTokens::operator=(const VTokens& t)
{
	if (&t != this) {
		reset();
		init();
		copyFrom((VTokens *)(&t));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
inline int VTokens::getPunctLimit() const
{
	return punctLimit;
}

#endif // VTOKENS_H
