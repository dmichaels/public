// $Header:   Q:/views/common/vcs/VLOCALE.H_V   1.10   Mar 24 1997 16:14:24   scottt  $

//	vlocale.h
//
//	VLocale class interface [Common]
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
//	10/21/95 pkt	added platform-specific #includes.
//	10/21/95 pkt	added 'id' and 'fromId'.
//	11/30/95 dgm	added GetLocaleInfo #define for 16-bit Borland C++.
//	12/06/95 pkt	fixed UNKNOWN_LOCALE for non-MS-Windows platforms.
//	12/06/95 pkt	changed include of Xlocale.h not to have trailing comment.
//	01/18/96 pkt	changed to prevent MSVC 1.5 from choking on the include
//					of Xlocale.h during its IDE's make process.
//	01/31/96 pkt	added 'description'. 
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VLOCALE_H
#define VLOCALE_H

#include "object.h"

CLASS VString;

// ---------------------------------------------------------------------------
// VPlatformLocale, the native representation of a locale
// with a peculiar value, UNKNOWN_LOCALE
//
#define Xlocale "X11/Xlocale.h"
#if defined(CV_WINDOWS)
# if defined(CV_WIN32)
#  include <winnls.h>
# endif
# if defined(CV_WIN16)
#  include <olenls.h>
#   if defined(__BCPLUSPLUS__) && !defined(GetLocaleInfo)
#     define GetLocaleInfo GetLocaleInfoA
#   endif
# endif
  typedef LCID VPlatformLocale;
#elif defined(PM)
  typedef ULONG VPlatformLocale;
#elif defined(MOTIF)
  // for 'setlocale' on any X system
# include Xlocale
  typedef const char *VPlatformLocale;
#else
  typedef ? VPlatformLocale;
#endif
#define UNKNOWN_LOCALE ((VPlatformLocale)0)

// ---------------------------------------------------------------------------
// A VLocale represents a language/region/culture for which software may need
// to be specialized.  A VLocale does not itself indicate a way of encoding
// characters.  For instance the Japan locale would be consistent with code
// page	932, or Shift-JIS, or EUC, or Unicode.  It would not imply any of
// these encodings.
//
// VLocales are managed based on the ISO 639 codes for designating languages
// and the ISO 3166 codes for territories.  For example, en, de, ja, en_US,
// fr_CA.
//
CLASS VLocale : public VObject {

public:
	// constructors/destructor

					VLocale();
					VLocale(const VString& std_code);
					~VLocale();

	VLocale			language() const;
	VLocale			region() const;
	boolean			isValid() const;
	const char	   *gets() const;
	VString			description() const;

	static VLocale	current();

	//	Equivalence
	boolean 		isEqual(VObject *obj) const;
	HashKey 		hash() const;
	int 			compare(VObject *obj) const;

	//	Other overrides to support VObject behavior
	virtual void	reset();
	virtual boolean	copyTo(VObject *obj);
	virtual boolean	copyFrom(VObject *obj);

	// Platform-dependent
	static VLocale	fromId(VPlatformLocale id);
	VPlatformLocale	id() const;

#ifndef CV_NOARCHIVER
	// Archival
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

	VClass		   *iam();
	
protected:
	void			init(const VString& std_code);
	void			init();

	char			code[8];
};

extern VClass *VLocaleCls;

#endif	//	VLOCALE_H
