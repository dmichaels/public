//	evtype.cpp
//
//	VEventType implementation [Common]
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

#include "evtype.h"

// --------------------------------------------------------------------------
// Class identification.
//
defineClass(VEventType, VObject)

// --------------------------------------------------------------------------
//
static char *strnew(const char *);

// --------------------------------------------------------------------------
//
VEventType::VEventType(const char *name)
{
	eventTypeName	= strnew(name);
	eventTypeId		= 0;
}

// --------------------------------------------------------------------------
//
VEventType::VEventType(const VEventType& event)
{
	eventTypeName	= strnew(event.eventTypeName);
	eventTypeId		= event.eventTypeId;
}

// --------------------------------------------------------------------------
//
VEventType& VEventType::operator=(const VEventType& event)
{
	if (this != &event) {
		if (eventTypeName != 0) {
			delete eventTypeName;
		}
		eventTypeName	= strnew(event.eventTypeName);
		eventTypeId		= event.eventTypeId;
	}
	return *this;
}

// --------------------------------------------------------------------------
//
VEventType::~VEventType()
{
	if (eventTypeName != 0) {
		delete eventTypeName;
	}
}

// --------------------------------------------------------------------------
//
boolean VEventType::operator==(const VEventType& event) const
{
	if (this == &event) {
		return TRUE;
	}
	else if (eventTypeName != 0) {
		if (event.eventTypeName != 0) {
			for (int i = 0 ; 1 ; i++) {
				if (eventTypeName[i] != event.eventTypeName[i]) {
					return FALSE;
				}
				else if (eventTypeName[i] == '\0') {
					break;
				}
			}
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else if (event.eventTypeName != 0) {
		return FALSE;
	}
	else {
		return eventTypeId == event.eventTypeId;
	}
}

// --------------------------------------------------------------------------
//
boolean VEventType::operator==(const char *name) const
{
	if (eventTypeName == name) {
		return TRUE;
	}
	else if (eventTypeName == 0) {
		return FALSE;
	}
	for (int i = 0 ; 1 ; i++) {
		if (eventTypeName[i] != name[i]) {
			return FALSE;
		}
		else if (eventTypeName[i] == '\0') {
			break;
		}
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VEventType::isEqual(VObject *object) const
{
	if (object == 0) {
		return FALSE;
	}
	else if (object == (const VObject *)this) {
		return TRUE;
	}
	else if (!object->isA(VEventTypeCls)) {
		return FALSE;
	}
	else {
		return operator==(*((VEventType *)object));
	}
}

// --------------------------------------------------------------------------
//
HashKey VEventType::hash() const
{
	if (eventTypeName != 0) {
		HashKey hashkey = 0;
		for (const char *p = eventTypeName ; *p != '\0' ; p++) {
			hashkey += *p & 0x7F;
		}
		return hashkey & HASH_KEY_MASK;
	}
	else {
		return long(eventTypeId);
	}
}

// --------------------------------------------------------------------------
//
static char *strnew(const char *s)
{
	if (s == 0) {
		return 0;
	}
	int n = 1;
	for (const char *p = s ; *p != '\0' ; p++) {
		n++;
	}
	char *news = new char [n];
	while (--n >= 0) {
		news[n] = s[n];
	}
	return news;
}
