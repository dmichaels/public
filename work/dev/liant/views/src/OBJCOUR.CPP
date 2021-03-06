//	objcour.cpp -- [UNDER DEVELOPMENT]
//
//	VObjectCourier implementation
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

#include "objcour.h"
#include "objtype.h"
#include "ordcllct.h"
#include "iterator.h"
#include "archiver.h"

// --------------------------------------------------------------------------
//
defineClass(VObjectCourier, VObject)

// --------------------------------------------------------------------------
//
#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
VObjectCourier::VObjectCourier()
{
	objectTypeList = 0;
	objectPointerOrArchiver = 0;
	objectOperation = MoveOp;
	usingObjectArchiver = 0;
}

// --------------------------------------------------------------------------
//
VObjectCourier::~VObjectCourier()
{
	if (objectTypeList != 0) {
		objectTypeList->freeContents();
		delete objectTypeList;
	}
}

// --------------------------------------------------------------------------
// Set the object of this VObjectCourier to refer to the given VObject.
// The type(s) supported by this VObjectCourier will automatically be set
// according to the class(es) of the given VObject; specifically, the type(s)
// will be the type (class) of the VObject itself plus the type of VObject's
// base class, if any, and so on up to most base class (i.e. VObject).
//
void VObjectCourier::setObject(const VObject *object)
{
	if (objectPointer == object) {
		return;
	}
	if (objectTypeList != 0) {
		objectTypeList->freeContents();
		delete objectTypeList;
		objectTypeList = 0;
	}
	objectPointer = (VObject *)object;
}

// --------------------------------------------------------------------------
//
void VObjectCourier::archiveObject(const VObject *object)
{
	if (object == 0) {
		return;
	}
	usingObjectArchiver = 1;
	if (objectArchiver != 0) {
		delete objectArchiver;
	}
	if (object == 0) {
		objectArchiver = 0;
		return;
	}
	objectArchiver = new VArchiver;
	objectArchiver->putObject((VObject *)object);
	setupTypes(object);
}

// --------------------------------------------------------------------------
//
void VObjectCourier::setupTypes(const VObject *object)
{
	if (objectTypeList != 0) {
		return;
	}
	if (object == 0) {
		if ((objectPointer == 0) || usingObjectArchiver) {
			return;
		}
		object = objectPointer;
	}
	VClass *object_class = ((VObject *)object)->iam();
	if (object_class == 0) {
		return;
	}
	objectTypeList = new VOrdCollect;
	objectTypeList->add(new VObjectType(object_class));
	while ((object_class = object_class->getBase()) != 0) {
		objectTypeList->add(new VObjectType(object_class));
	}
}

// --------------------------------------------------------------------------
//
boolean VObjectCourier::contains(const VObjectType& object_type) const
{
	((VObjectCourier *)this)->setupTypes();
	if ((objectTypeList == 0) || (objectTypeList->count() <= 0)) {
		return FALSE;
	}
	DO (*objectTypeList, VObjectType, ot)
		if (*ot == object_type) {
			return TRUE;
		}
	END
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VObjectCourier::contains(const VClass *object_class) const
{
	if (object_class == 0) {
		return FALSE;
	}
	((VObjectCourier *)this)->setupTypes();
	if ((objectTypeList == 0) || (objectTypeList->count() <= 0)) {
		return FALSE;
	}
	DO (*objectTypeList, VObjectType, ot)
		if (*ot == object_class) {
			return TRUE;
		}
	END
	return FALSE;
}

// --------------------------------------------------------------------------
//
VObject *VObjectCourier::getObject(const VClass *object_class) const
{
	if (usingObjectArchiver) {
		if (objectArchiver != 0) {
			VObject *object = objectArchiver->getObject(0);
			if (object != 0) {
				if ((object_class == 0) ||
					 object->isA((VClass *)object_class)) {
					return object;
				}
				else {
					delete object;
					return 0;
				}
			}
			else {
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	else {
		if ((object_class == 0) ||
			 objectPointer->isA((VClass *)object_class)) {
			return (VObject *)objectPointer;
		}
		else {
			return 0;
		}
	}
}

#endif // CV_OLE_DRAG_DROP && !CV_WIN16
