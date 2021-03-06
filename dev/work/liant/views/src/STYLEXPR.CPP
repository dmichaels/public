// $Header:   Q:/views/common/vcs/STYLEXPR.CPV   1.8   Mar 24 1997 16:13:38   scottt  $

//	stylexpr.cpp
//
//	Style Expression Class [Common]
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
//	10/02/95 dss	StyleOperation now enum and moved in class VStyle.
//	12/05/95 dss	name change: VStyleExpression.
//	12/15/95 dss	CV_EXPORT_FUNC() hack for borland.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "object.h"
#include "archiver.h"

#include "cvassert.h"

#include "bitset.h"
#include "style.h"
#include "exclstyl.h"
#include "implstyl.h"

#include "stylexpr.h"

// dss 12/15/95 - this is a temp hack for borland PM. All compilers 
// should support CV_EXPORT_FUNC using CV_DLL_EXPORT and extern.
#if defined(__BCPLUSPLUS__) && defined(PM)
	#define CV_EXPORT_FUNC(type, func) extern type CV_DLL_EXPORT func
#else
	#define CV_EXPORT_FUNC(type, func) type func
#endif

CV_EXPORT_FUNC( VStyleExpression, operator+(StyleManip manip, const VStyle& s))
{
	VStyleExpression	tmp;
	manip(tmp, TRUE);
	tmp.add(s, VStyle::setOp);
	return tmp;
}

CV_EXPORT_FUNC( VStyleExpression, operator+(const VStyle& s, StyleManip manip))
{
	VStyleExpression	tmp(s);
	return manip(tmp, TRUE);
}

CV_EXPORT_FUNC( VStyleExpression, operator-(StyleManip manip, const VStyle& s))
{
	VStyleExpression	tmp;
	manip(tmp, TRUE);
	tmp.add(s, VStyle::removeOp);
	return tmp;
}

CV_EXPORT_FUNC( VStyleExpression, operator-(const VStyle& s, StyleManip manip))
{
	VStyleExpression	tmp(s);
	return manip(tmp, FALSE);
}

CV_EXPORT_FUNC( VStyleExpression, operator+(const VStyle& a, const VStyle& b))
{
	VStyleExpression tmp(a);
	return tmp += b;
}

CV_EXPORT_FUNC( VStyleExpression, operator-(const VStyle& a, const VStyle& b))
{
	VStyleExpression tmp(a);
	return tmp -= b;
}

//!!!dss add dynamic sizing
boolean VStyleExpression::remove(const VStyle& s)
{
	for (int i = 0; i < nextSlot; i++) {
		if (&s == expression[i].style) {
			// memmove() would be more efficient
			for (int j = i; j < nextSlot - 1; j++) {
				expression[j] = expression[j + 1];
			}
			nextSlot--;
			dirty = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

VStyle* VStyleExpression::first(void)
{
	operand = 0;
	return expression[0].style; 
}

VStyle* VStyleExpression::next(void)
{
	if (operand < nextSlot) 
		return expression[++operand].style;
	else
		return 0;
}

VStyle* VStyleExpression::prev(void)
{
	if (operand > 0) 
		return expression[--operand].style;
	else
		return 0;
}

VStyle* VStyleExpression::last(void)
{
	if (nextSlot > 0)
		return expression[nextSlot - 1].style;
	else
		return 0;
}

VStyle* VStyleExpression::current(void)
{
	return expression[operand].style;
}

void VStyleExpression::evaluate(void)
{
	if (!dirty)
		return;

	for (StyleOperand *o = expression; o < expression + nextSlot; o++) {
		(*o)(result, *o->style);
	}

	dirty = FALSE;
}

const VImplicitStyle& VStyleExpression::getResult(void) const
{
	if (dirty)
		// evaluate's not const but this method should appear to be
		// as its just a get method.
		((VStyleExpression*)this)->evaluate();

	return result;
}

const VImplicitStyle VStyleExpression::getPersistent(VStyle::StyleOperation operation) const
{
	VImplicitStyle	tmp;

	if (operation == VStyle::addOp) {
		for (StyleOperand *o = expression; o < expression + nextSlot; o++) {
			if (!o->transient && 
				 (o->operation == VStyle::addOp || o->operation == VStyle::setOp))
				tmp.add(*o->style);
		}
		return tmp;
	}
	else if (operation == VStyle::removeOp) {
		for (StyleOperand *o = expression; o < expression + nextSlot; o++) {
			if (!o->transient && o->operation == VStyle::removeOp)
				tmp.remove(*o->style);
		}
		return tmp;
	}

	cv_assert(operation == VStyle::addOp || operation == VStyle::removeOp);

	return tmp;
}
