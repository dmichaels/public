// $Header:   Q:/views/common/vcs/shdctrl.h_v   1.8   May 30 1997 17:16:28   scottt  $ 

//	shdctrl.h
//
//	VShadeControl class interface [Common]
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
//	04/14/93 jmd	created
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	09/21/93 dgm	added archiver support.
//	05/20/94 dgm	added virtSetupBackground().
//	05/17/95 pkt	preened and merged.
//	10/18/95 dgm	Added getInfo() to avoid duplicate 3D.
//	11/14/95 dgm	Added getInternalBorderGeometry().
//	11/15/95 dgm	Updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VSHDCTRL_H
#define VSHDCTRL_H

#include "control.h"
#include "shade.h"

CLASS VArchiver;
CLASS VWinInfo;

extern VClass *VShadeControlCls;

CLASS VShadeControl : public VControl {
public:
	enum		ShadeStyle		{ HDip, HBump, VDip, VBump,
								  Raised, Indented, Flat, Pressed };

				VShadeControl(const VFrame& f, VWindow *parent,
								ShadeStyle ss = VShadeControl::Raised);
				VShadeControl();
				~VShadeControl();

	VClass	   *iam();
	boolean		paint();
	boolean		erased();

	VShade& 	getShade();
	ShadeStyle 	getStyle();

	void		setStyle(ShadeStyle ss);
	void		getMinSize(short& w, short& h);

	void		getInfo(VWinInfo&);

    virtual void	getInternalBorderGeometry(int *, int *,
											  int *, int *) const;

#ifndef CV_NOARCHIVER
	void  		putTo(VArchiver& a);
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *pwin,
						boolean do_realize = TRUE);
	void		realize(VWindow *pwin);
#endif

#if defined(MOTIF)
protected:
	virtual void	virtSetupBackground(VBrush *);
	virtual void	addChild(VWindow *);
	virtual boolean	removeChild(VWindow *);
#endif
    virtual void    systemColorChanged();

private:
	boolean		isPanel();

#ifndef CV_NOARCHIVER
	void		putTo(VArchiver& a, boolean most_derived);
#endif

private:
	VShade		theShade;
	ShadeStyle	style;
};

// ---------------------------------------------------------------------------
//
inline VShade& VShadeControl::getShade()
{
	return theShade;
}

// ---------------------------------------------------------------------------
//
inline VShadeControl::ShadeStyle VShadeControl::getStyle()
{
	return style;
}

// ---------------------------------------------------------------------------
//
inline void VShadeControl::setStyle(VShadeControl::ShadeStyle ss)
{
	style = ss;
}

#endif // VSHDCTRL_H
