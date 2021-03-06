// $Header:   Q:/views/common/vcs/ACCEL.H_V   1.5   Mar 24 1997 16:07:54   scottt  $

//	accel.h
//
//	VAccelerator class interface [Common]
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
//	04/09/93 pmcm	changed id to VObject *
//	07/19/93 pmcm	added VArchiver support; preened.
//	07/27/93 pmcm	removed free()
//					added client and method indices
//	07/31/93 jmd	made count, indexOf, hash, etc. const
//	01/27/94 dgm	added additional getFrom() to avoid hiding warnings.
//	05/16/95 pkt	preened.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef ACCEL_H
#define ACCEL_H

#include "object.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VAcceleratorCls;

// Introduction
//
// A VAccelerator object is used to maintain an association
// between an accelerator key (a virtual key code) and a member
// function of some object. Instances of the VAccelerator class
// are created by the VNotifier. Whenever the virtual key is
// pressed, the VNotifier calls the call() method of the
// VAccelerator object to invoke the member function of the
// contained object, i.e., perform the callback on the
// contained object.

CLASS VAccelerator : public VObject {
public:
				VAccelerator();
				VAccelerator(int key);
				VAccelerator(int key, VObject *clnt, method m = 0);

	VClass 	   *iam();

	// Callback
	void 		call();

	// Equivalence
	boolean 	isEqual(VObject *obj) const;
	HashKey 	hash() const;

#ifndef CV_NOARCHIVER
	// Archival and Retrieval

	void		putTo(VArchiver& a);
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *data,
						boolean do_realize = TRUE);
#endif

protected:

	ushort 		wKey;			// virtual key code
	VObject    *pClient;		// client object to be called
	method 		pMethod;		// client method to be called

private:

#ifndef CV_NOARCHIVER
	integer		clientIndex;
	integer		methodIndex;
#endif
};

#endif	// ACCEL_H
