// $Header:   Q:/views/win/vcs/pen.h_v   1.8   Mar 04 1997 16:32:44   gregm  $ 

//	pen.h
//
//	VPen interface
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
//	04/09/93 pmcm	changed id to VObject *
//	07/20/93 jmd	preened, removed free(), changed rgbColor to VColor
//	07/21/93 pmcm	changed class to CLASS in forward declarations.
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/04/94 pmcm	STRICT: hPen is now HPEN, not HANDLE.
//	09/13/96 dgm	Added system().
// ----------------------------------------------------------------------------

#ifndef VPEN_H
#define VPEN_H

#include "object.h"
#include "wintypes.h"
#include "ordcllct.h"
#include "clrscm.h"

CLASS VPort;

extern VClass *VPenCls;

typedef enum {
	SolidLine = PS_SOLID,
	DashLine = PS_DASH,
	DotLine = PS_DOT, 
	DashDotLine = PS_DASHDOT,
	DashDotDotLine = PS_DASHDOTDOT,
	NoLine = PS_NULL
} LineType;

//
// The VPen class defines properties of lines drawn in the VPort
// class.  Three properties are defined:  width, color, and pattern.  Width is
// in screen units, color is of type rgbColor (see wintypes.h for rgbColor
// definitions), and pattern is of type LineType(or one of: SolidLine,
// DashLine, DotLine, DashDotLine, DashDotLine or DashDotDotLine). 
//

CLASS VPen : public VObject {
public:
			VPen();
			VPen(VColor);
			VPen(int width, LineType pat, VColor clr);
			~VPen();

	VClass *iam();

	void	reset();
	void	color(const VColor& clr);
	const VColor&	color();
	void	pattern(LineType);
	LineType pattern();
	void	width(int);
	int		width();
	HPEN	getHndl();

	static VPen&	system(VColorScheme::Color);

protected:
	boolean  stock;			// PRIVATE: flag to indicate that the pen handle is a
							// MS Windows stock object so that it can't be deleted
	HPEN	 hpen;			// The GUI system handle to the underlying system object.
	int		 penSize;		// Width of drawing pen.
	LineType penPat;		// Drawing pen pattern.
	VColor	 penColor;		// Drawing pen color.
	VOrdCollect  *ports;	// Ordered VCollection of VPort objects that this
							// VPen is associated with.
	method	pReset;
	method	pSet;

	void	bind(VObject *p, boolean b);
	friend	VPort;
	boolean	make();

};

// ---------------------------------------------------------------------------
// Return the current color of this VPen.
//
inline const VColor& VPen::color()
{
	return penColor;
}

// ---------------------------------------------------------------------------
// Return the current pattern of this VPen.
//
inline LineType VPen::pattern()
{
	return penPat;
}

// ---------------------------------------------------------------------------
// Return the current width of this VPen.
//
inline int VPen::width()
{
	return penSize;
}

// ---------------------------------------------------------------------------
// Return the platform dependent handle for this VPen object.
//
inline HPEN VPen::getHndl()
{
	return hpen;
}

inline VPen& VPen::system(VColorScheme::Color color)
{
	return VColorScheme::systemPen(color);
}

#endif // VPEN_H
