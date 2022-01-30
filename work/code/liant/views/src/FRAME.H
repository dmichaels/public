// $Header:   Q:/views/common/vcs/FRAME.H_V   1.8   Mar 24 1997 16:08:58   scottt  $ 

//	frame.h
//
//	VFrame class interface [Common]
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
//	05/05/93 jmd	added units
//	05/20/93 jmd	preened, changed id to VObject *
//	07/19/93 jmd	made operator = use a const arg
//	08/09/93 jmd	removed free
//	08/18/93 dgm	added isFont16() and setUnits().
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	10/15/93 pkt	removed print.
//	11/13/93 pkt	made VLocation argument to get const.
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	02/15/94 pmcm	made VLocation& parameters const in resize(), move(),
//					xlate(), get(), convertOpt(), and setAsBefore()
//	05/19/95 pkt	merged for common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VFRAME_H
#define VFRAME_H

// --------------------------------------------------------------------------
//
#include "rect.h"
#include "ratio.h"

// --------------------------------------------------------------------------
//
CLASS VLocation;
CLASS VStream;
CLASS VWindow;

// The VFrame class represents a rectangular area. It is used by the
// VWindow class hierarchy to maintain the size and position of VWindow
// objects.  An instance of VFrame can be described by two points, or by a
// point (one of the four corners) and an extent (the width and height of the
// frame).  VFrame objects are constructed using VRatio objects which can
// describe fixed or ratioed (percents) coordinates.  To facilitate the
// construction and movement of VWindow objects, the position and size
// attributes of VFrame objects can be retrieved, moved or translated
// relative to the dimensions of an enclosing VFrame (typically the
// dimensions of the VFrame of a parent VWindow).

CLASS VFrame : public VObject {
public:
	enum Units {
				Pixels,
				Font16
	};

public:
	// Constructors
				VFrame();
				VFrame(VRatio w, VRatio h);
				VFrame(VRatio x, VRatio y, VRatio w, VRatio h,
								RectOpts opts = CornerDim,
								Units u = Pixels);

				VFrame(VRatio x, VRatio y, VRatio w, VRatio h, Units u);
	VClass	   *iam();

	Units		getUnits();

	// Interrogating and Setting Coordinates
	void		set(VRatio x, VRatio y, VRatio w, VRatio h,
					RectOpts opts = CornerDim);
	void		isRatio(boolean& x, boolean& y, boolean& w, boolean& h);
	void		isFont16(boolean& x, boolean& y, boolean& w, boolean& h);
	void		get(VRatio& x, VRatio& y, VRatio& w, VRatio& h, RectOpts& opts);
	void		get(const VLocation& pnt, VRectangle& r);
	void		get(const VLocation& pnt, int *x, int *y, int *w, int *h);
	void		get(VWindow *win, const VLocation& pnt,
					int *x, int *y, int *w, int *h);

	RectOpts	howFramed();

	void		setHowFramed(RectOpts hownow);
	void		setUnits(VFrame::Units);

	void		convertOpt(const VLocation& pnt, RectOpts);
	void		setAsBefore(const VLocation& pnt, int, int, int, int);

	// Operations on a VFrame
	void		resize(const VLocation& pnt, VRatio w, VRatio h);
	void		move(const VLocation& pnt, VRatio x, VRatio y);
	void		xlate(const VLocation& pnt, VRatio dx, VRatio dy);

	VFrame&		operator=(VRectangle& r);
	boolean		noRatio();

	// Copying
	boolean		copyFrom(VObject *obj);
	boolean		copyTo(VObject *obj);
	VFrame&		operator= (const VFrame& f);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:
	// How the coordinates are to be construed
	RectOpts	how;
	// The X-value of a corner
	int			xf;
	// The Y-value of a corner
	int			yf;
	// The X-value of a corner or the width of the frame
	int			wf;
	// The Y-value of a corner or the height of the frame
	int			hf;

	Units		units;

};

extern VClass *VFrameCls;

// ---------------------------------------------------------------------------
//
inline VFrame::Units VFrame::getUnits()
{
	return units;
}

// ---------------------------------------------------------------------------
// Return the 'how-framed' value of this VFrame as one of the enum
// RectOpts constants: Corners, CenterDim, or CornerDim.
//
inline RectOpts VFrame::howFramed()
{
	return how;
}

// ---------------------------------------------------------------------------
// Set the 'how-framed' value of this VFrame as one of the enum
// RectOpts constants: Corners, CenterDim, or CornerDim.
//
inline void VFrame::setHowFramed(RectOpts hownow)
{
	how = hownow;
}

// ---------------------------------------------------------------------------
//
inline void VFrame::setUnits(VFrame::Units u)
{
	units = u;
}

#endif // VFRAME_H
