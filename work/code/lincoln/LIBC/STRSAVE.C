#include "cdef.h"
#include <stdio.h>

/*
 *  strsave
 *
 *  Return copy of string `s'. NULL if no space.
 */

char *
strsave(s)
register char *s;
{
	char *malloc();
	register char *p = NULL, *q;

	if((p = malloc(strlen(s) + 1)) != NULL)
		for(q = p ; (*q++ = *s++) != EOS ; )
			;
	return(p);
}
