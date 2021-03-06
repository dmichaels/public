// $Header:   Q:/views/common/vcs/TOKSTRM.CPV   1.17   Mar 24 1997 16:13:52   scottt  $ 

//	tokstrm.cpp
//
//	VTokenStream class implementation [Common]
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
//	03/31/93 pkt	Fixed copyFrom to keep its own tokens as needed.
//	04/07/93 pmcm	changed id to VObject *
//	08/09/93 jmd	removed free
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened.
//	01/19/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	01/30/94 dgm	simplified -- expression in getToken() to
//					avoid apparent Borland C++ 4.0 compiler bug.
//					simplified ++ expression in adch() macro to
//					avoid apparent Borland C++ 4.0 compiler bug.
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	04/04/95 evc    made MBCS safe
//	04/11/95 pkt	updated for VWideChar.
//	04/11/95 pkt	updated for VWideCharAux.
//	04/12/95 pkt	worked around 'iswxxx' macro problem.
//	04/21/95 pkt	replaced MB_XXX with CV_MB_XXX.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	combined international changes into common version.
//	08/18/95 pkt	fixed MBCS safe 'getToken' to catch end-of-file.
//	11/29/95 tlf	updated for HPUX.
//	01/09/96 pkt	fixed 'isnum' to require initial digits, i.e. reject E12.
//	04/01/96 pkt	fixed so that ungetToken works within a VFile object.
//	04/08/96 glm	check for Symantec
//	04/18/96 pkt	fixed 4/1 fix, so that punctuation tokens are returned
//					correctly.
//	04/30/96 pkt	changed 'getToken' to perform better in single byte
//					character set locales.
//	10/10/96 pkt	boosted MAXtokenLength.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "tokstrm.h"
#include "tokens.h"

extern "C" {
#	include <ctype.h>
}

// ---------------------------------------------------------------------------
//
defineClass(VTokenStream, VTagStream)

// ---------------------------------------------------------------------------
//
#define isident(c) (isalnum(c) || c == '_')  
#ifndef __SC__
#define iswident(c) ((VWideCharAux::iswalnum)(c) || c == CV_WCHAR_CONST('_'))
#else
#define iswident(c) (VWideCharAux::iswalnum(c) || c == CV_WCHAR_CONST('_'))
#endif

#define MAXtokenLength 1024



// ---------------------------------------------------------------------------
// Reset this VTokenStream and select the default tokens (see 
// VTokens::defaultTokens()).
//
void VTokenStream::reset()
{
	if (!defTokens) {
		tokens = new VTokens;
		tokens->setDefault();
		defTokens = TRUE;
	}
	tokens->useTokens();
	VTagStream::reset();
}

// ---------------------------------------------------------------------------
// Use tok to define the tokens for this VStream.
//
void VTokenStream::useTokens(VTokens *tok)
{
	if (defTokens && tokens) {
		delete tokens;		// jmd (was free)
		defTokens = FALSE;
	}
	tokens = tok;
	tok->useTokens(); 
}

// ---------------------------------------------------------------------------
// Construct a VTokenStream from the VTokenStream ts.
//
VTokenStream::VTokenStream(const VTokenStream& ts) :
	tokens(0),
	defTokens(FALSE)
{
	init();
	copyFrom((VTokenStream *)(&ts));
}

// ---------------------------------------------------------------------------
// Construct a VTokenStream with default size and token types.
//
VTokenStream::VTokenStream() :
	tokens(0),
	defTokens(FALSE)
{
	VTokenStream::init();
}

