// $Header:   Q:/views/common/vcs/VLOCALE.CPV   1.18   Mar 24 1997 16:14:24   scottt  $

//	vlocale.cpp
//
//	VLocale class implementation [Common]
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
//	10/10/95 pkt	added 'copyFrom', 'copyTo', and 'reset'.
//	10/10/95 pkt	fixed 'putTo' because it uses a "temporary" VString.
//	10/12/95 pkt	enhanced to support Win16.
//	10/12/95 pkt	added archiver tag.
//	10/16/95 pkt	fixed to deal with extra chars after period in
//					locale name, especially on Motif.
//	11/20/95 pkt	fixed errors in subcode assignment.
//	11/21/95 pkt	moved platform-specific #includes to vlocale.h.
//	11/21/95 pkt	added 'id' and 'fromId'.
//	11/30/95 dgm	Changes for Borland C++:
//					1. cast return value of strchr() to char * (!).
//					2. MAKELCID() takes one rather than two parameters (?!).
//	12/01/95 dgm	Another minor Borland C++ (32-bit) change.
//	12/06/95 pkt	fixed declaration of idList.
//	12/15/95 tlf	fixed to compile on HPUX.
//	01/30/96 tlf	fixed to work on HPUX.
//	01/31/96 pkt	added 'description'.
//	02/01/96 dgm	Minor Borland C++ change.
//	02/16/96 pkt	MAKELCID takes one parameter in WIN16.
//  02/21/96 evc	borland 16 bit doesn't support GetUserDefaultLCID()
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "vlocale.h"
#include "vwchar.h"
#include "str.h"
#include "dictinry.h"
#if !defined(CV_NOARCHIVER)
# include "archiver.h"
# include "archvrtg.h"
#endif

defineClass(VLocale, VObject)
defineArchiveRevision(VLocale, 0)

// ---------------------------------------------------------------------------
// Construct an invalid locale.
//
VLocale::VLocale()
{
	init();
}

// ---------------------------------------------------------------------------
// Initialize to an invalid locale.
//
void VLocale::init()
{
	code[0] = '\0';
}

// ---------------------------------------------------------------------------
// Construct a locale based on the standard locale encoding.  Must consist
// either of a two-lowercase-letter language code, or a language code
// followed by underbar, followed by a two-uppercase-letter territory code.
//
// Optionally, the code may be 4 or 7 characters: 2 or 5 as described above, 
// followed by period and one uppercase character
//    .W: Windows
//    .M: Macintosh
//    .O: OS/2
//    .S: Solaris
//    .H: HP
//    .D: Dec
//    .I: AIX
//
VLocale::VLocale(const VString& std_code)
{
	init(std_code);
}

// ---------------------------------------------------------------------------
//
void VLocale::init(const VString& std_code)
{
	code[0] = '\0';
	char c0, c1, c2;
	integer size = std_code.getCharSize();
	integer i = 0;
	boolean OK = TRUE;
	if (   size >= 2
		&& VWideCharAux::wctoa(c0, std_code.wcharAt(0))
		&& VWideCharAux::wctoa(c1, std_code.wcharAt(1))
		&& 'a' <= c0 && c0 <= 'z'
		&& 'a' <= c1 && c1 <= 'z') {

		// First 2 characters are OK as language.
		code[i] = c0;
		code[i + 1] = c1;

		i += 2;
	}
	else {
		OK = FALSE;
	}


	if (   OK
		&& size >= i + 3
		&& std_code.wcharAt(i) == CV_WCHAR_CONST('_')) {
		
		if (   VWideCharAux::wctoa(c1, std_code.wcharAt(i + 1))
			&& VWideCharAux::wctoa(c2, std_code.wcharAt(i + 2))
			&& 'A' <= c1 && c1 <= 'Z'
			&& 'A' <= c2 && c2 <= 'Z') {

			// Last 3 characters are OK for territory.
			code[i] = '_';
			code[i + 1] = c1;
			code[i + 2] = c2;

			i += 3;
		}
		else {
			// Invalid territory code.
			OK = FALSE;
		}
	}
	else {
		// still OK; Territory code is optional.
	}

	if (   OK
		&& size >= i + 2
		&& std_code.wcharAt(i) == CV_WCHAR_CONST('.')) {

		if (   VWideCharAux::wctoa(c1, std_code.wcharAt(i + 1))
			&& 'A' <= c1 && c1 <= 'Z') {
		
			// Last 2 characters are OK for OS source.
			code[i] = '.';
			code[i + 1] = c1;

			i += 2;
		}
		else {
			// Invalid OS code.
			OK = FALSE;
		}
	}
	else {
		// still OK; OS is optional.
	}

	OK = OK && size == i; // require no left-over characters

	if (OK) {
		code[i] = '\0';
	}
	else {
		code[0] = '\0';
	}
}

