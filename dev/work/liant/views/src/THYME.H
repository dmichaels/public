// $Header:   Q:/views/common/vcs/THYME.H_V   1.10   Mar 24 1997 16:13:44   scottt  $

//	thyme.h -- Goofy name so as not to conflict with ANSI-C <time.h>
//
//	VTime interface [Common]
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
//	05/31/95 dgm	Original.
//	08/30/95 dgm	Unsigned to signed changes.
//	10/24/95 dgm	Fixed postfix ++ and --.
//	01/31/96 dgm	Fixed typo in declaration on getSecondString().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
// ---------------------------------------------------------------------------

#ifndef VTIME_H
#define VTIME_H

// --------------------------------------------------------------------------
//
#include "defs.h"

// --------------------------------------------------------------------------
//
CLASS	VObject;
CLASS	VString;
struct	tm;

// --------------------------------------------------------------------------
//
#if defined(max)
#undef max
#endif
#if defined(min)
#undef min
#endif

// --------------------------------------------------------------------------
//
CLASS VTime {

public:
						VTime();
						VTime(boolean set_current);
						VTime(int hour,
							  int minute,
							  int second = 0);
						VTime(const struct tm *);
						VTime(const VTime&);

	void				set();
	void				set(int hour,
							int minute = 0,
							int second = 0);
	void				set(const struct tm *);
	void				set(const VTime&);

	VTime&				operator=(const VTime&);

	void				setHour(int);
	void				setMinute(int);
	void				setSecond(int);
	void				setSeconds(long);
	void				setNull();

	boolean				isValid() const;
	boolean				isNull() const;

	int					getHour() const;
	int					getMinute() const;
	int					getSecond() const;
	long				getSeconds() const;
	void				getTm(struct tm *) const;

	void				getHourString(VString&, boolean = FALSE);
	void				getMinuteString(VString&, boolean = FALSE);
	void				getSecondString(VString&, boolean = FALSE);
	void				getString(VString&, const char *format = 0);

	void				addHours(int);
	void				addMinutes(int);
	void				addSeconds(int);

	VTime				plusHours(int) const;
	VTime				plusMinutes(int) const;
	VTime				plusSeconds(int) const;

	VTime				operator+(int) const;
	VTime&				operator++();
	VTime				operator++(int);
	VTime&				operator+=(int);
	VTime				operator-(int) const;
	VTime&				operator--();
	VTime				operator--(int);
	VTime&				operator-=(int);
	long				operator-(const VTime&) const;

	boolean				operator==(const VTime&) const;
	boolean				operator!=(const VTime&) const;
	boolean				operator<(const VTime&) const;
	boolean				operator>(const VTime&) const;
	boolean				operator<=(const VTime&) const;
	boolean				operator>=(const VTime&) const;
	boolean				between(const VTime&, const VTime&) const;
	VTime&				max(const VTime&) const;
	VTime&				min(const VTime&) const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

public:
	enum				Constants
	{
						SecondsPerMinute	= 60,
						MinutesPerHour		= 60,
						SecondsPerHour		= 3600,
						HoursPerDay			= 24,
						MinutesPerDay		= 1440
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	void				initialize();

private:
	long				theTime;	// seconds from beginning of day

};

// --------------------------------------------------------------------------
// Default constructor.
// Construct this VTime object to represent the current time of day.
//
inline VTime::VTime()
{
	set();
}

// --------------------------------------------------------------------------
// Copy constructor.
//
inline VTime::VTime(const VTime& time)
{
	theTime = time.theTime;
}

// --------------------------------------------------------------------------
// Set this VTime object to contain the time of day specified
// in the given VTime object.
//
inline void VTime::set(const VTime& time)
{
	theTime = time.theTime;
}

// --------------------------------------------------------------------------
// Set this VTime to contain the given number of seconds
// from beginning of the day.
//
inline void VTime::setSeconds(long seconds)
{
	if (seconds < 0) {
		theTime = 86400L + (seconds % 86400L);
	}
	else {
		theTime = seconds % 86400L;
	}
}

// --------------------------------------------------------------------------
//
inline void VTime::setNull()
{
	theTime = 86401L;
}

// --------------------------------------------------------------------------
// Return the seconds from the beginning of the day,
// represented by this VTime object.
//
inline long VTime::getSeconds() const
{
	return theTime;
}

// --------------------------------------------------------------------------
// Assignment operator.
//
inline VTime& VTime::operator=(const VTime& time)
{
	set(time);
	return *this;
}

// --------------------------------------------------------------------------
// Return a VTime object which represents this VTime object plus the given
// number of seconds.
//
inline VTime VTime::operator+(int n) const
{
	return plusSeconds(n);
}

// --------------------------------------------------------------------------
// Prefix increment operator.  Add one second to the time represented by this
// VTime object.  Return a reference to this VTime object.
//
inline VTime& VTime::operator++()
{
	addSeconds(1);
	return *this;
}

// --------------------------------------------------------------------------
// Postfix increment operator.  Add one second to the time represented by
// this VTime object.  Return a reference to this VTime object.
//
inline VTime VTime::operator++(int)
{
	VTime time(*this);
	addSeconds(1);
	return time;
}

// --------------------------------------------------------------------------
// Add the given number of seconds to the time represented by this VTime
// object.  Return a reference to this VTime object.
//
inline VTime& VTime::operator+=(int n)
{
	addSeconds(n);
	return *this;
}

// --------------------------------------------------------------------------
// Return a VTime object which represents this VTime object minus the given
// number of seconds.
//
inline VTime VTime::operator-(int n) const
{
	return plusSeconds(-n);
}

// --------------------------------------------------------------------------
// Prefix decrement operator.  Subtract one second from the time represented by
// this VTime object.  Return a reference to this VTime object.
//
inline VTime& VTime::operator--()
{
	addSeconds(-1);
	return *this;
}

// --------------------------------------------------------------------------
// Postfix decrement operator.  Subtract one second from the time represented
// by this VTime object.  Return a reference to this VTime object.
//
inline VTime VTime::operator--(int)
{
	VTime time(*this);
	addSeconds(-1);
	return time;
}

// --------------------------------------------------------------------------
// Add the given number of seconds to the time represented by this VTime
// object.  Return a reference to this VTime object.
//
inline VTime& VTime::operator-=(int n)
{
	addSeconds(-n);
	return *this;
}

// --------------------------------------------------------------------------
// Return the difference in seconds between the time represented by this
// VTime object and the given VTime object.   If this time is less than the
// given time then the value is negative.
//
inline long VTime::operator-(const VTime& time) const
{
	return theTime - time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::operator==(const VTime& time) const
{
	return theTime == time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::operator!=(const VTime& time) const
{
	return theTime != time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::operator<(const VTime& time) const
{
	return theTime < time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::operator>(const VTime& time) const
{
	return theTime > time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::operator<=(const VTime& time) const
{
	return theTime <= time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::operator>=(const VTime& time) const
{
	return theTime >= time.theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VTime::between(const VTime& a, const VTime& b) const
{
	return (theTime >= a.theTime) && (theTime <= b.theTime);
}

// --------------------------------------------------------------------------
//
inline VTime& VTime::max(const VTime& time) const
{
	if (*this >= time) {
		return (VTime&)*this;
	}
	else {
		return (VTime&)time;
	}
}

// --------------------------------------------------------------------------
//
inline VTime& VTime::min(const VTime& time) const
{
	if (*this <= time) {
		return (VTime&)*this;
	}
	else {
		return (VTime&)time;
	}
}

// --------------------------------------------------------------------------
// Return TRUE if this VTime object represents a valid time, otherwise FALSE.
//
inline boolean VTime::isValid() const
{
	return (theTime >= 0) && (theTime <= 86400L);
}

// --------------------------------------------------------------------------
//
inline boolean VTime::isNull() const
{
	return theTime == 86401L;
}

// --------------------------------------------------------------------------
//
inline void VTime::initialize()
{
	theTime = 0;
}

#endif // VTIME_H