// ---------------------------------------------------------------------------
//
VTokenStream& VTokenStream::operator=(const VTokenStream &ts)
{
	if (&ts != this) {
		reset();
		init();
		copyFrom((VTokenStream *)(&ts));
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
VTokenStream::~VTokenStream()
{
	if (defTokens && (tokens != 0)) {
		delete tokens;		// jmd (was free)
		tokens = 0;
		defTokens = FALSE;
	}
}

// ---------------------------------------------------------------------------
// Returns IntTok if s refers to a possibly signed integer, FloatTok
// if s refers to a standard floating point number, ala scanf(), or EndTok.
//
int VTokenStream::isnum(char *s, int next)
{
	int type = IntTok;
	boolean digit_seen = FALSE;
	s[next] = '\0';
	if (*s == '-' || *s == '+') {
		s++;
	}
	while (isdigit(*s)) {
		s++;
		digit_seen = TRUE;
	}
	if (*s == '.') {
		s++;
		type = FloatTok;
		while (isdigit(*s)) {
			s++;
			digit_seen = TRUE;
		}
	}
	if (*s == 'e' || *s == 'E') {
		s++;
		type = FloatTok;
		if (*s == '-' || *s == '+') {
			s++;
		}
		while (isdigit(*s)) {
			s++;
		}
	}
	if (*s != '\0' || !digit_seen) {
		type = EndTok; 
	}
	return type;
}  


// ---------------------------------------------------------------------------
// Wide character version of isnum()
//
int VTokenStream::isnum(VWideChar *s, int next)
{
	int type = IntTok;
	boolean digit_seen = FALSE;
	s[next] = CV_WCHAR_CONST('\0');
	if (*s == CV_WCHAR_CONST('-') || *s == CV_WCHAR_CONST('+')) {
		s++;
	}
#ifndef __SC__
	while ((VWideCharAux::iswdigit)(*s)) {
#else
	while (VWideCharAux::iswdigit(*s)) {
#endif
		s++;
		digit_seen = TRUE;
	}
	if (*s == CV_WCHAR_CONST('.')) {
		s++;
		type = FloatTok;
#ifndef __SC__
		while ((VWideCharAux::iswdigit)(*s)) {
#else
		while (VWideCharAux::iswdigit(*s)) {
#endif
			s++;
			digit_seen = TRUE;
		}
	}
	if (*s == CV_WCHAR_CONST('e') || *s == CV_WCHAR_CONST('E')) {
		s++;
		type = FloatTok;
		if (*s == CV_WCHAR_CONST('-') || *s == CV_WCHAR_CONST('+')) {
			s++;
		}
		while (isdigit(*s)) {
			s++;
		}
	}
	if (*s != '\0' || !digit_seen) {
		type = EndTok; 
	}
	return type;
}


// ---------------------------------------------------------------------------
//
#define addch(t, c) ((next < MAXtokenLength - 1) ? t[next] = c, next++ : 0)

// ---------------------------------------------------------------------------
// Read characters using getch() until a token is found.  Return
// the type of the token. The VTagStream member variables toLastAt
// and fromLastAt refer to the first character of the token.
//
int VTokenStream::getToken()
{
	return getToken(*(VString *)0);
}

// ---------------------------------------------------------------------------
// Read characters using getch() until a token is found placing the
// token into VString s.  Return the type of the token.
//
int VTokenStream::getToken(VString& s)
{
	int type = 0;
	// int onext;
	int next = 0;
	char value[MAXtokenLength];   
	VWideChar Wvalue[MAXtokenLength]; 
	
	char * t = value; 
	VWideChar * Wt = Wvalue;

	boolean speedy = getFromStream()->isAsByte() || SINGLE_BYTE_CHAR_SET;
	
	while (!type) { 
		if (speedy) {
			// Swallow white space.
			int c;  
			while (TRUE) {
				toLastAt = getToPos();
				fromLastAt = getFromPos();  
				if ((c = getch()) == EOF) {
					break;
				}
				if (tokens->isFirst(c)) {
					break;
				}
			}
			// I'm done. 
			if (c == EOF) {
				type = EndTok;
				break;
			}
			next = 0;
			if (iscntrl(c) || c == ' ') {
				// Got a control character
				addch(t, c);
				t[next] = '\0';
				if ((type = tokens->tokenOf(next, t)) != 0) {
					break;
				}
				else if (tokens->isOk(SPACE_OK) && isspace(c)) {
					type = SpaceTok;
				}
				else if (tokens->isOk(CNTRL_OK)) {
					type = CntrlTok;
				}
				else {
					next = 0;
				}
				continue;
			} 
			if (isident(c)) {
				// got an identifier or number
				number:
				addch(t, c);
				// collect ident characters
				while ((c = getch()) != EOF) {
					if (isident(c) ||
						(c == '.' && tokens->isOk(FLOAT_OK) && isnum(t, next))) {

						addch(t, c);
					}
					else {
						ungetch((char)c);
						break;
					}
				}			
            
				if (tokens->isOk(FLOAT_OK) || tokens->isOk(INT_OK)) {
					type = isnum(t, next);
				}
				// got an identifier? numbers can qualify as idents.
				if (type == 0) {
					t[next] = '\0';
					if ((type = tokens->tokenOf(next, t)) != 0) {
						break;
					}
					else if (tokens->isOk(IDENT_OK)) {
						type = IdentTok;
					}
					else {
						next = 0;
					}
					continue;
				}
				// got a float?
				if (type == FloatTok && tokens->isOk(FLOAT_OK)) {
					continue;
				}
				// got an int?
				if (type == IntTok && tokens->isOk(INT_OK)) {
					continue;
				}
				next = 0;
				continue;
			} 

			if (ispunct((char) c)) {
				// got a punctuation character
				int i = peekch();    
				// check to see if it is a signed number
				if ((((char)c == '-' || (char)c == '+') && (i == '.' || isdigit(i))) || 
					(((char)c == '.' || (char)c == '-' || (char)c == '+') && isdigit(i))) {

					if (tokens->isOk(FLOAT_OK) || tokens->isOk(INT_OK)) {
						goto number;
					}
				}

				// to_found_at and from_found_at hold the position for back-up after all
				// punctuation characters have been scanned. If no token is found, we
				// back up to the position which follows the first punctuation character,
				// which is the current position.
				integer to_found_at = getToPos();
				integer from_found_at = getFromPos();

				// collect punctuation characters
				int found_len = 0;
				int found_type = 0;
				while (TRUE) {
					addch(t, c);
					t[next] = '\0';
					type = tokens->tokenOf(next, t);
					if (type == 0 && next == 1 && tokens->isOk(DELIM_OK)) {
						type = DelimTok;
					}
					if (type != 0) {
						found_type = type;
						found_len = next;
						to_found_at = getToPos();
						from_found_at = getFromPos();
					}
 
					if (next >= tokens->getPunctLimit()) {
						break;
					}

					if ((c = getch()) == EOF) {
						break;
					}
					if (!ispunct(c)) {
						ungetch((char)c);
						break;
					}
				}     

				// if one or more unneeded punctuation characters processed,
				// adjust current pointers
				if (next != found_len) {
					integer saved_to = toLastAt;
					integer saved_from = fromLastAt;
					toLastAt = to_found_at;
					fromLastAt = from_found_at;
					ungetToken();
					toLastAt = saved_to;
					fromLastAt = saved_from;
				}

				type = found_type;
				next = found_len;
				continue;
			}
		}
		else {                         
			// MBCS 
			// Swallow white space.
			VWideChar  c;
			boolean end_of_file;  
			while (TRUE) {
				char mbc[CV_MB_LEN_MAX];
				toLastAt = getToPos();
				fromLastAt = getFromPos();  
				if (end_of_file = !getch(c)) {
					break;
				}
				if (VWideCharAux::wctomb(mbc, c) == 1) {
					if (tokens->isFirst(mbc[0]& 0177)) {
						break;
					}
				}
				else {
					// This is a first character that is a 2 (or more) byte character.
					// for internationalization sake we let them start a token.
					break;
				}
			}
			// I'm done. 
			
			if (end_of_file) {
				type = EndTok;
				break;
			}

			next = 0;
	
#ifndef __SC__
			if ((VWideCharAux::iswcntrl)(c) || c == CV_WCHAR_CONST(' ')) {
#else
			if (VWideCharAux::iswcntrl(c) || c == CV_WCHAR_CONST(' ')) {
#endif
				// Got a control character
				addch(Wt, c);
				Wt[next] = '\0';
				if ((type = tokens->tokenOf(next, Wt))) {
					break;
				}
				else if (tokens->isOk(SPACE_OK) && isspace(c)) {
					type = SpaceTok;
				}
				else if (tokens->isOk(CNTRL_OK)) {
					type = CntrlTok;
				}
				else {
					next = 0;
				}
				continue;
			} 
			if (iswident(c)) {
				// got an identifier or number
				wnumber:
				addch(Wt, c);
				// collect ident characters
				while (getch(c)) {
					if (iswident(c) ||
						(c == CV_WCHAR_CONST('.') && tokens->isOk(FLOAT_OK) && isnum(Wt, next))) {

						addch(Wt, c);
					}
					else {
						ungetch(c);
						break;
					}
				}			
            
				if (tokens->isOk(FLOAT_OK) || tokens->isOk(INT_OK)) {
					type = isnum(Wt, next);
				}
				// got an identifier? numbers can qualify as idents.
				if (type == 0) {
					Wt[next] = '\0';
					if (type = tokens->tokenOf(next, Wt)) {
						break;
					}
					else if (tokens->isOk(IDENT_OK)) {
						type = IdentTok;
					}
					else {
						next = 0;
					}
					continue;
				}
				// got a float?
				if (type == FloatTok && tokens->isOk(FLOAT_OK)) {
					continue;
				}
				// got an int?
				if (type == IntTok && tokens->isOk(INT_OK)) {
					continue;
				}
				next = 0;
				continue;
			} 

#ifndef __SC__
			if ((VWideCharAux::iswpunct)(c)) {
#else
			if (VWideCharAux::iswpunct(c)) {
#endif
				// got a punctuation character
				VWideChar i; 
				peekch(i);    
			
				// check to see if it is a signed number
#ifndef __SC__
				if (((c == CV_WCHAR_CONST('-') || c == CV_WCHAR_CONST('+')) && (i == CV_WCHAR_CONST('.') || (VWideCharAux::iswdigit)(i))) || 
					((c == CV_WCHAR_CONST('.') || c == CV_WCHAR_CONST('-') || c == CV_WCHAR_CONST('+')) && ((VWideCharAux::iswdigit)(i)))) {
#else
				if (((c == CV_WCHAR_CONST('-') || c == CV_WCHAR_CONST('+')) && (i == CV_WCHAR_CONST('.') || VWideCharAux::iswdigit(i))) || 
					((c == CV_WCHAR_CONST('.') || c == CV_WCHAR_CONST('-') || c == CV_WCHAR_CONST('+')) && (VWideCharAux::iswdigit(i)))) {
#endif
					if (tokens->isOk(FLOAT_OK) || tokens->isOk(INT_OK)) {
						goto wnumber;
					}
				}

				// to_found_at and from_found_at hold the position for back-up after all
				// punctuation characters have been scanned. If no token is found, we
				// back up to the position which follows the first punctuation character,
				// which is the current position.
				integer to_found_at = getToPos();
				integer from_found_at = getFromPos();

				// collect punctuation characters
				int found_len = 0;
				int found_type = 0;
				while (TRUE) {
					addch(Wt, c);
					Wt[next] = CV_WCHAR_CONST('\0');
					type = tokens->tokenOf(next, Wt);
					if (type == 0 && next == 1 && tokens->isOk(DELIM_OK)) {
						type = DelimTok;
					}
					if (type != 0) {
						found_type = type;
						found_len = next;
						to_found_at = getToPos();
						from_found_at = getFromPos();
					}

					if (next >= tokens->getPunctLimit()) {
						break;
					}

					if (!getch(c)) {
						break;
					}
#ifndef __SC__
					if (!(VWideCharAux::iswpunct)((VWideChar )c)) {
#else
					if (!VWideCharAux::iswpunct((VWideChar )c)) {
#endif
						ungetch(c);
						break;
					}
				}     

				// if one or more unneeded punctuation characters processed,
				// adjust current pointers
				if (next != found_len) {
					integer saved_to = toLastAt;
					integer saved_from = fromLastAt;
					toLastAt = to_found_at;
					fromLastAt = from_found_at;
					ungetToken();
					toLastAt = saved_to;
					fromLastAt = saved_from;
				}

				type = found_type;
				next = found_len;
				continue;
			}
		} 
	} // end while (!type)

	if (speedy) {		
		t[next] = '\0'; 
	
		if (&s) {
			s = value;
		}
	} 
	else {	
		Wt[next] = CV_WCHAR_CONST('\0'); 
		if (&s) {
			s = Wvalue;
		}
	} 
	
	return type;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store this VTokenStream into VArchiver a. The current VTokens
// is not written.
//
void VTokenStream::putTo(VArchiver &a)
{
	VTagStream::putTo(a);
}

// ---------------------------------------------------------------------------
// Read this VTokenStream from VArchiver 'a'. The current VTokens
// is set to the default.
//
void VTokenStream::getFrom(VArchiver &a)
{
	VTagStream::getFrom(a);
	tokens = new VTokens;
	tokens->setDefault();
	defTokens = TRUE;
	tokens->useTokens();
}

// ----------------------------------------------------------------------------
//
void VTokenStream::getFrom(VArchiver& a,
						   VObject * /* data */ , boolean /* do_realize */ )
{
	getFrom(a);
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// A VTokenStream can copy from a VStream object.
//
boolean VTokenStream::copyFrom(VObject *obj)
{
	if (!VTagStream::copyFrom(obj)) {
		return FALSE;
	}
	if (!obj->isA(VTokenStreamCls)) {
		return TRUE;
	}
	VTokenStream *ts = (VTokenStream *)obj;
	defTokens = ts->defTokens;
	if (defTokens && ts->tokens != 0) {
		tokens = new VTokens(*ts->tokens);
	}
	else {
		tokens = ts->tokens;
	}
	return TRUE;
}
