// $Header:   Q:/views/common/vcs/TIMEZONE.CPV   1.7   Mar 24 1997 16:13:44   scottt  $

//	timezone.cpp
//
//	VTimeZone implementation [Common]
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
//	07/06/95 dgm	Fix in isDST().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "timezone.h"
#include "datetime.h"
#include "ordcllct.h"
#include "iterator.h"
#include "str.h"
#include <time.h>

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

// ==========================================================================
// VDST interface
//
CLASS VDST : public VObject {
public:
					VDST();
private:
					VDST(const VDST&);
					VDST(int          begin_year,
						 int          end_year,
						 int          begin_month,
						 int          begin_day_of_month,
				  		 const VTime& begin_time,
						 int          end_month,
						 int          end_day_of_month,
				  		 const VTime& end_time);
					VDST(int          begin_year,
						 int          end_year,
						 int          begin_month,
						 int          begin_day_of_week,
						 int          begin_nth_day_of_week,
				  		 const VTime& begin_time,
						 int          end_month,
						 int          end_day_of_week,
						 int          end_nth_day_of_week,
				  		 const VTime& end_time);

	void			getBeginDateTime(int year, VDateTime&) const;
	void			getEndDateTime(int year, VDateTime&) const;

#ifndef CV_NOARCHIVER
	virtual void	putTo(VArchiver&);
	virtual void	getFrom(VArchiver&);
	virtual void	getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif

	VClass		   *iam();

private:
	VTime			beginTime,         endTime;
	short			beginYear,         endYear;
	char			beginMonth,        endMonth;
	char			beginDay,          endDay;
	char			beginNthDayOfWeek, endNthDayOfWeek;
	char			usingNthDayOfWeek   : 1;
	char			dstEndIsBeforeBegin : 1;

private:
	friend			VTimeZone;
};

extern VClass *VDSTCls;

// ==========================================================================
// VTimeZone implementation

// --------------------------------------------------------------------------
//
defineClass(VTimeZone, VObject)

// --------------------------------------------------------------------------
//
static VTimeZone *localTimeZone	= 0;
static VTimeZone *gmtTimeZone	= 0;

// --------------------------------------------------------------------------
// This apparent nonsense is to ensure that the static local and GMT
// VTimeZone objects, if created, are deleted on program termination.
// The reason we don't use global objects is simply so to avoid the
// restriction that the user may not delete object returned by
// VTimeZone::local() and VTimeZone::gmt(), and also so that
// these objects aren't created unless they're asked for.
//
struct VDummy { ~VDummy() { delete localTimeZone; delete gmtTimeZone; } };
static VDummy dummy;

// --------------------------------------------------------------------------
//
VTimeZone::VTimeZone(int code, VOrdCollect *dst)
{
	construct(code, dst);
}

// --------------------------------------------------------------------------
//
VTimeZone::~VTimeZone()
{
	if (daylightSavingsTime != 0) {
		daylightSavingsTime->freeContents();
		delete daylightSavingsTime;
	}
	//
	// Note that this allows for careless users deleting the objects
	// returned by VTimeZone::local() and VTimeZone::gmt().
	//
	if (this == localTimeZone) {
		localTimeZone = 0;
	}
	if (this == gmtTimeZone) {
		gmtTimeZone = 0;
	}
}

// --------------------------------------------------------------------------
//
void VTimeZone::construct(int code, VOrdCollect *dst)
{
	if (code == VTimeZone::NoTZ) {
		daylightSavingsTime = 0;
		minutesFromGMT = 0;
		isLocal = 0;
		return;
	}
	else if (code == VTimeZone::Local) {
		minutesFromGMT = determineLocalMinutesFromGMT();
		isLocal = 1;
	}
	else if ((code >= VTimeZone::TZ00) && (code <= VTimeZone::TZ24)) {
		minutesFromGMT = code;
		isLocal = 0;
	}
	else {
		minutesFromGMT = 0;
		isLocal = 0;
	}
	if (dst != 0) {
		daylightSavingsTime = copyDST(dst);
	}
	else {
		daylightSavingsTime = 0;
		setupDST();
	}
}

// --------------------------------------------------------------------------
//
void VTimeZone::getName(VString& s, boolean dst, boolean abbreviated) const
{
	int minutes_from_gmt = getMinutesFromGMT(dst);
	int hours_from_gmt = minutes_from_gmt / 60;
	char sign;

	if (minutes_from_gmt < 0) {
		sign = '-';
		hours_from_gmt = -hours_from_gmt;
	}
	else if (minutes_from_gmt > 0) {
		sign = '+';
	}
	else {
		if (abbreviated) {
			s.prints("GMT", hours_from_gmt);
		}
		else {
			s.prints("Greenwich Mean Time", hours_from_gmt);
		}
		return;
	}
	s.prints("GMT%c%02d:%02d", sign, hours_from_gmt, minutes_from_gmt % 60);
}

