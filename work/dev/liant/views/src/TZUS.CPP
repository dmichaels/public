// $Header:   Q:/views/common/vcs/TZUS.CPV   1.6   Mar 24 1997 16:14:00   scottt  $

//	tzus.cpp
//
//	VTimeZoneUS implementation [Common]
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
//	07/06/95 dgm	Fix in constructor.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "tzus.h"
#include "datetime.h"
#include "str.h"

// --------------------------------------------------------------------------
//
VTimeZoneUS::VTimeZoneUS(int code, VOrdCollect *dst)
	: VTimeZone(VTimeZone::NoTZ)
{
	construct(VTimeZone::Code(code), dst);
}

// --------------------------------------------------------------------------
//
VTimeZoneUS::~VTimeZoneUS()
{
}

// --------------------------------------------------------------------------
//
void VTimeZoneUS::getName(VString& s, boolean dst, boolean abbreviated) const
{
	switch (getTZ()) {

	case Atlantic:
		if (abbreviated) {
			VTimeZone::getName(s, dst, abbreviated);
		}
		else {
			if (dst) { s  = "Atlantic Daylight Time"; }
			else     { s  = "Atlantic Standard Time"; }
		}
		break;

	case Eastern:
		if (abbreviated) {
			if (dst) { s = "EDT"; }
			else     { s = "EST"; }
		}
		else {
			if (dst) { s  = "Eastern Daylight Time"; }
			else     { s  = "Eastern Standard Time"; }
		}
		break;

	case Central:
		if (abbreviated) {
			if (dst) { s = "CDT"; }
			else     { s = "CST"; }
		}
		else {
			if (dst) { s  = "Central Daylight Time"; }
			else     { s  = "Central Standard Time"; }
		}
		break;

	case Mountain:
		if (abbreviated) {
			if (dst) { s = "MDT"; }
			else     { s = "MST"; }
		}
		else {
			if (dst) { s  = "Mountain Daylight Time"; }
			else     { s  = "Mountain Standard Time"; }
		}
		break;

	case Pacific:
		if (abbreviated) {
			if (dst) { s = "PDT"; }
			else     { s = "PST"; }
		}
		else {
			if (dst) { s  = "Pacific Daylight Time"; }
			else     { s  = "Pacific Standard Time"; }
		}
		break;

	case Alaska:
		if (abbreviated) {
			VTimeZone::getName(s, dst, abbreviated);
		}
		else {
			if (dst) { s  = "Alaska Daylight Time"; }
			else     { s  = "Alaska Standard Time"; }
		}
		break;

	case Hawaii:
		if (abbreviated) {
			VTimeZone::getName(s, dst, abbreviated);
		}
		else {
			if (dst) { s  = "Hawaii Daylight Time"; }
			else     { s  = "Hawaii Standard Time"; }
		}
		break;

	case Samoa:
		if (abbreviated) {
			VTimeZone::getName(s, dst, abbreviated);
		}
		else {
			if (dst) { s  = "Samoa Daylight Time"; }
			else     { s  = "Samoa Standard Time"; }
		}
		break;
	}
}

// --------------------------------------------------------------------------
//
void VTimeZoneUS::setupDST()
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 0-1966: 2 AM, last Sunday, April -> 2 AM, last Sunday, October
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addToDST(0, 1966,
			 VDate::April,   VDate::Sunday, 0, VTime(2, 0, 0),
			 VDate::October, VDate::Sunday, 0, VTime(2, 0, 0));

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 1967-1973: 2 AM, last Sunday, April -> 2 AM, last Sunday, October
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addToDST(1967, 1973,
			 VDate::April,   VDate::Sunday, 0, VTime(2, 0, 0),
			 VDate::October, VDate::Sunday, 0, VTime(2, 0, 0));

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 1974: 2 AM, first Sunday, Jan -> 2 AM, last Sunday, October
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addToDST(1974, 1974,
			 VDate::January,  VDate::Sunday, 1, VTime(2, 0, 0),
			 VDate::October,  VDate::Sunday, 0, VTime(2, 0, 0));
	
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 1975: 2 AM, last Sunday, Feb -> 2 AM, last Sunday, October
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addToDST(1975, 1975,
			 VDate::February, VDate::Sunday, 0, VTime(2, 0, 0),
			 VDate::October,  VDate::Sunday, 0, VTime(2, 0, 0));

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 1976-1986: 2 AM, last Sunday, April -> 2 AM, last Sunday, October
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addToDST(1976, 1986,
			 VDate::April,    VDate::Sunday, 0, VTime(2, 0, 0),
			 VDate::October,  VDate::Sunday, 0, VTime(2, 0, 0));

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 1986-present: 2 AM, first Sunday, April -> 2 AM, last Sunday, October
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	addToDST(1986, 0,
			 VDate::April,    VDate::Sunday, 1, VTime(2, 0, 0),
			 VDate::October,  VDate::Sunday, 0, VTime(2, 0, 0));
}
