// $Header:   Q:/views/common/vcs/GEOCOL.H_V   1.4   Mar 24 1997 16:09:00   scottt  $

//	geocol.h
//
//	VGeoColumn interface [Common]
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

#ifndef VGEOCOL_H
#define VGEOCOL_H

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
CLASS VGeoColumn : public VGeoRowOrColumn {
public:
						VGeoColumn(const VFrame&, VGeoManager *parent);
						VGeoColumn(int x, int y, int w, int h,
								   VGeoManager *parent);
						VGeoColumn(int w, int h, VGeoManager *parent);
						VGeoColumn(VGeoManager *parent);
						VGeoColumn(const VFrame&, VWindow *parent_window);
						VGeoColumn(int x, int y, int w, int h,
								VWindow *parent_window);
						VGeoColumn(int w, int h, VWindow *parent_window);
						VGeoColumn(VWindow *parent_window);
						VGeoColumn();
	VClass			   *iam();

	int					getPreferredNumberOfColumns() const;
	void				setPreferredNumberOfColumns(int);

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
						VGeoColumn(const VGeoColumn&);
	VGeoColumn&			operator=(const VGeoColumn&);
};

extern VClass *VGeoColumnCls;

// --------------------------------------------------------------------------
//
inline int VGeoColumn::getPreferredNumberOfColumns() const
{
	return VGeoRowOrColumn::getPreferredNumberOfRowsOrColumns();
}

// --------------------------------------------------------------------------
//
inline void VGeoColumn::setPreferredNumberOfColumns(int n)
{
	VGeoRowOrColumn::setPreferredNumberOfRowsOrColumns(n);
}

#endif // VGEOCOL_H
