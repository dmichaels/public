// $Header:   Q:/views/common/vcs/semaphor.cpv   1.2   May 23 1997 15:09:24   scottt  $

//	semaphor.cpp
//
//	VSemaphore implementation
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
//	03/13/97 tlf	Split this class out of  the original cvthread.cpp.
//	04/15/97 tlf	Updated for DEC UNIX.
//	--------------------------------------------------------------------------

#include "defs.h"
#include "semaphor.h"

#if defined(CV_WINDOWS)
VSemaphore::VSemaphore(void) {
	startCount = 0;
}

VSemaphore::VSemaphore(int available) {
	startCount = available;
}

VSemaphore::~VSemaphore(void) {
	CloseHandle(semaphore);
}

boolean VSemaphore::start(void) {
	// fix to correctly set the semaphore handle
	semaphore = CreateSemaphore((LPSECURITY_ATTRIBUTES)0, startCount, 0x7ffffff,  (LPCTSTR)0);
	if (semaphore != NULL)
		return TRUE;
	else
		return FALSE;
}

int VSemaphore::wait(long lWaitPeriod){
    
	int rc;
    
	rc = WaitForSingleObject(semaphore, (unsigned long) lWaitPeriod);
	
	if (rc == WAIT_OBJECT_0)
		return(VSemaphore::WaitSignaled);    // Semaphore was posted
	else if (rc == WAIT_TIMEOUT)
		return(VSemaphore::WaitTimeout);    // Timed out
	else
		return(VSemaphore::WaitFailure);    // Failure of some type
}

void VSemaphore::wait(void) {
	WaitForSingleObject(semaphore, INFINITE);
}

void VSemaphore::post(void) {
	ReleaseSemaphore(semaphore, 1, (LPLONG)0);
}

int VSemaphore::resetSemaphore(void){

	int rc;
 
	rc = WaitForSingleObject(semaphore, 0);
	if (rc != WAIT_OBJECT_0)
		return (VSemaphore::ResetAlreadyReset);  //  Semaphore was already reset
	while (rc == WAIT_OBJECT_0)
		rc = WaitForSingleObject(semaphore, 0);
	return(VSemaphore::ResetComplete); 
}
 
int VSemaphore::checkSemaphore(void)
{
	int rc;
 
	rc = WaitForSingleObject(semaphore, 0);
	
	if (rc == WAIT_OBJECT_0)
		return(VSemaphore::CheckSignaled);  	// Signal is waiting
	else if (rc == WAIT_TIMEOUT)
		return(VSemaphore::CheckNotSignaled);  	// No signal waiting
	else 
		return(VSemaphore::CheckFailure); 	// Bad semaphore
}

#endif // CV_WINDOWS


#if defined(CV_MOTIF)

VSemaphore::VSemaphore(void) {
	init(0);
}

VSemaphore::VSemaphore(int available) {
	init(available);
}

void VSemaphore::init(int available) {
	startCount = available;
}

boolean VSemaphore::start(void) {

	postcount = startCount;  //  Should this be set to 'startCount'?
#if defined(CPPV_AIX6000) || defined(SOLARIS)
	if ((pthread_mutex_init(&semidMutex, NULL)) != 0)
		return FALSE;
	if ((pthread_cond_init(&semid,NULL)) != 0)
		return FALSE;
	return TRUE;
#endif
	
#if defined (CPPV_HPUX) || (defined(__osf__) && defined(__DECCXX))
	if ((pthread_mutex_init(&semidMutex, pthread_mutexattr_default)) != 0)
		return FALSE;
	if ((pthread_cond_init(&semid, pthread_condattr_default)) != 0 )
		return FALSE;
	return TRUE;
#endif  //end of CPPV_HPUX

}

VSemaphore::~VSemaphore(void) {
	pthread_mutex_destroy(&semidMutex);
	pthread_cond_destroy(&semid);
}

void VSemaphore::wait(void) {
	int rc;

	pthread_mutex_lock(&semidMutex);
	if (postcount > 0)
	{
		postcount--;
		pthread_mutex_unlock(&semidMutex);
		return;
	}
	rc = pthread_cond_wait(&semid, &semidMutex);
	postcount--;
	pthread_mutex_unlock(&semidMutex);
}

int VSemaphore::wait(long lWaitPeriod)
{
	int rc = 0;

	struct timespec deltatime, absolutetime;
	int prc;
	
	// lWaitPeriod is in milliseconds.   The time spec has 2 fields,
	// tv_sec and tv_nsec.  tv_sec is in seconds and tv_nsec is in 
	// nano seconds.
	if (lWaitPeriod < 1000) // we don't have seconds
	{
		deltatime.tv_sec = 0;
		deltatime.tv_nsec = lWaitPeriod * 1000000;
	}
	else
	{
		deltatime.tv_sec  = lWaitPeriod / 1000;
		deltatime.tv_nsec = (lWaitPeriod - (deltatime.tv_sec * 1000)) * 1000000;
	}
	   
#if defined(CPPV_AIX6000) || defined(CPPV_HPUX)
//  The following function is NOT POSIX compliant:
	pthread_get_expiration_np(&deltatime, &absolutetime);
#else
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	current_time.tv_usec *= 1000;	// Convert to nanoseconds
	if (current_time.tv_usec + deltatime.tv_nsec > 1000000000) {
		current_time.tv_usec = current_time.tv_usec + deltatime.tv_nsec 
			- 1000000000;
		current_time.tv_sec++;
	}
	absolutetime.tv_sec = current_time.tv_sec + deltatime.tv_sec;
	absolutetime.tv_nsec = current_time.tv_usec + deltatime.tv_nsec;
#endif

	pthread_mutex_lock(&semidMutex);
	if (postcount > 0)
	{
		postcount--;
		pthread_mutex_unlock(&semidMutex);
		return(VSemaphore::WaitSignaled);
	}

	prc = pthread_cond_timedwait(&semid, &semidMutex, &absolutetime);

	if (prc == 0) {
		postcount--;	
		rc = 0;
	}
	else 
		rc = 1;

	pthread_mutex_unlock(&semidMutex);
	
	if (rc == 0)
        	return(VSemaphore::WaitSignaled);  
	else if (prc == EINVAL)
        	return(VSemaphore::WaitFailure); 
	else
		return(VSemaphore::WaitTimeout);
}
 
void VSemaphore::post(void) {

	pthread_mutex_lock(&semidMutex);
	postcount++;                 // make sure they find out it was posted;
	pthread_cond_signal(&semid);
	pthread_mutex_unlock(&semidMutex);

}

int VSemaphore::resetSemaphore(void) {
	int rc = 0;
	if (postcount > 0) {
		postcount = 0;
		rc = VSemaphore::ResetComplete;        
	}
	else
		rc = VSemaphore::ResetAlreadyReset;
	return(rc);
}
 
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
int VSemaphore::checkSemaphore(void)
{
	int rc=0, iSemval=0;
       
	if (postcount > 0)
		return(VSemaphore::CheckSignaled);	// semaphore is posted
	else 
		return(VSemaphore::CheckNotSignaled); 	// semaphore not posted
       
}

#endif  // CV_MOTIF
