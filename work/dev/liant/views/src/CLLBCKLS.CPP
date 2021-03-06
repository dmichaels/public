// $Header:   Q:/views/common/vcs/CLLBCKLS.CPV   1.16   Mar 24 1997 16:08:28   scottt  $ 

//	cllbckls.cpp
//
//	VCallbackList implementation [Common]
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
//	04/15/93 dgm	comments
//	08/25/93 pmcm	original.
//	09/02/93 pmcm	preened.
//	09/03/93 pmcm	fixed return of index in findCallback() for ghost
//					callbacks with actual methods
//	10/19/93 dgm	UNIX changes (from atb).
//	11/03/93 pkt	moved exportCallback from cllbckls.h.
//	11/09/93 pkt	added internal-linkage lstrcmp.
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened NULL.
//	01/23/94 pkt	fixed merge of lstrcmp.
//	01/30/94 dgm	simplified ++ expression in getList() to
//					avoid apparent Borland C++ 4.0 compiler bug.
//	02/25/94 pmcm	added CV_NEARDATA attribute to VInitDLL object
//	02/24/94 dgm	old C preprocessor changes
//	06/02/94 dgm	Simplified ?: expressions in function calls to
//					work-around SPARCworks C++ compiler (4.0) bug.
//	08/30/94 pkt	updated for VNOMALLOC version of VMemory.
//	09/07/94 dgm	Minor work-around for Lucid C++ compiler bug.
//  02/09/95 evc    changed VCallbackList::getList so table it expands
//					correctly. Fixes bug report 2661; (when more that 9
//					copies of demovrf executed simultatiously using DLLs,
//					the next one executed crashes)
//	05/19/95 pkt	merged for common source.
//  08/08/95 evc    Got rid of memhndl and initDLL for WIN32.
//					Preened some conditional comp. Added thread safety
//	08/15/95 evc    Changed #ifdef DLL to #if defined DLL
//  02/20/96 evc    Got rid of memhndl references
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//	03/17/97 glm	updated VMutex, lock, unlock to conform with cvthread
//	03/24/97 tlf	Added include of mutex.h.
// ---------------------------------------------------------------------------

#include "defs.h"

#include "cllbckls.h"	
#include "callback.h"	/* for VCallback class */
#include "classtbl.h"	/* for VInitDLL class */
#include "memry.h"		/* for local and global heap allocations */
#include "window.h"		/* for finding client for callbacks */
#include "notifier.h"

/* 
TABLESIZE is the number of slots in an array of pointers to callback
tables.  In the 16 bit DLL case the correct table is selected
according to task ID.  The value of TABLESIZE is the number
of tasks that can be accomodated by the Views DLL before the 
"table of tables" needs expansion.

Now 32 bit operating systems that have DLLs (OS/2 and WIN32)
map a unique instance of a DLL's data to each client. We never need more 
that one entry in the "table of tables" because the operating system 
guarentees that only one client is associated the array.
Threrfore we allocate only 1 entry. We leave intact the notion of 
searching a "table of tables" in these 32 bit cases to maintain 
common code. 
*/

#if defined (DLL) && defined (CV_WIN16)
#define TABLESIZE 10
extern VInitDLL CV_NEARDATA initDLL;
#else
#define TABLESIZE 1
#endif

#ifndef CV_WINDOWS
#	include <string.h>
#	define lstrcmp strcmp
#endif


#if defined (CV_THREADSAFE)
// Since the callback table is process wide global,
// access must be protected for threads.  We use
// critical sections to accomplish this.
// Note that this table is created during static initialization
// so we only protect function that can be called after that.
// Basicly add and remove.

#include "cvthread.h"
#include "mutex.h"
static VMutex * pCriticalSection=0;
#define LOCK if (pCriticalSection == 0){\
		pCriticalSection = new VMutex();\
		pCriticalSection->lock();}

#define UNLOCK  pCriticalSection->unlock();
#else
#define LOCK
#define UNLOCK
#endif







defineClass(VCallbackList, VGlobal)

// ---------------------------------------------------------------------------
//	pointer to the global VCallbackList object (see globals.cpp)

VCallbackList *callbackList = 0;

// ---------------------------------------------------------------------------
//
VCallbackList::VCallbackList() 
{
	top 	= 0;
	max 	= 0;
	table 	= 0;
	initTable();
}

// ---------------------------------------------------------------------------
//
VCallbackList::~VCallbackList()
{
	freeTable();
#if defined (CV_THREADSAFE)
	delete pCriticalSection;
#endif
}