// ---------------------------------------------------------------------------
//
VLocale::~VLocale()
{
}

// ---------------------------------------------------------------------------
// Return a locale which includes only the language portion of this locale.
//
VLocale VLocale::language() const
{
	if (!isValid() || code[3] == 0) {
		return *this;
	}
	else {
		// This locale has a territory, to be truncated.
		return VLocale(VString(code, 2));
	}
}

// ---------------------------------------------------------------------------
// Return a locale which includes only the language and region portion of this locale.
//
VLocale VLocale::region() const
{
	if (!isValid() || code[2] == 0 || (code[2] == '_' && code[6] == 0)) {
		return *this;
	}
	else {
		// This locale has an OS code, to be truncated.
		return VLocale(VString(code, (code[2] == '_') ? 5 : 2));
	}
}
// ---------------------------------------------------------------------------
//
boolean VLocale::isEqual(VObject *obj) const
{
	if (obj->getIsA() == VLocaleCls) {
		VLocale *that = (VLocale *)obj;
		return strcmp(code, that->code) == 0;
	}
	else {
		return 0;
	}
}

// ---------------------------------------------------------------------------
// The more general (shorter) locale designation compares greater.
// This function will return 1 (left operand greater) when the two locales 
// are incomparable.  Therefore, to test whether locale x is at least as
// general as y, one must test
//             y <= x
// because  x >= y  will return TRUE  
//
int VLocale::compare(VObject *obj) const
{
	if (obj->getIsA() == VLocaleCls) {
		VLocale *that = (VLocale *)obj;
		const char *p = code;
		const char *tp = that->code;
		for (;; p += 1, tp += 1) {
			if (*tp == '\0') {
				if (*p == '\0') {
					// equal
					return 0;
				}
				else {
					// 'that' is shorter.
					return -1;
				}
			}
			else if (*p != *tp) {
				// Either 'this' is shorter, or the locales are incomparable.
				return 1;
			}
		}
	}
	else {
		return 1;
	}
}

// ---------------------------------------------------------------------------
//
HashKey VLocale::hash() const
{
	HashKey key = 0;
	if (code[0] != 0) {
		key = code[1];
		key	^= code[0] << 1;
	}
	if (code[2] != 0) {
		key ^= code[3] << 2;
	}
	return key & HASH_KEY_MASK;
}

// ---------------------------------------------------------------------------
//
void VLocale::reset()
{
	VObject::reset();
	code[0] = '\0';
}

// ---------------------------------------------------------------------------
//
boolean VLocale::copyTo(VObject *obj)
{
	VObject::copyTo(obj);
	if (obj->isA(VLocaleCls)) {
		VLocale *loc = (VLocale *)obj;
		memcpy(loc->code, code, sizeof(code));
		return TRUE;
	}
	return FALSE;		
}

