//	evnode.cpp
//
//	VEventNode implementation [Common]
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

#include "evnode.h"
#include "evtype.h"

// ---------------------------------------------------------------------------
//
VEventNode::VEventNode()
{
	next = 0;
	prev = 0;
	callbackList = 0;
	currentCallback = 0;
	nextCallback = 0;
}

// ---------------------------------------------------------------------------
//
VEventNode::VEventNode(const VEventType& event)
	: eventType(event)
{
	next = 0;
	prev = 0;
	callbackList = 0;
	currentCallback = 0;
	nextCallback = 0;
}

// ---------------------------------------------------------------------------
//
VEventNode::VEventNode(const char *name)
	: eventType(name)
{
	next = 0;
	prev = 0;
	callbackList = 0;
	currentCallback = 0;
	nextCallback = 0;
}

// ---------------------------------------------------------------------------
//
VEventNode::VEventNode(unsigned long id)
	: eventType(id)
{
	next = 0;
	prev = 0;
	callbackList = 0;
	currentCallback = 0;
	nextCallback = 0;
}

// --------------------------------------------------------------------------
//
VEventNode::~VEventNode()
{
	for (Callback *c = callbackList ; c != 0 ; ) {
		Callback *doomed = c;
		c = c->next;
		delete doomed;
	}
}

// ---------------------------------------------------------------------------
// Add the given callback object/method and subscriber object to the callback
// list.  If the either given callback object or method are null, then return
// FALSE, otherwise return TRUE.  The given subscriber may be null.  If the
// given callback object/method and subscriber are already in the callback
// list, then do nothing and return TRUE.
//
boolean VEventNode::addCallback(VObject *o, method m, void *subscriber)
{
	if ((o == 0) || (m == 0)) {
		return FALSE;
	}
	for (Callback *c = callbackList ; c != 0 ; c = c->next) {
		if ((c->callbackObject == o) &&
			(method(c->callbackMethod) == m) &&
			(c->subscriberObject == subscriber)) {
			return TRUE;
		}
	}
	addCallback(new Callback(o, m, subscriber));
	return TRUE;
}

// ---------------------------------------------------------------------------
// Remove the callback specified by the given callback object/method from the
// callback list.  If the given subscriber is null, then *all* the callbacks
// matching the given callback object/method on the callback list will be
// removed.  Likewise, if the given callback object or method are null, then
// *all* the callbacks matching the given callback object and/or method
// and/or subscriber will be removed.  If any callbacks are removed from the
// callback list, then return TRUE, otherwise return FALSE.
//
boolean VEventNode::removeCallback(VObject *o, method m, void *subscriber)
{
	if ((o == 0) || (m == 0) || (subscriber == 0)) {
		boolean status = FALSE;
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if (((o == 0) || (c->callbackObject == o)) &&
				((m == 0) || (method(c->callbackMethod) == m)) &&
				((subscriber == 0) || (c->subscriberObject == subscriber))) {
				deleteCallback(c);
				status = TRUE;
			}
		}
		return status;
	}
	for (Callback *c = callbackList ; c != 0 ; c = c->next) {
		if ((c->callbackObject == o) && (method(c->callbackMethod) == m) &&
			(c->subscriberObject == subscriber)) {
			deleteCallback(c);
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Trigger the callback(s) for the given subscriber object passing the given
// event type, subscriber, and data as arguments.  If the given subscriber
// object is null, then *all* of the callbacks will be triggered.  If any
// of the callbacks return TRUE, then return TRUE, otherwise return FALSE.
//
boolean VEventNode::notifyCallbacks(void *subscriber, void *data)
{
	boolean result = FALSE;
	for (Callback *c = callbackList ; c != 0 ; c = c->next) {
		if (((c->subscriberObject == 0) ||
			 (c->subscriberObject == subscriber)) && !c->doingNotify) {
			c->doingNotify = 1;
			currentCallback = c;
			nextCallback = c->next;
			result = result || (c->callbackObject->*c->callbackMethod)
								(eventType, subscriber, data);
			if (currentCallback != 0) {
				c->doingNotify = 0;
			}
		}
	}
	return result;
}

// ---------------------------------------------------------------------------
//
void VEventNode::objectDeleted(void *object)
{
	if (object != 0) {
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if ((c->callbackObject == object) ||
				(c->subscriberObject == object)) {
				deleteCallback(c);
			}
		}
	}
}

// ---------------------------------------------------------------------------
//
boolean VEventNode::hasCallback(VObject *o, method m, void *s) const
{
	if (o != 0) {
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if (o == c->callbackObject) {
				if (((m == 0) ||
					 (m == method(c->callbackMethod))) &&
					(s == c->subscriberObject)) {
					return TRUE;
				}
			}
		}
	}
	else if (m != 0) {
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if ((m == method(c->callbackMethod)) &&
				(s == c->subscriberObject)) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VEventNode::hasCallback(VObject *o, method m) const
{
	if (o != 0) {
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if (o == c->callbackObject) {
				if ((m == 0) || (m == method(c->callbackMethod))) {
					return TRUE;
				}
			}
		}
	}
	else if (m != 0) {
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if (m == method(c->callbackMethod)) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VEventNode::hasSubscriber(void *s) const
{
	if (s != 0) {
		for (Callback *c = callbackList ; c != 0 ; c = c->next) {
			if (s == c->subscriberObject) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Add the given Callback node to the callback list.
//
void VEventNode::addCallback(Callback *callback)
{
	if (callbackList == 0) {
		callbackList = callback;
	}
	else {
		callbackList->prev->next = callback;
		callback->prev = callbackList->prev;
	}
	callbackList->prev = callback;
	if ((currentCallback != 0) && (nextCallback == 0)) {
		nextCallback = callback;
	}
}

// ---------------------------------------------------------------------------
// Remove the given Callback node from the callback list.
//
void VEventNode::deleteCallback(Callback *callback)
{
	if (callback == currentCallback) {
		currentCallback = 0;
	}
	else if (callback == nextCallback) {
		nextCallback = nextCallback->next;
	}
	if (callbackList == callback) {
		callbackList->prev = callback->next;
	}
	callback->prev->next = callback->next;
	delete callback;
}
