#include "cdef.h"
#include <stdio.h>

/*
 *  ftell_pat
 *
 *  Return the offset in the file `f' to the first occurance
 *  of the pattern string `pat'.  Return -1 if not found.
 *
 *  If flag == 0:
 *  The returned offset will be that of the first character in the
 *  pattern string.  That is, the next getc(f) will yield the first
 *  character of first occurrence of the pattern string in the file.
 *
 *  If flag != 0:
 *  The returned offset will be that of the last character in the
 *  pattern string.  That is, the next getc(f) will yield the character
 *  immediately following the last character of first occurrence of the
 *  pattern string in the file.
 *
 *  WARNING:  This version uses fseek(3S) and ftell(3S);  If a caller
 *  wants to issue a subsequent seek command, fseek(3S) must be used
 *  rather the lseek(3) which for some reason is not compatible with
 *  lseek.
 */

#define FSEEK_BEGIN	0
#define FSEEK_CURRENT	1
#define FSEEK_END	2
#define NOTFOUND	(-1)

long
ftell_pat(f,pat,flag)
register FILE *f;
register char *pat;
unsigned int flag;
{
	register int c;
	register unsigned int i;
	long adjust, saveoff, patoff;

	patoff = NOTFOUND;
	adjust = (flag == 0 ? (long)(-strlen(pat)) : 0L);

	saveoff = ftell(f);

	while(TRUE) {
		i = 0;
		while(pat[i] != EOS && (c = getc(f)) != EOF &&  c == pat[i])
			i++;
		if(c == EOF)				/* not found */
			break;
		if(pat[i] == EOS){			/* found */
			fseek(f,adjust,FSEEK_CURRENT);	/* adjust IO ptr */
			patoff = ftell(f);		/* get IO ptr value */
			break;
		}
	}
	fseek(f,saveoff,0);				/* restore IO ptr */
	return(patoff);
}
