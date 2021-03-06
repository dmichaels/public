// $Header:   Q:/views/common/vcs/STYLEXPR.H_V   1.12   Mar 24 1997 16:13:40   scottt  $

//	stylexpr.h
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
//	10/10/95 tomf	Declared setTransient static.
//	12/06/95 dss	name changes: VSTYLEEXPRESSION_H, VStyleExpression.
//					Added operator |.
//	12/15/95 dss	CV_EXPORT_FUNC() hack for borland.
//
//	02/04/97 pkt	preened.
//	02/14/97 tlf	Modified to compile on HPUX and AIX.
//	02/19/97 pkt	modified to work with any compiler.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef	VSTYLEEXPRESSION_H
#define VSTYLEEXPRESSION_H

#include <memory.h>
#include "defs.h"
#include "cvassert.h"

#include "implstyl.h"

#define INITIAL_COLLECTION_SIZE 10

class VStyleExpression;

typedef VStyleExpression& (*StyleManip)(VStyleExpression&, boolean);

struct StyleOperand;
// NOTE: !!! should you derive this class from VStyle, make sure you
// call the superclass where appropriate, eg CTOR's, operator=, etc.

struct StyleOperand {
	VStyle				*style;
	VStyle::StyleOperation		operation;
	boolean				transient;

	VStyle&				operator()(VStyle& client, VStyle& arg);
};

// VStyleExpression Manipulator
static VStyleExpression& setTransient(VStyleExpression& sc, boolean transient);


class VStyleExpression {
public:
	friend VStyleExpression& setTransient(VStyleExpression&, boolean);

	VStyleExpression();
	VStyleExpression(const VStyle& s) ;
	VStyleExpression(const VStyleExpression& s);

	~VStyleExpression();
	VStyleExpression& operator=(const VStyleExpression& s);

	void init(int size = INITIAL_COLLECTION_SIZE);

	void 	clear(void);
	boolean	isEmpty(void);

	//!!!dss add dynamic sizing
	void 	add(const VStyle& s, VStyle::StyleOperation operation);

	boolean remove(const VStyle& s);

	VStyle* first(void);
	VStyle* next(void);
	VStyle* prev(void);
	VStyle* last(void);
	VStyle* current(void);

	const VImplicitStyle&	getResult(void) const;
	const VImplicitStyle		getPersistent(VStyle::StyleOperation) const;

	VStyleExpression& operator+(const VStyle& s);
	VStyleExpression& operator-(const VStyle& s);

	VStyleExpression& operator+(StyleManip manip);
	VStyleExpression& operator-(StyleManip manip);

	VStyleExpression& operator=(const VStyle& s);
	VStyleExpression& operator+=(const VStyle& s);
	VStyleExpression& operator-=(const VStyle& s);

	operator const VStyle&();
	operator const VExclusiveStyle&();
	operator const VImplicitStyle&();

protected:
	void	evaluate(void);

protected:
	StyleOperand	*expression;
	int				numElements;
	StyleOperand	data[INITIAL_COLLECTION_SIZE];

	int				nextSlot;
	int				operand;

	boolean			dirty;
	VImplicitStyle	result;

	boolean			transient;
};

#undef CV_IMPORT_FUNC

// dss 12/15/95 - this is a temp hack for borland PM. All compilers 
// should support CV_IMPORT_FUNC using CV_DLL_IMPORT and extern.
#if defined(__BCPLUSPLUS__) && defined(PM)
	#define CV_IMPORT_FUNC(type, func) extern type func
#else
	#define CV_IMPORT_FUNC(type, func) type func
#endif

CV_IMPORT_FUNC(VStyleExpression, operator+(const VStyle& a, const VStyle& b));
CV_IMPORT_FUNC(VStyleExpression, operator-(const VStyle& a, const VStyle& b));

CV_IMPORT_FUNC(VStyleExpression, operator+(StyleManip, const VStyle&));
CV_IMPORT_FUNC(VStyleExpression, operator+(const VStyle&, StyleManip));
CV_IMPORT_FUNC(VStyleExpression, operator-(StyleManip, const VStyle&));
CV_IMPORT_FUNC(VStyleExpression, operator-(const VStyle&, StyleManip));

