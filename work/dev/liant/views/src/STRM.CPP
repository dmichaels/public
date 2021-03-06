// $Header:   Q:/views/common/vcs/STRM.CPV   1.36   Mar 24 1997 16:13:30   scottt  $ 

//  strm.cpp
//
//  VStream class implementation [Common]
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
//  09/23/92 jmd    added va_ changes to scan() and format()
//  04/01/93 pmcm   added const to gets() per motif 2.1  source
//  04/01/93 pmcm   preened
//  04/01/93 pmcm   fixed puts(VString *) to return number of characters
//                  actually put, not the size of the string argument
//  04/07/93 pmcm   changed id to VObject *
//  07/03/93 pmcm   changed expandTo() to expand only if currently too small.
//  07/08/93 jmd    added tests for data = 0, fixed cast in breakAt
//  07/08/93 pkt    Copied change to copyFrom from up-to-date source code.
//  07/21/93 jmd    made puts and concat take a const char*
//                  removed free, added const to some members
//  07/31/93 pkt    eliminated #ifdef macintosh.
//  08/03/93 pkt    Fixed 07/08 fix.
//  08/11/93 jmd    fixed problem in getFrom (setting nEle at the wrong time)
//  08/11/93 pmcm   made atPut(), write() and putch() take const arg
//  08/23/93 pmcm   made helpers format(), scan(), struct fmtstr protected.
//  09/02/93 dgm    M8 changes; changed operator>>(_fmtstr&)
//                  to take (const _fmtstr&).
//  09/22/93 dgm    (pmcm) null check data in expandTo().
//  10/29/93 pkt    fixed return value of scanToMatching.
//  10/29/93 pkt    fixed gets(char *)const.
//  11/11/93 pmcm   added wsscanf fudge for M80 IN_DLL
//  11/12/93 pkt    removed self.
//  12/15/93 jmd    reset data when realloc fails
//  01/10/94 dgm    fix in VStream::at(int) to treat a negative argument as
//                  an offset from the end of the stream -- as documented.
//  01/13/94 dgm    merged and preened
//  01/19/94 pkt    preened and merged with Macintosh.
//  01/19/94 pkt    removed declarations of non-member helper functions.
//  01/30/94 dgm    simplified ++ expression in getch() & putch() to
//                  avoid apparent Borland C++ 4.0 compiler bug.
//                  simplified -- expression in gettrim() to
//                  avoid apparent Borland C++ 4.0 compiler bug.
//  02/24/94 dgm    old C preprocessor changes
//  04/04/94 pmcm   CV_WIN32: excluded wsscanf fudge for M80 IN_DLL
//  06/06/94 jld    removed VMemory::memory from expandTo()
//  07/11/94 dgm    fixed up assignment-operator and copy-constructor.
//  08/30/94 pkt    updated for VNOMALLOC version of VMemory.
//  09/08/94 dgm    Minor coding change to suppress MetaWare C++ warning.
//  11/02/94 jmd    added test for > 64K in expandTo() for 16bit case
//  02/10/94 pkt    updated for internationalization.
//  02/10/94 pkt    added support for currentEle and lengthEle.
//  02/15/95 pkt    added wide character versions of member functions.
//  03/07/95 pkt    added overloaded variants taking VString& arguments.
//  03/07/95 pkt    fixed 'write' to count characters correctly.
//  03/07/95 evc    got rid of redundent delete [] olddata in expandTo
//  03/08/95 pkt    improved 'atPut' to deal with invalid and partial
//                  multibyte characters.
//  03/08/95 pkt    improved 'getln' in case of multibyte strings.
//  03/09/95 evc    changed 'operator>>' to work around absence of 'swscanf'
//                  on some platforms
//  03/20/95 pkt    added stubs for overrides of VString manupulation
//                  functions.
//  03/24/95 pkt    implemented overrides of VString manipulation functions.
//  03/30/95 pkt    updated to use 'isAsByte' rather than 'asByteFlag'.
//  03/31/95 pkt    fixed 'read' to use the correct position.
//  03/31/95 pkt    fixed 'getLn' to handle 'eleCount' correctly.
//  04/05/95 pkt    fixed 'asWideString(TRUE)' to set 'currentEle' and
//                  'lengthEle'.
//  04/11/95 pkt    updated for VWideChar.
//  04/11/95 pkt    updated for VWideCharAux.
//  04/12/95 pkt    fixed so that 'current' may be -1 to indicate unknown.
//  04/12/95 pkt    worked around 'iswxxx' macro problem.
//  04/21/95 pkt    replaced MB_XXX with CV_MB_XXX.
//  05/19/95 pkt    merged for common source.
//  05/26/95 pkt    combined international changes into common version.
//  05/30/95 pkt    fixed 'getch(VWideChar&)' to initialize 'count'.
//  06/14/95 pkt    fixed merge problem in 'putln(const VWideChar *)'.
//  08/02/95 pkt    updated for VWideCharAux::swscanf.
//  08/16/95 pkt    revised 'asWideString' to preserve nEle and to honor
//                  embedded null characters.
//  08/16/95 pkt    fixed 'expandTo' not to affect asWCharFlag.
//  08/16/95 pkt    fixed a bug in 'write' tracking character position.
//  08/17/95 pkt    fixed bugs in 8/16 revised 'asWideString'.
//  08/21/95 pkt    fixed 'at(integer)' to set 'current' if isAsWide().
//  08/21/95 pkt    fixed 'ungetch(VWideChar)' to ignore LEOF.
//  09/06/95 pkt    removed OFI code.
//  09/06/95 pkt    updated for revised C++ standard for loops.
//  09/15/95 dss    removed asWideFlag as it is not defined anywhere.
//  09/14/95 pkt    improved comments and code to support more consistent
//                  measurement of size.
//  09/14/95 pkt    changed 'at(integer)' always to position the stream in
//                  terms of bytes in a multibyte representation.
//  09/19/95 pkt    restored cv_assert (removed 9/14), fixed to refer to
//                  asWCharFlag.
//  10/09/95 pkt    fixed 'deleteAtCharPos' stack overflow.
//  10/11/95 pkt    trimmed dead code from 'getch()'.
//  10/11/95 pkt    fixed 'scanToMatching' to handle multibyte characters.
//  11/29/95 tlf    updated for HPUX.
//  12/06/95 pkt    made 'operator<<(int)' non-inline.
//  12/12/95 pkt    added variants of 'puts'.
//  01/10/96 pkt    removed new 'puts' variants; redundant with 'write'.
//  01/10/96 pkt    improved expansion test in 'write'.
//  01/26/96 pkt    added 'roundUp' to alleviate allocation congestion from
//                  repeated appends to a stream.
//  03/28/96 pkt    added 'getPos', and 'setPos'.
//  04/08/96 glm    check for Symantec
//  04/11/96 pkt    fixed 'scanToMatching' bug introduced 10/11/95.
//	07/02/96 pkt	fixed 'asByteString' not to crash if mbtowc returns 0
//					at end of stream.
//	01/24/97 pkt	added 'VStream(const VString&)' constructor.
//	01/27/97 pkt	fixed 'VStream(const VString&)' constructor.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"

#include "strm.h"
#include "archiver.h"
#include "cvassert.h"

extern "C" {
#   include <string.h>
#   include <stdarg.h>
#   include <stdio.h>
#   include <ctype.h>
#   if _MSC_VER >= 800
#       if defined(IN_DLL) && !defined(CV_WIN32) 
#           include "wsscanf.h" //  because Win16 M80 does not supply for DLLs
#           define sscanf wsscanf
#       endif
#   endif
}

#define STREAMinitLength 50

// ---------------------------------------------------------------------------
//
defineClass(VStream, VString)

// ---------------------------------------------------------------------------
//  Construct an expandable VStream with length zero.
//
VStream::VStream()
{
    VStream::init();
    expandable(TRUE);
}

// ---------------------------------------------------------------------------
//  Construct an expandable VStream with length n.
//
VStream::VStream(integer n)
{
    VStream::init();
    expandable(TRUE);
    expandTo(n);
}

// ---------------------------------------------------------------------------
//  Construct a VStream from the VString s.
//
VStream::VStream(const VString& s)
{
    init();
	expandable(TRUE);
    copyFrom((VString *)(&s));
}

// ---------------------------------------------------------------------------
//  Construct a VStream from the VStream s.
//
VStream::VStream(const VStream& s)
{
    init();
    copyFrom((VStream *)(&s));
}

// ---------------------------------------------------------------------------
//
VStream& VStream::operator=(const VStream &s)
{
    if (&s != this) {
        reset();
        init();
        copyFrom((VStream *)(&s));
    }
    return *this;
}

// ---------------------------------------------------------------------------
//  Return TRUE if o is a VStream and this VStream has the same contents.
//
boolean VStream::isEqual(VObject *o) const
{ 
    if (!o || !o->isA(VStreamCls)) {
        return FALSE;
    }
    VStream *s = (VStream *)o;
    if (length != s->getCharSize()) {
        return FALSE;
    }
    if (asWCharFlag ? (getWideData() == 0) : (getData() == 0)) {
        return TRUE;
    }
    if (s->asWCharFlag ? (s->getWideData() == 0) : (s->getData() == 0)) {
        return TRUE;
    }
    integer i;
    if (isAsByte() || s->isAsByte() || SINGLE_BYTE_CHAR_SET) {
        pointer p = getData();
        pointer q = s->getData();
        for (i = 0; i < length; i++) {
            if (*p != *q) {
                break;
            }
        }
    }
    else {
        for (i = 0; i < length; i++) {
            if (wcharAt(i) != s->wcharAt(i)) {
                break;
            }
        }
    }
    return i == length;
}

