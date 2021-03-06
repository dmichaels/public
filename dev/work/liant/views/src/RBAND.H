// $Header:   Q:/views/common/vcs/rband.h_v   1.10   25 Feb 1997 10:41:06   tomf  $
//	rband.h
//
//	VRubberBand interface [Common]
//
//  Allegris Foundation 1.1.00
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
//	11/01/95 dgm	Original.
//	12/07/95 dgm	Added disableDrawing().
//	12/08/95 dgm	Added setWindow(), initializeWindow(), getWindowSize().
//	03/06/96 dgm	Added undraw().
//	08/25/96 dgm	Made getCurrent() non-inline - trouble with 16-bit DLL.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VRBAND_H
#define VRBAND_H

#include "object.h"

// --------------------------------------------------------------------------
//
CLASS VObject;
CLASS VWindow;

// --------------------------------------------------------------------------
// The VRubberBand class provides a mechanism for the user to perform
// the interactive resizing or moving through the "rubber-banding" of
// a rectangular outline.  A VRubberBand object may be created and
// associated with any VWindow object or the root screen.
//
CLASS VRubberBand {
public:
	enum Mode
	{					Move				= 0x0001,
						Resize				= 0x0010,
						ResizeTop			= 0x0020,
						ResizeBottom		= 0x0040,
						ResizeLeft			= 0x0080,
						ResizeRight			= 0x0100,
						ResizeTopLeft		= ResizeTop    | ResizeLeft,
						ResizeTopRight		= ResizeTop    | ResizeRight,
						ResizeBottomLeft	= ResizeBottom | ResizeLeft,
						ResizeBottomRight	= ResizeBottom | ResizeRight
	};
	enum DoneStatus
	{					NeverStarted		=  0,
						Unchanged			= -1,
		 				Cancelled			= -2,
						Changed				=  1
	};

public:
						VRubberBand(VWindow * = 0);
				       ~VRubberBand();

	int					resize(int mode, int& x, int& y, int& w, int& h,
							   int&  mx, int& my, VWindow * = 0);
	int					move(int& x, int& y, int& w, int& h,
							 int& mx, int& my, VWindow * = 0);

	int					resizeLine(int& x1, int& y1, int& x2, int& y2,
								   int&  mx, int& my, VWindow * = 0);
	int					moveLine(int& x1, int& y1, int& x2, int& y2,
								 int& mx, int& my, VWindow * = 0);

	void				setWindow(VWindow *);
	void				setMinimum(int w, int h);
	void				confineToWindow(boolean);
	void				setThickness(int);
	void				setHysteresis(int, int);
	void				dontGrabDisplay(boolean);
	void				disableDrawing(boolean);
	void				dontMoveX(boolean);
	void				dontMoveY(boolean);
	void				uponBeginResize(VObject *, method);
	void				uponResize(VObject *, method);
	void				uponBeginMove(VObject *, method);
	void				uponMove(VObject *, method);
	void				draw(int, int, int, int);
	void				undraw();

	static VRubberBand *getCurrent();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	boolean				beginResizeCallback();
	boolean				resizeCallback(int, int, int&, int&, int);
	boolean				beginMoveCallback();
	boolean				moveCallback(int&, int&, int&, int&, int, int);
private:
	void				initializeWindow(int * = 0, int * = 0);
	void				deinitializeWindow();
	void				createRubberBand();
	void				destroyRubberBand();
	void				drawRectangle(int, int, int, int);
	void				drawLine(int, int, int, int);
	void				undrawLine();
	void				captureMouse(VWindow * = 0);
	void				uncaptureMouse();

private:
	VWindow			   *theWindow;
	VObject			   *clientObject;
	boolean	 (VObject::*uponBeginResizeMethod)();
	boolean	 (VObject::*uponResizeMethod)(int, int, int&, int&, int);
	boolean	 (VObject::*uponBeginMoveMethod)();
	boolean	 (VObject::*uponMoveMethod)(int&, int&, int&, int&, int, int);
	int					minimumWidth;
	int					minimumHeight;
	int					maximumWidth;
	int					maximumHeight;
	int					thickness;
	int					hysteresisX;
	int					hysteresisY;
	int					confinedToWindow : 1;
	int					stretching : 1;
	int					drawingDisabled : 1;
	int					doNotMoveX : 1;
	int					doNotMoveY : 1;
	int					doLine : 1;

private:
	static VRubberBand *currentRubberBand;
};

// --------------------------------------------------------------------------
//
inline void VRubberBand::setMinimum(int w, int h)
{
	minimumWidth = w;
	minimumHeight = h;
}

// --------------------------------------------------------------------------
//
inline void VRubberBand::confineToWindow(boolean b)
{
	confinedToWindow = b;
}

// --------------------------------------------------------------------------
//
inline void VRubberBand::setHysteresis(int x, int y)
{
	hysteresisX = x;
	hysteresisY = y;
}

// --------------------------------------------------------------------------
//
inline void VRubberBand::dontMoveX(boolean b)
{
	doNotMoveX = b;
}

// --------------------------------------------------------------------------
//
inline void VRubberBand::dontMoveY(boolean b)
{
	doNotMoveY = b;
}

#endif // VRBAND_H
