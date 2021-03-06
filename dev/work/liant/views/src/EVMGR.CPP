//	evmgr.cpp
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

#include "evmgr.h"
#include "evnode.h"
#include "evtype.h"

// --------------------------------------------------------------------------
// Class identification.
//
defineClass(VEventManager, VObject)

// --------------------------------------------------------------------------
//
VEventManager::~VEventManager()
{
	for (VEventNode *node = eventList ; node != 0 ; ) {
		VEventNode *doomed = node;
		node = node->next;
		delete doomed;
	}
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::defineEvent(const VEventType& event)
{
	VEventNode *node = findEventNode(event);
	if (node == 0) {
		addEventNode(node = new VEventNode(event));
	}
	return node;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::defineEvent(const char *name)
{
	VEventNode *node = findEventNode(name);
	if (node == 0) {
		addEventNode(node = new VEventNode(name));
	}
	return node;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::defineEvent(unsigned long id)
{
	VEventNode *node = findEventNode(id);
	if (node == 0) {
		addEventNode(node = new VEventNode(id));
	}
	return node;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::undefineEvent(const VEventType& event)
{
	VEventNode *node = findEventNode(event);
	if (node != 0) {
		deleteEventNode(node);
		return TRUE;
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::undefineEvent(const char *name)
{
	VEventNode *node = findEventNode(name);
	if (node != 0) {
		deleteEventNode(node);
		return TRUE;
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::undefineEvent(unsigned long id)
{
	VEventNode *node = findEventNode(id);
	if (node != 0) {
		deleteEventNode(node);
		return TRUE;
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::subscribeEvent(const VEventType& event,
										  VObject *o, method m, void *data)
{
	VEventNode *node = defineEvent(event);
	node->addCallback(o, m, data);
	return node;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::subscribeEvent(const char *name,
										  VObject *o, method m, void *data)
{
	VEventNode *node = defineEvent(name);
	node->addCallback(o, m, data);
	return node;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::subscribeEvent(unsigned long id,
										  VObject *o, method m, void *data)
{
	VEventNode *node = defineEvent(id);
	node->addCallback(o, m, data);
	return node;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::unsubscribeEvent(const VEventType& event,
											VObject *o, method m, void *data)
{
	VEventNode *node = findEventNode(event);
	if (node != 0) {
		return node->removeCallback(o, m, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::unsubscribeEvent(const char *name,
											VObject *o, method m, void *data)
{
	VEventNode *node = findEventNode(name);
	if (node != 0) {
		return node->removeCallback(o, m, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::unsubscribeEvent(unsigned long id,
											VObject *o, method m, void *data)
{
	VEventNode *node = findEventNode(id);
	if (node != 0) {
		return node->removeCallback(o, m, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::triggerEvent(const VEventType& event,
									void *subscriber, void *data)
{
	VEventNode *node = findEventNode(event);
	if (node != 0) {
		return node->notifyCallbacks(subscriber, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::triggerEvent(const char *name,
									void *subscriber, void *data)
{
	VEventNode *node = findEventNode(name);
	if (node != 0) {
		return node->notifyCallbacks(subscriber, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::triggerEvent(unsigned long id,
									void *subscriber, void *data)
{
	VEventNode *node = findEventNode(id);
	if (node != 0) {
		return node->notifyCallbacks(subscriber, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VEventManager::triggerEvent(VEventNode *node,
									void *subscriber, void *data)
{
	if (node != 0) {
		return node->notifyCallbacks(subscriber, data);
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::findEventNode(const VEventType& event) const
{
	for (VEventNode *node = eventList ; node != 0 ; node = node->next) {
		if (node->getType() == event) {
			return node;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::findEventNode(const char *name) const
{
	for (VEventNode *node = eventList ; node != 0 ; node = node->next) {
		if (node->getType() == name) {
			return node;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
VEventNode *VEventManager::findEventNode(unsigned long id) const
{
	for (VEventNode *node = eventList ; node != 0 ; node = node->next) {
		if (node->getType() == id) {
			return node;
		}
	}
	return 0;
}

// --------------------------------------------------------------------------
//
void VEventManager::objectDeleted(void *object)
{
	for (VEventNode *node = eventList ; node != 0 ; node = node->next) {
		node->objectDeleted(object);
	}
}

// --------------------------------------------------------------------------
//
void VEventManager::addEventNode(VEventNode *node)
{
	if (eventList == 0) {
		eventList = node;
	}
	else {
		eventList->prev->next = node;
		node->prev = eventList->prev;
	}
	eventList->prev = node;
}

// --------------------------------------------------------------------------
//
void VEventManager::deleteEventNode(VEventNode *node)
{
	if (eventList == node) {
		eventList->prev = node->next;
	}
	node->prev->next = node->next;
	delete node;
}
