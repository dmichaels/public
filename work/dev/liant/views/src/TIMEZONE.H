// $Header:   Q:/views/common/vcs/TIMEZONE.H_V   1.8   Mar 24 1997 16:13:46   scottt  $

//	timezone.h
//
//	VTimeZone interface [Common]
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
//	05/31/95 dgm	Original
//	10/19/95 dgm	Changed default abbreviated parameter on getname()
//					from FALSE to TRUE.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTIMEZONE_H
#define VTIMEZONE_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include "date.h"
#include "thyme.h"

// --------------------------------------------------------------------------
//
CLASS	VDate;
CLASS	VDateTime;
CLASS	VObject;
CLASS	VOrdCollect;
CLASS	VString;
CLASS	VTime;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VTimeZone : public VObject {

public:
	enum Code {			TZ00	= -720,	// GMT-12:00
						TZ01	= -660,	// GMT-11:00
						TZ02	= -600,	// GMT-10:00
						TZ03	= -540,	// GMT-09:00
						TZ04	= -480,	// GMT-08:00
						TZ05	= -420,	// GMT-07:00
						TZ06	= -360,	// GMT-06:00
						TZ07	= -300,	// GMT-05:00
						TZ08	= -240,	// GMT-04:00
						TZ09	= -180,	// GMT-03:00
						TZ10	= -120,	// GMT-02:00
						TZ11	= - 60,	// GMT-01:00
						TZ12	=    0,	// GMT
						TZ13	= + 60,	// GMT+01:00
						TZ14	= +120,	// GMT+02:00
						TZ15	= +180,	// GMT+03:00
						TZ16	= +240,	// GMT+04:00
						TZ17	= +300,	// GMT+05:00
						TZ18	= +360,	// GMT+06:00
						TZ19	= +420,	// GMT+07:00
						TZ20	= +480,	// GMT+08:00
						TZ21	= +540,	// GMT+09:00
						TZ22	= +600,	// GMT+10:00
						TZ23	= +660,	// GMT+11:00
						TZ24	= +720,	// GMT+12:00

						GMT		= TZ12,
						Local	= 32766,
						NoTZ	= 32767
	};

public:
						VTimeZone(int = Local, VOrdCollect *dst = 0);
						VTimeZone(const VTimeZone&);
	virtual			   ~VTimeZone();
	VTimeZone&			operator=(const VTimeZone&);
	virtual void		getName(VString&, boolean dst = FALSE,
								boolean abbreviated = TRUE) const;
	virtual int			getMinutesFromGMT(boolean dst = FALSE) const;
	virtual boolean		hasDST() const;
	virtual boolean		isDST(const VDateTime&) const;
	virtual boolean		getBeginDST(VDate::Year, VDateTime&) const;
	virtual boolean		getEndDST(VDate::Year, VDateTime&) const;

	static VTimeZone&	local(boolean dst = TRUE);
	static VTimeZone&	gmt();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef CV_NOARCHIVER
	virtual void		putTo(VArchiver&);
	virtual void		getFrom(VArchiver&);
	virtual void 		getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif

	VClass			   *iam();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	void				construct(int, VOrdCollect *dst = 0);
	int					getTZ() const;
	virtual void		setupDST();
	virtual int			determineLocalMinutesFromGMT() const;
	void				addToDST(int          begin_year,
								 int          end_year,
								 int          begin_month,
								 int          begin_day_of_month,
								 const VTime& begin_time,
								 int          end_month,
								 int          end_day_of_month,
								 const VTime& end_time);
	void				addToDST(int          begin_year,
								 int          end_year,
								 int          begin_month,
								 int          begin_day_of_week,
								 int          begin_nth_day_of_week,
								 const VTime& begin_time,
								 int          end_month,
								 int          end_day_of_week,
								 int          end_nth_day_of_week,
								 const VTime& end_time);
	static VOrdCollect *copyDST(VOrdCollect *);

protected:
	VOrdCollect		   *daylightSavingsTime;
	short				minutesFromGMT;
	short				isLocal : 1;

};

extern VClass *VTimeZoneCls;

// --------------------------------------------------------------------------
//
inline int VTimeZone::getTZ() const
{
	int tz = (minutesFromGMT / 60) * 60;
	if ((tz < TZ00) || (tz > TZ24)) {
		tz = GMT;
	}
	return tz;
}

#endif // VTIMEZONE_H