// ---------------------------------------------------------------------------
//	Allocates a callback table with room for 10 entries.  
//
//	Note:	in a 16 bit DLL context, the memory block for this table comes
//			from the global heap and is both moveable and shareable.
//			in a non-DLL context, the local heap is used, via malloc.
//
void VCallbackList::initTable()
{
	if (table == 0) {
		hTable 	= 0; // not used
		table = (CallbackList *) new CallbackList [TABLESIZE];
		if (table != 0) {
			max = TABLESIZE;
			for(int i = 0; i < max; i++) {
				table[i].real 	= 0;
				table[i].ghost 	= 0;
				table[i].hTask 	= 0;
			}
		}
	}
}

// ---------------------------------------------------------------------------
//
boolean VCallbackList::expandTo(int n)
{
	if (n <= max) {
		return TRUE; 
	}

	if (table == 0) {
		initTable();
		if (table == 0) {
			return FALSE; 
		}
	}	

	CallbackList *newTable = 0;

	newTable = (CallbackList *) new CallbackList [n];
	// copy the old table into the new table
	for(int i = 0; i < max; i++) {
		newTable[i] = table[i];
	}
	delete [] table;


	if (newTable != 0) {
		//	zero the new list heads
		for(int i = max; i < n; i++) {
			table[i].real 	= 0;
			table[i].ghost 	= 0;
			table[i].hTask 	= 0;
		}
		//	update table size and table pointer
		max = n;
		table = newTable;
	}

	return newTable != 0;
}

// ---------------------------------------------------------------------------
//
void VCallbackList::freeTable()
{
	if (table != 0) {
		delete [] table;
		table = 0;
	}
}

// ---------------------------------------------------------------------------
//	Return a pointer to the callback table associated with this application.
//
//	Note:  VCallbackList is a VGlobal object and one may exist for each 
//	application.  Normally, VGlobal classes overload this operator to
//	lookup the VGlobal object associated with the current task.  However, 
//	in this implementation, there is only one callback table.  To the 
//	Views GUI DLL, this object is accessible through the global 
//	VCallbackList *callbackList.  To an application, is indirectly 
//	accessible through the static method of this classs.
//
VCallbackList *VCallbackList::operator -> ()
{
	if (callbackList == 0) {
		callbackList = new VCallbackList();
	}
	return callbackList;
}

// ---------------------------------------------------------------------------
//	Return a pointer to the callback list associated with the current task.
//	If the callback list does not exist, then create it.
//
VCallbackList::CallbackList *VCallbackList::getList()
{
#if defined(DLL) && defined(CV_WIN16)
	return getList(initDLL.get() == 0 ? GetCurrentTask() : 0);
#else
	return getList(0);
#endif
}

// ---------------------------------------------------------------------------
//	Return a pointer to the callback list associated with the given task.
//	If the callback list does not exist, then create it.
//
VCallbackList::CallbackList *VCallbackList::getList(HANDLE h)
{
	if (table == 0) {
	  	//	create the table of lists, if it does not exist.

LOCK
	 	initTable(); 
UNLOCK
		if (table == 0) {
			return 0;
		}
	}

	//	search the table for the task's callback list
	
	for (int i = 0; i < top; i++) {
		if (table[i].hTask == h) {
			return &table[i]; 
		}
	}

	//	a callback list for the given task is not in table.
	//	add a new one, expanding as necessary.
    // changed so expandTo happens when top == max evc 2/9/95
	if (top >= max && !expandTo(max + TABLESIZE)) {
		return 0;
	}
	table[top].hTask = h;
	VCallbackList::CallbackList *result = &table[top];
	top++;
	return result;
}

// ---------------------------------------------------------------------------
//	Add the given callback to the head of the list of ghost callbacks.
//	Return TRUE, if successful; FALSE, if not.
//
//	Note that this method is static.
//
boolean VCallbackList::addGhostCallback(VCallback *callback)
{
 	return callbackList->addCallback(callback, TRUE);
}

// ---------------------------------------------------------------------------
//	Remove the given callback from the list of ghost callbacks.
//	Return TRUE, if successful; FALSE, the callback was not found in the list.
//
//	Note that this method is static.
//
boolean VCallbackList::removeGhostCallback(VCallback *callback)
{
 	return callbackList->removeCallback(callback, TRUE);
}

