#include "cdef.h"
#include <stdio.h>

/*
 *  sindex
 *
 *  Return pointer to first occurrence of
 *  pattern string `pat' in the given string
 *  `str'.  Return NULL if not found.
 *
 *  If flag = 0:
 *  Returned pointer points to the
 *  the first character of the string.
 *
 *  If flag != 0:
 *  Returned pointer points to the
 *  the last character of the string.
 *
 *  Brute force algorithm.
 */

char *
sindex(str,pat,flag)
register char *str, *pat;
int flag;
{
	register char *p = pat;
	register char *s = str;

	do {
		if(*s == *p) {
			s++;
			p++;
		}
		else {
			s = ++str;
			p = pat;
		}
	} while(*p != EOS && *s != EOS);

	return(*p == EOS ? (flag == 0 ? str : (str+(p-pat)-1)) : NULL);
}
