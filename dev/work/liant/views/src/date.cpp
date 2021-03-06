// $Header:   Q:/views/common/vcs/date.cpv   1.11   13 May 1997 15:33:40   davidmi  $

//	date.cpp
//
//	VDate implementation [Common]
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
//	01/12/96 dgm	Fix in setDayOfMonth().
//	01/31/96 dgm	Removed static functions: getYearString(),
//					getMonthString(), getDayOfWeekString(),
//					and getRomanYearString().
//	05/13/97 dgm	Fix in set(... nth_day_of_week_within_month ...).
// ---------------------------------------------------------------------------

#include "date.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// --------------------------------------------------------------------------
//
#if defined(_MSC_VER) && defined(DLL) && defined(CV_WIN16)
extern size_t strftime(char *, size_t, const char *, const struct tm *);
#endif

// --------------------------------------------------------------------------
// Local/private macro definitions.
//
#define IS_LEAP_YEAR(y)					((((y) % 4 == 0) &&		\
										  ((y) % 100 != 0)) ||	\
										 ((y) % 400 == 0))
#define GET_YEAR_FROM_TM(tm)			(Year((tm)->tm_year + 1900))
#define GET_MONTH_FROM_TM(tm)			(Month((tm)->tm_mon + 1))
#define GET_DAY_OF_MONTH_FROM_TM(tm)	(DayOfMonth((tm)->tm_mday))

// --------------------------------------------------------------------------
// Local/private static data.
//
static char daysPerMonth[] =
{
	31,		// January
	28,		// February
	31,		// March
	30,		// April
	31,		// May
	30,		// June
	31,		// July
	31,		// August
	30,		// September
	31,		// October
	30,		// November
	31		// December
};
static char daysPerLeapYearMonth[] =
{
	31,		// January
	29,		// February
	31,		// March
	30,		// April
	31,		// May
	30,		// June
	31,		// July
	31,		// August
	30,		// September
	31,		// October
	30,		// November
	31		// December
};
static short ndaysBeforeMonth[] =
{
	 0,		// January	 (31)
	 31,	// February	 (28)
	 59,	// March	 (31)
	 90,	// April	 (30)
	120,	// May		 (31)
	151,	// June		 (30)
	181,	// July		 (31)
	212,	// August	 (31)
	243,	// September (30)
	273,	// October	 (31)
	304,	// November	 (30)
	334		// December	 (31)
};
static short ndaysBeforeMonthInLeapYear[] =
{
	 0,		// January	 (31)
	 31,	// February	 (29)
	 60,	// March	 (31)
	 91,	// April	 (30)
	121,	// May		 (31)
	152,	// June		 (30)
	182,	// July		 (31)
	213,	// August	 (31)
	244,	// September (30)
	274,	// October	 (31)
	305,	// November	 (30)
	335,	// December	 (31)
};

// --------------------------------------------------------------------------
// Construct this VDate object to represent the system's best idea of what
// the current date is if the given boolean is TRUE, otherwise initialize it.
//
VDate::VDate(boolean set_current)
{
	if (set_current) {
		set();
	}
	else {
		initialize();
	}
}

// --------------------------------------------------------------------------
// Construct this VDate object to represent the given day-of-year of the
//
VDate::VDate(VDate::Year year, VDate::DayOfYear day_of_year)
{
	set(year, day_of_year);
}

// --------------------------------------------------------------------------
// Construct this VDate object to represent the given year, month, and
// day-of-month.
//
VDate::VDate(VDate::Year year, VDate::Month month,
			 VDate::DayOfMonth day_of_month)
{
	set(year, month, day_of_month);
}

// --------------------------------------------------------------------------
// Construct this VDate object to represent the given month and day-of-month
// of the current year.
//
VDate::VDate(VDate::Month month, VDate::DayOfMonth day_of_month)
{
	set(month, day_of_month);
}

// --------------------------------------------------------------------------
// Construct this VDate object to the value representing the given
// ANSI-C "tm" structure.
//
VDate::VDate(const struct tm *tm)
{
	set(tm);
}

// --------------------------------------------------------------------------
// Construct this VDate object to represent the given year, month, and nth
// day-of-week within the month.  If the that nth day-of-week is out
// of range is out of range, low then it will quietly be slid into range.
//
VDate::VDate(VDate::Year year, VDate::Month month,
			 VDate::DayOfWeek day_of_week,
			 int nth_day_of_week_within_month)
{
	set(year, month, day_of_week, nth_day_of_week_within_month);
}

