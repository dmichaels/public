//	evmgr.h
//
//	VEventManager interface [Common]
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

#ifndef VEVMGR_H
#define VEVMGR_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include "evtype.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VEventNode;
CLASS	VEventType;
CLASS	VObject;

// --------------------------------------------------------------------------
//
CLASS VEventManager : public VObject {

public:
					VEventManager();
				   ~VEventManager();
	VClass		   *iam();

	// Define event types

	VEventNode	   *defineEvent(const VEventType&);
	VEventNode	   *defineEvent(const char *);
	VEventNode	   *defineEvent(unsigned long);

	// Undefine event types

	boolean			undefineEvent(const VEventType&);
	boolean			undefineEvent(const char *);
	boolean			undefineEvent(unsigned long);

	// Subscribe to events

	VEventNode	   *subscribeEvent(const VEventType&,
								   VObject *, method, void * = 0);
	VEventNode	   *subscribeEvent(const char *,
								   VObject *, method, void * = 0);
	VEventNode	   *subscribeEvent(unsigned long,
								   VObject *, method, void * = 0);
	VEventNode	   *subscribeEvent(VEventNode *,
								   VObject *, method, void * = 0);

	// Unsubscribe from events

	boolean			unsubscribeEvent(const VEventType&,
									 VObject *, method = 0, void * = 0);
	boolean			unsubscribeEvent(const char *,
									 VObject *, method = 0, void * = 0);
	boolean			unsubscribeEvent(unsigned long,
									 VObject *, method = 0, void * = 0);

	// Trigger events

	boolean			triggerEvent(const VEventType&, void * = 0, void * = 0);
	boolean			triggerEvent(const char *, void * = 0, void * = 0);
	boolean			triggerEvent(unsigned long, void * = 0, void * = 0);
	boolean			triggerEvent(VEventNode *, void * = 0, void * = 0);

	// Find events

	VEventNode	   *findEventNode(const VEventType&) const;
	VEventNode	   *findEventNode(const char *) const;
	VEventNode	   *findEventNode(unsigned long) const;

	// Miscellaneous

	virtual void	objectDeleted(void *);
	VEventNode	   *getEventNodeList() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	void			addEventNode(VEventNode *);
	void			deleteEventNode(VEventNode *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	VEventNode	   *eventList;
};

extern VClass *VEventManagerCls;

// --------------------------------------------------------------------------
//
inline VEventManager::VEventManager()
{
	eventList = 0;
}

inline VEventNode *VEventManager::getEventNodeList() const
{
	return eventList;
}

#endif // VEVMGR_H
