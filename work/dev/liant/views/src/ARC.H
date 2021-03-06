// $Header:   Q:/views/win/vcs/arc.h_v   1.7   Mar 04 1997 16:31:50   gregm  $ 

//	arc.h
//
//	VArc class interface
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
//	10/14/92 jmd	created
//	07/18/93 jmd	added new port fill interface
//	08/09/93 jmd	removed free
//	09/12/93 jmd	preened, removed getAngles, added ArcStyle
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/05/94 pmcm	getHndl() is protected
//	08/08/94 jld	added move()
//	08/09/94 jld	added inset()
// ---------------------------------------------------------------------------

#ifndef VARC_H
#define VARC_H
#include "ellipse.h"
#include "rect.h"

CLASS VPort;

extern VClass *VArcCls;

// The VArc class defines an elliptical region which is specified
///through the coordinates of a bounding rectangle and a starting
// and ending angle
// The VArc can have one of several styles (VArc::ArcStyle):
//
//	VArc::Arc		draw only the circular section of the arc
//	VArc::Pie		draw a pie slice-shaped wedge
//	VArc::Chord		draw the circular section plus the chord connecting
//					the endpoints
//

CLASS VArc : public VEllipse {

public:

	enum	AngleType	{ Radians, Degrees };
	enum	ArcStyle	{ Arc, Pie, Chord };

					VArc();
					VArc(RectOpts, int i1, int i2, int i3, int i4, 
						float start = 0.0F, float end = 0.0F, 
						AngleType at = Radians);

	VClass 		   *iam();

	void			setAngles(float start, float end, AngleType at = Radians);
	void			getAngles(float* start, float* end, AngleType at = Radians);

	boolean			isChord() const;
	void			setChord(boolean tf);

	VArc::ArcStyle	getStyle() const;
	void		  	setStyle(VArc::ArcStyle as);

	boolean			move(int dx, int dy);
	boolean			inset(int dx, int dy);


protected:

	ArcStyle		_style;
	int				_ptCount;
	POINT			_poly[7];

	float			_startAngle;
	float			_endAngle;

	void			makePolygon();
	int				getIntersection(int w, int h, float angle, int* x, int* y);
	virtual HRGN	getHndl();
	void			fill(VPort *port);

};


// ---------------------------------------------------------------------------
// For v2.1 compatability
//
inline boolean VArc::isChord() const
{
	return (_style == Chord);
}

// ---------------------------------------------------------------------------
// For v2.1 compatability
//
inline void VArc::setChord(boolean tf)
{
	_style = (tf) ? Chord : Pie;
}

// ---------------------------------------------------------------------------
inline VArc::ArcStyle VArc::getStyle() const
{
	return _style;
}

// ---------------------------------------------------------------------------
inline void VArc::setStyle(VArc::ArcStyle as)
{
	_style = as;
}

#endif // VARC_H
