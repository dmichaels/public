/*
$Header:   Q:/views/common/vcs/EVENTMGR.CPV   1.2   Mar 24 1997 16:08:48   scottt  $
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
#include "eventmgr.h"
#include "dictinry.h"
#include "set.h"
#include "ordcllct.h"
#include "iterator.h"
#include "assoc.h"

// ---------------------------------------------------------------------------
//	VEventAssoc class (private)
//
CLASS VEventAssoc : public VObject
{
public:
	// Constructors
	VEventAssoc();
	VEventAssoc(VObject* publisher, VObject* subscriber, method cllbck);
	VEventAssoc(const VEventAssoc&);

	VClass*		iam();

	VEventAssoc& operator=(const VEventAssoc&);

	// Equivalence
	HashKey		hash() const;
	boolean		isEqual(VObject* obj) const;

	// Data
	VObject*	publisher;
	VObject*	subscriber;
	method	    callback;
};

// ---------------------------------------------------------------------------
//	VEventAtom class
//
defineClass(VEventAtom, VObject)

// ---------------------------------------------------------------------------
//
VEventAtom::VEventAtom()
	:	m_multiCallback(FALSE)
{
}

// ---------------------------------------------------------------------------
//
VEventAtom::VEventAtom(const VString& name, boolean mc)
	:	m_name(name)
	,	m_multiCallback(mc)
{
//	eventManager->publishEvent(this);
}

// ---------------------------------------------------------------------------
//
VEventAtom::~VEventAtom()
{
// 	eventManager->unpublishEvent(this);
}

// ---------------------------------------------------------------------------
//
VEventAtom::VEventAtom(const VEventAtom& atom)
	:	m_name(atom.m_name)
	,	m_multiCallback(atom.m_multiCallback)
{
}

// ---------------------------------------------------------------------------
//
VEventAtom& VEventAtom::operator=(const VEventAtom& atom)
{
	m_name = atom.m_name;
	m_multiCallback = atom.m_multiCallback;

	return *this;
}

// ---------------------------------------------------------------------------
//
HashKey VEventAtom::hash() const
{
	return m_name.hash();
}

// ---------------------------------------------------------------------------
//
boolean	VEventAtom::isEqual(VObject* obj) const
{
	if (!obj->isA(VEventAtomCls))
		return FALSE;

	return m_name.compareVString(((VEventAtom*)obj)->m_name) == 0 ? TRUE : FALSE;
}

defineClass(VEventManager, VGlobal)

// ---------------------------------------------------------------------------
//	VEventManager class
//
VEventManager::VEventManager()
{
	objectSubscriptions = 0;
	classSubscriptions = 0;
	inited = FALSE;
}

#ifdef DLL
// ---------------------------------------------------------------------------
//
VEventManager::VEventManager(boolean dll)
	:	VGlobal(GLOBALeventmanager)
{
	objectSubscriptions = 0;
	classSubscriptions = 0;
	inited = FALSE;
}
#endif

// ---------------------------------------------------------------------------
//
VEventManager::~VEventManager()
{
	if (objectSubscriptions) {
		DO (objectSubscriptions, VAssoc, assoc)
			if (assoc->key) {
				delete assoc->key;
			}
			if (assoc->value) {
				((VOrdCollect*) assoc->value)->freeContents();
			}
		END
	}
	if (classSubscriptions) {
		DO (classSubscriptions, VAssoc, assoc)
			if (assoc->key) {
				delete assoc->key;
			}
			if (assoc->value) {
				((VOrdCollect*)assoc->value)->freeContents();
			}
		END
	}
}

// ---------------------------------------------------------------------------
//
void VEventManager::init()
{
	if (objectSubscriptions == 0)
		objectSubscriptions = new VDictionary;
	if (classSubscriptions == 0)
		classSubscriptions = new VDictionary;
	if (atomDictionary == 0)
		atomDictionary = new VSet;
	inited = TRUE;
}

// ---------------------------------------------------------------------------
//
VEventManager* VEventManager::operator->()
{
#ifdef DLL
	VEventManager* n;
	if (!(n = (VEventManager*) lookup(GLOBALeventmanager)))  {
		if (!inited)
			init();
		return this;
	}
	if (!inited)
		init();
	return n;
#else
	if (!inited)
		init();
	return this;
#endif
}

// ---------------------------------------------------------------------------
//
VEventAtom* VEventManager::publishEvent(const VString& name, boolean multipleCallback)
{
	VEventAtom* atom = new VEventAtom(name, multipleCallback);

	if (atomDictionary->add(atom)) {
		return atom;
	}

	delete atom;
	return 0;
}

// ---------------------------------------------------------------------------
//
VEventAtom* VEventManager::publishEvent(VEventAtom* atom)
{
	if (atom) {
		if (atomDictionary->add(atom))
			return atom;
	}
	return 0;
}

// ---------------------------------------------------------------------------
//
VEventAtom* VEventManager::unpublishEvent(const VString& name)
{
	VEventAtom temp(name);
	VEventAtom* atom = (VEventAtom*) atomDictionary->remove(&temp);
	return atom;
}

// ---------------------------------------------------------------------------
//
VEventAtom* VEventManager::unpublishEvent(VEventAtom* atom)
{
	if (atom) {
		atom = (VEventAtom*) atomDictionary->remove(atom);
	}
	return atom;
}

// ---------------------------------------------------------------------------
//
VEventAtom* VEventManager::findEvent(const VString& name)
{
	VEventAtom temp(name);
	VEventAtom* foundAtom = (VEventAtom*) atomDictionary->find(&temp);
	return foundAtom;
}

// ---------------------------------------------------------------------------
// subscribe for any publisher
// storing into classSubscriptions
//
boolean VEventManager::subscribe(const VString& name,
	VObject* subscriber,
	method cllbck)
{
	// find event name, create one if it doesn't exist
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		atom = publishEvent(name);
	}

	return subscribe(*atom, subscriber, cllbck);
}

// ---------------------------------------------------------------------------
// subscribe for any publisher
// storing into classSubscriptions
//
boolean VEventManager::subscribe(const VEventAtom& atom,
	VObject* subscriber,
	method cllbck)
{
	VEventAssoc* newAssoc = 0;

	// get event collection
	VOrdCollect* eventCollect = (VOrdCollect*) classSubscriptions->getValueAtKey((VObject*) &atom);
	if (eventCollect == 0) {
		eventCollect = new VOrdCollect;
		classSubscriptions->atKeyPut((VObject*) new VEventAtom(atom), eventCollect);
	}

	// add this callback without checking
	if (atom.canMultiCallback()) {
		newAssoc = new VEventAssoc(0, subscriber, cllbck);
	}
	// check first, then add/update
	else {
		newAssoc = (VEventAssoc*) eventCollect->idAt(0);

		if (newAssoc == 0) {
			newAssoc = new VEventAssoc;
		}

		newAssoc->publisher = 0;
		newAssoc->subscriber = subscriber;
		newAssoc->callback = cllbck;
	}

	// add to event collection
	eventCollect->add(newAssoc);

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::unsubscribe(const VString& name, VObject* subscriber, method cllbck)
{
	// find event name, nothing to do if not found
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		return TRUE;
	}

	return unsubscribe(*atom, subscriber, cllbck);
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::unsubscribe(const VEventAtom& atom, VObject* subscriber, method cllbck)
{
	// get event collection
	VOrdCollect* eventCollect = (VOrdCollect*) classSubscriptions->getValueAtKey((VObject*) &atom);
	if (eventCollect == 0) {
		return TRUE;
	}

	// remove specified subscriber
	// TODO: find a faster method
	REVERSEDO (eventCollect, VEventAssoc, assoc)
		if (assoc->subscriber == subscriber &&
			(cllbck == 0 || assoc->callback == cllbck)) {
			eventCollect->removeSame(assoc);
			delete assoc;
		}
	END


	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::subscribe(const VString& name,
	VObject* publisher,
	VObject* subscriber, 
	method cllbck)
{
	// find event name, create one if it doesn't exist
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		atom = publishEvent(name);
	}

	return subscribe(*atom, publisher, subscriber, cllbck);
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::subscribe(const VEventAtom& atom, 
	VObject* publisher,
	VObject* subscriber, 
	method cllbck)
{
	VEventAssoc* newAssoc = 0;

	// get event collection
	VOrdCollect* eventCollect = (VOrdCollect*) objectSubscriptions->getValueAtKey((VObject*) &atom);
	if (eventCollect == 0) {
		eventCollect = new VOrdCollect;
		objectSubscriptions->atKeyPut((VObject*) new VEventAtom(atom), eventCollect);
	}

	// add this callback without checking
	if (atom.canMultiCallback()) {
		newAssoc = new VEventAssoc(publisher, subscriber, cllbck);
	}
	// check first, then add/update
	else {
		newAssoc = (VEventAssoc*) eventCollect->idAt(0);

		if (newAssoc == 0) {
			newAssoc = new VEventAssoc;
		}

		newAssoc->publisher = publisher;
		newAssoc->subscriber = subscriber;
		newAssoc->callback = cllbck;
	}

	// add to event collection
	eventCollect->add(newAssoc);

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::unsubscribe(const VString& name,
	VObject* publisher, 
	VObject* subscriber, 
	method cllbck)
{
	// find event name, nothing to do if not found
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		return TRUE;
	}

	return unsubscribe(*atom, publisher, subscriber, cllbck);
	// get event collection
	VOrdCollect* eventCollect = (VOrdCollect*) objectSubscriptions->getValueAtKey((VObject*) atom);
	if (eventCollect == 0) {
		return TRUE;
	}

	// remove specified subscriber
	// TODO: find a faster method
	REVERSEDO (eventCollect, VEventAssoc, assoc)
		if (assoc->publisher == publisher &&
			assoc->subscriber == subscriber &&
			(cllbck == 0 || assoc->callback == cllbck)) {
			eventCollect->removeSame(assoc);
			delete assoc;
		}
	END

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::unsubscribe(const VEventAtom& atom, 
	VObject* publisher, 
	VObject* subscriber, 
	method cllbck)
{
	// get event collection
	VOrdCollect* eventCollect = (VOrdCollect*) objectSubscriptions->getValueAtKey((VObject*) &atom);
	if (eventCollect == 0) {
		return TRUE;
	}

	// remove specified subscriber
	// TODO: find a faster method
	REVERSEDO (eventCollect, VEventAssoc, assoc)
		if (assoc->publisher == publisher &&
			assoc->subscriber == subscriber &&
			(cllbck == 0 || assoc->callback == cllbck)) {
			eventCollect->removeSame(assoc);
			delete assoc;
		}
	END

	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VEventManager::removeAsSubscriber(VObject* subscriber)
{
	VOrdCollect* eventCollect;

	// remove from object subscriptions
	DO (objectSubscriptions, VAssoc, assoc)
		eventCollect = (VOrdCollect*) assoc->value;
		REVERSEDO (eventCollect, VEventAssoc, assoc)
			if (assoc && assoc->subscriber == subscriber) {
				eventCollect->removeSame(assoc);
				delete assoc;
			}
		END
	END

	// remove from class subscriptions
	DO (classSubscriptions, VAssoc, assoc)
		eventCollect = (VOrdCollect*) assoc->value;
		REVERSEDO (eventCollect, VEventAssoc, assoc)
			if (assoc && assoc->subscriber == subscriber) {
				eventCollect->removeSame(assoc);
				delete assoc;
			}
		END
	END
}

// ---------------------------------------------------------------------------
//
void VEventManager::removeAsPublisher(VObject* publisher)
{
	VOrdCollect* eventCollect;

	// remove from object subscriptions
	DO (objectSubscriptions, VAssoc, assoc)
		eventCollect = (VOrdCollect*) assoc->value;
		REVERSEDO (eventCollect, VEventAssoc, assoc)
			if (assoc && assoc->publisher == publisher) {
				eventCollect->removeSame(assoc);
				delete assoc;
			}
		END
	END
}	

// ---------------------------------------------------------------------------
//
boolean VEventManager::hasSubscriber(const VString& name)
{
	// find event name, nothing to do if not found
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		return FALSE;
	}

	return hasSubscriber(*atom);
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::hasSubscriber(const VEventAtom& atom)
{
	VOrdCollect* objectCollect = (VOrdCollect*) objectSubscriptions->getValueAtKey((VObject*) &atom);
	VOrdCollect* classCollect = (VOrdCollect*) classSubscriptions->getValueAtKey((VObject*) &atom);

	return (objectCollect != 0 || classCollect != 0) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::hasSubscriber(const VString& name, VObject* publisher)
{
	// find event name, nothing to do if not found
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		return FALSE;
	}

	return hasSubscriber(*atom, publisher);
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::hasSubscriber(const VEventAtom& atom, VObject* publisher)
{
	VOrdCollect* objectCollect = (VOrdCollect*) objectSubscriptions->getValueAtKey((VObject*) &atom);
	VOrdCollect* classCollect = (VOrdCollect*) classSubscriptions->getValueAtKey((VObject*) &atom);

	// look for any publisher
	if (publisher == 0) {
		// found at least one object or class subscriber
		if (objectCollect != 0 || classCollect != 0) {
			return TRUE;
		}
	}

	// look for specific publisher
	else {
		// found at least one but must search for specific publisher
		if (objectCollect) {
			// TODO: find a faster method
			DO (objectCollect, VEventAssoc, assoc)
				if (assoc->publisher == publisher) {
					return TRUE;
				}
			END
		}
		// found at least one but must search for specific publisher
		if (classCollect) {
			// TODO: find a faster method
			DO (classCollect, VEventAssoc, assoc)
				if (assoc->publisher == publisher) {
					return TRUE;
				}
			END
		}
	}

	// none
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::notify(const VEventAtom& atom, VObject* publisher)
{
	VOrdCollect resultCollect;
	getEventCollect(atom, publisher, resultCollect);

	if (resultCollect.count() > 0) {
		DO (resultCollect, VEventAssoc, assoc)
			if (!assoc->subscriber->perform(assoc->callback, (long) publisher))
				return FALSE;
		END
		return TRUE;
	}
	else {
		return -1;
	}
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::notify(const VString& name, VObject* publisher, long p1)
{
	// find event name, nothing to do if not found
	VEventAtom* atom = findEvent(name);
	if (atom == 0) {
		return -1;
	}

	return notify(*atom, publisher, p1);
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::notify(const VEventAtom& atom, VObject* publisher, long p1)
{
	VOrdCollect resultCollect;
	getEventCollect(atom, publisher, resultCollect);

	if (resultCollect.count() > 0) {
		DO (resultCollect, VEventAssoc, assoc)
			if (!assoc->subscriber->perform(assoc->callback, (long) publisher, p1))
				return FALSE;
		END
		return TRUE;
	}
	else {
		return -1;
	}
}

// ---------------------------------------------------------------------------
//
boolean VEventManager::notify(const VEventAtom& atom, VObject* publisher, long p1, long p2)
{
	VOrdCollect resultCollect;
	getEventCollect(atom, publisher, resultCollect);

	if (resultCollect.count() > 0) {
		DO (resultCollect, VEventAssoc, assoc)
			if (!assoc->subscriber->perform(assoc->callback, (long) publisher, p1, p2))
				return FALSE;
		END
		return TRUE;
	}
	else {
		return -1;
	}
}

// ---------------------------------------------------------------------------
//
void VEventManager::getEventCollect(const VEventAtom& atom, 
	VObject* publisher, 
	VOrdCollect& resultCollect)
{
	// get events from object subscriptions
	VOrdCollect* eventCollect = (VOrdCollect*) objectSubscriptions->getValueAtKey((VObject*) &atom);
	if (eventCollect != 0) {
		DO (eventCollect, VEventAssoc, assoc)
			if (assoc->publisher == publisher) {
				resultCollect.add(assoc);
			}
		END
	}

	// get events from class subscriptions
	eventCollect = (VOrdCollect*) classSubscriptions->getValueAtKey((VObject*) &atom);
	if (eventCollect != 0) {
		resultCollect.addTo(eventCollect);
	}
}

// ---------------------------------------------------------------------------
//	VEventAssoc class (private)
//
defineClass(VEventAssoc, VObject)

// ---------------------------------------------------------------------------
// Contruct an VEventAssoc object key k and value v set to 0L.
//
VEventAssoc::VEventAssoc()
{
	publisher = 0;
	subscriber = 0;
	callback = NIL_METHOD;
}

// ---------------------------------------------------------------------------
// Contruct an VEventAssoc object with key k and value v.
//
VEventAssoc::VEventAssoc(VObject* p, VObject* s, method m)
{ 
	publisher = p;
	subscriber = s;
	callback = m;
}

// ---------------------------------------------------------------------------
//
VEventAssoc::VEventAssoc(const VEventAssoc &ca)
{	
	publisher = ca.publisher;
	subscriber = ca.subscriber;
	callback = ca.callback;
}

// ---------------------------------------------------------------------------
// This hash function returns the result of calling the hash function
// on the key integer.
//
HashKey VEventAssoc::hash() const
{
	return (HashKey)(publisher) & (HashKey)(subscriber) & HASH_KEY_MASK;
}

// ---------------------------------------------------------------------------
// Return TRUE if the key of this VEventAssoc is the same as the key of
// VEventAssoc object a, otherwise return FALSE.   Comparison is by integer
// value.
//
boolean VEventAssoc::isEqual(VObject *a) const
{
	if (a == 0) {
		return FALSE;
	}
	if (a->isA(VEventAssocCls)) {
		if (publisher == ((VEventAssoc*) a)->publisher &&
			subscriber == ((VEventAssoc*) a)->subscriber) {
			return TRUE;
		}
	}
	return FALSE;
}
