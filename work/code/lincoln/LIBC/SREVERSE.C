#include "cdef.h"

/*
 *  sreverse
 *
 *  Reverse the given string `s' and return it.
 */

char *
sreverse(s)
register char *s;
{
	register char *a, *z, c;

	a = s;
	z = s + strlen(s) - 1;
	while(a < z) {
		c = *a;
		*a = *z;
		*z = c;
		a++;
		z--;
	}
	return(s);
}
