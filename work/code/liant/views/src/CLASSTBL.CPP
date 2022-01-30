// $Header:   Q:/views/common/vcs/CLASSTBL.CPV   1.22   Mar 24 1997 16:08:24   scottt  $

//	classtbl.cpp
//
//	VClassTable implementation [Common]
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
//	03/05/93 pmcm	preened, commented
//	05/17/93 pmcm	changed unsigned int to HANDLE in remove() and of()
//	08/25/93 pmcm	moved dllInit flag from dll.cpp to here (w/ VInitDLL)
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened.
//	01/31/94 pmcm	added ~VInitDLL() to delete memory object in DLL context.
//	04/19/94 pmcm	CV_WIN32:  skipped client id check in lookups
//	08/30/94 pkt	updated for VNOMALLOC version of VMemory.
//	04/11/95 pkt	made 'of' char* parameter const.
//	05/22/95 pkt	selected Windows as common source.
//	05/26/95 pkt	combined international changes into common version.
//  07/28/95 evc    Got rid of memhndl  and VInitDLL usage for WIN32
//					Put in fixes that should have been in 1.7 but got lost
//					This comment should be for 1.8
//  08/14/95 evc    Simplified logic so we have either a 16 bit DLL case
//					or a regular case. The "regular" case means 1 class
//					table per process where the mapping is done by the os.
//					Added thread safety.
//	08/29/95 evc    Fixed problem with expanding table with 16 bit DLLs.
//	08/31/95 evc    Fixed UNLOCK definition
//	09/06/95 pkt	updated for revised C++ standard for loops.
//  10/13/95 evc 	fixed LOCK/UNLOCK again
//  11/28/95 evc    Removed deletion of VMemory object from ~VInitDLL
//					(not needed because there is only 1 VMemory object
//					that will be cleaned up by Windows when the DLL exits.)
//					Also got rid of some special case code obsoleted
//					now that all memory is from 1 sharable pool.
//	06/12/96 pkt	added #include "defs.h".
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
//	03/17/97 glm	updated VMutex, lock, unlock to conform with cvthread
//	03/24/97 tlf	Added include of mutex.h.
// ---------------------------------------------------------------------------

#include "defs.h"

#ifdef CV_DLL_MSG
#include <windows.h>
#endif

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

#include "classtbl.h"

#if defined (DLL) && defined (CV_WIN16)
#include "memry.h"
#endif

extern "C" {
#	include <string.h>
#	include <stdio.h>
}

defineClass(VClassTable, VGlobal)

// ---------------------------------------------------------------------------
//	pointer to the global VClassTable object
//	the global VClassTable object is constructed upon first reference
//
VClassTable *classTable = 0;

#if defined (DLL) && defined (CV_WIN16)
//	Preset dll initialization flag.  When set, the DLL is 
//  either initializing or cleaning up.

int dllInit = 1;
#endif

// ---------------------------------------------------------------------------
//  Lookup the instance of this class table associated with	this
//  application.  If none exists, create it.
//
VClassTable *VClassTable::operator->()
{
	if (!classTable) {
		classTable = new VClassTable();
	}
	return(classTable);
}

// ---------------------------------------------------------------------------
//  Constructs a VClassTable object with room for MAX_CLASSES.
//  (MAX_CLASSES is defined in defs.h)
//  If more than MAX_CLASSES are used in an application, the class
//  table will expand accordingly.
//
VClassTable::VClassTable() 
{
	top = max = 0;
	clsTbl = 0;
	init();
}

// ---------------------------------------------------------------------------
//  Allocates a class table with room for MAX_CLASSES entries. 
//  (MAX_CLASSES is defined in defs.h)
//
//  Note:	in a DLL context, the memory block for this table comes
//          from the global heap and is both moveable and shareable.
//          in a non-DLL context, the local heap is used, via malloc.
//
void VClassTable::init()
{
	if (!clsTbl) {
		clsTbl = (ClsObjs *) new ClsObjs  [ (max = MAX_CLASSES)];
		hClsTbl = 0; // not used
	}
}

// ---------------------------------------------------------------------------
//  Free the class table memory block.
//
VClassTable::~VClassTable()
{
	if (clsTbl) {
		delete [] clsTbl;
		#if defined(CV_THREADSAFE)
			delete pCriticalSection;
			pCriticalSection=0;
		#endif
	}

	clsTbl = 0;
}

VClass *nilCls = 0;

// ---------------------------------------------------------------------------
//  Return a pointer to the Meta-VClass (that is, the VClass object)
//  associated with the C++ class named 'name'.  For example, the
//  call classTable->of(VTextEditor) would return the value of
//  VTextEditorCls.
//
VClass *VClassTable::of(const char *name)
{
	int i;
	ClsObjs *cp;

	if (!clsTbl) {
		// no class table, no meta-class entry
		return(nilCls);
	}

#if defined(DLL) && defined(CV_WIN16)
	//	in a 16 bit DLL context, match the handle of the current
	//	task. Under Win32 the DLL is mapped into the client process
	//	address space, so we do not need a process id to distinguish
	//	one DLL client from another.

	HANDLE h = GetCurrentTask();

#else
	//	in a non-DLL context, we do not need a task handle
	//  This goes for 32 bit DLLs a la os/2 and WIN32 

	HANDLE h = 0;
#endif

	//	search the table 

LOCK
	for (i = 0; i < top; i++) {
		cp = &clsTbl[i];
		if (!strcmp(name, cp->cls->nameOf()) 
		&& (!cp->hMe || (cp->hMe == h))) {
		// The class table is process global and
		// for 32 bit DLLs as well as 16/32 bit static links 
		// the HANDLE h is 0 so it will match 
		// the first time the name matches.

UNLOCK
			//	found
			return(cp->cls);
		}
	}

	//	not found
UNLOCK
	return(nilCls);
}