// ---------------------------------------------------------------------------
//
boolean VLocale::copyFrom(VObject *obj)
{
	VObject::copyFrom(obj);
	if (obj->isA(VLocaleCls)) {
		VLocale *loc = (VLocale *)obj;
		memcpy(code, loc->code, sizeof(code));
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VLocale::isValid() const
{
	return code[0] != 0;
}

// ---------------------------------------------------------------------------
//
const char *VLocale::gets() const
{
	return code;
}

// ---------------------------------------------------------------------------
//
#if defined(CV_WINDOWS)
static const int code_count = 0x25;
static char *std_code[code_count] = {
	0,
	"ar", // Arabic         0x01
	"bg", // Bulgarian      0x02
	"ca", // Catalan        0x03
	"zh", // Chinese        0x04
	"cs", // Czech          0x05
	"da", // Danish         0x06
	"de", // German         0x07
	"el", // Greek          0x08
	"en", // English        0x09
	"es", // Spanish        0x0a
	"fi", // Finnish        0x0b
	"fr", // French         0x0c
	"iw", // Hebrew         0x0d
	"hu", // Hungarian      0x0e
	"is", // Icelandic      0x0f
	"it", // Italian        0x10
	"ja", // Japanese       0x11
	"ko", // Korean         0x12
	"nl", // Dutch          0x13
	"no", // Norwegian      0x14
	"pl", // Polish         0x15
	"pt", // Portuguese     0x16
	"rm", // Rhaeto/Romance 0x17
	"ro", // Romanian       0x18
	"ru", // Russian        0x19
	"sh", // Serbo-Croatian 0x1a
	"sk", // Slovak         0x1b
	0,
	"sv", // Swedish        0x1d
	"th", // Thai           0x1e
	"tr", // Turkish        0x1f
	"ur", // Urdu           0x20
	0,    // Bahasa         0x21
	0,
	0,
	"sl"  // Slovenian      0x24
};
#elif defined(PM)
typedef struct { ULONG id; char *code; } CtryCodeItm;
static CtryCodeItm std_code[] = {
	785, "ar", // Arabic
	 86, "zh", // Chinese
	 88, "zh_TW", // Taiwan
	 42, "cs", // Czech
	 45, "da", // Danish
	 49, "de", // German
	 44, "en_UK", // United Kingdom
	  1, "en_US", // U.S.A.
	 99, "en", // Asian English -- should be more specific?
	 61, "en_AU",  // Australia
	 32, "fr_BE", // Belgium (French language)
	 32, "nl_BE", // Belgium (Dutch language)
	 34, "es", // 034 is Spain, Spanish
	  3, "es", // 003 is Latin America, "es" is Spanish -- should be more specific?
	358, "fi", // Finnish
	 33, "fr", // French
	  2, "fr_CA",  // French, Canadian
	972, "iw", // Hebrew
	 36, "hu", // Hungarian
	354, "is", // Icelandic
	 39, "it", // Italian
	 81, "ja", // Japanese
	 82, "ko", // Korean
	 31, "nl", // Dutch
	 47, "no", // Norwegian
	 48, "pl", // Polish
	351, "pt", // Portuguese
	 55, "pt", // Brazil -- should be more specific?
	 38, "sh", // 038 is Yugoslavia. -- "sh" is Serbo-Croation
	 41, "de_CH", // 041 is Switzerland -- "de" is arbitrary.
	 46, "sv", // Swedish
	 90, "tr" // Turkish
};
#endif

// ---------------------------------------------------------------------------
//
VLocale VLocale::fromId(VPlatformLocale pl)
{
#if defined(CV_WINDOWS)
	LANGID lid = LANGIDFROMLCID(pl);
	LANGID plid = PRIMARYLANGID(lid);
	LANGID slid = SUBLANGID(lid);
	if (lid == 0) {
		// failed
	}
	else if (plid == LANG_NEUTRAL) {
		// Doesn't know what language is going on.
	}
	else if (0 <= plid && plid < code_count) {
		const char *code = std_code[plid];
		const char *scode = 0;
		if (code != 0) {
			if (plid == LANG_DUTCH && slid == SUBLANG_DUTCH_BELGIAN) {
				scode = "BE";
			}
			if (plid == LANG_ENGLISH && slid == SUBLANG_ENGLISH_US) {
				scode = "US";
			}
			if (plid == LANG_ENGLISH && slid == SUBLANG_ENGLISH_UK) {
				scode = "UK";
			}
			if (plid == LANG_ENGLISH && slid == SUBLANG_ENGLISH_AUS) {
				scode = "AU";
			}
			if (plid == LANG_ENGLISH && slid == SUBLANG_ENGLISH_CAN) {
				scode = "CA";
			}
			if (plid == LANG_FRENCH && slid == SUBLANG_FRENCH_BELGIAN) {
				scode = "BE";
			}
			if (plid == LANG_FRENCH && slid == SUBLANG_FRENCH_CANADIAN) {
				scode = "CA";
			}
			if (plid == LANG_FRENCH && slid == SUBLANG_FRENCH_SWISS) {
				scode = "CH";
			}
			if (plid == LANG_GERMAN && slid == SUBLANG_GERMAN_SWISS) {
				scode = "CH";
			}
			if (plid == LANG_ITALIAN && slid == SUBLANG_ITALIAN_SWISS) {
				scode = "CH";
			}
			VString lang = code;
			if (scode != 0) {
				lang += "_";
				lang += scode;
			}
			return VLocale(lang + VString(".W"));
		}
	}
#elif defined(PM)
	ULONG country = pl;
	// typedef struct { ULONG id; char *code } CtryCodeItm;
	// static CtryCodeItm std_code[]
	int num_codes = sizeof(std_code) / sizeof(CtryCodeItm);
	// Search the list of country codes.
	// A slow, simple linear search for now.
	for (int i = 0; i < num_codes; i += 1) {
		if (std_code[i].id == country) {
			return VLocale(VString(std_code[i].code) + VString(".O"));
		}
	}
#elif defined(MOTIF)
	VString ls = pl;
	if (ls == "C") {
		// Not a useful locale.
		// Views failed to coax a useful locale out of the system
		// when it started up.
	}
	else {
		char *variant =
#if defined(SOLARIS)
						".S"
#elif defined(CPPV_HPUX)
						".H"
#elif defined(CPPV_AIX6000)
						".I"
#elif defined(__osf__) && defined(__DECCXX)
						".D"
#else
						0
#endif
						;
		integer period_loc = ls.contains(".");
		if (period_loc >= 0) {
			ls.truncAtCharPos(period_loc);
		}	
#ifdef CPPV_HPUX
		// Map the non-standard HPUX names to standard encodings.
		static VDictionary *lookup = 0;
		if (lookup == 0 && (lookup = new VDictionary) != 0) {
			lookup->atKeyPut(new VString("arabic"), new VString("ar"));
			lookup->atKeyPut(new VString("bulgarian"), new VString("bg"));
			lookup->atKeyPut(new VString("catalan"), new VString("ca"));
			lookup->atKeyPut(new VString("chinese-s"), new VString("zh"));
			lookup->atKeyPut(new VString("chinese-t"), new VString("zh_TW"));
			lookup->atKeyPut(new VString("czech"), new VString("cs"));
			lookup->atKeyPut(new VString("danish"), new VString("da"));
			lookup->atKeyPut(new VString("german"), new VString("de"));
			lookup->atKeyPut(new VString("german in Switzerland"), new VString("de_CH"));
			lookup->atKeyPut(new VString("greek"), new VString("el"));
			lookup->atKeyPut(new VString("english in Asia"), new VString("en"));
			lookup->atKeyPut(new VString("english in Australia"), new VString("en_AU"));
			lookup->atKeyPut(new VString("english in Canada"), new VString("en_CA"));
			lookup->atKeyPut(new VString("english"), new VString("en_UK"));
			lookup->atKeyPut(new VString("american"), new VString("en_US"));
			lookup->atKeyPut(new VString("spanish"), new VString("es"));
			lookup->atKeyPut(new VString("finnish"), new VString("fi"));
			lookup->atKeyPut(new VString("french"), new VString("fr"));
			lookup->atKeyPut(new VString("belgium (French language)"), new VString("fr_BE"));
			lookup->atKeyPut(new VString("french in Switzerland"), new VString("fr_CH"));
			lookup->atKeyPut(new VString("hebrew"), new VString("iw"));
			lookup->atKeyPut(new VString("hungarian"), new VString("hu"));
			lookup->atKeyPut(new VString("icelandic"), new VString("is"));
			lookup->atKeyPut(new VString("italian"), new VString("it"));
			lookup->atKeyPut(new VString("italian in Switzerland"), new VString("it_CH"));
			lookup->atKeyPut(new VString("japanese"), new VString("ja"));
			lookup->atKeyPut(new VString("korean"), new VString("ko"));
			lookup->atKeyPut(new VString("dutch"), new VString("nl"));
			lookup->atKeyPut(new VString("dutch in Belgium"), new VString("nl_BE"));
			lookup->atKeyPut(new VString("norwegian"), new VString("no"));
			lookup->atKeyPut(new VString("polish"), new VString("pl"));
			lookup->atKeyPut(new VString("portuguese"), new VString("pt"));
			lookup->atKeyPut(new VString("rhaeto/Romance"), new VString("rm"));
			lookup->atKeyPut(new VString("rumanian"), new VString("ro"));
			lookup->atKeyPut(new VString("russian"), new VString("ru"));
			lookup->atKeyPut(new VString("serbocroatian"), new VString("sh"));
			lookup->atKeyPut(new VString("slovak"), new VString("sk"));
			lookup->atKeyPut(new VString("slovene"), new VString("sl"));
			lookup->atKeyPut(new VString("swedish"), new VString("sv"));
			lookup->atKeyPut(new VString("thai"), new VString("th"));
			lookup->atKeyPut(new VString("turkish"), new VString("tr"));
			lookup->atKeyPut(new VString("urdu"), new VString("ur"));
		}
		if (lookup != 0) {
			VString *lsp = &ls;
			VObject *vptr = (VObject *)lsp;
			VString *val = (VString *)lookup->getValueAtKey(vptr);
			if (val != 0) {
				ls = *val;
			}
		}
#endif
		if (variant != 0) {
			return VLocale(ls + VString(variant));
		}
	}
#endif
	return VLocale();
}

// ---------------------------------------------------------------------------
//
static char thisPlatformChar()
{
#if defined(CV_WINDOWS)
	return 'W';
#elif defined(PM)
	return 'O';
#elif defined(MOTIF)
# if defined(SOLARIS)
	return 'S';
# elif defined(CPPV_HPUX)
	return 'H';
# elif defined(CPPV_AIX6000)
	return 'I';
# elif defined(__osf__) && defined(__DECCXX)
	return 'D';
# else
	return '\0';
# endif
#else
	return '\0';
#endif
}

// ---------------------------------------------------------------------------
//
VLocale	VLocale::current()
{
#if defined(CV_WINDOWS) && !(defined (CV_WIN16) && defined (__BCPLUSPLUS__))
	return fromId(GetUserDefaultLCID());
#elif defined (CV_WIN16) && defined (__BCPLUSPLUS__)
	// borland 16 bit doesn't support GetUserDefaultLCID()

	return VLocale();
#elif defined(PM)
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
		return fromId(info.country);
	}
#elif defined(MOTIF)
	char *loc = setlocale(LC_CTYPE, 0);
	VString ls = (loc == 0) ? "" : loc;
#ifdef CPPV_HPUX
		// The string returned by "setlocale" on HPUX is of the
		// form:
		//		"/\00n:<name>;/"
		//
		// Here, we get rid of everything EXCEPT the <name>
		// between the ":" and the ";".
		integer punct_loc = ls.contains(";");
		if (punct_loc >= 0) {
			ls.truncAtCharPos(punct_loc);
		}	
		punct_loc = ls.contains(":");
		if (punct_loc >= 0) {
			ls.deleteAtCharPos(0,punct_loc + 1);
		}	
#endif
# ifdef LC_MESSAGES
	char *mloc = setlocale(LC_MESSAGES, 0);
	VString mls = (mloc == 0) ? "" : loc;
#ifdef CPPV_HPUX
		punct_loc = mls.contains(";");
		if (punct_loc >= 0) {
			mls.truncAtCharPos(punct_loc);
		}	
		punct_loc = mls.contains(":");
		if (punct_loc >= 0) {
			mls.deleteAtCharPos(0, punct_loc + 1);
		}	
#endif
	if (ls != mls) {
		ls = "";
	}
# endif
	if (ls == "C") {
		// Not a useful locale.
		// Views failed to coax a useful locale out of the system
		// when it started up.
	}
	else {
		char *variant =
#if defined(SOLARIS)
						".S"
#elif defined(CPPV_HPUX)
						".H"
#elif defined(CPPV_AIX6000)
						".I"
#elif defined(__osf__) && defined(__DECCXX)
						".D"
#else
						0
#endif
						;
		integer period_loc = ls.contains(".");
		if (period_loc >= 0) {
			ls.truncAtCharPos(period_loc);
		}	
#ifdef CPPV_HPUX
		period_loc = ls.contains(";");
		if (period_loc >= 0) {
			ls.truncAtCharPos(period_loc);
		}	
		period_loc = ls.contains(":");
		if (period_loc >= 0) {
			ls.deleteAtCharPos(period_loc, period_loc + 1);
		}	
#endif
		
		if (variant != 0) {
			return fromId(ls + VString(variant));
		}
	}
#endif
	return VLocale();
}

#if defined(MOTIF)
static struct IdLink { const char *id; struct IdLink *next; } *idList = 0;
#endif

// ---------------------------------------------------------------------------
// Return the platform specific locale id for this VLocale.
//
VPlatformLocale	VLocale::id() const
{
	if (!isValid()) {
		return UNKNOWN_LOCALE;
	}
	char *p = (char *)strchr(code, '.');
	if (p == 0) {
		return UNKNOWN_LOCALE;
	}
	const char platform = p[1];
#if defined(CV_WINDOWS)
	if (platform != 'W') {
		return UNKNOWN_LOCALE;
	}
	for (LANGID plid = 0; plid < code_count; plid += 1) {
		char *this_code = std_code[plid];
		if (this_code != 0 && this_code[0] == code[0] && this_code[1] == code[1]) {
			LANGID slid = 0;
			if (code[2] == '_') {
				const char *scode = code + 3;
				boolean BE = scode[0] == 'B' && scode[1] == 'E';
				boolean US = scode[0] == 'U' && scode[1] == 'S';
				boolean UK = scode[0] == 'U' && scode[1] == 'K';
				boolean AU = scode[0] == 'A' && scode[1] == 'U';
				boolean CA = scode[0] == 'C' && scode[1] == 'A';
				boolean CH = scode[0] == 'C' && scode[1] == 'H';
				if (plid == LANG_DUTCH && BE) {
					slid = SUBLANG_DUTCH_BELGIAN;
				}
				if (plid == LANG_ENGLISH && US) {
					slid = SUBLANG_ENGLISH_US;
				}
				if (plid == LANG_ENGLISH && UK) {
					slid = SUBLANG_ENGLISH_UK;
				}
				if (plid == LANG_ENGLISH && AU) {
					slid = SUBLANG_ENGLISH_AUS;
				}
				if (plid == LANG_ENGLISH && CA) {
					slid = SUBLANG_ENGLISH_CAN;
				}
				if (plid == LANG_FRENCH && BE) {
					slid = SUBLANG_FRENCH_BELGIAN;
				}
				if (plid == LANG_FRENCH && CA) {
					slid = SUBLANG_FRENCH_CANADIAN;
				}
				if (plid == LANG_FRENCH && CH) { 
					slid = SUBLANG_FRENCH_SWISS;
				}
				if (plid == LANG_GERMAN && CH) {
					slid = SUBLANG_GERMAN_SWISS;
				}
				if (plid == LANG_ITALIAN && CH) {
					slid = SUBLANG_ITALIAN_SWISS;
				}
				if (slid == 0) {
					return UNKNOWN_LOCALE;
				}
			}
#if defined(CV_WIN16)
			return MAKELCID(MAKELANGID(plid, slid));
#else
			return MAKELCID(MAKELANGID(plid, slid), SORT_DEFAULT);
#endif
		}
	}
#elif defined(PM)
	if (platform != 'O') {
		return UNKNOWN_LOCALE;
	}
	int num_codes = sizeof(std_code) / sizeof(CtryCodeItm);
	// Search the list of country codes.
	// A slow, simple linear search for now.
	for (int i = 0; i < num_codes; i += 1) {
		char *a_code = std_code[i].code;
		int a_len = strlen(a_code);
		if ((p - code) == a_len && strncmp(code, a_code, a_len) == 0) {
			return std_code[i].id;
		}
	}

#elif defined(MOTIF)
	char *variant =
#if defined(SOLARIS)
					".S"
#elif defined(CPPV_HPUX)
					".H"
#elif defined(CPPV_AIX6000)
					".I"
#elif defined(__osf__) && defined(__DECCXX)
					".D"
#else
					0
#endif
					;
	if (variant == 0 || variant[0] == '\0' || variant[1] != platform) {
		return UNKNOWN_LOCALE;
	}
	VString ls = code;
	integer period_loc = ls.contains(".");
	if (period_loc >= 0) {
		ls.truncAtCharPos(period_loc);
	}
	// We keep a list so that each locale will have its own pointer
	// value, for easy comparison purposes.
	// static struct IdLink { const char *id; struct IdLink *next; } idList = 0;
	struct IdLink *ip = idList;
	while (ip != 0) {
		if (ls == ip->id) {
			return ip->id;
		}
		ip = ip->next;
	}
	// Not found on the list; add a link.
	ip = idList;
	idList = new IdLink;
	idList->next = ip;
	char *locid = new char[ls.size() + 1];
	strcpy(locid, ls.gets());
	idList->id = locid;
	return locid;		
#endif
	return UNKNOWN_LOCALE;
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store the contents of this VLocale into VArchiver a.
//
void VLocale::putTo(VArchiver& a)
{
	a << VLocaleArchiveTag;

	VString str(code);
	str.isTemporaryObject(TRUE);
	a.putObject(&str);
}

// ---------------------------------------------------------------------------
// Load the contents of this VLocale from VArchiver a.
//
void VLocale::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VLocaleArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VLocaleArchiveTag));
		return;
	}

	VString str;
	a.getObject(&str);
	VLocale temp(str); // Validate the string.
	this->operator=(temp);
}

