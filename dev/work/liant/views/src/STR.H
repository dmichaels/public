// $Header:   Q:/views/common/vcs/STR.H_V   1.28   Mar 24 1997 16:13:28   scottt  $ 

//	str.h
//
//	VString class interface [Common]
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
//	09/13/92 jmd	preened
//	04/01/93 pmcm	added const to gets() per motif 2.1  source
//	04/09/93 pmcm	changed id to VObject *
//	07/04/93 pmcm	fireworks on the charles with a cast-o'-thousands;
//					preened.
//	07/19/93 jmd	made operator = take a const VString&, minor preening
//	07/19/93 jmd	made size() const
//	07/20/93 jmd	made comparison operators const
//	07/22/93 jmd	made char * args into const char *, preened
//					removed extra operator=(const VString&)
//					removed (VString&) constructor
//					made charIn const
//	07/31/93 jmd	made compare, isEqual etc. const
//	08/16/93 gjl	added OS/2 support, scanAmper()
//	08/30/93 jmd	added return value to getResource()
//	08/31/93 mrp	added operator == to compare two VStrings.
//	10/29/93 pkt	changed getResource to return # chars in string.
//	11/01/93 pkt	interchanged some inline definitions to prevent cfront
//					diagnostics about used before inlined.
//	11/02/93 pkt	changed so all relationals use compareCStr rather
//					than compare(VObject*).
//	11/02/93 pkt	made gets(char *, integer) const.
//	11/09/93 pkt	merged changes for MSC.
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened and merged with Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	02/23/94 pmcm	added dataCapacity member
//	07/11/94 dgm	fixed up assignment-operator and copy-constructor.
//	07/29/94 pkt	added 'identifyFormatConversion', 'measureFormat', and
//					selectFormatBuffer.
//	08/26/94 pkt	added return type declarations for relational operators.
//	01/12/95 pkt	added internationalization support.
//	01/12/95 pkt	added extensions for manipulating strings.
//	01/15/95 pkt	made operator VWideChar*()const return pointer to const.
//	01/20/95 pkt	overhauled for internationalization.
//	01/27/95 pkt	improved MBCS support.
//	02/01/95 pkt	made 'data' private.
//	02/01/95 pkt	made 'getData', 'setData', and 'clearData' protected.
//	02/01/95 pkt	replaced dataCapacity with nEle (pulled down from VStream).
//	02/02/95 pkt	changed 'tabTo' to 'padTo'.
//	02/10/95 pkt	added support for CV_CALL_SINGLE_BYTE_GETS.
//	03/07/95 pkt	added overloaded variants that take VString&.
//	03/20/95 pkt	changed some manipulation functions to virtual.
//	03/24/95 pkt	small fixes to manipulation functions.
//	03/30/95 pkt	updated so 'asByteFlag' can coexist with 'asWCharFlag'.
//	04/11/95 pkt	updated for change from VWChar to VWideChar.
//	04/11/95 pkt	updated for VWideCharAux.
//	04/12/95 pkt	fixed calls to 'wcstol' and 'wcstod'.
//	04/21/95 pkt	replaced MB_XXX with CV_MB_XXX.
//	04/21/95 pkt	moved definition to vwchar.h, from here, of CV_WCHAR,
//					SINGLE_BYTE_CHAR_SET, and CV_MB_HAS_STATE.
//	05/26/95 pkt	combined international changes into common version.
//	06/27/95 pkt	added CV_NO_STOL to suppress implicit conversion to long.
//	07/13/95 pkt	changed PM's 'scanAmper' to accept a const char *.
//	07/21/95 pkt	changed 'gets(VWideChar *, integer)' to not be const.
//	08/14/95 pkt	moved 'operator long()' down to avoid forward reference
//					to inline function.
//	08/16/95 pkt	added 'swapData'.
//	08/25/95 pkt	made class VWideCharAux a friend.
//	09/06/95 pkt	removed OFI code.
//	09/14/95 pkt	updated 'operator!' to not call 'size' on VStrings.
//	09/14/95 pkt	updated 'putln(const VWideChar *)' to return count of 
//					characters appended.
//	09/26/95 pkt	moved definition of 'getWideData' to pacify IBM compiler.
//	10/03/95 pkt	inline definitions of 'atowc' and 'wctoa' use VWideCharAux.
//	12/12/95 pkt	added variants of 'puts'.
//	01/10/96 pkt	changed new 'puts' variants to 'write'.
//	02/13/96 pkt	added 'gets(char *, integer) const' and 
//					'gets(VWideChar *, integer) const'.
//	02/15/96 pkt	changed 'operator[]' to avoid warnings in 16-bit compiles.
//	06/05/96 pkt	fixed 'insertAtCharPos' to insert string when default
//					insertion size applies.
//	01/21/97 pkt	fixed 'operator=(const char *)' not to append when 'this'
//					is a VStream.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSTRING_H
#define VSTRING_H

