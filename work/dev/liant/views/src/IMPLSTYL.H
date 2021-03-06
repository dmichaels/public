// $Header:   Q:/views/common/vcs/IMPLSTYL.H_V   1.8   Mar 24 1997 16:09:22   scottt  $

//	implstyl.h
//
//	Implicit Style [Common]
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
//	09/26/95 dss	convertOldToNew() arg change.
//	12/06/95 dss	name change: VStyleExpression, VIMPLSTYLE_H.
//
//	02/03/97 pkt	preened.
//	02/03/97 pkt	updated 'implicit' to be a pointer, allocating as needed.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VIMPLSTYLE_H
#define VIMPLSTYLE_H

#include "exclstyl.h"

CLASS VStyleExpression;
CLASS VWindow;

CLASS VImplicitStyle : public VExclusiveStyle {
public:

	VImplicitStyle() : VExclusiveStyle(), implicit(0) {;}

	VImplicitStyle(char *a) : VExclusiveStyle(a), implicit(0) {;} 

	VImplicitStyle(const VExclusiveStyle& a) : VExclusiveStyle(a), implicit(0) {;}

	VImplicitStyle(int n, char *a) 
		: VExclusiveStyle(n, FALSE, a), implicit(0) {;}

	VImplicitStyle(int n, int creationStyle = FALSE, char *a = "?") 
		: VExclusiveStyle(n, creationStyle, a), implicit(0) {;}

				~VImplicitStyle();

	VClass		*iam();

	static void VImplicitStyle::setExclusive(class VImplicitStyle **a);
	static void convertOldToNew(short oldStyle, VWindow& win);

#if defined(V_UNIT_TEST)
	void dump(ostream&) const;
#endif

#ifdef CVS_PROTECT_MODS
protected:
#endif
	void		VImplicitStyle::setImplicit(const class VImplicitStyle **a);
	void		VImplicitStyle::addImplicit(const class VImplicitStyle **a);
	void		VImplicitStyle::removeImplicit(const class VImplicitStyle **a);
	void		VImplicitStyle::setImplicit(const class VImplicitStyle& a);
	void		VImplicitStyle::addImplicit(const class VImplicitStyle& a);
	void		VImplicitStyle::removeImplicit(const class VImplicitStyle& a);

	void	set(const VImplicitStyle& a);
	void	add(const VImplicitStyle& a);
	void	remove(const VImplicitStyle& a);

	void	set(const VStyle& a);
	void	add(const VStyle& a);
	void	remove(const VStyle& a);

	void	set(const VStyleExpression& a);
	void	add(const VStyleExpression& a);
	void	remove(const VStyleExpression& a);

	void	set(int n) {VExclusiveStyle::set(n);}

	VImplicitStyle& operator = (const VImplicitStyle& a) {set(a); return *this;}
	VImplicitStyle& operator += (const VImplicitStyle& a) {add(a); return *this;}
	VImplicitStyle& operator -= (const VImplicitStyle& a) {remove(a); return *this;}

	// commented out because compiler says its ambiguous (???)
	VImplicitStyle& operator = (const VStyle& a) {set(a); return *this;}
	VImplicitStyle& operator += (const VStyle& a) {add(a); return *this;}
	VImplicitStyle& operator -= (const VStyle& a) {remove(a); return *this;}

	VImplicitStyle& operator = (const VStyleExpression& a)  {set(a); return *this;}
	VImplicitStyle& operator += (const VStyleExpression& a) {add(a); return *this;}
	VImplicitStyle& operator -= (const VStyleExpression& a) {remove(a); return *this;}
#ifdef CVS_PROTECT_MODS
public:
#endif

#ifndef CV_NOARCHIVER
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *data, boolean do_realize = TRUE)
				{VExclusiveStyle::getFrom(a, data, do_realize);}
	void		putTo(VArchiver& a);
#endif

protected:
	VExclusiveStyle	   *implicit;
};

#if 0 // !defined(USE_EXPRESSIONS)
VImplicitStyle operator+(const VImplicitStyle& a, const VImplicitStyle& b);
VImplicitStyle operator-(const VImplicitStyle& a, const VImplicitStyle& b);
#endif

typedef void (VImplicitStyle::*ImplicitStyleOperation)(const VImplicitStyle &);

extern VClass *VImplicitStyleCls;

#endif	//  VIMPLSTYLE_H

