// $Header:   Q:/views/common/vcs/GLOBAL.CPV   1.23   Jun 16 1997 13:52:50   scottt  $ 

//	global.cpp
//
//	VGlobal class implementation [Common]
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
//	11/08/92 jmd	preened
//
//	03/05/93 pmcm	preened, commented
//	04/07/93 pmcm	changed id to VObject *
//	07/30/93 pkt	eliminated #ifdef macintosh.
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	01/13/94 dgm	merged and preened
//	01/27/94 dgm	changed name "max" to "maxgbls" -- conflict with "max"
//					template function in Borland C++ 4.0 header <stdlib.h>.
//					added #include of "wintypes.h" for HANDLE and <windows.h>
//					for various Windows API function declarations.
//	01/30/94 dgm	simplified ++ expression in VGlobal(unsigned) to
//					avoid apparent Borland C++ 4.0 compiler bug.
//	01/31/94 pmcm	fixed lookup() to use call argument in lookup.
//	02/17/94 pmcm	added type cast of cls in lookup() to quell warning
//	04/19/94 pmcm	CV_WIN32:  skipped client process id check in lookups
//	04/20/94 pmcm	CV_WIN32:  excluded delGlobs()
//	11/04/94 dss	STRICT: typed task handles to HTASK.
//	05/23/95 pkt	merged for common source.
//	05/29/95 atb	fixed missing brace typo
//	07/06/95 evc	folded in some intersolv changes (minor)
//  08/14/95 evc	added thread safety.  Made the OS/2 (ifdef PM) 
//					implementation of globals like WIN32. Got rid of initDLL
//					for all but CV_WIN16.
//	08/22/95 evc	Some tinkering for OS2 DLLs
//	08/31/95 evc	Some tinkering for thread safety
//	10/13/95 evc	Fixed LOCK/UNCLK definition
//	01/19/95 evc	Removed unused VGlobal constructor. Also 
//					changed allocation of globals table to use new and delete 
//					because new and delete make use of global sharable memory.
//					Also removed some unnecessary #if defined (...)
//	06/12/96 pkt	added support for port of library to ISAPI.
//	11/06/96 pkt	Updated spelling of VAppThreadSet::GetAppThreadId.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//	03/17/97 glm	updated VMutex, lock, unlock to conform with cvthread
//	03/24/97 glm	Added include of mutex.h.
//	05/02/97 pkt	added CV_NSAPI support.
// ---------------------------------------------------------------------------

#include "global.h"
#include "defs.h"
#if defined(CV_WINDOWS)
# include "memry.h"
#else
# include "set.h"
#endif
#if defined(PM)
#include "winclass.h"
#endif

#include <stdio.h>

#ifdef CV_WIN16
#include "wintypes.h"
#include <windows.h>
#endif

#if defined(DLL)
# if defined(CV_WINDOWS)
#  include <windows.h>
#  include "classtbl.h"
# endif
#endif

#include "cvassert.h"

#if defined(CV_THREADSAFE)
	#include "cvthread.h"
	#include "mutex.h"
	static VMutex * pCriticalSection=0;
	#define LOCK if (pCriticalSection == 0){\
			pCriticalSection = new VMutex();}\
			pCriticalSection->lock();

	#define UNLOCK if (pCriticalSection) {pCriticalSection->unlock();}

	#else
	#define LOCK
	#define UNLOCK
#endif // CV_THREADSAFE

#if defined(CV_ISAPI) || defined(CV_NSAPI)
#include "apthreds.h"
#endif

defineClass(VGlobal, VObject) 

#if defined(DLL) || defined(CV_THREADSAFE)
# define MAX_GBLS 100

// 'globals' is a list of the VGlobal objects for all 16 bit applications 
// that use this Allegris DLL.  Each entry in the list is a pointer
// to a VGlobal object.  The VGlobal object contains an application
// task handle that uniquely identifies the application to which it
// belongs and a class type that identifies the kind of global it is.
// For 32 bit DLLs the "globals" list  is mapped to each process
// by the OS.  But for thread safe DLL's we use the list to hold 
// thread-specific globals  and look them up mush like the 16 bit DLL
// look up the appropriate global for a process.

VGlobal **globals = 0;
HANDLE hGlobals = 0;

int top = 0;
int maxgbls = 0;
#if defined (CV_WIN16)
extern int dllInit;
#endif

#endif // DLL

// ---------------------------------------------------------------------------
// Constructor used by subclasses that are global objects in applications
// that do not use Allegris DLLs, but instead are built with the linkable 
// libraries.
//
VGlobal::VGlobal()
{
#if defined(DLL) || defined(CV_THREADSAFE)
	hMe = 0;
	wCls = 0;
#endif
}

