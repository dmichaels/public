//	objtype.h -- [UNDER DEVELOPMENT]
//
//	VObjectType interface
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

#ifndef VOBJTYPE_H
#define VOBJTYPE_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include <string.h>

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VObject;

// --------------------------------------------------------------------------
// This is used to represent object types which may be transferred, via
// drag-and-drop, clipboard (or any other method) both within the same
// application as well as between different applications, of which only
// one need be a C++/Views application.
//
CLASS VObjectType : public VObject {
public:
	typedef unsigned long	Handle;

public:
							VObjectType();
							VObjectType(Handle);
							VObjectType(const VClass *);
							VObjectType(const char *, int name_space = 0);
							VObjectType(const VObjectType&);
						   ~VObjectType();
	VClass				   *iam();

	void					set(const char *, int name_space = 0);
	void					set(const VClass *);
	void					set(const VObjectType&);
	VObjectType&			operator=(const char *);
	VObjectType&			operator=(const VClass *);
	VObjectType&			operator=(const VObjectType&);

	const char			   *getName() const;
	operator				const char *() const;
	VClass				   *getClass() const;
	operator				VClass *() const;
	Handle					getHandle() const;
	operator				Handle() const;

	boolean					operator==(const VObjectType&) const;
	boolean					operator!=(const VObjectType&) const;
	boolean					operator==(const VClass *) const;
	boolean					operator!=(const VClass *) const;
	boolean					operator==(const char *) const;
	boolean					operator!=(const char *) const;
	boolean					operator==(Handle) const;
	boolean					operator!=(Handle) const;
	boolean					operator==(int) const;
	boolean					operator!=(int) const;

	boolean					isViewsType() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Private section; please KEEP OUT!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	void					construct(const char *, int);
	void					construct(const VClass *);
	void					realize();
	void					destroy();
	void					setupTypeName();

private:
	Handle					typeHandle;
	VClass				   *typeClass;
	char				   *typeName;
	short					nameSpace;
};

extern VClass *VObjectTypeCls;

#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
inline VObjectType::VObjectType()
{
	typeHandle	= 0;
	typeClass	= 0;
	typeName	= 0;
	nameSpace	= 0;
}

// --------------------------------------------------------------------------
//
inline VObjectType::VObjectType(VObjectType::Handle type_handle)
{
	typeHandle	= type_handle;
	typeClass	= 0;
	typeName	= 0;
	nameSpace	= 0;
}

// --------------------------------------------------------------------------
//
inline VObjectType::VObjectType(const VClass *type_class)
{
	construct(type_class);
}

// --------------------------------------------------------------------------
//
inline VObjectType::VObjectType(const char *type_name, int name_space)
{
	construct(type_name, name_space);
}

// --------------------------------------------------------------------------
//
inline VObjectType::~VObjectType()
{
	destroy();
}

// --------------------------------------------------------------------------
//
inline void VObjectType::construct(const VClass *type_class)
{
	typeHandle	= 0;
	typeClass	= (VClass *)type_class;
	typeName	= 0;
	nameSpace	= 0;
}

// --------------------------------------------------------------------------
//
inline const char *VObjectType::getName() const
{
	if (typeName == 0) {
		((VObjectType *)this)->setupTypeName();
	}
	return typeName;
}

// --------------------------------------------------------------------------
//
inline VObjectType::operator const char *() const
{
	return getName();
}

// --------------------------------------------------------------------------
//
inline VClass *VObjectType::getClass() const
{
	if (typeClass != 0) {
		return typeClass;
	}

	((VObjectType *)this)->setupTypeName();

	if ((typeName[0] == 'C') &&
		(typeName[1] == 'V') &&
		(typeName[2] == '_') &&
		(typeName[3] != '\0')) {
		((VObjectType *)this)->typeClass =
			VClass::of((char *)(&typeName[3]));
		return typeClass;
	}

	return 0;
}

// --------------------------------------------------------------------------
//
inline VObjectType::operator VClass *() const
{
	return getClass();
}

// --------------------------------------------------------------------------
//
inline VObjectType::Handle VObjectType::getHandle() const
{
	if (typeHandle == 0) {
		((VObjectType *)this)->realize();
	}
	return typeHandle;
}

// --------------------------------------------------------------------------
//
inline VObjectType::operator VObjectType::Handle() const
{
	return getHandle();
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator==(int i) const
{
	return getHandle() == Handle(i);
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator!=(int i) const
{
	return !operator==(i);
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator==(VObjectType::Handle h) const
{
	return getHandle() == h;
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator!=(VObjectType::Handle h) const
{
	return !operator==(h);
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator==(const VClass *c) const
{
	return c == typeClass;
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator!=(const VClass *c) const
{
	return !operator==(c);
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator!=(const VObjectType& ot) const
{
	return !operator==(ot);
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::operator!=(const char *s) const
{
	return !operator==(s);
}

// --------------------------------------------------------------------------
//
inline boolean VObjectType::isViewsType() const
{
	return getClass() != 0;
}

#else // !CV_OLE_DRAG_DROP || CV_WIN16

inline VObjectType::VObjectType() { }
inline VObjectType::VObjectType(VObjectType::Handle) { }
inline VObjectType::VObjectType(const VClass *) { }
inline VObjectType::VObjectType(const char *, int) { }
inline VObjectType::VObjectType(const VObjectType&) { }
inline VObjectType::~VObjectType() { }
inline void VObjectType::set(const char *, int) { }
inline void VObjectType::set(const VClass *) { }
inline void VObjectType::set(const VObjectType&) { }
inline VObjectType& VObjectType::operator=(const char *) { return *this; }
inline VObjectType& VObjectType::operator=(const VClass *) { return *this; }
inline VObjectType& VObjectType::operator=(const VObjectType&) { return *this; }
inline const char *VObjectType::getName() const { return 0; }
inline VObjectType::operator const char *() const { return 0; }
inline VClass *VObjectType::getClass() const { return 0; }
inline VObjectType::operator VClass *() const { return 0; }
inline VObjectType::Handle VObjectType::getHandle() const { return 0; }
inline VObjectType::operator VObjectType::Handle() const { return 0; }
inline boolean VObjectType::operator==(const VObjectType&) const { return 0; }
inline boolean VObjectType::operator!=(const VObjectType&) const { return 0; }
inline boolean VObjectType::operator==(const VClass *) const { return 0; }
inline boolean VObjectType::operator!=(const VClass *) const { return 0; }
inline boolean VObjectType::operator==(const char *) const { return 0; }
inline boolean VObjectType::operator!=(const char *) const { return 0; }
inline boolean VObjectType::operator==(VObjectType::Handle) const { return 0; }
inline boolean VObjectType::operator!=(VObjectType::Handle) const { return 0; }
inline boolean VObjectType::operator==(int) const { return 0; }
inline boolean VObjectType::operator!=(int) const { return 0; }
inline boolean VObjectType::isViewsType() const { return 0; }
inline void VObjectType::construct(const char *, int) { }
inline void VObjectType::construct(const VClass *) { }
inline void VObjectType::realize() { }
inline void VObjectType::destroy() { }
inline void VObjectType::setupTypeName() { }

#endif // !CV_OLE_DRAG_DROP || CV_WIN16

#endif // VOBJTYPE_H
