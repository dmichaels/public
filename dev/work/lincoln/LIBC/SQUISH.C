#include "cdef.h"
#include <stdio.h>
#include <ctype.h>

/*
 *  squish
 *
 *  Converts multiple consecutive occurences of characters given
 *  in the `multchar' string in the target string `str', into one
 *  `onechar' character.  If the `multchar' string is NULL or empty,
 *  then it will be assumed to represent all white space characters.
 *  Returns the string.
 */

char *
squish(str,multchar,onechar)
register char *str, *multchar, onechar;
{
	static char whitespace[] = " \t\n";
	register char *p;
	register int nchars = 0;

	if(multchar == NULL || *multchar == EOS)
		multchar = whitespace;

	for(p = str ; *p != EOS ; p++) {
		if(index(multchar,*p) != NULL) {
			nchars++;
			continue;
		}
		if(nchars > 0)
			*(p - nchars) = onechar;
		*(p - nchars) = *p;
	}
	*(p - nchars) = EOS;
	return(str);
}
