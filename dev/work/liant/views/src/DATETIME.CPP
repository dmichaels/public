// $Header:   Q:/views/common/vcs/DATETIME.CPV   1.10   Mar 24 1997 16:08:42   scottt  $

//	datetime.cpp
//
//	VDateTime implementation [Common]
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
//	08/30/95 dgm	Archiving fix & unsigned to signed changes.
//	09/06/95 pkt	updated for revised C++ standard for loops.
// ---------------------------------------------------------------------------

#include "datetime.h"
#include "timezone.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
#if defined(_MSC_VER) && defined(DLL) && defined(CV_WIN16)
extern size_t strftime(char *, size_t, const char *, const struct tm *);
#endif

// --------------------------------------------------------------------------
//
defineClass(VDateTime, VObject)

// --------------------------------------------------------------------------
//
static const long SecondsPerDay = 86400L;

// --------------------------------------------------------------------------
// If the given flag is TRUE, then construct this VDateTime object
// to contain the the current date and time of day, relative to the
// local time zone, otherwise initialize it to an invalid value.
//
VDateTime::VDateTime(boolean set_current)
	: theDate(FALSE), theTime(FALSE)
{
	if (set_current) {
		set();
	}
	else {
		theZone = 0;
	}
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the given current date and time
// relative to the given time zone.
//
VDateTime::VDateTime(const VTimeZone& timezn)
	: theDate(FALSE), theTime(FALSE)
{
	set();
	setTimeZone(timezn);
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the given date and the current
// time of day, relative to the local time zone.
//
VDateTime::VDateTime(const VDate& date)
	: theDate(date), theTime(FALSE)
{
	time_t t = time(0);
	theTime.set(localtime(&t));
	theZone = 0;
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the given time and the current
// date.
//
VDateTime::VDateTime(const VTime& time)
	: theDate(TRUE), theTime(time)
{
	theZone = 0;
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the given time and date,
// relative to the local time zone.
//
VDateTime::VDateTime(const VDate& date, const VTime& time)
	: theDate(date), theTime(time)
{
	theZone = 0;
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the given time and date,
// relative to the given time zone.
//
VDateTime::VDateTime(const VDate& date, const VTime& time,
					 const VTimeZone& timezn)
	: theDate(date), theTime(time)
{
	theZone = &timezn;
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the date and time of day
// specified in the given standard ANSI-C "tm" structure, relative to
// the local time zone.
//
VDateTime::VDateTime(const struct tm *tm)
	: theDate(tm), theTime(tm)
{
	theZone = 0;
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the date and time of day
// specified in the given standard ANSI-C "tm" structure, relative to
// the given time zone.
//
VDateTime::VDateTime(const struct tm *tm, const VTimeZone& timezn)
	: theDate(tm), theTime(tm)
{
	theZone = &timezn;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the current date and time of day,
// relative to the local time zone.
//
void VDateTime::set()
{
	time_t t = time(0);
	struct tm *tm = localtime(&t);
	theDate.set(tm);
	theTime.set(tm);
	theZone = 0;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given date and the current time.
//
void VDateTime::set(const VDate& date)
{
	theDate.set(date);
	theTime.set();
	theZone = 0;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given date and the current time.
//
void VDateTime::set(const VTime& time)
{
	theDate.set();
	theTime.set(time);
	theZone = 0;
}

// --------------------------------------------------------------------------
//
void VDateTime::set(const VDate& date, const VTime& time)
{
	theDate.set(date);
	theTime.set(time);
	theZone = 0;
}

// --------------------------------------------------------------------------
//
void VDateTime::set(const VDate& date, const VTime& time,
					const VTimeZone& timezn)
{
	theDate.set(date);
	theTime.set(time);
	theZone = &timezn;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the date and time of day
// specified in the given standard ANSI-C "tm" structure.
//
void VDateTime::set(const struct tm *tm)
{
	theDate.set(tm);
	theTime.set(tm);
	theZone = 0;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the date and time of day
// specified in the given standard ANSI-C "tm" structure.
//
void VDateTime::set(const struct tm *tm, const VTimeZone& timezn)
{
	theDate.set(tm);
	theTime.set(tm);
	theZone = &timezn;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the current date, relative to the
// local time zone; the current time of day and time zone are unchanged.
//
void VDateTime::setDate()
{
	time_t t = time(0);
	theDate.set(localtime(&t));
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given date;
// the current time of day and time zone are unchanged.
//
void VDateTime::setDate(const VDate& date)
{
	theDate.set(date);
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given date;
// the current time of day and time zone are unchanged.
//
void VDateTime::setDate(VDate::Year year, VDate::DayOfYear day_of_year)
{
	theDate.set(year, day_of_year);
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given date;
// the current time of day and time zone are unchanged.
//
void VDateTime::setDate(VDate::Year year,
						VDate::Month month,
						VDate::DayOfMonth day_of_month)
{
	theDate.set(year, month, day_of_month);
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given date;
// the current time of day and time zone are unchanged.
//
void VDateTime::setDate(VDate::Month month, VDate::DayOfMonth day_of_month)
{
	theDate.set(month, day_of_month);
}

// --------------------------------------------------------------------------
// Set the year of this VDateTime object to the given value;
// the current day, day-of-month, time, and time zone are unchanged.
//
void VDateTime::setYear(VDate::Year year)
{
	theDate.setYear(year);
}

// --------------------------------------------------------------------------
// Set the year of this VDateTime object to the given valuej;
// the current year, day-of-month, time, and time zone are unchanged.
//
void VDateTime::setMonth(VDate::Month month)
{
	theDate.setMonth(month);
}

// --------------------------------------------------------------------------
// Set the day-of-week of this VDateTime object to the given value within
// the same week as the current week; the current year, day-of-month,
// time, and time zone are unchanged.
//
void VDateTime::setDayOfWeek(VDate::DayOfWeek day_of_week)
{
	theDate.setDayOfWeek(day_of_week);
}

// --------------------------------------------------------------------------
// Set the day-of-month of this VDateTime object to the given value;
// the current year, month, time, and time zone are unchanged.
//
void VDateTime::setDayOfMonth(VDate::DayOfMonth day_of_month)
{
	theDate.setDayOfMonth(day_of_month);
}

// --------------------------------------------------------------------------
// Set the day-of-year of this VDateTime object to the given value;
// the current year, time, and time zone are unchanged.
//
void VDateTime::setDayOfYear(VDate::DayOfYear day_of_year)
{
	theDate.setDayOfYear(day_of_year);
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the current time, relative to
// the current time zone; the current date and time zone are unchanged.
//
void VDateTime::setTime()
{
	time_t t = time(0);
	theTime.set(localtime(&t));
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given time;
// the current date and time zone are unchanged.
//
void VDateTime::setTime(const VTime& time)
{
	theTime.set(time);
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the given hour, minute, and second;
// the current date and time zone are unchanged.
//
void VDateTime::setTime(int hour, int minute, int second)
{
	theTime.set(hour, minute, second);
}

// --------------------------------------------------------------------------
// Set the hour of this VDateTime object to the given value;
// the current minute, second, date, and time zone are unchanged.
//
void VDateTime::setHour(int hour)
{
	theTime.setHour(hour);
}

// --------------------------------------------------------------------------
// Set the minute of this VDateTime object to the given value;
// the current hour, second, and date are unchanged.
//
void VDateTime::setMinute(int minute)
{
	theTime.setMinute(minute);
}

// --------------------------------------------------------------------------
// Set the second of this VDateTime object to the given value;
// the current minute, second, date, and time zone are unchanged.
//
void VDateTime::setSecond(int second)
{
	theTime.setSecond(second);
}

// --------------------------------------------------------------------------
// Set the time zone of this VDateTime object to the given VTimeZone.
//
void VDateTime::setTimeZone(const VTimeZone& timezn)
{
	const VTimeZone &this_timezone = getTimeZone();
	int current_minutes_from_gmt =
		this_timezone.getMinutesFromGMT(this_timezone.isDST(*this));
	theZone = &timezn;
	int new_minutes_from_gmt =
		timezn.getMinutesFromGMT(timezn.isDST(*this));
	addMinutes(new_minutes_from_gmt - current_minutes_from_gmt);
}

// --------------------------------------------------------------------------
// Set the time zone of this VDateTime object to the local time zone.
//
void VDateTime::setTimeZone()
{
	if (theZone == 0) {
		return;
	}
	VTimeZone& local_timezone = VTimeZone::local();
	if (theZone == &local_timezone) {
		return;
	}
	const VTimeZone &this_timezone = getTimeZone();
	int current_minutes_from_gmt =
		this_timezone.getMinutesFromGMT(this_timezone.isDST(*this));
	int new_minutes_from_gmt =
		local_timezone.getMinutesFromGMT(local_timezone.isDST(*this));
	theZone = 0;
	addMinutes(new_minutes_from_gmt - current_minutes_from_gmt);
}

// --------------------------------------------------------------------------
// Return the time zone of this VDateTime object to the given VTimeZone.
//
const VTimeZone& VDateTime::getTimeZone() const
{
	if (theZone == 0) {
		((VDateTime *)this)->theZone = &VTimeZone::local();
	}
	return *theZone;
}

// --------------------------------------------------------------------------
//
VDate::Year VDateTime::getYear() const
{
	return theDate.getYear();
}

// --------------------------------------------------------------------------
//
VDate::Month VDateTime::getMonth() const
{
	return theDate.getMonth();
}

// --------------------------------------------------------------------------
//
VDate::DayOfMonth VDateTime::getDayOfMonth() const
{
	return theDate.getDayOfMonth();
}

// --------------------------------------------------------------------------
//
VDate::DayOfWeek VDateTime::getDayOfWeek() const
{
	return theDate.getDayOfWeek();
}

// --------------------------------------------------------------------------
//
VDate::DayOfYear VDateTime::getDayOfYear() const
{
	return theDate.getDayOfYear();
}

// --------------------------------------------------------------------------
//
int VDateTime::getDaysPerYear() const
{
	return theDate.getDaysPerYear();
}

// --------------------------------------------------------------------------
//
int VDateTime::getDaysPerMonth() const
{
	return theDate.getDaysPerMonth();
}

// --------------------------------------------------------------------------
//
boolean VDateTime::isLeapYear() const
{
	return theDate.isLeapYear();
}

// --------------------------------------------------------------------------
//
long VDateTime::getJulianDay() const
{
	return theDate.getJulianDay();
}

// --------------------------------------------------------------------------
// Return the hour represented by this VDateTime object.
//
int VDateTime::getHour() const
{
	return theTime.getHour();
}

// --------------------------------------------------------------------------
// Return the minute represented by this VDateTime object.
//
int VDateTime::getMinute() const
{
	return theTime.getMinute();
}

// --------------------------------------------------------------------------
// Return the second represented by this VDateTime object.
//
int VDateTime::getSecond() const
{
	return theTime.getSecond();
}

// --------------------------------------------------------------------------
//
void VDateTime::getTm(struct tm *tm, boolean getdst) const
{
	theDate.getTm(tm);
	theTime.getTm(tm);
	if (getdst) {
		tm->tm_isdst = getTimeZone().isDST(*this) ? 1 : 0;
	}
	else {
		tm->tm_isdst = -1;
	}
}

// --------------------------------------------------------------------------
// Return TRUE if Daylight Savings Time is in effect for the date, time, and
// time zone currently represented by this VDateTime object, otherwise FALSE.
//
boolean VDateTime::isDST() const
{
	return getTimeZone().isDST(*this);
}

// --------------------------------------------------------------------------
//
void VDateTime::getYearString(VString& s, boolean abbreviated)
{
	theDate.getYearString(s, abbreviated);
}

// --------------------------------------------------------------------------
//
void VDateTime::getRomanYearString(VString& s)
{
	theDate.getRomanYearString(s);
}

// --------------------------------------------------------------------------
//
void VDateTime::getMonthString(VString& s, boolean abbreviated)
{
	theDate.getMonthString(s, abbreviated);
}

// --------------------------------------------------------------------------
//
void VDateTime::getDayOfWeekString(VString& s, boolean abbreviated)
{
	theDate.getDayOfWeekString(s, abbreviated);
}

// --------------------------------------------------------------------------
//
void VDateTime::getHourString(VString& s, boolean abbreviated)
{
	theTime.getHourString(s, abbreviated);
}

// --------------------------------------------------------------------------
//
void VDateTime::getMinuteString(VString& s, boolean abbreviated)
{
	theTime.getMinuteString(s);
}

// --------------------------------------------------------------------------
//
void VDateTime::getSecondString(VString& s, boolean abbreviated)
{
	theTime.getSecondString(s);
}

// --------------------------------------------------------------------------
//
void VDateTime::getTimeZoneString(VString& s, boolean abbreviated)
{
	const VTimeZone &this_timezone = getTimeZone();
	this_timezone.getName(s, this_timezone.isDST(*this), abbreviated);
}

// --------------------------------------------------------------------------
// Return in the given VString argument, this VDateTime object represented as
// a string with in a format described by the given format argument.  This
// format string follows exactly the same conventions as the format string
// used by the function strftime() on the ANSI-C library.
//
// Date Related:
// -------------
// %a - replaced by the locale's abbreviated weekday name.
// %A - replaced by the locale's full weekday name.
// %b - replaced by the locale's abbreviated month name.
// %B - replaced by the locale's full month name.
// %d - replaced by the day of the month as a decimal number (01-31).
// %j - replaced by the day of the year as a decimal number (001-366).
// %m - replaced by the month as a decimal number (01-12).
// %U - replaced by the week number of the year as a decimal number (00-53);
//      the first Sunday is the first day of week 1.
// %w - replaced by the weekday as a decimal number (0-6), where Sunday is 0.
// %W - replaced by the week number of the year as a decimal number (00-53);
//      the first Monday is the first day of week 1.
// %x - replaced by the locale's appropriate date representation.
// %y - replaced by the year without century as a decimal number (00-99).
// %Y - replace by the year with century as a decimal number.
//
// Time Related:
// -------------
// %H - replaced by the hour (24-hour clock) as a decimal number(00-23).
// %I - replaced by the hour (12-hour clock) as a decimal number (01-12).
// %M - replaced by the minute as a decimal number (00-59).
// %p - replaced by the locale's equivalent of the AM/PM designations,
//      associated with a 12-hour clock.
// %S - replaced by the second as a decimal number (00-61).
// %X - replaced by the locale's appropriate time representation.
//
// Date/Time Related:
// ------------------
// %c - replaced by the locale's appropriate date and time representation.
// %Z - replaced by the time zone name or abbreviation
//      or by nothing if the time zone is not determinable.
//
// Problems:
// ---------
// o The %c, %x, and %X formats only work for the local time zone.
// o Years before 1900 will be correctly formatted, but only if the
//   string does not contain digit characters adjacent to the year.
// o Years greater than 9599 will not be correctly formatted.
//
void VDateTime::getString(VString& s, const char *format)
{
	if (format == 0) {
		format = "%c";
	}

	struct tm tm;
	getTm(&tm);

	char buffer[1024];

	int year = theDate.getYear();

	if (year < 1900) {
		//
		// The following nonsense is just to handle years before 1900.
		// To do this, we scan change tm_year value which is less than
		// 1900 to a compatible (leap/non-leap) year in the range 9600-9999.
		// We then scan the resultant string (strftime()) for any numbers
		// in the range 9600-9999, and change them back to the original
		// value.  This is gross because if such a string is in the given
		// format string, that will be changed too, and if the year is
		// adjacent to another number it my not get changed at all.
		// It would be best if we could just do the format scanning
		// by hand (instead of using strftime()), but the problem is
		// determining %c and %x values!
		//
		tm.tm_year = (((year / 100) * 100) % 400) +
					 (9600 - 1900) + (year % 100);
		strftime(buffer, 1023, format, &tm);
		char ns[4], ys[4 + 1]; int i = -1;
		sprintf(ys, "%d", year);
		for (char *p = buffer ; *p != '\0' ; p++) {
			if (isdigit(*p)) {
				if (i >= (4 - 1)) {
					for (p++ ; (*p != '\0') && isdigit(*p) ; p++) {
						;
					}
					i = -1;
				}
				else {
					ns[++i] = *p;
				}
			}
			else if (i > 0) {
				ReplaceBogusYear:
				int n = 0;
				for (int m = 1, j = i ; j >= 0 ; j--, m *= 10) {
					n += (ns[j] - '0') * m;
				}
				if ((n >= 9600) && (n <= 9999)) {
					int j;
					for (j = 0 ; j <= i ; j++) {
						p[j - 4] = ys[j];
					}
					if (j < (4 - 1)) {
						int k = (4 - 1) - j;
					 	for (int l = 0 ; 1 ; l++) {
							if ((p[l - k] = p[l]) == '\0') {
								break;
							}
						}
						p -= k;
					}
				}
				i = -1;
			}
			else {
				i = -1;
			}
		}
		if (i > 0) {
			goto ReplaceBogusYear;
		}
	}
	else {
		strftime(buffer, 1023, format, &tm);
	}

	s = buffer;
}

// --------------------------------------------------------------------------
//
void VDateTime::addYears(int n)
{
	theDate.addYears(n);
}

// --------------------------------------------------------------------------
//
void VDateTime::addMonths(int n)
{
	theDate.addMonths(n);
}

// --------------------------------------------------------------------------
//
void VDateTime::addWeeks(int n)
{
	theDate.addWeeks(n);
}

// --------------------------------------------------------------------------
//
void VDateTime::addDays(int n)
{
	theDate.addDays(n);
}

// --------------------------------------------------------------------------
//
void VDateTime::addHours(int n)
{
	addSeconds(n * VTime::SecondsPerHour);
}

// --------------------------------------------------------------------------
//
void VDateTime::addMinutes(int n)
{
	addSeconds(n * VTime::SecondsPerMinute);
}

// --------------------------------------------------------------------------
//
void VDateTime::addSeconds(int n)
{
	long t = theTime.getSeconds() + n, d;
	if (t < 0) {
		d = t / SecondsPerDay - 1;
	}
	else {
		d = t / SecondsPerDay;
	}
	if (d != 0) {
		theDate.addDays(d);
	}
	theTime.setSeconds(t);
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusYears(int n) const
{
	VDateTime datetime(*this);
	datetime.addYears(n);
	return datetime;
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusMonths(int n) const
{
	VDateTime datetime(*this);
	datetime.addMonths(n);
	return datetime;
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusWeeks(int n) const
{
	VDateTime datetime(*this);
	datetime.addWeeks(n);
	return datetime;
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusDays(int n) const
{
	VDateTime datetime(*this);
	datetime.addDays(n);
	return datetime;
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusHours(int n) const
{
	VDateTime datetime(*this);
	datetime.addHours(n);
	return datetime;
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusMinutes(int n) const
{
	VDateTime datetime(*this);
	datetime.addMinutes(n);
	return datetime;
}

// --------------------------------------------------------------------------
//
VDateTime VDateTime::plusSeconds(int n) const
{
	VDateTime datetime(*this);
	datetime.addSeconds(n);
	return datetime;
}

// --------------------------------------------------------------------------
// Return the difference in seconds between the date/time represented by this
// VDateTime object and the given VDateTime object.   If this time is less
// than the given time then the value is negative.
//
long VDateTime::operator-(const VDateTime& datetime) const
{
	long date_difference, time_difference;

	if (theZone == datetime.theZone) {
		date_difference = theDate - datetime.theDate;
		time_difference = theTime - datetime.theTime;
	}
	else if (datetime.theZone != 0) {
		VDateTime d(*this);
		d.setTimeZone(*datetime.theZone);
		date_difference = d.theDate - datetime.theDate;
		time_difference = d.theTime - datetime.theTime;
	}
	else {
		VDateTime d(datetime);
		d.setTimeZone(*theZone);
		date_difference = theDate - d.theDate;
		time_difference = theTime - d.theTime;
	}
	return date_difference * SecondsPerDay + time_difference;
}

// --------------------------------------------------------------------------
//
boolean VDateTime::operator==(const VDateTime& datetime) const
{
	if (theZone == datetime.theZone) {
		return (theDate == datetime.theDate) &&
			   (theTime == datetime.theTime);
	}
	else if (datetime.theZone != 0) {
		VDateTime dt(*this, *datetime.theZone);
		return (dt.theDate == datetime.theDate) &&
			   (dt.theTime == datetime.theTime);
	}
	else {
		VDateTime dt(datetime, *theZone);
		return (theDate == dt.theDate) &&
			   (theTime == dt.theTime);
	}
}

// --------------------------------------------------------------------------
//
boolean VDateTime::operator!=(const VDateTime& datetime) const
{
	if (theZone == datetime.theZone) {
		return (theDate != datetime.theDate) ||
			   (theTime != datetime.theTime);
	}
	else if (datetime.theZone != 0) {
		VDateTime dt(*this, *datetime.theZone);
		return (dt.theDate != datetime.theDate) ||
			   (dt.theTime != datetime.theTime);
	}
	else {
		VDateTime dt(datetime, *theZone);
		return (theDate != dt.theDate) ||
			   (theTime != dt.theTime);
	}
}

// --------------------------------------------------------------------------
//
boolean VDateTime::operator<(const VDateTime& datetime) const
{
	if (theZone == datetime.theZone) {
		return (theDate < datetime.theDate) ||
			   ((theDate == datetime.theDate) &&
			    (theTime < datetime.theTime));
	}
	else if (datetime.theZone != 0) {
		VDateTime dt(*this, *datetime.theZone);
		return (dt.theDate < datetime.theDate) ||
			   ((dt.theDate == datetime.theDate) &&
			    (dt.theTime < datetime.theTime));
	}
	else {
		VDateTime dt(datetime, *theZone);
		return (theDate < dt.theDate) ||
			   ((theDate == dt.theDate) &&
			    (theTime < dt.theTime));
	}
}

// --------------------------------------------------------------------------
//
boolean VDateTime::operator>(const VDateTime& datetime) const
{
	if (theZone == datetime.theZone) {
		return (theDate > datetime.theDate) ||
			   ((theDate == datetime.theDate) &&
			    (theTime > datetime.theTime));
	}
	else if (datetime.theZone != 0) {
		VDateTime dt(*this, *datetime.theZone);
		return (dt.theDate > datetime.theDate) ||
			   ((dt.theDate == datetime.theDate) &&
			    (dt.theTime > datetime.theTime));
	}
	else {
		VDateTime dt(datetime, *theZone);
		return (theDate > dt.theDate) ||
			   ((theDate == dt.theDate) &&
			    (theTime > dt.theTime));
	}
}

// --------------------------------------------------------------------------
//
boolean VDateTime::operator<=(const VDateTime& datetime) const
{
	if (theZone == datetime.theZone) {
		return (theDate < datetime.theDate) ||
			   ((theDate == datetime.theDate) &&
			    (theTime <= datetime.theTime));
	}
	else if (datetime.theZone != 0) {
		VDateTime dt(*this, *datetime.theZone);
		return (dt.theDate < datetime.theDate) ||
			   ((dt.theDate == datetime.theDate) &&
			    (dt.theTime <= datetime.theTime));
	}
	else {
		VDateTime dt(datetime, *theZone);
		return (theDate < dt.theDate) ||
			   ((theDate == dt.theDate) &&
			    (theTime <= dt.theTime));
	}
}

// --------------------------------------------------------------------------
//
boolean VDateTime::operator>=(const VDateTime& datetime) const
{
	if (theZone == datetime.theZone) {
		return (theDate > datetime.theDate) ||
			   ((theDate == datetime.theDate) &&
			    (theTime >= datetime.theTime));
	}
	else if (datetime.theZone != 0) {
		VDateTime dt(*this, *datetime.theZone);
		return (dt.theDate > datetime.theDate) ||
			   ((dt.theDate == datetime.theDate) &&
			    (dt.theTime >= datetime.theTime));
	}
	else {
		VDateTime dt(datetime, *theZone);
		return (theDate > dt.theDate) ||
			   ((theDate == dt.theDate) &&
			    (theTime >= dt.theTime));
	}
}

// --------------------------------------------------------------------------
//
boolean VDateTime::isEqual(VObject *object) const
{
	if ((object == 0) || !object->isA(VDateTimeCls)) {
		return FALSE;
	}
	return *this == *((VDateTime *)object);
}

// --------------------------------------------------------------------------
//
HashKey VDateTime::hash() const
{
	VDateTime dt(VDate(1901, VDate::January, VDate::DayOfMonth(1)),
				 VTime(0, 0, 0));
	return HashKey(*this - dt);
}

// --------------------------------------------------------------------------
//
int VDateTime::compare(VObject *object) const
{
	if ((object == 0) || !object->isA(VDateTimeCls)) {
		return 0;
	}
	long d = *this - *((VDateTime *)object);
	if (d < 0) {
		return -1;
	}
	else if (d > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
//
void VDateTime::putTo(VArchiver& a)
{
	a << theDate.getJulianDay();
	a << theTime.getSeconds();
	if ((theZone == 0) || (theZone == &VTimeZone::local())) {
		a << char(-1);
		a.putObject((VTimeZone *)(&VTimeZone::local()), TRUE);
	}
	else {
		a << char(1);
		a.putObject((VTimeZone *)theZone, TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VDateTime::getFrom(VArchiver& a)
{
	long jd;
	long s;
	char c;
	a >> jd; theDate.setJulianDay(jd);
	a >> s; theTime.setSeconds(s);
	a >> c;
	if (c != 0) {
		theZone = (VTimeZone *)(a.getObject(0));
		if (c < 0) {
			VTimeZone *tz = (VTimeZone *)theZone;
			setTimeZone();
			delete tz;
			theZone = 0;
		}
	}
	else {
		theZone = 0;
	}
}

// --------------------------------------------------------------------------
//
void VDateTime::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

#endif