// --------------------------------------------------------------------------
// Set this VDate object to represent the current date,
// relative to the local time zone.
//
void VDate::set()
{
	time_t t = time(0);
	set(localtime(&t));
}

// --------------------------------------------------------------------------
// Set this VDate object to represent the given day-of-year of the given year. 
//
void VDate::set(VDate::Year year, VDate::DayOfYear day_of_year)
{
	VDateData dd;
	dd.year = year;
	dd.day_of_year = day_of_year;
	getDataFromYearAndDay(dd);
	theYear = dd.year;
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Set this VDate object to represent the given year, month, and day-of-month.
//
void VDate::set(VDate::Year year, VDate::Month month,
				VDate::DayOfMonth day_of_month)
{
	VDateData dd;
	dd.year = year;
	dd.month = month;
	dd.day_of_month = day_of_month;
	getDataFromYearMonthAndDay(dd);
	theYear = dd.year;
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Set this VDate object to represent the given year, month, and nth
// day-of-week within the month.  If the that nth day-of-week is too
// high or zero or negative, then it is assumed to refer to the last
// day-of-week within the month.
//
void VDate::set(VDate::Year year, VDate::Month month,
				VDate::DayOfWeek day_of_week,
				int nth_day_of_week_within_month)
{
	VDateData dd;
	dd.year = year;
	dd.month = month;
	dd.day_of_month = 1;
	getDataFromYearMonthAndDay(dd);
	if (day_of_week < FirstDayOfWeek) {
		day_of_week = FirstDayOfWeek;
	}
	else if (day_of_week > LastDayOfWeek) {
		day_of_week = LastDayOfWeek;
	}
	int first, nth;
	if (day_of_week > dd.day_of_week) {
		first = day_of_week - dd.day_of_week + 1;
	}
	else if (day_of_week < dd.day_of_week) {
		first = (DaysPerWeek - dd.day_of_week) + day_of_week + 1;
	}
	else {
		first = 1;
	}
	if (nth_day_of_week_within_month <= 0) {
		nth_day_of_week_within_month = 32767;
	}
	nth = first + ((nth_day_of_week_within_month - 1) * DaysPerWeek);
	if (nth > dd.days_in_month) {
		for (nth  = first + DaysPerWeek ; 1 ; nth += DaysPerWeek) {
			if (nth > dd.days_in_month) {
				nth -= DaysPerWeek;
				break;
			}
		}
	}
	dd.day_of_month = nth;
	getDataFromYearMonthAndDay(dd);
	theYear = dd.year;
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Set this VDate object to represent the given month and day-of-month
// of the current year, relative to the local time zone.
//
void VDate::set(VDate::Month month, VDate::DayOfMonth day_of_month)
{
	time_t t = time(0);
	struct tm *tm;

	if (t == time_t(-1)) {
		initialize();
	}
	else if ((tm = localtime(&t)) == 0) {
		initialize();
	}
	else {
		set(GET_YEAR_FROM_TM(tm), month, day_of_month);
	}
}

// --------------------------------------------------------------------------
// Set this VDate object to the value representing the given
// ANSI-C "tm" structure.
//
void VDate::set(const struct tm *tm)
{
	if (tm == 0) {
		initialize();
	}
	else {
		set(GET_YEAR_FROM_TM(tm),
			GET_MONTH_FROM_TM(tm),
			GET_DAY_OF_MONTH_FROM_TM(tm));
	}
}

// --------------------------------------------------------------------------
// Return the value of this VDate object in the given ANSI-C "tm" structure;
// set the time-of-day related fields of the structure to null.
//
void VDate::getTm(struct tm *tm) const
{
	if (tm == 0) {
		return;
	}
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	tm->tm_year = dd.year <= 1900 ? 0 : dd.year - 1900;
	tm->tm_yday = dd.day_of_year - 1;
	tm->tm_mon = dd.month - 1;
	tm->tm_mday = dd.day_of_month;
	tm->tm_wday = dd.day_of_week - 1;
}

// --------------------------------------------------------------------------
// Set the year of this VDate to the given value while maintaining
// the month and day-of-month.
//
void VDate::setYear(VDate::Year year)
{
	if (year == theYear) {
		return;
	}
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	dd.year = year;
	getDataFromYearMonthAndDay(dd);
	theYear = dd.year;
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Set the month of this VDate object to the given value while maintaining
// the year and the day-of-month.
//
void VDate::setMonth(VDate::Month month)
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	dd.month = month;
	getDataFromYearMonthAndDay(dd);
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Set the day-of-week of this VDate to the given day-of-week
// within the same week as the current.
//
void VDate::setDayOfWeek(VDate::DayOfWeek day_of_week)
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	if (day_of_week == dd.day_of_week) {
		return;
	}
	if (day_of_week < FirstDayOfWeek) {
		day_of_week = FirstDayOfWeek;
	}
	else if (day_of_week > LastDayOfWeek) {
		day_of_week = LastDayOfWeek;
	}
	addDays(day_of_week - dd.day_of_week);
}

// --------------------------------------------------------------------------
// Set the day-of-month of this VDate object to the given value while
// maintaining the year and month.
//
void VDate::setDayOfMonth(VDate::DayOfMonth day_of_month)
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	if (day_of_month == dd.day_of_month) {
		return;
	}
	dd.day_of_month = day_of_month;
	getDataFromYearMonthAndDay(dd);
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Set the day-of-year of this VDate object to the given value while
// maintaining the year and month.
//
void VDate::setDayOfYear(VDate::DayOfYear day_of_year)
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = day_of_year;
	getDataFromYearAndDay(dd);
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
//
void VDate::setJulianDay(long j)
{
	long y, m, d;
	getYearMonthAndDayFromJulianDay(j, y, m, d);
	set(Year(y), Month(m), DayOfMonth(d));
}

// --------------------------------------------------------------------------
// Set this VDate object to represent Friday, October 15, 1582,
// which is the official date of the Gregorian calendar ordered
// by Pope Gregory XIII.  The date before that day was Thursday,
// October 4, 1582 which was the last day of the Julian calendar.
//
void VDate::setStartOfGregorian()
{
	set(Year(1582), Month(October), DayOfMonth(15));
}

// --------------------------------------------------------------------------
// Return the year represented by this VDate object.
//
VDate::Year VDate::getYear() const
{
	return theYear;
}

// --------------------------------------------------------------------------
// Return the month represented by this VDate object.
//
VDate::Month VDate::getMonth() const
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	return dd.month;
}

// --------------------------------------------------------------------------
// Return the day-of-month represented by this VDate object.
//
VDate::DayOfMonth VDate::getDayOfMonth() const
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	return dd.day_of_month;
}

// --------------------------------------------------------------------------
// Return the day-of-week represented by this VDate object.
//
VDate::DayOfWeek VDate::getDayOfWeek() const
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	return dd.day_of_week;
}

