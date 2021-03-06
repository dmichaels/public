// $Header:   Q:/views/win/vcs/DOSDTTM.H_v   1.2   Mar 04 1997 14:12:26   gregm  $

//	dosdttm.h
//
//	Dos Data & Time class interface
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	01/23/96 rhw	Initial Revision.
// --------------------------------------------------------------------------

#ifndef DOSDTTM_H
#define DOSDTTM_H

struct DOSDateBits
{
	unsigned short dayOfMonth	: 5;
	unsigned short month		: 4;
	short year					: 7;	// Offset from 1980
};

struct DOSTimeBits
{
	unsigned short second		: 5;	// Divided by 2 (thus can't represent odd numbered seconds)
	unsigned short minute		: 6;
	unsigned short hour			: 5;
};

union DOSDate
{
	unsigned short	dosDate;
	DOSDateBits		dosDateBits;
};

union DOSTime
{
	unsigned short	dosTime;
	DOSTimeBits		dosTimeBits;
};

#endif // DOSDTTM_H
