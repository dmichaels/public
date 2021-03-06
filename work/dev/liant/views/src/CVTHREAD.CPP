// $Header:   Q:/views/win/vcs/cvthread.cpv   1.17   May 23 1997 15:09:26   scottt  $

//	cvthread.cpp
//
//	VThread implementation
//
//	Allegris Foundation 1.1.00 [Common]
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
//	08/11/95 evc	original
//	09/28/95 evc	Small changes to fix complilation errors under VC++ 2.1
//	10/03/95 pw 	added constructor that allows pre-existing threads to
//					be cvthread objects
//	10/03/95 pw 	bug fixes for suspendThread, resumeThread, and isSuspended
//	10/06/95 pw 	fixed bug where thread pseudohandles were used instead
//					of real thread handles
//	10/08/95 pw 	fixed bugs in suspendThread(), resumeThread()
//	10/13/95 pw 	added locks around suspendThread, resumeThread, fixed up
//					resumeThread so it cleans up all suspends
//	10/23/95 evc	Small change because of differing return codes for
//					ResumeThread under 95 and NT.
//	11/14/95 evc	changed class names to conform to conventions -> VThread
//	03/12/97 tlf	Modified to conform to Foundation coding standards.
//					Preliminary attempt to port to Motif.
//	03/13/97 tlf	Modified to contain only VThread code.
//	03/18/97 tlf	Added member function start.
//	04/15/97 tlf	Updated for DEC UNIX.
//	--------------------------------------------------------------------------

#include "cvthread.h"
#include "mutex.h"

// following could result in race condition, if first
// resumeThread & suspendThread happen near-simultaneously

static VMutex * pCriticalSection=0;
#define LOCK if (pCriticalSection == 0){\
		pCriticalSection = new VMutex();}\
		pCriticalSection->lock();

#define UNLOCK if (pCriticalSection) {pCriticalSection->unlock();}

VThread::VThread() {
	isPreexisting = FALSE;
	isSuspendedFlag = FALSE;
#if !defined(CPPV_HPUX) && !(defined(__osf__) && defined(__DECCXX))
	hThread = 0;
	threadId = 0;
#endif
}

VThread::VThread( CV_THREAD_FUNC_PTR thread_proc,
		void * arg_ptr,  	// access this inside the threadProc
		void * arg_security,  	// no security
		unsigned stack_size, 	// will default
		unsigned create_flag	// create suspended if you use CREATE_SUSPENDED FLAG
 )
{
	isPreexisting = FALSE;
	isSuspendedFlag = FALSE;
#if !defined(CPPV_HPUX) && !(defined(__osf__) && defined(__DECCXX))
	hThread = 0;
	threadId = 0;
#endif

	build( thread_proc, arg_ptr, arg_security, stack_size, create_flag);

}

VThread::~VThread() {
	if (isPreexisting) {
#if defined (CV_WINDOWS)
		CloseHandle(hThread);
#elif defined(CV_MOTIF)
		pthread_cancel(hThread);
#endif
	}
}

#if defined(CV_WINDOWS)
void VThread::buildPreExisting( THREADHANDLETYPE existingThreadHandle, THREADIDTYPE existingThreadId, unsigned createFlag ) {
	if (createFlag == CREATE_SUSPENDED) {
		isSuspendedFlag = TRUE;
	} else {
		isSuspendedFlag = FALSE;
	}

	isPreexisting = TRUE;

	DuplicateHandle(GetCurrentProcess(),existingThreadHandle,GetCurrentProcess(),&hThread,0,TRUE,DUPLICATE_SAME_ACCESS);
	threadId = existingThreadId;
}
#endif

void VThread::build(CV_THREAD_FUNC_PTR thread_proc, void * arg_ptr,
	void * arg_security, unsigned stack_size, unsigned create_flag ){
	this->threadProc = thread_proc;
	this->argPtr = arg_ptr;
	this->security = arg_security;
	this->stackSize = stack_size;
	this->createFlag = create_flag;
}