// ---------------------------------------------------------------------------
//  Add the Meta-VClass instance 'cls' to this class table.
//
void VClassTable::add(VClass *cls)
{
	if (!clsTbl) {
		//	create the missing table
		init();
	}
LOCK

	if (top >= max) {
			//	expand the full table to accomodate the entry
		int oldmax = max;

		// we will never be expanding this table of tables
		ClsObjs * tempClassTbl = (ClsObjs *) new ClsObjs  [max += INCR_CLASSES];

		for (int i=0; i<oldmax; i++){
			tempClassTbl[i] = clsTbl[i] ;
		}
		delete [] clsTbl;
		clsTbl = tempClassTbl;
	}

	clsTbl[top].cls = cls;

#if defined(DLL) && defined(CV_WIN16)
	//	in a 16 bit DLL context, enter the application's task handle
	// Enter 0 if the metaclass was staticly initialized in the DLL
	// because it belongs to all clients.
	clsTbl[top].hMe = dllInit ? 0: GetCurrentTask();
#else
	//	Under 32 bit  the DLL is mapped into the process
	//	address space, so we do not need a process id to distinguish
	//	one DLL client from another.  
	clsTbl[top].hMe = 0;
#endif
	top++;
UNLOCK
}

// ---------------------------------------------------------------------------
//  Remove the Meta-Class instance 'cls' from this classtable.
//
void VClassTable::remove(VClass *cls)
{
LOCK

#if defined(DLL) && defined(CV_WIN16)
	//	in a 16 DLL context, we need to match both the meta-class and
	//	the handle of the current task. Under 32 bit DLLs the DLL is mapped 
	//	into the client process address space, so we do not need a process 
	//	id to distinguish one DLL client from another.  In non-16 bit 
	//  situations we do lookups with the HANDLE h set to 0.  
	//	We set it to 0 so it always matches the first entry
	//  that matches the task we are looking for.

	HANDLE h = GetCurrentTask();
#else
	//	in 32 bit DLLs or non-DLL context, we want a 0 task handle
	// 	so the first table will be selected
	HANDLE h = 0;
#endif

	//	search the class table

	int i;
	for (i = 0; i < top; i++) {

		if ((cls == clsTbl[i].cls) 
		&& (clsTbl[i].hMe == 0 || clsTbl[i].hMe == h)) {
			//	found it
			break;
		}
	}

	//	if the meta-class is found, then slide entries to fill the gap

	if (i < top) {
		for (int j = i + 1; j < top; j++) {
			clsTbl[j -1] = clsTbl[j];
		}
		-- top;
	}

	//	if the class table is empty, free it

	if (top <= 0) {
#if defined(DLL) && defined(CV_WIN16)
		//OutputDebugString("The Class table hClsTbl is KAPUT\n");
		GlobalUnlock(hClsTbl);
		GlobalFree(hClsTbl);
		hClsTbl = 0;
#else
		delete [] clsTbl;
#endif
		clsTbl = 0;
	}
UNLOCK
}

// ==========================================================================

#if defined (DLL) && defined (CV_WIN16)

// ---------------------------------------------------------------------------
//	A non-zero (TRUE) value indicates that the DLL is initializing or
//	cleaning up; a zero (FALSE), otherwise.
//
VInitDLL::VInitDLL(short isInitializing)
{
	dllInit = isInitializing;

#ifdef CV_DLL_MSG
		char buf[101];
		char msg[101];
	
		sprintf(msg, "Constructing VInitDLL object for %s", 
				dllInit ? "DLL" : "Application");

		GetModuleFileName(0, buf, 100);
		MessageBox(GetFocus(), buf, msg, MB_OK | MB_SYSTEMMODAL);
#endif
}

// ---------------------------------------------------------------------------
//
void VInitDLL::clear()
{
	dllInit = 0;
		
#ifdef CV_DLL_MSG
		char buf[101];

		GetModuleFileName(0, buf, 100);
		MessageBox(GetFocus(), buf, "VInitDLL object flag cleared", 
					MB_OK | MB_SYSTEMMODAL);
#endif
}

// ---------------------------------------------------------------------------
//
void VInitDLL::set()
{
	dllInit = 1;
	
#ifdef CV_DLL_MSG
		char buf[101];

		GetModuleFileName(0, buf, 100);
		MessageBox(GetFocus(), buf, "VInitDLL object flag set", 
					MB_OK | MB_SYSTEMMODAL);
#endif
}

// ---------------------------------------------------------------------------
//
short VInitDLL::get()
{
	return dllInit;
}

// --------------------------------------------------------------------------
//
VInitDLL::~VInitDLL()
{
;
}

// --------------------------------------------------------------------------

#endif /* defined DLL and WIN16 */