// --------------------------------------------------------------------------
// Return the day-of-year represented by this VDate object.
//
VDate::DayOfYear VDate::getDayOfYear() const
{
	return dayOfYear;
}

// --------------------------------------------------------------------------
// Return the number of days-per-year of the year represented by this VDate
// object.
//
int VDate::getDaysPerYear() const
{
	return IS_LEAP_YEAR(theYear) ? DaysPerLeapYear : DaysPerNonLeapYear;
}

// --------------------------------------------------------------------------
// Return the number of days-per-year of the year represented by the given
// year.
//
int VDate::getDaysPerYear(VDate::Year year)
{
	return IS_LEAP_YEAR(year) ? DaysPerLeapYear : DaysPerNonLeapYear;
}

// --------------------------------------------------------------------------
// Return the number of days-per-month of the month represented by this VDate
// object.
//
int VDate::getDaysPerMonth() const
{
	if (IS_LEAP_YEAR(theYear)) {
		return daysPerLeapYearMonth[getMonth()];
	}
	else {
		return daysPerMonth[getMonth()];
	}
}

// --------------------------------------------------------------------------
// Return the number of days-per-month of the month represented by the given
// month and year.
//
int VDate::getDaysPerMonth(VDate::Month month, VDate::Year year)
{
	if (month < FirstMonth) {
		month = FirstMonth;
	}
	else if (month > LastMonth) {
		month = LastMonth;
	}
	if (IS_LEAP_YEAR(year)) {
		return daysPerLeapYearMonth[month];
	}
	else {
		return daysPerMonth[month];
	}
}

