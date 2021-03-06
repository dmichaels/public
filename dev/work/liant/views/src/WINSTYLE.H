// $Header:   Q:/views/common/vcs/WINSTYLE.H_V   1.13   Mar 24 1997 16:14:36   scottt  $

//  winstyle.h
//
//	Window Style Class [Common]
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	09/15/92 jmd	added StyleReadOnly
//	05/24/93 jmd	preened, added StyleShadow
//	03/24/94 pat	auto defined CV_WINDOWS under CV_WIN32
//	03/25/94 pat	cast styles to short for Visual C++ for NT
//	07/14/94 jld	added defs.h dependency for CV_WINDOWS
//	07/19/94 jld	added StyleEditSecure
//	05/25/95 pkt	merged for common source.
//	07/03/95 dgm	Merged in Intersolv changes: Style3D
//	09/01/95 dss	Moved contents to styles.h because VWinStyle was created
//					and I needed the filename.
//	09/01/95 dss	VImplicitStyle changes.
//	09/20/95 dss	Borland bug fixes.
//	09/26/95 dss	Move ctors w/class args before ctor's with basic types
//					as arg. Replace ctor w/VImplictStyle w/ctor w/VStyle.
//	12/06/95 dss	name change: VStyleExpression
//	02/09/96 dgm	Added isPersistent() and isTransient().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef	WINSTYLE_H
#define WINSTYLE_H

#include "implstyl.h"

CLASS VStyleExpression;

CLASS VWinStyle : public VImplicitStyle
{
public:
    VWinStyle() : VImplicitStyle(), persistent("persistent") {;}

    VWinStyle(const VWinStyle& s) : VImplicitStyle(s),  persistent("persistent") {;}
    VWinStyle(const VStyle& s) : persistent("persistent") {set(s);}

    VWinStyle(char *a) : VImplicitStyle(a), persistent("persistent") {;}
    VWinStyle(int n) : VImplicitStyle(n, "?"),  persistent("persistent") {;}
    VWinStyle(int n, char *a) : VImplicitStyle(n, a),  persistent("persistent") {;}

    VClass      *iam();

#ifdef CVS_PROTECT_MODS
protected:
#endif
    void    set(const VWinStyle& a);
    void    add(const VWinStyle& a);
    void    remove(const VWinStyle& a);

    void    set(const VStyle& a);
    void    add(const VStyle& a);
    void    remove(const VStyle& a);

    void    set(const VStyleExpression& a);
    void    add(const VStyleExpression& a);
    void    remove(const VStyleExpression& a);

    void    set(int n) {VImplicitStyle::set(n);}
#ifdef CVS_PROTECT_MODS
public:
#endif

#if defined(V_UNIT_TEST)
    void dump(ostream&) const;
#endif

#ifdef CVS_PROTECT_MODS
protected:
#endif
    // single style operand operations are persistent by default.
    VWinStyle& operator = (const VImplicitStyle& s) {set(s); return *this;}
    VWinStyle& operator += (const VImplicitStyle& s) {add(s); return *this;}
    VWinStyle& operator -= (const VImplicitStyle& s) {remove(s); return *this;}

    VWinStyle& operator = (const VStyle& s) {set(s); return *this;}
    VWinStyle& operator += (const VStyle& s) {add(s); return *this;}
    VWinStyle& operator -= (const VStyle& s) {remove(s); return *this;}

    // style VStyleExpression operations contain persistence info.
    VWinStyle& operator = (const VStyleExpression& a) {set(a); return *this;}
    VWinStyle& operator += (const VStyleExpression& a) {add(a); return *this;}
    VWinStyle& operator -= (const VStyleExpression& a) {remove(a); return *this;}
#ifdef CVS_PROTECT_MODS
public:
#endif

	boolean		isPersistent(const VStyle&) const;
	boolean		isTransient(const VStyle&) const;

#ifndef CV_NOARCHIVER
    void        getFrom(VArchiver& a);
    void        getFrom(VArchiver& a, VObject *data, boolean do_realize = TRUE)
                {VImplicitStyle::getFrom(a, data, do_realize);}
    void        putTo(VArchiver& a);
#endif

protected:
    VImplicitStyle  persistent;
};

// ----------------------------------------------------------------------------
//
inline boolean VWinStyle::isPersistent(const VStyle& style) const
{
	return persistent.contains(style);
}

inline boolean VWinStyle::isTransient(const VStyle& style) const
{
	return contains(style) && !persistent.contains(style);
}

#endif  //  WINSTYLE_H
