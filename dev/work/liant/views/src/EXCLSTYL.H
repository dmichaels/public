// $Header:   Q:/views/common/vcs/EXCLSTYL.H_V   1.9   Mar 24 1997 16:08:52   scottt  $ 

//	exclstyl.h
//
//	Exclusive Style class interface [Common]
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
//	09/01/95 dss	created.
//	09/20/95 dss	ambigous ctor bug fix.
//	09/20/95 dss	Borland bug fixes.
//
//	02/04/97 pkt	Preened.
//	02/04/97 pkt	Removed 'creation'.
//	02/14/97 tlf	Modified to compile on HPUX and AIX.
//	02/19/97 pkt	Modified to compile with any compiler.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#if !defined(_EXCLSTYLE_H_)
#define _EXCLSTYLE_H_

#include "style.h"

CLASS VExclusiveStyle : public VStyle {

public:

	VExclusiveStyle(char *a = "?"); 
	VExclusiveStyle(const VExclusiveStyle& a);
	VExclusiveStyle(int n, char *a);
	VExclusiveStyle(int n, int creationStyle = FALSE, char *a = "?");

	VClass		*iam();

#ifdef CVS_PROTECT_MODS
protected:
#endif
	void	clear(void);

	void	set(const VExclusiveStyle& a);
	void	add(const VExclusiveStyle& a);
	void	remove(const VExclusiveStyle& a);

	void	set(const VStyle& a);
	void	add(const VStyle& a);
	void	remove(const VStyle& a);

	void	set(int n);
#ifdef CVS_PROTECT_MODS
public:
#endif

	static void	VExclusiveStyle::setExclusive(class VExclusiveStyle *a[]);

#if defined(V_UNIT_TEST)
	void dump(ostream&) const;
#endif

#ifdef CVS_PROTECT_MODS
protected:
#endif
	VExclusiveStyle& operator = (const VExclusiveStyle& a);
	VExclusiveStyle& operator += (const VExclusiveStyle& a);
	VExclusiveStyle& operator -= (const VExclusiveStyle& a);

	VExclusiveStyle& operator = (const VStyle& a);
	VExclusiveStyle& operator += (const VStyle& a);
	VExclusiveStyle& operator -= (const VStyle& a);
#ifdef CVS_PROTECT_MODS
public:
#endif

#ifndef CV_NOARCHIVER
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *data, boolean do_realize = TRUE);
	void		putTo(VArchiver& a);
#endif

protected:
	VStyle		exclude;
#ifndef CVS_SMALL
	VStyle		creation;
#endif
};

typedef void (VExclusiveStyle::*ExclusiveStyleOperation)(const VExclusiveStyle &);

extern VClass *VExclusiveStyleCls;

#ifndef CVS_PROTECT_MODS

// ---------------------------------------------------------------------------
//
inline void	VExclusiveStyle::clear(void)
{
	VStyle::clear();
	exclude.clear();
#ifndef CVS_SMALL
	creation.clear();
#endif
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle::VExclusiveStyle(char *a)
	: VStyle(a)
	, exclude("exclude")
#ifndef CVS_SMALL
	, creation("creation")
#endif
{
	clear();
} 

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle::VExclusiveStyle(const VExclusiveStyle& a)
	: VStyle(a)
	, exclude("exclude")
#ifndef CVS_SMALL
	, creation("creation")
#endif 
{
	exclude = a.exclude;
#ifndef CVS_SMALL
	creation = a.creation;
#endif
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle::VExclusiveStyle(int n, char *a) 
	: VStyle(n, a)
	, exclude("exclude")
#ifndef CVS_SMALL
	, creation("creation")
#endif
{
	;
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle::VExclusiveStyle(int n, int creationStyle, char *a) 
	: VStyle(n, a)
	, exclude("exclude")
#ifndef CVS_SMALL
	, creation("creation")
#endif
{
	if (creationStyle) {
#ifndef CVS_SMALL
		creation.set(n);
#endif
	}
}

// ---------------------------------------------------------------------------
//
inline void	VExclusiveStyle::set(int n)
{
	VStyle::set(n);
}


// ---------------------------------------------------------------------------
//
inline VExclusiveStyle& VExclusiveStyle::operator = (const VExclusiveStyle& a)
{
	set(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle& VExclusiveStyle::operator += (const VExclusiveStyle& a)
{
	add(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle& VExclusiveStyle::operator -= (const VExclusiveStyle& a)
{
	remove(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle& VExclusiveStyle::operator = (const VStyle& a)
{
	set(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle& VExclusiveStyle::operator += (const VStyle& a)
{
	add(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VExclusiveStyle& VExclusiveStyle::operator -= (const VStyle& a)
{
	remove(a);
	return *this;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
//
inline void VExclusiveStyle::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VStyle::getFrom(a, data, do_realize);
}
#endif

#endif // CVS_PROTECT_MODS

#endif	//  _EXCLSTYLE_H_