// --------------------------------------------------------------------------
// Return TRUE if the year represented by this VDate object is a leap-year,
// otherwise return FALSE.
//
boolean VDate::isLeapYear() const
{
	return IS_LEAP_YEAR(theYear);
}

// --------------------------------------------------------------------------
// Return TRUE if the year represented by the given year a leap-year,
// otherwise return FALSE.
//
int VDate::isLeapYear(VDate::Year year)
{
	return IS_LEAP_YEAR(year);
}

// --------------------------------------------------------------------------
//
long VDate::getJulianDay() const
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	long j;
	getJulianDayFromYearMonthAndDay(dd.year, dd.month, dd.day_of_month, j);
	return j;
}

// --------------------------------------------------------------------------
// Return in the given VString object, the name of the week represented by
// this VDate object.
//
void VDate::getDayOfWeekString(VString& s, boolean abbreviated)
{
	char buffer[512], *format = abbreviated ? "%a" : "%A";
	struct tm tm;
	tm.tm_wday = getDayOfWeek() - 1;
	strftime(buffer, 511, format, &tm);
	s = buffer;
}

// --------------------------------------------------------------------------
// Return in the given VString object, the name of the month represented by
// this VDate object.
//
void VDate::getMonthString(VString& s, boolean abbreviated)
{
	char buffer[512], *format = abbreviated ? "%b" : "%B";
	struct tm tm;
	tm.tm_mon = getMonth() - 1;
	strftime(buffer, 511, format, &tm);
	s = buffer;
}

// --------------------------------------------------------------------------
// Return in the given VString object, the name of the year represented by
// this VDate object.
//
void VDate::getYearString(VString& s, boolean abbreviated)
{
	if (abbreviated) {
		s.prints("%d", theYear % 100);
	}
	else {
		s.prints("%d", theYear);
	}
}

// --------------------------------------------------------------------------
// Return in the given VString object, the name of the year represented by
// this VDate object in roman numeral form.
//
void VDate::getRomanYearString(VString& s)
{
	getRomanNumeralString(s, theYear);
}

// --------------------------------------------------------------------------
// Return in the given VString argument, this VDate object represented as a
// string with in a format described by the given format argument.  This
// format string follows exactly the same conventions as the format string
// used by the function strftime() on the ANSI-C library.
//
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
void VDate::getString(VString& s, const char *format)
{
	if (format == 0) {
		format = "%x";
	}
	struct tm tm;
	getTm(&tm);
	VString new_format_string;
	if (tm.tm_year < 1900) {
		VString year_string;
		for (const char *p = format ; *p != '\0' ; p++) {
			if (*p == '%') {
				p++;
				if (*p == 'y') {
					getYearString(year_string, 1);
					new_format_string += year_string;
				}
				else if (*p == 'Y') {
					getYearString(year_string, 0);
					new_format_string += year_string;
				}
				else {
					new_format_string += '%';
					new_format_string += *p;
				}
			}
			else {
				new_format_string += *p;
			}
		}
		format = new_format_string.gets();
	}
	char buffer[512];
	strftime(buffer, 511, format, &tm);
	s = buffer;
}

// --------------------------------------------------------------------------
// Return a VDate object representing the first day of the week containing
// the date represented by this VDate object.  ToDo: I think that the notion
// of the first day of the week is locale specific, e.g. doesn't a week
// begin on Monday in France?
//
VDate VDate::weekOf() const
{
	return weekOf(VDate::Sunday);
}

// --------------------------------------------------------------------------
// Return a VDate object representing the first day of the week containing
// the date represented by this VDate object.  The first day of the week
// is defined to be the given argument.
//
VDate VDate::weekOf(VDate::DayOfWeek first_day_of_week) const
{
	DayOfWeek this_day_of_week = getDayOfWeek();
	if (first_day_of_week == this_day_of_week) {
		return *this;
	}
	else {
		VDate d(*this);
		d.addDays((first_day_of_week - this_day_of_week) % 7);
		return d;
	}
}