// ---------------------------------------------------------------------------
// Destruct this global object and remove it from the global list
// (if using DLLs).
//
VGlobal::~VGlobal()
{
#if defined(DLL) || defined(CV_THREADSAFE)
LOCK
	// globals should not be 0 if calling a destructor
	if (!globals) { // Added this code - DM
		UNLOCK
		return;
	}

	// search the global list for this object's 
	// global class type and its task handle

	int i;
	for (i = 0; i < top; i++) {
  		if (globals[i] == this) {  // Changed logic - DM
			//      found
			break;
		}
	}

	// if found, slide later entries down to fill the gap

	if (i < top) {
		for (int j = i +1; j < top; j++) {
			globals[j - 1] = globals[j];
		}
		--top;
	}

	// if the globals list is empty, free it

	if (top == 0) {
		delete []  globals; 
	#if defined(CV_THREADSAFE)
		delete pCriticalSection;
		pCriticalSection=0;
	#endif
		globals = 0;   // EVC 6/14/95
	}
UNLOCK
#endif // DLL
}

// ===========================================================================
#if defined(DLL) || defined(CV_THREADSAFE)
// ---------------------------------------------------------------------------
// Construct a global object of type 'myCls' associated with the
// current task and add it to the global list (used only with DLLs).
//
VGlobal::VGlobal(unsigned myCls)
{
	VGlobal *g;
	int i;

LOCK

#if defined(CV_WIN16)
	isDll = dllInit;
#endif

#if (defined(CV_WIN32) || defined(PM) ) && !defined (CV_THREADSAFE)
	// Since Win32 maps the DLL into the process we do not a task handle.
	hMe = 0;
#elif defined(CV_ISAPI) || defined(CV_NSAPI)
	hMe = VAppThreadSet::getAppThreadSet()->getAppThreadId();
#elif defined(CV_THREADSAFE)
	hMe = (HANDLE) GetCurrentThreadId();
#else
	// We need a task handle to identify a particular application's globals.
	hMe = GetCurrentTask();
#endif

	wCls = myCls;


	if (!globals) {

		// Note that the new and delete operators
		// for 16 bit DLLs use global shareable
		// memory provided by View's overload of the global new and delete.

		// In 32 bit situations only one task is using the flobals table 
		// becasue the DLL 's address space is mapped into the 
		// address space of the client.

		globals = (VGlobal **) new VGlobal * [ (maxgbls = MAX_GBLS)];
		hGlobals = 0; // not used

	}


	// Search the global list and match both
	// the global class type and the task (or thread) handle.

	for (i = 0; i < top; i++) {
		g = globals[i];

#if (defined(CV_WIN32) || defined(PM) ) && !defined (CV_THREADSAFE)
		if (g->wCls == wCls) 
#else
		if (g->wCls == wCls && g->hMe == hMe) 
#endif
		{
			// found
			break;
		}
	}
	if (i >= top) {
		// not found, add it
		globals[top] = this;
		top++;
	}
	else {
		// found, replace
		globals[i] = this;
	}
	UNLOCK
}

// ---------------------------------------------------------------------------
// Return the global object of type 'cls' associated with the currently
// running task, or return a null pointer if not found (used only for DLLs).
//
VGlobal *VGlobal::lookup(unsigned cls)
{
LOCK

#if (defined(CV_WIN32) || defined(PM) ) && !defined (CV_THREADSAFE)
	// regular 32 bit thing - no threads  
	HANDLE h = 0;
#elif defined(CV_WIN16)
	HTASK h = GetCurrentTask();

#elif defined(CV_ISAPI) || defined(CV_NSAPI)
	DWORD h = VAppThreadSet::getAppThreadSet()->getAppThreadId();
#elif defined(CV_THREADSAFE)
   // 32 bits but we want the lookup to be by thread.
   // Only globals constructed with VGlobal(int) get 
   //in the table.

   HANDLE h = (HANDLE) GetCurrentThreadId();
#endif

	// Added this code - DM
	if (!globals) {
		UNLOCK
		return NULL;
	}

	VGlobal *g;
	int i;
	for (i = 0; i < top; i++) {
		g = globals[i];

#if !defined(CV_WIN16) && !defined(CV_THREADSAFE)
		if (g->wCls == (short) cls) 
#else
		if (g->wCls == (short) cls && g->hMe ==  h) 
#endif
		{
			break;
		}
	}
	UNLOCK
	return (i < top) ? g : 0;
}

// ---------------------------------------------------------------------------
// Return TRUE if 'obj' is a global object of the same type class type
// associated with the same application task handle (used only with DLLs).
//
boolean VGlobal::isEqual(VObject *obj) const
{
	VGlobal *g = (VGlobal *)obj;
	return (g->wCls == wCls && g->hMe == hMe) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Return a hash key computed from the current application task handle
// and the global class type (used only with DLLs).
//
HashKey VGlobal::hash() const
{
	return ((unsigned short)hMe + wCls) & HASH_KEY_MASK;
}

// ---------------------------------------------------------------------------
//
// if there are none in the table then return 0;
// otherwise return the first VGLobal pointer
// in the table that is the same kind as myCls

VGlobal*  VGlobal::returnFirstLikeThis(unsigned myCls){
	VGlobal *g; 
	int     i;
LOCK
	if (!globals) { // no table, return 0  
		UNLOCK
		return (VGlobal*) 0;
	} 
	for (i = 0; i < top; i++) {
		g = globals[i];
       	if ( (unsigned) g->wCls == myCls)  return g;
   	}  
	UNLOCK
   	return (VGlobal*) 0; 
}
#endif // DLL
