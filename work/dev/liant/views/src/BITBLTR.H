// $Header:   Q:/views/win/vcs/bitbltr.h_v   1.6   Mar 04 1997 16:31:52   gregm  $ 

//	bitbltr.h
//
//	VBitBltr class interface
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
//  08/09/93 jmd	removed free
//	10/10/93 jmd	SC6 changes, preened
//	04/08/96 dgm	Added undocumented/non-portable version of bitbltr()
//					which implements StretchBlt functionality.
// ---------------------------------------------------------------------------

#ifndef VBITBLTR_H
#define VBITBLTR_H
#include "object.h"
#include "display.h"
#include "rect.h"

CLASS VBitMap;
CLASS VWindow;

extern VClass *VBitBltrCls;

typedef enum {
	CopyRule = R2_COPYPEN,
	OrRule = R2_MERGEPEN,
	XorRule = R2_XORPEN, 
	NoCopyRule = R2_NOTCOPYPEN,
	NotOrRule = R2_MERGENOTPEN,
	NotXorRule = R2_NOTXORPEN, 
	EraseRule = R2_MASKNOTPEN,
	AndRule = R2_MASKPEN
} RasterRule; 

// The VBitBltr class supports the transfer of pixels (bits)
// from a source display object to a destination display object.  The source
// of the bits for transfer can be instances of the VBitMap class or instances
// of subclasses of the VWindow class.  The destination display objects can be
// instances of the VBitMap class, the VPrinter class, or instances of
// subclasses of the VWindow class.  The transfer of bits between the source
// and destination display objects is restricted to a rectangular subset of
// both objects.  The bitbltr methods of the VBitBltr class are used to perform
// the "bit block transfer" between the source and destination rectangles.  
// However, before using the bitbltr methods, the origin (in pixels) of the
// transfer rectangle in both the source and destination display spaces must
// be defined.  Since no stretching or shrinking of pixels is supported, only
// the width and height of the transfer rectangle has to be defined for both
// the source and destination.  Additionaly, the rule for logically combining
// the bits of the source and destination objects must be specified.  These
// combination rules are of type enum RasterRule: 
// CopyRule, OrRule, XorRule, NoCopyRule, NotOrRule, NotXorRule, 
// EraseRule, or AndRule.
//

CLASS VBitBltr : public VObject {
public:
			VBitBltr();
			VBitBltr(VDisplay *, VDisplay *);
	VClass *iam();

	void	bitbltr(int, int, int, int, int, int, RasterRule);
	void	bitbltr(int, int, RasterRule);
	void	source(VRectangle *);
	void	source(RectOpts, int, int, int, int);

	// Undocumented and non-portable (!) -- StretchBlt functionality.

	void	bitbltr(int, int, int, int, int, int, int, int, RasterRule);

protected:
	VDisplay  *src;		// Source image - any subclass of the VDisplay class
	VDisplay  *dest;	// Destination image - any subclass of the VDisplay class
	VRectangle srcRect; // Source VRectangle - a rectangular area of the source image
};

#endif // VBITBLTR_H
