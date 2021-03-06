// $Header:   Q:/views/common/vcs/STR.CPV   1.40   Mar 24 1997 16:13:26   scottt  $ 

//  str.cpp
//
//  VString class implementation [Common]
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  02/16/92 dr     added getNamedResource, putNamedResource
//  09/09/92 jmd    made WIN31 auto define MS_WINDOWS
//  09/13/92 jmd    preened
//  10/16/92 gjl    added VString &VString::operator=(const
//                  VString &s) for ultrix
//  04/01/93 pmcm   added const to gets() per motif 2.1  source
//  04/07/93 pmcm   changed id to VObject *
//  04/16/93 pmcm   added matching va_end for va_start
//  05/14/93 pmcm   added check on data in puts() for putting to same
//  05/17/93 pmcm   merged MOTIF and MS-Windows versions, including:
//  03/08/93 gjl    added const to copy constructor
//  03/30/93 pkt    fixed VString::compare to use
//                  case sensitive strcmp in MS_WINDOWS
//  06/24/93 pmcm   added null check to isEqual
//  07/19/93 jmd    made size() const
//  07/22/93 jmd    made char * args into const char *, preened
//  07/31/93 jmd    made compare, isEqual etc. const
//  07/31/93 pkt    eliminated #ifdef macintosh.
//  08/30/93 jmd    added return value to getResource()
//  10/12/93 jmd    fixed notifier include guard
//  10/18/93 dgm    UNIX changes; minor.
//  10/29/93 pkt    changed getResource to return # chars in string.
//  11/02/93 pkt    made gets(char *, integer) const.
//  12/27/93 pkt    fixed for Symantec compiler.
//  01/13/94 dgm    merged and preened
//  01/14/94 pkt    removed Macintosh #define of VNOTIFIER_H and VCOLOR_H.
//  01/19/94 pkt    preened and merged with Macintosh.
//  01/17/94 dgm    cfront C++ changes
//  02/24/94 pmcm   fixed expandTo() to initialize brand new buffer correctly.
//  03/07/94 pmcm   included wintypes.h after winclass.h to prevent
//                  multiple windows.h inclusion
//  03/24/94 pmcm   auto defined MS_WINDOWS under CV_WIN32
//  06/06/94 jld    removed VMemory::memory from setData(), init()
//                  and expandTo()
//  07/06/94 pkt    grabbed speedup for 'mStripAmpersands' & 'stripCmdKey'.
//  07/06/94 pkt    grabbed speedup and fix to 'concat' and 'puts'.
//  07/11/94 dgm    merged.
//  07/11/94 dgm    fixed up assignment-operator and copy-constructor.
//  07/14/94 jld    added defs.h dependency for MS_WINDOWS
//  07/29/94 pkt    added 'identifyFormatConversion', 'measureFormat', and
//                  selectFormatBuffer.
//  08/12/94 pkt    fixed 'concat(VString*)' not to do virtual call.
//  08/23/94 dgm    Fix for SPARCworks C++ compiler bug in expandTo();
//                  cannot delete [] null-pointer without messing up!
//  08/30/94 pkt    updated for VNOMALLOC version of VMemory.
//  09/08/94 dgm    Minor coding change to suppress MetaWare C++ warning.
//  09/13/94 pkt    workaround to Metrowerks compiler bug.
//  09/30/94 jld    fixed memory leak or error in getFrom()
//  10/18/94 dss    redefine lstrcmp for MSDOS (was just MS_WINDOWS).
//  12/06/94 pkt    fixed 'prints' to work for VStreams, too (regression on
//                  07/29/94).
//  12/16/94 pkt    fixed 'getFrom' to match new behavior of
//                  VArchiver::operator>>.
//  01/19/95 pkt    overhauled for internationalization.
//  01/30/95 pkt    fixes to MBCS support, including activation of the
//                  'dataCapacity' member.
//  02/01/95 pkt    changed 'dataCapacity' to 'nEle'.
//  02/02/95 pkt    changed 'tabTo' to 'padTo'.
//  02/07/95 pkt    fix to 'asWideString'.
//  02/08/95 pkt    fix to 'asWideString' to prevent crashing.
//  02/09/95 pkt    added checks for SINGLE_BYTE_CHAR_SET so that VStrings
//                  don't needlessly mutate to wchar_t representation.
//  02/09/95 pkt    fixes to 'toLower', 'toUpper' and other minor fixes.
//  02/10/95 pkt    added support for CV_CALL_SINGLE_BYTE_GETS.
//  03/07/95 pkt    added overloaded variant taking VString& argument.
//  03/09/95 pkt    fixed 'concat' to check for null pointer before calling
//                  'wcslen'.
//  03/20/95 pkt    updated 'stripLeading' and 'stripTrailing'.
//  03/24/95 pkt    small fixes to manipulation functions.
//  03/24/95 pkt    changed 'asWideString' to call 'size' non-virtually.
//  03/30/95 pkt    updated 'getFrom' for internationalization changes.
//  03/30/95 pkt    updated so 'asByteFlag' can coexist with 'asWCharFlag'.
//  04/05/95 pkt    fixed 'asWideString' to not assume 0 at end of data.
//  04/11/95 pkt    updated for VWideChar.
//  04/11/95 pkt    updated for VWideCharAux.
//  04/12/95 pkt    worked around 'iswxxx' problem.
//  04/21/95 pkt    replaced MB_XXX with CV_MB_XXX.
//  05/19/95 pkt    merged for common source.
//  05/25/95 dgm    minor changes for Visual C++ 2.1.
//  05/26/95 pkt    fixed to define lstrcmp macro on most platforms.
//  05/26/95 pkt    combined international changes into common version.
//  07/13/95 pkt    changed PM's 'scanAmper' to accept a const char *.
//  07/21/95 pkt    changed 'gets(VWideChar *, integer)' to not be const.
//  08/16/95 pkt    added 'swapData'.
//  08/17/95 pkt    fixed 'atowc' to get mbtowc from the VWideCharAux class.
//  08/25/95 pkt    changed to get strcoll, toupper, and tolower from the
//                  VWideCharAux class.
//  09/05/95 pkt    completed 8/25 change for non-OS/2 platforms.
//  09/06/95 pkt    removed OFI code.
//  09/06/95 pkt    updated for revised C++ standard for loops.
//  09/11/95 pkt    fixed va_start macros to compile under Unix.
//  09/14/95 pkt    changed 'size' to return size in bytes of this string
//                  as a multibyte string.
//  09/18/95 dss    Put missing parentheses around VWideCharAux::tolower
//                  and toupper to avoid conflicts with global macro defs.
//  09/19/95 pkt    fixed stack overflow in 'asWideString'.
//  10/03/95 pkt    moved 'atowc' and 'wctoa' to VWideCharAux.
//  10/09/95 pkt    fixed 'putTo' to work when given a string in wide
//                  character internal representation.
//  11/29/95 tlf    updated for HPUX.
//  12/12/95 pkt    added variants of 'puts'.
//  12/12/95 pkt    fixed VString(const VWideChar*, unsigned int).
//  01/05/96 pkt    fixed 'insertAtCharPos' wide char case.
//  01/10/96 pkt    changed new 'puts' variants to 'write'.
//  01/31/96 pkt    fixed VString(const char *, unsigned int).
//  01/31/96 pkt    fixed VString(const VWideChar*, unsigned int length) to
//                  work when the length is 1.
//  02/13/96 pkt    added 'gets(char *, integer) const' and 
//                  'gets(VWideChar *, integer) const'.
//  02/15/96 pkt    changed 'charReferenceAt' to avoid warnings in 16-bit
//                  compiles.
//  02/21/96 pkt    fixed 'padTo' to terminate string properly.
//  04/08/96 glm    check for Symantec
//  06/05/96 pkt    fixed 'insertAtCharPos' not to loop nastily when
//                  insertion is empty.
//	09/06/96 glm	removed extern C from include of math.h for VC4.2
//	09/27/96 pkt	fixed 'insertAtCharPos' not to crash when insertion point
//					is invalid (negative).
//	11/06/96 pkt	revised to work with no notifier.
//	11/07/96 pkt	tweaked to work around mystifying behavior of MSVC2.2.
//	11/15/96 pkt	fixed 'charAt' and 'wcharAt' to not crash when data==0.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------


#include "defs.h"
#include "winclass.h"
#include "wintypes.h"
#include "cvassert.h"
#include <limits.h>
#if (defined(CV_WINDOWS) || defined(PM)) && !defined(CV_NO_NOTIFIER)
#   include "notifier.h"
#endif
#include "archiver.h"
#include "str.h"

#if !defined(lstrcmp)
# if defined(MSDOS) || defined(V_MACINTOSH) || defined(UNIX) || defined(PM)
#   define lstrcmp strcmp
# endif
#endif

extern "C" {
#   undef NULL
#   include <stdarg.h>
#   include <stdio.h>
#   include <string.h>
#   include <ctype.h>
#   ifdef WCHAR_H_EXISTS
#    include <wchar.h>
#   else
#    include "vwchar.h"
#   endif
}

#include <math.h>

#ifdef UNICODE
const static boolean default_is_wchar = !CV_CALL_SINGLE_BYTE_GETS;
#else
const static boolean default_is_wchar = FALSE;
#endif

// ---------------------------------------------------------------------------
//
defineClass(VString, VObject)

// ---------------------------------------------------------------------------
// Construct a VString of zero length.
//
VString::VString() :
    asByteFlag(0),
    asWCharFlag(default_is_wchar)
{
    data = 0;
    nEle = 0;
}

// ---------------------------------------------------------------------------
// Construct a VString with contents from the NULL terminated C string str.
//
VString::VString(const char *str) :
    asByteFlag(0),
    asWCharFlag(0)
{
    data = 0;
    nEle = 0;

    if (str != 0) {
        puts(str);
    }
}

// ---------------------------------------------------------------------------
// Construct a VString that is an independent copy of s.
//
VString::VString(const VString &s) :
    asByteFlag(s.asByteFlag)
{
    data = 0;
    nEle = 0;
    init();
    asWCharFlag = s.asWCharFlag;
    if (s.asWCharFlag && !CV_CALL_SINGLE_BYTE_GETS) {
        VString::puts(s.getwcs());
    }
    else {
        VString::puts(s.gets());
    }
}

