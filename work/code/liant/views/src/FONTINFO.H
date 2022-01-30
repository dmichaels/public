// $Header:   Q:/views/common/vcs/FONTINFO.H_V   1.9   Mar 24 1997 16:08:56   scottt  $ 

//	fontinfo.h
//
//	VFontInfo class interface [Common]
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
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	05/16/95 pkt	preened
//  09/12/95 evc    Added cross platform font information and access
//	10/04/95 evc	Added stockobject member:
//	10/12/95 tlf    Moved declaration of "this_platform" to the top of
//                      the class, to preceed references to it.
//	06/12/96 pkt	added class VFont name declaration.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VFONTINFO_H
#define VFONTINFO_H

#include "object.h"
#include "str.h"

CLASS VFont;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

#define PLATFORMS 3
extern VClass *VFontInfoCls;

CLASS VFontInfo : public VObject {

public:
enum Platform { first_platform,  mswin=first_platform, 
os2 , motif, last_platform = motif } ;

	static Platform this_platform; 
					VFontInfo();
					VFontInfo(const VFont *font);
					~VFontInfo();
					VFontInfo(const VFontInfo&); // copy constructor 

					VFontInfo& operator= (const VFontInfo&);

	VClass 	   	   *iam();


	void 			orientation(Orient o, Platform p= this_platform);
	Orient			orientation(Platform p=this_platform) const;
	void 			pointSize(int ps, Platform p=this_platform);
	int 			pointSize(Platform p=this_platform) const;
	void 			bold(boolean b, Platform p=this_platform);
	boolean	    	bold(Platform p=this_platform) const;	
	void 			italic(boolean i, Platform p=this_platform);
	boolean	    	italic(Platform p=this_platform) const;
	void 			underline(boolean u, Platform p=this_platform);
	boolean	    	underline(Platform p=this_platform) const;
	void 			strikeOut(boolean so, Platform p=this_platform);
	boolean	    	strikeOut(Platform p=this_platform) const;
	void 			face(const VString& f, Platform p=this_platform);
	const VString&	face(Platform p=this_platform) const;
	void 			stockObject( int value, Platform p=this_platform);
	int 			stockObject(Platform p=this_platform) const;


	// Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
#endif


protected:

	VString 	typeface[PLATFORMS]; 
	int 		pts[PLATFORMS]; 
	Orient		orient[PLATFORMS];
	boolean 	isBold[PLATFORMS];
	boolean 	isItalic[PLATFORMS];
	boolean 	isUnderLine[PLATFORMS];
	boolean 	isStrikeOut[PLATFORMS];
	int			stockObjectId[PLATFORMS];
	

private:

	void init(const VFont *font);
};

// ---------------------------------------------------------------------------
//
inline void VFontInfo::orientation(Orient o, Platform p)
{
	orient[p] = o;
}

// ---------------------------------------------------------------------------
//
inline Orient VFontInfo::orientation(Platform p) const
{
	return orient[p];	
}

// ---------------------------------------------------------------------------
//
inline void VFontInfo::pointSize(int ps, Platform p)
{
	pts[p] = ps;
}

// ---------------------------------------------------------------------------
//
inline int VFontInfo::pointSize(Platform p) const
{
	return pts[p];
}

// ---------------------------------------------------------------------------
//
inline void VFontInfo::bold(boolean b, Platform p)
{
	isBold[p] = b;
}

// ---------------------------------------------------------------------------
//
inline boolean VFontInfo::bold(Platform p) const
{
	return isBold[p];
}

// ---------------------------------------------------------------------------
//
inline void VFontInfo::italic(boolean i, Platform p)
{
	isItalic[p] = i;
}

// ---------------------------------------------------------------------------
//
inline boolean VFontInfo::italic(Platform p) const
{
	return isItalic[p];
}

// ---------------------------------------------------------------------------
//
inline void VFontInfo::underline(boolean u, Platform p)
{
	isUnderLine[p]= u;
}

// ---------------------------------------------------------------------------
//
inline boolean VFontInfo::underline(Platform p) const
{
	return isUnderLine[p];
}

// ---------------------------------------------------------------------------
//
inline void VFontInfo::strikeOut(boolean so, Platform p)
{
	isStrikeOut[p] = so;
}

// ---------------------------------------------------------------------------
//
inline boolean VFontInfo::strikeOut(Platform p) const
{
	return isStrikeOut[p];
}

// ---------------------------------------------------------------------------
//
inline void VFontInfo::face(const VString& f, Platform p)
{
	typeface[p] = f;
}

// ---------------------------------------------------------------------------
//
inline const VString& VFontInfo::face(Platform p) const
{
	return typeface[p];
}

// ---------------------------------------------------------------------------
inline void VFontInfo::stockObject( int value, Platform p)
{
	stockObjectId[p] = value;
}

// ---------------------------------------------------------------------------
//
inline int VFontInfo::stockObject(Platform p) const
{
	return stockObjectId[p];
}



#endif	// !VFONTINFO_H 
