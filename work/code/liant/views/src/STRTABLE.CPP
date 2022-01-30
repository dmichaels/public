// $Header:   Q:/views/common/vcs/STRTABLE.CPV   1.22   Mar 24 1997 16:13:32   scottt  $

//	strtable.cpp
//
//	VStringTable class implementation [Common]
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
//	10/03/95 pkt	created.
//	10/12/95 pkt	added archiver tag.
//	10/18/95 pkt	fixed 'skip_whitespace' to avoid problem with 'iswspace'
//					macro.
//	10/18/95 pkt	derived from VInternationalTable class.
//	10/18/95 pkt	upped archiver tag.
//	10/20/95 pkt	debugged.
//	11/02/95 pkt	added 'optimize' flag to getFrom(VRscArchiver&,...).
//	11/20/95 pkt	added 'canPut' and 'canGet'.
//	11/24/95 pkt	added 'supports'.
//	12/06/95 pkt	fixed 'charEncoding' for PM.
//	12/07/95 pkt	fixed 'charEncoding' to compile on Win16.
//	12/07/95 pkt	changed to eliminate Borland warning.
//	12/15/95 tlf	fixed to compile on HPUX.
//	12/19/95 tlf	fixed 'charEncoding' to compile with Borland on PM.
//						(pkt's fix of 12/13/95 that I over-wrote)
//	01/16/96 pkt	fixed 'getCurrentEncoding'.
//	01/29/96 pkt	changed 'canGet' and 'canPut' to always handle the
//					current locale.
//	01/29/96 pkt	primitive 'charEncoding' implementation for Motif.
//	01/30/96 tlf	fixed to compile on HPUX.
//	02/02/96 tlf	fixed to compile on AIX.
//	02/02/96 pkt	one last fix to 'charEncoding' for Motif.
//	02/16/96 pkt	changed to open resource using specific class.
//	02/21/96 evc	charEncoding() fix for borland 16
//						(GetLocaleInfoA not supported)
//	04/08/96 glm	check for Symantec
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "strtable.h"
#include "iterator.h"
#include "str.h"
#include "strm.h"
#include "file.h"
#if !defined(CV_NOARCHIVER)
# include "archiver.h"
# include "rscarch.h"
# include "archvrtg.h"
#endif

// Implementation Note
// 
// The keys and values involved in a VStringTable all belong to the 
// VStringTable.  Since they are not automatically managed by VDictionary
// and VMultiSet, they are managed here.
//
// The following heap-allocated objects are managed:
//		one VString per tag (can be found in 'tags')
//		one VLocale per locale (can be found in 'messages')
//		one VDictionary per locale (can be found in 'messages')
//		one VString per message (can be found in the locale-specific VDictionaries)

defineClass(VStringTable, VInternationalTable)
//
// revision 0	original
// revision 1	derived from VInternationalTable
//
defineArchiveRevision(VStringTable, 1)

// ---------------------------------------------------------------------------
// Construct a non-operational string table.
//
VStringTable::VStringTable()
{
}

// ---------------------------------------------------------------------------
//
VStringTable::~VStringTable()
{
}

// ---------------------------------------------------------------------------
// Virtual override for VInternationalTable::isValidTag
//
boolean	VStringTable::isValidTag(VObject *tag) 
{
	return tag != 0 && tag->isA(VStringCls) && isValidTag(*(VString *)tag);
}

