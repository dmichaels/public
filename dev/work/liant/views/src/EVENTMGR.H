/*
$Header:   Q:/views/common/vcs/EVENTMGR.H_V   1.3   Jun 16 1997 17:33:30   scottt  $ 
*/
//  eventmgr.h
//
//  VEventManager class interface
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  08/15/96 krb	First pass at generic callback class.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//	06/16/97 pkt	fixed so that declaration of VEventManager(boolean) is
//					present in case of CV_THREADSAFE.
// ---------------------------------------------------------------------------

//	Preferred usage:
//	-	Add a VEventManager member to the class needing event/callback support
//	-	Add a new method uponEvent(const VEventAtom& atom, VObject* clnt, method mthd);
//	-	Body of uponEvent should call VEventManager::subscribe()
//	-	When event needs to be thrown, call VEventManager::perform(atom, ...)
//
//	-   Atoms for events should be defined as
//			extern const VEventAtom eventName;
//		and declared as:
//			extern const VEventAtom eventName(-1);
//		TODO: may replace with macro?

#ifndef EVENTMGR_H
#define EVENTMGR_H

#include "global.h"
#include "str.h"

CLASS VDictionary;
CLASS VSet;
CLASS VOrdCollect;

// EVENT test
CLASS VEventAtom : public VObject
{
public:
	// ctor/dtor
	VEventAtom();
	~VEventAtom();

	VClass*	iam();

	// actual declaration ctor
	VEventAtom(const VString& name, boolean multipleCallback = TRUE);

	// canonical ctor/=
	VEventAtom(const VEventAtom& atom);
	VEventAtom& operator=(const VEventAtom& atom);

	// attributes
	const VString& 	getName() const { return m_name; }
	boolean 	canMultiCallback() const { return m_multiCallback; }

	// equivalence
	HashKey		hash() const;
	boolean		isEqual(VObject* obj) const;

private:
	VString		m_name;
	boolean		m_multiCallback;
};

#define GLOBALeventmanager 7

CLASS VEventManager : public VGlobal
{
public:
	VEventManager();
#if defined(DLL) || defined(CV_THREADSAFE)
	VEventManager(boolean dll);
#endif
	~VEventManager();

	// publish event for subscription
	VEventAtom* publishEvent(const VString& name, boolean multipleCallback = TRUE);
	VEventAtom* unpublishEvent(const VString& name);

	VEventAtom* publishEvent(VEventAtom* atom);
	VEventAtom* unpublishEvent(VEventAtom* atom);

	// subscribe for any publisher
	boolean subscribe(const VString& name, VObject* subscriber, method cllbck);
	boolean subscribe(const VEventAtom& atom, VObject* subscriber, method cllbck);
	// unsubscribe for any publisher
	boolean unsubscribe(const VString& name, VObject* subscriber, method cllbck = NIL_METHOD);
	boolean unsubscribe(const VEventAtom& atom, VObject* subscriber, method cllbck = NIL_METHOD);

	// subscribe for specific publisher
	boolean subscribe(const VString& name, VObject* publisher, VObject* subscriber, method cllbck);
	boolean subscribe(const VEventAtom& atom, VObject* publisher, VObject* subscriber, method cllbck);
	// unsubscribe for specific publisher
	boolean unsubscribe(const VString& name, VObject* publisher, VObject* subscriber, method cllbck = NIL_METHOD);
	boolean unsubscribe(const VEventAtom& atom, VObject* publisher, VObject* subscriber, method cllbck = NIL_METHOD);

	// remove as subscriber
	void removeAsSubscriber(VObject* subscriber);
	// remove as publisher
	void removeAsPublisher(VObject* publisher);

	// query subscribers for any publisher
	boolean hasSubscriber(const VString& name);
	boolean hasSubscriber(const VEventAtom& atom);
	// query subscribers for specific publisher
	boolean hasSubscriber(const VString& name, VObject* publisher);
	boolean hasSubscriber(const VEventAtom& atom, VObject* publisher);

	// trigger notification
	// preferred methods
	boolean notify(const VString& name, VObject* publisher, long);
	boolean notify(const VEventAtom& atom, VObject* publisher, long);

	// additional methods
	boolean notify(const VEventAtom& atom, VObject* publisher);
	boolean notify(const VEventAtom& atom, VObject* publisher, long, long);
	boolean notify(const VEventAtom& atom, VObject* publisher, long, long, long);

	VEventAtom* findEvent(const VString& name);

	// TODO: add additional styles to support only a single callback/event
	// or multiples, etc.

	// VGlobal methods
	VClass* iam();
	VEventManager* operator->();
	
private:
	void init();
	void getEventCollect(const VEventAtom& atom, VObject* publisher, VOrdCollect& resultCollect);

	VDictionary*	objectSubscriptions;
	VDictionary*	classSubscriptions;
	boolean			inited;

	VSet*			atomDictionary;
};

extern VEventManager CV_NEARDATA eventManager;

#endif // EVENTMGR_H
