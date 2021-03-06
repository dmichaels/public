/*
$Header:   Q:/views/common/vcs/BITSET.CPV   1.7   Mar 24 1997 16:08:10   scottt  $ 
*/
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
//	11/13/95 dss	VBitSet name change.
//  12/01/95 evc    Removed include of ostream.h when CV_DEBUG
//	07/31/96 pkt	fixed VBitSet(int n) to initialize data.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "archiver.h"
#include "object.h"
#include "cvassert.h"

#include "bitset.h"

extern "C" {
	#include <memory.h>
}

#define getMajorPosition(n)	(((n)-1) >> 5)  // (n-1) / 32
#define getMinorPosition(n)	(1L << (((n) & 0x1f) - 1)) // (1L << (((n) % 32) - 1))

#if defined(V_UNIT_TEST)

#define SIZEOF_BIT2TEXT (sizeof(bit2Text)/sizeof(bit2Text[0]))
static char *bit2Text[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

void VBitSet::dump(ostream& s) const
{
#if 0
	long		a = 1 << SIZEOF_BIT2TEXT;
	while (a) {
		s << (a & data ? '1' : '0');
		a = a >> 1;
	}
#else
	long				bitNameIndex = 0;
	unsigned long	lastBitPosition = 1 << SIZEOF_BIT2TEXT;
	unsigned long	bitPosition = 1;
	boolean	wrote = FALSE;

	s << '<';
	while (bitPosition <= lastBitPosition) {
		//!!!dss HACK: only uses first 32 bits => "data[0]"
		if (bitPosition & data[0]) {
			if (wrote) s << ',';
			s << bit2Text[bitNameIndex];
			wrote = TRUE;
		}
		bitPosition = bitPosition << 1;
		bitNameIndex++;
	}
	s << '>';
#endif
}
#endif

VBitSet::VBitSet()
{
	cv_assert(sizeof(data[0]) == 4);
	clear();
}

VBitSet::VBitSet(const VBitSet& b) 
{
	cv_assert(sizeof(data[0]) == 4);

	set(b);
}

VBitSet::VBitSet(int n)
{
	clear();

	int				i = getMajorPosition(n);
	unsigned long	bit = getMinorPosition(n);

	data[i] |= bit;
}

void VBitSet::set(int n)
{
	int				i = getMajorPosition(n);
	unsigned long	bit = getMinorPosition(n);

	data[i] |= bit;
}

void VBitSet::reset(int n)
{
	int				i = getMajorPosition(n);
	unsigned long	bit = getMinorPosition(n);

	data[i] &= ~bit;
}

boolean	VBitSet::operator==(const VBitSet& b) const
{
	return	memcmp(data, b.data, sizeof(data)) == 0;
}

void VBitSet::add(const VBitSet& b)
{
	data[0] |= b.data[0];
	data[1] |= b.data[1];
	data[2] |= b.data[2];
}

void VBitSet::remove(const VBitSet& b)
{
	data[0] &= ~b.data[0];
	data[1] &= ~b.data[1];
	data[2] &= ~b.data[2];
}

void VBitSet::set(const VBitSet& b)
{
	memcpy(data, b.data, sizeof(data));
}

void	VBitSet::clear(void) 
{
	memset(&data, '\0', sizeof(data));
}

boolean VBitSet::isEmpty(void) const
{
	return (data[0] == 0) && 
			 (data[1] == 0) && 
			 (data[2] == 0);
}

//
// This methods returns TRUE if ANY OF the bits, b, are a member of
// the bit set.
//
// This methods returns TRUE if the intersection of the bits, b, 
// and the bits in this bitset are not the empty set.
//
boolean VBitSet::contains(const VBitSet& b) const
{ 
	return 	(data[0] & b.data[0]) != 0 || 
				(data[1] & b.data[1]) != 0 ||
				(data[2] & b.data[2]) != 0;
}

#ifndef CV_NOARCHIVER
void VBitSet::getFrom(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "retrieving bitset" << endl;
#endif
	a >> data[0];
	a >> data[1];
	a >> data[2];
}

void VBitSet::putTo(VArchiver& a)
{
#if defined(V_UNIT_TEST)
	myCout << "archiving bitset" << endl;
#endif

	a << data[0];
	a << data[1];
	a << data[2];
}
#endif
