// $Header:   Q:/views/win/vcs/port.h_v   1.8   Mar 04 1997 16:32:50   gregm  $ 

//	port.h
//
//	VPort interface
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
//	09/21/92 jmd	preened
//	10/21/92 jmd	added fillFrame
//	07/18/93 jmd	preened, removed free()
//	07/18/93 jmd	added new port fill interface, added getHDC
//	07/20/93 jmd	changed rgbColor to VColor
//	10/05/94 jld	changed JustifyBottom/Top/Vcenter to single line
//	02/13/95 jld	changed wrtText() arg to const char*
//	07/18/95 dgm	Miscellaneous const char * changes.
// ----------------------------------------------------------------------------

#ifndef VPORT_H
#define VPORT_H

#include "display.h"
#include "bitbltr.h"
#include "pen.h"
#include "brush.h"
#include "rect.h"
#include "font.h"

#define	JustifyTop		0x0000 | 0x0020	// DT_TOP | DT_SINGLELINE
#define JustifyLeft		0x0000			// DT_LEFT
#define JustifyCenter	0x0001 | 0x0020	// DT_CENTER | DT_SINGLELINE
#define JustifyRight	0x0002			// DT_RIGHT
#define JustifyVcenter	0x0004			// DT_VCENTER
#define JustifyBottom	0x0008 | 0x0020	// DT_BOTTOM | DT_SINGLELINE
#define WordBreak		0x0010			// DT_WORDBREAK
#define SingleLine		0x0020			// DT_SINGLELINE
#define ExpandTabs		0x0040			// DT_EXPANDTABS
#define TabStop			0x0080			// DT_TABSTOPS
#define NoClipping		0x0100			// DT_NOCLIP
#define CalcRect		0x0400			// DT_CALCRECT
#define NoPrefix		0x0800			// DT_NOPREFIX

extern VClass *VPortCls;

CLASS VPointArray;
CLASS VBitMap;
CLASS VRegion;
CLASS VString;
CLASS VWindow;
CLASS VIcon;

//
// The VPort class is the main graphic output class.  VPort objects
// provide functions for drawing and painting simple shapes, drawing
// lines, drawing text, and performing viewing transformations.  VPort
// objects work with a VPen that supports multiple pen colors,
// widths, and patterns.  Regions, rectangles, circles and ellipses can
// be filled with various patterns and colors using a VBrush object.
// Text is written with a VFont object.
// All VPort objects share a default VPen, VFont and VBrush
// (see VPen::reset() and VBrush::reset() for default values).
// VPort objects may be opened on VWindow objects, VBitMap objects,
// or VPrinter objects.  VPort objects must not be left open for long
// periods of time, they should always be closed when not in use.
// This open -	close pairing is very IMPORTANT for VPort objects
// to be used effectively.
//

CLASS VPort : public VObject {
public:
			VPort();
			VPort(VDisplay *display);
			~VPort();

	VBrush	brush;				// The default VBrush object.
	VPen	pen;				// The default drawing VPen object.
	VFont	font;				// The default drawing VFont object.

	VClass *iam();

	boolean isOpen();
	boolean open();
	boolean openOn(VDisplay *display);
	void	reset();
	void	close();
	void	clear();

	// Viewing Transformations

	void	viewPort(VRectangle *);
	void	viewWindow(VRectangle *);
	void	viewPort(RectOpts, int, int, int, int);
	void	viewWindow(RectOpts, int, int, int, int);
	boolean useXform(boolean);
	boolean xForm(VRectangle *);
	boolean unXform(VRectangle *);
	boolean xForm(int *, int *);
	boolean unXform(int *, int *);
	boolean scaleLogicalToDevice(int*, int*);
	boolean scaleDeviceToLogical(int*, int*);

	// Text Operations
	void	enableBkColor(boolean);
	int		wrtText(const VString *, int, int);
	int		wrtText(const char *, int, int);
	int		wrtText(const char *, VRectangle *, short);
	int		textLineSpace();
	void	textSize(const char *, int *, int *);

	// Drawing Operations
	void	drawBitMap(VBitMap *, int, int);
	void	drawIcon(VIcon *, int, int);
	void	drawLines(VPointArray *points);
	void	frameRegion(VRegion *);
	void	lineTo(int, int);
	void	moveTo(int, int);

	// Painting Operations
	void	eraseRegion(VRegion *);
	void	fillRegion(VRegion *, boolean border = TRUE);
	void	invertRegion(VRegion *);

	float	aspectRatio();
	VBrush *currentBrush();
	VPen   *currentPen();
	VFont  *currentFont();
	boolean pixelsPerInch(int *, int *);
	boolean setClipRgn(VRegion *);

	VDisplay *getImage();
 	VRegion  *getClipRgn();

	boolean useBrush(VBrush *);
	boolean usePen(VPen *);
	boolean useFont(VFont *);
	boolean resetPen();
	boolean resetBrush();
	boolean resetFont();
	void	rule(RasterRule);

	void	fillFrame(VRectangle *outer, VRectangle *inner);

	RasterRule rule();

	HDC		getHDC();			// jmd

protected:

	VDisplay *image;			// The VDisplay object where VPort
								// output is directed.

	boolean bkColorEnabled;		// Indicator to specify whether to use
								// the backGroundClr for drawing patterned
								// lines and for writing text.
	
	VColor backGroundClr;		// Background color for area covered by
								// a VPort. Used as background for drawing
								// lines and text.  Also used to color a
								// VPort object's background with
								// the clear() method.
	VColor oldTextClr;
	HDC		 hDC;

	VRectangle area;			// Position of the origin on the output device.   

	HANDLE    hRgn;				// PRIVATE: handle to clip region. 
	HANDLE	  oldPen;			// PRIVATE: handle to old pen object. 
	HANDLE	  oldBrush;			// PRIVATE: handle to old brush object 
	HANDLE	  oldFont;			// PRIVATE: handle to old font object 

	VRegion *clipRgn;			// Clipping region of a VPort
								// (an instance of VRegion or VRectangle). 

	VPen	*currPen;			// Current VPen object. 
	VBrush	*currBrush;			// Current VBrush object. 
	VFont	*currFont;			// Current VFont object. 

	VRectangle vRect;			// VRectangle describing the world coordinates
								// of a viewing	transformation.

	VRectangle vpRect;			// VRectangle describing the device
								// coordinates of a viewing	transformation.

	boolean  mapMode;			// TRUE if output is through virtual coordinates,
								// that is,	through a viewing transform.
	
	RasterRule rastRule;		// Raster operation - the rule for applying
								// VPens and VBrushs in output operations.

	void	clean();
	void	init();
	void	setArea();

	friend	VRegion;
	friend	VRectangle;
	friend	VFont;

};

// ---------------------------------------------------------------------------
// Return the image (the VDisplay object) this VPort is bound
// to.  Return 0 if it is not bound to an image.
//
inline VDisplay *VPort::getImage()
{
	return image;
}

// ---------------------------------------------------------------------------
// Return the current clipping region.
//
inline VRegion *VPort::getClipRgn()
{
	return clipRgn;
}

// ---------------------------------------------------------------------------
// Return the rastor-op mode of this VPort.
//
inline RasterRule VPort::rule()
{
	return rastRule;
}

// ---------------------------------------------------------------------------
// Not portable...
//
inline HDC VPort::getHDC()
{
	return hDC;
}


#endif // VPORT_H
