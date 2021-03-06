// $Header:   Q:/views/common/vcs/DATETIME.H_V   1.10   Mar 24 1997 16:08:42   scottt  $

//	datetime.h
//
//	VDateTime interface [Common]
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
//	07/06/95 dgm	Befriended VTimeZone.
//	08/30/95 dgm	Unsigned to signed changes.
// ---------------------------------------------------------------------------

#ifndef VDATETIME_H
#define VDATETIME_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include "date.h"
#include "thyme.h"
#include "timezone.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VDate;
CLASS	VObject;
CLASS	VString;
CLASS	VTime;
CLASS	VTimeZone;
struct	tm;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VDateTime : public VObject {

public:
						VDateTime();
						VDateTime(boolean set_current);
						VDateTime(const VTimeZone&);
						VDateTime(const VDate&);
						VDateTime(const VTime&);
						VDateTime(const VDate&, const VTime&);
						VDateTime(const VDate&, const VTime&,
								  const VTimeZone&);
						VDateTime(const struct tm *);
						VDateTime(const struct tm *, const VTimeZone&);
						VDateTime(const VDateTime&);
						VDateTime(const VDateTime&, const VTimeZone&);
	VClass			   *iam();

	void				set();
	void				set(const VDate&);
	void				set(const VTime&);
	void				set(const VDate&, const VTime&);
	void				set(const VDate&, const VTime&, const VTimeZone&);
	void				set(const struct tm *);
	void				set(const struct tm *, const VTimeZone&);
	void				set(const VDateTime&);
	void				set(const VDateTime&, const VTimeZone&);
	void				setNull();

	VDateTime&			operator=(const VDateTime&);

	void				setDate();
	void				setDate(const VDate&);
	void				setDate(VDate::Year, VDate::DayOfYear = 1);
	void				setDate(VDate::Year, VDate::Month,
								VDate::DayOfMonth = 1);
	void				setDate(VDate::Month, VDate::DayOfMonth = 1);
	void				setYear(VDate::Year);
	void				setMonth(VDate::Month);
	void				setDayOfWeek(VDate::DayOfWeek);
	void				setDayOfMonth(VDate::DayOfMonth);
	void				setDayOfYear(VDate::DayOfYear);

	void				setTime();
	void				setTime(const VTime&);
	void				setTime(int hour,
								int minute = 0,
								int second = 0);
	void				setHour(int);
	void				setMinute(int);
	void				setSecond(int);

	void				setTimeZone(const VTimeZone&);
	void				setTimeZone();
	const VTimeZone&	getTimeZone() const;

	VDate::Year			getYear() const;
	VDate::Month		getMonth() const;
	VDate::DayOfMonth	getDayOfMonth() const;
	VDate::DayOfWeek	getDayOfWeek() const;
	VDate::DayOfYear	getDayOfYear() const;
	int					getDaysPerYear() const;
	int					getDaysPerMonth() const;
	boolean				isLeapYear() const;
	long				getJulianDay() const;
	int					getHour() const;
	int					getMinute() const;
	int					getSecond() const;
	void				getTm(struct tm *, boolean getdst = TRUE) const;
	boolean				isDST() const;

	void				getYearString(VString&, boolean = FALSE);
	void				getRomanYearString(VString&);
	void				getMonthString(VString&, boolean = FALSE);
	void				getDayOfWeekString(VString&, boolean = FALSE);
	void				getHourString(VString&, boolean = FALSE);
	void				getMinuteString(VString&, boolean = FALSE);
	void				getSecondString(VString&, boolean = FALSE);
	void				getTimeZoneString(VString&, boolean = TRUE);
	void				getString(VString&, const char *format = 0);

	void				addYears(int);
	void				addMonths(int);
	void				addWeeks(int);
	void				addDays(int);
	void				addHours(int);
	void				addMinutes(int);
	void				addSeconds(int);

	VDateTime			plusYears(int) const;
	VDateTime			plusMonths(int) const;
	VDateTime			plusWeeks(int) const;
	VDateTime			plusDays(int) const;
	VDateTime			plusHours(int) const;
	VDateTime			plusMinutes(int) const;
	VDateTime			plusSeconds(int) const;

	long				operator-(const VDateTime&) const;

