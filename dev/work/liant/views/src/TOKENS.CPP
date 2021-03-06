// $Header:   Q:/views/common/vcs/TOKENS.CPV   1.12   Mar 24 1997 16:13:50   scottt  $ 

//	tokens.cpp
//
//	VTokens class implementation [Common]
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
//	04/16/93 pmcm	added matching va_end for va_start
//	08/09/93 jmd	removed free
//	09/21/93 pmcm	changed idptr to VObject **
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	06/06/94 jld	removed VMemory::memory from clean() and useTokens()
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	09/08/94 dgm	Minor coding change to suppress MetaWare C++ warning.
//	04/10/95 pkt	merged Mac and internationalized versions.
//	04/11/95 pkt	updated for VWideChar.
//	04/19/95 jld	clean() deletes charToks
//	05/26/95 pkt	combined international changes into common.
//	06/19/95 pkt	fixed 5/26 'useTokens' to not scribble on memory.
//	11/02/95 dgm	Moved tokenOf() from tokens.h to suppress warnings
//					about loops within inline functions.
//	03/05/96 pkt	changed 'char *' to 'const char *' in 'add'.
//	04/30/96 pkt	added 'getPunctLimit'; changed 'chars' back to char*.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "tokstrm.h"
#include "str.h"
#include "tokens.h"
#include "iterator.h"
#include "vwchar.h"

extern "C" {
#	include <string.h>
#	include <stdarg.h>
}

// ---------------------------------------------------------------------------
//
defineClass(VTokens, VDictionary)

// ---------------------------------------------------------------------------
// Return the "ok" value associated with token type. If type
// is not one of the built-in types, then return zero.
//
static int getOk(int type) 
{
	switch (type) {
	case IntTok:
		return INT_OK;
	case DelimTok:
		return DELIM_OK;
	case FloatTok:
		return FLOAT_OK;
	case CntrlTok:
		return CNTRL_OK;
	case IdentTok:
		return IDENT_OK;
	case SpaceTok:
		return SPACE_OK;
	default:
		return 0;
	}
}

