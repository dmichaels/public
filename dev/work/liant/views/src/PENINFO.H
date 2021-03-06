// $Header:   Q:/views/common/vcs/PENINFO.H_V   1.6   Mar 24 1997 16:12:54   scottt  $ 

//	peninfo.h
//
//	VPenInfo class interface [Common]
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
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	11/03/93 pkt	removed 'enum' from return type declaration.
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	05/19/95 pkt	merged for common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VPENINFO_H
#define VPENINFO_H

#include "object.h"
#include "color.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VPenInfoCls;

CLASS VPenInfo : public VObject {

public:
					VPenInfo();
					VPenInfo(const VPen *pen);
					~VPenInfo();

	VClass 	   	   *iam();	

	void	   		color(const VColor& clr);
	const VColor&	color() const;
	void	   		pattern(LineType pat);
	LineType	 	pattern() const;
	void			width(int w);
	int				width() const;

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif

protected:

	int				penSize;
	LineType	 	penPat;	
	VColor			penColor;

private:

	void init(const VPen *pen);

};

// ---------------------------------------------------------------------------
//
inline const VColor& VPenInfo::color() const
{
	return penColor;
}

// ---------------------------------------------------------------------------
//
inline void VPenInfo::color(const VColor& clr)
{
	penColor = clr;
}

// ---------------------------------------------------------------------------
//
inline LineType VPenInfo::pattern() const
{
	return penPat;
}

// ---------------------------------------------------------------------------
//
inline void VPenInfo::pattern(LineType pat) 
{
	penPat = pat;
}

// ---------------------------------------------------------------------------
//
inline int VPenInfo::width() const
{
	return penSize;
}

// ---------------------------------------------------------------------------
//
inline void VPenInfo::width(int w)
{
	penSize = w;
}

#endif // VPENINFO_H
