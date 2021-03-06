// $Header:   Q:/views/common/vcs/vwchar.cpv   1.17   Jun 20 1997 13:34:22   scottt  $

//	vwchar.cpp
//									 
//	Implementation of substitute wchar.h functions [Common]
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
//	02/14/95 pkt	originated.
//	03/02/95 evc	added defs.h to expose WCHAR_H_EXISTS.
//	03/17/95 pkt	added 'iswspace'.
//	04/04/95 evc	added iswalpha   & iswpunct
//	04/04/95 evc	made iswdigit and iswspace public
//	04/11/95 pkt	updated for VWideChar and VWideCharAux.
//	04/12/95 pkt	worked around 'iswxxx' macro problem.
//	04/18/95 pkt	implemented multibyte string functions for Macintosh.
//	04/21/95 pkt	replaced MB_XXX with CV_MB_XXX.
//	08/17/95 pkt	added standard-conforming version of 'wcstok'.
//	08/21/95 pkt	fixed to include the right headers.
//	08/30/95 pkt	added 'swscanf', 'strcoll', 'tolower', 'toupper',
//					'towlower', 'towupper', 'wcstol', 'wcstoul', and 'wcstod'.
//	08/31/95 pkt	implemented Win 16 support.
//	08/31/95 pkt	added 'checkLocale'.
//	09/05/95 pkt	fixed hang in 'wcstoul' which affected 'swscanf' etc.
//	09/06/95 pkt	updated Mac discriminator to V_MACINTOSH.
//	09/07/95 pkt	fixed #ifdef syntax; added include of <Resources.h>.
//	09/18/95 pkt	added 'mbslen' functions.
//	10/03/95 pkt	picked up 'wctoa' and 'atowc' from VString.
//	10/11/95 pkt	revised for updated 'getByteTypeTablePtr' interface.
//	11/29/95 tlf	updated for HPUX.
//	12/15/95 pkt	changed 'checkLocale' to support Motif initialization.
//	03/06/96 pkt	fixed 'wcstombs' to not scribble out of bounds.
//	06/06/96 pkt	fixed 'wcstok' to not go out of bounds.
//	12/04/96 pkt	changed to compile on SunOS.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//	06/20/97 pkt	fixed 'wcstok' to return a correct value.
// ---------------------------------------------------------------------------
#include "defs.h"
#include "vwchar.h"
#include "notifier.h"

#if defined(V_MACINTOSH)
#include <Resources.h>
#endif

static int isdpoint(VWideChar wc);

// ===========================================================================
//   Functions Always Defined
//

#include <stdio.h>
#include <stdarg.h>
#include <locale.h>
#include "str.h"

// ---------------------------------------------------------------------------
// Return whether the character is a decimal point.
//
static int isdpoint(VWideChar wc)
{
	static VString *pdpoints = 0;

	if (pdpoints == 0) {
#if !defined(CV_SYNTHESIZE_WCHAR)
		VWideCharAux::checkLocale();
#endif
		struct lconv *plconv = localeconv();
		char *dpcs = plconv != 0 ? plconv->decimal_point : ".";
		pdpoints = new VString(dpcs);
		if (pdpoints != 0 && pdpoints->size() == 0) {
			(*pdpoints) = ".";
		}
	}
	if (pdpoints != 0) {
		return pdpoints->charIn(wc);
	}
	return wc == CV_WCHAR_CONST('.');
}

// ---------------------------------------------------------------------------
// Scan a decimal integer from the wide character string pointed to by
// current_in.  Advance current_in to the next character beyond any decimal
// digits found.  Set value to the integral value found in the string.
// Return whether any digits were processed.
//
static int scanDecimalInteger(const VWideChar *&current_in,
									 long& value)
{
	const VWideChar *endptr;
	value = VWideCharAux::wcstol(current_in, (VWideChar **)&endptr, 10);
	int value_present = endptr != current_in;
	current_in = endptr;
	return value_present;
}