// ---------------------------------------------------------------------------
//
void VTokens::useTokens()
{
	int i;
	pointer p;
	VObject ** ip;
	VAssoc *a;

	if (!(ok & DELTA_TOKS)) {
		return;
	}
	ok &= ~DELTA_TOKS;
	for (i = 0; i < 128; i++) {
		firstChar[i] = 0;
	}
	if ((ok & INT_OK) || (ok & FLOAT_OK)) {
		for (i = '0'; i <= '9'; i++) {
			firstChar[i]++;
		}
	}

	if (ok & DELIM_OK) {
		for (i = '!'; i <= '/'; i++) {
			firstChar[i]++;
		}
		for (i = ':'; i <= '@'; i++) {
			firstChar[i]++;
		}
		for (i = '['; i <= '`'; i++) {
			firstChar[i]++;
		}
		for (i = '{'; i <= '~'; i++) {
			firstChar[i]++;
		}
	}

	if (ok & CNTRL_OK) {
		for (i = 1; i <= 37; i++) {
			firstChar[i]++;
		}
		firstChar[0177]++;
		firstChar['\r'] =
		firstChar['\n'] =
		firstChar[' ']  =
		firstChar['\f'] =
		firstChar['\t'] = 0;
	}

	if (ok & SPACE_OK) {
		firstChar['\r'] =
		firstChar['\n'] =
		firstChar[' ']  =
		firstChar['\f'] =
		firstChar['\t'] = 1;
	}
	if (ok & IDENT_OK) {
		for (i = 'A'; i <= 'Z'; i++) {
			firstChar[i]++;
		}
		for (i = 'a'; i <= 'z'; i++) {
			firstChar[i]++;
		}
		firstChar['_']++;
	}
	if (charToks != 0) {
		char *s;
		if (chars != 0) {
			delete [] chars;
		}
		integer n_chars = charToks->size();
		chars = new char[n_chars + 1];

		int p;
		const char *ct_chars = charToks->gets();
	    for (p = 0; p <= n_chars; p++) {
	    	chars[p] = ct_chars[p];  
	    }  
	    chars[n_chars] = 0;
		
		for (s = chars; *s; s++) {
			firstChar[*s & 0177]++;
		}
	}

	// Determine length of longest all-punctuation token.
	punctLimit = 0;

	integer n = contents.size();
	if (n == 0) {
		return;
	}
	ip = (VObject **)contents.getData();
	for (i = 0; i < n; i++) {
		a = (VAssoc *)ip[i];
		if (a != 0) {
			//if ((p = ((VString *)a->key)->getData()) != 0) {
			VString *key_string = (VString *)a->key;
			if (p = (char *)key_string->gets()) {
				firstChar[*p & 0177]++;
			}

			// Check for longer all-punctuation token.
			integer len = key_string->getCharSize();
			boolean all_punct = TRUE;
			for (integer k = 0; k < len; k += 1) {
#ifndef __SC__
				boolean pun = (VWideCharAux::iswpunct)(key_string->wcharAt(k));
#else
				boolean pun = VWideCharAux::iswpunct(key_string->wcharAt(k));
#endif
				if (!pun) {
					all_punct = FALSE;
					break;
				}
			}
			if (all_punct && len > punctLimit) {
				punctLimit = len;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Construct a VTokens object with no token types defined.
//
VTokens::VTokens()
{
	init();
}

// ---------------------------------------------------------------------------
//
void VTokens::init()
{
	chars = 0;
	charToks = 0;
	reset();
}

// ---------------------------------------------------------------------------
// Reset this VTokens object so that the following token types are
// defined: IdentTok, IntTok, FloatTok, DelimTok, CntrlTok and
// SpaceTok.
//
void VTokens::setDefault()
{
	reset();
	ok |= INT_OK | FLOAT_OK | DELIM_OK | IDENT_OK | CNTRL_OK | SPACE_OK;
}

// ---------------------------------------------------------------------------
// Reset this VTokens object so that no token types are defined.
//
void VTokens::reset()
{
	if (charToks) {
		charToks->puts("");
	}
	ok = DELTA_TOKS;
	punctLimit = 0;
	VDictionary::reset();
}

// ---------------------------------------------------------------------------
// Add the token types given as arguments. The last argument must
// be EndTok. If a type was already defined, return FALSE, otherwise
// return TRUE.
//
boolean VTokens::add(int i, int j, ...)
{
	va_list a;
	int ret = TRUE;
	if (i == 0) {
		return TRUE;
	}
	if (!add(i)) {
		ret = FALSE;
	}
	ok |= DELTA_TOKS;
	if (!j) {
		return TRUE;
	}
	if (!add(j)) {
		ret = FALSE;
	}
	va_start(a, j);
	while ((i = va_arg(a, int)) != 0) {
		if (!add(i)) {
			ret = FALSE;
		}
	}
	va_end(a);
	return ret;
}

// ---------------------------------------------------------------------------
// Define the token type 'type' as the VString 'value'.  Return FALSE
// if 'type' was already defined, otherwise return TRUE.
//
boolean VTokens::add(int type, VString *value)
{
	VString *s = (VString *)value->copy();
	if (getOk(type)) {
		return FALSE;
	}
	if ((s == 0) || !atKeyPut(s, type) ? TRUE: FALSE) {
		if (s != 0) {
			delete s;		// jmd (was free)
		}
		return FALSE;
	}
	ok |= DELTA_TOKS;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Define the token type 'type' as the string 'value'.  Return FALSE
// if 'type' was already defined, otherwise return TRUE.
//
boolean VTokens::add(int type, const char *value)
{
	VString *s;
	if (getOk(type)) {
		return FALSE;
	}
	if ((s = new VString(value)) == 0) {
		return FALSE;
	}
	if (!atKeyPut(s, type)) {
		delete s;		// jmd (was free)
		return FALSE;
	}
	ok |= DELTA_TOKS;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Define the token type 'type'.  If type is not a built-in type,
// then it is defined as a token with type 'type' and a value of the single
// character 'type'.  Return FALSE if type was already defined, otherwise
// return TRUE.
//
boolean VTokens::add(int type)
{
	int n;
	if ((n = getOk(type)) != 0) {
		ok |= n;
		return TRUE;
	}

	if ((charToks == 0) && ((charToks = new VString) == 0)) {
		return FALSE;
	}
	if (charToks->charIn((char)type)) {
		return FALSE; // type already defined
	}
	VWideChar wc;
	if (!VWideCharAux::atowc(wc, type)) {
		// 'type' can't be a character by itself.
		return FALSE;
	}
	charToks->concat((char)type);
	ok |= DELTA_TOKS;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Remove the tokens given as arguments, via remove(int).
// The last Argument must be EndTok.
//
void VTokens::remove(int i, int j, ...)
{
	va_list a;
	if (i != 0) {
		remove(i);
	}
	else {
		return;
	}
	if (j != 0) {
		remove(j);
	}
	else {
		return;
	}
	va_start(a, j);
	while ((i = va_arg(a, int)) != 0) {
		remove(i);
	}
	va_end(a);
	ok |= DELTA_TOKS;
}

// ---------------------------------------------------------------------------
// Remove the token type 'type'.
//
boolean VTokens::remove(int type)
{
	VString *str;
	pointer p;
	int i, count = 0;

	if ((i = getOk(type)) != 0) {
		ok &= ~i;
		return TRUE;
	}
	if ((charToks != 0) && charToks->charIn((char)type)) {
	//p = charToks->getData();
		p = (char *) charToks->gets();
		while (*p != type) {
			p++;
		}
		while ((*p = p[1]) != 0) {
			p++;
		}
		ok |= DELTA_TOKS;
		return TRUE;
	}

	DO (this, VAssoc, a)
		if ((int)(long)a->value == type) {
			removeKey((VObject *)(str = (VString *)a->key));
			delete str;		// jmd (was free)
			count++;
		}
	END 

	if (count == 0) {
		return FALSE;
	}
	ok |= DELTA_TOKS;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
VTokens::~VTokens()
{
	clean();
}

// ---------------------------------------------------------------------------
//
void VTokens::clean()
{
	VString *s;

	DO (this, VAssoc, a) 
		s = (VString *)a->key;
		delete s;	// jmd (was free)
	END 

	if (chars != 0) {
		delete [] chars;
	}
	if (charToks != 0) {
		delete charToks;
	}
}

// ---------------------------------------------------------------------------
// Copy the contents of VTokens object obj to this VTokens.
//
boolean VTokens::copyFrom(VObject *o)
{
	if (!VDictionary::copyFrom(o)) {
		return FALSE;
	}
	if (!o->isA(VTokensCls)) {
		return TRUE;
	}
	VTokens *t = (VTokens *)o;
	if (t->charToks != 0) {
		charToks = (VString *)t->charToks->copy();
	}
	chars = 0;
	ok = t->ok;
	ok |= DELTA_TOKS;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return the key associated with s.
//
int VTokens::getKey(char *s)
{
	VString st(s);

	return int(long(getValueAtKey(&st)));
}  

// ---------------------------------------------------------------------------
// Return the key associated with s.
//
int VTokens::getKey(VWideChar *ws)
{
	VString st(ws);

	return int(long(getValueAtKey(&st)));
}


// ---------------------------------------------------------------------------
// Construct a VTokens object from VTokens object toks.
//
VTokens::VTokens(const VTokens& t)
{
	init();
	copyFrom((VTokens *)(&t));
}

// ---------------------------------------------------------------------------
// Return the token associated with the string s.  Return 0
// if it isn't defined or return a -1 if there is a fault.
//
int VTokens::tokenOf(unsigned i, char *s)
{  
	boolean char_token = ((i == 1) && (chars != 0) && (strchr(chars, *s) != 0));   
	return char_token ? *s
				      : ((count() != 0) ? getKey(s) : 0);
}

// ---------------------------------------------------------------------------
// Wide character string being processed, otherwise same as above.
//
int VTokens::tokenOf(unsigned i, VWideChar *ws)
{  
	boolean char_token = FALSE;
	char c; 
	if (VWideCharAux::wctoa(c, *ws) && i == 1 && chars != 0) {   
		char_token = (strchr(chars, c) != 0);
	}  
	return (char_token) ? c
				        : ((count() != 0) ? getKey(ws) : 0);
}
