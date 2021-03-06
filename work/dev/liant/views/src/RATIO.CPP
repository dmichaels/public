// $Header:   Q:/views/common/vcs/RATIO.CPV   1.7   Mar 24 1997 16:13:08   scottt  $ 

//	ratio.cpp
//
//	VRatio class implementation [Common]
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
//	10/02/93 dgm	Fix in asString().
//	11/02/94 dgm	Moved operator() to here from ratio.h and applied
//					pkt modification which fixed rounding problem which
//					only appeared when using Microsoft Visual C++.
//	05/16/95 pkt	preened.
//	11/20/95 dgm	Fix in asFloat().
//	11/20/95 pkt	updated for MRATIO macro.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "ratio.h"
#include "str.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ---------------------------------------------------------------------------
// Convert this `VRatio` to ASCII and place it into `VString` 's'.
//
void VRatio::asString(VString& s)
{
	if (n <= MRATIO) {
		int i = n - IRATIO;
		float f = i /RATIO;
#if 0
		s.prints("%f", f);
#else
		char buffer[64];
		sprintf(buffer, "%f", f);
		s.prints("%s", buffer);
#endif
	}
	else {
		s.prints("%d", n);
	}
}
// ---------------------------------------------------------------------------
// Convert `VString` object 's' to a value for this `VRatio`.
//
void VRatio::fromString(VString &s)
{
	const char *p = s.gets();
	float f;
	if (strchr(p, '.')) {
		f = (float)atof(p);
		n = (int)(f *RATIO + IRATIO);
	}
	else {
		n = atoi(p);
	}
}

// ---------------------------------------------------------------------------
//
void VRatio::asFloat(float &f)
{
	if (n <= MRATIO) {
		int i = n - IRATIO;
		f = i / RATIO;
	}
}
