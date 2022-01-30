#include "cdef.h"

/*
*  gcd
*
*  Return the greatest common divisor of the
*  two given numbers.  Algorithm by Euclid.
*/

int
gcd(u,v)
register int u, v;
{
	register int t;

	while(v != 0){
		t = u % v;
		u = v;
		v = t;
	}
	return(u);
}