// --------------------------------------------------------------------------
//
void VDate::getRomanNumeralString(VString& s, long number)
{
	static struct decimal_roman_map_struct {
		short	decimal;
		char   *roman;
	} decimal_roman_map[] = { { 1000, "M", }, {  900, "CM" },
							  {  500, "D"  }, {  400, "CD" },
							  {  100, "C"  }, {   90, "XC" },
							  {   50, "L"  }, {   40, "XL" },
							  {   10, "X"  }, {    9, "IX" },
							  {    5, "V"  }, {    4, "IV" },
							  {    1, "I"  }, {    0, ""   } };
	char buffer[256];
	buffer[0] = '\0';
	for (int n = number, i = 0; decimal_roman_map[i].decimal > 0 ; i++) {
		while (n >= decimal_roman_map[i].decimal) {
			strcat(buffer, decimal_roman_map[i].roman);
			n -= decimal_roman_map[i].decimal;
		}
	}
	s = buffer;
}

// --------------------------------------------------------------------------
// Add the specified number of days to the date represented by this VDate
// object; the given number of days may be negative.  
//
void VDate::addDays(int n)
{
	int days_per_year;

	dayOfYear += n;

	if (dayOfYear > MinDaysPerYear) {
		while (dayOfYear > (days_per_year = getDaysPerYear(theYear))) {
			theYear++;
			dayOfYear -= days_per_year;
		}
	}
	else while (dayOfYear <= 0) {
		theYear--;
		dayOfYear += (days_per_year = getDaysPerYear(theYear));
	}
}

// --------------------------------------------------------------------------
// Add the specified number of weeks to the date represented by this VDate
// object; the given number of weeks may be negative.  
//
void VDate::addWeeks(int n)
{
	addDays(n * DaysPerWeek);
}

// --------------------------------------------------------------------------
// Add the specified number of months to the date represented by this VDate
// object; the given number of months may be negative.  
//
void VDate::addMonths(int n)
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	dd.year += (n / MonthsPerYear);
	if ((n + dd.month) < FirstMonth) {
		dd.year--;
		dd.month = Month(MonthsPerYear - (dd.month + (n % MonthsPerYear)));
	}
	else if ((n + dd.month) > LastMonth) {
		dd.year++;
		dd.month = Month(MonthsPerYear - (dd.month + (n % MonthsPerYear)));
	}
	else {
		dd.month = Month(dd.month + (n % MonthsPerYear));
	}
	getDataFromYearMonthAndDay(dd);
	theYear = dd.year;
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Add the specified number of years to the date represented by this VDate
// object; the given number of years may be negative.  
//
void VDate::addYears(int n)
{
	VDateData dd;
	dd.year = theYear;
	dd.day_of_year = dayOfYear;
	getDataFromYearAndDay(dd);
	dd.year = Year(dd.year + n);
	getDataFromYearMonthAndDay(dd);
	theYear = dd.year;
	dayOfYear = dd.day_of_year;
}

// --------------------------------------------------------------------------
// Return a VDate object containing the date represented by this VDate object
// plus the specified number of days; the given number of days may be
// negative.  
//
VDate VDate::plusDays(int n) const
{
	VDate date(*this);
	date.addDays(n);
	return date;
}

// --------------------------------------------------------------------------
// Return a VDate object containing the date represented by this VDate object
// plus the specified number of weeks; the given number of weeks may be
// negative.  
//
VDate VDate::plusWeeks(int n) const
{
	VDate date(*this);
	date.addWeeks(n);
	return date;
}

// --------------------------------------------------------------------------
// Return a VDate object containing the date represented by this VDate object
// plus the specified number of months; the given number of months may be
// negative.  
//
VDate VDate::plusMonths(int n) const
{
	VDate date(*this);
	date.addMonths(n);
	return date;
}

// --------------------------------------------------------------------------
// Return a VDate object containing the date represented by this VDate object
// plus the specified number of years; the given number of years may be
// negative.  
//
VDate VDate::plusYears(int n) const
{
	VDate date(*this);
	date.addYears(n);
	return date;
}

