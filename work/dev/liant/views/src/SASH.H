// $Header:   Q:/views/common/vcs/SASH.H_V   1.11   Mar 24 1997 16:13:18   scottt  $ 

//	sash.h
//
//	VSash interface [Common]
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
//	07/02/93 dgm	original.
//	08/09/93 jmd	added erased
//	08/23/93 dgm	added focus support (from Tony).
//					added archiver support.
//	08/25/93 dgm	removed static members.
//	08/30/93 dgm	added disabled and shaded support.
//	02/27/94 jmd	changed port to thePort, pen to thePen
//	03/25/94 pat	removed VSash:: scope from param declaration in constructor
//	07/29/94 dgm	removed focus support.
//	05/16/95 pkt	preened and merged.
//	09/12/95 pkt	added 'mouseExit' for Motif.
//	10/23/95 dss	Added getInfo() to remove WIN95 native 3D client edge.
//	10/24/95 dss	whoops! getInfo() is protected, not public.
//	09/17/96 dgm	Fixed up colors a bit; this *really* needs to be rewritten.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSASH_H
#define VSASH_H

// ---------------------------------------------------------------------------

#undef CV_USE_STATIC_MEMBERS

// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "control.h"

// ---------------------------------------------------------------------------
// External class declarations
// ---------------------------------------------------------------------------

CLASS VArchiver;
CLASS VBrush;
CLASS VClass;
CLASS VControl;
CLASS VMouseCursor;
CLASS VObject;
CLASS VPen;
CLASS VPort;
CLASS VShade;
CLASS VWindow;

// ---------------------------------------------------------------------------
// Class definition
// ---------------------------------------------------------------------------

CLASS VSash : public VControl {

public:
	enum Style {
						Horizontal	= 0x0001,
						Vertical	= 0x0002,
						NoShade		= 0x0004,
						DipShade	= 0x0008,
						BumpShade	= 0x0010,
						Hidden		= 0x0020,
						Border		= 0x0040,
						Default		= Horizontal | NoShade
	};

public:
						VSash();
						VSash(const VFrame& frame, VWindow *parent,
							  Style style = Default);
						VSash(const VString& name,
							  const VFrame& frame, VWindow *parent,
							  Style style = Default);
						~VSash();
	VClass			   *iam();
	VSash::Style		getStyle() const;
	void				setStyle(VSash::Style);
	boolean				drag();
	void				constrainBySiblingSashes(boolean);
	boolean				erased();
	void				disable(boolean = TRUE);
	void				enable(boolean = TRUE);
	boolean				isEnabled() const;
	VShade			   *getShade() const;

#ifndef CV_NOARCHIVER
	void		   		putTo(VArchiver& a);
	void				getFrom(VArchiver& a);
	void				getFrom(VArchiver& a, VObject *pwin,
							    boolean do_realize = TRUE);
	void				realize(VWindow *pwin);
#endif

protected:
	boolean				mouseDn(int mx, int my);
	boolean				mouseDbl(int mx, int my);
	boolean				mouseMv(int mx, int my, int mbutton);
	boolean				mouseUp(int mx, int my);
	boolean				mouseEnter(int mbutton);
#ifdef MOTIF
	boolean				mouseExit(int mbutton);
#endif
	boolean				key(int virtual_key, char ascii_key);
	boolean				paint();
	void				getInfo(VWinInfo&);
    void		    	systemColorChanged();

private:
	void				getMinSize(short& w, short& h);
	void				initialize();
	void				setupForNextPosition(int mx, int my);
	void				setupConstraints();
	void				disableClipping();
	void				restoreClipping();

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a, boolean most_derived);
#endif

private:
	VSash::Style		sashStyle;
	boolean				constrainedBySiblingSashes;
	boolean				disabledSash;
	VShade			   *theShade;

#if defined(CV_USE_STATIC_MEMBERS)
private:
	static void			allocateStaticResources();
	static void			freeStaticResources();
#endif

private:

#if defined(CV_USE_STATIC_MEMBERS)
	static long			numberOfSashes;
	static int			sashX;
	static int			sashY;
	static int			sashWidth;
	static int			sashHeight;
	static int			startX;
	static int			startY;
	static int			endX;
	static int			endY;
	static int			mousePosition;
	static VMouseCursor*horizontalCursor;
	static VMouseCursor*verticalCursor;
	static VPort	   *thePort;
	static VPen		   *thePen;
	static VWindow	   *sashParent;
	static int			minSashPosition;
	static int			maxSashPosition;
#else
	long				numberOfSashes;
	int					sashX;
	int					sashY;
	int					sashWidth;
	int					sashHeight;
	int					startX;
	int					startY;
	int					endX;
	int					endY;
	int					mousePosition;
	VMouseCursor	   *horizontalCursor;
	VMouseCursor	   *verticalCursor;
	VPort			   *thePort;
	VPen			   *thePen;
	VWindow			   *sashParent;
	int					minSashPosition;
	int					maxSashPosition;
#endif

};

extern VClass *VSashCls;

// ---------------------------------------------------------------------------
// Inline member-function definitions
// ---------------------------------------------------------------------------
//
inline VSash::Style VSash::getStyle() const
{
	return (sashStyle);
}

// ---------------------------------------------------------------------------
//
inline void VSash::disable(boolean b)
{
	disabledSash = b;
}

// ---------------------------------------------------------------------------
//
inline void VSash::enable(boolean b)
{
	disable(!b);
}

// ---------------------------------------------------------------------------
//
inline boolean VSash::isEnabled() const
{
	return !disabledSash;
}

// ---------------------------------------------------------------------------
//
inline VShade *VSash::getShade() const
{
	if (sashStyle & NoShade) {
		return 0;
	}
	else {
		return theShade;
	}
}

#endif // VSASH_H
