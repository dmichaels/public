// $Header:   Q:/views/common/vcs/STYLE.H_V   1.11   Mar 24 1997 16:13:36   scottt  $ 

//	style.cpp
//
//	Exclusive Style Class [Common]
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
//	09/20/95 dss	Borland bug fixes.
//	10/02/95 dss	StyleOperation changed to StyleMethod. enum StyleOperation 
//						in class VStyle.
//	11/13/95 dss	VBitSet name change.
//
//	02/04/97 pkt	preened.
//	02/04/97 pkt	removed 'name'.
//	02/04/97 pkt	added #define name support for 4 levels of improvements to
//					style usage in the library.
//	02/14/97 pkt	removed the default enablement of the "fancy new" stuff.
//					This should be reinstated after 1.1 is out the door.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef STYLE_H
#define STYLE_H

// options for various style changes
#ifndef CVS_NO
// #define CVS_PROTECT_MODS     // to track down places where styles are set
		#define CVS_SMALL            // eliminate unused 'name', 'creation'
# if 0
# ifndef CVS_NO_LATER_DEFAULT
		#define CVS_LATER_DEFAULT    // process defaults at realization time rather than init time
#  ifndef CVS_NO_USE_TRANSIENT
		#define CVS_USE_TRANSIENT    // use setTransient when appropriate
#	ifndef CVS_NO_RATIONAL_DEFAULT
		#define CVS_RATIONAL_DEFAULT // use rational logic in VAppView::getInfo and VDialog::getInfo
#	endif
#  endif
# endif
		#define CVS_DYNAMIC          // track dynamic styles such as StyleAutoToggle
# endif
#endif

#include "bitset.h"

CLASS VStyle : public VObject
{
public:
	enum StyleOperation {setOp, addOp, removeOp};

	VStyle(char *a = "?");
	VStyle(int n, char *a = "?");
	VStyle(const VStyle& a);

	VClass		*iam();

#ifdef CVS_PROTECT_MODS
protected:
#endif
	virtual void	clear(void);
#ifdef CVS_PROTECT_MODS
public:
#endif
	boolean			isEmpty(void) const;

#ifdef CVS_PROTECT_MODS
protected:
#endif
	virtual void	set(const VStyle& a);
	virtual void	add(const VStyle& a);
	virtual void	remove(const VStyle& a);

	virtual void	set(int n);
	virtual void	reset(int n);

	virtual void 	reset();
	
#ifdef CVS_PROTECT_MODS
public:
#endif
	virtual boolean	contains(const VStyle& a) const;

#if defined(V_UNIT_TEST)
	virtual void dump(ostream&) const;
#endif

#ifdef CVS_PROTECT_MODS
protected:
#endif
	VStyle& operator = (const VStyle& a);
	VStyle& operator += (const VStyle& a);
	VStyle& operator -= (const VStyle& a);
#ifdef CVS_PROTECT_MODS
public:
#endif

	boolean operator==(const VStyle& a) const;

#ifndef CV_NOARCHIVER
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *data, boolean do_realize = TRUE);
	void		putTo(VArchiver& a);
#endif

protected:

	VBitSet			bitSet;
#ifndef CVS_SMALL
	char				*name;
#endif

};

typedef	(VStyle::*StyleMethod)(const VStyle&);

extern VClass *VStyleCls;

// ---------------------------------------------------------------------------
//
inline VStyle::VStyle(char *a)
{
#ifndef CVS_SMALL
	name = a;
#endif
}

// ---------------------------------------------------------------------------
//
inline VStyle::VStyle(int n, char *a)
	: bitSet(n)
{
#ifndef CVS_SMALL
	name = a;
#endif
}

// ---------------------------------------------------------------------------
//
inline VStyle::VStyle(const VStyle& a)
	: bitSet(a.bitSet)
{
#ifndef CVS_SMALL
	name = a.name;
#endif
}

// ---------------------------------------------------------------------------
//
inline void	VStyle::clear(void)
{
	bitSet.clear();
}

// ---------------------------------------------------------------------------
//
inline boolean VStyle::isEmpty(void) const
{
	return bitSet.isEmpty();
}

// ---------------------------------------------------------------------------
//
inline void	VStyle::set(int n)
{
	bitSet.set(n);
}

// ---------------------------------------------------------------------------
//
inline void	VStyle::reset(int n)
{
	bitSet.reset(n);
}

// ---------------------------------------------------------------------------
//
inline void VStyle::reset()
{
	VObject::reset();
}

// ---------------------------------------------------------------------------
//
inline VStyle& VStyle::operator = (const VStyle& a)
{
	set(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyle& VStyle::operator += (const VStyle& a)
{
	add(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyle& VStyle::operator -= (const VStyle& a)
{
	remove(a);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline boolean VStyle::operator==(const VStyle& a) const
{
	return bitSet == a.bitSet;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
//
inline void VStyle::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
	VObject::getFrom(a, data, do_realize);
}
#endif

#endif	// _STYLE_H_