// ---------------------------------------------------------------------------
// Analyze the given character string f for a printf-style conversion sequence.
// Return the number of characters in the conversion sequence, or 0 if the
// string is not a recognizable conversion sequence.
//
// If the specifier is 'n' then the value of so_far is stored.
//
// Sets 'consumed' to the number of characters of input consumed
// before the end of the matched specification.
//
static int scanOneSpec(const VWideChar *f,
					   const VWideChar *in,
					   long &consumed,
					   VWideChar &spec,
					   boolean &premature_eof,
					   long so_far,
					   va_list args)
{
	premature_eof = FALSE;
	if (f == 0 || *f != CV_WCHAR_CONST('%')) {
		return 0;
	}

	// % character introduces a conversion specification.
	const VWideChar *current_f = f + 1;

	// Look for suppression of assignment.
	int suppress_assignment = (*current_f == CV_WCHAR_CONST('*'));
	if (suppress_assignment) {
		current_f += 1;
	}

	// Look for field width specification.
	long field_width = 0;
	int field_width_present = scanDecimalInteger(current_f, field_width);

	// Look for receiver size specification.
	VWideChar sizer = *current_f;
	if (sizer == CV_WCHAR_CONST('h') || sizer == CV_WCHAR_CONST('l') || sizer == CV_WCHAR_CONST('L')) {
		// indication of size of receiving object
		current_f += 1;
	}
	else {
		sizer = 0;
	}

	const VWideChar specifier = spec = *current_f;
	current_f += 1;
	const VWideChar *current_in = in;
#ifndef __SC__
	if ((VWideCharAux::iswalpha)(specifier)) {
#else
	if (VWideCharAux::iswalpha(specifier)) {
#endif
		switch (specifier) {
		case CV_WCHAR_CONST('d'):
		case CV_WCHAR_CONST('i'):
			{
				const VWideChar *endptr;
				long value = VWideCharAux::wcstol(current_in,
												  (VWideChar **)&endptr,
												  specifier == CV_WCHAR_CONST('d') ? 10 : 0);
				if (sizer == 0) {
					int *p = va_arg(args, int*);
					if (endptr != current_in && p != 0) {
						*p = int(value);
						if (value != *p) {
							if (value > 0) {
								*p = INT_MAX;
							}
							else {
								*p = INT_MIN;
							}
						}
					}
				}
				else if (sizer == CV_WCHAR_CONST('h')) {
					short *p = va_arg(args, short*);
					if (endptr != current_in && p != 0) {
						*p = short(value);
						if (value != *p) {
							if (value > 0) {
								*p = SHRT_MAX;
							}
							else {
								*p = SHRT_MIN;
							}
						}
					}
				}
				else if (sizer == CV_WCHAR_CONST('l')) {
					long *p = va_arg(args, long*);
					if (endptr != current_in && p != 0) {
						*p = value;
					}
				}
				current_in = endptr;
			}
			break;
		case CV_WCHAR_CONST('o'):
		case CV_WCHAR_CONST('u'):
		case CV_WCHAR_CONST('x'):
		case CV_WCHAR_CONST('X'):
			{
				const VWideChar *endptr;
				int base = specifier == CV_WCHAR_CONST('o') ? 8 :
						   specifier == CV_WCHAR_CONST('u') ? 10 :
						   16;
				unsigned long value = VWideCharAux::wcstoul(current_in,
															(VWideChar **)&endptr,
															base);
				if (sizer == 0) {
					unsigned int *p = va_arg(args, unsigned int*);
					if (endptr != current_in && p != 0) {
						*p = (unsigned int)value;
						if (value != *p) {
							*p = UINT_MAX;
						}
					}
				}
				else if (sizer == CV_WCHAR_CONST('h')) {
					unsigned short *p = va_arg(args, unsigned short*);
					if (endptr != current_in && p != 0) {
						*p = (unsigned short)value;
						if (value != *p) {
							*p = USHRT_MAX;
						}
					}
				}
				else if (sizer == CV_WCHAR_CONST('l')) {
					unsigned long *p = va_arg(args, unsigned long*);
					if (p != 0) {
						*p = value;
					}
				}
				current_in = endptr;
			}
			break;
		case CV_WCHAR_CONST('e'):
		case CV_WCHAR_CONST('E'):
		case CV_WCHAR_CONST('f'):
		case CV_WCHAR_CONST('g'):
		case CV_WCHAR_CONST('G'):
			{
				const VWideChar *endptr;
				double value = VWideCharAux::wcstod(current_in,
													(VWideChar **)&endptr);
				if (sizer == CV_WCHAR_CONST('l')) {
					double *p = va_arg(args, double*);
					if (endptr != current_in && p != 0) {
						*p = value;
					}
				}
				else {
					float *p = va_arg(args, float*);
					if (endptr != current_in && p != 0) {
						*p = float(value);
					}
				}
				current_in = endptr;
			}
			break;
		case CV_WCHAR_CONST('s'):
			{
				VWideChar *p = va_arg(args, VWideChar *);
				for (;;) {
#ifndef __SC__
					if ((VWideCharAux::iswspace)(*current_in)) {
#else
					if (VWideCharAux::iswspace(*current_in)) {
#endif
						if (p != 0) {
							*p = CV_WCHAR_CONST('\0');
						}
						break;
					}
					if (p != 0) {
						*p = *current_in;
						p += 1;
					}
					current_in += 1;
				}
			}
			break;
		case CV_WCHAR_CONST('c'):
			{
				if (!field_width_present) {
					field_width = 1;
				}
				VWideChar *p = va_arg(args, VWideChar *);
				long count = 0;
				for (;;) {
					if (*current_in == CV_WCHAR_CONST('\0') || count >= field_width) {
						break;
					}
					if (p != 0) {
						*p = *current_in;
						p += 1;
					}
					count += 1;
					current_in += 1;
				}
				premature_eof = count < field_width;
			}
			break;
		case CV_WCHAR_CONST('p'):
			{
				void **p = va_arg(args, void **);
				const VWideChar *endptr;
				unsigned long value = VWideCharAux::wcstoul(current_in,
															(VWideChar **)&endptr,
															0);
				if (endptr != current_in) {
					if (p != 0) {
						*p = (void *)value;
					}
				}
				current_in = endptr;
			}
			break;
		case CV_WCHAR_CONST('n'):
			{
				if (sizer == 0) {
					int *p = va_arg(args, int*);
					if (p != 0) {
						*p = int(so_far);
					}
				}
				else if (sizer == CV_WCHAR_CONST('h')) {
					short *p = va_arg(args, short*);
					if (p != 0) {
						*p = short(so_far);
					}
				}
				else if (sizer == CV_WCHAR_CONST('l')) {
					long *p = va_arg(args, long*);
					if (p != 0) {
						*p = so_far;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else if (specifier == CV_WCHAR_CONST('[')) {
		boolean negate = FALSE;
		if (*current_f == CV_WCHAR_CONST('^')) {
			negate = TRUE;
			current_f += 1;
		}
		VString scanlist;
		if (*current_f == CV_WCHAR_CONST(']')) {
			scanlist += *current_f;
			current_f += 1;
		}
		while (*current_f != CV_WCHAR_CONST('\0') && *current_f != CV_WCHAR_CONST(']')) {
			scanlist += *current_f;
			current_f += 1;
		}
		if (*current_f == CV_WCHAR_CONST(']')) {
			current_f += 1;
		}
		else {
			// no matching bracket
			return 0;
		}
		VWideChar *p = va_arg(args, VWideChar *);
		for (;;) {
			if (   *current_in == CV_WCHAR_CONST('\0')
				|| scanlist.charIn(*current_in) == negate) {

				if (p != 0) {
					*p = CV_WCHAR_CONST('\0');
				}
				break;
			}
			if (p != 0) {
				*p = *current_in;
				p += 1;
			}
			current_in += 1;
		}
	}
	else {
		// Looked like a conversion specifier directive, but
		// syntactically incorrect.
		return 0;
	}
	consumed = current_in - in;
	return current_f - f;
}

// ---------------------------------------------------------------------------
// Like sscanf, except the input and format strings are both composed of
// wide characters.  Similarly, conversion specs such as %s must refer to
// arrays of wide characters.
//
int VWideCharAux::swscanf(const VWideChar *input, const VWideChar *format, ...)
{
	// Even though MSVC2.1 has a proper implementation of swscanf,
	// we can't simply call it from here, because there's no way to pass
	// along the variable part of the argument list.  The implementation
	// below is not rigorous in its error handling.

	if (input == 0) {
		return EOF;
	}
	int n_items_assigned = 0;
	va_list args;
	va_start(args, format);
	const VWideChar *current_in = input;
	const VWideChar *current_f = format;
	for (;;) {
		// get a directive
		boolean input_fails = FALSE;
		boolean matching_fails = FALSE;
		boolean format_fails = FALSE;
		if (iswspace(*current_f)) {
			// white space
			do {
				current_f += 1;
			} while (iswspace(*current_f));
			
			// Read up to the first non-white-space.
			while (iswspace(*current_in)) {
				current_in += 1;
			}
		}
		else if (*current_f != CV_WCHAR_CONST('%')) {
			// ordinary character
			const VWideChar f = *current_f;
			const VWideChar c = *current_in;
			current_f += 1;
			if (f == c) {
				current_in += 1;
			}
			else {
				matching_fails = TRUE;
			}
		}
		else if (current_f[1] == CV_WCHAR_CONST('%')) {
			// ordinary match of %
			const VWideChar c = *current_in;
			current_f += 2;
			if (c == CV_WCHAR_CONST('%')) {
				current_in += 1;
			}
			else {
				matching_fails = TRUE;
			}
		}
		else {
			// % -- looks like a conversion specifier
			long consumed;
			VWideChar specifier;
			boolean premature_eof;
			int advance = scanOneSpec(current_f, current_in,
									  consumed,
									  specifier,
									  premature_eof,
									  current_in - input,
									  args);
			if (advance > 0) {
				current_f += advance;
				if (consumed > 0) {
					current_in += consumed;
					n_items_assigned += 1;
				}
				else if (   specifier == CV_WCHAR_CONST('c')
						 || specifier == CV_WCHAR_CONST('[')
						 || specifier == CV_WCHAR_CONST('s')) {
					
					input_fails = premature_eof;
					n_items_assigned += 1;
				}
				else if (specifier == CV_WCHAR_CONST('n')) {
					// normal for 'n' specifier
				}
				else {
					// have specifier but no matching item in input
					matching_fails = TRUE;
				}
			}
			else {
				// no valid specifier
				current_f += 1;
				format_fails = TRUE;
			}
		}
		if (input_fails || matching_fails) {
			break;
		}
	}

	return n_items_assigned;
}

// ---------------------------------------------------------------------------
// Return the number of characters in the multibyte character string pointed
// to by 'p'.  Note that this does not correspond to a standard function.
// If 'p' indicates a position in the midst of a multibyte character, then 
// the result of this function must be used with caution.
//
size_t VWideCharAux::mbslen(const char *p)
{
	size_t len = 0;
	VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
	for (;;) {
		int ch_size = VWideCharAux::mbtowc(0, p, CV_MB_LEN_MAX);
		if (ch_size <= 0) {
			break;
		}
		p += ch_size;
		len++;
	}
	return len;
}

// ---------------------------------------------------------------------------
// Return the number of characters in the multibyte character string pointed
// to by 'p' and occupying the 'n' bytes following that location.  Note that
// this does not correspond to a standard function.  Null bytes are treated
// as single characters.  So are other bytes that cannot be interpreted as
// part of a proper multibyte character.  If 'p' or 'len' indicates a
// position in the midst of a multibyte character, then the result of this
// function must be used with caution.
//
size_t VWideCharAux::mbslen(const char *p, size_t len)
{
	size_t i = 0;
	size_t charCount = 0;
	VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
	for (; i < len; charCount += 1) {
		int ch_size = VWideCharAux::mbtowc(0, p + i, len - i);
		if (ch_size == 0) {
			// Pass null characters through.
			ch_size = 1;
		}
		else if (ch_size < 0) {
			if (len - i < size_t(CV_MB_CUR_MAX)) {
				// It appears that we have been asked to count an
				// incomplete multibyte character.
				break;
			}
			else {
				// An invalid character is being counted.
				// Impossible to count true multibyte characters, but it's
				// less misleading to count such a thing as a one byte character
				// than as no characters at all.
				ch_size = 1;
			}
		}
		i += ch_size;
	}
	return charCount;
}

// ---------------------------------------------------------------------------
// Translate the character 'c' to a wide character.
// Return TRUE for success.
//
boolean VWideCharAux::atowc(VWideChar& wc, char c)
{
	if (c == 0) {
		wc = 0;
		return TRUE;
	}
	else {
		mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
		int ch_size = mbtowc(&wc, &c, 1);
		return ch_size == 1;
	}
}

// ---------------------------------------------------------------------------
// Translate the wide character 'wc' to a single byte.
// Return TRUE for success.
//
boolean VWideCharAux::wctoa(char& c, const VWideChar& wc)
{
	if (wc == 0) {
		c = 0;
		return TRUE;
	}
	else {
		char mbc[CV_MB_LEN_MAX];
		wctomb(0, 0); // Initialize the multibyte state.
		int ch_size = wctomb(mbc, wc);
		boolean success = (ch_size == 1);
		if (success) {
			c = mbc[0];
		}
		return ch_size == 1;
	}
}

// ===========================================================================
//   Functions Synthesized for wchar.h
//

#if !defined(WCHAR_H_EXISTS) || defined(CV_SYNTHESIZE_WCHAR)

#include <limits.h>
#include <string.h>
#include <ctype.h>

// ---------------------------------------------------------------------------
//
VWideChar *VWideCharAux::wcscpy(VWideChar *to_in, const VWideChar *from_in)
{
	if (to_in != 0 && from_in != 0) {
		const VWideChar *from_p = from_in;
		VWideChar * to_p = to_in;
		for (;;) {
			VWideChar wc = *from_p;
			*to_p = wc;
			if (wc == CV_WCHAR_CONST('\0')) {
				break;
			}
			to_p += 1;
			from_p += 1;
		}
	}
	return to_in;
}

// ---------------------------------------------------------------------------
//
size_t VWideCharAux::wcslen(const VWideChar *p)
{
	if (p == 0) {
		return 0;
	}
	const VWideChar *q = p;
	while (*q != CV_WCHAR_CONST('\0')) {
		q += 1;
	}
	return q - p;
}

// ---------------------------------------------------------------------------
//
int VWideCharAux::wcscmp(const VWideChar *s1, const VWideChar *s2)
{
	if (s2 == 0) {
		return s1 != 0;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		for (;;) {
			if (*s1 > *s2) {
				return 1;
			}
			else if  (*s1 < *s2) {
				return -1;
			}
			else if (*s1 == 0) {
				return 0;
			}
			s1 += 1;
			s2 += 1;
		}
	}
}

// ---------------------------------------------------------------------------
//
int VWideCharAux::wcsncmp(const VWideChar *s1, const VWideChar *s2, size_t n)
{
	if (s2 == 0) {
		return s1 != 0;
	}
	else if (s1 == 0) {
		return -1;
	}
	else {
		for (; n > 0; n -= 1) {
			if (*s1 > *s2) {
				return 1;
			}
			else if  (*s1 < *s2) {
				return -1;
			}
			else if (*s1 == 0) {
				return 0;
			}
			s1 += 1;
			s2 += 1;
		}
		return 0;
	}
}

// ---------------------------------------------------------------------------
//
int VWideCharAux::wcscoll(const VWideChar *s1, const VWideChar *s2)
{
#if defined(CV_WIN32) 
	// For now, there's no knowledge of the collating sequence available.
	return wcscmp(s1, s2);
#else
	// The native support (if any) for comparing international strings is in
	// terms of char *.
	VString vs1 = s1;
	VString vs2 = s2;
	return strcoll(vs1.gets(), vs2.gets());
#endif
}

// ---------------------------------------------------------------------------
//
VWideChar * VWideCharAux::wcschr(const VWideChar *str, VWideChar c)
{
	if (str != 0) {
		while (*str != CV_WCHAR_CONST('\0') && *str != c) {
			str += 1;
		}
		if (*str == c) {
			return (VWideChar *) str;
		}
	}
	return 0;
}

// ---------------------------------------------------------------------------
//
int (VWideCharAux::iswdigit)(VWideChar digit)
{
	switch (digit) {
	case CV_WCHAR_CONST('0'):
	case CV_WCHAR_CONST('1'):
	case CV_WCHAR_CONST('2'):
	case CV_WCHAR_CONST('3'):
	case CV_WCHAR_CONST('4'):
	case CV_WCHAR_CONST('5'):
	case CV_WCHAR_CONST('6'):
	case CV_WCHAR_CONST('7'):
	case CV_WCHAR_CONST('8'):
	case CV_WCHAR_CONST('9'):
		return 1;
	}
	return 0;
}

// ---------------------------------------------------------------------------
//
int (VWideCharAux::iswspace)(VWideChar wc)
{
	if (wc == 0) {
		return FALSE;
	}
	else {
		char mbc[CV_MB_LEN_MAX];
		if (wctomb(mbc, wc) != 1) {
			return FALSE;
		}
		return isspace(mbc[0]);
	}
}

// ---------------------------------------------------------------------------
// 
VWideChar *VWideCharAux::wcstok(VWideChar *string1, const VWideChar *delims) { 
	static VWideChar* buffer = 0;
	return wcstok(string1, delims, &buffer);
}  

// ---------------------------------------------------------------------------
// 
int (VWideCharAux::iswalpha)(VWideChar wc) {
	if ((wc >= CV_WCHAR_CONST('a') && wc <= CV_WCHAR_CONST('z')) || (wc >= CV_WCHAR_CONST('A') && wc <= CV_WCHAR_CONST('Z'))){
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// 
int (VWideCharAux::iswalnum)(VWideChar c){
	if (iswalpha(c) || iswdigit(c)) return TRUE;
	return FALSE;
}   

// ---------------------------------------------------------------------------
// 
int (VWideCharAux::iswpunct)(VWideChar wc){  
	if (wc == 0) {
		return FALSE;
	}
	else {
		char mbc[CV_MB_LEN_MAX];
		if (wctomb(mbc, wc) != 1) {
			return FALSE;
		}
		return ispunct(mbc[0]);
	}
}  

// ---------------------------------------------------------------------------
// 
int (VWideCharAux::iswcntrl)(VWideChar wc){  
	if (wc == 0) {
		return TRUE;
	}
	else {
		char mbc[CV_MB_LEN_MAX];
		if (wctomb(mbc, wc) != 1) {
			return FALSE;
		}
		return iscntrl(mbc[0]);
	}
}  

// ===========================================================================
//   Locale-dependent Synthesized Functions
//

#if defined(CV_SYNTHESIZE_WCHAR)
// ---------------------------------------------------------------------------
//
int VWideCharAux::strcoll(const char *s1, const char *s2)
{
#if defined(PM)
	ULONG d = WinCompareStrings(VNotifier::getHab(), 0UL, 0UL,
								(PSZ)s1, (PSZ)s2,
								0UL);
	if (d == WCS_EQ) {
		return 0;
	}
	else if (d == WCS_LT) {
		return -1;
	}
	else if (d == WCS_GT) {
		return 1;
	}
	return 0;
#elif defined(CV_WINDOWS)
	return lstrcmp((LPCSTR)s1, (LPCSTR)s2);
#endif
}

// ---------------------------------------------------------------------------
// Stand-in for standard tolower.
//
int (VWideCharAux::tolower)(int c)
{
#if defined(PM)
	// no native support in OS/2
	return (::tolower)(c);
#elif defined(CV_WIN16)
	char cc = char(c);
	if (cc == c) {
		AnsiLowerBuff(&cc, 1);
		return int(cc);
	}
	return c;
#endif
}

// ---------------------------------------------------------------------------
// Stand-in for standard toupper.
//
int (VWideCharAux::toupper)(int c)
{
#if defined(PM)
	if (c != int(char(c))) {
		return c;
	}
	ULONG u = WinUpperChar(VNotifier::getHab(), 0UL, 0UL, ULONG(c)); 
	return u != 0 ? int(u) : c;
#elif defined(CV_WIN16)
	char cc = char(c);
	if (cc == c) {
		AnsiUpperBuff(&cc, 1);
		return int(cc);
	}
	return c;
#endif
}
#endif // synthesizing international string support

// ===========================================================================
//   Functions Synthesized for wchar.h
//

// ---------------------------------------------------------------------------
// Stand-in for standard towlower.
//
VWideInt VWideCharAux::towlower(VWideInt wi)
{
	char c;
	VWideChar wc = VWideChar(wi);
	if (wctoa(c, wc)) {
#ifndef __SC__
		c = (char)(tolower)((int)c);
#else
		c = (char)tolower((int)c);
#endif
		atowc(wc, c);
	}
	return VWideInt(wc);
}

// ---------------------------------------------------------------------------
// Stand-in for standard towupper.
//
VWideInt VWideCharAux::towupper(VWideInt wi)
{
	char c;
	VWideChar wc = VWideChar(wi);
	if (wctoa(c, wc)) {
#ifndef __SC__
		c = (char)(toupper)((int)c);
#else
		c = (char)toupper((int)c);
#endif
		atowc(wc, c);
	}
	return VWideInt(wc);
}

// ---------------------------------------------------------------------------
// Like strtol, except does not set ERRNO.
//
long VWideCharAux::wcstol(const VWideChar *nptr, VWideChar **endptr,
												 int base)
{
	const VWideChar *current_in = nptr;

	// Skip past white space.
#ifndef __SC__
	while ((iswspace)(*current_in)) {
#else
	while (iswspace(*current_in)) {
#endif
		current_in += 1;
	}

	// Allow optional sign.
	int minus = 0;
	if (*current_in == CV_WCHAR_CONST('+') || *current_in == CV_WCHAR_CONST('-')) {
		current_in += 1;
		if (*current_in == CV_WCHAR_CONST('-')) {
			minus = 1;
		}
	}

	const VWideChar *ep;
	unsigned long v = wcstoul(current_in, (VWideChar **)&ep, base);
	if (endptr != 0) {
		*endptr = (VWideChar *)ep;
	}

	long value = minus ? -long(v) : long(v);
	if (v != 0 && (value < 0) != minus) {
		return minus ? LONG_MIN : LONG_MAX;
	}
	return value;
}

// ---------------------------------------------------------------------------
// Like strtoul, except does not set ERRNO.
//
unsigned long VWideCharAux::wcstoul(const VWideChar *nptr,
									VWideChar **endptr,
									int base)
{
#ifdef CPPV_HPUX
static VWideChar wide_lowercase[] = {
			(VWideChar)'0',
			(VWideChar)'1',
			(VWideChar)'2',
			(VWideChar)'3',
			(VWideChar)'4',
			(VWideChar)'5',
			(VWideChar)'6',
			(VWideChar)'7',
			(VWideChar)'8',
			(VWideChar)'9',
			(VWideChar)'a',
			(VWideChar)'b',
			(VWideChar)'c',
			(VWideChar)'d',
			(VWideChar)'e',
			(VWideChar)'f',
			(VWideChar)'g',
			(VWideChar)'h',
			(VWideChar)'i',
			(VWideChar)'j',
			(VWideChar)'k',
			(VWideChar)'l',
			(VWideChar)'m',
			(VWideChar)'n',
			(VWideChar)'o',
			(VWideChar)'p',
			(VWideChar)'q',
			(VWideChar)'r',
			(VWideChar)'s',
			(VWideChar)'t',
			(VWideChar)'u',
			(VWideChar)'v',
			(VWideChar)'w',
			(VWideChar)'x',
			(VWideChar)'y',
			(VWideChar)'z'
			};

static VWideChar wide_uppercase[] = {
			(VWideChar)'0',
			(VWideChar)'1',
			(VWideChar)'2',
			(VWideChar)'3',
			(VWideChar)'4',
			(VWideChar)'5',
			(VWideChar)'6',
			(VWideChar)'7',
			(VWideChar)'8',
			(VWideChar)'9',
			(VWideChar)'A',
			(VWideChar)'B',
			(VWideChar)'C',
			(VWideChar)'D',
			(VWideChar)'E',
			(VWideChar)'F',
			(VWideChar)'G',
			(VWideChar)'H',
			(VWideChar)'I',
			(VWideChar)'J',
			(VWideChar)'K',
			(VWideChar)'L',
			(VWideChar)'M',
			(VWideChar)'N',
			(VWideChar)'O',
			(VWideChar)'P',
			(VWideChar)'Q',
			(VWideChar)'R',
			(VWideChar)'S',
			(VWideChar)'T',
			(VWideChar)'U',
			(VWideChar)'V',
			(VWideChar)'W',
			(VWideChar)'X',
			(VWideChar)'Y',
			(VWideChar)'Z'
			};
#endif

	const VWideChar *current_in = nptr;

	// Skip past white space.
#ifndef __SC__
	while ((iswspace)(*current_in)) {
#else
	while (iswspace(*current_in)) {
#endif
		current_in += 1;
	}

	// Allow optional sign.
	boolean has_minus = FALSE;
	if (*current_in == CV_WCHAR_CONST('+') || *current_in == CV_WCHAR_CONST('-')) {
		current_in += 1;
		if (*current_in == CV_WCHAR_CONST('-')) {
			has_minus = TRUE;
		}
	}

	// Base of 0 handles flexible integral token.
	if (base == 0) {
		if (*current_in == CV_WCHAR_CONST('0')) {
			VWideChar next = current_in[1];
			if (next == CV_WCHAR_CONST('x') || next == CV_WCHAR_CONST('X')) {
				base = 16;
			}
			else {
				base = 8;
			}
		}
		else {
			base = 10;
		}
	}

	// Allow optional 0x on hex number.
	if (   base == 16
		&& *current_in == CV_WCHAR_CONST('0')
		&& (current_in[1] == CV_WCHAR_CONST('x') || current_in[1] == CV_WCHAR_CONST('X'))) {

		current_in += 1;
	}

	unsigned long v = 0;
	int overflow = 0;
	while (TRUE) {
		VWideChar digit = *current_in;
		int i;
		for (i = 0; i < base; i += 1) {
#ifdef CPPV_HPUX
			if (   digit == wide_lowercase[i]
				|| digit == wide_uppercase[i] )
#else
			if (   digit == L"0123456789abcdefghijklmnopqrstuvwxyz"[i]
				|| digit == L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i]) 
#endif
		{
				break;
			}
		}
		if (i >= base) {
#ifndef __SC__
			if (!(iswdigit)(digit)) {
#else
			if (!iswdigit(digit)) {
#endif
				// not a digit
				break;
			}
			// non-identifiable digit
			i = 0;
		}
		unsigned long prev = v;
		v = v * base + i;
		if (   v < 0
			|| v % (unsigned long)base != (unsigned long)i
			|| (v / (unsigned long)base) != prev) {

			overflow = 1;
		}
		current_in += 1;
	}

	if (endptr != 0) {
		*endptr = (VWideChar *)current_in;
	}

	unsigned long value = 0;
	if (!overflow) {
		value = has_minus ? (unsigned long)(-long(v)) : v;
		if (v != 0 && has_minus) {
			overflow = TRUE;
		}
	}
	if (overflow) {
		return ULONG_MAX;
	}
	return value;
}

// ---------------------------------------------------------------------------
// Like strtoul, except does not set ERRNO.
//
double VWideCharAux::wcstod(const VWideChar *nptr, VWideChar **endptr)
{
#ifdef CPPV_HPUX
static VWideChar wide_digits[] = {
			(VWideChar)'0',
			(VWideChar)'1',
			(VWideChar)'2',
			(VWideChar)'3',
			(VWideChar)'4',
			(VWideChar)'5',
			(VWideChar)'6',
			(VWideChar)'7',
			(VWideChar)'8',
			(VWideChar)'9'
			};
#endif

	const VWideChar *current_in = nptr;

	// Skip past white space.
#ifndef __SC__
	while ((iswspace)(*current_in)) {
#else
	while (iswspace(*current_in)) {
#endif
		current_in += 1;
	}

	// Allow optional sign.
	int minus = 0;
	if (*current_in == CV_WCHAR_CONST('+') || *current_in == CV_WCHAR_CONST('-')) {
		current_in += 1;
		if (*current_in == CV_WCHAR_CONST('-')) {
			minus = 1;
		}
	}

	// Scan to end of "argument", accumulating value in v.
	const VWideChar *arg_start = current_in;
	double v = 0.0;
	const VWideChar *decimal_point = current_in;
	long fractional_digits = 0;
#ifndef __SC__
	while ((iswdigit)(*current_in) || isdpoint(*current_in)) {
#else
	while (iswdigit(*current_in) || isdpoint(*current_in)) {
#endif
		if (isdpoint(*current_in)) {
			if (decimal_point == 0) {
				decimal_point = current_in;
			}
		}
		else {
			VWideChar digit = *current_in;
			int i;
			for (i = 0; i < 10; i += 1) {
#ifdef CPPV_HPUX
				if (digit == wide_digits[i])
#else
				if (digit == L"0123456789"[i]) 
#endif
			{
					break;
				}
			}
			if (i >= 10) {
				// non-identifiable digit
				i = 0;
			}
			v *= 10;
			v += i;
			if (decimal_point != 0) {
				fractional_digits += 1;
			}
		}
		current_in += 1;
	}
	const VWideChar *arg_end = current_in;

	// Handle explicit exponent part.
	long explicit_exponent = 0;
	if (*current_in == CV_WCHAR_CONST('e') || *current_in == CV_WCHAR_CONST('E')) {
		VWideChar *ep;
		explicit_exponent = wcstol(current_in + 1, &ep, 10);
		current_in = ep;
	}

	// Adjust v for exponents.
	long exponent = explicit_exponent - fractional_digits;
	while (exponent != 0) {
		if (exponent > 0) {
			v *= 10;
			exponent -= 1;
		}
		else {
			v /= 10;
			exponent += 1;
		}
	}

	if (endptr != 0) {
		*endptr = (VWideChar *)current_in;
	}

	return v;
}

#endif // not WCHAR_H_EXISTS, or synthesizing all functions

// ===========================================================================
//   Function Synthesized for wchar.h or Microsoft Systems
//

#if !defined(WCHAR_H_EXISTS) || defined(CV_SYNTHESIZE_WCHAR) || defined(MSDOS)
// ---------------------------------------------------------------------------
// Microsoft's wchar.h supplies a non-standard wcstok.
// 
VWideChar *VWideCharAux::wcstok(VWideChar *string1, const VWideChar *delims,
								VWideChar **buffer_param) 
{
	if (buffer_param == 0) {
		// error
		return 0;
	}
	VWideChar *&buffer_p = *buffer_param;
	// This implementation only works for 1 delimiter.
	if (string1) { 
	 // set up the buffer and scan for first token
	     buffer_p = string1;
	} 
	if (buffer_p == 0) {
		// Incorrect call; no position currently saved.
		return 0;
	}
	const VWideChar delim = delims[0];
	if (delim == 0 || delims[1] != 0) {
		// This implementation is limited to 1 delimiter.
		return 0;
	}
	// Skip past delimiters.
	while (*buffer_p != 0 && *buffer_p == delim) {
		buffer_p += 1;
	}
	// Return 0 if no token follows delimiters.
	if (*buffer_p == 0) {
		// Subsequent calls must specify a string.
		buffer_p = 0;
		return 0;
	}
	VWideChar *tok = buffer_p;
	// Scan the token, to retain where the next scan starts.
	while (*buffer_p != 0 && *buffer_p != delim) {
		buffer_p += 1;
	}
	// Terminate the token with a null wide character.
	if (*buffer_p != 0) {
		*buffer_p = 0;
		buffer_p += 1; // Keep going.
	}
	else {
		// There are no more tokens.
		buffer_p = 0;
	}
	return tok;
}
#endif  

// ===========================================================================
//   Core Locale-dependent Synthesized Functions
//

#if defined(CV_SYNTHESIZE_WCHAR)

// ---------------------------------------------------------------------------
// The following functions work like the standard functions, only with
// VWideChar rather than the built-in wchar_t.
//
// ---------------------------------------------------------------------------
//
int (VWideCharAux::mblen)(const char *s, size_t n)
{
	if (s == 0) {
		return 0;
	}
	else if (n == 0) {
		return -1; // not a valid character
	}
	else {
		register unsigned char first = (unsigned char)(s[0]);
		if (first == '\0') {
			return 0;
		}
		else {
#if defined(CV_WINDOWS)
			if (!IsDBCSLeadByte(BYTE(first))) {
				// single byte character
				return 1;
			}
			else if (n > 1) {
				// assume valid double byte character
				return 2;
			}
			else {
				return -1;
			}
#else
			signed char *tp = (signed char *)notifier->getByteTypeTablePtr();
			if (tp == 0) {
				return 1;
			}
			else if (tp[first] >= 0) {
				// single-byte character
				return 1;
			}
			else if (n > 1 && tp[(unsigned char)s[1]] <= 0) {
				// valid double-byte character
				return 2;
			}
			else {
				return -1;
			}
#endif
		}
	}		
}

// ---------------------------------------------------------------------------
//
int VWideCharAux::mbtowc(VWideChar *pwc, const char *s, size_t n)
{
	if (s == 0) {
		return 0;
	}
	else if (n == 0) {
		return -1; // not a valid character
	}
	else {
		register unsigned char first = (unsigned char)(s[0]);
		if (first == '\0') {
			if (pwc != 0) {
				*pwc = 0;
			}
			return 0;
		}
		else {
#if defined(CV_WINDOWS)
			int count = 1;
			if (IsDBCSLeadByte(BYTE(first))) {
				// double-byte character
				count = 2;
				if (size_t(count) > n) {
					return -1;
				}
				unsigned char second = (unsigned char)s[1];
				if (pwc != 0) {
					*pwc = (first << 8) | second;
				}
			}
			else {
				if (pwc != 0) {
					*pwc = first;
				}
			}
#else
			signed char *tp = (signed char *)notifier->getByteTypeTablePtr();
			int count = 1;
			if (tp != 0 && tp[first] < 0) {
				// double-byte character
				count = 2;
				if (count > n) {
					return -1;
				}
				unsigned char second = (unsigned char)s[1];
				if (tp[second] > 0) {
					// invalid double-byte character
					return -1;
				}
				if (pwc != 0) {
					*pwc = (first << 8) | second;
				}
			}
			else {
				if (pwc != 0) {
					*pwc = first;
				}
			}
#endif		
			return count;
		}
	}
}

// ---------------------------------------------------------------------------
//
int VWideCharAux::wctomb(char *s, VWideChar wchar)
{
	if (s == 0) {
		return 0;
	}
#if defined(CV_WINDOWS)
	char high = char(wchar >> 8);
	if (high != 0) {
		char low = char(wchar & 0xFF);
		if (!IsDBCSLeadByte(BYTE(high))) {
			// invalid double-byte character
			return -1;
		}
		if (s != 0) {
			s[0] = high;
			s[1] = low;
		}
		return 2;
	}
	else {
		if (IsDBCSLeadByte(BYTE(wchar))) {
			// invalid single-byte character
			return -1;
		}
		if (s != 0) {
			*s = char(wchar);
		}
	}
#else
	signed char *tp = (signed char *)notifier->getByteTypeTablePtr();
	char high = wchar >> 8;
	if (high != 0) {
		char low = wchar & 0xFF;
		signed char high_type = tp[(unsigned char)high];
		signed char low_type = tp[(unsigned char)low];
		if (tp == 0 || high_type >= 0 || low_type > 0) {
			// invalid double-byte character
			return -1;
		}
		if (s != 0) {
			s[0] = high;
			s[1] = low;
		}
		return 2;
	}
	else {
		if (tp != 0 && tp[wchar] < 0) {
			// invalid single-byte character
			return -1;
		}
		if (s != 0) {
			*s = wchar;
		}
	}
#endif
	return 1;
}

// ---------------------------------------------------------------------------
//
size_t VWideCharAux::mbstowcs(VWideChar *pwcs, const char *s, size_t n)
{
	if (pwcs == 0 || s == 0) {
		return 0;
	}
	size_t modified_count = 0;
	size_t read_count = 0;
	while (modified_count < n) {
		int count = mbtowc(pwcs, s, 2);
		if (count <= 0) {
			if (count < 0) {
				return (size_t)-1;
			}
			break;
		}
		pwcs += 1;
		s += count;
		read_count += count;
		modified_count += 1;
	}
	return modified_count;
}

// ---------------------------------------------------------------------------
//
size_t VWideCharAux::wcstombs(char *s, const VWideChar *pwcs, size_t n)
{
	if (pwcs == 0 || s == 0) {
		return 0;
	}
	size_t modified_count = 0;
	while (modified_count < n) {
		char buf[CV_MB_LEN_MAX];
		int count = wctomb(buf, *pwcs++);
		if (count < 0) {
			return (size_t)-1;
		}
		for (int i = 0; i < count; ) {
			if ((*s++ = buf[i++]) == '\0') {
				return modified_count;
			}
			if (++modified_count >= n) {
				return modified_count;
			}
		}
	}
	return modified_count;
}

#if defined(V_MACINTOSH)
// ---------------------------------------------------------------------------
// Called during Mac initialization by the notifier, this sets up a table
// of byte types.
//
void VWideCharAux::setupByteTypeTable()
{
	void * table = notifier->getByteTypeTablePtr();
	char *tp = (char *)table;
	if (tp != 0) {
		delete [] tp;
		notifier->setByteTypeTablePtr(0);
	}
	long flags = GetScriptVariable(smSystemScript, smScriptFlags);
	if (((flags >> smsfSingByte) & 1) != 0) {
		// Flags indicate script has only single-byte characters.
		// Leave table null.
	}
	else {
		// Character set has double bytes.  Allocate table.
		tp = new char[256];
		if (tp != 0) {
			// Fill table.
			long itl5_res_ID = GetScriptVariable(smSystemScript, smScriptEncoding);
			Handle rh = GetResource('itl5', itl5_res_ID);
			HLock(rh);
			const Itl5Record *rp = (const Itl5Record *)(*rh);
			if (   rp->numberOfTables >= 1
				&& rp->tableDirectory[0].tableSignature == 'btyp'
				&& rp->tableDirectory[0].tableSize == 256) {
				
				const char *rtp = (const char *)rp + rp->tableDirectory[0].tableStartOffset;
				memcpy(tp, rtp, 256);
				// This table is copied right from the 'itl5' resource.
				// For each possible byte value, it indicates one of
				//	  1		1-byte character only
				//	  0		1-byte character or low-order byte of a 2-byte character
				//	 -1		high-order or low-order byte of a 2-byte character
			}
			HUnlock(rh);
			ReleaseResource(rh);
			notifier->setByteTypeTablePtr(tp);
		}
	}
}
#endif

#if defined(PM)
// ---------------------------------------------------------------------------
// Called during Mac initialization by the notifier, this sets up a table
// of byte types.
//
void VWideCharAux::setupByteTypeTable()
{
	void * table = notifier->getByteTypeTablePtr();
	signed char *tp = (signed char *)table;
	if (tp != 0) {
		delete [] tp;
		notifier->setByteTypeTablePtr(0);
	}

	const int SBCS_SIZE = 256;
	const int RANGE_TABLE_SIZE = 20;
	signed char _DBCStype[SBCS_SIZE];
	CHAR rangeTbl[RANGE_TABLE_SIZE];

	COUNTRYCODE ctryc;
	USHORT rc;
	unsigned char i;
	SHORT j;
	boolean has_db = FALSE;
	memset(_DBCStype, 0, SBCS_SIZE);
	memset(rangeTbl, 0, RANGE_TABLE_SIZE);
	ctryc.country = ctryc.codepage = 0;

	rc = DosGetDBCSEv(RANGE_TABLE_SIZE, &ctryc, rangeTbl);
	if (rc != 0) {
		return;
	}
	if (*rangeTbl == 0) {
		return;
	}

	// to make sure the range is not overshot
	rangeTbl[RANGE_TABLE_SIZE] = 0;
	rangeTbl[RANGE_TABLE_SIZE - 1] = 0;

	for (j = 0; rangeTbl[j] != 0 && rangeTbl[j + 1] != 0; j += 2) {
		for (i = rangeTbl[j]; i <= (unsigned char)rangeTbl[j + 1]; i++) {
			_DBCStype[i] = -1;  // lead byte, not single byte
			has_db = TRUE;
		}
	}

	if (!has_db) {
		// Flags indicate script has only single-byte characters.
		// Leave table null.
	}
	else {
		// Character set has double bytes.  Allocate table.
		tp = new signed char[SBCS_SIZE];
		if (tp != 0) {
			// Fill table.
			memcpy((char *)tp, _DBCStype, SBCS_SIZE);
			// For each possible byte value, it indicates one of
			//	  1		1-byte character only
			//	  0		1-byte character or low-order byte of a 2-byte character
			//	 -1		high-order or low-order byte of a 2-byte character
			notifier->setByteTypeTablePtr(0);
		}
	}
}
#endif /* PM */

// ---------------------------------------------------------------------------
//
boolean VWideCharAux::isSingleByteCharSet() 
{
#if defined(PM) || defined(V_MACINTOSH)
	return notifier->getByteTypeTablePtr() == 0;
#elif defined(CV_WIN16)
	static boolean initialized = FALSE;
	static boolean single_byte = TRUE;
	if (!initialized) {
		initialized = TRUE;
		for (short i = 0; i < 256; i += 1) {
			if (IsDBCSLeadByte(BYTE(i))) {
				single_byte = FALSE;
			}
		}
	}
#if defined(CV_DEBUG_950905)
	return FALSE;
#else
	return single_byte;
#endif
#endif
}

#endif /* synthesizing international string support */

// ===========================================================================
//   Functions to Support Native Compiler-based Locales
//

#if !defined(CV_SYNTHESIZE_WCHAR)
void VWideCharAux::checkLocale()
{
#if defined(CV_MOTIF)
	static boolean initialized = FALSE;
	if (!initialized) {
		XtSetLanguageProc(0, 0, 0);
		initialized = TRUE;
	}
#else
	static boolean honored = (setlocale(LC_ALL, "") != 0);
#endif
}
#endif