// --------------------------------------------------------------------------
// Return the difference, in days, between this and the given VDate objects.
// If the date represented by this VDate object is before the date
// represented by the given VDate object, then the returned value will
// be positive.  If the date represented by this VDate object is after
// the date represented by the given VDate object, then the returned
// value will be negative.  If the two dates are equal then the
// returned value will be zero.
//
long VDate::operator-(const VDate& date) const
{
	long n;

	if (theYear == date.theYear) {
		return dayOfYear - date.dayOfYear;
	}
	else if (theYear > date.theYear) {
		if (IS_LEAP_YEAR(date.theYear)) {
			n = DaysPerLeapYear - date.dayOfYear;
		}
		else {
			n = DaysPerNonLeapYear - date.dayOfYear;
		}
		for (Year y = date.theYear + 1 ; y < theYear ; y++) {
			if (IS_LEAP_YEAR(y)) {
				n += DaysPerLeapYear;
			}
			else {
				n += DaysPerNonLeapYear;
			}
		}
		return n + dayOfYear;
	}
	else {
		if (IS_LEAP_YEAR(theYear)) {
			n = DaysPerLeapYear - dayOfYear;
		}
		else {
			n = DaysPerNonLeapYear - dayOfYear;
		}
		for (Year y = theYear + 1 ; y < date.theYear ; y++) {
			if (IS_LEAP_YEAR(y)) {
				n += DaysPerLeapYear;
			}
			else {
				n += DaysPerNonLeapYear;
			}
		}
		return -(n + dayOfYear);
	}
}

// --------------------------------------------------------------------------
// Fill in the date data for the given VDateData argument as follows:
//
// Input:	dd.year
//			dd.day_of_year
// Output:	dd.month
//			dd.day_of_month
//			dd.day_of_week
//			dd.days_in_year
//			dd.days_in_month
//
// N.B. Any input values which are out of range will simply
//      be slid into range with no error notification.
//
void VDate::getDataFromYearAndDay(VDateData& dd) const
{
	// Sanity checks; if the year or day-of-year values are out
	// of range, then simply slide them into range; no error.

	if (dd.year < 0) {
		dd.year = Year(0);
	}
	if (shortYearMappingEnabled) {
		dd.year = mapShortYear(dd.year);
	}
	else if (dd.year == 0) {
		dd.year = Year(1);
	}

	if (dd.day_of_year == 0) {
		dd.day_of_year = 1;
	}

	// Determine the day-of-week of the first day of the
	// year and whether or not the year is a leap-year.

	int	first_day_of_year, is_leap_year;

	first_day_of_year = getFirstDayOfWeekInYear(dd.year, &is_leap_year);

	// Determine the days-in-year.

	char  *ndays_in_month;
	short *ndays_before_month;

	if (is_leap_year) {
		ndays_in_month = daysPerLeapYearMonth;
		ndays_before_month = ndaysBeforeMonthInLeapYear;
		dd.days_in_year = DaysPerLeapYear;
	}
	else {
		ndays_in_month = daysPerMonth;
		ndays_before_month = ndaysBeforeMonth;
		dd.days_in_year = DaysPerNonLeapYear;
	}

	// Sanity checks; if the day-of-year is out of
	// range, then simply slide it into range; no error.

	if (dd.day_of_year > dd.days_in_year) {
		dd.day_of_year = dd.days_in_year;
	}

	// Determine the month, day-of-month, and days-in-month.

	dd.month = Month(dd.day_of_year / MinDaysPerMonth);
	if (dd.month >= MonthsPerYear) {
		dd.month = LastMonth;
	}
	else if (ndays_before_month[dd.month] < dd.day_of_year) {
		dd.month = Month(dd.month + 1);
	}
	dd.day_of_month = dd.day_of_year - ndays_before_month[dd.month - 1];
	dd.days_in_month = int(ndays_in_month[dd.month - 1]);


	// Deterimine the day-of-week from the day-of-year
	// and the day-of-week of the first day of the year.

	dd.day_of_week = DayOfWeek((dd.day_of_year +
								first_day_of_year - 1) % DaysPerWeek);

	if (dd.day_of_week == 0) {
		dd.day_of_week = DayOfWeek(DaysPerWeek);
	}
}

