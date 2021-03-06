// $Header:   Q:/views/win/vcs/memry.h_v   1.6   Mar 04 1997 16:32:38   gregm  $ 

//	memry.h
//
//	VMemory interface
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	09/10/92 jmd	made MSC ifdefs use >= instead of ==
//	10/26/92 jmd	preened, made memInit static (for non-DLL)
//	03/05/93 pat	preened
//	03/15/93 pat	changed memInit from int to long, made non-static
//	03/19/93 pat	removed redundant CV_WINDOWS #define
//	04/09/93 pat	changed id to VObject *
//	06/02/93 pkt	Removed #ifdef USED and #ifdef MEM_TRACE.  Interface will
//					be as if USED and MEM_TRACE were always TRUE.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	07/12/94 jmd	split
// 	08/17/95 evc	memry32.h no longer needed for 32 bit stuff
// ----------------------------------------------------------------------------

#ifndef VMEMRY_H
#define VMEMRY_H

#if defined(CV_WIN16)
#	include "memry16.h"
#endif

#endif	// VMEMRY_H