boolean VThread::start(void){

#if defined (CV_WINDOWS)
	hThread = CreateThread(	(LPSECURITY_ATTRIBUTES) security, 
			stackSize,	/*     stack size        */
			(LPTHREAD_START_ROUTINE) threadProc, 
			argPtr, 
			(DWORD) createFlag,
			(LPDWORD)&threadId);

	if (hThread == NULL)
		return FALSE;
	if (createFlag ==  CREATE_SUSPENDED)
		isSuspendedFlag = TRUE;
	return TRUE;
#elif defined(PM)
	hThread = _beginthread(threadProc, NULL, stackSize, arg_ptr);
	if (hThread == -1) {
    		hThread = NULL;
		return FALSE;
	}
	else
		return TRUE;
#elif defined(CV_MOTIF)
	int rc = pthread_create (&hThread,
#if defined(CPPV_HPUX) || (defined(__osf__) && defined(__DECCXX))
		pthread_attr_default,
#else
		NULL,
#endif
		threadProc, argPtr);
	if (rc)  {
#if !defined(CPPV_HPUX) && !(defined(__osf__) && defined(__DECCXX))
	  	hThread = NULL;
#endif
		return FALSE;
	}
	return TRUE;
	
#endif

}

void VThread::suspendThread(){
// We insure that this is called only 1 time so the
// suspension count is 1.  That way resumeThread will always work.
// The NT API simply increments a count and keeps the thread suspended
// if the count is > 0;

#if defined(CV_WINDOWS) || defined(CPPV_AIX6000) || defined(SOLARIS)
	if (getRunningThreadId() == threadId) {
#elif defined(CPPV_HPUX) || (defined(__osf__) && defined(__DECCXX))
	THREADIDTYPE tempid = getRunningThreadId();
	if pthread_equal(tempid , threadId) {
#endif
		LOCK
		if (!isSuspendedFlag){
			isSuspendedFlag = TRUE;
			UNLOCK
#if defined (CV_WINDOWS)
			SuspendThread(hThread);
// #elif defined(CPPV_AIX6000) || defined(CPPV_HPUX) || defined(SOLARIS)
#elif defined(CV_MOTIF)
			//  Nothing!!  No suspend functionality is available.
#endif
		} else {
			UNLOCK
		}
	} else {
		LOCK
		isSuspendedFlag = TRUE;
#if defined (CV_WINDOWS)
		SuspendThread(hThread);
// #elif defined(CPPV_AIX6000) || defined(CPPV_HPUX) || defined(SOLARIS)
#elif defined(CV_MOTIF)
		//  Nothing!!  No suspend functionality is available.
#endif
		UNLOCK
	}
}

void VThread::resumeThread(){
#if defined(CV_WINDOWS)
	DWORD numberSuspensions;

	LOCK
	do {
		numberSuspensions=ResumeThread(hThread);
	} while (numberSuspensions > 1 && numberSuspensions != 0xFFFFFFFF);
	isSuspendedFlag = FALSE;
	UNLOCK
#elif defined(CV_MOTIF)
	//  Nothing!!  No suspend/resume functionality is available.
#endif
}

void VThread::setRelativePriority (RelativePriority p){
#if defined(CV_WINDOWS)
	if (!hThread) return;
	int setting;
	switch (p){

		case AboveNormal: {
			setting = THREAD_PRIORITY_ABOVE_NORMAL;
			}
			break;
		case BelowNormal: {
			setting = THREAD_PRIORITY_BELOW_NORMAL;
			}
			break;
		case Highest:	{
			setting = THREAD_PRIORITY_HIGHEST;
			}
			break;
		case Lowest:{
			setting = THREAD_PRIORITY_LOWEST;
			}
			break;
		case Normal:{
			setting = THREAD_PRIORITY_NORMAL;
			}
			break;
		case TimeCritical:{
			setting = THREAD_PRIORITY_IDLE;
			}
			break;
		case PriorityIdle:{
				setting = THREAD_PRIORITY_IDLE;
			  }
			break;
		default:
			break;
	}
	SetThreadPriority (hThread, setting);
#endif
}
