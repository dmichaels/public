// $Header:   Q:/views/common/vcs/BRUSHINF.H_V   1.6   Mar 24 1997 16:08:18   scottt  $ 

//	brushinf.h
//
//	VBrushInfo class interface [Common]
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
//	09/01/93 pmcm	original
//	09/23/93 pmcm	added isDefaultBrush
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	11/03/93 pkt	removed 'enum' from return type declaration.
//	01/17/94 dgm	cfront C++ changes
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	08/27/94 pkt	updated for Metrowerks C++.
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VBRUSHINFO_H
#define VBRUSHINFO_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include "color.h"

// --------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

// --------------------------------------------------------------------------
//
extern VClass *VBrushInfoCls;

CLASS VBrushInfo : public VObject {
public:
						VBrushInfo();
						VBrushInfo(const VBrush *brush);
						~VBrushInfo();

	VClass 			   *iam();

	void				background(const VColor& c);
	const VColor& 		background() const;
	void				foreground(const VColor& c);
	const VColor& 		foreground() const;
	void				pattern(FillPattern f);
	FillPattern			pattern() const;
	void				isDefault(boolean tf);
	boolean				isDefault() const;

#ifndef CV_NOARCHIVER
	void				putTo(VArchiver& a);
	void				getFrom(VArchiver& a);
	void				getFrom(VArchiver& a, VObject *data,
								boolean do_realize = TRUE);
#endif 

protected:

	FillPattern		 	fillPattern;
	VColor				fgColor;
	VColor				bgColor;
	boolean				isDefaultBrush;

private:

	void init(const VBrush *brush);

};

// ---------------------------------------------------------------------------
//
inline const VColor& VBrushInfo::background() const
{
	return bgColor;
}

// ---------------------------------------------------------------------------
//
inline void VBrushInfo::background(const VColor& color)
{
	bgColor = color;
}

// ---------------------------------------------------------------------------
//
inline const VColor& VBrushInfo::foreground() const
{
	return fgColor;
}

// ---------------------------------------------------------------------------
//
inline void VBrushInfo::foreground(const VColor& color)
{
	fgColor = color;
}

// ---------------------------------------------------------------------------
//
inline FillPattern VBrushInfo::pattern() const
{
	return fillPattern;
}

// ---------------------------------------------------------------------------
//
inline void VBrushInfo::pattern(FillPattern f)
{
	fillPattern = f;
}

// ---------------------------------------------------------------------------
//
inline void VBrushInfo::isDefault(boolean tf)
{
	isDefaultBrush = tf;
}

// ---------------------------------------------------------------------------
//
inline boolean VBrushInfo::isDefault() const
{
	return isDefaultBrush;
}

#endif // VBRUSHINFO_H
