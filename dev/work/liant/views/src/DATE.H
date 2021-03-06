// $Header:   Q:/views/common/vcs/DATE.H_V   1.12   Mar 24 1997 16:08:40   scottt  $

//	date.h
//
//	VDate interface [Common]
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
//	01/03/96 dgm	Fixed operator<().
//	01/31/96 dgm	Removed static functions: getYearString(),
//					getMonthString(), getDayOfWeekString(),
//					and getRomanYearString().
// ---------------------------------------------------------------------------

#ifndef VDATE_H
#define VDATE_H

// --------------------------------------------------------------------------
//
#include "defs.h"

// --------------------------------------------------------------------------
//
CLASS	VDate;
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
// The VDate class may be used to represent calendar dates.
// It follows the traditional Gregorian calendar which officially began
// on Friday, October 15, 1582 under the order of Pope Gregory XIII; the
// previous day, Thursday, October 4, 1582, was the last day of the Julian
// calendar.  Although dates all the way back to the year 1 may be legally
// represented by VDate, they will not be valid before October 15, 1582.
// Note also that the Gregorian calendar was not adopted in England until
// Thursday, September 14, 1752, so again VDate values before this date
// will not be valid if you, as many do, consider this to be the real
// beginning of the Gregorian.  The year 0 is not valid.
//
// A VDate object may be constructed with some combination of year, month,
// day-of-month, day-of-year, and/or day-of-week.  Thereafter, years,
// months, weeks, and/or days may be added or subtracted to/from the date.
// The pertinent values of the date may be queried at any during the object's
// lifetime.  Any erroneous values passed in to any member-functions are
// modified to be within the proper range without any error notification.
//
CLASS VDate {

public:

	typedef short		Year;
	typedef short		DayOfYear;
	typedef char		DayOfMonth;

	enum				DayOfWeek
	{
						NullDayOfWeek	= 0,
						Sunday			= 1,
						Monday			= 2,
						Tuesday			= 3,
						Wednesday		= 4,
						Thursday		= 5,
						Friday			= 6,
						Saturday		= 7,
						FirstDayOfWeek	= Sunday,
						LastDayOfWeek	= Saturday
	};
	enum				Month
	{
						NullMonth		=  0,
						January			=  1,
						February		=  2,
						March			=  3,
						April			=  4,
						May				=  5,
						June			=  6,
						July			=  7,
						August			=  8,
						September		=  9,
						October			= 10,
						November		= 11,
						December		= 12,
						FirstMonth		= January,
						LastMonth		= December
	};
	enum				Constants
	{
						DaysPerWeek			=   7,
						MinDaysPerMonth		=  28,
						MaxDaysPerMonth		=  31,
						MonthsPerYear		=  12,
						DaysPerNonLeapYear	= 365,
						DaysPerLeapYear		= 366,
						MinDaysPerYear		= 365,
						MaxDaysPerYear		= 366,
						NullDayOfMonth		=   0,
						NullDayOfYear		=   0
	};

public:
						VDate();
						VDate(boolean set_current);
						VDate(Year,
							  DayOfYear = 1);
						VDate(Year,
							  Month,
							  DayOfMonth = 1);
						VDate(Year,
							  Month,
							  DayOfWeek,
							  int nth_day_of_week_within_month = 1);
						VDate(Month,
							  DayOfMonth = 1);
						VDate(const struct tm *);
						VDate(const VDate&);

	void				set();
	void				set(VDate::Year, VDate::DayOfYear = 1);
	void				set(VDate::Year, VDate::Month,
							VDate::DayOfMonth = 1);
	void				set(VDate::Year, VDate::Month, VDate::DayOfWeek,
							int nth_day_of_week_within_month = 1);
	void				set(VDate::Month, VDate::DayOfMonth = 1);
	void				set(const struct tm *);
	void				set(const VDate&);
	void				setNull();

	VDate&				operator=(const VDate&);

	void				setYear(VDate::Year);
	void				setMonth(VDate::Month);
	void				setDayOfWeek(VDate::DayOfWeek);
	void				setDayOfMonth(VDate::DayOfMonth);
	void				setDayOfYear(VDate::DayOfYear);
	void				setJulianDay(long);

	VDate::Year			getYear() const;
	VDate::Month		getMonth() const;
	VDate::DayOfMonth	getDayOfMonth() const;
	VDate::DayOfWeek	getDayOfWeek() const;
	VDate::DayOfYear	getDayOfYear() const;
	void				getTm(struct tm *) const;
	long				getJulianDay() const;

	int					getDaysPerYear() const;
	int					getDaysPerMonth() const;
	boolean				isLeapYear() const;

	void				getDayOfWeekString(VString&, boolean = FALSE);
	void				getMonthString(VString&, boolean = FALSE);
	void				getYearString(VString&, boolean = FALSE);
	void				getRomanYearString(VString&);
	void				getString(VString&, const char *format = 0);

	VDate				weekOf() const;
	VDate				weekOf(VDate::DayOfWeek) const;

	void				addDays(int);
	void				addWeeks(int);
	void				addMonths(int);
	void				addYears(int);

	VDate				plusYears(int) const;
	VDate				plusMonths(int) const;
	VDate				plusWeeks(int) const;
	VDate				plusDays(int) const;

	VDate				operator+(int) const;
	VDate&				operator++();
	VDate				operator++(int);
	VDate&				operator+=(int);
	VDate				operator-(int) const;
	VDate&				operator--();
	VDate				operator--(int);
	VDate&				operator-=(int);
	long				operator-(const VDate&) const;

	boolean				operator==(const VDate&) const;
	boolean				operator!=(const VDate&) const;
	boolean				operator<(const VDate&) const;
	boolean				operator>(const VDate&) const;
	boolean				operator<=(const VDate&) const;
	boolean				operator>=(const VDate&) const;
	boolean				between(const VDate&, const VDate&) const;
	VDate&				max(const VDate&) const;
	VDate&				min(const VDate&) const;

	boolean				isValid() const;
	boolean				isNull() const;

	// Public static member functions.

	static int			getDaysPerYear(VDate::Year);
	static int			getDaysPerMonth(VDate::Month,
										VDate::Year = VDate::Year(1));
	static boolean		isLeapYear(VDate::Year);
	static void			getJulianDayFromYearMonthAndDay(long y,
													    long m,
													    long d,
													    long& j);
	static void			getYearMonthAndDayFromJulianDay(long j,
														long &y,
														long &m,
														long &d);
	static void			getRomanNumeralString(VString&, long);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	struct	VDateData {
			Year		year;
			Month		month;
			DayOfMonth	day_of_month;
			DayOfWeek	day_of_week;
			DayOfYear	day_of_year;
			DayOfMonth	days_in_month;
			DayOfYear	days_in_year;
	};

protected:
	void				initialize();
	void				getDataFromYearMonthAndDay(VDate::VDateData&) const;
	void				getDataFromYearAndDay(VDate::VDateData&) const;
	VDate::DayOfWeek	getFirstDayOfWeekInYear(Year, int * = 0) const;
	void				setStartOfGregorian();

private:
	VDate::Year			theYear;
	VDate::DayOfYear	dayOfYear;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//
public:
	static void			enableShortYearMapping(boolean = TRUE);
	static void			disableShortYearMapping(boolean = TRUE);
	static boolean		isShortYearMappingEnabled();
	static boolean		isShortYearMappingDisabled();
	static void			setShortYearMappingYear(Year);
	static Year			getShortYearMappingYear();
	static Year			mapShortYear(Year);

protected:
	static void			calculateShortYearRollOverData(Year = 0);

private:
	static boolean		shortYearMappingEnabled;
	static Year			rollOverYear;
	static Year			rollOverCentury;

};

