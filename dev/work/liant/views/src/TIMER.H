// $Header:   Q:/views/win/vcs/timer.h_v   1.5   Mar 04 1997 16:33:12   gregm  $ 

//	timer.h
//
//	VTimer interface
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
//	04/09/93 pmcm	changed id to VObject *
//	08/09/93 jmd	removed free
//	10/11/94 dss	Preened.
// ----------------------------------------------------------------------------

#ifndef timer_h
#define timer_h
#include "defs.h"
#include "object.h"

extern VClass *VTimerCls;

//	Instances of the VTimer class are used to provide timed events to
//	client objects where client objects can be instances of any type of class.  
//	Timing events can be set up to occur periodically (that is, every so many
//	milliseconds) or for one time interval (one-shots).  The client object must
//	provide its VObject *, as well as, a method selector of one of its methods that will
//	be invoked when the timer event occurs.  Timing events are terminated by
//	either freeing the VTimer object or by calling stopTimer().

CLASS VTimer : public VObject {

protected:
	int myIndex;			// CV_WINDOWS timer id
	VObject *client;		// Client to notify of timeout
	method sel;				// Method to call on client
	unsigned timeout;		// timeout value in milliseconds
	boolean  isOneshot;	// TRUE if oneshot
	boolean  isTiming;	// TRUE if timer is active
	boolean  isPause;		// TRUE if timer notification is paused
	void init();
	public:
	VClass *iam();

	VTimer(VObject *, method);
	VTimer();
	~VTimer();

	void uponTimeout(VObject *, method);

	boolean timing();
	boolean oneshot(unsigned);
	void pause();
	boolean restart();
	boolean resume();
	boolean start(unsigned);
	void stop();

	// Event Handling CallBack
	boolean timerExpired();
};

#endif /* timer_h */
