// $Header:   Q:/views/win/vcs/brush.h_v   1.9   Mar 04 1997 16:31:56   gregm  $ 

//	brush.h
//
//	VBrush class interface
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
//	04/09/93 pmcm	changed id to VObject *
//	07/20/93 jmd	preened, removed free(), changed rgbColor to VColor
//	08/23/93 pmcm	removed CLASS keyword from friend declaration
//	09/01/93 dgm	M8 changes; added CLASS VWindow.
//	10/09/93 jmd	made pattern not inline
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/04/94 pmcm	STRICT:	hBrush is now HBRUSH, not HANDLE.
//	11/01/94 jmd	changed last rgbColor to VColor
//	09/11/96 dgm	Added StipplePat.
//	09/13/96 dgm	Added system().
// ---------------------------------------------------------------------------

#ifndef VBRUSH_H
#define VBRUSH_H

#include "object.h"
#include "wintypes.h"
#include "ordcllct.h"
#include "clrscm.h"

CLASS VPort;
CLASS VWindow;

typedef enum {	BackgroundPat,
					ForegroundPat,
					DarkGreyPat,
					LightGreyPat,
					GreyPat,
					HorizontalPat,
					VerticalPat,
					ForeDiagPat,
					BackDiagPat,
					CrossHatchPat,
					DiagHatchPat,
					StipplePat } FillPattern;

//
// The VBrush class defines properties of areas filled by the VPort
// class.  Three properties are defined as: foreground color, background color
// and fill pattern.  Foreground and background color are of type VColor
// (defined in color.h), and the fill pattern is one of type
// enum FillPattern: BackgroundPat, ForegroundPat, DarkGreyPat,
// LightGreyPat, GreyPat, HorizontalPat, VerticalPat, ForeDiagPat,
// BackDiagPat, CrossHatchPat, or DiagHatchPat.
//

extern VClass *VBrushCls;

CLASS VBrush : public VObject {
public:

			VBrush();
			VBrush(VColor color);
			VBrush(FillPattern ptrn, VColor fore, VColor back);
			~VBrush();

	VClass *iam();

	void	background(const VColor&);
	const VColor&	background();
	void	foreground(const VColor&);
	const VColor&	foreground();
	void	pattern(FillPattern ptrn);
	FillPattern pattern();

	void	reset();
	HBRUSH	getHndl();
	boolean	isBuiltin();

	static VBrush&	system(VColorScheme::Color);

protected:

	boolean	stock;		// PRIVATE: flag to indicate that brush handle
						// is a stock object so that it can't be deleted
	HBRUSH hbrush;		// The GUI system handle to the underlying
						// system object.

	FillPattern fillPat;	// Fill pattern for painting.

	VColor	fgColor;	// Foreground color for patterns.
	VColor	bkColor;	// Background color for patterns.

	VOrdCollect *ports;	// VCollection of VPort objects that this
						// VBrush is associated with.

	method	pReset;
	method	pSet;
	void	bind(VObject *p, boolean b);
	boolean make();

	friend VPort;
	friend VWindow;

};

// ---------------------------------------------------------------------------
// Return the background color of this VBrush.
//
inline const VColor& VBrush::background()
{
	return bkColor;
}

// ---------------------------------------------------------------------------
// Return the foreground color of this VBrush.
//
inline const VColor& VBrush::foreground()
{
	return fgColor;
}

// ---------------------------------------------------------------------------
// Return the platform dependent handle for this VBrush object.
//
inline HBRUSH VBrush::getHndl()
{
	return hbrush;
}

// ---------------------------------------------------------------------------
// Return TRUE if this VBrush is a 'builtin' system brush, i.e.,
// a stock brush in MS Windows.
//
inline boolean VBrush::isBuiltin()
{
	return stock;
}

inline VBrush& VBrush::system(VColorScheme::Color color)
{
	return VColorScheme::systemBrush(color);
}

#endif // VBRUSH_H
