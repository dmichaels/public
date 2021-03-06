// $Header:   Q:/views/common/vcs/GEOROW.H_V   1.4   Mar 24 1997 16:09:16   scottt  $

//	georow.h
//
//	VGeoRow interface [Common]
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
//	09/29/95 dgm	Original.
//	10/02/95 dgm	Updated.
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VGEOROW_H
#define VGEOROW_H

// --------------------------------------------------------------------------
//
#include "georoc.h"

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VFrame;
CLASS VObject;
CLASS VWindow;

#if !defined(CV_NOARCHIVER)
CLASS VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VGeoRow : public VGeoRowOrColumn {
public:
						VGeoRow(const VFrame&, VGeoManager *parent);
						VGeoRow(int x, int y, int w, int h,
								VGeoManager *parent);
						VGeoRow(int w, int h, VGeoManager *parent);
						VGeoRow(VGeoManager *parent);
						VGeoRow(const VFrame&, VWindow *parent_window);
						VGeoRow(int x, int y, int w, int h,
								VWindow *parent_window);
						VGeoRow(int w, int h, VWindow *parent_window);
						VGeoRow(VWindow *parent_window);
						VGeoRow();
	VClass			   *iam();

	int					getPreferredNumberOfRows() const;
	void				setPreferredNumberOfRows(int);

#if !defined(CV_NOARCHIVER)
	virtual void		putTo(VArchiver&);
	virtual void		getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:

#if !defined(CV_NOARCHIVER)
    virtual void		putTo(VArchiver&, boolean);
	virtual void		getFrom(VArchiver&);
#endif

private:
						VGeoRow(const VGeoRow&);
	VGeoRow&			operator=(const VGeoRow&);
};

extern VClass *VGeoRowCls;

// --------------------------------------------------------------------------
//
inline int VGeoRow::getPreferredNumberOfRows() const
{
	return VGeoRowOrColumn::getPreferredNumberOfRowsOrColumns();
}

// --------------------------------------------------------------------------
//
inline void VGeoRow::setPreferredNumberOfRows(int n)
{
	VGeoRowOrColumn::setPreferredNumberOfRowsOrColumns(n);
}

#endif // VGEOROW_H
