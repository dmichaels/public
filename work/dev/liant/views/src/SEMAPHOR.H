// $Header:   Q:/views/common/vcs/semaphor.h_v   1.2   May 23 1997 15:09:24   scottt  $

//	semaphor.h
//
//	VSemaphore interface
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
//	03/13/97 tlf	Original.  Split out from original cvthread.h.
//	04/15/97 tlf	Updated for DEC UNIX.
//	---------------------------------------------------------------------------

#ifndef CV_SEMAPHOR_H
#define CV_SEMAPHOR_H
#ifdef CV_WINDOWS
#include "defs.h"
#include <windows.h>
#include "object.h"
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
#include "object.h"
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


CLASS VSemaphore : public VObject {
public:
	enum WaitReturn { 	WaitSignaled,
				WaitTimeout,
				WaitFailure
	};
	enum ResetReturn { 	ResetComplete,
				ResetAlreadyReset,
				ResetFailure
	};
	enum CheckReturn { 	CheckSignaled,
				CheckNotSignaled,
				CheckFailure
	};
				VSemaphore(void) ;
				VSemaphore(int available) ;
				~VSemaphore(void);
		boolean		start(void); 
		void		wait(void); 
		int		wait(long lWaitPeriod);
		void		post(void);
		int		resetSemaphore(void);
		int		checkSemaphore(void);

private:
		void		VSemaphore::init(int available);

private:
		int		startCount;
#if defined (PM)
		HEV semaphore;
		unsigned long ulPostCount;
#elif defined (CV_WINDOWS)
		HANDLE semaphore;
#endif		 

#if defined(CV_MOTIF)
		pthread_cond_t 	semid;
		pthread_mutex_t semidMutex;
		int 		postcount;
#endif

};

#endif // CV_SEMAPHOR_H
