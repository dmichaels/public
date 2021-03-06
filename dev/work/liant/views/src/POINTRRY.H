// $Header:   Q:/views/common/vcs/POINTRRY.H_V   1.10   Mar 24 1997 16:12:56   scottt  $ 

//	pointrry.h
//
//	VPointArray class interface [Common]
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
//	07/31/93 jmd	made size const
//	08/09/93 jmd	removed free
//	09/26/93 jmd	preened (a little)
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened.
//	01/17/94 dgm	cfront C++ changes
//	03/26/94 pmcm	Win32 POINT coordinates are longs. 
//	07/13/94 jmd	preened (a little more)
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VPOINTRRY_H
#define VPOINTRRY_H

// ---------------------------------------------------------------------------
//
#include "containr.h"
#include "location.h"

#if defined(CV_WIN32) 
	//	03/26/94 pat--note that Win32 POINT coordinates are longs. 
	//	C++/Views coordinates are represented by ints.  
	//	VPolygon::set() and VPort:drawLines() rely on the fact that the
	//	internal representation of the VPointArray object 
	//	(via VContainer::getData()) coincides with POINT.  Rather than copy
	//	arrays of integer coordinates to long coordinates to use with the
	//	Win32 calls under VPolygon::set() and VPort:drawLines(), I 
	//	choose to continue to those methods allow this assumption.
	typedef POINT point; 
#else
	typedef struct tagPoint {
		short x, y;
	} point;
#endif

// ---------------------------------------------------------------------------
// A VPointArray is a VContainer of points which are (x,y) pairs 
// of integers.  
//
CLASS VPointArray : public VContainer {
public:
	// Constructors
					VPointArray();
					VPointArray(integer);
					VPointArray(const VPointArray&);
	VClass		   *iam();

	VPointArray&	operator=(const VPointArray&);

	// VPointArray Properties
	integer			size() const;

	// Setting and Interrogating Points
	void			add(int, int);
	boolean			getpnt(integer, int *, int *);
	VLocation		getpnt(integer n);
	void			putpnt(integer, int, int);
	void			putpnt(integer n, VLocation &p);

	// Initialization
	void			reset();

	// Copying
	boolean			copyFrom(VObject *);

#ifndef CV_NOARCHIVER
	// Archival
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);
	void			putTo(VArchiver&);
#endif

protected:
	void			init();

protected:
	integer			howMany;	// number of points in this VPointArray


};

extern VClass *VPointArrayCls;

// ---------------------------------------------------------------------------
//
inline void VPointArray::init()
{
	howMany = 0;
	eleSize = sizeof(point);
}

// ---------------------------------------------------------------------------
// Return the nth point as a VLocation object. 
//
inline VLocation VPointArray::getpnt(integer n)
{
	int x, y;
	getpnt(n, &x, &y);
	return(VLocation(x, y));
}

// ---------------------------------------------------------------------------
// Set the nth point of this VPointArray to the VLocation p.
//
inline void VPointArray::putpnt(integer n, VLocation &p)
{
	putpnt(n, p.x(), p.y());
}

#endif // VPOINTRRY_H