// --------------------------------------------------------------------------
// Fill in the date data for the given VDateData argument as follows:
//
// Input:	dd.year
//			dd.month
//			dd.day_of_month
// Output:	dd.day_of_year
//			dd.day_of_week
//			dd.days_in_year
//			dd.days_in_month
//
// N.B. Any input values which are out of range will simply
//      be slid into range with no error notification.
//
void VDate::getDataFromYearMonthAndDay(VDateData& dd) const
{
	// Sanity checks; if the year or month values are out
	// of range, then simply slide them into range; no error.

	if (dd.year < 0) {
		dd.year = Year(0);
	}
	if (shortYearMappingEnabled) {
		dd.year = mapShortYear(dd.year);
	}
	else if (dd.year == 0) {
		dd.year = Year(1);
	}

	if (dd.month < FirstMonth) {
		dd.month = FirstMonth;
	}
	else if (dd.month > LastMonth) {
		dd.month = LastMonth;
	}
	if (dd.day_of_month < 1) {
		dd.day_of_month = 1;
	}

	// Determine the day-of-week of the first day of the
	// year and whether or not the year is a leap-year.

	int	first_day_of_year, is_leap_year;

	first_day_of_year = getFirstDayOfWeekInYear(dd.year, &is_leap_year);

	// Determine the days-in-year and days-in-month
	// from the month and leap-year flag.

	char  *ndays_in_month;
	short *ndays_before_month;

	if (is_leap_year) {
		ndays_in_month = daysPerLeapYearMonth;
		ndays_before_month = ndaysBeforeMonthInLeapYear;
		dd.days_in_year = DaysPerLeapYear;
	}
	else {
		ndays_in_month = daysPerMonth;
		ndays_before_month = ndaysBeforeMonth;
		dd.days_in_year = DaysPerNonLeapYear;
	}

	dd.days_in_month = DayOfMonth(ndays_in_month[dd.month - 1]);

	// Sanity check; if the day-of-month is out of
	// range, then simply slide it into range; no error.

	if (dd.day_of_month > ndays_in_month[dd.month - 1]) {
		dd.day_of_month = ndays_in_month[dd.month - 1];
	}

	// Determine the day-of-year.

	dd.day_of_year = ndays_before_month[dd.month - 1] + dd.day_of_month;

	// Deterimine the day-of-week from the day-of-year
	// and the day-of-week of the first day of the year.

	dd.day_of_week = DayOfWeek((dd.day_of_year +
							    first_day_of_year - 1) % DaysPerWeek);

	if (dd.day_of_week == 0) {
		dd.day_of_week = DayOfWeek(DaysPerWeek);
	}
}

// --------------------------------------------------------------------------
// Return the day-of-week of the first day (January 1) of the given year.
// In addition, if desired return whether or not thie givevn given is a
// leap-year.
//
// N.B. Any input values which are out of range will simply
//      be slid into range with no error notification.
//
VDate::DayOfWeek VDate::getFirstDayOfWeekInYear
						(VDate::Year year, int *is_leap_year) const
{
	if (year < 0) {
		year = Year(0);
	}
	if (shortYearMappingEnabled) {
		year = mapShortYear(year);
	}
	else if (year == 0) {
		year = Year(1);
	}

	// Determine the day of the week of the first
	// day (January 1st), of the century the given year.
	// Note that 400-year leap-years always begin on a Saturday.

	int	first_day_of_century;
	int is_century_leap_year;
	long century_year = (year / 100) * 100;

	switch (century_year % 400) {
	case 0:
		first_day_of_century = Saturday;
		is_century_leap_year = 1;
		break;
	case 100:
		first_day_of_century = Friday;
		is_century_leap_year = 0;
		break;
	case 200:
		first_day_of_century = Wednesday;
		is_century_leap_year = 0;
		break;
	case 300:
		first_day_of_century = Monday;
		is_century_leap_year = 0;
		break;
	}

	// Set the first day-of-week of the year.

	int	first_day_of_year, nyears, nleaps;

	nyears = year - century_year;
	nleaps	= nyears / 4;

	if (is_century_leap_year) {
		nleaps++;
	}

	if (IS_LEAP_YEAR(year)) {
		if (is_leap_year != 0) {
			*is_leap_year = 1;
		}
		nleaps--;
	}
	else if (is_leap_year != 0) {
		*is_leap_year = 0;
	}

	first_day_of_year = ((first_day_of_century +
						  nyears + nleaps) % DaysPerWeek);

	if (first_day_of_year == 0) {
		first_day_of_year = DaysPerWeek;
	}

	return DayOfWeek(first_day_of_year);
}

