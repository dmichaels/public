// $Header:   Q:/views/win/vcs/timer.cpv   1.6   Mar 04 1997 17:55:56   gregm  $ 

//	timer.cpp
//
//	VTimer implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	04/07/93 pat	changed id to VObject *
//	08/09/93 jmd	removed free
//	03/25/94 pat	STRICT:	added TIMERPROC cast to SetTimer() call.
//	10/11/94 dss	Preened. Removed uponTick(). Replaced MSW timer functions
//					with notifier functions.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "timer.h"
#include "notifier.h"
#include "set.h"

defineClass(VTimer, VObject)

//---------------------------------------------------------------------------
//
//	Construct an initialized VTimer object with no client.
//
VTimer::VTimer()
{
	isPause = FALSE;
	isTiming = FALSE;
	isOneshot = FALSE;
	timeout = 0;
	client = 0;
	sel = 0;
}

//---------------------------------------------------------------------------
// 
// Construct a VTimer for the client 'clnt' with callback 'mthd'.
//
VTimer::VTimer(VObject *clnt, method mthd)
{
	isPause = FALSE;
	isTiming = FALSE;
	isOneshot = FALSE;
	timeout = 0;
	client = clnt;
	sel = mthd;
}

VTimer::~VTimer()
{
	stop();
}

//---------------------------------------------------------------------------
//
// Return TRUE if this VTimer is currently timing events, otherwise
// return FALSE.  
//
boolean VTimer::timing()
{
	return(isTiming && !isPause);
}

//---------------------------------------------------------------------------
//
// Start timing for a single period of time (in millisec). 
// Return FALSE if the timer can not be set, otherwise return TRUE. 
//
boolean VTimer::oneshot(unsigned timeout)
{
	isOneshot = TRUE;
	return(start(timeout));
}

// Temporarily pause or suspend notification of timer events.   
void VTimer::pause()
{
	isPause = TRUE;
}

//---------------------------------------------------------------------------
//
// Restart this VTimer at the previous time interval (in millisec). 
// Return FALSE if the timer can not be set, otherwise return TRUE.   
//
boolean VTimer::restart()
{
	if (timeout) {
		return(start(timeout));
	}
	else  {
		return(FALSE);
	}
}

//---------------------------------------------------------------------------
//
// Resume the timer event notifications for this VTimer that were
// previously suspended with the pause() message.  
//
boolean VTimer::resume()
{
	isPause = FALSE;
	return(TRUE);
}

//---------------------------------------------------------------------------
//
// Set the client object to 'clnt' and the callback method to 'mthd'.
//
void VTimer::uponTimeout(VObject *clnt, method mthd)
{
	client = clnt;
	sel = mthd;
}

//---------------------------------------------------------------------------
//
// Start this VTimer to callback every 'timeout' milliseconds. 
// Return FALSE if the timer can not be set, otherwise return TRUE.   
//
boolean VTimer::start(unsigned timeout)
{
	if (isTiming) {
		stop();
	}

	if (myIndex = notifier->startTimer(this, timeout)) {
		this->timeout = timeout;
		isTiming = TRUE;
		resume();
		return(TRUE);
	}

	return(FALSE);
}

//---------------------------------------------------------------------------
//
// Stop this VTimer.  The event timing can be restarted with the
// restart() method.  
//
void VTimer::stop()
{
	if (isTiming) {
		isTiming = FALSE;
		notifier->stopTimer(myIndex);
	}
}

//---------------------------------------------------------------------------
//
// VEvent notification when this VTimer\'s 'timeout' has expired.  If
// this VTimer was set up to time one time period, then further time events
// are cancelled.    
//
boolean VTimer::timerExpired()
{
	if (!isPause) {
		if (isOneshot) {
			stop();
		}
		if ((client != 0) && (sel != 0)) {
			pause();
			client->perform(sel, (long)this);
			resume();
		}
	}
	return(TRUE);
}

//---------------------------------------------------------------------------
//
//	Initialize the state of the VTimer object.
//
void VTimer::init()
{
	isPause = FALSE;
	isTiming = FALSE;
	isOneshot = FALSE;
}