// ---------------------------------------------------------------------------
//
//#define WCHAR_H_EXISTS
#include "object.h"
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include "vwchar.h"

// ---------------------------------------------------------------------------
//
#define CV_GETS_CONST const
#define CV_CALL_SINGLE_BYTE_GETS (0)
#define MAX_STRING 256

// ---------------------------------------------------------------------------
//	A VString is a 0 terminated string of ASCII characters.
//	The content of a VString with a null data pointer is considered to be "",
//	not a null pointer.
//
CLASS VString : public VObject {
public:
	//	Constructors & Destructor
					VString();
					VString(const char *s);
					VString(const char *s, unsigned int length);
#ifdef CV_WCHAR
					VString(const VWideChar *s);
					VString(const VWideChar *s, unsigned int length);
#endif
					VString(const VString& s);
				   ~VString();

	VClass         *iam();

	//	Reading and Writing
	integer			putln(const char *s);
	integer			putln(const VString& s);
	integer			prints(const char *s, ...);
	integer			println(const char *s, ...);
#ifdef CV_WCHAR
	integer			putln(const VWideChar *s);
	integer			prints(const VWideChar *s, ...);
	integer			println(const VWideChar *s, ...);
#endif
	integer			gets(char *s, integer n) const;
	virtual CV_GETS_CONST char * gets() const;
	virtual const VWideChar *getwcs() const;
	const char	   *gets(char) const;
	virtual char   *gets(char *s) const;
#ifdef CV_WCHAR
	integer			gets(VWideChar *s, integer n) const;
	const VWideChar*gets(VWideChar) const;
#endif
	integer			getTill(char *s, char c) const;
	virtual integer puts(const char *s);
	virtual integer puts(VString *s);
	integer			puts(const VString& s);
	virtual integer write(const char *s, integer length);
#ifdef CV_WCHAR
	virtual integer puts(const VWideChar *s);
	virtual integer write(const VWideChar *s, integer length);
#endif

	//	Character <--> Byte Correspondence
	virtual void	asByteString(boolean) const;
	boolean			isAsByte() const;
	boolean			isAsWide() const;

	//	Assignment
	VString&		operator=  (const VString& s);
	VString&		operator=  (const char *s);
	VString&		operator=  (char c);
#ifdef CV_WCHAR
	VString&		operator=  (const VWideChar *s);
	VString&		operator=  (VWideChar c);
#endif

	//	Interrogation
	virtual			integer size() const;
	virtual			integer getCharSize() const;
	char			charAt(integer i) const;
	boolean			charIn(char c) const;
	CV_GETS_CONST char& operator[](integer i);
	VWideChar		wcharAt(integer i) const;
#ifdef CV_WCHAR
	boolean			charIn(VWideChar c) const;
#endif
	void			setAt(integer i, char c);
#ifdef CV_WCHAR
	void			setAt(integer i, VWideChar c);
#endif

	//	Comparison
	boolean			operator==(const VString& s) const;
	boolean			operator!=(const VString& s) const;
#if defined(CV_WCHAR)
	boolean			operator<=(const VString& s) const;
	boolean			operator>=(const VString& s) const;
	boolean			operator<(const VString& s) const;
	boolean			operator>(const VString& s) const;
#endif
	boolean			operator==(const char *s) const;
	boolean			operator!=(const char *s) const;
	boolean			operator>(const char *s) const;
	boolean			operator<(const char *s) const;
	boolean			operator>=(const char *s) const;
	boolean			operator<=(const char *s) const;

#if 1
#if _MSC_VER >= 700
	boolean			operator==(char *) const;
	boolean			operator!=(char *) const;
#endif
#endif

	int				operator!() const;

	int				compare(VObject *) const;
	int				compareCStr(const char *) const;
	int				compareVString(const VString&) const;
#ifdef CV_WCHAR
	int				compareCStr(const VWideChar *) const;
#endif

	//	Concatenation
	virtual integer	concat(VString *s);
	integer			concat(const VString& s);
	virtual integer	concat(const char *s);
	integer			concatTo(VString *s);
	integer			concat(char c);
	VString&		operator+=(const VString& s);
	VString&		operator+=(const char *s);
	VString&		operator+=(char c);
	integer			concatln();
	integer			concatPrints(const char *, ...);
#ifdef CV_WCHAR
	virtual integer	concat(const VWideChar *s);
	integer			concat(VWideChar c);
	VString&		operator+=(const VWideChar *s);
	VString&		operator+=(VWideChar c);
	integer			concatPrints(const VWideChar *, ...);
#endif

	//	Return a VString that is the concatenation of this and another
	VString			operator+(const VString& s) const;
	VString			operator+(char c) const;
#ifdef CV_WCHAR
	VString			operator+(VWideChar c) const;
#endif

	//	Equivalence
	boolean			isEqual(VObject *obj) const;
	HashKey			hash() const;

	//	Conversion to/from Other Formats
					operator CV_GETS_CONST char *() const;
#ifndef CV_NO_STOL
					operator long() const;
#endif
					operator double() const;
	long			stol() const;
	double			stod() const;
	VString&		toUpper();
	VString&		toLower();
#ifdef CV_WCHAR
					operator const VWideChar *() const;
#endif

	//	Manipulation
	virtual integer	contains(const char *text, integer start_idx = 0) const;
	integer			contains(const VString& text, integer start_idx = 0) const;
#ifdef CV_WCHAR
	virtual integer	contains(const VWideChar *text, integer start_idx = 0) const;
#endif

	virtual boolean	truncAtCharPos(integer idx);
	virtual boolean	padTo(integer idx, char pad = ' ');
	void			setCharSize(integer idx, char pad = ' ');
	virtual boolean	deleteAtCharPos(integer idx, integer count = 1);
	virtual void	insertAtCharPos(const VString& ins, integer idx);
	void			insertAtCharPos(const char *ins, integer idx = 0, integer inssize = -1, char pad = ' ');
	void			insertAtCharPos(const char c, integer idx, char pad = ' ');
	void			replaceAtCharPos(const char c, integer idx, char pad = ' ');
#ifdef CV_WCHAR
	virtual boolean	padTo(integer idx, VWideChar pad);
	void			setCharSize(integer idx, VWideChar pad);
	void			insertAtCharPos(const VWideChar *ins, integer idx, integer inssize, VWideChar pad);
	void			insertAtCharPos(const VWideChar c, integer idx, VWideChar pad);
	void			replaceAtCharPos(const VWideChar c, integer idx, VWideChar pad);
#endif

	boolean			strip();
	boolean			stripTrailing();
	boolean			stripLeading();

	//	Formatting utilities 
	static long		measureFormat(const char *format, va_list arg);
	static char	   *selectFormatBuffer(char *buffer, long buffer_length,
						               VString *sbuffer,
						               const char *format, va_list arg);
#ifdef CV_WCHAR
	static long		measureFormat(const VWideChar *format, va_list arg);
	static VWideChar*selectFormatBuffer(VWideChar *buffer, long buffer_length,
						               VString *sbuffer,
						               const VWideChar *format, va_list arg);
#endif

#ifdef __ultrix
	//	gjl--OFI cats--new function added for AR's convenience!
					operator const char *() const;
#endif

	//	Initialization
	virtual void	reset();
	virtual void	expandTo(integer n);
	boolean			copyFrom(VObject *obj);

	//	String Resources
	int				getResource(unsigned resId);
#ifdef V_MACINTOSH
	void			putResource(unsigned resId);
	void			getNamedResource(const char *resName);
	void			putNamedResource(const char *resName);
#endif

#ifndef CV_NOARCHIVER
	//  Archival and retrieval
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void	getFrom(VArchiver& a, VObject *data,
						    boolean do_realize = TRUE);
#endif

#ifdef PM
	//	for PM - copy string replacing & with ~ . 
	//	Option to replace cr/lf with cr 

	VString&		scanAmper(const char *s, boolean crlf = FALSE);
#endif

#ifdef CV_WINDOWS
	//	for Windows - copy string replacing ~ with & . 
	VString&		scanTilde(const char *s);
#endif


protected:
	void			init();
	virtual integer	virtGets(char *s, integer n) const;
	virtual integer	virtGets(VWideChar *s, integer n) const;

	static int		identifyFormatConversion(const char *s,
						                     char &specifier,
						                     char &modifier,
						                     int &field_width, 
						                     int &precision);
#ifdef CV_WCHAR
	static int		identifyFormatConversion(const VWideChar *s,
						                     VWideChar &specifier,
						                     VWideChar &modifier,
						                     int &field_width, 
						                     int &precision);
#endif

	//	Wide character helpers
	static boolean	atowc(VWideChar& wc, char c);
	static boolean	wctoa(char& c, const VWideChar& wc);
	
	boolean			asMultibyte() const;

	//	Data access
	void			clearData();
	pointer			getData() const;
	void			setData(pointer d);
	void			swapData(VString &s);
	VWideChar	   *getWideData() const;
	void			setWideData(VWideChar *d);
	virtual void	asWideString(boolean) const;
	
	CV_GETS_CONST char& charReferenceAt(integer i);

	const VWideChar*getWideLineTerm();

	integer			nEle;
	unsigned short	asByteFlag:1;
	unsigned short	asWCharFlag:1;
	
private:
	pointer			data;

	friend VWideCharAux;

};