// --------------------------------------------------------------------------
// Return the Julian day for this VDate object.  A Julian day is defined as
// the number of days since January 1, 4713 BC, the most recent time that
// three major chronological cycles began on the same day -- (1) the 28-year
// solar cycle; (2) the 19-year lunar cycle; and (3) the 15-year indiction
// cycle (used in ancient Rome to regulate taxes).  It will take 7980 years
// to complete the period (28 * 19 * 15).  The Julian day was devised in 1582
// by Joseph Scaliger and named after his father Julius Caesar Scaliger (not
// after the Julian calendar, which preceeded the Gregorian calendar).  Just
// abouta any World Almanac has this information.
//
// This algorithm (and its inverse in getYearMonthAndDayFromJulianDay())
// to convert a Gregorian calendar date to a Julian day was authored
// by Robert G. Tantzen, Holloman Air Force Base, New Mexico, and was
// published in the Communications of the ACM, Volume 6, Number 8,
// August 1963 (Algorithm 199, page 444).
//
void VDate::getJulianDayFromYearMonthAndDay(long y, long m, long d, long& j)
{
	long c, ya;
	if (m > 2) {
		m -= 3;
	}
	else {
		m += 9;
		y -= 1;
	}
	c = y / 100;
	ya = y - 100 * c;
	j = (146097 * c) / 4 + (1461 * ya) / 4 + (153 * m + 2) / 5 + d + 1721119;

}

// --------------------------------------------------------------------------
// Return in (y,m,d), the Gregorian calendar date corresponding to the given
// Julian day.  See getJulianDayFromYearMonthAndDay() for information on the
// Julian day and this algorithm.
//
void VDate::getYearMonthAndDayFromJulianDay(long j, long& y, long& m, long& d)
{
	j = j - 1721119;
	y = (4 * j - 1) / 146097;
	j = 4 * j - 1 - 146097 * y;
	d = j / 4;
	j = (4 * d + 3) / 1461;
	d = 4 * d + 3 - 1461 * j;
	d = (d + 4) / 4;
	m = (5 * d - 3) / 153;
	d = 5 * d - 3 - 153 * m;
	d = (d + 5) / 5;
	y = 100 * y + j;
	if (m < 10) {
		m += 3;
	}
	else {
		m -= 9;
		y += 1;
	}
}

// --------------------------------------------------------------------------
// If desired (enable/disableShortYearMapping()), the years 0 thru 99 may be
// made to map automatically to the 100 years surrounding the current year,
// i.e. to the 50 years after the current year, the current year, or the 49
// years before the current year.  This short year mapping year may also be
// set and queried via setShortYearMappingYear() and getShortYearMappingYear().
// By default, this short year mapping functionality is *disabled*.
//
boolean		VDate::shortYearMappingEnabled	= FALSE;
VDate::Year	VDate::rollOverYear				= 0;
VDate::Year	VDate::rollOverCentury			= 0;

// --------------------------------------------------------------------------
//
VDate::Year VDate::mapShortYear(VDate::Year year)
{
	if ((year >= 0) && (year <= 99)) {
		if (rollOverCentury == 0) {
			calculateShortYearRollOverData();
		}
		if (year < rollOverYear) {
			return year + rollOverCentury + 100;
		}
		else {
			return year + rollOverCentury;
		}
	}
	else {
		return year;
	}
}

// --------------------------------------------------------------------------
//
void VDate::enableShortYearMapping(boolean b)
{
	shortYearMappingEnabled = b;
}

// --------------------------------------------------------------------------
//
void VDate::disableShortYearMapping(boolean b)
{
	shortYearMappingEnabled = !b;
}

// --------------------------------------------------------------------------
//
boolean VDate::isShortYearMappingEnabled()
{
	return shortYearMappingEnabled;
}

// --------------------------------------------------------------------------
//
boolean VDate::isShortYearMappingDisabled()
{
	return !shortYearMappingEnabled;
}

// --------------------------------------------------------------------------
//
void VDate::setShortYearMappingYear(VDate::Year year)
{
	calculateShortYearRollOverData(year);
}

// --------------------------------------------------------------------------
//
VDate::Year VDate::getShortYearMappingYear()
{
	if (rollOverCentury == 0) {
		calculateShortYearRollOverData();
	}
	return rollOverCentury + rollOverYear + 49;
}

// --------------------------------------------------------------------------
//
void VDate::calculateShortYearRollOverData(VDate::Year year)
{
	if (year < 100) {
		VDate date;
		date.addYears(-49);
		year = date.getYear();
	}
	rollOverCentury = (year / 100) * 100;
	rollOverYear = year % 100;
}