// for backward compatibility.
inline VStyleExpression operator|(const VStyle& a, const VStyle& b) {return operator+(a, b);}
inline VStyleExpression operator|(StyleManip m, const VStyle& a) {return operator+(m, a);}
inline VStyleExpression operator|(const VStyle& a, StyleManip m) {return operator+(a, m);}

#undef CV_IMPORT_FUNC

// VStyleExpression Manipulator
inline VStyleExpression& setTransient(VStyleExpression& sc, boolean transient)
{
	sc.transient = transient;
	return sc;
}

#ifndef CVS_PROTECT_MODS

inline VStyle& StyleOperand::operator()(VStyle& client, VStyle& arg)
{
	if (operation == VStyle::setOp) {
		client.set(arg);
	}
	else if (operation == VStyle::addOp) {
		client.add(arg);
	}
	else if (operation == VStyle::removeOp) {
		client.remove(arg);
	}
	else {
		cv_assert(FALSE);
	}

	return client;
}

// ---------------------------------------------------------------------------
//
inline void VStyleExpression::clear(void)
{
	dirty = nextSlot = operand = 0;
}

// ---------------------------------------------------------------------------
//
inline void VStyleExpression::init(int size)
{
	expression = data; 
	numElements = size; 
	transient = FALSE;
	clear();
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::VStyleExpression()
{
	init();
}

// ---------------------------------------------------------------------------
//
//!!!dss add dynamic sizing
inline void VStyleExpression::add(const VStyle& s, VStyle::StyleOperation operation)
{
	// remove this cv_assert when dynamic sizing is implemented
	cv_assert(nextSlot < numElements);
	expression[nextSlot].style = (VStyle*)&s;
	expression[nextSlot].transient = transient;
	expression[nextSlot++].operation = operation;
	dirty = TRUE;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::VStyleExpression(const VStyle& s) 
{
	//!!!dss: an optimization: size the data array based on s.numElements
	// so add doesn't have to do it. On the other hand, add is doing
	// it already.
	init();
	add( s, VStyle::setOp);
}
			
// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator=(const VStyleExpression& s)
{
	nextSlot = s.nextSlot;
	operand = s.operand;

	// remove this cv_assert when dynamic sizing is implemented
	cv_assert(s.nextSlot < numElements);
	memcpy(data, s.data, s.nextSlot * sizeof(data[0]));
	result = s.result;
	dirty = s.dirty;
	transient = s.transient;
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::VStyleExpression(const VStyleExpression& s)
{
	init();
	*this = s;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::~VStyleExpression()
{
	if (expression != data) {
		delete expression;
	}
}

// ---------------------------------------------------------------------------
//
inline boolean VStyleExpression::isEmpty(void)
{
	return nextSlot == 0;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator+(const VStyle& s) 
{
	if (nextSlot == 0) {
		add(s, VStyle::setOp);
	}
	else {
		add(s, VStyle::addOp);
	}
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator-(const VStyle& s)
{
	add(s, VStyle::removeOp); 
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator+(StyleManip manip)
{
	return manip(*this, TRUE);
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator-(StyleManip manip)
{
	return manip(*this, FALSE);
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator=(const VStyle& s)
{
	clear();
	add(s, VStyle::setOp);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator+=(const VStyle& s)
{
	add(s, VStyle::addOp);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression& VStyleExpression::operator-=(const VStyle& s)
{
	add(s, VStyle::removeOp);
	return *this;
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::operator const VStyle&()
{
	return getResult();
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::operator const VExclusiveStyle&()
{
	return (VExclusiveStyle&) getResult();
}

// ---------------------------------------------------------------------------
//
inline VStyleExpression::operator const VImplicitStyle&()
{
	return (VImplicitStyle&) getResult();
}

#endif

#endif // VSTYLEEXPRESSION_H
