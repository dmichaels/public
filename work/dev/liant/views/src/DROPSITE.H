//	dropsite.h -- [UNDER DEVELOPMENT]
//
//	VDropSite interface
//
//	Allegris Foundation 1.0.03
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

#ifndef VDROPSITE_H
#define VDROPSITE_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include "objcour.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VDragSiteImp;
CLASS	VDropSiteImp;
CLASS	VNotifier;
CLASS	VObject;
CLASS	VObjectCourier;
CLASS	VWindow;

// --------------------------------------------------------------------------
//
CLASS VDropSite : public VObject {
public:
	enum					Operation {
							NoOp			= VObjectCourier::NoOp,
							CopyOp			= VObjectCourier::CopyOp,
							MoveOp			= VObjectCourier::MoveOp,
							LinkOp			= VObjectCourier::LinkOp,
							AnyOp			= VObjectCourier::AnyOp,
							InvalidOp		= 0x1000
	};

public:
							VDropSite();
							VDropSite(const VDropSite&);
						   ~VDropSite();
	VClass				   *iam();

	void					uponDragObjectEnter(VObject *, method);
	void					uponDragObjectExit(VObject *, method);
	void					uponDragObjectOver(VObject *, method);
	void					uponObjectDropped(VObject *, method);

	void					registerValidType(const VObjectType&,
											  VObject * = 0, method = 0);
	void					registerValidTypes(VOrdCollect *,
											   VObject * = 0, method = 0);
	void					registerValidOperations(long);

	boolean					isValidType(const VObjectType&) const;
	boolean					isValidType(const VClass *) const;

	void					setDragTracking(boolean);
	boolean					isDragTracking() const;

	VWindow				   *getWindow() const;
	VObjectCourier		   *getObjectCourier() const;

	boolean					registerWindow(VWindow *);
	void					unregisterWindow(VWindow *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please keep out if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	virtual long			dragObjectEnter();
	virtual void			dragObjectExit();
	virtual long			dragObjectOver(int x, int y);
	virtual long			objectDropped(int x, int y);
	virtual boolean			isValidType(VObjectCourier *) const;
	virtual int				getNumberOfValidTypes() const;
	virtual int				getValidOperations() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Private section; for internal use only!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	void					setObjectCourier(VObjectCourier *);

private:
	VWindow				   *theWindow;
	VObject				   *clientObject;
	long		 (VObject::*dragObjectEnterMethod)(VDropSite *);
	void		 (VObject::*dragObjectExitMethod)(VDropSite *);
	long		 (VObject::*dragObjectOverMethod)(VDropSite *, int, int);
	long		 (VObject::*objectDroppedMethod)(VDropSite *, int, int);
	VOrdCollect			   *validTypes;
	short					validOperations;
	short					dragTracking : 1;
	VObjectCourier		   *objectCourier;
	VDropSiteImp		   *dropSiteImp;

private:
	friend					VDropSiteImp;
	friend					VDragSiteImp;
	friend					VWindow;
	friend					VNotifier;
};

extern VClass *VDropSiteCls;

#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
inline void VDropSite::setDragTracking(boolean b)
{
	dragTracking = b;
}

// --------------------------------------------------------------------------
//
inline boolean VDropSite::isDragTracking() const
{
	return dragTracking;
}

// --------------------------------------------------------------------------
//
inline VWindow *VDropSite::getWindow() const
{
	return theWindow;
}

// --------------------------------------------------------------------------
//
inline VObjectCourier *VDropSite::getObjectCourier() const
{
	return objectCourier;
}

// --------------------------------------------------------------------------
//
inline void VDropSite::setObjectCourier(VObjectCourier *object_courier)
{
	objectCourier = object_courier;
}

#else // !CV_OLE_DRAG_DROP || CV_WIN16

inline VDropSite::VDropSite() { }
inline VDropSite::VDropSite(const VDropSite&) { }
inline VDropSite::~VDropSite() { }
inline void VDropSite::uponDragObjectEnter(VObject *, method) { }
inline void VDropSite::uponDragObjectExit(VObject *, method) { }
inline void VDropSite::uponDragObjectOver(VObject *, method) { }
inline void VDropSite::uponObjectDropped(VObject *, method) { }
inline void VDropSite::registerValidType(const VObjectType&,
										 VObject *, method) { }
inline void VDropSite::registerValidTypes(VOrdCollect *, VObject *, method) { }
inline void VDropSite::registerValidOperations(long) { }
inline boolean VDropSite::isValidType(const VObjectType&) const { return 0; }
inline boolean VDropSite::isValidType(const VClass *) const { return 0; }
inline void VDropSite::setDragTracking(boolean) { }
inline boolean VDropSite::isDragTracking() const { return 0; }
inline VWindow *VDropSite::getWindow() const { return 0; }
inline VObjectCourier *VDropSite::getObjectCourier() const { return 0; }
inline boolean VDropSite::registerWindow(VWindow *) { return 0; }
inline void VDropSite::unregisterWindow(VWindow *) { }
inline long VDropSite::dragObjectEnter() { return 0; }
inline void VDropSite::dragObjectExit() { }
inline long VDropSite::dragObjectOver(int, int) { return 0; }
inline long VDropSite::objectDropped(int, int) { return 0; }
inline boolean VDropSite::isValidType(VObjectCourier *) const { return 0; }
inline int VDropSite::getNumberOfValidTypes() const { return 0; }
inline int VDropSite::getValidOperations() const { return 0; }
inline void VDropSite::setObjectCourier(VObjectCourier *) { }

#endif // !CV_OLE_DRAG_DROP || CV_WIN16

#endif // VDROPSITE_H
