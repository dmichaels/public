#include "cdef.h"

/*
 *  Bit-map manipulation routines.
 *
 *  The bit map is specified by a character array,
 *  and is zero indexed.  No checking is done on
 *  the bit number (n) in any of these routines.
 */

#define BYTE_SIZE	8
#define SET_MASK	(~0)
#define CLR_MASK	(0)

#define BYTE_INDEX(n)	((n) / BYTE_SIZE)
#define BIT_MASK(n)	(01 << ((n) % BYTE_SIZE))
#define GETBIT(bmap,n)	((int)((bmap)[BYTE_INDEX(n)] & (BIT_MASK(n))))


/*
 *  getbit
 *
 *  Return the `n'th bit of the given bit map.
 */

int
getbit(bmap,n)
register char *bmap;
register int n;
{
	return(GETBIT(bmap,n));
}


/*
 *  bit_is_set
 *
 *  Return TRUE if bit `n' in the  bitmap
 *  `bmap' is set, otherwise FALSE (zero).
 */

bool
bit_is_set(bmap,n)
register char *bmap;
register int n;
{
	return(GETBIT(bmap,n) == 0 ? FALSE : TRUE);
}


/*
 *  bit_is_clr
 *
 *  Return TRUE if bit `n' in the bitmap
 *  `bmap' is clear, otherwise FALSE (zero).
 */

bool
bit_is_clr(bmap,n)
register char *bmap;
register int n;
{
	return(GETBIT(bmap,n) == 0 ? TRUE : FALSE);
}


/*
 *  setbit
 *
 *  Takes a bit map `bmap' and a number `n' corresponding to
 *  the `n'th bit and sets it if it was clear, and returns 0.
 *  Otherwise returns 1 (already set).  That is, returns the
 *  value of the bit before the function call.
 */

unsigned int
setbit(bmap,n)
register char *bmap;
register int n;
{
	register int q;  /* byte within map */
	register int m;  /* bit within byte */

	q = BYTE_INDEX(n);
	m = BIT_MASK(n);

	if(bmap[q] & m)		/* already set */
		return(1);
	bmap[q] |= m;
	return(0);
}


/*
 *  clrbit
 *
 *  Takes a bit map (bmap) and a number `n' corresponding to
 *  the `n'th bit and clears it if it was set, and returns 1.
 *  Otherwise returns 0 (already clear).  That is, returns
 *  the value of the bit before the function call.
 */

unsigned int
clrbit(bmap,n)
register char *bmap;
register int n;
{
	register int q;  /* byte within map */
	register int m;  /* bit within byte */

	q = BYTE_INDEX(n);
	m = BIT_MASK(n);

	if(!(bmap[q] & m))	/* already clear */
		return(0);
	bmap[q] &= ~m;
	return(1);
}


/*
 *  setmap
 *
 *  Sets all `n' bits in the given bit map to 1.
 */

void
setmap(bmap,n)
register char *bmap;
register unsigned int n;
{
	while(--n > 0)
		bmap[n] = SET_MASK;
}


/*
 *  clrmap
 *
 *  Clears all `n' bits in the given bit map to 0.
 */

void
clrmap(bmap,n)
register char *bmap;
register unsigned int n;
{
	while(--n > 0)
		bmap[n] = CLR_MASK;
}
