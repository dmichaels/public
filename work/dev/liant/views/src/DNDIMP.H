//	dndimp.h -- [UNDER DEVELOPMENT]
//
//	VDragSiteImp & VDropSiteImp interfaces [MS/Windows, OLE, Internal]
//
//	Allegris Foundation 1.1.00 for MS/Windows
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
//	06/28/95 dgm	Original
// --------------------------------------------------------------------------

#ifndef VDNDIMP_H
#define VDNDIMP_H

// --------------------------------------------------------------------------
//
#include "window.h"

// --------------------------------------------------------------------------
//
CLASS	VDragSite;
CLASS	VDropSite;
CLASS	VDragSiteImp;
CLASS	VDropSiteImp;
CLASS	VMouseCursor;
CLASS	VNotifier;
CLASS	VObjectCourier;
CLASS	VOleDataObject;
CLASS	VOleDropSource;
CLASS	VOleDropTarget;
CLASS	VWindow;

// --------------------------------------------------------------------------
//
CLASS VDragSiteImp {
private:
							VDragSiteImp(VDragSite *);
private:
	boolean					registerDragSite();
	void					unregisterDragSite();
private:
	boolean					dragLoop(VObjectCourier *);
	boolean					dragFeedback(long);
private:
	VDragSite			   *dragSite;
	VOleDropSource		   *oleDropSource;
	VOleDataObject		   *oleDataObject;
private:
	friend					VDragSite;
	friend					VNotifier;
	friend					VOleDropSource;
};

// --------------------------------------------------------------------------
//
CLASS VDropSiteImp {
private:
							VDropSiteImp(VDropSite *);
						   ~VDropSiteImp();
private:
	boolean					registerDropSite();
	void					unregisterDropSite();
private:
	long					dragObjectEnter();
	void					dragObjectExit();
	long					dragObjectOver(int, int);
	long					objectDropped(int, int);
	VObjectCourier		   *getObjectCourier() const;
	void					setObjectCourier(VObjectCourier *);
	HWND					getWindowHandle() const;
	boolean					isDragTracking() const;
private:
	VDropSite			   *dropSite;
	VOleDropTarget		   *oleDropTarget;
private:
	friend					VDropSite;
	friend					VOleDropTarget;
};

#if defined(CV_OLE_DRAG_DROP) && defined(CV_WIN32)

// --------------------------------------------------------------------------
//
#include "dropsite.h"

// --------------------------------------------------------------------------
//
inline long VDropSiteImp::dragObjectEnter()
{
	return dropSite->dragObjectEnter();
}

// --------------------------------------------------------------------------
//
inline void VDropSiteImp::dragObjectExit()
{
	dropSite->dragObjectExit();
}

// --------------------------------------------------------------------------
//
inline long VDropSiteImp::dragObjectOver(int x, int y)
{
	return dropSite->dragObjectOver(x, y);
}

// --------------------------------------------------------------------------
//
inline long VDropSiteImp::objectDropped(int x, int y)
{
	return dropSite->objectDropped(x, y);
}

// --------------------------------------------------------------------------
//
inline VObjectCourier *VDropSiteImp::getObjectCourier() const
{
	return dropSite->getObjectCourier();
}

// --------------------------------------------------------------------------
//
inline void VDropSiteImp::setObjectCourier(VObjectCourier *oc)
{
	dropSite->setObjectCourier(oc);
}

// --------------------------------------------------------------------------
//
inline HWND VDropSiteImp::getWindowHandle() const
{
	return dropSite->theWindow->hwnd();
}

// --------------------------------------------------------------------------
//
inline boolean VDropSiteImp::isDragTracking() const
{
	return dropSite->isDragTracking();
}

#else // !CV_OLE_DRAG_DROP || !CV_WIN32

inline VDragSiteImp::VDragSiteImp(VDragSite *) { }
inline boolean VDragSiteImp::registerDragSite() { return 0; }
inline void VDragSiteImp::unregisterDragSite() { }
inline boolean VDragSiteImp::dragLoop(VObjectCourier *) { return 0;}
inline boolean VDragSiteImp::dragFeedback(long) { return 0; }
inline VDropSiteImp::VDropSiteImp(VDropSite *) { }
inline VDropSiteImp::~VDropSiteImp() { }
inline boolean VDropSiteImp::registerDropSite() { return 0; }
inline void VDropSiteImp::unregisterDropSite() { }
inline long VDropSiteImp::dragObjectEnter() { return 0; }
inline void VDropSiteImp::dragObjectExit() { }
inline long VDropSiteImp::dragObjectOver(int, int) { return 0; }
inline long VDropSiteImp::objectDropped(int, int) { return 0; }
inline VObjectCourier *VDropSiteImp::getObjectCourier() const { return 0; }
inline void VDropSiteImp::setObjectCourier(VObjectCourier *) { }
inline HWND VDropSiteImp::getWindowHandle() const { return 0; }
inline boolean VDropSiteImp::isDragTracking() const { return 0; }

#endif // !CV_OLE_DRAG_DROP || !CV_WIN32

#endif // VDNDIMP_H