// ---------------------------------------------------------------------------
//  Return a hash key computed from the contents of this VStream.
//
HashKey VStream::hash() const
{
    pointer p;
    integer i;
    HashKey key = 0;
    asWideString(FALSE);
    if ((p = getData()) == 0) {
        return 0;
    }
    for (i = 0; i < getLengthEle(); i++) {
        key = (key + (0177 & *p++)) & HASH_KEY_MASK;
    }
    return key;
}

// ---------------------------------------------------------------------------
//  Return TRUE if this VStream has no contents, otherwise return FALSE.
//
boolean VStream::empty()
{ 
    return lengthEle == 0;
}

// ---------------------------------------------------------------------------
//  Return TRUE if the current position is at the end of this VStream,
//  otherwise return FALSE.
//
boolean VStream::eof()
{ 
    return currentEle == lengthEle;
}

// ---------------------------------------------------------------------------
//  Return the current position of this VStream.
//
integer VStream::at()
{
    return getCurrentEle();
}

// ---------------------------------------------------------------------------
//  Return the current position of this VStream.
//
integer VStream::getCharPos() const
{
    integer c = getCurrent();
    if (c >= 0) {
        return c;
    }
    else {
        return fromElePosition(getCurrentEle(), 0);
    }
}

// ---------------------------------------------------------------------------
//  Return the current position of this VStream.
//  The method of measurement (e.g. byte count vs. character count)
//  is whatever is effective for the object.
//
integer VStream::getPos() const
{
    return getCharPos();
}

// ---------------------------------------------------------------------------
//  Reset the length and current position of this VStream to zero.
//
void VStream::reset()
{ 
    current = 0;
    currentEle = 0;
    byteOfCharacter = 0;
    bytesOfPrev = 0;
    length  = 0;
    lengthEle = 0;
}

// ---------------------------------------------------------------------------
//  Break this VStream at its current position and return a new
//  VStream with the residue.
//
VObject *VStream::breakAt()
{ 
    return breakAt(VStream::at()); 
}

// ---------------------------------------------------------------------------
//  Break this VStream at position n by first making a copy of it
//  then copying from position n to length into the new VStream. 
//  The original VStream is truncated at n.  The current position of
//  the original VStream is preserved if current < n, otherwise it is
//  set to length.  The current position of the new VStream is set
//  to zero if n < length , otherwise it is set to current - length.
//
VObject *VStream::breakAt(integer n)
{
    VStream *new_stream;
    integer i;

    //  construct a VStream with the same flags as this

    new_stream = (VStream *) getIsA()->construct();
    if (new_stream == 0) {
        return 0;       //jmd 7/8/93
    }
    new_stream->flags = flags;

    if (n >= lengthEle) {
        truncAt(lengthEle);
        new_stream->truncAt(0);
        return new_stream;
    }

    //  Copy the end of this stream to the new stream.
    //  Discard the end of this stream.
    if (!asWCharFlag) {
        pointer q, p;
        new_stream->expandTo(lengthEle - n + 1);
        p = getData();
        q = new_stream->getData();
        p += n;
        new_stream->current = 0;
        new_stream->currentEle = 0;
        new_stream->byteOfCharacter = 0;
        new_stream->bytesOfPrev = 0;
        new_stream->lengthEle = lengthEle - n;
        for (i = 0; i < new_stream->getLengthEle(); i++) {
            *q++ = *p++;
        }
        new_stream->length = new_stream->fromElePosition(new_stream->lengthEle, 0);
    }
    else {
        VWideChar *q;
        VWideChar *p;
        asWideString(TRUE);
        new_stream->asWideString(TRUE);
        new_stream->expandTo(length - n + 1);
        p = getWideData();
        q = new_stream->getWideData();
        p += n;
        new_stream->current = 0;
        new_stream->currentEle = 0;
        new_stream->byteOfCharacter = 0;
        new_stream->bytesOfPrev = 0;
        new_stream->length = length - n;
        new_stream->lengthEle = lengthEle - n;
        for (i = 0; i < new_stream->length; i++) {
            *q++ = *p++;
        }
    }

    //  finally, adjust the current position in both streams

    if (currentEle > n) {
        new_stream->currentEle = currentEle - n;
        new_stream->current = new_stream->fromElePosition(new_stream->currentEle,
                                                          &new_stream->byteOfCharacter);
        current = n;
        currentEle = toElePosition(current);
        byteOfCharacter = 0;
        bytesOfPrev = 0;
    }
    else {
        new_stream->current = 0;
        new_stream->currentEle = 0;
        new_stream->byteOfCharacter = 0;
    }
    new_stream->bytesOfPrev = 0;
    truncAt(n);

    return new_stream;
}

// ---------------------------------------------------------------------------
//  Truncate this VStream at its current position.
//
boolean VStream::truncAt()
{ 
    return truncAt(VStream::at());
}

// ---------------------------------------------------------------------------
//  Truncate this VStream at position n.  If the current
//  position is greater than n, then set it to n.
//
boolean VStream::truncAt(integer n)
{
    lengthEle = n;
    short boc;
    length = fromElePosition(lengthEle, &boc);
    if (currentEle > lengthEle) {
        current = length;
        currentEle = lengthEle;
        byteOfCharacter = boc;
        bytesOfPrev = 0;
    }
    expandTo(getLengthEle());
    return TRUE;
}

// ---------------------------------------------------------------------------
// Expand the contents of this VStream to n elements plus a 0-termination.
// If this VStream is in a wide character state, n gives the number of
// wide characters; otherwise it gives the number of bytes.
//
void VStream::expandTo(integer n)
{
    if (nEle < n) {
        integer element_size = asWCharFlag ? sizeof(VWideChar) : 1;
#ifdef CV_WIN16
        // 16bit OS
        if (element_size * n > integer(0xFFF0)) {
            n = integer(0xFFF0) / element_size;
            // stream cannot be expanded further!
            expandable(FALSE);
        }
#endif
        char *old_data = asWCharFlag ? (char *)getWideData() : getData();
        char *new_data = new char [(n + 1) * element_size];
        
        if (new_data != 0) {
            register integer i;
            if (asWCharFlag) {
                VWideChar *new_wide_data = (VWideChar *)new_data;
                VWideChar *old_wide_data = (VWideChar *)old_data;
                for (i = 0; i < nEle; i++) {
                    new_wide_data[i] = old_wide_data[i];
                }
                for (; i < n; i++) {
                    new_wide_data[i] = 0;
                }                   
            }
            else {
                for (i = 0; i < nEle; i++) {
                    new_data[i] = old_data[i];
                }
                for (; i < n; i++) {
                    new_data[i] = 0;
                }
            }
        }
        if (asWCharFlag) {
            VString::setWideData((VWideChar *)new_data);
        }
        else {
            VString::setData(new_data);
        }
        nEle = (new_data != 0) ? n : 0;
    }

    if (asWCharFlag ? (getWideData() == 0) : (getData() == 0)) {
        reset();
    }
    else if (asWCharFlag) {
        getWideData()[n] = 0;
    }
    else {
        getData()[n] = 0;
    }
}

// ---------------------------------------------------------------------------
// Expand this VStream by 50% of its current size
// or by 10K chunks if it is bigger than 30K.
//
void VStream::expand()
{ 
    integer n = nEle;
    if (!expandable()) {
        return;
    }
    if (n < 2) {
        n = STREAMinitLength;
    }
    else if (n > 32000) {
        n += 10000;
    }
    else {
        n += (n / 2);
    }
    expandTo(n);
}

// ---------------------------------------------------------------------------
//  Return the character at the next position past the current position
//  of this VStream.  Return a -1 if the next position is at the end of
//  this VStream.  The current position is left unaltered.
//
int VStream::peekch()
{
    int c = getch();
    ungetch((char)c);
    return c;
}

// ---------------------------------------------------------------------------
//  Set c to the wide character at the next position past the current position
//  of this VStream.  Return a FALSE if the next position is at the end of
//  this VStream.  The current position is left unaltered.
//
boolean VStream::peekch(VWideChar &c)
{
    boolean ok = getch(c);
    if (ok) {
        ungetch(c);
    }
    return ok;
}

// ---------------------------------------------------------------------------
//  Return the byte at the next position past the current element position
//  of this VStream, or return a -1 if the end has been reached.  
//  The current element position is advanced by one.
//  
//  Warning:  Calling this function in an internationalized environment
//  can leave the VStream in positioned in the middle of a character.
//  A number of important stream-based operations will yield undefined
//  results if they are invoked in such a state.  Either make sure that
//  you call getch enough times to form a complete multibyte character,
//  or chall getch(VWideChar &).
//
int VStream::getch()
{
    if (currentEle == lengthEle) {
        //  end of stream already reached
        return -1;
    }
    // Just grab one byte.
    pointer p = getData();
    char c = p[getCurrentEle()];
    currentEle++;
    if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
        if (current >= 0) {
            current++;
        }
        bytesOfPrev = 1;
    }
    else {
        byteOfCharacter += 1; // valid only if still within a character
        int ch_size = VWideCharAux::mbtowc(0, p + currentEle - byteOfCharacter, byteOfCharacter);
        // Process null characters as valid.
        ch_size = (ch_size == 0) ? 1 : ch_size;
        if (ch_size >= 0) {
            // A character is complete.
            if (current >= 0) {
                current++;
            }
            bytesOfPrev = byteOfCharacter;
            byteOfCharacter = 0;
        }
    }
    // Return bytes as positive values.
    return int(c) & 0377;   //  valid character values are 0 to 255, (0x00 to 0xFF)
}

