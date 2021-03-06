//	evtype.h
//
//	VEventType interface [Common]
//
//  Allegris Foundation 1.1.00
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
//	01/29/97 dgm	Original.
// --------------------------------------------------------------------------

#ifndef VEVTYPE_H
#define VEVTYPE_H

// --------------------------------------------------------------------------
//
#include "object.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VObject;

// --------------------------------------------------------------------------
//
CLASS VEventType : public VObject {
public:
						VEventType(const char *);
						VEventType(unsigned long);
						VEventType(const VEventType&);
						VEventType();
					   ~VEventType();
	VClass			   *iam();

	const char		   *getName() const;
	unsigned long		getId() const;

	boolean				isName() const;
	boolean				isId() const;

	VEventType&			operator=(const VEventType&);
	boolean				operator==(const VEventType&) const;
	boolean				operator!=(const VEventType&) const;
	boolean				operator==(const char *) const;
	boolean				operator!=(const char *) const;
	boolean				operator==(unsigned long) const;
	boolean				operator!=(unsigned long) const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:
	boolean				isEqual(VObject *) const;
	HashKey				hash() const;

protected:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	char			   *eventTypeName;
	unsigned long		eventTypeId;
};

extern VClass *VEventTypeCls;

// --------------------------------------------------------------------------
//
inline VEventType::VEventType()
{
	eventTypeName	= 0;
	eventTypeId		= 0;
}

inline VEventType::VEventType(unsigned long id)
{
	eventTypeName	= 0;
	eventTypeId		= id;
}

inline const char *VEventType::getName() const
{
	return eventTypeName;
}

inline unsigned long VEventType::getId() const
{
	return eventTypeId;
}

inline boolean VEventType::isName() const
{
	return eventTypeName != 0;
}

inline boolean VEventType::isId() const
{
	return eventTypeName == 0;
}

inline boolean VEventType::operator!=(const VEventType& event) const
{
	return !operator==(event);
}

inline boolean VEventType::operator!=(const char *name) const
{
	return !operator==(name);
}

inline boolean VEventType::operator==(unsigned long id) const
{
	return (eventTypeName == 0) && (eventTypeId == id);
}

inline boolean VEventType::operator!=(unsigned long id) const
{
	return (eventTypeName != 0) || (eventTypeId != id);
}

#endif // VEVTYPE_H
