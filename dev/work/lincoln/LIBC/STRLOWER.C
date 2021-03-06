#include "cdef.h"
#include <ctype.h>

/*
 *  strlower
 *
 *  Convert all upper case letters in the given
 *  string `s' to lower case.  Return the string.
 */

char *
strlower(s)
register char *s;
{
	register char *p;

	for(p = s ; *p != EOS ; p++)
		if(isupper(*p))
			*p = tolower(*p);
	return(s);
}