// ---------------------------------------------------------------------------
// Construct a VString with contents from the array of char starting with s
// and consisting of length bytes.  Although the the array s is measured in 
// bytes, this VString will be interpreted as a multibyte character string.
//
VString::VString(const char *s, unsigned int length) :
    asByteFlag(0)
{
    data = 0;
    nEle = 0;
    init();
    asWCharFlag = 0;
    if (length > 0) {
        expandTo(length + 1);
        if (data != 0) {
            memcpy(data, s, length);
            data[length] = '\0';
        }
    }
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Construct a VString with contents from the NULL terminated C string str.
//
VString::VString(const VWideChar *str) :
    asByteFlag(0),
    asWCharFlag(1)
{
    data = 0;
    nEle = 0;

    if (str != 0) {
        puts(str);
    }
}

// ---------------------------------------------------------------------------
// Construct a VString with contents from the array of char starting with s
// and consisting of length characters.
//
VString::VString(const VWideChar *s, unsigned int length) :
    asByteFlag(0)
{
    data = 0;
    nEle = 0;
    init();
    asWCharFlag = 1;
    if (length > 0) {
        expandTo(length + 1);
        if (data != 0) {
            memcpy(data, s, length * sizeof(VWideChar));
            getWideData()[length] = 0;
        }
    }
}
#endif

// ---------------------------------------------------------------------------
//
VString::~VString()
{
    VString::init();
}

// ---------------------------------------------------------------------------
// Return the number of bytes in this VString when viewed as a
// multibyte character string.
//
integer VString::size() const
{
    if (data == 0) {
        return 0;
    }
    if (asWCharFlag) {
        if (asByteFlag || SINGLE_BYTE_CHAR_SET) {
            // no need to transmute the string
            return VWideCharAux::wcslen(getWideData()); // ISO C Amendment 1 function
        }
        // Switch to multibyte form.
        asWideString(FALSE);
    }
    return strlen(data);
}

// ---------------------------------------------------------------------------
// Return the number of characters in this VString.
//
integer VString::getCharSize() const
{
    if (data == 0) {
        return 0;
    }
    else if (asWCharFlag) {
        return VWideCharAux::wcslen(getWideData());
    }
    else if (asByteFlag || SINGLE_BYTE_CHAR_SET) {
        return strlen(data);
    }
    else {
        return VWideCharAux::mbslen(data);
    }
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Store the contents of this VString into VArchiver a.
//
void VString::putTo(VArchiver& a)
{
    if (data == 0) {
        a << data;
    }
    else {
        a << gets();
    }
}

// ---------------------------------------------------------------------------
// Load the contents of this VString from VArchiver a.
//
void VString::getFrom(VArchiver& a)
{
    // Free the existing data, and set 'data' to 0
    // so that VArchiver::operator>> will reallocate it.
    setData(0);
    nEle = 0;
    asWCharFlag = FALSE;
    a >> data;
    nEle = size();
}

// ---------------------------------------------------------------------------
//
void VString::getFrom(VArchiver& a,
                      VObject * /* data */, boolean /* do_realize */ )
{
    getFrom(a);
}

#endif

// ---------------------------------------------------------------------------
// Load the contents of the resource string resID into this VString.
// Returns the number of byte elements in the string (or 0 for failure)
//
int VString::getResource(unsigned resID)
{
    int ret = 0;

#if defined(PM)
    char buffer[256];
    ret = WinLoadString(hab, 0, resID, 255, (PSZ) buffer);
    if (ret != 0) {
        puts(buffer);
    }
#elif defined(CV_WINDOWS) && !defined(CV_NO_NOTIFIER)
    char buffer[256];
    if ((ret = LoadString(notifier->inst(),
                          resID, (pointer)buffer, 255)) != 0) {
        puts(buffer);
    }
#elif defined(V_MACINTOSH)
    // not yet implemented
#else
    resID = 0;
#endif

    return ret;
}

#ifdef V_MACINTOSH
// ---------------------------------------------------------------------------
// Load the contents of the resource string name 'resName' into this
// VString.
//
void VString::getNamedResource(const char *resName)
{
    // not yet implemented
    puts("");
}

// ---------------------------------------------------------------------------
// Puts the string into the resource string named 'resName'.
//
void VString::putNamedResource(const char *resName)
{
    // not yet implemented
}
#endif // V_MACINTOSH

// ---------------------------------------------------------------------------
// Return TRUE if character c is in this VString, otherwise return FALSE.
//
boolean VString::charIn(char c) const
{
    pointer p = data;
    if (p == 0) {
        return FALSE;
    }
    if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
        while ((*p != 0) && (*p != c)) {
            p++;
        }
        return *p ? TRUE : FALSE;
    }
    else {
        VWideChar wc;
        atowc(wc, c);
        if (asWCharFlag) {
            return charIn(wc);
        }
        else {
            integer len = 0;
            VWideChar this_wchar;
            VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
            for (;;) {
                int ch_size = VWideCharAux::mbtowc(&this_wchar, p, CV_MB_LEN_MAX);
                if (ch_size <= 0) {
                    break;
                }
                if (this_wchar == wc) {
                    return TRUE;
                }
                p += ch_size;
            }
            return FALSE;
        }
    }
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Return TRUE if character c is in this VString, otherwise return FALSE.
//
boolean VString::charIn(VWideChar c) const
{
    if (data == 0) {
        return FALSE;
    }
    if (isAsByte()) {
        char *p = getData();
        char nc;
        if (!wctoa(nc, c)) {
            return FALSE; // c does not fit into a char.
        }
        while ((*p != 0) && (*p != nc)) {
            p++;
        }
        return *p ? TRUE : FALSE;
    }
    else {
        asWideString(TRUE);
        VWideChar *p = getWideData();
        while ((*p != 0) && (*p != c)) {
            p++;
        }
        return *p ? TRUE : FALSE;
    }
}
#endif
    
// ---------------------------------------------------------------------------
// Synonym for operator[].  The inline operator[] calls this for
// the hairy cases. It is up to the caller to ensure that
// 'i' is within the bounds of this string.
//
CV_GETS_CONST char& VString::charReferenceAt(integer i)
{
    if ((SINGLE_BYTE_CHAR_SET || asByteFlag) && !asWCharFlag) {
#if defined(CV_WIN16)
        return data[(int)i];
#else
        return data[i];
#endif
    }
    else {
        asWideString(FALSE);
        if (i < 0) {
            i = 0;
        }
        // Scan the string to find the position as byte offset.
        VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
        int ch_size;
        char *cp0 = getData();
        char *cp = cp0;
        for (integer pos = 0;; pos += 1, cp += ch_size) {
            if (pos == i) {
                i = cp - cp0;
                break;
            }
            ch_size = VWideCharAux::mbtowc(0, cp, CV_MB_LEN_MAX);
            if (ch_size <= 0) {
                i = cp - cp0;
                break;
            }
        }
        return data[i];
    }
}

// ---------------------------------------------------------------------------
// Return the character at index idx or return  -1 if idx is out of range.
// Also return -1 if the character won't fit into a single byte.
//
char VString::charAt(integer idx) const
{
    if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
        if (idx > size()) {
            return -1;
        }
        return gets()[idx];
    }
    asWideString(TRUE);
    if (idx > getCharSize()) {
        return -1;
    }
    VWideChar wc = getwcs()[idx];
    char nc;
    if (!wctoa(nc, wc)) {
        return '?'; // not possible to convert
    }
    return nc;
}

// ---------------------------------------------------------------------------
// Return the character at index idx or return  -1 if idx is out of range.
// Also return -1 if the character won't fit into a single byte.
//
VWideChar VString::wcharAt(integer idx) const
{
    if (isAsByte()) {
        if (idx > size()) {
            return (VWideChar) -1;
        }
        VWideChar wc;
        if (!atowc(wc, gets()[idx])) {
            return (VWideChar) -1;
        }
        return wc;
    }
    asWideString(TRUE);
    if (idx > getCharSize()) {
        return (VWideChar) -1;
    }
    return getwcs()[idx];
}

// ---------------------------------------------------------------------------
// This implementation performs a comparison of this VString
// with the VString obj using the C strcmp() function. Therefore,
// Return a value less than 0 if this VString is less than str;
// a value of 0 if this VString is equal to str; and a value greater than
// 0 if this VString is greater than str.
//
int VString::compare(VObject *obj) const
{
#if 1
    if (!obj->isA(VStringCls)) {
        return 1;
    }
    return compareVString(*(VString*)obj);
#else
    VString *sp = castObject(obj, VString);
    return (sp != 0) ? compareVString(*sp) : 1;
#endif
}

// ---------------------------------------------------------------------------
// This implementation performs a comparison of this VString
// with the VString obj using the C strcmp() function. Therefore,
// Return a value less than 0 if this VString is less than str;
// a value of 0 if this VString is equal to str; and a value greater than
// 0 if this VString is greater than str.
//
int VString::compareVString(const VString& obj) const
{
    const VString *str = &obj;
    if (asWCharFlag == str->asWCharFlag) {
        if (isAsByte() || str->isAsByte()) {
            return lstrcmp(gets(), str->gets());
        }
        else if (!asWCharFlag) {
            return VWideCharAux::strcoll(gets(), str->gets());
        }  
        const VWideChar *this_p;
        const VWideChar *that_p;
        VString this_s;
        VString that_s;
        if (CV_CALL_SINGLE_BYTE_GETS) {
            this_s = *this;
            that_s = *str;
            this_p = this_s.getwcs();
            that_p = that_s.getwcs();
        }
        else {
            this_p = getwcs();
            that_p = str->getwcs();
        }
        return VWideCharAux::wcscoll(this_p, that_p);
    }
    else if (isAsByte() || str->isAsByte()) {
        // One of the VStrings is in wide character format and the other
        // is considered just a string of bytes.  This is an odd kind
        // of comparison, but the bytes can be fetched as wide characters
        // and compared that way.
        for (int i = 0;; i += 1) {
            VWideChar this_c = wcharAt(i);
            VWideChar that_c = str->wcharAt(i);
            if (this_c > that_c) {
                return 1;
            }
            else if (this_c < that_c) {
                return -1;
            }
            else if (this_c == 0) {
                return 0;
            }
        }
    }
    else {
        // One of the VStrings is in wide character format and the other
        // is a multibyte string.  Mutate the one which is not in the
        // current preferred format, then compare.
        VString& this_string = *(VString *)this;
        VString& obj_string = *(VString *)str;
        boolean mutate_this_not_that = asWCharFlag != default_is_wchar;
        VString &s = mutate_this_not_that ? this_string : obj_string;
        s.asWideString(default_is_wchar);
        return this_string.compareVString(obj_string);
    }
}

// ---------------------------------------------------------------------------
// Return a value less than 0 if this VString is less than str;
// a value of 0 if this VString is equal to str; and a value greater than
// 0 if this VString is greater than str.
//
int VString::compareCStr(const char *str) const
{
    if (!asWCharFlag) {
        if (isAsByte()) {
            return lstrcmp((pointer)gets(), (pointer)str);
        } 
        else {
            return VWideCharAux::strcoll(gets(), (pointer)str);
        }
    }
    VString that = str;
    return compareVString(that);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Return a value less than 0 if this VString is less than str;
// a value of 0 if this VString is equal to str; and a value greater than
// 0 if this VString is greater than str.
//
int VString::compareCStr(const VWideChar *str) const
{
    if (asWCharFlag) {
        const VWideChar *this_str = getwcs();
        return VWideCharAux::wcscoll(this_str, str);
    }
    else {
        VString that = str;
        return compareVString(that);
    }
}
#endif

// ---------------------------------------------------------------------------
// Concatenate this VString onto the end of str and return
// the number of characters copied.
//
integer VString::concatTo(VString *str)
{
    return str->concat(this);
}

// ---------------------------------------------------------------------------
// Concatenate str onto the end of this VString and return
// the number of characters copied.
//
// Since this is the base class version of a virtual function, it should
// invoke the base class version of concat(const char *). 
//
integer VString::concat(VString *str)
{
    if (str->asWCharFlag && !CV_CALL_SINGLE_BYTE_GETS) { 
        return VString::concat(str->getwcs());
    }
    else {
        const char *strbytes = str->gets();
        integer sz = integer(VWideCharAux::mbslen(strbytes));
        VString::concat(str->gets());
        return sz;
    }
}

// ---------------------------------------------------------------------------
// Concatenate character c onto the end of this VString and return
// the number of bytes copied.
//
integer VString::concat(char c)
{
    char b[2];
    b[0] = c;
    b[1] = 0;
    return concat(b);
}

// ---------------------------------------------------------------------------
// Concatenate the 0 terminated C string str onto the end of this
// VString and return the number of bytes copied.
//
integer VString::concat(const char *str)
{
    asWideString(FALSE);

    integer n = strlen((char *)str);
    integer i = (data != 0) ? strlen(data) : 0;
    expandTo((i + n + 7) / 8 * 8); // Try to minimize thrashing.
    
    // ExpandTo() can fail if out of memory.  'data' will be NULL if so.  jrl 1/20/94
    if (data != 0) {
        strcpy(data + i, (char *)str);
    }
    return n;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Concatenate the 0 terminated C string str onto the end of this
// VString and return the number of characters copied.
//
integer VString::concat(const VWideChar *str)
{
    asWideString(TRUE);

    integer n = str != 0 ? VWideCharAux::wcslen(str) : 0;
    integer this_len = data != 0 ? VWideCharAux::wcslen(getWideData()) : 0;
    integer i = (data != 0) ? this_len : 0;
    expandTo((i + n + 7) / 8 * 8); // Try to minimize thrashing.
    
    // ExpandTo() can fail if out of memory.  'data' will be NULL if so.  jrl 1/20/94
    if (data != 0) {
        VWideCharAux::wcscpy(getWideData() + i, str);
    }
    return n;
}

// ---------------------------------------------------------------------------
// Concatenate character c onto the end of this VString and return
// the number of characters copied.
//
integer VString::concat(VWideChar c)
{
    VWideChar b[2];
    b[0] = c;
    b[1] = 0;
    if (CV_CALL_SINGLE_BYTE_GETS) {
        VString vs = b;
        return concat(vs.gets());
    }
    return concat(b);
}
#endif

// ---------------------------------------------------------------------------
// Return a pointer to the contents of this VString.  The contents
// should not be modified.
//
CV_GETS_CONST char *VString::gets() const
{
    asWideString(FALSE);
    return (data == 0) ? "" : data;
}

// ---------------------------------------------------------------------------
// Return a pointer to the contents of this VString.  The contents
// should not be modified.
//
const VWideChar *VString::getwcs() const
{
    asWideString(TRUE);
// #ifndef CV_OWN_VWIDECHAR
#if !defined(CV_OWN_VWIDECHAR) && !defined(CPPV_HPUX)
    return (data == 0) ? L"" : getWideData();
#else
    VWideChar ws[1];
    ws[0] = 0;
    return (data == 0) ? ws : getWideData();
#endif
}

#if 0
// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str up to len
// bytes.  A NULL character is added to the end of str.  Room for
// len + 1 bytes must be provided for in str.  Return the number
// of bytes copied.
//
// Overridden by VStream, etc. to read characters starting at the current
// position.
//
integer VString::gets(char *str, integer len)
{
    return ((const VString *)this)->VString::gets(str, len);
}
#endif

// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str up to len
// bytes.  A NULL character is added to the end of str.  Room for
// len + 1 bytes must be provided for in str.  Return the number
// of bytes copied.
//
// NOT virtual.
//
integer VString::gets(char *str, integer len) const
{
    return virtGets(str, len);
}

// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str up to len
// bytes.  A NULL character is added to the end of str.  Room for
// len + 1 bytes must be provided for in str.  Return the number
// of bytes copied.
//
// Virtual, but NOT overridden by VStream, etc.
//
integer VString::virtGets(char *str, integer len) const
{
    asWideString(FALSE);

    if (isAsByte() || data == 0) {
        pointer p;
        integer n = 0;
    
        if (data != 0) {
            for (p = getData(); *p && n < len; n++) {
                *str++ = *p++;
            }
        }
        *str = '\0';
        return n;
    }
    else {
        // Find the quantity of multibyte characters which can be moved
        // to the target storage in their entirety.
        const char *p = data;
        VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
        for (;;) {
            int ch_size = VWideCharAux::mbtowc(0, p, CV_MB_LEN_MAX);
            if (ch_size <= 0) {
                len = p - data;
                break;
            }
            p += ch_size;
            if (p - data > len) {
                len = (p - data) - ch_size;
                break;
            }
        }
        memcpy(str, data, (int)len);
        str[len] = '\0';
        return len; // the number of bytes copied
    }
}

#ifdef CV_WCHAR
#if 0
// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str up to len
// bytes.  A NULL character is added to the end of str.  Room for
// len + 1 bytes must be provided for in str.  Return the number
// of wide characters copied.
//
// This virtual function cannot be const because it is overridden in
// VStream with a version which modifies the stream's position.
//
integer VString::gets(VWideChar *str, integer len)
{
    return ((const VString *)this)->VString::gets(str, len);
}
#endif

// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str up to len
// bytes.  A NULL character is added to the end of str.  Room for
// len + 1 bytes must be provided for in str.  Return the number
// of wide characters copied.
//
// NOT virtual.
//
integer VString::gets(VWideChar *str, integer len) const
{
    return virtGets(str, len);
}

// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str up to len
// bytes.  A NULL character is added to the end of str.  Room for
// len + 1 bytes must be provided for in str.  Return the number
// of wide characters copied.
//
// Virtual, NOT overridden by VStream, etc.
//
integer VString::virtGets(VWideChar *str, integer len) const
{
    asWideString(TRUE);
    
    VWideChar *p;
    integer n = 0;

    if (data != 0) {
        for (p = getWideData(); *p != CV_WCHAR_CONST('\0') && n < len; n++) {
            *str++ = *p++;
        }
    }
    *str = CV_WCHAR_CONST('\0');
    return n;
}
#endif

// ---------------------------------------------------------------------------
// Copy the contents of this VString into the C string str and
// return it.  The character buffer pointer to by str must be large enough
// to receive all of the contents.
//
char *VString::gets(char *str) const
{
    asWideString(FALSE);
    strcpy(str, gets());
    return str;
}

// ---------------------------------------------------------------------------
// Copy the contents of this VString into str until c is reached,
// or until the end of this VString is reached.  A NULL character is appended
// to str.  Return the number of bytes copied.
//
integer VString::getTill(char *str, char c) const
{
    asWideString(FALSE);
    
    integer i = 0;
    if (isAsByte() || data == 0) {
        pointer    ptr;
        
        if ((ptr = getData()) != 0) {
            while (*ptr && *ptr != c) {
                *str++ = *ptr++;
                i++;
            }
        }
    }
    else {
        const char *p = data;
        VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
        for (;;) {
            int ch_size = VWideCharAux::mbtowc(0, p, CV_MB_LEN_MAX);
            if (ch_size <= 0) {
                break;
            }
            if (ch_size == 1 && *p == c) {
                break;
            }
            for (integer n = 0; n < ch_size; n += 1) {
                *str++ = *p++;
                i++;
            }
        }
    }
    *str = '\0';
    return i;
}

// ---------------------------------------------------------------------------
// Return the hash key for this VString computed from the characters
// it contains.
//
HashKey VString::hash() const
{
    if (default_is_wchar != asWCharFlag) {
        VString temp = *this;
        temp.asWideString(default_is_wchar);
        return temp.hash();
    }
    integer i;
    HashKey key = 0;
    if (asWCharFlag) {
        integer n = getCharSize();
        const VWideChar *p = getwcs();
        if (p == 0) {
            return 0;
        }
        for (i = 0; i < n; i += 2, p += 2) {
            key += (*p) & 0177;
        }
    }
    else {
        integer n = size();
        const char *p = gets();
        if (p == 0) {
            return 0;
        }
        for (i = 0; i < n; i += 2, p += 2) {
            key += (*p) & 0177;
        }
    }
    return key & HASH_KEY_MASK;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are the same as str.
//
boolean VString::isEqual(VObject *str) const
{
    if ((str == 0) || !str->isA(VStringCls)) {
        return FALSE;
    }
    return compareVString(*(VString *)str) == 0 ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Expand the contents of this VString to n elements plus a 0-termination.
// If this VString is in a wide character state, n gives the number of
// wide characters; otherwise it gives the number of bytes.
//
void VString::expandTo(integer n)
{
    integer term = asWCharFlag ? getCharSize() : size();
    size_t bytes_per_character = asWCharFlag ? sizeof(VWideChar) : 1;
    integer bytes_now = (nEle + 1) * bytes_per_character;
    integer bytes_active = (term + 1) * bytes_per_character;
    integer bytes_needed = (n + 1) * bytes_per_character;
    if (bytes_now < bytes_needed || data == 0) {
        char *newData = new char[bytes_needed];
        if (newData != 0) {
            if (term == 0L) {
                memset(newData, 0, bytes_per_character);  // terminate new empty string
            }
            else {
                memcpy(newData, data, bytes_active);
            }
        }
        if (data != 0) {
            delete [] data;
        }
        data = newData;
    }
    nEle = n;
}

// ---------------------------------------------------------------------------
// Copy the contents of VString str into this VString and return
// the number of characters copied.
//
integer VString::puts(VString *str)
{
    if (str->getData() == this->getData()) {
        //  setting data store to itself
        return getCharSize();
    }
    
    // Force this VString's use of wide characters to match str.
    if (asWCharFlag != str->asWCharFlag) {
        reset(); // avoid pointless translation
        asWideString(str->asWCharFlag);
    }

    integer n;
    if (asWCharFlag) {
        n = str->getCharSize();
        if (n > getCharSize()) {
            expandTo(n);
        }
        if (data != 0) {
            if (!CV_CALL_SINGLE_BYTE_GETS) {
                VWideCharAux::wcscpy(getWideData(), str->getwcs());
            }
            else {
                strcpy(getData(), str->gets());
            }
        }
    }
    else {
        n = str->size();
        if (n > size()) {
            expandTo(n);
        }
        if (data != 0) {
            strcpy(getData(), str->gets());
        }
        n = getCharSize();
    }
    return n;
}

// ---------------------------------------------------------------------------
// Copy the contents of the NULL terminated C string str into this
// VString and return the number of bytes copied.
//
integer VString::puts(const char *str)
{
    if (str == getData()) {
        //  setting data store to itself
        return size();
    }
    
    // Force this VString to not be wide.
    if (asWCharFlag) {
        reset(); // avoid pointless translation
        asWideString(FALSE);
    }

    if (!str) {
        reset();
        asWideString(FALSE);
        return 0;
    }
    integer n = strlen(str);
    expandTo(n);
    strcpy(getData(), str);
    return n;
}

// ---------------------------------------------------------------------------
// Copy the contents of the byte buffer str into this
// VString and return the number of bytes copied.
//
integer VString::write(const char *str, integer length)
{
    if (str == getData()) {
        //  Error: setting data store to itself
        return size();
    }
    
    // Force this VString to not be wide.
    if (asWCharFlag) {
        reset(); // avoid pointless translation
        asWideString(FALSE);
    }

    if (str == 0) {
        reset();
        asWideString(FALSE);
        return 0;
    }
    expandTo(length);
    if (data == 0) {
        return 0;
    }
    memcpy(data, str, length);
    getData()[length] = '\0';
    return length;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Copy the contents of the NULL terminated C string str into this
// VString and return the number of characters copied.
//
integer VString::puts(const VWideChar *str)
{
    if (str == getWideData()) {
        //  setting data store to itself
        return getCharSize();
    }
    
    // Force this VString to be wide.
    if (!asWCharFlag) {
        reset(); // avoid pointless translation
        asWideString(TRUE);
    }

    if (!str) {
        reset();
        asWideString(TRUE);
        return 0;
    }
    integer n = VWideCharAux::wcslen(str);
    expandTo(n);
    VWideCharAux::wcscpy(getWideData(), str);
    return n;
}

// ---------------------------------------------------------------------------
// Copy the contents of the character buffer str into this
// VString and return the number of characters copied.
//
integer VString::write(const VWideChar *str, integer length)
{
    if (str == getWideData()) {
        //  Error: setting data store to itself - punt
        return getCharSize();
    }
    
    // Force this VString to be wide.
    if (!asWCharFlag) {
        reset(); // avoid pointless translation
        asWideString(TRUE);
    }

    if (str == 0) {
        reset();
        asWideString(TRUE);
        return 0;
    }
    expandTo(length);
    if (data == 0) {
        return 0;
    }
    memcpy(data, str, length * sizeof(VWideChar));
    getWideData()[length] = 0;
    return length;
}
#endif

#ifdef V_MACINTOSH
// ---------------------------------------------------------------------------
// Write the contents of this VString into the resource string 'resID'.
//
void VString::putResource(unsigned resID)
{
    // not yet implemented
}
#endif // V_MACINTOSH

// ---------------------------------------------------------------------------
// Concatenate the result of this printf style format onto the end of
// this VString returning the number of bytes copied.  The number of
// characters can exceed MAX_STRING (see str.h).  
//
integer VString::concatPrints(const char *p...)
{
    char b[MAX_STRING + 1];
    VString s;
    va_list a;
    va_start(a, p);
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vsprintf(bufp, p, a);
        va_end(a);
        return concat(bufp);
    }
    return 0;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Concatenate the result of this printf style format onto the end of
// this VString returning the number of wide characters copied.  The number of
// characters can exceed MAX_STRING (see str.h).
//
integer VString::concatPrints(const VWideChar *p...)
{   
#ifdef WCHAR_H_EXISTS
    VWideChar b[MAX_STRING + 1];
    VString s;
    va_list a;
    va_start(a, p);
    VWideChar *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vswprintf(bufp, p, a);
        va_end(a);
        return concat(bufp);
    }
    return 0; 
#else      
    VString format_string(p);
    const char *narrow_p = format_string.gets();
    char b[MAX_STRING + 1];
    VString s;
    va_list a;
    va_start(a, p);
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, narrow_p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vsprintf(bufp, narrow_p, a);
        va_end(a);
        integer character_count = getCharSize();
        concat(bufp);
        asWideString(TRUE);
        return getCharSize() - character_count;
    }
    return 0;
#endif
}
#endif

// ---------------------------------------------------------------------------
// Copy the result of this printf style format into this VString,
// add line-termination, and return the number of bytes copied.  The
// number of characters can exceed MAX_STRING (see str.h).
//
integer VString::println(const char *p...)
{
    char b[MAX_STRING + 1];
    VString s;
    va_list a;
    va_start(a, p);
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vsprintf(bufp, p, a);
        va_end(a);
        return putln(bufp);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Note: integer VString::println(const VString& s)
// is not implemented even though it could help prevent some difficulty
// in the transition to internationalized C++/Views.  The variable length
// argument list prevents code reuse, so adding this would be costly.
// 

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Copy the result of this printf style format into this VString,
// add line-termination, and return the number of wide characters copied.  The
// number of characters can exceed MAX_STRING (see str.h).
//
integer VString::println(const VWideChar *p...)
{
#ifdef WCHAR_H_EXISTS
    VWideChar b[MAX_STRING + 1];
    VString s;
    va_list a;
    va_start(a, p);
    VWideChar *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vswprintf(bufp, p, a);
        va_end(a);
        return putln(bufp);
    }
    return 0;
#else
    VString format_string(p);  
    const char *narrow_p = format_string.gets();
    char b[MAX_STRING + 1];
    VString s;
    va_list a;
    va_start(a, p);
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, narrow_p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vsprintf(bufp, narrow_p, a);
        va_end(a);
        putln(bufp);
        asWideString(TRUE);
        return getCharSize();
    }
    return 0;
#endif
}
#endif

// ---------------------------------------------------------------------------
// Copy the result of this printf style format into this VString and
// return the number of bytes copied.  The number of characters can
// exceed MAX_STRING (see str.h).
//
integer VString::prints(const char *p...)
{
    char b[MAX_STRING + 1];
    va_list a;
    va_start(a, p);
    VString s;
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vsprintf(bufp, p, a);
        va_end(a);
        return puts(bufp);
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Note: integer VString::println(const VString& s)
// is not implemented even though it could help prevent some difficulty
// in the transition to internationalized C++/Views.  The variable length
// argument list prevents code reuse, so adding this would be costly.
// 

#if 0
// ---------------------------------------------------------------------------
// Copy the result of this printf style format into this VString and
// return the number of characters copied.  The number of characters can
// exceed MAX_STRING (see str.h).
//
integer VString::prints(const VString& str...)
{
#ifdef WCHAR_H_EXISTS
    if (str.asWCharFlag) {
        VWideChar b[MAX_STRING + 1];
        const VWideChar *p = str.getwcs();
        va_list a;
        va_start(a, p);
        VString s;
        VWideChar *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
        va_end(a);
        if (bufp != 0) {
            va_start(a, p);
            vswprintf(bufp, p, a);
            va_end(a);
            return puts(bufp);
        }
        return 0;
    }
#endif
    const char *narrow_p = str.gets();
    char b[MAX_STRING + 1];
    va_list a;
    va_start(a, narrow_p);
    VString s;
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, narrow_p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, narrow_p);
        vsprintf(bufp, narrow_p, a);
        va_end(a);
        integer sz = VWideCharAux::mbslen(bufp);
        return puts(bufp), sz;
    }
    return 0;
}
#endif

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Copy the result of this printf style format into this VString and
// return the number of characters copied.  The number of characters can
// exceed MAX_STRING (see str.h).
//
integer VString::prints(const VWideChar *p...)
{
#ifdef WCHAR_H_EXISTS
    VWideChar b[MAX_STRING + 1];
    va_list a;
    va_start(a, p);
    VString s;
    VWideChar *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vswprintf(bufp, p, a);
        va_end(a);
        return puts(bufp);
    }
    return 0;
#else
    VString format_string = p;
    const char *narrow_p = format_string.gets();
    char b[MAX_STRING + 1];
    va_list a;
    va_start(a, p);
    VString s;
    char *bufp = selectFormatBuffer(b, MAX_STRING + 1, &s, narrow_p, a);
    va_end(a);
    if (bufp != 0) {
        va_start(a, p);
        vsprintf(bufp, narrow_p, a);
        va_end(a);
        integer sz = VWideCharAux::mbslen(bufp);
        puts(bufp);
        asWideString(TRUE);
        return sz;
    }
    return 0;
#endif
}
#endif

// ---------------------------------------------------------------------------
// Set this VString to have length 0 and contain no characters.
//
void VString::reset()
{
    VString::init();
}

// ---------------------------------------------------------------------------
// Make the contents of VString o match the contents of this VString.
//
boolean VString::copyFrom(VObject *o)
{
    if (!o->isA(VStringCls)) {
        return FALSE;
    }
    VString::puts((VString *)o);
    return TRUE;
}

// ---------------------------------------------------------------------------
// Make the contents of this VString the same as the character c.
//
VString& VString::operator=(char c)
{
    char b[2];
    b[0] = c;
    b[1] = 0;
    reset();
    puts(b);
    return *this;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Make the contents of this VString the same as the character c.
//
VString& VString::operator=(VWideChar c)
{
    VWideChar b[2];
    b[0] = c;
    b[1] = 0;
    if (CV_CALL_SINGLE_BYTE_GETS) {
        VString vs = b;
        return operator=(vs.gets());
    }
    reset();
    puts(b);
    return *this;
}
#endif

// ---------------------------------------------------------------------------
//
void VString::init()
{
    setData(0);
    nEle = 0;
    asWCharFlag = default_is_wchar;
}

// ---------------------------------------------------------------------------
// Release the dynamic storage previously used by this VString.
// Set the pointer which this VString uses to represent its value, to 'p'.
//
void VString::setData(char *p)
{
    if (data != 0) {
        delete [] data;
    }
    data = p;
    nEle = 0;
    asWCharFlag = 0;
}

// ---------------------------------------------------------------------------
// Release the dynamic storage previously used by this VString.
// Set the pointer which this VString uses to represent its value, to 'p'.
//
void VString::setWideData(VWideChar *p)
{
    if (data != 0) {
        delete [] data;
    }
    data = (char *)p;
    nEle = 0;
    asWCharFlag = 1;
}

// ---------------------------------------------------------------------------
// Convert this VString to all lower case characters.
//
VString& VString::toLower()
{
    if (data != 0) {
        if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
            for (pointer p = getData(); *p != 0; p++) {
#ifndef __SC__
                *p = (VWideCharAux::tolower)(*p);
#else
# undef tolower
                *p = VWideCharAux::tolower(*p);
#endif
            }
        }
        else {
            asWideString(TRUE);
            for (VWideChar *p = getWideData(); *p != 0; p++) {
                *p = VWideCharAux::towlower(*p);
            }
        }
    }
    return *this;
}

// ---------------------------------------------------------------------------
// Convert this VString to all upper case characters.
//
VString& VString::toUpper()
{
    if (data != 0) {
        if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
            for (pointer p = getData(); *p != 0; p++) {
#ifndef __SC__
                *p = (VWideCharAux::toupper)(*p);
#else
                *p = VWideCharAux::toupper(*p);
#endif
            }
        }
        else {
            asWideString(TRUE);
            for (VWideChar *p = getWideData(); *p != 0; p++) {
                *p = VWideCharAux::towupper(*p);
            }
        }
    }
    return *this;
}