// --------------------------------------------------------------------------
// Default constructor.
// Construct this VDate object to represent the current date.
//
inline VDate::VDate()
{
	set();
}

// --------------------------------------------------------------------------
// Copy constructor.
//
inline VDate::VDate(const VDate& date)
{
	theYear = date.theYear;
	dayOfYear = date.dayOfYear;
}

// --------------------------------------------------------------------------
// Set this VDate object to the value of the given VDate object.
//
inline void VDate::set(const VDate& date)
{
	theYear = date.theYear;
	dayOfYear = date.dayOfYear;
}

// --------------------------------------------------------------------------
//
inline void VDate::setNull()
{
	theYear = 0;
	dayOfYear = 1;
}

// --------------------------------------------------------------------------
// Assignment operator.
//
inline VDate& VDate::operator=(const VDate& date)
{
	theYear = date.theYear;
	dayOfYear = date.dayOfYear;
	return *this;
}

// --------------------------------------------------------------------------
// Return a VDate object which represents this VDate object plus the given
// number of days.
//
inline VDate VDate::operator+(int n) const
{
	return plusDays(n);
}

// --------------------------------------------------------------------------
// Prefix increment operator.  Add one day to the date represented by this
// VDate object.  Return a reference to this VDate object.
//
inline VDate& VDate::operator++()
{
	addDays(1);
	return *this;
}