						operator VDate&();
						operator VTime&();

	boolean				operator==(const VDateTime&) const;
	boolean				operator!=(const VDateTime&) const;
	boolean				operator<(const VDateTime&) const;
	boolean				operator>(const VDateTime&) const;
	boolean				operator<=(const VDateTime&) const;
	boolean				operator>=(const VDateTime&) const;
	boolean				between(const VDateTime&, const VDateTime&) const;
	VDateTime&			max(const VDateTime&) const;
	VDateTime&			min(const VDateTime&) const;

	boolean				isValid() const;
	boolean				isNull() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	virtual boolean		isEqual(VObject *) const;
	virtual HashKey		hash() const;
	virtual int 		compare(VObject *) const;

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver&);
	virtual void		getFrom(VArchiver&);
	virtual void 		getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// N.B. the VDateTime object does not own the VTimeZone object,
	// and is not responsible for deleting it, and copying VDateTime
	// objects using this method or the assignment operator will result
	// in the VTimeZone pointer being copied; it is the responsibility
	// of the caller to manage this correctly.

protected:
	VTime				theTime;
	VDate				theDate;
	const VTimeZone	   *theZone;

private:
	friend				VTimeZone;

};

extern VClass *VDateTimeCls;

// --------------------------------------------------------------------------
// Default constructor.
// Construct this VDateTime object to represent
// the current date and time of day.
//
inline VDateTime::VDateTime()
	: theDate(FALSE), theTime(FALSE)
{
	set();
}

// --------------------------------------------------------------------------
// Copy constructor.
//
inline VDateTime::VDateTime(const VDateTime& datetime)
	: theDate(datetime.theDate), theTime(datetime.theTime)
{
	theZone = datetime.theZone;
}

// --------------------------------------------------------------------------
// Construct this VDateTime object to contain the given date and time
// translated to the given time zone.
//
inline VDateTime::VDateTime(const VDateTime& datetime,
							const VTimeZone& timezone)
	: theDate(datetime.theDate), theTime(datetime.theTime)
{
	theZone = datetime.theZone;
	setTimeZone(timezone);
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the date and time of day specified
// in the given VDateTime object.
//
inline void VDateTime::set(const VDateTime& datetime)
{
	theDate.set(datetime.theDate);
	theTime.set(datetime.theTime);
	theZone = datetime.theZone;
}

// --------------------------------------------------------------------------
// Set this VDateTime object to contain the date and time of day specified
// in the given VDateTime object, translated to the given time zone.
//
inline void VDateTime::set(const VDateTime& datetime,
						   const VTimeZone& timezone)
{
	set(datetime);
	setTimeZone(timezone);
}

// --------------------------------------------------------------------------
//
inline void VDateTime::setNull()
{
	theDate.setNull();
	theTime.setNull();
}

// --------------------------------------------------------------------------
// Assignment operator.
//
inline VDateTime& VDateTime::operator=(const VDateTime& datetime)
{
	set(datetime);
	return *this;
}

// --------------------------------------------------------------------------
//
inline VDateTime::operator VDate&()
{
	return theDate;
}

// --------------------------------------------------------------------------
//
inline VDateTime::operator VTime&()
{
	return theTime;
}

// --------------------------------------------------------------------------
//
inline boolean VDateTime::between(const VDateTime& a,
								  const VDateTime& b) const
{
	return (*this >= a) && (*this <= b);
}

// --------------------------------------------------------------------------
//
inline VDateTime& VDateTime::max(const VDateTime& datetime) const
{
	if (*this >= datetime) {
		return (VDateTime&)*this;
	}
	else {
		return (VDateTime&)datetime;
	}
}

// --------------------------------------------------------------------------
//
inline VDateTime& VDateTime::min(const VDateTime& datetime) const
{
	if (*this <= datetime) {
		return (VDateTime&)*this;
	}
	else {
		return (VDateTime&)datetime;
	}
}

// --------------------------------------------------------------------------
//
inline boolean VDateTime::isValid() const
{
	return (theDate.isValid() && theTime.isValid());
}

// --------------------------------------------------------------------------
//
inline boolean VDateTime::isNull() const
{
	return theDate.isNull() && theTime.isNull();
}

#endif // VDATETIME_H