// ---------------------------------------------------------------------------
// Return a VString object that is the concatenation of this VString
// and VString s.
//
VString VString::operator+(const VString &s) const
{
    VString ns(*this);
    ns += s;
    return ns;
}

// ---------------------------------------------------------------------------
// Return a VString object that is the concatenation of this VString
// and character c.
//
VString VString::operator+(char c) const
{
    VString s(*this);
    s += c;
    return s;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Return a VString object that is the concatenation of this VString
// and character c.
//
VString VString::operator+(VWideChar c) const
{
    VString s(*this);
    s += c;
    return s;
}
#endif

// ---------------------------------------------------------------------------
// If asByte is TRUE, disable all interpretation of this VString as
// multibyte characters: each byte is assumed to represent 1 character.
// String comparison is by byte value, string size is in bytes, etc.
//
// If asByte is FALSE, reenable interpretation of this VString as
// multibyte characters.
//
void VString::asByteString(boolean asByte) const
{
    if (asByte != (boolean) asByteFlag) {
        ((VString *)this)->asByteFlag = asByte;
    }
    if (asByte) {
        asWideString(FALSE);
    }
}

// ---------------------------------------------------------------------------
// This is not a public function, as VString will convert back and forth
// between multibyte and wide representations as demanded by context.
//
// If asWide is TRUE, convert the internal representation of this VString
// to an array of wide characters.  If asWide is FALSE, and the internal
// representation uses wide characters, convert it back to multibyte
// characters.
//
// If the VString to be converted to wide characters has asByteFlag TRUE,
// nevertheless the array of char is interpreted as a multibyte string.
//
void VString::asWideString(boolean asWide) const
{
    if (asWide != (boolean) asWCharFlag) {
        VString &changeable = *(VString *)this;
        VString swapper;
        swapper.asWCharFlag = asWide;
		// !!! Here asByteFlag should probably be copied, so that semantics
		// are not changed for this VString if it happens to be forced to wide
		// representation and back.
        if (asWide) {
            // This VString is multibyte; swapper has wide characters.
            if (data != 0) {
                integer num_chars = getCharSize();
                swapper.expandTo(num_chars);
                VWideChar *swapper_data = swapper.getWideData();
                size_t n = VWideCharAux::mbstowcs(swapper_data, data, (size_t)num_chars);
                // Just in case, terminate the array with 0.
                if (n == (size_t)-1) {
                    swapper_data[0] = CV_WCHAR_CONST('\0');
                }
                else {
                    swapper_data[n] = CV_WCHAR_CONST('\0');
                }
                swapper_data[num_chars] = CV_WCHAR_CONST('\0');
            }
        }
        else if (data != 0) {
            // This VString has wide characters; swapper is multibyte.
            integer num_chars = VString::getCharSize(); // can't be virtual 3/24/95, or VFile::size gets confused.
            size_t mbchars_estimate = (size_t) num_chars;
            size_t n;
            for (;;) {
                swapper.expandTo(mbchars_estimate);
                memset(swapper.data, 1, mbchars_estimate + 1); // to detect completion
                n = VWideCharAux::wcstombs(swapper.data, getWideData(), mbchars_estimate + 1);
                if (n == (size_t)-1) {
                    swapper.data[0] = '\0';
                    break;
                }
                if (n <= mbchars_estimate && swapper.data[n] == '\0'){
                    break;
                }
                // Terminate with a 0 to ensure 'swapper' is in VString form.
                swapper.data[mbchars_estimate] = '\0';
                mbchars_estimate = mbchars_estimate * 2 + 1;
            } 
        }
        // Swap the data pointer of this VString with swapper.data.
        changeable.swapData(swapper);
    }   
}

// ---------------------------------------------------------------------------
//
void VString::swapData(VString &swapper)
{
    char *temp_data = data;
    unsigned long temp_nEle = nEle;
    unsigned short temp_wide = asWCharFlag;
    unsigned short temp_byte = asByteFlag;
    data = swapper.data;
    nEle = swapper.nEle;
    asWCharFlag = swapper.asWCharFlag;
    asByteFlag = swapper.asByteFlag;
    swapper.data = temp_data;
    swapper.nEle = temp_nEle;
    swapper.asWCharFlag = temp_wide;
}

// ---------------------------------------------------------------------------
//
const VWideChar *VString::getWideLineTerm()
{
    static VWideChar *line_term_p = 0;
    static VWideChar buffer[5];
    
    if (line_term_p == 0) {
        VString s = LINE_TERM;
        s.gets(buffer, 5);
        line_term_p = buffer;
    }
    return line_term_p;
}

// ---------------------------------------------------------------------------
// Analyze the given character string for a printf-style conversion sequence.
// Return the number of bytes in the conversion sequence, or 0 if the 
// string is not a recognizable conversion sequence.
//
// If a conversion sequence is recognized, sets 'c' to the conversion 
// specifier (normally the last character of the sequence (e.g. 's', 'd', 'p').
// Note that a '%' specifier does not correspond to an argument in the
// variable argument list, and that a 'n' specifier writes to the argument
// rather than converting it.
//
// The field width and precision are also returned.  If either is absent,
// -1 is returned.  If the either is '*', -2 is returned.
//
// The modifier is set to 0, 'h', 'l', or 'L'.
//
int VString::identifyFormatConversion(const char *s,
                                      char &c, 
                                      char &modifier,
                                      int &field_width, 
                                      int &precision)
{
    if (s == 0 || *s != '%') {
        return 0;
    }
    const char *p = s + 1;
    
    // Parse the flags (0 or more of -+ #0).
    for (;; p += 1) {
        boolean done = FALSE;
        
        switch (*p) {
        default:
            done = TRUE;
            break;
        case '-':
        case '+':
        case ' ':
        case '#':
        case '0':
            break;
        }
        
        if (done) {
            break;
        }
    }
    
    // Parse the optional field width, an integer or an asterisk.
    if (*p == '*') {
        p += 1;
        field_width = -2;
    }
    else if (!isdigit(*p)) {
        field_width = -1;
    }
    else {
        field_width = 0;
        while (isdigit(*p)) {
            field_width = field_width * 10 + (*p - '0');
            p += 1;
        }
    }
    
    // Parse the optional precision, a period followed by an integer.
    precision = -1;
    if (*p == '.') {
        precision = 0;
        p += 1;
        if (*p == '*') {
            p += 1;
            precision = -2;
        }
        else {
            while (isdigit(*p)) {
                precision = precision * 10 + (*p - '0');
                p += 1;
            }
        }
    }
    
    // Parse the conversion modifier, one of hlL.
    modifier = 0;
    if (*p == 'h' || *p == 'l' || *p == 'L') {
        modifier = *p;
        p += 1;
    }
    
    // Check the conversion specifier.
    switch(*p) {
    default:
        return 0; // unrecognized
    case 'd':
    case 'i':
    case 'o':
    case 'u':
    case 'x':
    case 'X':
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 'c':
    case 's':
    case 'p':
    case 'n':
    case '%':
        c = *p;
        p += 1;
    }
    
    // The conversion sequence is recognized, with c the specifier, and
    // p pointing to the character after it. 
    return p - s;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Wide character version of the above.
//
int VString::identifyFormatConversion(const VWideChar *s,
                                      VWideChar &c, 
                                      VWideChar &modifier,
                                      int &field_width, 
                                      int &precision)
{
    if (s == 0 || *s != CV_WCHAR_CONST('%')) {
        return 0;
    }
    const VWideChar *p = s + 1;
    
    // Parse the flags (0 or more of -+ #0).
    for (;; p += 1) {
        boolean done = FALSE;
        
        switch (*p) {
        default:
            done = TRUE;
            break;
        case CV_WCHAR_CONST('-'):
        case CV_WCHAR_CONST('+'):
        case CV_WCHAR_CONST(' '):
        case CV_WCHAR_CONST('#'):
        case CV_WCHAR_CONST('0'):
            break;
        }
        
        if (done) {
            break;
        }
    }
    
    // Parse the optional field width, an integer or an asterisk.
    if (*p == CV_WCHAR_CONST('*')) {
        p += 1;
        field_width = -2;
    }
#ifndef __SC__
    else if (!(VWideCharAux::iswdigit)(*p)) {
#else
    else if (!VWideCharAux::iswdigit(*p)) {
#endif
        field_width = -1;
    }
    else {
        field_width = 0;
#ifndef __SC__
        while ((VWideCharAux::iswdigit)(*p)) {
#else
        while (VWideCharAux::iswdigit(*p)) {
#endif
            field_width = field_width * 10 + (*p - CV_WCHAR_CONST('0'));
            p += 1;
        }
    }
    
    // Parse the optional precision, a period followed by an integer.
    precision = -1;
    if (*p == CV_WCHAR_CONST('.')) {
        precision = 0;
        p += 1;
        if (*p == CV_WCHAR_CONST('*')) {
            p += 1;
            precision = -2;
        }
        else {
            while (isdigit(*p)) {
                precision = precision * 10 + (*p - CV_WCHAR_CONST('0'));
                p += 1;
            }
        }
    }
    
    // Parse the conversion modifier, one of hlL.
    modifier = 0;
    if (*p == CV_WCHAR_CONST('h') || *p == CV_WCHAR_CONST('l') || *p == CV_WCHAR_CONST('L')) {
        modifier = *p;
        p += 1;
    }
    
    // Check the conversion specifier.
    switch(*p) {
    default:
        return 0; // unrecognized
    case CV_WCHAR_CONST('d'):
    case CV_WCHAR_CONST('i'):
    case CV_WCHAR_CONST('o'):
    case CV_WCHAR_CONST('u'):
    case CV_WCHAR_CONST('x'):
    case CV_WCHAR_CONST('X'):
    case CV_WCHAR_CONST('f'):
    case CV_WCHAR_CONST('e'):
    case CV_WCHAR_CONST('E'):
    case CV_WCHAR_CONST('g'):
    case CV_WCHAR_CONST('G'):
    case CV_WCHAR_CONST('c'):
    case CV_WCHAR_CONST('s'):
    case CV_WCHAR_CONST('p'):
    case CV_WCHAR_CONST('n'):
    case CV_WCHAR_CONST('%'):
        c = *p;
        p += 1;
    }
    
    // The conversion sequence is recognized, with c the specifier, and
    // p pointing to the character after it. 
    return p - s;
}
#endif

// ---------------------------------------------------------------------------
// Analyze the given printf-style formatting string for the given argument
// list.  Return a conservative figure for how much string space is required
// to do a vsprintf, measured in bytes.
//
// As this function invokes the va_arg macro, the value of 'arg' after
// the return is indeterminate.
//
long VString::measureFormat(const char *format, va_list arg)
{
    if (format == 0) {
        return 0;
    }
    int result = 0;
    short *sval;
    long *lval;
    int *val;
    while (*format != 0) {
        // Speedup: scan to '%'.
#ifdef __MWERKS__
        const char *percent = format;
        while (*percent != '%' && *percent != '\0') {
            percent += 1;
        }
        if (*percent != '%') {
            percent = 0;
        }
#else
        const char *percent = strchr(format, '%');
#endif
        if (percent == 0) {
            // no more %s
            return result + strlen(format);
        }
        result += (percent - format);
        format = percent;
        
        char specifier;
        char modifier;
        int field_width;
        int precision;
        int len = identifyFormatConversion(format, specifier, modifier, field_width, precision);
        if (len == 0) {
            result += 1;
            format += 1;
            continue;
        }
        if (field_width == -2) {
            // Read in value for '*' field width. 
            field_width = va_arg(arg, int);
            if (field_width < 0) {
                field_width = -1;
            }
        }
        if (precision == -2) {
            // Read in value for '*' precision.
            precision = va_arg(arg, int);
            if (precision < 0) {
                precision = -1;
            }
        }
        int allowance = 0;
        
        switch (specifier) {
        case 'd':
        case 'i':
            {
                if (modifier == 'l') {
                    long val = va_arg(arg, long);
                }
                else {
                    int val = va_arg(arg, int);
                }
                allowance = 10;
                if (precision > allowance) {
                    allowance = precision;
                }
                allowance += 1; // for sign
            }
            break;
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            {
                if (modifier == 'l') {
                    unsigned long val = va_arg(arg, unsigned long);
                }
                else {
                    unsigned val = va_arg(arg, unsigned);
                }
                allowance = 11;
                if (precision > allowance) {
                    allowance = precision;
                }
                allowance += 2; // possible '0x' prefix.
            }
            break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            {
                double val = (modifier == 'L')  ? (double)va_arg(arg, long double)
                                                : va_arg(arg, double);
                if (precision < 0) {
                    precision = 6;
                }
                int exp;
                frexp(val, &exp);
                exp = (exp < 0) ? 0 : exp / 3; // digits required left of decimal pt.
                if (specifier == 'e' && specifier == 'E') {
                    allowance = precision + 10;
                }
                else {
                    // 'f', 'g', or 'G' specifier
                    // This oversimplifies the 'g' & 'G' specifiers.
                    allowance = exp + precision + 4; // 'f' or 'g' like 'f'
                    if (specifier != 'f' && (precision + 10) > allowance) {
                        allowance = precision + 10; // 'g', like 'e' or 'E'
                    }
                }
            }
            break;
        case 'c':
            {
                int val = va_arg(arg, int);
                allowance = 1;
            }
            break;
        case 's':
            {
                char *cp = va_arg(arg, char *);
                allowance = strlen(cp);
                if (0 <= precision && precision < allowance) {
                    allowance = precision;
                }
            }
            break;
        case 'p':
            {
                void *val = va_arg(arg, void *);
                allowance = 10;
            }
            break;
        case 'n':
            {
                switch (modifier) {
                case 'h':
                    {
                        sval = va_arg(arg, short *); 
                    }
                    break;
                case 'l':
                    {
                        lval = va_arg(arg, long *); 
                    }
                    break;
                default:
                    {
                        val = va_arg(arg, int *);
                    }
                    break;
                }
                allowance = 0;
            }
            break;
        case '%':
            {
                allowance = 1;
            }
            break;
        }
        if (allowance < field_width) {
            allowance = field_width;
        }
        result += allowance;
        format += len;
    }
    return result;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Like the above, for wide characters.
//
long VString::measureFormat(const VWideChar *format, va_list arg)
{
    if (format == 0) {
        return 0;
    }
    int result = 0;
    short *sval;
    long *lval;
    int *val;
    while (*format != 0) {
        // Speedup: scan to '%'.
        const VWideChar *percent = VWideCharAux::wcschr(format, CV_WCHAR_CONST('%'));
        if (percent == 0) {
            // no more %s
            return result + VWideCharAux::wcslen(format);
        }
        result += (percent - format);
        format = percent;
        
        VWideChar specifier;
        VWideChar modifier;
        int field_width;
        int precision;
        int len = identifyFormatConversion(format, specifier, modifier, field_width, precision);
        if (len == 0) {
            result += 1;
            format += 1;
            continue;
        }
        if (field_width == -2) {
            // Read in value for '*' field width. 
            field_width = va_arg(arg, int);
            if (field_width < 0) {
                field_width = -1;
            }
        }
        if (precision == -2) {
            // Read in value for '*' precision.
            precision = va_arg(arg, int);
            if (precision < 0) {
                precision = -1;
            }
        }
        int allowance = 0;
        
        switch (specifier) {
        case CV_WCHAR_CONST('d'):
        case CV_WCHAR_CONST('i'):
            {
                if (modifier == CV_WCHAR_CONST('l')) {
                    long val = va_arg(arg, long);
                }
                else {
                    int val = va_arg(arg, int);
                }
                allowance = 10;
                if (precision > allowance) {
                    allowance = precision;
                }
                allowance += 1; // for sign
            }
            break;
        case CV_WCHAR_CONST('o'):
        case CV_WCHAR_CONST('u'):
        case CV_WCHAR_CONST('x'):
        case CV_WCHAR_CONST('X'):
            {
                if (modifier == CV_WCHAR_CONST('l')) {
                    unsigned long val = va_arg(arg, unsigned long);
                }
                else {
                    unsigned val = va_arg(arg, unsigned);
                }
                allowance = 11;
                if (precision > allowance) {
                    allowance = precision;
                }
                allowance += 2; // possible '0x' prefix.
            }
            break;
        case CV_WCHAR_CONST('f'):
        case CV_WCHAR_CONST('e'):
        case CV_WCHAR_CONST('E'):
        case CV_WCHAR_CONST('g'):
        case CV_WCHAR_CONST('G'):
            {
                double val = (modifier == CV_WCHAR_CONST('L'))  ? (double)va_arg(arg, long double)
                                                 : va_arg(arg, double);
                if (precision < 0) {
                    precision = 6;
                }
                int exp;
                frexp(val, &exp);
                exp = (exp < 0) ? 0 : exp / 3; // digits required left of decimal pt.
                if (specifier == 'e' && specifier == 'E') {
                    allowance = precision + 10;
                }
                else {
                    // 'f', 'g', or 'G' specifier
                    // This oversimplifies the 'g' & 'G' specifiers.
                    allowance = exp + precision + 4; // 'f' or 'g' like 'f'
                    if (specifier != 'f' && (precision + 10) > allowance) {
                        allowance = precision + 10; // 'g', like 'e' or 'E'
                    }
                }
            }
            break;
        case CV_WCHAR_CONST('c'):
            {
                int val = va_arg(arg, int);
                allowance = 1;
            }
            break;
        case CV_WCHAR_CONST('s'):
            {
                VWideChar *cp = va_arg(arg, VWideChar *);
                allowance = VWideCharAux::wcslen(cp);
                if (0 <= precision && precision < allowance) {
                    allowance = precision;
                }
            }
            break;
        case CV_WCHAR_CONST('p'):
            {
                void *val = va_arg(arg, void *);
                allowance = 10;
            }
            break;
        case CV_WCHAR_CONST('n'):
            {
                switch (modifier) {
                case CV_WCHAR_CONST('h'):
                    {
                        sval = va_arg(arg, short *); 
                    }
                    break;
                case CV_WCHAR_CONST('l'):
                    {
                        lval = va_arg(arg, long *); 
                    }
                    break;
                default:
                    {
                        val = va_arg(arg, int *);
                    }
                    break;
                }
                allowance = 0;
            }
            break;
        case '%':
            {
                allowance = 1;
            }
            break;
        }
        if (allowance < field_width) {
            allowance = field_width;
        }
        result += allowance;
        format += len;
    }
    return result;
}
#endif

// ---------------------------------------------------------------------------
// The caller offers 'buffer' and/or 'sbuffer' as storage for printf-style
// formatting.  Based on 'format' and 'arg', chooses the most appropriate
// and return a pointer into its buffer.  If 'sbuffer' is chosen, prepares
// it with enough memory to hold the formatted string.  Returns the chosen
// buffer, or a null pointer if neither is OK.
//
// As this function invokes the va_arg macro, the value of 'arg' after
// the return is indeterminate.
//
char *VString::selectFormatBuffer(char *buffer, long buffer_length,
                                  VString *sbuffer,
                                  const char *format, va_list arg)
{
    long formatted_length = measureFormat(format, arg);
    if (buffer != 0 && formatted_length + 1 <= buffer_length) {
        return buffer;
    }
    if (sbuffer != 0) {
        sbuffer->asWideString(FALSE);
        sbuffer->expandTo(formatted_length);
        return (char *)sbuffer->getData();
    }
    return 0;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// The caller offers 'buffer' and/or 'sbuffer' as storage for printf-style
// formatting.  Based on 'format' and 'arg', chooses the most appropriate
// and return a pointer into its buffer.  If 'sbuffer' is chosen, prepares
// it with enough memory to hold the formatted string.  Returns the chosen
// buffer, or a null pointer if neither is OK.
//
// As this function invokes the va_arg macro, the value of 'arg' after
// the return is indeterminate.
//
VWideChar *VString::selectFormatBuffer(VWideChar *buffer, long buffer_length,
                                     VString *sbuffer,
                                     const VWideChar *format, va_list arg)
{
    long formatted_length = measureFormat(format, arg);
    if (buffer != 0 && formatted_length + 1 <= buffer_length) {
        return buffer;
    }
    if (sbuffer != 0) {
        sbuffer->asWideString(TRUE);
        sbuffer->expandTo(formatted_length);
        return (VWideChar *)sbuffer->getWideData();
    }
    return 0;
}
#endif


#if defined(PM)
// ---------------------------------------------------------------------------
// PRIVATE PM : Scan the string replacing '&' with '~'
// PM uses ~ for marking accelerators
//
VString &VString::scanAmper(const char *s, boolean crlf)
{
	reset();
	if (s == NULL) {
		return *this;
	}

	while (*s) {
		if (*s == '&') {
			/*
         ** test for escape char
         */
			if ((*(s + 1) != '\0') && (*(s + 1) == '&')) {
				concat(*s++);
			}
			else {

				concat('~');
			}
		}
		else if (crlf && (*s == '\r') && (*(s + 1) == '\n')) {
			concat(*s++);
		}
		else {

			concat(*s);
		}
		s++;
	}
	return *this;
}
#endif


#if defined(CV_WINDOWS)
// ---------------------------------------------------------------------------
// PRIVATE Windows : Scan the string replacing '~' with '&'
// Windows uses & for marking accelerators
//
VString &VString::scanTilde(const char *s)
{
    reset();
    if (s == NULL) {
        return *this;
    }

    while (*s) {
        if (*s == '~') {
         // test for escape char
         if ((*(s + 1) != '\0') && (*(s + 1) == '~')) {
                concat(*s++);
            }
            else {

                concat('&');
            }
        }
        else {
            concat(*s);
        }
        s++;
    }
    return *this;
}
#endif

// ---------------------------------------------------------------------------
// Set the i-th character in this VString to c (zero-indexed).
// If the there are not more than i characters, does nothing.
//
void VString::setAt(integer i, char c)
{
    if (0 <= i && i < getCharSize()) {
        // The character is properly placed.
        if (FALSE) {
        }
#ifdef CV_WCHAR
        else if (asWCharFlag || !(asByteFlag  || SINGLE_BYTE_CHAR_SET)) {
            VWideChar wc;
            if (atowc(wc, c)) {
                setAt(i, wc);
            }
        }
#endif
        else {
            data[i] = c;
        }
    }
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Set the i-th character in this VString to c (zero-indexed).
// If the there are not more than i characters, does nothing.
//
void VString::setAt(integer i, VWideChar c)
{
    if (0 <= i && i < getCharSize()) {
        // The character is properly placed.
        if (isAsByte()) {
            char nc;
            if (wctoa(nc, c)) {
                setAt(i, nc);
            }
        }
        else {
            asWideString(TRUE);
            getWideData()[i] = c;
        }
    }   
}
#endif

// ---------------------------------------------------------------------------
// If this VString contains 'text', return the zero-based index of the
// first match, otherwise return -1.
//
integer VString::contains(const char *text, integer start_idx) const
{
    if (data == 0) {
    }
    else if (SINGLE_BYTE_CHAR_SET || isAsByte()) {
        asWideString(FALSE);
        if (start_idx == 0 || start_idx < getCharSize()) {
            unsigned int limit = strlen(text);
            integer sz = size();
            const char *str = gets();
            
            for (const char *p_str = str + start_idx;
                 ;
                 p_str += 1) {

                // Locate the next occurrence of text[0],
                // allowing for embedded null characters in VStreams.
                for (;;) {
                    const char *p2_str = strchr(p_str, text[0]);
                    if (p2_str != 0) {
                        p_str = p2_str;
                        break;
                    }
                    p_str = strchr(p_str, '\0');
                    if (p_str - str >= sz) {
                        return -1;
                    }
                    p_str += 1;
                }
            
                if (strncmp(p_str, text, limit) == 0) {
                    return (p_str - str);
                }
            }
        }
    }
    else {
        // Easier to search with character-oriented data.
        VString text_string = text;
        return VString::contains(text_string.getwcs(), start_idx);
    }
    return -1;
}

// ---------------------------------------------------------------------------
// If this VString contains 'text', return the zero-based index of the
// first match, otherwise return -1.
//
integer VString::contains(const VString& text, integer start_idx) const
{
    if (text.asWCharFlag) {
        return contains(text.getwcs(), start_idx);
    }
    else {
        return contains(text.gets(), start_idx);
    }
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// If this VString contains 'text', return the zero-based index of the
// first match, otherwise return -1.
//
integer VString::contains(const VWideChar *text, integer start_idx) const
{
    if (data != 0) {
        asWideString(TRUE);
        if (start_idx == 0 || start_idx < getCharSize()) {
            unsigned int limit = VWideCharAux::wcslen(text);
            integer sz = getCharSize();
            const VWideChar *str = getwcs();
    
            for (const VWideChar *p_str = str + start_idx;
                 ;
                 p_str ++) {
                                 
                // Locate the next occurrence of text[0],
                // allowing for embedded null characters in VStreams.
                for (;;) {
                    const VWideChar *p2_str = VWideCharAux::wcschr(p_str, text[0]);
                    if (p2_str != 0) {
                        p_str = p2_str;
                        break;
                    }
                    p_str = VWideCharAux::wcschr(p_str, CV_WCHAR_CONST('\0'));
                    if (p_str - str >= sz) {
                        return -1;
                    }
                    p_str += 1;
                }

            
                if (VWideCharAux::wcsncmp(p_str, text, limit) == 0) {
                    return (p_str - str);
                }
            }
        }
    }
    return -1;
}
#endif

// ---------------------------------------------------------------------------
// Truncate the string at zero-based offset 'idx'.  Returns TRUE if
// the string was changed by this operation.  No effect if 'idx' is
// past the end of the string.
//
boolean VString::truncAtCharPos(integer idx)
{
    if (idx < 0) {
        return FALSE;
    }
    if (idx < getCharSize()) {
        if (asWCharFlag) {
            getWideData()[idx] = CV_WCHAR_CONST('\0');
        }
        else if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
            data[idx] = '\0';       // data != NULL because size() > 0
        }
        else {
            // A multibyte string.
            VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
            int ch_size;
            // Scan and count the existing characters.
            char *cp;
            for (cp = data; idx > 0; idx -= 1) {
                ch_size = VWideCharAux::mbtowc(0, cp, CV_MB_LEN_MAX);
                if (ch_size <= 0) {
                    return FALSE;
                }
                cp += ch_size;
            }
            *cp = '\0';
        }
        return TRUE;
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// Guarantee the string is at least 'idx' characters long by appending
// 'padchar' until the specified size is reached.  No effect if
// the string is longer than 'idx' characters.  Returns TRUE if
// the string was changed.  'padchar' defaults to blanks.
//
boolean VString::padTo(integer idx, char padchar)
{
    register integer len = getCharSize();

    if (len < idx) {
        VWideChar w_padchar;
        if (!isAsByte() && !atowc(w_padchar, padchar)) {
            w_padchar = CV_WCHAR_CONST(' ');
        }
        if (asWCharFlag) {
            return padTo(idx, w_padchar);
        }
        
        size_t sz = data != 0 ? strlen(data) : 0;
        expandTo((idx - len) * (isAsByte() ? 1 : CV_MB_CUR_MAX) + sz);

        if (data != 0) {
            register char *cp;

            if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
                for (cp = data + sz; len < idx; len += 1) {
                    *cp++ = padchar;
                }
            }
            else {
                // A multibyte string.
                VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
                int ch_size;
                // Scan and count the existing characters.
                for (cp = data;;) {
                    ch_size = VWideCharAux::mbtowc(0, cp, CV_MB_LEN_MAX);
                    if (ch_size <= 0) {
                        break;
                    }
                    cp += ch_size;
                    sz -= ch_size;
                    idx -= 1;
                    len -= 1;
                }
                // Pad with the desired character.
                if (ch_size == 0 && sz == 0) {
                    for (; len < idx; len += 1) {
                        ch_size = VWideCharAux::wctomb(cp, w_padchar);
                        cp += ch_size;
                    }
                }
            }
            *cp = '\0';
        }
        return TRUE;
    }
    return FALSE;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Guarantee the string is at least 'idx' characters long by appending
// 'padchar' until the specified size is reached.  No effect if
// the string is longer than 'idx' characters.  Returns TRUE if
// the string was changed.  'padchar' defaults to blanks.
//
boolean VString::padTo(integer idx, VWideChar padchar)
{
    register integer len = getCharSize();

    if (len < idx) {
        if (!asWCharFlag) {
            char n_padchar;
            if (wctoa(n_padchar, padchar)) {
                return padTo(idx, n_padchar);
            }
        }
        
        expandTo(idx);

        if (data) {
            register VWideChar *cp;

            for (cp = getWideData() + len ; len < idx ; len += 1) {
                *cp++ = padchar;
            }
            *cp = CV_WCHAR_CONST('\0');
        }
        return TRUE;
    }
    return FALSE;
}
#endif

// ---------------------------------------------------------------------------
//
void VString::setCharSize(integer idx, char padchar)
{
    integer sz = getCharSize();

    if (sz < idx) {
        padTo(idx, padchar);        // a NOP if already larger than this
    }

    // in case allocation failed
    truncAtCharPos(idx);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//
void VString::setCharSize(integer idx, VWideChar padchar)
{
    integer sz = getCharSize();

    if (sz < idx) {
        padTo(idx, padchar);        // a NOP if already larger than this
    }

    // in case allocation failed
    truncAtCharPos(idx);
}
#endif

// ---------------------------------------------------------------------------
// Delete 'count' characters starting at offset 'idx' in the VString.
// Returns TRUE if something deleted.   If 'idx' is larger than the
// size of the string, no operation occurs.  If 'count' specifies more
// than the number of remaining characters, all remaining characters
// are deleted.
//
boolean VString::deleteAtCharPos(integer idx, integer count)
{   
    register integer sz = getCharSize();

    if (idx < 0) {
        // nonsensical parameter
    }
    else if (sz <= idx) {
        // beyond end of string; nothing to delete
    }
    else if (idx + count >= sz) {
        // Delete all remaining characters:
        // hand off the job to truncAtCharPos because it avoids mutating
        // a multibyte character string to wide characters.
        VString::truncAtCharPos(idx);
        return TRUE;
    }
    else {                  // if true, data can't be NULL
        if (count > 0) {
            sz -= (idx + count - 1);    // move '\0' too!
    
            if (sz != 0) {
                if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
                    register char *src = data + idx + count;
                    register char *dest = data + idx;
        
                    while (sz--) {
                        *dest++ = *src++;
                    }
                }
                else {
                    asWideString(TRUE);
                    
                    register VWideChar *src = getWideData() + idx + count;
                    register VWideChar *dest = getWideData() + idx;
        
                    while (sz--) {
                        *dest++ = *src++;
                    }
                    
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// Insert 'ins' into this VString.
// 'idx' is the index of the character the new string will precede.
// For instance, if 'idx' is 0 (the default) then 'ins' is inserted at the
// beginning of this VString.  If 'idx' is larger than the string 
// (e.g. (unsigned) -1), the 'ins' string is appended.
//
void VString::insertAtCharPos(const VString& ins, integer idx)
{
    // Set up this VString and a compatible insertion string.
    // If 'ins' can't be used as is, copy it into 'temp' and adjust
    // 'temp' to be compatible with this VString.
    VString temp;
    const VString *insp = &ins;
    if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
        if (!ins.asByteFlag) {
            temp = ins;
            temp.asWideString(FALSE);
            temp.asByteFlag = TRUE;
            insp = &temp;
        }
    }
    else {
        asWideString(TRUE);
        if (!ins.asWCharFlag) {
            temp = ins;
            temp.asWideString(TRUE);
            insp = &temp;
        }
    }
    const VString& insertion = *insp;
    
    integer sz = getCharSize();
    if (idx > sz) {
        idx = sz;                       // insert before this offset
    }
    integer inssize = insertion.getCharSize();
    if (inssize > 0 && idx >= 0) {
        asWideString(!isAsByte());          // Expansion must happen in the representation that will be used.
        expandTo(sz + inssize);             // expand target string

        // Slide the part of the string following 'idx'.
        integer to_index = sz + inssize;
        integer from_index = sz;
        for (;; to_index -= 1, from_index -= 1) {
            if (isAsByte()) {
                data[to_index] = data[from_index];
            }
            else {
                getWideData()[to_index] = getWideData()[from_index];
            }
            if (from_index == idx) {
                break;
            }
        }
        cv_assert(to_index == idx + inssize);
    
        // Insert the new string.
        integer ins_index = inssize;
        for (;;) {
            ins_index -= 1;
            to_index -= 1;
            if (isAsByte()) {
                data[to_index] = insertion.getData()[ins_index];
            }
            else {
                getWideData()[to_index] = insertion.getWideData()[ins_index];
            }
            if (ins_index == 0) {
                break;
            }
        }
        cv_assert(to_index == idx);
    }
}

// ---------------------------------------------------------------------------
// Insert 'ins' into this VString.
// 'idx' is the index of the character the new string will precede.
// For instance, if 'idx' is 0 (the default) then 'ins' is inserted at the
// beginning of this VString.  If 'idx' is larger than the string 
// (e.g. (unsigned) -1), the 'ins' string is appended.
// If 'inssize' is omitted, the entire 'ins' string is inserted.  Otherwise, 
// 'ins' is truncated or padded with 'pad' to 'newsize' before insertion.
// Inssize is measured in characters, normally multibyte.
//
void VString::insertAtCharPos(const char *ins, integer idx, integer inssize, char pad)
{
    VString i = ins;
    i.setCharSize(inssize, pad);
    insertAtCharPos(i, idx);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Insert 'ins' into this VString.
// 'idx' is the index of the character the new string will precede.
// For instance, if 'idx' is 0 (the default) then 'ins' is inserted at the
// beginning of this VString.  If 'idx' is larger than the string 
// (e.g. (unsigned) -1), the 'ins' string is appended.
// If 'inssize' is omitted, the entire 'ins' string is inserted.  Otherwise, 
// 'ins' is truncated or padded with 'pad' to 'newsize' before insertion.
// Inssize is measured in characters, normally multibyte.
//
void VString::insertAtCharPos(const VWideChar *ins, integer idx, integer inssize, VWideChar pad)
{
    VString i = ins;
    i.setCharSize(inssize, pad);
    insertAtCharPos(i, idx);
}
#endif

// ---------------------------------------------------------------------------
// Insert character 'c' into thisString as character at position 'idx'.
// Pad with character 'pad' if 'idx' is past the end of the string.
//
void VString::insertAtCharPos(const char c, integer idx, char pad)
{
    char a[2];
    a[0] = c;
    a[1] = '\0';
    insertAtCharPos(a, idx, 1, pad);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Insert character 'c' into thisString as character at position 'idx'.
// Pad with character 'pad' if 'idx' is past the end of the string.
//
void VString::insertAtCharPos(const VWideChar c, integer idx, VWideChar pad)
{
    VWideChar a[2];
    a[0] = c;
    a[1] = '\0';
    insertAtCharPos(a, idx, 1, pad);
}
#endif

// ---------------------------------------------------------------------------
// Replace character 'idx' in thisString with character 'c'.
// Pad with character 'pad' if 'idx' is past the end of the string.
//
void VString::replaceAtCharPos(const char c, integer idx, char pad)
{
    padTo(idx + 1, pad);        // pad with PAD as necessary
    setAt(idx, c);              // store character
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Replace character 'idx' in thisString with character 'c'.
// Pad with character 'pad' if 'idx' is past the end of the string.
//
void VString::replaceAtCharPos(const VWideChar c, integer idx, VWideChar pad)
{
    padTo(idx + 1, pad);        // pad with PAD as necessary
    setAt(idx, c);              // store character
}
#endif

// ---------------------------------------------------------------------------
// Strip leading and trailing whitespace.  Returns FALSE if the resulting
// string is empty.
//
boolean VString::strip()
{      
    stripTrailing();                    // do this first: less for next to move
    return stripLeading();
}

// ---------------------------------------------------------------------------
// Strip trailing whitespace.   Returns FALSE if resulting string empty.
//
boolean VString::stripTrailing()
{
    if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
        unsigned sz = getCharSize();
        register char *d = data + sz;       // OK if data is NIL
    
        integer found_index = -1;
        while (sz--) {                      // sz == 0 if data == NIL
            if (isspace(*--d)) {
                found_index = d - data;     // where to truncate string
            }
            else {
                break;
            }
        }
        if (found_index >= 0) {
            truncAtCharPos(found_index);
        }
        return (data && *data != 0);
    }
    else {
        asWideString(TRUE);
        integer sz = getCharSize();
        integer found_index = -1;
        for (;;) {
            sz -= 1;
            if (sz < 0) {
                break;
            }
            VWideChar wc = wcharAt(sz);
#ifndef __SC__
            if ((VWideCharAux::iswspace)(wc)) {
#else
            if (VWideCharAux::iswspace(wc)) {
#endif
                found_index = sz;
            }
            else {
                break;
            }
        }
        if (found_index >= 0) {
            truncAtCharPos(found_index);
        }
        return sz >= 0;
    }
}

// ---------------------------------------------------------------------------
// Strip leading whitespace.  Returns FALSE if resulting string is empty.
//
boolean VString::stripLeading()
{
    if (data) {
        integer found_non_space = -1;
        if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
            register char *d = data;        // -> first char
            register char *s = d;           // -> source
    
            while (*s && isspace(*s)) {
                ++s;
            }
            deleteAtCharPos(0, s - d);
            return (*data != 0);
        }
        else {
            asWideString(TRUE);
            integer idx = 0;
            VWideChar wc;
            for (;;) {
                wc = wcharAt(idx);
                if (wc == 0) {
                    break;
                }
#ifndef __SC__
                if (!(VWideCharAux::iswspace)(wc)) {
#else
                if (!VWideCharAux::iswspace(wc)) {
#endif
                    break;
                }
                idx += 1;
            }
            deleteAtCharPos(0, idx);
            return wc != 0;
        }
    }
    return FALSE;
}
