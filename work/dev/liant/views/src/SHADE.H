// $Header:   Q:/views/common/vcs/shade.h_v   1.9   Mar 24 1997 16:14:40   scottt  $ 

//	shade.h
//
//	VShade interface [Common]
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
//	04/12/93 jmd	preened, added fill flag to VShade::panel()
//					made protected variables private
//	06/26/93 jmd	added RaisedButton
//	07/20/93 jmd	changed rgbColor to VColor
//	05/20/94 dgm	Added setDefaultColors().
//	05/17/95 pkt	preened and merged.
//	12/05/95 dgm	Moved setColors() to shade.cpp.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSHADE_H
#define VSHADE_H

#include "defs.h"
#include "wintypes.h"
#include "color.h"

enum panelType { Raised, Indented, Flat, Pressed, RaisedButton };
enum lineType  { VBump, HBump, VDip, HDip };

// Instances of the VShade class are used to perform 3-D shading
// within VDisplay objects.  This class is often used to create the 'chisled'
// steel look on VWindow objects. Rectangular areas can be be shaded using 
// the panel() method with any one of the four enum panelType shade styles:
// Raised, Indented, Flat (that is, no 3-D effects), and Pressed
// ( a 'pressed' pushbutton effect). Shaded lines can be drawn using the
// line() method with any one of the four enum lineType line styles:
// HBump, VBump, HDip and VDip. Both the panel() and line() methods
// take an opened VPort object to use in their shading operations. The
// depth of the 3-D look, the thickness of the beveling, is set to two pixels
// by default.  However, the bevel thickness and all of the color attributes
// can be set through the setBevelThickness() and setColors() methods,
// respectively.

// ---------------------------------------------------------------------------
//
CLASS VDisplay;
CLASS VPort;
CLASS VLocation;
CLASS VRectangle;

// ---------------------------------------------------------------------------
//
CLASS VShade {
public:
	// Constructors & Destructor
					VShade();
					~VShade();

	// Drawing 3-D Shades
	void			line(lineType type, int i1, int i2, int i3, VPort& port);
	void			panel(panelType type, VRectangle& rect, short border,
						  VPort& port, boolean fill = TRUE);

	// Interrogating and Setting Attributes
	void			setBevelThickness(short bt);
	short			getBevelThickness();
	void			setColors(VColor hilite, VColor shadow, VColor surface);
	void			getColors(VColor *hilite, VColor *shadow, VColor *surface);

	// Undocumented & unofficial ...

	const VColor&	getShadowColor() const;
	const VColor&	getHighlightColor() const;
	const VColor&	getSurfaceColor() const;

	void			setShadowColor(const VColor&);
	void			setHighlightColor(const VColor&);
	void			setSurfaceColor(const VColor&);

	void			setColors(const VColor&, VColor *foreground = 0);
	void			setDefaultColors();

private:
	VColor			sHilite;			// the hilite color used in 3-D shading
	VColor 			sShadow;			// the shadow color used in 3-D shading
	VColor 			sSurface;			// the surface color for shaded panels
	short			bevelThickness;		// the thickness of the 3-D bevel

};

// ---------------------------------------------------------------------------
// Set the bevel thickness of this VShade object to 'bt' pixels.
//
inline void VShade::setBevelThickness(short bt)
{
	bevelThickness = bt;
}

// ---------------------------------------------------------------------------
// Return the bevel thickness value of this VShade object.
//
inline short VShade::getBevelThickness()
{
	return bevelThickness;
}

// ---------------------------------------------------------------------------
// Return the hilite, shadow and surface colors of this VShade object
// into '*hilite', '*shadow' and '*surface', respectively.
//
inline void VShade::getColors(VColor *hilite, VColor *shadow,
							  VColor *surface)
{
	*hilite = sHilite;
	*shadow = sShadow;
	*surface = sSurface;
}

// ---------------------------------------------------------------------------
//
inline const VColor& VShade::getShadowColor() const
{
	return sShadow;
}

// ---------------------------------------------------------------------------
//
inline const VColor& VShade::getHighlightColor() const
{
	return sHilite;
}

// ---------------------------------------------------------------------------
//
inline const VColor& VShade::getSurfaceColor() const
{
	return sSurface;
}

// ---------------------------------------------------------------------------
//
inline void VShade::setShadowColor(const VColor& shadow)
{
	sShadow = shadow;
}

// ---------------------------------------------------------------------------
//
inline void VShade::setHighlightColor(const VColor& highlight)
{
	sHilite = highlight;
}

// ---------------------------------------------------------------------------
//
inline void VShade::setSurfaceColor(const VColor& surface)
{
	sSurface = surface;
}

#endif // VSHADE_H
