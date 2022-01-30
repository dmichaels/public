//	evnode.h
//
//	VEventNode interface [Common]
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

#ifndef VEVNODE_H
#define VEVNODE_H

// --------------------------------------------------------------------------
//
#include "evtype.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VEventManager;
CLASS	VEventType;
CLASS	VObject;

// --------------------------------------------------------------------------
// The VEventNode class is used to represent one VEventType and a list
// of callbacks (a VObject/method pair) to be invoked when the event is
// triggered.  An arbitrary object pointer, referred to as a "subscriber"
// object may be associated with each callback so that events can be
// triggered for specific subscribers.  If a subscriber is null then the
// callback will *always* be invoked.
//
// This class is used primarily internally be VEventManager which maintains
// a list of these.
//
CLASS VEventNode {
public:
						VEventNode(const VEventType&);
						VEventNode(const char *);
						VEventNode(unsigned long);
						VEventNode();
					   ~VEventNode();

	boolean				addCallback(VObject *, method, void * = 0);
	boolean				removeCallback(VObject *, method = 0, void * = 0);
	boolean				notifyCallbacks(void * = 0, void * = 0);

	const VEventType&	getType() const;
	boolean				hasCallbacks() const;
	boolean				hasCallback(VObject *, method = 0) const;
	boolean				hasCallback(VObject *, method, void *) const;
	boolean				hasSubscriber(void *) const;
	void				objectDeleted(void *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	CLASS Callback {
	public:
		VObject			   *getCallbackObject() const;
		method				getCallbackMethod() const;
		void				getCallback(VObject*&, method&) const;
		void			   *getSubscriberObject() const;
		Callback		   *getNext() const;
	protected:
							Callback();
							Callback(VObject *, method, void * = 0);
	private:
		Callback		   *next;
		Callback		   *prev;
		VObject			   *callbackObject;
		boolean	 (VObject::*callbackMethod)(VEventType&, void *, void *);
		void			   *subscriberObject;
		unsigned char		doingNotify : 1;
	private:
		friend				VEventNode;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	Callback		   *getCallbackList() const;
	VEventNode		   *getNext() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	void				addCallback(Callback *);
	void				deleteCallback(Callback *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	VEventNode		   *next;
	VEventNode		   *prev;
	VEventType			eventType;
	Callback		   *callbackList;
	Callback		   *currentCallback;
	Callback		   *nextCallback;

private:
	friend				VEventManager;
};

// --------------------------------------------------------------------------
//
inline const VEventType& VEventNode::getType() const
{
	return eventType;
}

inline VEventNode::Callback::Callback()
{
	next = 0;
	prev = 0;
	subscriberObject = 0;
	callbackObject = 0;
	callbackMethod = 0;
	doingNotify = 0;
}

inline VEventNode::Callback::Callback(VObject *o, method m, void *subscriber)
{
	next = 0;
	prev = 0;
	subscriberObject = 0;
	callbackObject = o;
	callbackMethod = (boolean (VObject::*)(VEventType&, void *, void *))m;
	doingNotify = 0;
}

inline boolean VEventNode::hasCallbacks() const
{
	return callbackList != 0;
}

inline VEventNode::Callback *VEventNode::getCallbackList() const
{
	return callbackList;
}

inline VEventNode *VEventNode::getNext() const
{
	return next;
}

inline VObject *VEventNode::Callback::getCallbackObject() const
{
	return callbackObject;
}

inline method VEventNode::Callback::getCallbackMethod() const
{
	return method(callbackMethod);
}

inline void VEventNode::Callback::getCallback(VObject *&o, method& m) const
{
	o = callbackObject;
	m = method(callbackMethod);
}

inline void *VEventNode::Callback::getSubscriberObject() const
{
	return subscriberObject;
}

inline VEventNode::Callback *VEventNode::Callback::getNext() const
{
	return next;
}

#endif // VEVNODE_H
