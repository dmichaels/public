// $Header:   Q:/views/win/vcs/color.h_v   1.9   Mar 04 1997 16:32:02   gregm  $ 

//	color.h
//
//	VColor interface
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
//	07/19/93 jmd	created
//	09/29/93 dgm	updated
//	10/13/93 jmd	removed static members
//	01/31/94 pmcm	added RGB set methods
//	02/17/94 pmcm	made v2.x globals Gray,DarkGray,LightGray CV_NEARDATA.
//	03/05/94 pmcm	Win32 changes: CV_NEARDATA is whitespace for CV_WIN32
//	07/13/94 jld	move CV_NEARDATA to defs.h
//	03/31/95 jld	externs on Grays confuse Symantec
//	12/05/95 dgm	Added destructor and call to
//					VColorScheme::checkSystemColorDeletion().
//	09/13/96 dgm	Added system().
// --------------------------------------------------------------------------

#ifndef VCOLOR_H
#define VCOLOR_H

#include "defs.h"
#include "clrscm.h"

CLASS VColor {
public:
	// Windows does not like static data members: use an enum

	enum ColorValue {
			Black = 0,
			DarkBlue = 1,
			DarkGreen = 2,
			DarkCyan = 3,
			DarkRed = 4,
			DarkMagenta = 5,
			DarkYellow = 6,
			LightGray = 7,
			Gray = 8,
			DarkGray = 9,
			Blue = 10,
			Green = 11,
			Cyan = 12,
			Red = 13,
			Magenta = 14,
			Yellow = 15,
			White = 16
	};
						VColor(unsigned long val = 0L);
						VColor(short r, short g, short b);
						VColor(const VColor& clr);
						VColor(ColorValue cv);
					   ~VColor();

	short				getRed() const;
	short				getGreen() const;
	short				getBlue() const;

	void 				setRed(short r);
	void				setGreen(short g);
	void				setBlue(short b);
	void				setRGB(short r, short g, short b);

	VColor& 			operator= (const VColor& clr);
						operator long() const;

	static VColor&		system(VColorScheme::Color);

private:
	unsigned long		rgbValue;

};

// For v2.x compatability

#define rgbColor		VColor
#define RGBCLR(r,g,b)	(VColor(r,g,b))

#define BLACK			VColor::Black
#define DARKBLUE 		VColor::DarkBlue
#define DARKGREEN 		VColor::DarkGreen
#define DARKCYAN 		VColor::DarkCyan
#define DARKRED 		VColor::DarkRed
#define DARKMAGENTA 	VColor::DarkMagenta
#define DARKYELLOW 		VColor::DarkYellow
#define BROWN			VColor::DarkYellow
#define LIGHTGRAY 		VColor::LightGray
#define GRAY 			VColor::DarkGray
#define DARKGRAY		VColor::DarkGray
#define BLUE 			VColor::Blue
#define GREEN			VColor::Green
#define CYAN 			VColor::Cyan
#define RED 			VColor::Red
#define MAGENTA			VColor::Magenta
#define YELLOW			VColor::Yellow
#define WHITE			VColor::White

// For v2.x compatability
#if !defined(DLL) && !defined(__SC__)
	extern const VColor CV_NEARDATA Gray;
	extern const VColor CV_NEARDATA LightGray;
	extern const VColor CV_NEARDATA DarkGray;
#endif

// ---------------------------------------------------------------------------
inline VColor::VColor(unsigned long val)
{
	rgbValue = val;
}

// ---------------------------------------------------------------------------
inline VColor::VColor(short r, short g, short b)
{
	rgbValue = ((unsigned long)
				(((unsigned char)(r) |
				  ((unsigned short)(g) << 8)) |
				 (((unsigned long)(unsigned char)(b)) << 16)));
}

// ---------------------------------------------------------------------------
inline VColor::VColor(const VColor& clr)
{
	rgbValue = clr.rgbValue;
}

// ---------------------------------------------------------------------------
inline VColor::operator long() const
{ 
	return rgbValue; 
}

// ---------------------------------------------------------------------------
inline VColor& VColor::operator= (const VColor& clr)
{
	rgbValue = clr.rgbValue;
	return *this;
}

// ---------------------------------------------------------------------------
inline void VColor::setRed(short r)
{
	rgbValue &= ~((unsigned long) ((unsigned char) ~0));
	rgbValue |=  ((unsigned long) ((unsigned char)  r));
}

// ---------------------------------------------------------------------------
inline void VColor::setGreen(short g)
{
	rgbValue &= ~(((unsigned long) ((unsigned char) ~0)) << 8);
	rgbValue |=  (((unsigned long) ((unsigned char)  g)) << 8);
}

// ---------------------------------------------------------------------------
inline void VColor::setBlue(short b)
{
	rgbValue &= ~(((unsigned long) ((unsigned char) ~0)) << 16);
	rgbValue |=  (((unsigned long) ((unsigned char)  b)) << 16);
}

// ---------------------------------------------------------------------------
inline void VColor::setRGB(short r, short g, short b)
{
	rgbValue = ((unsigned long)
				(((unsigned char)(r) |
				  ((unsigned short)(g) << 8)) |
				 (((unsigned long)(unsigned char)(b)) << 16)));
}

// ---------------------------------------------------------------------------
//
inline VColor& VColor::system(VColorScheme::Color color)
{
	return VColorScheme::systemColor(color);
}

#endif // VCOLOR_H

