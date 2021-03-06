// $Header:   Q:/views/common/vcs/BITSET.H_V   1.10   Mar 24 1997 16:08:12   scottt  $ 

//	bitset.h
//
//	set of bits class [Common]
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
//	09/01/95 dss	created.
//	11/10/95 evc	changed class to CLASS
//	11/13/95 dss	VBitSet name change.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VBITSET_H
#define VBITSET_H

#include "object.h"

CLASS VBitSet : public VObject {

public:

	VBitSet();
	VBitSet(int n);
	VBitSet(const VBitSet& b);

	virtual boolean contains(const VBitSet& a) const;

	virtual void	set(int n);
	virtual void	reset(int n);

	virtual void 	reset() {VObject::reset();}

	boolean	operator==(const VBitSet& b) const;

	void		add(const VBitSet& b);
	void		remove(const VBitSet& b);
	void		set(const VBitSet& b);

	VBitSet&	operator=(const VBitSet& b) {set(b); return *this;}
	VBitSet&	operator+=(const VBitSet& b) {add(b); return *this;}
	VBitSet&	operator-=(const VBitSet& b) {remove(b); return *this;}

	void				clear(void);
	boolean			isEmpty(void) const;

#if defined(V_UNIT_TEST)
	virtual void dump(ostream&) const;
#endif

#ifndef CV_NOARCHIVER
	void		getFrom(VArchiver& a);
	void		getFrom(VArchiver& a, VObject *data, boolean do_realize = TRUE)
				{VObject::getFrom(a, data, do_realize);}
	void		putTo(VArchiver& a);
#endif

protected:
	unsigned long	data[3];

};

#endif // _BITSET_H_
