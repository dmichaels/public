// $Header:   Q:/views/common/vcs/THYME.CPV   1.8   Mar 24 1997 16:13:44   scottt  $

//	thyme.cpp
//
//	VTime implementation [Common]
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
//	08/28/95 dgm	Added #ifdef CV_NO_SYSTEM_STRFTIME; the ANSI-C strftime()
//					function is not supported in the Microsoft C++ Win16 DLLs!
//					ToDo: implement a version of strftime() for this case.
//	08/30/95 dgm	Unsigned to signed changes.
//	01/12/96 dgm	Fix in setHour().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "thyme.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// --------------------------------------------------------------------------
//
#if defined(_MSC_VER) && defined(CV_WIN16) && defined(DLL)
#define CV_NO_SYSTEM_STRFTIME
#endif

// --------------------------------------------------------------------------
//
static const long SecondsPerDay = 86400L;

// --------------------------------------------------------------------------
//
#if defined(CV_NO_SYSTEM_STRFTIME)

size_t strftime(char *s, size_t max, const char *format, const struct tm *tm)
{
	//
	// ToDo!!!
	//
	return 0;
}

#endif

// --------------------------------------------------------------------------
// If the given flag is TRUE, then construct this VTime object to
// contain the time of day, otherwise initialize it to an invalid value.
//
VTime::VTime(boolean set_current)
{
	if (set_current) {
		set();
	}
	else {
		initialize();
	}
}

// --------------------------------------------------------------------------
// Construct this VTime object to contain the given time of day in hours,
// minutes, and seconds.
//
VTime::VTime(int hour, int minute, int second)
{
	set(hour, minute, second);
}

// --------------------------------------------------------------------------
// Construct this VTime object to contain the time of day
// specified in the given standard ANSI-C "tm" structure.
//
VTime::VTime(const struct tm *tm)
{
	set(tm);
}

// --------------------------------------------------------------------------
// Set this VTime object to contain the current time of day,
// relative to the local time zone.
//
void VTime::set()
{
	time_t t = time(0);
	set(localtime(&t));
}

// --------------------------------------------------------------------------
// Set this VTime object to contain the given time of day in hours,
// minutes, and seconds.
//
void VTime::set(int hour, int minute, int second)
{
	theTime = (long(hour) * SecondsPerHour) +
			  (long(minute) * SecondsPerMinute) + long(second);
	theTime %= SecondsPerDay;
}

// --------------------------------------------------------------------------
// Set this VTime object to contain the time of day
// specified in the given ANSI-C "tm" structure.
//
void VTime::set(const struct tm *tm)
{
	if (tm == 0) {
		initialize();
	}
	else {
		set(tm->tm_hour, tm->tm_min, tm->tm_sec);
	}
}

// --------------------------------------------------------------------------
// Set the hour of this VTime object to the given value;
// the current minute and second are unchanged.
//
void VTime::setHour(int hour)
{
	if (hour > 23) {
		hour = 23;
	}
	set(hour, getMinute(), getSecond());
}

// --------------------------------------------------------------------------
// Set the minute of this VTime object to the given value;
// the current hour and second are unchanged.
//
void VTime::setMinute(int minute)
{
	if (minute > 59) {
		minute = 59;
	}
	set(getHour(), minute, getSecond());
}

// --------------------------------------------------------------------------
// Set the second of this VTime object to the given value;
// the current minute and second are unchanged.
//
void VTime::setSecond(int second)
{
	if (second > 59) {
		second = 59;
	}
	set(getHour(), getMinute(), second);
}

// --------------------------------------------------------------------------
// Return the hour represented by this VTime object.
//
int VTime::getHour() const
{
	return theTime / SecondsPerHour;
}

// --------------------------------------------------------------------------
// Return the minute represented by this VTime object.
//
int VTime::getMinute() const
{
	return (theTime % SecondsPerHour) / MinutesPerHour;
}

// --------------------------------------------------------------------------
// Return the second represented by this VTime object.
//
int VTime::getSecond() const
{
	return (theTime % SecondsPerHour) % SecondsPerMinute;
}

// --------------------------------------------------------------------------
//
void VTime::getTm(struct tm *tm) const
{
	if (tm == 0) {
		return;
	}
	tm->tm_hour = getHour();
	tm->tm_min = getMinute();
	tm->tm_sec = getSecond();
}

// --------------------------------------------------------------------------
//
void VTime::getHourString(VString& s, boolean abbreviated)
{
	char *format = abbreviated ? "%I" : "%H";
	getString(s, format);
}

// --------------------------------------------------------------------------
//
void VTime::getMinuteString(VString& s, boolean)
{
	getString(s, "%M");
}

// --------------------------------------------------------------------------
//
void VTime::getSecondString(VString& s, boolean)
{
	getString(s, "%S");
}

// --------------------------------------------------------------------------
// Return in the given VString argument, this VTime object represented as a
// string with in a format described by the given format argument.  This
// format string follows exactly the same conventions as the format string
// used by the function strftime() on the ANSI-C library.
//
// %H - replaced by the hour (24-hour clock) as a decimal number(00-23).
// %I - replaced by the hour (12-hour clock) as a decimal number (01-12).
// %M - replaced by the minute as a decimal number (00-59).
// %p - replaced by the locale's equivalent of the AM/PM designations,
//      associated with a 12-hour clock.
// %S - replaced by the second as a decimal number (00-61).
// %X - replaced by the locale's appropriate time representation.
//
void VTime::getString(VString& s, const char *format)
{
	if (format == 0) {
		format = "%X";
	}
	struct tm tm;
	getTm(&tm);
	char buffer[512];
	strftime(buffer, 511, format, &tm);
	s = buffer;
}

// --------------------------------------------------------------------------
// Add the specified number of hours to the time represented by this VTime
// object; the given number of hours may be negative.  
//
void VTime::addHours(int n)
{
	theTime += n * SecondsPerHour;
	theTime %= SecondsPerDay;
	if (theTime < 0) {
		theTime = SecondsPerDay + theTime;
	}
}

// --------------------------------------------------------------------------
// Add the specified number of minutes to the time represented by this VTime
// object; the given number of minutes may be negative.  
//
void VTime::addMinutes(int n)
{
	theTime += n * SecondsPerMinute;
	theTime %= SecondsPerDay;
	if (theTime < 0) {
		theTime = SecondsPerDay + theTime;
	}
}

// --------------------------------------------------------------------------
// Add the specified number of seconds to the time represented by this VTime
// object; the given number of seconds may be negative.  
//
void VTime::addSeconds(int n)
{
	theTime += n;
	theTime %= SecondsPerDay;
	if (theTime < 0) {
		theTime = SecondsPerDay + theTime;
	}
}

// --------------------------------------------------------------------------
// Return a VTime object containing the time represented by this VTime object
// plus the specified number of hours; the given number of hours may be
// negative.  
//
VTime VTime::plusHours(int n) const
{
	VTime time(*this);
	time.addHours(n);
	return time;
}

// --------------------------------------------------------------------------
// Return a VTime object containing the time represented by this VTime object
// plus the specified number of minutes; the given number of minutes may be
// negative.  
//
VTime VTime::plusMinutes(int n) const
{
	VTime time(*this);
	time.addMinutes(n);
	return time;
}

// --------------------------------------------------------------------------
// Return a VTime object containing the time represented by this VTime object
// plus the specified number of seconds; the given number of seconds may be
// negative.  
//
VTime VTime::plusSeconds(int n) const
{
	VTime time(*this);
	time.addSeconds(n);
	return time;
}
