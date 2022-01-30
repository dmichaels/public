#include "cdef.h"
#include <ctype.h>

/*
 *  strupper
 *
 *  Convert all lower case letters in the given
 *  string `s' to upper case.  Return the string.
 */

char *
strupper(s)
register char *s;
{
	register char *p;

	for(p = s ; *p != EOS; p++)
		if (islower(*p))
			*p = toupper(*p);
	return(s);
}