extern VClass *VStringCls;

// ---------------------------------------------------------------------------
// Copy the contents of VString str into this VString and return
// the number of elements copied.
//
inline integer VString::puts(const VString& str)
{
	return puts((VString *)&str);
}

// ---------------------------------------------------------------------------
// Concatenate str onto the end of this VString and return
// the number of elements copied.
//
inline integer VString::concat(const VString& str)
{
	return concat((VString *)&str);
}

// ---------------------------------------------------------------------------
// Add a line-terminator (CR/NL) to this VString.
//
inline integer VString::concatln()
{
	if (asWCharFlag && !CV_CALL_SINGLE_BYTE_GETS) {
		return concat(getWideLineTerm());
	}
	else {
		return concat(LINE_TERM);
	}
}

// ---------------------------------------------------------------------------
// Copy 's' into this VString followed by a line-terminator.
//
inline integer VString::putln(const char *s)
{
	integer i = puts(s);
	i += concatln();
	return i;
}

// ---------------------------------------------------------------------------
// Copy 's' into this VString followed by a line-terminator.
// This added so that 'putln(func_returning_VString())' won't be amibiguous,
// now that 'putln' is overloaded.
//
inline integer VString::putln(const VString& s)
{
	integer i = puts((VString *)&s);
	i += concatln();
	return i;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Copy 's' into this VString followed by a line-terminator.
//
inline integer VString::putln(const VWideChar *s)
{
	if (CV_CALL_SINGLE_BYTE_GETS) {
		VString vs = s;
		putln(vs.gets());
		return VWideCharAux::wcslen(s) + strlen(LINE_TERM);
	}
	integer i = puts(s);
	i += concatln();
	return i;
}
#endif

// ---------------------------------------------------------------------------
//
inline VString& VString::operator=(const VString& s)
{
	if (&s != this) {
		reset();
		if (s.asWCharFlag && !CV_CALL_SINGLE_BYTE_GETS) {
			puts(s.getwcs());
		}
		else {
			puts(s.gets());
		}
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VString& VString::operator=(const char *s)
{
	// Resetting can be skipped as an optimization if this is
	// a plain VString.
	if (getIsA() != VStringCls) {
		reset();
	}
	puts(s);
	return *this;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//
inline VString& VString::operator=(const VWideChar *s)
{
	if (CV_CALL_SINGLE_BYTE_GETS) {
		VString vs = s;
		return operator=(vs.gets());
	}
	puts(s);
	return *this;
}
#endif

// ---------------------------------------------------------------------------
// Return the character at index 'i'. It is up to the caller to ensure that
// 'i' is within the bounds of this string.
//
inline CV_GETS_CONST char& VString::operator[](integer i)
{
	if ((CV_MB_LEN_MAX == 1 || asByteFlag) && !asWCharFlag) {
#if defined(CV_WIN16)
		return data[(int)i];
#else
		return data[i];
#endif
	}
	else {
		return charReferenceAt(i);
	}
}

// ---------------------------------------------------------------------------
// Clear the pointer which this VString uses to represent its value.
//
inline void VString::clearData()
{
	data = 0;
}

// ---------------------------------------------------------------------------
// Return the pointer which this VString uses to represent its value.
// Forces this VString to use a byte-oriented representation,
// unless 'data' is null.
//
inline pointer VString::getData() const
{
	if (asWCharFlag) {
		asWideString(FALSE);
	}
	return data;
}

// ---------------------------------------------------------------------------
// Return the pointer which this VString uses to represent its value.
// Forces this VString to use a byte-oriented representation,
// unless 'data' is null.
//
inline VWideChar *VString::getWideData()  const
{
	if (data != 0 && !asWCharFlag) {
		asWideString(TRUE);
	}
	return *(VWideChar **)&data;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
//
inline boolean VString::operator<=(const VString& s) const
{
	return compareVString(s) <= 0;
}

// ---------------------------------------------------------------------------
//
inline boolean VString::operator>=(const VString& s) const
{
	return compareVString(s) >= 0;
}

// ---------------------------------------------------------------------------
//
inline boolean VString::operator<(const VString& s) const
{
	return compareVString(s) < 0;
}

// ---------------------------------------------------------------------------
//
inline boolean VString::operator>(const VString& s) const
{
	return compareVString(s) > 0;
}
#endif

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are the same as 's'.
inline boolean VString::operator==(const VString& s) const
{
	return compareVString(s) == 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are the same as 's'.
//
inline boolean VString::operator==(const char *s) const
{
	return compareCStr(s) == 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are NO% the same as 's'.
inline boolean VString::operator!=(const VString& s) const
{
	return compareVString(s) != 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are NOT the same as 's'.
//
inline boolean VString::operator!=(const char *s) const
{
	return !(compareCStr(s) == 0);
}

#if 1
#if _MSC_VER >= 700
// ---------------------------------------------------------------------------
//
inline boolean VString::operator==(char *s) const
{
	return compareCStr(s) == 0;
}

// ---------------------------------------------------------------------------
//
inline boolean VString::operator!=(char *s) const
{
	return !(compareCStr(s) == 0);
}
#endif
#endif

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are 
// lexically greater than 's'.
//
inline boolean VString::operator>(const char *s) const
{
	return compareCStr(s) > 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are lexically
// less than 's'.
//
inline boolean VString::operator<(const char *s) const
{
	return compareCStr(s) < 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString are
// lexically greater or equal to 's'.
//
inline boolean VString::operator>=(const char *s) const
{
	return compareCStr(s) >= 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if the contents of this VString
// are lexically less or equal to 's'.
//
inline boolean VString::operator<=(const char *s) const
{
	return compareCStr(s) <= 0;
}

// ---------------------------------------------------------------------------
//
inline int VString::operator!() const
{
	if (getIsA() != VStringCls) {
		return size() == 0;
	}

	// Check efficiently, if it's a VString.
	if (data == 0) {
		return TRUE;
	}
	return asWCharFlag ? (getWideData()[0] == 0) : (data[0] == 0);
}

// ---------------------------------------------------------------------------
// Concatenate VString 's' onto the end of this VString.
//
inline VString& VString::operator+=(const VString& s)
{
	if (s.asWCharFlag && !CV_CALL_SINGLE_BYTE_GETS) {
		concat(s.getwcs());
	}
	else {
		concat(s.gets());
	}
	return *this;
}

// ---------------------------------------------------------------------------
// Concatenate 's' onto the end of this VString.
//
inline VString& VString::operator+=(const char *s)
{
	concat(s);
	return *this;
}

// ---------------------------------------------------------------------------
// Concatenate 'c' onto the end of this VString.
//
inline VString& VString::operator+=(char c)
{
	concat(c);
	return *this;
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Concatenate 's' onto the end of this VString.
//
inline VString& VString::operator+=(const VWideChar *s)
{
	if (CV_CALL_SINGLE_BYTE_GETS) {
		VString vs = s;
		return operator+=(vs.gets());
	}
	concat(s);
	return *this;
}

// ---------------------------------------------------------------------------
// Concatenate 'c' onto the end of this VString.
//
inline VString& VString::operator+=(VWideChar c)
{
	concat(c);
	return *this;
}
#endif

// ---------------------------------------------------------------------------
// Synonym for gets().
//
inline const char *VString::gets(char) const
{
	return gets();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Synonym for getwcs().
//
inline const VWideChar *VString::gets(VWideChar) const
{
	return getwcs();
}
#endif

// ---------------------------------------------------------------------------
// Synonym for gets().
//
inline VString::operator CV_GETS_CONST char *() const
{
	return gets();
}

#ifdef CV_WCHAR
// ---------------------------------------------------------------------------
// Synonym for getwcs().
//
inline VString::operator const VWideChar *() const
{
	return getwcs();
}
#endif

// ---------------------------------------------------------------------------
// Return the contents of this VString converted to a double.
//
inline VString::operator double() const
{
#ifdef WCHAR_H_EXISTS
	if (!CV_CALL_SINGLE_BYTE_GETS && asWCharFlag) {
		return wcstod(getwcs(), 0);
	}
#endif
	return atof(gets());
}

// ---------------------------------------------------------------------------
//
inline long VString::stol() const
{
#ifdef WCHAR_H_EXISTS
	if (!CV_CALL_SINGLE_BYTE_GETS && asWCharFlag) {
		return wcstol(getwcs(), 0, 10);
	}
#endif
	return atol(gets());
}

#ifndef CV_NO_STOL
// ---------------------------------------------------------------------------
// Return the contents of this VString converted to a long.
//
inline VString::operator long() const
{
	return stol();
}
#endif

// ---------------------------------------------------------------------------
//
inline double VString::stod() const
{
	return operator double();
}

// ---------------------------------------------------------------------------
//
#ifdef __ultrix
inline VString::operator const char *() const
{
	VString tmp(*this);
	return (const char *)tmp.gets();
}
#endif // __ultrix
	
// ---------------------------------------------------------------------------
// Return whether the VString is to be interpreted as multibyte characters.
//
inline boolean VString::asMultibyte() const
{
	return !asByteFlag && !asWCharFlag;
}
	
// ---------------------------------------------------------------------------
// Return whether the VString is to be interpreted as byte-size characters.
//
inline boolean VString::isAsByte() const
{
	return asByteFlag && !asWCharFlag;
}
	
// ---------------------------------------------------------------------------
// Return whether the VString is represented using an array of VWideChar.
// If FALSE is returned, it is represented using bytes.
//
inline boolean VString::isAsWide() const
{
	return asWCharFlag;
}

// ---------------------------------------------------------------------------
//
inline boolean VString::atowc(VWideChar& wc, char c)
{
	return VWideCharAux::atowc(wc, c);
}

// ---------------------------------------------------------------------------
//
inline boolean VString::wctoa(char& c, const VWideChar& wc)
{
	return VWideCharAux::wctoa(c, wc);
}

#endif // VSTRING_H
