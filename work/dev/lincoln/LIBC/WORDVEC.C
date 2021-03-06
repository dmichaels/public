#include "cdef.h"
#include <stdio.h>

/*
 *  wordvec
 *
 *  Construct a word vector consisting of words in the given string
 *  `str' delimited by characters in the character set `delim'.
 *  Pointers to the beginning of each word are placed in order in the
 *  character pointer array `vec'.  Return the number of words actually
 *  put into the vector.
 *
 *  The string `str' is munged since EOS characters are acually replace
 *  every delimiter character.
 *
 *  The character pointer array `vec' is assumed to have at least `nword'
 *  elements allocated for its use.
 */

int
wordvec(str,vec,nword,delim)
register char *str, **vec;
register int nword;
char *delim;
{
	register int	n;
	register bool	in_delim = TRUE;

	for(n = 0 ; n < nword && *str != EOS ; str++) {
		if(index(delim,*str)) {
			*str = EOS;
			in_delim = TRUE;
		}
		else if(in_delim == TRUE) {	
			vec[n++] = str;
			in_delim = FALSE;
		}
	}
	while(*str != EOS && index(delim,*str) == NULL)
		str++;
	*str = EOS;
	return(n);
}
