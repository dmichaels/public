// $Header:   Q:/views/common/vcs/TZUS.H_V   1.8   Mar 24 1997 16:14:00   scottt  $

//	tzus.h
//
//	VTimeZoneUS interface [Common]
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
//	01/29/96 dgm	Changed default abbreviated parameter on getname()
//					from FALSE to TRUE.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VTZUS_H
#define VTZUS_H

// --------------------------------------------------------------------------
//
#include "timezone.h"

// --------------------------------------------------------------------------
//
CLASS	VObject;
CLASS	VOrdCollect;
CLASS	VString;
CLASS	VTimeZone;

// --------------------------------------------------------------------------
//
CLASS VTimeZoneUS : public VTimeZone {

public:
	enum Code {
						Atlantic	= VTimeZone::TZ08,
						Eastern		= VTimeZone::TZ07,
						Central		= VTimeZone::TZ06,
						Mountain	= VTimeZone::TZ05,
						Pacific		= VTimeZone::TZ04,
						Alaska		= VTimeZone::TZ03,
						Hawaii		= VTimeZone::TZ02,
						Samoa		= VTimeZone::TZ01
	};

public:
						VTimeZoneUS(int = Eastern, VOrdCollect *dst = 0);
					   ~VTimeZoneUS();
	virtual void		getName(VString&, boolean dst = FALSE,
								boolean abbreviated = TRUE) const;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	virtual void		setupDST();

};

#endif // VTZUS_H