// ---------------------------------------------------------------------------
//
void VLocale::getFrom(VArchiver& a,
						   VObject * /* data */, boolean /* do_realize */)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Return a descriptive name for this locale.
//
VString VLocale::description() const
{
	if (!isValid()) {
		return "invalid locale";
	}
	static VDictionary *languages = 0;
	if (languages == 0 && (languages = new VDictionary) != 0) {
		languages->atKeyPut(new VString("ar"), new VString("Arabic"));
		languages->atKeyPut(new VString("bg"), new VString("Bulgarian"));
		languages->atKeyPut(new VString("ca"), new VString("Catalan"));
		languages->atKeyPut(new VString("zh"), new VString("Chinese"));
		languages->atKeyPut(new VString("cs"), new VString("Czech"));
		languages->atKeyPut(new VString("da"), new VString("Danish"));
		languages->atKeyPut(new VString("de"), new VString("German"));
		languages->atKeyPut(new VString("el"), new VString("Greek"));
		languages->atKeyPut(new VString("en"), new VString("English"));
		languages->atKeyPut(new VString("es"), new VString("Spanish"));
		languages->atKeyPut(new VString("fi"), new VString("Finnish"));
		languages->atKeyPut(new VString("fr"), new VString("French"));
		languages->atKeyPut(new VString("iw"), new VString("Hebrew"));
		languages->atKeyPut(new VString("hu"), new VString("Hungarian"));
		languages->atKeyPut(new VString("is"), new VString("Icelandic"));
		languages->atKeyPut(new VString("it"), new VString("Italian"));
		languages->atKeyPut(new VString("ja"), new VString("Japanese"));
		languages->atKeyPut(new VString("ko"), new VString("Korean"));
		languages->atKeyPut(new VString("nl"), new VString("Dutch"));
		languages->atKeyPut(new VString("no"), new VString("Norwegian"));
		languages->atKeyPut(new VString("pl"), new VString("Polish"));
		languages->atKeyPut(new VString("pt"), new VString("Portuguese"));
		languages->atKeyPut(new VString("rm"), new VString("Rhaeto/Romance"));
		languages->atKeyPut(new VString("ro"), new VString("Romanian"));
		languages->atKeyPut(new VString("ru"), new VString("Russian"));
		languages->atKeyPut(new VString("sh"), new VString("Serbo-Croatian"));
		languages->atKeyPut(new VString("sk"), new VString("Slovak"));
		languages->atKeyPut(new VString("sl"), new VString("Slovenian"));
		languages->atKeyPut(new VString("sv"), new VString("Swedish"));
		languages->atKeyPut(new VString("th"), new VString("Thai"));
		languages->atKeyPut(new VString("tr"), new VString("Turkish"));
		languages->atKeyPut(new VString("ur"), new VString("Urdu"));
	}
	static VDictionary *countries = 0;
	if (countries == 0 && (countries = new VDictionary) != 0) {
		countries->atKeyPut(new VString("AU"), new VString("Australia"));
		countries->atKeyPut(new VString("BE"), new VString("Belgium"));
		countries->atKeyPut(new VString("CA"), new VString("Canada"));
		countries->atKeyPut(new VString("CH"), new VString("Switzerland"));
		countries->atKeyPut(new VString("TW"), new VString("Taiwan"));
		countries->atKeyPut(new VString("UK"), new VString("United Kingdom"));
		countries->atKeyPut(new VString("US"), new VString("U.S.A."));
	}
	int code_len = strlen(code);
	const char *period = strchr(code, '.');
	if (period != 0) {
		code_len = period - code;
	}
	VString *language = 0;
	VString *country = 0;
	VString platform_str;
	VString *platform = 0;
	boolean can_describe = TRUE;
	if (code_len >= 2) {
		if (languages != 0) {
			VString lcode(code, 2);
			language = (VString *)languages->getValueAtKey(&lcode);
		}
		can_describe = language != 0;
	}
	if (code_len >= 5) {
		if (countries != 0) {
			VString ccode(code + 3, 2);
			country = (VString *)countries->getValueAtKey(&ccode);
		}
		can_describe = can_describe && country != 0;
	}

	// Handle the platform. Display only if the locale platform differs from
	// this platform.
	char plat_char = (code[code_len] == '.') ? code[code_len + 1] : '\0';
	switch(plat_char) {
	case 'W':
		platform_str = "Windows";
		break;
	case 'O':
		platform_str = "OS/2";
		break;
	case 'S':
		platform_str = "Solaris";
		break;
	case 'H':
		platform_str = "HP-UX";
		break;
	case 'I':
		platform_str = "AIX";
		break;
	case 'D':
		platform_str = "Digital UNIX";
		break;
	}
	if (plat_char == thisPlatformChar()) {
		// OK; don't show the platform.
	}
	else if (platform_str.size() > 0) {
		// OK; show the platform.
		platform = &platform_str;
	}
	else {
		can_describe = FALSE;
	}

	if (can_describe) {
		VString result;
		if (language != 0) {
			result += *language;
		}
		if (country != 0) {
			result += "(";
			result += *country;
			result += ")";
		}
		else if (platform != 0) {
			result += " ";
		}
		if (platform != 0) {
			result += *platform;
		}
		return result;
	}
	return gets();
}

#endif



