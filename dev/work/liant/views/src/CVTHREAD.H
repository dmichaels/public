// $Header:   Q:/views/win/vcs/cvthread.h_v   1.17   May 23 1997 15:09:26   scottt  $

//	cvthread.h
//
//	VThread interface
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
//	Revison Hisotry:
//	----------------
//	08/11/95 evc	original
//	09/28/95 evc	Small changes to fix complilation errors under VC++ 2.1
//	10/02/95 pw 	added two inline functions to get thread handles
//	10/03/95 pw 	added constructor that allows pre-existing threads to be
//					cvthread objects
//	10/03/95 pw 	bug fixes for suspendThread, resumeThread, and isSuspended
//	10/03/95 pw 	fixed typo
//	10/13/95 pw 	added cvthread::yield
//	11/14/95 evc	changed class names to conform to Views conventions
//	12/08/95 evc	Changed name of VEvent to VSynchEvent
//	03/12/97 tlf	Modified to conform to Foundation coding standards.
//					Preliminary attempt to port to Motif.
//	03/13/97 tlf	Modified to contain only VThread code.
//	04/15/97 tlf	Updated for DEC UNIX.
//	---------------------------------------------------------------------------

#ifndef CV_CVTHREAD_H
#define CV_CVTHREAD_H

#include "defs.h"
#ifdef CV_WINDOWS
#include <windows.h>
#endif

#ifdef PM
#define INCL_WIN
#define INCL_GPI
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSRESOURCES
#include <BSETIB.H>
#include <bsedos.h>

#ifndef INCLUDE_OS2H
#include <os2.h>
#define INCLUDE_OS2H
#endif

#endif // PM


#if defined(CV_MOTIF)
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/ipc.h>
#ifdef close
	#undef close
#endif
#ifdef connect
	#undef connect
#endif
#ifdef open
	#undef open
#endif
#ifdef read
	#undef read
#endif
#ifdef send
	#undef send
#endif
#ifdef wait
	#undef wait
#endif
#ifdef write
	#undef write
#endif
#endif // end of CV_MOTIF

#if defined (CV_WINDOWS)
typedef HANDLE THREADHANDLETYPE;
typedef DWORD  THREADIDTYPE;
#endif
#if defined (PM)
typedef   U32 THREADHANDLETYPE;             
typedef   U32 THREADIDTYPE;                     
#endif

#if defined(CV_MOTIF)
typedef  pthread_t  THREADIDTYPE;
typedef  pthread_t  THREADHANDLETYPE;
#endif

#if defined(CPPV_AIX6000) || defined(SOLARIS)
typedef  void * ( *CV_THREAD_FUNC_PTR ) (void *) ;
#define CV_THREAD_FUNC_DECLARE(name) \
	static void * name(void *)
#define CV_THREAD_FUNC_DEFINE(class, name, arg_name) \
	void * class::name ( void * arg_name)
#endif

#if defined(CPPV_HPUX) || (defined(__osf__) && defined(__DECCXX))

//  HPUX  and DEC define the thread subroutine pointer via a typedef,
//  that's why there is a unique entry for them.

typedef  pthread_startroutine_t CV_THREAD_FUNC_PTR ;

//  However, we need to know that the typdef is anyhow, in order
//  to define the following macros.

#define CV_THREAD_FUNC_DECLARE(name) \
	static void * name(void *)
#define CV_THREAD_FUNC_DEFINE(class, name, arg_name) \
	void * class::name ( void * arg_name)
#endif

#if defined (CV_WINDOWS)
typedef  unsigned long  ( *CV_THREAD_FUNC_PTR ) (void *) ;
#define CV_THREAD_FUNC_DECLARE(name) \
	static unsigned long name(void *)
#define CV_THREAD_FUNC_DEFINE(class, name, arg_name) \
	unsigned long class::name ( void * arg_name)
#endif

#if defined (PM)
typedef  void __Optlink ( *CV_THREAD_FUNC_PTR ) (void *) ;
#define CV_THREAD_FUNC_DECLARE(name) \
	static __Optlink name(void *)
#define CV_THREAD_FUNC_DEFINE(class, name, arg_name) \
	__Optlink class::name ( void * arg_name)
#endif


CLASS VThread {
// thread priority enumerations raise or lower a threads priority
// with respect to the base priority of the procrss that owns
// the thread.  This is a WIN32 specific model.
	public:
     
	enum RelativePriority{
		AboveNormal,
		BelowNormal,
		Highest,
		Lowest,
		Normal,
		TimeCritical,
		PriorityIdle
	} ;


	VThread();
	VThread( CV_THREAD_FUNC_PTR, void * ,  void * = 0, unsigned = 0 , 
		unsigned = 0);
	~VThread();

	boolean start(void);
	void build( CV_THREAD_FUNC_PTR, void *, void * = 0, unsigned = 0, unsigned = 0);
#if defined(CV_WINDOWS)
	void buildPreExisting(THREADHANDLETYPE, THREADIDTYPE, unsigned =0);
#endif
	void suspendThread();
	void resumeThread() ;
	boolean isSuspended();
	void setRelativePriority(RelativePriority );

	THREADHANDLETYPE getThreadHandle();
	THREADIDTYPE getThreadId();

	static THREADHANDLETYPE getRunningThreadHandle();
	static THREADIDTYPE getRunningThreadId();

	static void yield();

protected:
	THREADHANDLETYPE 	hThread;
	THREADIDTYPE		threadId;

private:
	boolean			isSuspendedFlag;
	boolean			isPreexisting;
	CV_THREAD_FUNC_PTR	threadProc;
	void			*argPtr;
	void			*security;
	unsigned		stackSize;
	unsigned		createFlag;
};

// inlines for VThread

inline boolean VThread::isSuspended() {
	return isSuspendedFlag;
}

inline THREADHANDLETYPE VThread::getThreadHandle() {
	return hThread;
}

inline THREADIDTYPE VThread::getThreadId() {
	return threadId;
}

inline THREADHANDLETYPE VThread::getRunningThreadHandle() {

#if defined (CV_WINDOWS)
	return GetCurrentThread();

#elif defined (PM)
	PTIB ptib;                      // Thread Information Structure
	PPIB ppib;                      // Process Information Structure
	
	DosGetInfoBlocks(&ptib, &ppib); // Get Process & Thread Info Blocks
	return ((DWORD)ptib->tib_ptib2->tib2_ultid);

#elif defined(CV_MOTIF)
	return (pthread_self());

#endif

}

inline THREADIDTYPE VThread::getRunningThreadId() {
#if defined (CV_WINDOWS)	
	return GetCurrentThreadId();
#elif defined(CV_MOTIF)
	return (pthread_self());
#endif

#if defined (PM)
		PTIB ptib;                      // Thread Information Structure
		PPIB ppib;                      // Process Information Structure
		DosGetInfoBlocks(&ptib, &ppib); // Get Process & Thread Info Blocks
		return (ptib->tib_ptib2->tib2_ultid); // return thread id
#endif
}

inline void VThread::yield() {
#if defined (CV_WINDOWS)	
	Sleep(0);
#elif defined(CPPV_AIX6000) || defined(CPPV_HPUX) 
	pthread_yield();
#elif defined (PM)
          DosSleep(0);
#endif

}

#endif // CV_CVTHREAD_H
