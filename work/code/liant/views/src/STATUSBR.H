// $Header:   Q:/views/common/vcs/STATUSBR.H_V   1.10   Mar 24 1997 16:13:24   scottt  $ 

//	statusbr.h
//
//	VStatusBar class interface [Common]
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
//	07/12/93 dgm	redone.
//	07/20/93 jmd	moved to library
//	09/20/93 dgm	removed static members.
//	09/26/93 jmd	changed rgbColor to VColor
//	09/30/93 dgm	added getMargins().
//	02/29/94 dgm	minor fix in setBackground().
//	03/25/94 pmcm	typedef'd Style enum
//	05/16/94 dgm	Added isStdMdiAppViewStatusBar.
//	06/06/94 dgm	Added Hidden style.
//	08/09/94 dgm	Added archiver support.
//	05/17/95 pkt	preened and merged.
//	11/29/95 tlf	updated for HPUX.
//	12/05/95 dgm	Moved getSurfaceColor() to statusbr.cpp.
//	09/17/96 dgm	Fixed up colors a bit; this *really* needs to be rewritten.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSTATUSBR_H
#define VSTATUSBR_H

// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "control.h"
#include "shade.h"
#include "textbox.h"
#include "brush.h"
#include "str.h"

// ---------------------------------------------------------------------------
// External class declarations
// ---------------------------------------------------------------------------

CLASS VAppView;
CLASS VBrush;
CLASS VClass;
CLASS VControl;
CLASS VFont;
CLASS VFrame;
CLASS VShade;
CLASS VString;
CLASS VTextBox;
CLASS VWindow;
CLASS VWinInfo;
CLASS VArchiver;
CLASS VStdMdiAppView;

// ---------------------------------------------------------------------------
// Class definition
// ---------------------------------------------------------------------------

CLASS VStatusBar : public VControl {

public:
#ifndef CPPV_HPUX
	typedef enum	{
						Simple          = 1,
						Custom          = 2,
						Hidden			= 4,
						Default         = Simple

					} Style;
#else
	enum Style	{
						Simple          = 1,
						Custom          = 2,
						Hidden			= 4,
						Default         = Simple

					} ;
#endif

public:
						 VStatusBar();
						 VStatusBar(const VFrame& frame,
									   VWindow *parent,
#ifndef CPPV_HPUX
									   Style style = Default);
#else
									   Style style = VStatusBar::Default);
#endif
						~VStatusBar();

	VClass             *iam();
	Style 				getStyle() const;
	int					panelHeight() const;
	VBrush			   *getBackground();
	void				setBackground(VBrush *);
	VColor				getSurfaceColor() const;
	void				getShadeColors(VColor *highlight, VColor *shadow,
									   VColor *surface) const;

	void				setShadeColors(const VColor& highlight,
									   const VColor& shadow,
									   const VColor& surface);

	int					getShadeBevelThickness() const;
	void				setShadeBevelThickness(int);

	void 				setMargins(int left, int right, int top, int bottom);
	void				getMargins(int *left, int *right,
								   int *top, int *bottom);

	void				putText(const VString& s);
	void				setFont(VFont *f);

	virtual boolean		frameChildren(int = -1, int = -1,
									  boolean test = FALSE);
	virtual void		getMinSize(short& w, short& h);

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver& a);
	virtual void		getFrom(VArchiver& a);
	virtual void		getFrom(VArchiver& a, VObject *pwin,
								boolean do_realize = TRUE);
	virtual void		realize(VWindow *pwin);
#endif

protected:
	virtual boolean 	paint();
	virtual boolean 	erased();
	virtual boolean 	resized(int w, int h);
	void				initialize();
	void				createDefaultPanel(boolean do_realize = TRUE);
	void				destroyDefaultPanel();
    virtual void    	systemColorChanged();

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a, boolean most_derived);
#endif

private:
	VStatusBar::Style	statusBarStyle;
	VShade			   *shade;
	short				leftMargin;
	short 				rightMargin;
	short 				topMargin;
	short 				bottomMargin;
	VTextBox 		   *defaultPanel;
	VBrush 			   *defaultPanelBackground;
	VFont 			   *defaultFont;
	VAppView		   *appView;
	int					isStdMdiViewStatusBar : 1;
	int					explicitlySetBackground : 1;
	int					explicitlySetShades : 1;

private:
	friend				VStdMdiAppView;

};

extern VClass *VStatusBarCls;

// ---------------------------------------------------------------------------
// Inline member-function definitions
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
inline int VStatusBar::getShadeBevelThickness() const
{
	return shade->getBevelThickness();
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::setShadeBevelThickness(int bevel_thickness)
{
	shade->setBevelThickness(bevel_thickness);
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::getShadeColors(VColor *highlight, VColor *shadow,
								   VColor *surface) const
{
	shade->getColors(highlight, shadow, surface);
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::setShadeColors(const VColor& highlight,
									   const VColor& shadow,
									   const VColor& surface)
{
	explicitlySetShades = 1;
	explicitlySetBackground = 1;
	shade->setColors(highlight, shadow, surface);
	if (defaultPanelBackground != 0) {
		defaultPanelBackground->background(surface);
	}
}

// ---------------------------------------------------------------------------
//
inline VBrush *VStatusBar::getBackground()
{
	if (defaultPanelBackground == 0) {
		defaultPanelBackground = new VBrush(getSurfaceColor());
	}
	return defaultPanelBackground;
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::setBackground(VBrush *brush)
{
	if (brush == 0) {
		delete defaultPanelBackground;
		defaultPanelBackground = 0;
		explicitlySetBackground = 0;
	}
	else {
		if (defaultPanelBackground == 0) {
			defaultPanelBackground = new VBrush;
		}
		defaultPanelBackground->background(brush->background());
		explicitlySetBackground = 1;
	}
}

// ---------------------------------------------------------------------------
//
inline int VStatusBar::panelHeight() const
{
	int w, h;
	((VStatusBar *)this)->sizeOfImage(&w, &h);
	return h - shade->getBevelThickness() - topMargin - bottomMargin;
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::getMargins(int *left, int *right, int *top, int *bottom)
{
	if (left != 0) {
		*left = leftMargin;
	}
	if (right != 0) {
		*right = rightMargin;
	}
	if (top != 0) {
		*top = topMargin;
	}
	if (bottom != 0) {
		*bottom = bottomMargin;
	}
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::setMargins(int left, int right, int top, int bottom)
{
	leftMargin = left;
	rightMargin = right;
	topMargin = top;
	bottomMargin = bottom;
}

// ---------------------------------------------------------------------------
//
inline VStatusBar::Style VStatusBar::getStyle() const
{
	return statusBarStyle;
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::putText(const VString& s)
{
	if (defaultPanel != 0) {
		defaultPanel->putText(s.gets());
	}
}

// ---------------------------------------------------------------------------
//
inline void VStatusBar::setFont(VFont *f)
{
	if (f == 0) {
		f = defaultFont;
	}
	VControl::setFont(f);
	if (defaultPanel != 0) {
		defaultPanel->setFont(f);
	}
}

#endif // VSTATUSBR_H
