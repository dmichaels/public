// $Header:   Q:/views/common/vcs/ATTACH.H_V   1.6   Mar 24 1997 16:08:04   scottt  $ 

//	attach.h
//
//	VAttach class interface [Common]
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
//	04/29/93 jmd	created
//	05/20/93 jmd	renamed None to UnAttached to avoid Motif problems
//	07/19/93 pmcm	changed class to CLASS in forward declarations.
//	08/11/93 dgm	added getType(), setType(), setOffset(), & setSibling().
//	10/21/93 dgm	3.0 changes; re-ported from MS/Windows.
//	05/25/94 dgm	Added VAttach::Type Child.
//	05/19/95 pkt	selected Motif as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VATTACH_H
#define VATTACH_H

#include "defs.h"

CLASS VWindow;
CLASS VStatusBar;

CLASS VAttach {
public:
	// this numbering is important, do not change
	enum Side	{ Top = 0, Bottom = 1, Left = 2, Right = 3 };
	enum Type	{ UnAttached, Parent, Sibling, SiblingSame, Frame, Child };

				VAttach();
			   ~VAttach();

	void		setAttach(Side s, Type t, int off = 0, VWindow *sib = 0);

	Type		getAttach(VAttach::Side s) const;
	int			getOffset(VAttach::Side s) const;
	VWindow	   *getSibling(VAttach::Side s) const;

	VAttach::Type	getType(VAttach::Side) const;
	void			setType(VAttach::Side, VAttach::Type);
	void			setOffset(VAttach::Side, int);
	void			setSibling(VAttach::Side, VWindow *);

	int			getPriority();
	void		setPriority(int p);

	unsigned	getPtest();
	void		setPtest(unsigned p);

private:
	Type		attach[4];
	int			offset[4];
	VWindow	   *sibling[4];

	int			priority;			// used to determine ordering priority
	unsigned	ptest;

private:
	friend		VStatusBar;
};

// ---------------------------------------------------------------------------
//
inline VAttach::Type VAttach::getAttach(VAttach::Side s) const
{
	return attach[s];
}

// ---------------------------------------------------------------------------
//
inline VAttach::Type VAttach::getType(VAttach::Side s) const
{
	return attach[s];
}

// ---------------------------------------------------------------------------
//
inline void VAttach::setType(VAttach::Side s, VAttach::Type t)
{
	attach[s] = t;
}

// ---------------------------------------------------------------------------
//
inline int VAttach::getOffset(VAttach::Side s) const
{
	return offset[s];
}

// ---------------------------------------------------------------------------
//
inline void VAttach::setOffset(VAttach::Side s, int o)
{
	offset[s] = o;
}

// ---------------------------------------------------------------------------
//
inline VWindow *VAttach::getSibling(VAttach::Side s) const
{
	return sibling[s];
}

// ---------------------------------------------------------------------------
//
inline void VAttach::setSibling(VAttach::Side s, VWindow *w)
{
	sibling[s] = w;
}

// ---------------------------------------------------------------------------
//
inline void VAttach::setPriority(int p)
{
	priority = p;
}

// ---------------------------------------------------------------------------
//
inline int VAttach::getPriority()
{
	return priority;
}

// ---------------------------------------------------------------------------
//
inline void VAttach::setPtest(unsigned p)
{
	ptest = p;
}

// ---------------------------------------------------------------------------
//
inline unsigned VAttach::getPtest()
{
	return ptest;
}

#endif // VATTACH_H
