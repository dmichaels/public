//	objcour.h -- [UNDER DEVELOPMENT]
//
//	VObjectCourier interface
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
//	06/28/95 dgm	Original
// --------------------------------------------------------------------------

#ifndef VOBJCOUR_H
#define VOBJCOUR_H

// --------------------------------------------------------------------------
//
#include "object.h"

// --------------------------------------------------------------------------
//
CLASS	VArchiver;
CLASS	VClass;
CLASS	VObject;
CLASS	VObjectType;
CLASS	VOrdCollect;

// --------------------------------------------------------------------------
//
CLASS VObjectCourier : public VObject {
public:
	enum Operation {
							NoOp	= 0x0001,
							CopyOp	= 0x0002,
							MoveOp	= 0x0004,
							LinkOp	= 0x0008,
							AnyOp	= 0x0010 | CopyOp | MoveOp | LinkOp,
	};

public:
							VObjectCourier();
						   ~VObjectCourier();
	VClass				   *iam();

	virtual void			setObject(const VObject *);
	virtual VObject		   *getObject(const VClass * = 0) const;
	virtual boolean			contains(const VClass *) const;
	virtual boolean			contains(const VObjectType&) const;
	virtual VOrdCollect	   *getTypes() const;

	void					setOperation(long);
	long					getOperation() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	void					archiveObject(const VObject *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	void					setupTypes(const VObject * = 0);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Private section; please KEEP OUT!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	union {
		VObject			   *objectPointer;
		VArchiver		   *objectArchiver;
		void			   *objectPointerOrArchiver;
	};
	VOrdCollect		  	   *objectTypeList;
	short					objectOperation;
	short					usingObjectArchiver : 1;
};

extern VClass *VObjectCourierCls;

#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
inline void VObjectCourier::setOperation(long op)
{
	objectOperation = short(op);
}

// --------------------------------------------------------------------------
//
inline long VObjectCourier::getOperation() const
{
	return objectOperation;
}

// --------------------------------------------------------------------------
//
inline VOrdCollect *VObjectCourier::getTypes() const
{
	((VObjectCourier *)this)->setupTypes();
	return objectTypeList;
}

#else // !CV_OLE_DRAG_DROP || CV_WIN16

inline VObjectCourier::VObjectCourier() { }
inline VObjectCourier::~VObjectCourier() { }
inline void VObjectCourier::setObject(const VObject *) { }
inline VObject *VObjectCourier::getObject(const VClass *) const { return 0; }
inline boolean VObjectCourier::contains(const VClass *) const { return 0; }
inline boolean VObjectCourier::contains(const VObjectType&) const { return 0; }
inline VOrdCollect *VObjectCourier::getTypes() const { return 0; }
inline void VObjectCourier::setOperation(long) { }
inline long VObjectCourier::getOperation() const { return 0; }
inline void VObjectCourier::archiveObject(const VObject *) { }
inline void VObjectCourier::setupTypes(const VObject *) { }

#endif // !CV_OLE_DRAG_DROP || CV_WIN16

#endif // VOBJCOUR_H
