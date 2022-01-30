// $Header:   Q:/views/common/vcs/bitmapvw.h_v   1.7   May 30 1997 17:14:32   scottt  $ 

//	bitmapvw.h
//
//	VBitMapView class interface [Common]
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
//	05/11/93 jmd	added getMinSize()
//	05/20/93 jmd	preened
//	07/22/93 pmcm	added constructor that takes name
//	07/23/93 pmcm	removed free()
//	07/23/93 pmcm	added getPalette() virtual override
//	08/09/93 jmd	made VFrame const
//	05/16/95 pkt	preened.
//	05/29/95 atb	ifdef'ed getPalette() for CV_WINDOWS only
//	09/01/95 dss	VStyle changes.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VBITMAPVW_H
#define VBITMAPVW_H

#include "view.h"

extern VClass *VBitMapViewCls;

CLASS VBitMap;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

CLASS VBitMapView : public VView {

public:
				VBitMapView();

				VBitMapView(const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault);

				VBitMapView(const VString& name, 
							const VFrame& frame, 
							VWindow *pwin, 
							const VStyle& style = StyleDefault);

				~VBitMapView();

	VBitMap    *getBitMap();
	void	 	setBitMap(VBitMap *bm);

	VClass 	   *iam();

	boolean		resized(int w, int h);
	boolean		paint();
	boolean		erased();

	void		getMinSize(short &w, short &h);

	// GUI Palette (non-portable, MS-Windows specific), virtual override
		
#ifdef CV_WINDOWS		
	HPALETTE 	getPalette();
#endif

	// Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

private:

	VBitMap	   *bmap;
	int			xOffset;
	int			yOffset;

	boolean		vtScrollBar(int i);
	boolean		hzScrollBar(int i);
	void		updateScrollBars();
	void		scrollBitMap(int dx, int dy);
};

// ---------------------------------------------------------------------------
//
inline VBitMap *VBitMapView::getBitMap()
{
	return bmap;
}

#endif // VBITMAPVW_H

