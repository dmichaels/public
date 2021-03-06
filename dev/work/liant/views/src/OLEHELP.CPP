// $Header:   Q:/views/win/vcs/OLEHELP.CPv   1.2   Mar 04 1997 14:13:20   gregm  $

//  cditem.cpp
//
//  Compound Document Item
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
//  Revision History:
//  -----------------
//	02/16/96 dss	Initial release.
//	--------------------------------------------------------------------------

// Note: Don't include ANYTHING or a link error will occur. Borland inaccurately
// declares BOleHelpNotify as "C++" in a system include file.

// This is a Borland only file.

#if defined(__BCPLUSPLUS__)
// This empty function satisfies an unresolved external in BOLEUI and BOLEUI32 which
// is defined, privately, in BOCOLE.
// BOleHelpNotify() sets up help which this function disables because it is empty.
// It is a hack for now, when we can compile VOLEUI(32).LIB for borland.
#pragma warn -par
extern "C"
void BOleHelpNotify (unsigned long a, int b)
{
}
#pragma warn .par
#endif