// ---------------------------------------------------------------------------
//  Set wc to the wide character at the next position past the current element 
//  position of this VStream, returning FALSE if the end has been reached.  
//  The current character position is advanced by one.
//
boolean VStream::getch(VWideChar &wc)
{
    if (currentEle == lengthEle) {
        //  end of stream already reached
        return FALSE;
    }
    if (CV_CALL_SINGLE_BYTE_GETS) {
        // Call single byte version of getch to support derived classes
        // which override only that virtual function.
        char mbc[CV_MB_LEN_MAX];
        int count;
        for (count = 0;; count += 1) {
            if (count >= CV_MB_CUR_MAX) {
                break;
            }
            int c = getch();
            // 'getch()' updates currentEle and current.
            if (c == -1) {
                return FALSE;
            }
            if (isAsByte()) {
                if (!atowc(wc, c)) {
                    wc = CV_WCHAR_CONST('?');
                }
                break;
            }
            mbc[count] = (char)c;
            int ch_size = VWideCharAux::mbtowc(&wc, mbc, count + 1);
            if (ch_size == count + 1) {
                // The wide character is complete.
                break;
            }
        }
        if (count >= CV_MB_CUR_MAX) {
            wc = CV_WCHAR_CONST('?');
        }
    }
    else if ((isAsByte() || SINGLE_BYTE_CHAR_SET) && !asWCharFlag) {
        pointer p = getData();
        char c = p[getCurrentEle()];
        if (current >= 0) {
            current++;
        }
        currentEle++;
        byteOfCharacter = 0;
        bytesOfPrev = 1;
        if (!atowc(wc, c)) {
            wc = CV_WCHAR_CONST('?');
        }
    }
    else if (!CV_MB_HAS_STATE && !asWCharFlag) {
        pointer p = getData();
        int ch_size = VWideCharAux::mbtowc(&wc, p + getCurrentEle(), CV_MB_LEN_MAX);
        if (ch_size < 0) {
            wc = CV_WCHAR_CONST('?');
        }
        if (current >= 0) {
            current++;
        }
        currentEle += ch_size;
        byteOfCharacter = 0;
        bytesOfPrev = ch_size;
    }
    else {
        VWideChar *p;
        p = getWideData();
        wc = p[currentEle];
        current += 1;
        currentEle += 1;
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
//  Put c into the current position of this VStream, advance the
//  current position and return TRUE.  If the current postion is the end
//  and this VStream is not expandable, then return FALSE.
//
boolean VStream::putch(const char c)
{
    pointer p;
    if (getCurrentEle() == nEle) {
        if (expandable()) {
            expand();
        }
        else {
            return FALSE;
        }
    }
    p = getData();
    p[getCurrentEle()] = c;
    currentEle++;
    getCurrent(); // because we need 'current', to keep 'length' up to date,
                  // and we don't do any checking for 'length' being unknown
    if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
        if (current >= 0) {
            current++;
        }
        bytesOfPrev = 1;
        byteOfCharacter = 0;
    }
    else {
        byteOfCharacter += 1; // valid only if still within a character
        int ch_size = VWideCharAux::mbtowc(0, p + currentEle - byteOfCharacter, byteOfCharacter);
        // Process null characters as valid.
        ch_size = (ch_size == 0) ? 1 : ch_size;
        if (ch_size >= 0) {
            // A character is complete.
            if (current >= 0) {
                current++;
            }
            bytesOfPrev = byteOfCharacter;
            byteOfCharacter = 0;
        }
    }
    if (current > length) {
        length = current;
    }
    if (currentEle > lengthEle) {
        lengthEle = currentEle;
    }
    return TRUE;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Put c into the current position of this VStream, advance the
//  current position and return TRUE.  If the current postion is the end
//  and this VStream is not expandable, then return FALSE.
//
boolean VStream::putch(const VWideChar c)
{
    if (getCurrentEle() + CV_MB_LEN_MAX > nEle) {
        if (expandable()) {
            expand();
        }
        else {
            return FALSE;
        }
    }
    if (CV_CALL_SINGLE_BYTE_GETS) {
        if (isAsByte()) {
            char one_byte;
            if (!wctoa(one_byte, c)) {
                one_byte = '?';
            }
            putch(one_byte);
        }
        else {
            char mbc[CV_MB_LEN_MAX];
            int nchars = VWideCharAux::wctomb(mbc, c);
            if (nchars < 0) {
                mbc[0] = '?';
                nchars = 1;
            }
            for (int i = 0; i < nchars; i += 1) {
                putch(mbc[i]);
            }
        }
    }
    else if (!asWCharFlag) {
        pointer p = getData();
        if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
            char ch;
            if (!wctoa(ch, c)) {
                ch = '?';
            }
            p[getCurrentEle()] = ch;
            currentEle++;
            if (current >= 0) {
                current++;
            }
            bytesOfPrev = 1;
            byteOfCharacter = 0;
        }
        else {
            // back up if within a multibyte character
            if (byteOfCharacter > 0) {
                currentEle -= byteOfCharacter;
                byteOfCharacter = 0;
            }

            int nchars = VWideCharAux::wctomb(p + getCurrentEle(), c);
            // Process null characters as valid.
            nchars = (nchars == 0) ? 1 : nchars;
            if (nchars < 0) {
                p[getCurrentEle()] = '?';
                nchars = 1;
            }
            // A character is complete.
            currentEle += nchars;
            if (current >= 0) {
                current++;
            }
            bytesOfPrev = byteOfCharacter;
            byteOfCharacter = 0;
        }
    }
    else {
        VWideChar *p = getWideData();
        p[getCurrentEle()] = c;
        current++;
        currentEle++;
    }
    if (current > length) {
        length = current;
    }
    if (currentEle > lengthEle) {
        lengthEle = currentEle;
    }
    return TRUE;
}
#endif

// ---------------------------------------------------------------------------
//  Copy up to n bytes of this VStream into p starting at
//  byte position i.  Return the actual number of elements (bytes) copied.
//
integer VStream::atGet(integer i, char *p, integer n)
{
    integer k;
    asWideString(FALSE);
    eleCount = 0;
    charCount = 0;
    if (i > lengthEle) {
        // integer n_characters = fromElePosition(nEle, 0);
        if (i >= nEle) {
            return getCount();
        }
    }
    if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
        if (i + n > nEle) {
            n = nEle - i;
        }
        pointer q = getData();
        if (q == 0) {
            return 0;
        }
        for (k = n, q += i; k > 0; k--) {
            *p++ = *q++;
        }
        eleCount = n;
        charCount = n;
    }
    else {
        // multibyte characters:
        // Note: This case doesn't work sensibly if multibyte characters
        // entail state.  But in case of state, 'atGet' itself is problematic,
        // because it's moving multibyte characters into a context with
        // unknown state.
        VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
        char *q0 = getData();
        char *q = q0;
        short byte_of_character = 0;
        if (i == currentEle) {
            q += i;
            byte_of_character = byteOfCharacter;
        }
        else if (FALSE) {
            integer pos = 0;
            // Scan the string to find the position as byte offset.
            int ch_size = 1;
            if (q0 == 0) {
                return 0;
            }
            for (; pos < i; pos += 1, q += ch_size) {
                ch_size = VWideCharAux::mbtowc(0, q, CV_MB_LEN_MAX);
                if (ch_size == 0) {
                    ch_size += 1;
                }
                if (ch_size <= 0) {
                    return 0;
                }
            }
        }
        else {
            q += i;
            byte_of_character = 0; // This assumption could be wrong.
        }
        char *q1 = q;
        char *q_end = q + n;
        q -= byteOfCharacter;
        int ch_size;
        for (; q < q_end; charCount += 1, q += ch_size) {
            ch_size = VWideCharAux::mbtowc(0, q, q_end - q);
            if (ch_size == 0) {
                // Pass null characters through.
                ch_size = 1;
            }
            else if (ch_size < 0) {
                if (q_end - q < CV_MB_CUR_MAX) {
                    // It appears that we have been asked to transfer an
                    // incomplete multibyte character.
                    break;
                }
                else {
                    // An invalid character is being transferred.
                    // Impossible to count true multibyte characters, but it's
                    // less misleading to count such a thing as a one byte character
                    // than as no characters at all.
                    ch_size = 1;
                }
            }
        }
        // Copy the range q1 -> q to the buffer pointed to by p.
        eleCount = q - q1;
        if (eleCount > 0) {
            cv_assert(eleCount <= n);
            memcpy(p, q1, eleCount);
        }
    }
    return getCount();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Copy up to n wide characters of this VStream into p starting at
//  wide character position i.  Return the actual number of characters copied.
//
integer VStream::atGet(integer i, VWideChar *p, integer n)
{
    if (CV_CALL_SINGLE_BYTE_GETS) {
        // It would be nice to call one of the single byte
        // virtual functions here, but too complicated.
        // The programmer who derives from VStream must
        // override this version of 'atGet'.
    }
    asWideString(TRUE);
    integer k;
    eleCount = 0;
    charCount = 0;
    if (i >= nEle) {
        return getCount();
    }
    if (i + n > nEle) {
        n = nEle - i;
    }
    VWideChar *q = getWideData();
    if (q == 0) {
        return 0;
    }
    for (k = n, q += i; k > 0; k--) {
        *p++ = *q++;
    }
    eleCount = n;
    charCount = n;
    return getCount();
}
#endif

// ---------------------------------------------------------------------------
// Round up an allocation size, so that little expansions don't lead to
// too many memory reallocations.
//
static integer roundUp(integer n)
{
    short shift_digits = 0;
    while (n > 8) {
        n = (n + (n & 1)) >> 1;
        shift_digits += 1;
    }
    return n << shift_digits;
}

// ---------------------------------------------------------------------------
//  Copy up to n bytes from p into this VStream starting at
//  byte position i.  If this VStream is expandable, then expand it to 
//  fit the n bytes.  Return the actual number of elements (bytes) copied.
//
//  If this stream is using a multibyte representation, then an invalid
//  or partial multibyte character will not be copied at the end.
//
integer VStream::atPut(integer i, const char *p, integer n)
{
    if (p == 0 || getData() == 0) {
        eleCount = 0;
        charCount = 0;
        return getCount();
    }
    asWideString(FALSE);
    integer iEle = i;
    if (FALSE && !(isAsByte() || SINGLE_BYTE_CHAR_SET)) {
        // Note: this code left over from when 'i' was measured in characters.
        // multibyte characters
        if (i == getCurrent()) {
            iEle = currentEle;
        }
        else if (i == length) {
            iEle = lengthEle;
        }
        else {
            iEle = toElePosition(i);
        }
    }
    if (iEle + n > nEle) {
        if (expandable()) {
            expandTo(roundUp(iEle + n));
        }
        else {
            n = nEle - iEle;
        }
    }
    pointer q = getData() + iEle;
    if (isAsByte() || SINGLE_BYTE_CHAR_SET) {
        for (integer k = n; k > 0; k--) {
            *q++ = *p++;
        }
        eleCount = n;
        charCount = n;
        return getCount();
    }
    else {
        cv_assert(VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX) == 0); // assert no state
        // The following works only if no state.
        eleCount = 0;
        charCount = 0;
        while (eleCount < n) {
            int ch_size = VWideCharAux::mbtowc(0, p + eleCount, n - eleCount);
            ch_size = (ch_size == 0) ? 1 : ch_size;
            if (ch_size <= 0) {
                if (ch_size == 0) {
                    // Handle null bytes as characters.
                }
                else if (eleCount == 0) {
                    // Gotta move SOMEthing.
                }
                else if (n - eleCount < CV_MB_CUR_MAX) {
                    // Assume it's an incomplete character.
                    break;
                }
                ch_size = 1;
            }
            eleCount += ch_size;
            charCount += 1;
        }
        memmove(q, p, eleCount);
        return getCount();
    }
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Copy up to n wide characters from p into this VStream starting at wide
//  character position i.  If this VStream is expandable, then expand it to 
//  fit the n wide characters.  Return the actual number of characters copied.
//
integer VStream::atPut(integer i, const VWideChar *p, integer n)
{
    if (CV_CALL_SINGLE_BYTE_GETS) {
        // It would be nice to call one of the single byte
        // virtual functions here, but too complicated.
        // The programmer who derives from VStream must
        // override this version of 'atPut'.
    }
    if (p == 0 || getWideData() == 0) {
        eleCount = 0;
        charCount = 0;
        return 0;
    }
    asWideString(TRUE);
    if (i + n > nEle) {
        if (expandable()) {
            expandTo(roundUp(i + n));
        }
        else {
            n = nEle - i;
        }
    }
    VWideChar *q = getWideData() + i;
    for (integer k = n; k > 0; k--) {
        *q++ = *p++;
    }
    eleCount = n;
    charCount = n;
    return n;
}
#endif

// ---------------------------------------------------------------------------
//  Put c back into this VStream and decrement the current position.
//
void VStream::ungetch(char c)
{
    if (getCurrentEle() == 0 || c == EOF) {
        return;
    }
    if (byteOfCharacter > 0) {
        currentEle -= 1;
        byteOfCharacter -= 1;
    }
    else if (!asWCharFlag && (SINGLE_BYTE_CHAR_SET || bytesOfPrev > 0)) {
        if (bytesOfPrev <= 0) {
            bytesOfPrev = 1;
        }
        currentEle -= 1;
        if (current > 0) {
            current -= 1;
        }
        byteOfCharacter = bytesOfPrev - 1;
        bytesOfPrev = 0;
    }
    else {
        asWideString(TRUE);
        current -= 1;
        currentEle -= 1;
    }
    atPut(currentEle, &c, 1);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Put c back into this VStream and decrement the current position.
//
void VStream::ungetch(VWideChar wc)
{
    if (getCurrentEle() == 0 /* || wc == LEOF */) { // pkt 08/21/95 LEOF obsolete
        return;
    }
    
    if (!asWCharFlag) {
        char mbc[CV_MB_LEN_MAX];
        int nchars;
        if ((asByteFlag || SINGLE_BYTE_CHAR_SET) && !asWCharFlag) {
            if (!wctoa(mbc[0], wc)) {
                mbc[0] = '?';
            }
            nchars = 1;
        }
        else {
            nchars = VWideCharAux::wctomb(mbc, wc);
            if (nchars < 0) {
                mbc[0] = '?';
                nchars = 1;
            }
        }
        if (byteOfCharacter > 0) {
            currentEle -= byteOfCharacter;
            byteOfCharacter = 0;
        }
        else if (SINGLE_BYTE_CHAR_SET || bytesOfPrev > 0) {
            if (bytesOfPrev <= 0) {
                bytesOfPrev = 1;
            }
            currentEle -= bytesOfPrev;
            if (current > 0) {
                current -= 1;
            }
            byteOfCharacter = 0;
            bytesOfPrev = 0;
        }
        else {
            // Is the following inefficiency better than doing nothing?
            if (getCurrent() <= 0) {
                return;
            }
            current -= 1;
            currentEle = toElePosition(current);
            byteOfCharacter = 0;
            bytesOfPrev = 0;
        }
        atPut(currentEle, mbc, nchars);
    }
    else {
        current -= 1;
        currentEle -= 1;
        atPut(current, &wc, 1);
    }
}
#endif

#if 0
// ---------------------------------------------------------------------------
//  Set the current position of this VStream to slot n or to its length
//  whichever is less.  If n is less than zero, then set the current position
//  the slot at the length of this VStream minus n.  Return the new 
//  current position.  Positions are measured in the elements of the
//  current representation (bytes or wide characters).
//
integer VStream::at(integer n)
{
    if (currentEle != n) {
        if (n < 0) {
            currentEle = lengthEle + n;
            if (currentEle < 0) {
                currentEle = 0;
            }
        }
        else if (n > lengthEle) {
            currentEle = lengthEle;
        }
        else {
            currentEle = n;
        }
        byteOfCharacter = 0;
        bytesOfPrev = 0;
        
        // Attempt to set 'current' to reflect the new 'currentEle'.
        if (isAsWide()) {
            current = currentEle;
        }
        else if (currentEle == 0) {
            current = 0;
        }
        else if (currentEle == lengthEle) {
            current = length;
        }
        else {
            current = -1; // unknown
        }
        // getCurrent();// temp fix to make sure current is not -1
        //          // when stream is used later - EC

    }
    return currentEle;
}
#else
// ---------------------------------------------------------------------------
//  Set the current position of this VStream to slot n or to its length
//  whichever is less.  If n is less than zero, then set the current position
//  the slot at the length of this VStream minus n.  Return the new 
//  current position.  The position is measured based on a multibyte 
//  representation of the VStream.
//
integer VStream::at(integer n)
{
    asWideString(FALSE);
    if (currentEle != n) {
        if (n < 0) {
            currentEle = lengthEle + n;
            if (currentEle < 0) {
                currentEle = 0;
            }
        }
        else if (n > lengthEle) {
            currentEle = lengthEle;
        }
        else {
            currentEle = n;
        }
        byteOfCharacter = 0;
        bytesOfPrev = 0;
        
        // Attempt to set 'current' to reflect the new 'currentEle'.
        if (currentEle == 0) {
            current = 0;
        }
        else if (currentEle == lengthEle) {
            current = length;
        }
        else {
            current = -1; // unknown
        }
    }
    return currentEle;
}
#endif

// ---------------------------------------------------------------------------
//  Set the current position of this VStream to slot n or to its length
//  whichever is less.  If n is less than zero, then set the current position
//  the slot at the length of this VStream minus n.  Return the new 
//  current position.  Positions are measured in characters.
//
integer VStream::setCharPos(integer n)
{
    if (getCurrent() != n) {
        if (n < 0) {
            current = length + n;
            if (current < 0) {
                current = 0;
            }
        }
        else if (n > length) {
            current = length;
        }
        else {
            current = n;
        }
        byteOfCharacter = 0;
        bytesOfPrev = 0;
        if (current == 0) {
            currentEle = 0;
        }
        else if (current == length) {
            currentEle = lengthEle;
        }
        else {
            // This could be speeded up for the case in which
            // the new position is beyond the old position,
            // because toElePosition will scan the entire
            // stream up to the current position.
            //
            // Also, we could do this in such a way that bytesOfPrev
            // would be set to allow ungetch.
            currentEle = toElePosition(current);
        }
    }
    return current;
}

// ---------------------------------------------------------------------------
//  Set the current position of this VStream to n.
//  n must be 0 or a value returned by getPos, because getPos and setPos
//  use whatever measuring system is most appropriate to the dynamic type
//  of this object.
//
integer VStream::setPos(integer n)
{
    return setCharPos(n);
}

// ---------------------------------------------------------------------------
//  Read upto n bytes from this VStream at the current position
//  into b and return the number of bytes read.  The return value may 
//  be less than n if the end of this VStream was reached first.
//
integer VStream::read(char *b, integer n)
{   
    if (b == 0 || n == 0 || lengthEle == currentEle) {
        charCount = 0;
        eleCount = 0;
        return 0;
    }
    if (n + getCurrentEle() > getLengthEle()) {
        n = getLengthEle() - getCurrentEle();
    }
    if (SINGLE_BYTE_CHAR_SET) {
        n = atGet(currentEle, b, n);
        if (current > 0) {
            current += charCount;
        }
        currentEle += eleCount;
        byteOfCharacter = 0;
        bytesOfPrev = 0;
    }
    else {
        n = atGet(currentEle, b, n);
        getCurrent();
        current += charCount;
        currentEle += eleCount;
        byteOfCharacter = 0;
        bytesOfPrev = 0;
        if (asMultibyte()) {
            // !!! Possibly too inefficient:
            integer currentCharacterEle = toElePosition(current);
            if (currentEle > currentCharacterEle) {
                byteOfCharacter = short(currentEle - currentCharacterEle);
            }
        }
    }
    return n;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Read upto n wide characters from this VStream at the current position
//  into b and return the number of characters read.  The return value
//  may be less than n if the end of this VStream was reached first.
//
integer VStream::read(VWideChar *b, integer n)
{
    if (CV_CALL_SINGLE_BYTE_GETS) {
        // It would be nice to call one of the single byte
        // virtual functions here, but too complicated.
        // The programmer who derives from VStream must
        // override this version of 'read'.
    }
    if (b == 0 || n == 0 || lengthEle == currentEle) {
        charCount = 0;
        eleCount = 0;
        return 0;
    }
    if (n + getCurrent() > length) {
        n = length - current;
    }
    n = atGet(current, b, n);
    current += charCount;
    currentEle += eleCount;
    byteOfCharacter = 0;
    bytesOfPrev = 0;
    if (asMultibyte()) {
        // !!! Possibly too inefficient:
        integer currentCharacterEle = toElePosition(current);
        if (currentEle > currentCharacterEle) {
            byteOfCharacter = short(currentEle - currentCharacterEle);
        }
    }
    return n;
}
#endif

// ---------------------------------------------------------------------------
//  Write upto n bytes of b into this VStream at the current
//  position and return the number of bytes written.  The number of
//  bytes returned may be less than n if the end of this VStream 
//  was reached and it is not expandable.
//
integer VStream::write(const char *b, integer n)
{
    if (b == 0 || n == 0) {
        charCount = 0;
        eleCount = 0;
        return 0;
    }
    if (n + getCurrentEle() > nEle) {
        if (expandable()) {
            expandTo(roundUp(n + getCurrentEle()));
        }
        else {
            n = nEle - getCurrentEle();
        }
    }
    integer chars = atPut(VStream::at(), b, n);
    getCurrent();
    current += charCount;
    if (!asMultibyte() || SINGLE_BYTE_CHAR_SET) {
        currentEle += eleCount;
        byteOfCharacter = 0;
        bytesOfPrev = 1;        
    }
    else {
        // multibyte representation:
        // Parse the written block of bytes into characters using mbtowc
        // in order to find the current byteOfCharacter, etc.
        integer chars_to_process = eleCount + byteOfCharacter;
        integer ele_to_process = currentEle - byteOfCharacter;
        char *p = getData();
        for (;;) {
            if (chars_to_process <= 0) {
                break;
            }
            int ch_size = VWideCharAux::mbtowc(0, p + ele_to_process, chars_to_process);
            // Process null characters as valid.
            ch_size = (ch_size == 0) ? 1 : ch_size;
            if (ch_size < 0 && chars_to_process < CV_MB_CUR_MAX) {
                // Assume that we have an incomplete multibyte character
                // at the end of the written block of bytes.
                break;
            }
            if (ch_size < 0) {
                // Cound not find a correct multibyte character.
                // Punt by taking the next byte, because
                // it's necessary to recover with some kind of a count.
                ch_size = 1;
            }
            chars_to_process -= ch_size;
            ele_to_process += ch_size;
            bytesOfPrev = ch_size;
            byteOfCharacter = 0;
        }
        if (chars_to_process > 0) {
            // an incomplete multibyte character
            byteOfCharacter = short(chars_to_process);
        }
        currentEle += eleCount;
    }
    if (current > length) {
        length = current;
    }
    if (getCurrentEle() > lengthEle) {
        lengthEle = getCurrentEle();
    }
    return getCount();
}

// ---------------------------------------------------------------------------
//  Write str into this VStream at the current
//  position and return the number of characters written.  The number of
//  elements returned may be less than n if the end of this VStream 
//  was reached and it is not expandable.
//
integer VStream::write(const VString& str)
{
    if (!isAsWide()) {
        const char *p = str.gets();
        return write(p, str.size()), charCount;
    }
    else {
        const VWideChar *wp = str.getwcs();
        return write(wp, str.getCharSize());
    } 
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Write upto n wide characters of b into this VStream at the current
//  position and return the number of characters written.  The number of
//  characters returned may be less than n if the end of this VStream 
//  was reached and it is not expandable.
//
integer VStream::write(const VWideChar *b, integer n)
{
    if (CV_CALL_SINGLE_BYTE_GETS) {
        // It would be nice to call one of the single byte
        // virtual functions here, but too complicated.
        // The programmer who derives from VStream must
        // override this version of 'write'.
    }
    if (b == 0 || n == 0) {
        eleCount = 0;
        charCount = 0;
        return 0;
    }
    if (n + getCurrent() > nEle) {
        if (expandable()) {
            expandTo(roundUp(n + getCurrentEle()));
        }
        else {
            n = nEle - getCurrentEle();
        }
    }
    atPut(VStream::getCharPos(), b, n);
    cv_assert(asWCharFlag);
    current += n;
    currentEle += n;
    if (current > length) {
        length = current;
    }
    if (getCurrentEle() > lengthEle) {
        lengthEle = getCurrentEle();
    }
    eleCount = n;
    charCount = n;
    return n;
}
#endif

// ---------------------------------------------------------------------------
//  Copy VString s into this VStream at the current position, advance
//  the current position by the number of characters copied, and return the
//  number of characters copied.
//
integer VStream::puts(VString *s)
{
    if (s == 0) {
        eleCount = 0;
        charCount = 0;
        return 0;
    }
    if (s->isAsByte()) {
        return VStream::puts(s->gets());
    }
    else if (s->isAsWide()) {
        return VStream::puts(s->getwcs());
    }
    else {
        VStream::puts(s->gets());
        return charCount;
    }
}

// ---------------------------------------------------------------------------
//  Copy the NULL terminated C string s into this VStream at the
//  current position, advance the current position by the number of bytes
//  copied, and return the number of bytes copied (do not copy the NULL
//  byte).
//
integer VStream::puts(const char *s)
{
    if (s == 0) {
        eleCount = 0;
        charCount = 0;
        return 0;
    }
    return write((char *)s, strlen((char *)s));
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Copy the NULL terminated wide character string s into this VStream at the
//  current position, advance the current position by the number of characters
//  copied, and return the number of characters copied (do not copy the NULL
//  byte).
//
integer VStream::puts(const VWideChar *s)
{
    if (s == 0) {
        eleCount = 0;
        charCount = 0;
        return 0;
    }
    asWideString(TRUE);
    
    // like write, either expand this VStream or shrink the
    // number n of characters to write
    integer n = (integer)VWideCharAux::wcslen(s);
    if (n + getCurrentEle() >= nEle) {
        if (expandable()) {
            expandTo(roundUp(n + getCurrentEle()));
        }
        else {
            n = nEle - getCurrentEle();
        }
    }
    
    // like atPut
    VWideChar *q = getWideData();
    if (q == 0) {
        eleCount = 0;
        charCount = 0;
        return 0;
    }
    q += current;
    for (integer k = n; k > 0; k--) {
        *q++ = *s++;
    }
    
    // continuing write-like
    current += n;
    currentEle += n;
    if (current > length) {
        length = current;
    }
    if (getCurrentEle() > lengthEle) {
        lengthEle = getCurrentEle();
    }
    eleCount = n;
    charCount = n;
    return n;
}
#endif

// ---------------------------------------------------------------------------
//  Copy the NULL terminated C string s into this VStream at the
//  current position, advance the current position, and add a line termination.  
//  Return the number of bytes copied.
//
integer VStream::putln(const char *s)
{
    integer nEle = 0;
    integer nChar = 0;
    if (s != 0) {
        puts(s);
        nEle = eleCount;
        nChar = charCount;
        s += nEle - 1;
    }
    if (s == 0 || !(*s == '\n' || *s == '\r')) {
        puts(LINE_TERM);
        eleCount += nEle;
        charCount += nChar;
    }
    return getCount();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Copy the NULL terminated C string s into this VStream at the
//  current position, advance the current position, and add a line termination.  
//  Return the number of characters copied.
//
integer VStream::putln(const VWideChar *s)
{
    integer nEle = 0;
    integer nChar = 0;
    if (s != 0) {
        puts(s);
        nEle = eleCount;
        nChar = charCount;
        s += nEle - 1;
    }
    if (s == 0 || !(*s == CV_WCHAR_CONST('\n') || *s == CV_WCHAR_CONST('\r'))) {
        puts(getWideLineTerm());
        eleCount += nEle;
        charCount += nChar;
    }
    return getCount();
}
#endif

// ---------------------------------------------------------------------------
//  Read upto n characters from this VStream at the current position
//  into s.  Add a NULL byte to s and return the number of bytes read.  
//  The return value will be less than n if the end of this VStream 
//  was reached. The current position is advanced by the number of 
//  characters read.
//
integer VStream::gets(char *s, integer n)
{
    integer i = read(s, n);
    pointer sp = s;
    sp[i] = '\0';
    return i;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Read upto n characters from this VStream at the current position
//  into s.  Add a NULL byte to s and return the number of characters read.  
//  The return value will be less than n if the end of this VStream 
//  was reached. The current position is advanced by the number of 
//  characters read.
//
integer VStream::gets(VWideChar *s, integer n)
{
    integer i = read(s, n);
    VWideChar *sp = s;
    sp[i] = CV_WCHAR_CONST('\0');
    return i;
}
#endif

// ---------------------------------------------------------------------------
//  Read upto n bytes, or a line-termination, into s from this
//  VStream at the current position.  Strip the line termination and return 
//  the number of bytes read.  The return value will be less than n
//  if the end of this VStream or a line termination was reached first. 
//  The current position is advanced by the bytes read.
//
integer VStream::gettrim(char *s, integer n)
{
    getln(s, n);
    while (eleCount) {
        if (s[eleCount - 1] != '\n' && s[eleCount - 1] != '\r') {
            // !!! For now, assume that a control character cannot be
            // a trailing byte of a multibyte character.
            break;
        }
        else {
            eleCount -= 1;
            charCount -= 1;
            s[eleCount] = '\0';
        }
    }
    return getCount();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Read upto n characters, or a line-termination, into s from this
//  VStream at the current position.  Strip the line termination and return 
//  the number of characters read.  The return value will be less than n
//  if the end of this VStream or a line termination was reached first. 
//  The current position is advanced by the number of characters read.
//
integer VStream::gettrim(VWideChar *s, integer n)
{
    getln(s, n);
    while (eleCount) {
        if (s[eleCount - 1] != CV_WCHAR_CONST('\n') && s[eleCount - 1] != CV_WCHAR_CONST('\r')) {
            break;
        }
        else {
            eleCount -= 1;
            charCount -= 1;
            s[eleCount] = CV_WCHAR_CONST('\0');
        }
    }
    return getCount();;
}
#endif

// ---------------------------------------------------------------------------
//  Read up to n bytes, or up to a line-termination, into s from
//  this VStream at the current position.  Add a null byte to s and 
//  return the number of bytes read.  The return value will be less 
//  than n if the end of this VStream or a line termination was reached 
//  first. The current position is advanced by the number of characters read.
//
integer VStream::getln(char *s, integer n)
{
    integer c;
    integer cur = getCurrent();
    integer byte_count = 0;
    short mb_cur_max = CV_MB_CUR_MAX;
    while (byte_count < n && (c = getch()) >= 0) {
        byte_count += 1;
        *s++ = (char)c;
        if (byteOfCharacter == 0) {
#ifdef V_MACINTOSH
            if (c == '\x0D')  {                 // Apr.27 This is a carriage return
                if ((peekch() == '\x0A')) {     // Apr.27 is the next a line feed
                    byte_count += 1;            // Apr.27
                    *s++ = (char)getch();       // Apr.27 if so, return it too.
                }                               // Apr.27
                break;
            }                                   // Apr.27
#else
            if (c == '\n') {
                break;
            }
#endif
        }
        if (byte_count + mb_cur_max > n) {
            // Reading further could strand us in the midst of a
            // multibyte character.
            break;
        }
    }
    *s++ = '\0';
    eleCount = byte_count;
    if (SINGLE_BYTE_CHAR_SET) {
        charCount = eleCount;
    }
    else {
        charCount = getCurrent() - cur;
    }
    return getCount();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Read upto n characters, or upto a line-termination, into s from
//  this VStream at the current position.  Add a NULL byte to s and 
//  return the number of characters read.  The return value will be less 
//  than n if the end of this VStream or a line termination was reached 
//  first. The current position is advanced by the number of characters read.
//
integer VStream::getln(VWideChar *s, integer n)
{
    integer element_count = 0;
    VWideChar c;
    while (element_count < n && getch(c)) {
        element_count += 1;
        *s++ = (char)c;
#ifdef V_MACINTOSH
        if (c == CV_WCHAR_CONST('\x0D'))  {             // Apr.27 This is a carriage return
            if ((peekch() == CV_WCHAR_CONST('\x0A'))) { // Apr.27 is the next a line feed
                element_count += 1;         // Apr.27
                getch(*s++);                // Apr.27 if so, return it too.
            }                               // Apr.27
            break;
        }                                   // Apr.27
#else
        if (c == CV_WCHAR_CONST('\n')) {
            break;
        }
#endif
    }
    *s++ = CV_WCHAR_CONST('\0');
    eleCount = element_count;
    charCount = eleCount;
    return eleCount;
}
#endif

// ===========================================================================
#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
//  Read this VStream from the VArchiver a.
//
void VStream::getFrom(VArchiver &a)
{
    integer n;      // jmd 8/11/93

    a >> current;
    currentEle = current;
    a >> length;
    lengthEle = length;
    a >> flags;
    a >> n;
    expandTo(n);
    a.get(getData(), n);
    asWCharFlag = 0;
    asByteFlag = 1;
    byteOfCharacter = 0;
    asByteString(FALSE);
}

// ---------------------------------------------------------------------------
//
void VStream::getFrom(VArchiver& a,
                      VObject * /* data */, boolean /* do_realize */ )
{
    getFrom(a);
}

// ---------------------------------------------------------------------------
//  Store this VStream into the VArchiver a.
//
void VStream::putTo(VArchiver& a)
{
    asWideString(FALSE);
    integer i = nEle;
    a << getCurrentEle();
    a << getLengthEle();
    a << flags;
    if (expandable()) {
        nEle = getLengthEle();
    }
    a << nEle;
    if (!(nEle == 0 || getData() == 0)) {
        a.put(getData(), nEle);
    }
    nEle = i;
}

#endif // ARCHIVER

// ===========================================================================

// ---------------------------------------------------------------------------
//  Call getch() until the byte c is encountered.  Return TRUE
//  if byte c is found, otherwise return FALSE.
//
boolean VStream::skipTo(char c)
{
    int t, lt = 0;
    while ((t = getch()) > 0) {
        if (t == '\\' && lt == '\\') {
            lt = 0;
            continue;
        }
        if (t == c && lt != '\\') {
            break;
        }
        lt = t;
    }
    return t >= 0;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Call getch() until the character c is encountered.  Return TRUE
//  if character c is found, otherwise return FALSE.
//
boolean VStream::skipTo(VWideChar c)
{
    boolean b;
    VWideChar t, lt = 0;
    while (b = getch(t)) {
        if (t == CV_WCHAR_CONST('\\') && lt == CV_WCHAR_CONST('\\')) {
            lt = 0;
            continue;
        }
        if (t == c && lt != CV_WCHAR_CONST('\\')) {
            break;
        }
        lt = t;
    }
    return b;
}
#endif

// ---------------------------------------------------------------------------
//  Scan this VStream until an unmatched 'end' character is reached.
//  As the scan progresses, each 'end' character is matched if possible
//  with a 'beg' character that precedes it and which was found during
//  the scan.  If the unmatched 'end' is found, the VStream is left
//  positioned after that character and TRUE is returned.  Otherwise
//  the VStream is left positioned at its end and FALSE is returned.
//
//  This method is useful, for example, in
//  parsing when a VStream is currently positioned at a '{' or '(' character
//  and you want to find the matching '}' or ')' characters taking into account 
//  nested occurences.
//
boolean VStream::scanToMatching(char beg, char end)
{
    integer count = 0;
    if (!asWCharFlag || CV_CALL_SINGLE_BYTE_GETS) {
        for (;;) {
            // Call single byte version of getch to support derived classes
            // which override only that virtual function.
            int c;
            char mbc[CV_MB_LEN_MAX];
            int byte_count = 0;
            for (;;) {
                if (byte_count >= CV_MB_CUR_MAX) {
                    break;
                }
                c = getch();
                // 'getch()' updates currentEle and current.
                if (c == -1) {
                    return FALSE;
                }
                byte_count += 1;
                if (isAsByte()) {
                    break;
                }
                mbc[byte_count] = (char)c;
                int ch_size = VWideCharAux::mbtowc(0, mbc, byte_count);
                if (ch_size == byte_count) {
                    // The wide character is complete.
                    break;
                }
            }
            if (byte_count == 1) { 
                if (c == beg) {
                    count++;
                }
                if (c == end && count-- == 0) {
                    return TRUE;
                }
            }
        }

    }
    else {
        VWideChar wbeg;
        VWideChar wend;
        boolean OK = atowc(wbeg, beg) && atowc(wend, end);
        for (;;) {
            VWideChar c;
            if (!getch(c)) {
                return FALSE;
            }
            if (OK) {
                if (c == wbeg) {
                    count++;
                }
                if (c == wend && count-- == 0) {
                    return TRUE;
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
//  Return a pointer to the contents of this VStream.  
//  The contents should not be modified.
//
CV_GETS_CONST char *VStream::gets() const
{ 
    if (getData() == 0) {
        return "";
    }
    getData()[getLengthEle()] = 0;
    return VString::gets(); 
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//  Return a pointer to the contents of this VStream.  
//  The contents should not be modified.
//
const VWideChar *VStream::getwcs() const
{ 
    if (getWideData() == 0) {
#if !defined(CV_OWN_VWIDECHAR) && !defined(CPPV_HPUX)
        return CV_WCHAR_CONST("");
#else
        static VWideChar wa[1];
        wa[0] = 0;
        return wa;
#endif
    }
    getWideData()[getLengthEle()] = 0;
    return VString::getwcs(); 
}
#endif

// ---------------------------------------------------------------------------
//  Copy the contents of this VStream into the C string 's' and
//  return it.  It is the caller's responsibility to ensure that the 
//  character buffer pointer to by s is large enough to receive the 
//  the entire contents.
//
char *VStream::gets(char *s) const
{ 
    if (getData() == 0) {
        s[0] = '\0';
        return s;
    }
    getData()[getLengthEle()] = 0;
    return VString::gets(s);
}

// ---------------------------------------------------------------------------
//  Make this VStream the same as object obj, where obj must be derived
//  from VString.  Return TRUE if successful, otherwise return FALSE.
//
boolean VStream::copyFrom(VObject *obj)
{ 
    if (obj->isA(VStringCls) && !obj->isA(VStreamCls)) {
        //  source is a string but not a stream
        reset();
        puts(((VString *)obj)->gets());
        return TRUE;
    }
    if (!obj->isA(VStreamCls)) {
        //  source is neither a string nor a stream
        return FALSE;
    }

    //  source is a stream
    VStream *s = (VStream *)obj;
    expandTo(s->nEle);
    length = s->length;
    lengthEle = s->lengthEle;
    integer i;
    asByteFlag = s->asByteFlag;
    if (asWCharFlag = s->asWCharFlag) {
        pointer p, q;
        if (getLengthEle() > 0) {
            for (p = getData(), q = s->getData(), i = 0; i < getLengthEle(); i++) {
                *p++ = *q++;
            }
        }
    }
    else {
        VWideChar *p;
        VWideChar *q;
        if (getLengthEle() > 0) {
            for (p = getWideData(), q = s->getWideData(), i = 0; i < getLengthEle(); i++) {
                *p++ = *q++;
            }
        }
    }
    current = s->getCurrent();
    currentEle = s->currentEle;
    byteOfCharacter = s->byteOfCharacter;
    bytesOfPrev = s->bytesOfPrev;
    flags = s->flags;
    return TRUE;
}

// ---------------------------------------------------------------------------
//
VStream& VStream::operator<<(int i)
{
    char b[12];
    sprintf(b, "%d ", i);
    return *this << b;
}

// ---------------------------------------------------------------------------
//
VStream& VStream::operator<<(long l)
{
    char b[12];
    sprintf(b, "%ld ", l);
    return *this << b;
}

// ---------------------------------------------------------------------------
//  Extract a format string (_fmtstr) from the current position of this
//  VStream into s.  Advance the current position by the number of 
//  characters extracted.
//
VStream& VStream::operator>> (const _fmtstr& s)
{
    if (s.fmt[1] == 'c') {
        ((int *) s.data)[0] = getch();
        return *this;
    }

    if (!asWCharFlag && (asByteFlag || SINGLE_BYTE_CHAR_SET)) {
        char    b[MAX_STRING + 1];
        char   *p = b;
        int     ch;
        
        while ((ch = getch()) >= 0) {
            if (isspace(ch)) {
                break;
            }
            *p++ = ch;
        }
        *p = 0;
        sscanf(b, s.fmt, s.data);
    }
    else {
        VWideChar b[MAX_STRING + 1];
        VWideChar *p = b;
        VWideChar ch;
        
        // Note that if getch(VWideChar&) is not overridden, and 
        // if CALL_SINGLE_BYTE_GETS is defined, then the call to 
        // getch(VWideChar&) will call the single byte version of getch().
        while (getch(ch)) {
#ifndef __SC__
            if ((VWideCharAux::iswspace)(ch)) {
#else
            if (VWideCharAux::iswspace(ch)) {
#endif
                break;
            }
            *p++ = ch;
        }
        *p = 0;
        VString fmt = s.fmt;
#if defined (CV_WIN32) &&  defined(_MSC_VER )
        if (s.fmt[1] != 's') {
            swscanf(b, fmt.getwcs(), s.data); // !!! pkt: will only work on NT
            //only MSVC21
        }
        else {
            VWideChar found[MAX_STRING];
            swscanf(b, fmt.getwcs(), found);
            VString found_string = found;
            strcpy((char *) s.data, found_string.gets());
        }
#elif defined(CV_HAS_SWSCANF)
        if (s.fmt[1] != 's') {
            VWideCharAux::swscanf(b, fmt.getwcs(), s.data);
        }
        else {
            VWideChar found[MAX_STRING];
            VWideCharAux::swscanf(b, fmt.getwcs(), found);
            VString found_string = found;
            strcpy((char *) s.data, found_string.gets());
        }
#else
// not internationalized because no swscanf in 16 bit runtimes
        char bb[MAX_STRING +1];
        VWideCharAux::wcstombs(bb,b, MAX_STRING+1);

        if (s.fmt[1] != 's') {
            sscanf( bb, (char *) fmt.gets(), s.data); 
        }
        else {
            char found[MAX_STRING];
            sscanf( bb, (char *) fmt.gets(), found);
            VString found_string = found;
            strcpy((char *) s.data, found_string.gets());
        }
#endif
    }
    return *this;
}

// ---------------------------------------------------------------------------
//
_fmtstr VStream::scan(char *p...)
{
    _fmtstr f;
    f.fmt = p;

    va_list a;
    va_start(a, p);
    f.data = va_arg(a, void *);
    va_end(a);

    return f;
}

// ---------------------------------------------------------------------------
//
VString VStream::format(char *p...)
{
    // char b[MAX_STRING + 1];
    VString s;

    va_list a;
    va_start(a, p);
    char *bufp = selectFormatBuffer(0, MAX_STRING + 1, &s, p, a);
    va_end(a);
    if (bufp != 0) {
        // bufp points into s.
        va_start(a, p);
        vsprintf(bufp, p, a);
        va_end(a);
    }
    return s;
}

// ---------------------------------------------------------------------------
// Truncate the string at zero-based offset 'idx'.  Returns TRUE if
// the stream was changed by this operation.  No effect if 'idx' is
// past the end of the string.
//
boolean VStream::truncAtCharPos(integer idx)
{
    integer ele_idx = toElePosition(idx);
    boolean result = ele_idx < lengthEle;
    VStream::truncAt(ele_idx);
    return result;
}

// ---------------------------------------------------------------------------
//
boolean VStream::padTo(integer idx, char pad)
{
    boolean result = VString::padTo(idx, pad);
    if (result && idx > length) {
        // The string was extended.
        length = idx;
        lengthEle = toElePosition(length);
    }
    return result;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//
boolean VStream::padTo(integer idx, VWideChar pad)
{
    boolean result = VString::padTo(idx, pad);
    if (result && idx > length) {
        // The string was extended.
        length = idx;
        lengthEle = toElePosition(length);
    }
    return result;
}
#endif

// ---------------------------------------------------------------------------
//
boolean VStream::deleteAtCharPos(integer idx, integer count)
{
    integer original_length = length;
    integer original_current = getCurrent();
    short original_byte = byteOfCharacter;
    boolean result = VString::deleteAtCharPos(idx, count);
    if (original_length >= idx + count) {
        length = original_length - count;
        lengthEle = toElePosition(length);
    }
    else if (original_length > idx) {
        length = idx;
        lengthEle = toElePosition(length);
    }
    if (original_current >= idx + count) {
        current = original_current - count;
        currentEle = toElePosition(current) + original_byte;
    }
    else if (original_current > idx) {
        current = idx;
        currentEle = toElePosition(current);
        byteOfCharacter = 0;
    }
    if (idx < original_current && original_current <= idx + count && result) {
        if (asMultibyte()) {
            bytesOfPrev = 0; // unknown
        }
        else {
            bytesOfPrev = 1;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
//
void VStream::insertAtCharPos(const VString& ins, integer idx)
{
    if (idx <= VString::getCharSize()) {
        integer count = ins.getCharSize();
        if (ins.isAsByte() && !isAsByte()) {
            VString copy = ins.gets(); // make multibyte copy
            count = copy.getCharSize();
        }
        integer original_length = length;
        integer original_current = getCurrent();
        short original_byte = byteOfCharacter;
        VString::insertAtCharPos(ins, idx);
        if (idx <= original_length) {
            length = original_length + count;
            lengthEle = toElePosition(length);
        }
        if (idx <= original_current) {
            current = original_current + count;
            currentEle = toElePosition(current) + original_byte;
        }
        if (idx == original_current) {
            if (asMultibyte()) {
                bytesOfPrev = 0; // unknown
            }
            else {
                bytesOfPrev = 1;
            }
        }
    }
    else if (idx <= length) {
        // Insertion among null characters is rather meaningless.
    }
}

// ---------------------------------------------------------------------------
// Prevent hiding of this variant of 'insertAtCharPos'.
//
void VStream::insertAtCharPos(const char *ins, integer idx, integer inssize, char pad)
{
    VString::insertAtCharPos(ins, idx, inssize, pad);
}

// ---------------------------------------------------------------------------
// Prevent hiding of this variant of 'insertAtCharPos'.
//
void VStream::insertAtCharPos(const char c, integer idx, char pad)
{
    VString::insertAtCharPos(c, idx, pad);
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//
void VStream::insertAtCharPos(const VWideChar *ins, integer idx, integer inssize, VWideChar pad)
{
    VString::insertAtCharPos(ins, idx, inssize, pad);
}

// ---------------------------------------------------------------------------
// Prevent hiding of this variant of 'insertAtCharPos'.
//
void VStream::insertAtCharPos(const VWideChar c, integer idx, VWideChar pad)
{
    VString::insertAtCharPos(c, idx, pad);
}
#endif

// ---------------------------------------------------------------------------
// Mutate this VStream between multibyte representation and 
// one-byte-per-character representation.
// If the current position is in the middle of a multibyte character,
// and the VStream is converted to a multibyte representation, then the
// position is adjusted to precede that multibyte character.
//
void VStream::asByteString(boolean byte) const
{
    VStream &changeable = *(VStream *)this;
    if (byte != asByteFlag || (byte && asWCharFlag)) {
        if (asWCharFlag) {
            asWideString(FALSE);
        }
        VString::asByteString(byte);
        if (getData() == 0) {
            changeable.length = 0;
            changeable.lengthEle = 0;
            changeable.current = 0;
            changeable.currentEle = 0;
            changeable.byteOfCharacter = 0;
            changeable.bytesOfPrev = 0;
        }
        else if (!byte) {           
            // Scan the stream so that the current position and length
            // can be represented by count of multibyte characters.
            VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
            int ch_size;
            char *cp0 = getData();
            char *cp = cp0;
            changeable.current = 0;
            integer valid_currentEle = 0;
            integer valid_lengthEle = 0;
            integer byte_length = length;
            changeable.length = 0;
            changeable.bytesOfPrev = 0;
            for (;;) {
                integer available_chars = byte_length - (cp - cp0);
                ch_size = VWideCharAux::mbtowc(0, cp, available_chars);
                // Handle null bytes as valid null characters.
                ch_size = ((ch_size == 0) && (available_chars > 0)) ? 1 : ch_size;
                if (ch_size <= 0) {
                    if (available_chars >= CV_MB_CUR_MAX) {
                        // There are enough bytes for any multibyte
                        // character, but we haven't reached the end
                        // of the stream.  Attempt to recover and still retain
                        // the stream's length, by treating this byte
                        // as a character.
                        ch_size = 1;
                    }
                    else {
                        break;
                    }
                }
                cp += ch_size;
                changeable.length += 1;
                valid_lengthEle = cp - cp0;
                if (valid_lengthEle <= currentEle) {
                    changeable.current = length;
                    valid_currentEle = valid_lengthEle;
                    changeable.bytesOfPrev = ch_size;
                }
            }
            changeable.byteOfCharacter = short(currentEle - valid_currentEle);
            changeable.currentEle = valid_currentEle;
            changeable.lengthEle = valid_lengthEle;
            getData()[lengthEle] = '\0';
        }
        else {
            changeable.length = lengthEle;
            changeable.current = currentEle;
            changeable.bytesOfPrev = currentEle > 0 ? 1 : 0;
            changeable.byteOfCharacter = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// Mutate this VStream between representations based on bytes and based
// on wide characters.  Preserves nEle (except when converting from wide
// character representation and the result won't fit).  Preserves embedded
// null characters.
//
void VStream::asWideString(boolean wide) const
{
    VStream &changeable = *(VStream *)this;
    if (wide != asWCharFlag) {
        integer old_nEle = nEle;

        VStream swapper;
        swapper.asWCharFlag = wide;
        swapper.asByteFlag = asByteFlag;
        swapper.flags = flags;
        if (wide ? (getData() == 0) : (getWideData() == 0)) {
            // Do nothing.  Swapper is already consistent with this VStream.
        }
        else if (wide) {
            // This VStream is multibyte; swapper has wide characters.

            // Ensure we have a valid current position.
            if (current < 0) {
                short byte_of_ch;
                changeable.current = fromElePosition(currentEle, &byte_of_ch);
                changeable.byteOfCharacter = byte_of_ch;
            }

            swapper.expandTo(nEle);
            VWideChar *swapper_data = swapper.getWideData();

            // Translate to wide characters as in mbstowcs,
            // but don't stop at a null character, and translate
            // unrecognizable characters as '?'.
            size_t n = 0;
            size_t read_count = 0;
            const char *cp = getData();         
            VWideChar *sdp = swapper_data;
            while ((integer)read_count < lengthEle) {
                int count = VWideCharAux::mbtowc(sdp, cp, lengthEle - read_count);
                if (count <=0) {
                    if (count < 0) {
                        // invalid multibyte character
                        *sdp = CV_WCHAR_CONST('?');
                    }
                    count = 1;
                }
                sdp += 1;
                cp += count;
                read_count += count;
                n += 1;
            }

            // Just in case, terminate the array with 0.
            if ((integer)n < nEle) {
                swapper_data[n] = CV_WCHAR_CONST('\0');
            }
            swapper_data[nEle] = CV_WCHAR_CONST('\0');

            // has been made into wide characters
            swapper.length = n;
            swapper.lengthEle = n;
            if (current > (integer)n) {
                cv_assert(FALSE);
                swapper.current = n;
            }
            else {
                swapper.current = current;
            }
            swapper.currentEle = swapper.current;
        }
        else {
            // This VStream has wide characters; swapper is multibyte.
            size_t mbchars_estimate = (size_t)nEle;
            for (;;) {
                swapper.expandTo(mbchars_estimate);

                swapper.lengthEle = -1;
                swapper.currentEle = -1;
                swapper.bytesOfPrev = 0;

                // Transform wide character data to multibyte string as per wcstombs,
                // except without stopping for null characters, and translating
                // unrecognized wide characters to '?'.
                char *s = swapper.getData();
                const VWideChar *pwcs = getWideData();
                size_t mb_max = CV_MB_CUR_MAX;
                size_t modified_count = 0;
                size_t read_count = 0;
                while ((integer)read_count < length + 1) {
                    if ((integer)read_count == length) {
                        swapper.lengthEle = modified_count;
                    }
                    if ((integer)read_count == current) {
                        swapper.currentEle = modified_count;
                    }
                    VWideChar wc = *pwcs;
                    size_t needed = wc ? mb_max : 1;
                    if (modified_count + needed >= mbchars_estimate + 1) {
                        break;
                    }
                    int count = VWideCharAux::wctomb(s, wc);
                    if (count < 0) {
                        count = 1;
                        *s = '?';
                    }
                    pwcs += 1;
                    s += count;
                    read_count += 1;
                    modified_count += count;
                    if ((integer)read_count == current) {
                        swapper.bytesOfPrev = count;
                    }
                }
                if ((integer)read_count == length + 1) {
                    break;
                }
                // Terminate with a 0 to ensure 'swapper' is in VString form.
                swapper.getData()[mbchars_estimate] = '\0';
                mbchars_estimate = mbchars_estimate * 2 + 1;
            }
            swapper.byteOfCharacter = 0;
            if (swapper.currentEle == -1) {
                swapper.currentEle = 0;
                swapper.current = 0;
            }
            else {
                swapper.current = current;
            }
            swapper.length = length;             
        }

        // Swap the content of this VStream with swapper.
        changeable.swapData(swapper);
        integer temp_length = length;
        integer temp_lengthEle = lengthEle;
        integer temp_current = current;
        integer temp_currentEle = currentEle;
        short temp_byteOfCharacter = byteOfCharacter;
        short temp_bytesOfPrev = bytesOfPrev;
        changeable.length = swapper.length;
        changeable.lengthEle = swapper.lengthEle;
        changeable.current = swapper.current;
        changeable.currentEle = swapper.currentEle;
        changeable.byteOfCharacter = swapper.byteOfCharacter;
        changeable.bytesOfPrev = swapper.bytesOfPrev;
        swapper.length = temp_length;
        swapper.lengthEle = temp_lengthEle;
        swapper.current = temp_current;
        swapper.currentEle = temp_currentEle;
        swapper.byteOfCharacter = temp_byteOfCharacter;
        swapper.bytesOfPrev = temp_bytesOfPrev;
    }
}

// ---------------------------------------------------------------------------
// Return the element position corresponding to the given character
// position.  They are the same unless the representation is currently
// multibyte characters.
//
integer VStream::toElePosition(integer character_pos) const
{
    if (!asMultibyte() || SINGLE_BYTE_CHAR_SET) {
        return character_pos;
    }
    else if (character_pos < 0) {
        return 0;
    }
    else {
        // Scan the string to find the position as byte offset.
        VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
        int ch_size;
        char *cp0 = getData();
        char *cp = cp0;
        for (integer pos = 0;; pos += 1, cp += ch_size) {
            if (pos == character_pos) {
                return cp - cp0;
            }
            ch_size = VWideCharAux::mbtowc(0, cp, CV_MB_LEN_MAX);
            if (ch_size <= 0) {
                return cp - cp0;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Return the character position corresponding to the given element
// position.  They are the same unless the representation is currently
// multibyte characters.
//
// If bp is not null, set *bp to the byte position of element_pos
// within the designated character.  This will be 0 if the element_pos
// is between characters.
//
integer VStream::fromElePosition(integer element_pos, short *bp) const
{
    if (!asMultibyte() || SINGLE_BYTE_CHAR_SET) {
        if (bp != 0) {
            *bp = 0;
        }
        return element_pos;
    }
    else if (element_pos < 0 || element_pos > nEle) {
        if (bp != 0) {
            *bp = 0;
        }
        return element_pos;
    }
    else {
        // Scan the stream to find the character position.
        VWideCharAux::mbtowc(0, 0, CV_MB_LEN_MAX); // Initialize the multibyte state.
        int ch_size;
        char *cp0 = getData();
        char *cp = cp0;
        integer pos = 0;
        for (;;) {
            integer cp_offset = cp - cp0;
            if (cp_offset > element_pos) {
                if (bp != 0) {
                    *bp = short(cp_offset - element_pos);
                }
                return pos - 1;
            }
            if (cp_offset == element_pos) {
                if (bp != 0) {
                    *bp = 0;
                }
                return pos;
            }
            integer available_chars = nEle - cp_offset;
            ch_size = VWideCharAux::mbtowc(0, cp, available_chars);
            // Handle null characters as valid characters.
            ch_size = ch_size == 0 ? 1 : ch_size;
            if (ch_size <= 0) {
                if (available_chars < CV_MB_CUR_MAX) {
                    // Reached an incomplete character.
                    if (bp != 0) {
                        *bp = short(available_chars);
                    }
                    return pos;
                }
                else {
                    // Reached an invalid, not an incomplete character. 
                    // Recover by treating the current byte as a
                    // character.
                    ch_size = 1;
                } 
            }
            cp += ch_size;
            pos += 1;
        }
    }
}