// --------------------------------------------------------------------------
// Postfix increment operator.  Add one day to the date represented by this
// VDate object.  Return a reference to this VDate object.
//
inline VDate VDate::operator++(int)
{
	VDate date(*this);
	addDays(1);
	return date;
}

// --------------------------------------------------------------------------
// Add the given number of days to the date represented by this VDate object.
// Return a reference to this VDate object.
//
inline VDate& VDate::operator+=(int n)
{
	addDays(n);
	return *this;
}

// --------------------------------------------------------------------------
// Return a VDate object which represents this VDate object minus the given
// number of days.
//
inline VDate VDate::operator-(int n) const
{
	return plusDays(-n);
}

// --------------------------------------------------------------------------
// Prefix decrement operator.  Subtract one day from the date represented by
// this VDate object.  Return a reference to this VDate object.
//
inline VDate& VDate::operator--()
{
	addDays(-1);
	return *this;
}

// --------------------------------------------------------------------------
// Postfix decrement operator.  Subtract one day from the date represented by
// this VDate object.  Return a reference to this VDate object.
//
inline VDate VDate::operator--(int)
{
	VDate date(*this);
	addDays(-1);
	return date;
}

// --------------------------------------------------------------------------
// Subtract the given number of days from the date represented by this VDate
// object.  Return a reference to this VDate object.
//
inline VDate& VDate::operator-=(int n)
{
	addDays(-n);
	return *this;
}

// --------------------------------------------------------------------------
// Return TRUE if the dates represented by this and the given VDate objects
// are equal, otherwise return FALSE.
//
inline boolean VDate::operator==(const VDate& date) const
{
	return (theYear == date.theYear) && (dayOfYear == date.dayOfYear);
}

// --------------------------------------------------------------------------
// Return TRUE if the dates represented by this and the given VDate objects
// are not equal, otherwise return FALSE.
//
inline boolean VDate::operator!=(const VDate& date) const
{
	return (theYear != date.theYear) || (dayOfYear != date.dayOfYear);
}

// --------------------------------------------------------------------------
// Return TRUE, if the date represented by this VDate object is before the
// date represented by the given VDate object, otherwise return FALSE.
//
inline boolean VDate::operator<(const VDate& date) const
{
	return (theYear < date.theYear) ||
		   ((theYear == date.theYear) && (dayOfYear < date.dayOfYear));
}

// --------------------------------------------------------------------------
// Return TRUE, if the date represented by this VDate object is after the
// date represented by the given VDate object, otherwise return FALSE.
//
inline boolean VDate::operator>(const VDate& date) const
{
	return (theYear > date.theYear) ||
		   ((theYear == date.theYear) && (dayOfYear > date.dayOfYear));
}

// --------------------------------------------------------------------------
// Return TRUE, if the date represented by this VDate object is before or is
// equal to the date represented by the given VDate object, otherwise return
// FALSE.
//
inline boolean VDate::operator<=(const VDate& date) const
{
	return ((theYear == date.theYear) && (dayOfYear == date.dayOfYear)) ||
			(theYear < date.theYear) || (dayOfYear < date.dayOfYear);
}

// --------------------------------------------------------------------------
// Return TRUE, if the date represented by this VDate object is equal to or
// is after the date represented by the given VDate object, otherwise return
// FALSE.
//
inline boolean VDate::operator>=(const VDate& date) const
{
	return ((theYear == date.theYear) && (dayOfYear == date.dayOfYear)) ||
			((theYear > date.theYear) ||
		     ((theYear == date.theYear) && (dayOfYear > date.dayOfYear)));
}

// --------------------------------------------------------------------------
//
inline boolean VDate::between(const VDate& a, const VDate& b) const
{
	return (*this >= a) && (*this <= b);
}

// --------------------------------------------------------------------------
//
inline VDate& VDate::max(const VDate& date) const
{
	if (*this >= date) {
		return (VDate&)*this;
	}
	else {
		return (VDate&)date;
	}
}

// --------------------------------------------------------------------------
//
inline VDate& VDate::min(const VDate& date) const
{
	if (*this <= date) {
		return (VDate&)*this;
	}
	else {
		return (VDate&)date;
	}
}

// --------------------------------------------------------------------------
// Return TRUE if this VDate object represents a valid date, otherwise FALSE.
//
inline boolean VDate::isValid() const
{
	return (theYear >= 0) &&
		   (dayOfYear >= 1) &&
		   (dayOfYear <= getDaysPerYear(theYear));
}

// --------------------------------------------------------------------------
//
inline boolean VDate::isNull() const
{
	return theYear == 0;
}

// --------------------------------------------------------------------------
//
inline void VDate::initialize()
{
	theYear = 0;
	dayOfYear = 1;
}

#endif // VDATE_H