// ---------------------------------------------------------------------------
// A tag is valid if every character in it is representable in the portable
// subset of ASCII.  This function returns whether the given string is a
// valid tag.
//
boolean VStringTable::isValidTag(const VString& tag)
{
	static const char valid[128] = {
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
		 ' ',  '!',  '"', '\0', '\0',  '%',  '&', '\'',
		 '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
		 '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
		 '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
		'\0',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
		 'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
		 'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
		 'X',  'Y',  'Z', '\0', '\0', '\0', '\0',  '_',
		'\0',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
		 'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
		 'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
		 'x',  'y',  'z', '\0', '\0', '\0', '\0', '\0'
	};
	integer len = tag.getCharSize();
	for (integer i = 0; i < len; i += 1) {
		VWideChar wc = tag.wcharAt(i);
		char c;
		if (   !VWideCharAux::wctoa(c, wc)
			|| c < 1 || int(c) >= 128
			|| valid[c] != c) {

			return FALSE;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Retrieve the message for a given tag. Return FALSE if no
// message was found appropriate to the current locale.
//
boolean VStringTable::get(const VString& tag, VString& message) const
{
	return VInternationalTable::get((VString *)&tag, &message);
}
	
// ---------------------------------------------------------------------------
// Add the association of 'tag' with 'message' to the string table for the
// current locale.  Returns FALSE if this fails, e.g. if there is no valid
// current locale, or if the string table was constructed with an optimized
// connection to a resource archiver, or if the tag uses non-portable
// characters.  If 'tag' already has an association
// in the current locale, replace it.
//
boolean VStringTable::put(const VString& tag_param, const VString& message)
{
	return VInternationalTable::put((VString *)&tag_param, (VString *)&message);
}
	
// ---------------------------------------------------------------------------
// Remove the association of 'tag' from the string table for the
// current locale.  Returns FALSE if this fails, e.g. if there is no valid
// current locale, or if the string table was constructed with an optimized
// connection to a resource archiver.  If 'tag' has no association in
// the current locale, does nothing and returns TRUE.
//
boolean VStringTable::remove(const VString& tag_param)
{
	return VInternationalTable::remove((VObject *)&tag_param);
}

// ---------------------------------------------------------------------------
// Parse a literal string delimited with double-quotes.
// Backslash escapes the following character to support
// embedded double-quotes and backslashes.
//
boolean VStringTable::parseString(VStream& strm, VString& str)
{
	str = "";
	VWideChar wc;
	if (!strm.getch(wc) || wc != CV_WCHAR_CONST('"')) {
		return FALSE;
	}
	while (strm.getch(wc)) {
		if (wc == CV_WCHAR_CONST('"')) {
			return TRUE;
		}
		else if (wc == CV_WCHAR_CONST('\\')) {
			// process escape sequence
			// Numeric escapes not supported.
			if (!strm.getch(wc)) {
				return FALSE;
			}
			if (wc == CV_WCHAR_CONST('n')) {
#if defined(CV_WINDOWS) || defined(PM)
				str += VWideChar(CV_WCHAR_CONST('\x0D'));
				wc = VWideChar(CV_WCHAR_CONST('\x0A'));
#else
				wc = VWideChar(CV_WCHAR_CONST('\n'));
#endif
			}
			else if (wc == CV_WCHAR_CONST('t')) {
				wc = VWideChar(CV_WCHAR_CONST('\t'));
			}
			str += wc;
		}
		else {
			str += wc;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
void VStringTable::skipWhitespace(VStream& strm)
{
	VWideChar wc;
	while (strm.getch(wc)) {
#ifndef __SC__
		if (!(VWideCharAux::iswspace)(wc)) {
#else
		if (!VWideCharAux::iswspace(wc)) {
#endif
			strm.ungetch(wc);
			return;
		}
	}
}

// ---------------------------------------------------------------------------
// Override VInternationalTable::importMessage to take a line of a text file
// representing a message, and put its tag and message into the current locale
// of this table. 
//
void VStringTable::importMessage(VStream& line)
{
	// Parse the line, which should consist of two quoted string
	// literals (as in C source code).
	VString tag;
	boolean OK1 = parseString(line, tag);
	VString message;
	skipWhitespace(line);
	boolean OK2 = OK1 && parseString(line, message);
	if (OK2 && line.eof()) {
		// parsing successful
		put(tag, message);
	}
}

// ---------------------------------------------------------------------------
// Put the given string in string literal form to the stream.
//
boolean VStringTable::escapeString(VStream& strm, const VString& str)
{
	strm.putch(VWideChar(CV_WCHAR_CONST('"')));
	integer count = str.getCharSize();
	for (integer idx = 0; idx < count; idx += 1) {
		VWideChar wc = str.wcharAt(idx);
		if (wc == CV_WCHAR_CONST('\t')) {
			strm.putch(VWideChar(CV_WCHAR_CONST('\\')));
			strm.putch(VWideChar(CV_WCHAR_CONST('t')));
		}
		else if (wc == CV_WCHAR_CONST('\n')) {
			strm.putch(VWideChar(CV_WCHAR_CONST('\\')));
			strm.putch(VWideChar(CV_WCHAR_CONST('n')));
		}
#if defined(PM) || defined(CV_WINDOWS)
		else if (wc == CV_WCHAR_CONST('\x0D') && str.wcharAt(idx + 1) == CV_WCHAR_CONST('\x0A')) {
			strm.putch(VWideChar(CV_WCHAR_CONST('\\')));
			strm.putch(VWideChar(CV_WCHAR_CONST('n')));
			idx += 1;
		}
#endif
		else {
			if (wc == CV_WCHAR_CONST('"') || wc == CV_WCHAR_CONST('\\')) {
				strm.putch(VWideChar(CV_WCHAR_CONST('\\')));
			}
			strm.putch(wc);
		}
	}
	strm.putch(VWideChar(CV_WCHAR_CONST('"')));
	return TRUE;
} 

// ---------------------------------------------------------------------------
// Override VInternationalTable::exportMessage to put a tag and message to
// a text stream.  Assume 'tag' and 'message' parameters are not null.
//
boolean VStringTable::exportMessage(VStream& file, VObject *tag, 
												   VObject *message) const
{
	escapeString(file, *(VString *)tag);
	file << ' ';
	escapeString(file, *(VString *)message);
	return TRUE;
}

// ---------------------------------------------------------------------------
// CharEncoding indicates the encoding of a character set in a platform-
// dependent way.  It is the code page on platforms that have "code page".
//
#if defined(CV_WINDOWS)
typedef UINT CharEncoding;
const CharEncoding unknownEncoding = 0;
#elif defined(PM)
typedef ULONG CharEncoding;
const CharEncoding unknownEncoding = 0;
#elif defined(MOTIF)
typedef short CharEncoding;
const CharEncoding unknownEncoding = 0;
#endif

// ---------------------------------------------------------------------------
// For OS/2, use a table of code page per locale, because this information
// doesn't seem to be available for all locales at once..
//
#if defined(PM)
typedef struct { ULONG cp; char *code; } CodePageItm;
static CodePageItm code_page[] = {
	  0, "ar", // Arabic
	  0, "zh", // Chinese
	  0, "zh_TW", // Taiwan
	852, "cs", // Czech
	850, "da", // Danish
	850, "de", // German
	850, "en_UK", // United Kingdom
	437, "en_US", // U.S.A.
	850, "en", // Asian English -- should be more specific?
	850, "en_AU",  // Australia
	850, "fr_BE", // Belgium
	850, "nl_BE", // Belgium
	850, "es", // 034 is Spain, Spanish
	850, "es", // 003 is Latin America, "es" is Spanish -- should be more specific?
	850, "fi", // Finnish
	850, "fr", // French
	863, "fr_CA",  // French, Canadian
	  0, "iw", // Hebrew
	852, "hu", // Hungarian
	861, "is", // Icelandic
	850, "it", // Italian
	  0, "ja", // Japanese
	  0, "ko", // Korean
	850, "nl", // Dutch
	865, "no", // Norwegian
	852, "pl", // Polish
	860, "pt", // Portugal
	852, "sh", // 038 is Yugoslavia. -- "sh" is Serbo-Croation
	850, "de_CH", // 041 is Switzerland -- "de" is arbitrary.
	865, "sv", // Swedish
	857, "tr" // Turkish
};

// ---------------------------------------------------------------------------
// Finding the current code page in OS/2. This is a necessary supplement to
// double-check the above CodePageItm table, and ensure that a string which
// which has been entered into the table actually has the right code page.
//
static CharEncoding getCurrentEncoding()
{
	// OS/2: go direct to the OS because the Views facilities may fail.
	CharEncoding current_encoding = 0;

	COUNTRYCODE query_country;
	COUNTRYINFO info;
	ULONG info_length;
	APIRET rc;

	query_country.country = 0;
	query_country.codepage = 0;
	rc = DosQueryCtryInfo(sizeof(info), &query_country, &info, &info_length);

	if (rc != 0) {
		// query failed
	}
	else if (info_length < 2 * sizeof(ULONG)) {
		// query failed
	}
	else {
		current_encoding = info.codepage;
	}
	return current_encoding;
}
#endif

// ---------------------------------------------------------------------------
//
static CharEncoding charEncoding(const VLocale& loc)
{
#if defined(CV_WINDOWS)
# ifndef LOCALE_IDEFAULTANSICODEPAGE
# define LOCALE_IDEFAULTANSICODEPAGE 0x1004
# endif
	LCID locid = loc.id();
	if (locid != UNKNOWN_LOCALE) {
		char buf[10];
		#if defined (CV_WIN16) && defined (__BCPLUSPLUS__)
		int n = 0;
		#else
		int n = GetLocaleInfoA(locid, LOCALE_IDEFAULTANSICODEPAGE, buf, 10);
		#endif
		if (n > 0) {
			// Buf holds the code page as 4 decimal digits.
			CharEncoding result = 0;
			if (n == 10) {
				// Sigh, it really returned them as Unicode digits,
				// even though it doesn't make sense.  Maybe the other branch
				// of this "if" should never be taken.
				for (wchar_t *pbuf = (wchar_t *)buf; *pbuf != CV_WCHAR_CONST('\0'); pbuf += 1) {
					result = (result * 10) + (*pbuf - CV_WCHAR_CONST('0'));
				}
			}
			else {
				for (char *pbuf = buf; *pbuf != '\0'; pbuf += 1) {
					result = (result * 10) + (*pbuf - '0');
				}
			}
			return result;
		}
	}
#elif defined(PM)
	if (loc.isValid()) {
		const char *code = loc.gets();
		const char *p = strchr(code, '.');
		if (p != 0) {
			const char platform = p[1];
			if (platform == 'O') {
				int num_codes = sizeof(code_page) / sizeof(CodePageItm);
				// Search the list of code pages.
				// A slow, simple linear search for now.
				for (int i = 0; i < num_codes; i += 1) {
					char *a_code = code_page[i].code;
					int a_len = strlen(a_code);
					if ((p - code) == a_len && strncmp(code, a_code, a_len) == 0) {
						return code_page[i].cp;
					}
				}
			}
		}
	}
#elif defined(MOTIF)
	static char *iso8859_1_languages[] = { "en",
									"da",
									"de",
									"en",
									"es",
									"fi",
									"fr",
									"is",
									"it",
									"nl",
									"no",
									"pt",
									"sv" };
	if (loc.isValid()) {
		const char *lang = loc.gets();
		int n_languages = sizeof(iso8859_1_languages) / sizeof(char*);
		for (int i = 0; i < n_languages; i += 1) {
			if (strncmp(lang, iso8859_1_languages[i], 2) == 0) {
				return 1;
			}
		}
	}
	return unknownEncoding;
#endif
	return unknownEncoding;
}

// ---------------------------------------------------------------------------
// Returns whether strings are savable for the locale 'loc'. Call this if you
// need to know (in advance of knowing any particular string) whether there is
// any useful savability to the given locale. For example this would be called
// prior to letting the user choose a certain locale for editing strings.
// If conversions are possible but incomplete, it is be a judgment call
// whether to return TRUE or FALSE.
//
// In the current implementation there are no nice conversions, so it's just
// a matter of whether 'loc' shares the same code page with the current locale.
//
boolean VStringTable::canPut(const VLocale& loc)
{
	// This simple implementation assumes that 'charEncoding' works
	// satisfactorily, and that there are no conversions between
	// character encodings available.
	VLocale cur = VLocale::current();
	if (loc == cur) {
		return TRUE;
	}
	CharEncoding loc_encoding = charEncoding(loc);
#ifndef PM
	CharEncoding current_encoding = charEncoding(VLocale::current());
#else
	CharEncoding current_encoding = getCurrentEncoding();
#endif
	return loc_encoding != unknownEncoding && loc_encoding == current_encoding;
}
	   
// ---------------------------------------------------------------------------
// Whether there's a capability of extracting strings of the given locale,
// into VStrings of the current code page.
//
boolean VStringTable::canGet(const VLocale& loc)
{
	// This simple implementation assumes that 'charEncoding' works
	// satisfactorily, and that there are no conversions between
	// character encodings available.
	VLocale cur = VLocale::current();
	if (loc == cur) {
		return TRUE;
	}
	CharEncoding loc_encoding = charEncoding(loc);
#ifndef PM
	CharEncoding current_encoding = charEncoding(VLocale::current());
#else
	CharEncoding current_encoding = getCurrentEncoding();
#endif
	return loc_encoding != unknownEncoding && loc_encoding == current_encoding;
}

// ---------------------------------------------------------------------------
//
boolean VStringTable::supports(const VLocale& loc) const
{
	return canGet(loc) && canPut(loc);		
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store the contents of this VStringTable into VArchiver a.
//
void VStringTable::putTo(VArchiver& a)
{
	a << VStringTableArchiveTag;
	VInternationalTable::putTo(a);
}

// ---------------------------------------------------------------------------
// Load the contents of this VStringTable from VArchiver a.
//
void VStringTable::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VStringTableArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VStringTableArchiveTag));
		return;
	}
	VInternationalTable::getFrom(a); 
}

// ---------------------------------------------------------------------------
//
void VStringTable::getFrom(VArchiver& a,
						   VObject * /* data */, boolean /* do_realize */)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Store the contents of this VStringTable to a resource archiver with the
// resource name 'rsc_name'.  The named resource is shrunken, and additional
// information is stored into the resource file so that the remainder of
// the string table can be recovered efficiently using
//		getFrom(VRscArchiver&, const VString&)
//
boolean VStringTable::putTo(VRscArchiver& rsca, const VString& rsc_name) const
{
	return VInternationalTable::putTo(rsca, rsc_name);
}

// ---------------------------------------------------------------------------
// Return a newly constructed VStringTable object, representing the string
// table from the resource file open in 'rsca', and named 'rsc_name'.
// 
// Eventually this will be implemented efficiently, so that all of the strings
// in the string table need not be loaded into memory by this function.
// The newly constructed VStringTable object will maintain a connection to
// the resource file it came from.
//
// The specified resource must have been written using
//		putTo(VRscArchiver&, const VString&) const
// because a special archive format is required.
//
VStringTable *VStringTable::getFrom(VRscArchiver& rsca,
									const VString& rsc_name,
									boolean optimize)
{
	VInternationalTable *it = VInternationalTable::getFrom(rsca,
														   rsc_name,
														   optimize,
														   VStringTableCls);
	if (it != 0 && it->isA(VStringTableCls)) {
		return (VStringTable *)it;
	}
	else {
		delete it;
		return 0;
	}
}

#endif


