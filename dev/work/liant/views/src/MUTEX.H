// $Header:   Q:/views/common/vcs/mutex.h_v   1.5   May 23 1997 15:09:22   scottt  $

//	mutex.h
//
//	VMutex interface
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
//	03/13/97 tlf	Original.  This code was split out of cvthread.h.
//	04/15/97 tlf	Updated for DEC UNIX.
//	---------------------------------------------------------------------------

#ifndef CV_MUTEX_H
#define CV_MUTEX_H
#ifdef CV_WINDOWS
#include "defs.h"
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
// #include <iostream.h>
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


#if defined(CV_WINDOWS)     
#define VMUT_LOCK_TYPE          CRITICAL_SECTION
#define VMUT_LOCK(locki)        (EnterCriticalSection(&locki))
#define VMUT_UNLOCK(locki)      (LeaveCriticalSection(&locki))
#define VMUT_INIT_LOCK(locki)   (InitializeCriticalSection(&locki))
#define VMUT_DELETE_LOCK(locki) (DeleteCriticalSection(&locki))
#endif // CV_WINDOWS

//
// OS/2 lock and unlock macros
//
#if defined(PM)
#define VMUT_LOCK_TYPE                   HMTX
#define VMUT_LOCK(locki)                 (DosRequestMutexSem(locki, (ULONG) SEM_INDEFINITE_WAIT))
#define VMUT_UNLOCK(locki)               (DosReleaseMutexSem(locki))
#define VMUT_INIT_LOCK(locki)    (DosCreateMutexSem(NULL,&locki,0,FALSE))
#define VMUT_DELETE_LOCK(locki)
#endif //PM

//
// Motif  (PTHREAD) lock and unlock
//
#if defined(CV_MOTIF)
#define VMUT_LOCK_TYPE			pthread_mutex_t
#define VMUT_LOCK(locki)		(pthread_mutex_lock(&locki))
#define VMUT_UNLOCK(locki)		(pthread_mutex_unlock(&locki))

// AS: fix to allow compilation on Solaris
#if ( defined(CPPV_AIX6000) || defined(SOLARIS) )
#define VMUT_INIT_LOCK(locki)     (pthread_mutex_init(&locki,NULL))
#else
#define VMUT_INIT_LOCK(locki)     (pthread_mutex_init(&locki,pthread_mutexattr_default))
#endif

#define VMUT_DELETE_LOCK(locki)   (pthread_mutex_destroy(&locki))
#endif 


CLASS VMutex {
public:
	VMutex(void );
	~VMutex(void) ;
	void lock(void) ;
	void unlock(void) ;

private:
	VMUT_LOCK_TYPE currentLock;

};

#endif  // CV_MUTEX_H