// --------------------------------------------------------------------------
//
int VTimeZone::getMinutesFromGMT(boolean dst) const
{
	if (dst) {
		return minutesFromGMT + 60;
	}
	else {
		return minutesFromGMT;
	}
}

// --------------------------------------------------------------------------
//
boolean VTimeZone::hasDST() const
{
	return daylightSavingsTime != 0;
}

// --------------------------------------------------------------------------
//
boolean VTimeZone::isDST(const VDateTime& datetime) const
{
	if (daylightSavingsTime == 0) {
		//
		// Since there is no Daylight Savings Time data in this VTimeZone
		// object, we'll try to determine this information from the system,
		// but only if this VTimeZone represents the local time zone.
		//
		if (isLocal) {
			struct tm tm;
			datetime.getTm(&tm, FALSE);
			tm.tm_isdst = -1;
			time_t t = mktime(&tm);
			return tm.tm_isdst;
		}
		else {
			return FALSE;
		}
	}

	VDate::Year year = datetime.getYear();
	VDateTime dst(FALSE);

	const VTimeZone *tz = datetime.theZone;
	((VDateTime&)datetime).theZone = 0;

	DO (*daylightSavingsTime, VDST, d)
		if (d->dstEndIsBeforeBegin) {
			if (year < d->endYear) {
				if (year >= d->beginYear) {
					d->getBeginDateTime(year, dst);
					if (datetime >= dst) {
						d->getEndDateTime(year, dst);
						if (datetime <= dst) {
							((VDateTime&)datetime).theZone = tz;
							return TRUE;
						}
					}
				}
			}
			else if (year == d->endYear) {
				if (year >= d->beginYear) {
					d->getBeginDateTime(year, dst);
					if (datetime >= dst) {
						DO (*daylightSavingsTime, VDST, dd)
							if (year >= d->beginYear) {
								d->getEndDateTime(year + 1, dst);
								if (datetime <= dst) {
									((VDateTime&)datetime).theZone = tz;
									return TRUE;
								}
							}
						END
					}
				}
			}
		}
		else if ((year >= d->beginYear) && (year <= d->endYear)) {
			d->getBeginDateTime(year, dst);
			if (datetime >= dst) {
				d->getEndDateTime(year, dst);
				if (datetime <= dst) {
					((VDateTime&)datetime).theZone = tz;
					return TRUE;
				}
			}
		}
	END

	((VDateTime&)datetime).theZone = tz;

	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VTimeZone::getBeginDST(VDate::Year year, VDateTime& datetime) const
{
	if (daylightSavingsTime != 0) {
		DO (*daylightSavingsTime, VDST, d)
			//
			// If endDST is before beginDST (as it would be in the
			// southern hemisphere), then if the given year equals
			// the endYear, then do not return that entry.
			//
			if (year == d->endYear) {
				if (d->dstEndIsBeforeBegin) {
					continue;
				}
				else {
					d->getBeginDateTime(year, datetime);
					return TRUE;
				}
			}
			else if ((year >= d->beginYear) && (year < d->endYear)) {
				d->getBeginDateTime(year, datetime);
				return TRUE;
			}
		END
	}
	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VTimeZone::getEndDST(VDate::Year year, VDateTime& datetime) const
{
	if (daylightSavingsTime != 0) {
		DO (*daylightSavingsTime, VDST, d)
			//
			// If endDST is before beginDST (as it would be in the
			// southern hemisphere), then if the given year equals
			// the beginYear, then do not return that entry.
			//
			if (year == d->beginYear) {
				if (d->dstEndIsBeforeBegin) {
					continue;
				}
				else {
					d->getEndDateTime(year, datetime);
					return TRUE;
				}
			}
			else if ((year > d->beginYear) && (year <= d->endYear)) {
				d->getEndDateTime(year, datetime);
				return TRUE;
			}
		END
	}
	return FALSE;
}

// --------------------------------------------------------------------------
// This should be overridden by derived classes and implemented using
// addToDST() if desired.
//
void VTimeZone::setupDST()
{
}

// --------------------------------------------------------------------------
//
void VTimeZone::addToDST(int          begin_year,
						 int          end_year,
						 int          begin_month,
						 int          begin_day_of_month,
						 const VTime& begin_time,
						 int          end_month,
						 int          end_day_of_month,
						 const VTime& end_time)
{
	if (end_year == 0) {
		end_year = 32767;
	}
	if (begin_year > end_year) {
		return;
	}
	if (daylightSavingsTime == 0) {
		daylightSavingsTime = new VOrdCollect;
	}
	VDST *d = new VDST(begin_year, end_year,
					   begin_month, begin_day_of_month, begin_time,
					   end_month, end_day_of_month, end_time);
	daylightSavingsTime->add(d);
}

// --------------------------------------------------------------------------
//
void VTimeZone::addToDST(int          begin_year,
						 int          end_year,
						 int          begin_month,
						 int          begin_day_of_week,
						 int          begin_nth_day_of_week,
						 const VTime& begin_time,
						 int          end_month,
						 int          end_day_of_week,
						 int          end_nth_day_of_week,
						 const VTime& end_time)
{
	if (end_year == 0) {
		end_year = 32767;
	}
	if (begin_year > end_year) {
		return;
	}
	if (daylightSavingsTime == 0) {
		daylightSavingsTime = new VOrdCollect;
	}
	VDST *d = new VDST(begin_year, end_year,
					   begin_month, begin_day_of_week,
					   begin_nth_day_of_week, begin_time,
					   end_month, end_day_of_week,
					   end_nth_day_of_week, end_time);
	daylightSavingsTime->add(d);
}

// --------------------------------------------------------------------------
//
VOrdCollect *VTimeZone::copyDST(VOrdCollect *dst)
{
	if (dst == 0) {
		return 0;
	}
	VOrdCollect *new_dst = new VOrdCollect;
	DO (*dst, VDST, d)
		VDST *new_d = new VDST(*d);
		new_dst->add(new_d);
	END
	return new_dst;
}

// --------------------------------------------------------------------------
// Return the number of minutes east or west of GMT the system seems
// to think we (locally) are at.  The returned value is negative for
// zones east of GMT and positive for zones west of GMT.
// Daylight Savings Time (DST) is disregarded.
//
int VTimeZone::determineLocalMinutesFromGMT() const
{
	time_t t = time(0);
	if (t == time_t(-1)) {
		return 0;
	}
	struct tm *local_tm = localtime(&t);
	if (local_tm == 0) {
		return 0;
	}
	if (local_tm->tm_isdst) {
		local_tm->tm_hour--;
	}
	VDateTime local_datetime(local_tm);
	struct tm *gmt_tm = gmtime(&t);
	if (gmt_tm == 0) {
		return 0;
	}
	if (gmt_tm->tm_isdst) {
		gmt_tm->tm_hour--;
	}
	VDateTime gmt_datetime(gmt_tm);
	return (local_datetime - gmt_datetime) / 60;
}

// --------------------------------------------------------------------------
// Copy constructor.
//
VTimeZone::VTimeZone(const VTimeZone& timezn)
{
	daylightSavingsTime = copyDST(timezn.daylightSavingsTime);
	minutesFromGMT = timezn.minutesFromGMT;
	isLocal = timezn.isLocal;
}

// --------------------------------------------------------------------------
// Assignment operator.
//
VTimeZone& VTimeZone::operator=(const VTimeZone& timezn)
{
	daylightSavingsTime = copyDST(timezn.daylightSavingsTime);
	minutesFromGMT = timezn.minutesFromGMT;
	return *this;
}

// --------------------------------------------------------------------------
// Return a reference to a VTimeZone object representing the local time.
//
// Note that only one instance of this local VTimeZone object will ever be
// created, and once created it will remain active until program termination.
//
VTimeZone& VTimeZone::local(boolean)
{
	if (localTimeZone == 0) {
		localTimeZone = new VTimeZone(VTimeZone::Local);
	}
	return *localTimeZone;
}

// --------------------------------------------------------------------------
// Return a reference to a VTimeZone object representing GMT.
//
// Note that only one instance of this local VTimeZone object will ever be
// created, and once created it will remain active until program termination.
//
VTimeZone& VTimeZone::gmt()
{
	if (gmtTimeZone == 0) {
		gmtTimeZone = new VTimeZone(VTimeZone::GMT);
	}
	return *gmtTimeZone;
}

// --------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
//
void VTimeZone::putTo(VArchiver& a)
{
	a << short(minutesFromGMT);
	a << short(isLocal);
	if ((daylightSavingsTime == 0) || (daylightSavingsTime->count() <= 0)) {
		a << char(0);
	}
	else {
		a << char(1);
		a.putObject(daylightSavingsTime);
	}
}

// --------------------------------------------------------------------------
//
void VTimeZone::getFrom(VArchiver& a)
{
	char c;
	short s;
	a >> s; minutesFromGMT = s;
	a >> s; isLocal = s;
	a >> c;
	if (c != 0) {
		daylightSavingsTime = (VOrdCollect *)(a.getObject(0));
	}
	else {
		daylightSavingsTime = 0;
	}
}

// --------------------------------------------------------------------------
//
void VTimeZone::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

#endif

// ==========================================================================
// VDST implementation

// --------------------------------------------------------------------------
//
defineClass(VDST, VObject)

// --------------------------------------------------------------------------
//
VDST::VDST()
{
}

// --------------------------------------------------------------------------
//
VDST::VDST(int          begin_year,
		   int          end_year,
		   int          begin_month,
		   int          begin_day_of_month,
		   const VTime& begin_time,
		   int          end_month,
		   int          end_day_of_month,
		   const VTime& end_time)
{
	beginYear			= begin_year;
	endYear				= end_year;
	beginMonth			= begin_month;
	beginDay			= begin_day_of_month;
	beginTime			= begin_time;
	endMonth			= end_month;
	endDay				= begin_day_of_month;
	endTime				= end_time;
	usingNthDayOfWeek	= 0;
	dstEndIsBeforeBegin	= (endMonth < beginMonth);
}

// --------------------------------------------------------------------------
//
VDST::VDST(int          begin_year,
		   int          end_year,
		   int          begin_month,
		   int          begin_day_of_week,
		   int          begin_nth_day_of_week,
		   const VTime& begin_time,
		   int          end_month,
		   int          end_day_of_week,
		   int          end_nth_day_of_week,
		   const VTime& end_time)
{
	beginYear			= begin_year;
	endYear				= end_year;
	beginMonth			= begin_month;
	beginDay			= begin_day_of_week;
	beginNthDayOfWeek	= begin_nth_day_of_week;
	beginTime			= begin_time;
	endMonth			= end_month;
	endDay				= begin_day_of_week;
	endNthDayOfWeek		= end_nth_day_of_week;
	endTime				= end_time;
	usingNthDayOfWeek	= 1;
	dstEndIsBeforeBegin	= (endMonth < beginMonth);
}

// --------------------------------------------------------------------------
//
VDST::VDST(const VDST& dst)
{
	beginTime			= dst.beginTime;
	endTime				= dst.endTime;
	beginYear			= dst.beginYear;
	endYear				= dst.endYear;
	beginMonth			= dst.beginMonth;
	endMonth			= dst.endMonth;
	beginDay			= dst.beginDay;
	endDay				= dst.endDay;
	beginNthDayOfWeek	= dst.beginNthDayOfWeek;
	endNthDayOfWeek		= dst.endNthDayOfWeek;
	usingNthDayOfWeek	= dst.usingNthDayOfWeek;
	dstEndIsBeforeBegin	= dst.dstEndIsBeforeBegin;
}

// --------------------------------------------------------------------------
//
void VDST::getBeginDateTime(int year, VDateTime& datetime) const
{
	if (usingNthDayOfWeek) {
		datetime.set(VDate(VDate::Year(year),
						   VDate::Month(beginMonth),
						   VDate::DayOfWeek(beginDay),
						   beginNthDayOfWeek),
						   beginTime);
	}
	else {
		datetime.set(VDate(VDate::Year(year),
						   VDate::Month(beginMonth),
						   VDate::DayOfMonth(beginDay)),
						   beginTime);
	}
}

// --------------------------------------------------------------------------
//
void VDST::getEndDateTime(int year, VDateTime& datetime) const
{
	if (usingNthDayOfWeek) {
		datetime.set(VDate(VDate::Year(year),
						   VDate::Month(endMonth),
						   VDate::DayOfWeek(endDay),
						   endNthDayOfWeek),
						   endTime);
	}
	else {
		datetime.set(VDate(VDate::Year(year),
						   VDate::Month(endMonth),
						   VDate::DayOfMonth(endDay)),
						   endTime);
	}
}

// --------------------------------------------------------------------------
//
#ifndef CV_NOARCHIVER

// --------------------------------------------------------------------------
//
void VDST::putTo(VArchiver& a)
{
	a << long(beginTime.getSeconds());
	a << long(endTime.getSeconds());
	a << short(beginYear);
	a << short(endYear);
	a << char(beginMonth);
	a << char(endMonth);
	a << char(beginDay);
	a << char(endDay);
	a << char(beginNthDayOfWeek);
	a << char(endNthDayOfWeek);
	a << char(usingNthDayOfWeek);
}

// --------------------------------------------------------------------------
//
void VDST::getFrom(VArchiver& a)
{
	long bt, et;
	char c;
	a >> bt;
	a >> et;
	a >> beginYear;
	a >> endYear;
	a >> beginMonth;
	a >> endMonth;
	a >> beginDay;
	a >> endDay;
	a >> beginNthDayOfWeek;
	a >> endNthDayOfWeek;
	a >> c;
	usingNthDayOfWeek = c;
	beginTime.set(bt);
	endTime.set(et);
	dstEndIsBeforeBegin	= (endMonth < beginMonth);
}

// --------------------------------------------------------------------------
//
void VDST::getFrom(VArchiver& a, VObject *, boolean)
{
	getFrom(a);
}

#endif
