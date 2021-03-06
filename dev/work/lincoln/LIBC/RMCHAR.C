#include "cdef.h"
#include <stdio.h>

/*
 *  rmchar
 *
 *  Remove the first `n' occurrences in string `s' of any
 *  character appearing in the character set string `set'.
 *  If `n' is less than or equal to zero, then all occurrences
 *  of any `set' characters will be removed.  Return the number
 *  of characters actually removed.
 */

int
rmchar(s,set,n)
register char *s, *set;
register int n;
{
	static char nullstring[] = "";
	static char whitespace[] = " \t\n";
	register char *p;
	register int nchars = 0;
	register bool all = FALSE;

	if(n <= 0)
		all = TRUE;

	if(set == NULL || *set == EOS)
		set = whitespace;

	for( ; *s != EOS ; s++) {
		for(p = set ; *p != EOS ; p++) {
			if(*s == *p){
				if(n-- > 0 || all)
					goto Remove;
				else
					set = nullstring;
			}
		}
		*(s - nchars) = *s;
		continue;
	 Remove:
		nchars++;
	}
	*(s - nchars) = EOS;
	return(nchars);
}