// ---------------------------------------------------------------------------
//	Add the given callback to the real or ghost callback list, as indicated,
//	for the current application.
//
//	Return TRUE, if added; FALSE, otherwise.
//
//	Note that this method does not check for duplicates.
//
boolean VCallbackList::addCallback(VCallback *callback, boolean ghost)
{
	CallbackList *list;
	if (callback == 0 || (list = getList()) == 0) {
	 	return FALSE;
	}

LOCK
	
	if (ghost) {
		callback->setNext(list->ghost);
		if (list->ghost == 0) {
			callback->setIndex(0);
		}
		else {
			callback->setIndex(callback->getNext()->getIndex() + 1L);
		}
		list->ghost = callback;
	}
	else {
		callback->setNext(list->real);
		callback->setIndex(-1L);
		list->real = callback;
	}
UNLOCK
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Remove (unlink) the given callback from the real or ghost list, as 
//	indicated, of the current application.
//
//	Return TRUE, if the callback was found and removed; FALSE, if it was
//	not found.
//
//	Note that this method does not destroy the VCallback object.
//
boolean VCallbackList::removeCallback(VCallback *callback, boolean ghost)
{
	if (callback == 0) {
	 	return FALSE;
	}	

LOCK

	CallbackList *list = getList();
	if (list == 0 || (ghost && (list->ghost == 0)) || 
		(!ghost && (list->real == 0))) {
	
UNLOCK
		return FALSE;
	}
	
	VCallback *prior = 0;
	VCallback *curr = ghost ? list->ghost : list->real;
	while (curr != 0) {
		if (curr == callback) {
			break;
		}
		prior = curr;
		curr = curr->getNext();
	}

	if (curr != 0) {
		if (prior != 0) {
			prior->setNext(curr->getNext());
		}
		else {
			if (ghost) {
				list->ghost = curr->getNext();
			}
			else {
			 	list->real = curr->getNext();
			}
		}
		curr->setNext(0);
		curr->setIndex(-1L);
UNLOCK
		return TRUE;
	}

UNLOCK
 	return FALSE;
}

// ---------------------------------------------------------------------------
//	Return a pointer to the VCallback object at the head of the linked list
//	of real callbacks for the current application.
//
//	Note that method is static!
//
//	Note that under DLL initialization the head DLL real callback list is 
//	returned--since the application's real callback list does not exist.
//
const VCallback *VCallbackList::getCallbackList()
{
#if defined(DLL) && defined(CV_WIN16)
	return callbackList->getCallbackList(initDLL.get() == 0 ? 
												GetCurrentTask() : 0, FALSE);
#else
	return callbackList->getCallbackList(0, FALSE);
#endif
}

// ---------------------------------------------------------------------------
//	Return a pointer to the VCallback object at the head of the linked list
//	of ghost callbacks for the current application.
//
//	Note that method is static!
//
//	Note that under DLL initialization the head DLL real callback list is 
//	returned--since an application ghost list does not yet exist.
//
const VCallback *VCallbackList::getGhostCallbackList()
{
#if defined(DLL) && defined(CV_WIN16)
	return callbackList->getCallbackList(initDLL.get() == 0 ? GetCurrentTask() : 0, TRUE);
#else
	return callbackList->getCallbackList(0, TRUE);
#endif
}

// ---------------------------------------------------------------------------
// 	Search for the named callback in the current application's callback list.
//	If the application has a ghost callback list, then search only the ghost
//	list; otherwise, search the real callback list.
//
//	Return the actual member pointer for the callback, if found.  If the
//	caller has provided an return index argument, then set the index to that
//	of the found callback.  Return NIL_METHOD and set the index to -1L, if
//	the named callback is not in the callback list.
//	
//	Note that if the caller supplies a return index argument and the named
//	callback is not already in the ghost list, then this method will create
//	a VCallback object for the named callback and add it to ghost list.
//
//	Note that method is static!
//
method VCallbackList::findCallback(		const char *className, 
										const char *memberName, 
										integer	   *index)
{
	if (index != 0) {
		//	if return index argument is present, 
		//	then pre-set it to an invalid value

	 	*index = -1L; 
	}

	if (className == 0 || memberName == 0) {
		return NIL_METHOD;			//	nothing to search for
	}

LOCK
	//	if the ghost list is present, use it instead of the real list.

	boolean ghost = FALSE;
	const VCallback *p = callbackList->getGhostCallbackList();
	if (p != 0 && notifier->getPainterMode() == TRUE) {
		ghost = TRUE;
	}
	else if ((p = callbackList->getCallbackList()) == 0) {
		UNLOCK
		return 0;			//	no lists, no method
	}

	
	method m = 0;
	integer idx = -1L;
	for ( ; p != 0; p = p->getNext()) {
		if ((p->getClassName() == 0) || 
			(p->getMemberName() == 0)) {

			continue;		//	don't try to compare null strings
		}

		if ((lstrcmp((pointer) className, 
			(pointer) p->getClassName()) == 0) &&
		    (lstrcmp((pointer) memberName, 
			(pointer) p->getMemberName()) == 0))	{

			m = p->getMethod();
			idx = p->getIndex();
			break;
		}	
	}

	if (ghost && index != 0) {
	 	*index = idx;
	}

	if (ghost && m == 0) {
			//	maybe we found ghost callback 
			//	(at least there is no member-pointer)

		if (index != 0 && *index < 0L) {
			//	caller has provided a return store for the index, so...
			//	not found, so add it and return new index

			VCallback *call = new VCallback(className, memberName, 
											0, -1L, VCallback::Ghost);
			*index = call->getIndex();
		}
	}

UNLOCK
	return m;
}

// ---------------------------------------------------------------------------
// 	Return the class and member-function names for the given callback.
//	A ghost callback is given by the index argument.  A real callback is 
//	given by the method argument.
//	
//	If a ghost callback list is present for the current application, search 
//	only the ghost list; otherwise, search the real callback list.
//
//	Return TRUE, and pass the names back in the className and memberName
//	arguments, if found; FALSE, and 0 in the return name arguments, if not
//	found.
//
//	Note that method is static!
//
boolean VCallbackList::findCallback(	method	 		m,
			      						const char *& 	className, 
										const char *& 	memberName,
										integer 		index)
{
LOCK

	className = memberName = 0;

	//	if the ghost list is present, use it instead of the real list.

	boolean ghost = FALSE;
	const VCallback *p = callbackList->getGhostCallbackList();
	if (p != 0 && notifier->getPainterMode() == TRUE) {
		ghost = TRUE;
	}
	else if ((p = callbackList->getCallbackList()) == 0) {
UNLOCK
		return FALSE;	//	no lists
	}

	if ((ghost && index == -1L) || (!ghost && m == 0)) {
UNLOCK
		return FALSE;	//	ghost search needs index; real search, method
	}

	for ( ; p != 0; p = p->getNext()) {
		if ((ghost && index == p->getIndex()) || 
			(!ghost && m == p->getMethod())) {

			className = p->getClassName();
			memberName = p->getMemberName();
UNLOCK
			return TRUE;
		}
	}
UNLOCK
	return FALSE;
}

// ---------------------------------------------------------------------------
//	Search the real callback list of the current application for the named 
//	callback.  This method does not search the ghost callback list.
//
//	Return the actual member pointer for the named callback, if found;
//	NIL_METHOD, if not.
//
//	Note that this method is static.
//
method VCallbackList::findCallback(const char *className,
			  				   			const char *memberName) 
{
LOCK
	const VCallback *c = callbackList->findCallback(className, 
							memberName, callbackList->getList(), FALSE);
	if (c == 0) {
UNLOCK
		return NIL_METHOD;
	}

	method m = c->getMethod();

UNLOCK
	return m;
}

// ---------------------------------------------------------------------------
//	Search the given list for the named callback and return a pointer to the
//	callback, if found; 0, if not.
//
const VCallback *VCallbackList::findCallback(	const char *className,
												const char *memberName,
												CallbackList *list, 
												boolean ghost)
{
	if (className == 0 || memberName == 0 || list == 0) {
		return 0;
	}

LOCK
	VCallback *p = ghost ? list->ghost : list->real;

	for ( ; p != 0; p = p->getNext()) {
		if ((p->getClassName() == 0) || 
			(p->getMemberName() == 0)) {

			continue;		//	don't try to compare null strings
		}

		if ((lstrcmp((pointer) className, 
			(pointer) p->getClassName()) == 0) &&
		    (lstrcmp((pointer) memberName, 
			(pointer) p->getMemberName()) == 0))	{

UNLOCK
			return p;	
		}	
	}
UNLOCK
	return 0;
}

VObject* VCallbackList::findClientForCallback(method m, VWindow* win)
{
	if (win == 0 || m == 0) {
		return 0;
	}

LOCK
	VObject* client = 0;
	const char *method_name = 0;
	const char *class_name = 0;
	// use callback name to get class of callback
	VCallbackList::findCallback(m, class_name, method_name, -1);
	if (class_name) {
		VClass*  cls = VClass::of(class_name);
		if (cls) {
			// find parent window with same classtype as callback
		    for (; win && !win->isA(cls); win = win->getParent()) {
		        ;       // NO CODE HERE
		    }
			client = win;
		}
	}
UNLOCK
	return client;
}

// ---------------------------------------------------------------------------
//	Return the head of the list of callbacks for the specified task.
//
VCallback *VCallbackList::getCallbackList(HANDLE h, boolean ghost)
{
	if (table == 0) {
		return 0;
	}

#ifndef DLL
	h = 0; 	//	task handle is irrelevant in non-DLL context
#endif
	CallbackList *list = getList(h);
	return (list == 0) ? 0 : (ghost ? list->ghost : list->real);
}

// ---------------------------------------------------------------------------
//	This non-static method is for use by VCallback::VCallback() under the 
//	importCallback() macro.  This method is applicable only in USE_DLL
//	contexts.  It searches the Views DLL callback list for the named
//	callback and returns the actual member pointer for the callback, if found;
//	0, if not.
//
method VCallbackList::exportCallback(const char *className,
					   								const char *memberName) 
{
	const VCallback *c = findCallback(className, memberName, 
														getList(0), FALSE);

	method m;

	if (c == 0) {
		m = 0;
	}
	else {
		m = c->getMethod();
	}

	return m;
}
