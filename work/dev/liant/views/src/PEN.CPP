// $Header:   Q:/views/win/vcs/pen.cpv   1.8   Mar 04 1997 17:55:30   gregm  $ 

//	pen.cpp
//
//	VPen implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	11/03/92 jmd	preened
//	04/07/93 pat	changed id to VObject *
//	07/20/93 jmd	preened, removed free(), changed rgbColor to VColor
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/05/94 pat	STRICT: cast to HGDIOBJ for DeleteObject();
//					HANDLE to HPEN; cast to HPEN for GetStockObject().
//	07/14/94 jld	replace NULL with 0
//	12/05/95 dgm	Added VColorScheme::checkSystemPenDeletion() call to ~.
//	01/09/96 dgm	Added assertion for shared system pen deletion.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "pen.h"
#include "iterator.h"
#include "clrscm.h"
#include "cvassert.h"

defineClass(VPen, VObject)

// ---------------------------------------------------------------------------
// Construct a VPen with color 'clr', a default width of one and
// pattern SolidLine.
//
VPen::VPen(VColor clr) :
	hpen(0),
	stock(FALSE),
	ports(0)
{
	penSize = 1;
	penPat = SolidLine;
	penColor = clr;
	make();
}

// ---------------------------------------------------------------------------
// Construct a VPen with width 'width', line pattern 'pat', and
// color 'clr'.
//
VPen::VPen(int width, LineType pat, VColor clr) :
	hpen(0),
	stock(FALSE),
	ports(0)
{
	penSize = width;
	penPat = pat;
	penColor = clr;
	make();
}

// ---------------------------------------------------------------------------
// Construct a VPen with default width, color and pattern (see reset()).
//
VPen::VPen() :
	hpen(0),
	stock(FALSE),
	ports(0)
{
	reset();
}

// ---------------------------------------------------------------------------
VPen::~VPen()
{
	if (hpen && !stock) {
		DeleteObject((HGDIOBJ) hpen);
	}

	if (ports != 0) {
		if (ports->count()) {
			VObject *p;
			VIterator i(ports);
			while (p = i++) {
				p->perform(pSet, (long)NIL);
			}
		}
		delete ports;
	}

	boolean deleting_non_shared_resource = !VColorScheme::
										    checkSystemPenDeletion(this);
	cv_assert(deleting_non_shared_resource);
}

// ---------------------------------------------------------------------------
// Construct an MS Windows pen object
//
boolean VPen::make()
{
	HPEN	h;
	VColor	cl = color();
	LineType ptn = pattern();
	int		w = width();
	boolean oldStock = stock;
	VObject *p;

	if (ptn == SolidLine && cl == WHITE && w == 1) {
		h = (HPEN) GetStockObject(WHITE_PEN);
		stock = TRUE;
	}
	else if (ptn == SolidLine && cl == BLACK && w == 1) {
		h = (HPEN) GetStockObject(BLACK_PEN);
		stock = TRUE;
	}
	else {
		h = CreatePen(ptn, w, cl);
		stock = FALSE;
	}

	if (h == (HANDLE)0) {
		print("invalid pen handle");
		return FALSE;
	}
	else {
		if (ports == 0 || !ports->count()) {
			if (hpen && !oldStock) {
				DeleteObject((HGDIOBJ) hpen);
			}
			hpen = h;
		}
		else {
			VIterator i(ports);
			while (p = i++) {
				p->perform(pReset);
			}
			if (hpen && !oldStock)  {
				DeleteObject((HGDIOBJ) hpen);
			}
			hpen = h;
			i.start();
			while (p = i++) {
				p->perform(pSet, (long)this);
			}
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Reset this VPen to width 1, color to the GUI system default text
// color, and pattern to SolidLine.
//
void VPen::reset()
{
	penSize = 1;
	penPat = SolidLine;
	penColor = GetSysColor(COLOR_WINDOWTEXT);
	make();
}

// ---------------------------------------------------------------------------
// Set the pen color to 'clr' (that is, the foreground color for
// patterned drawing).  
// 
void VPen::color(const VColor& clr)
{
	if (penColor != clr) {
		penColor = clr;
		make();
	}
}

// ---------------------------------------------------------------------------
// Set the pattern of the drawing pen to 'ptrn' where 'ptrn' is one of: 
// enum LineType: SolidLine, DashLine, DotLine, DashDotLine, 
// DashDotDotLine, or NoLine.
// 
void VPen::pattern(LineType ptrn)
{
	if (penPat != ptrn) {
		penPat = ptrn;
		make();
	}
}

// ---------------------------------------------------------------------------
// Set the drawing pen width to 'size' units.  If a viewing
// transformation is enabled in a VPort, then the pen width will be
// in those units.
//
void VPen::width(int size)
{
	if (penSize != size) {
		penSize = size;
		make();
	}
}

// ---------------------------------------------------------------------------
// VPen objects can be bound to more than one VPort object at a time. This 
// mechanism is designed to support the techniques used by MS Windows to 
// create its pen objects.  If an attribute of a pen changes, e.g., the color, 
// then in MS Windows the existing pen object must be destroyed and a new one
// created. Hence, because of this binding the VPen objects can then notify the
// VPort objects they are bound too when the MS Windows pen handle has changed.
//
void VPen::bind(VObject *p, boolean b)
{
	if (ports == 0) {
		ports = new VOrdCollect();
	}
	if (b) {
		if (!(ports->findSame(p))) {
			ports->add(p);
		}
	}
	else {
		ports->removeSame(p);
	}
}

// ---------------------------------------------------------------------------
