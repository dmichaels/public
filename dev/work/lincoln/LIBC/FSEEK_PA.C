#include "cdef.h"
#include <stdio.h>

/*
 *  fseek_pat
 *
 *  Set the IO pointer of file `f' to the position of the next
 *  occurrence of the pattern string `pat'.  If the pattern
 *  is found, the old file offset will be returned, and the IO
 *  pointer will be set to the last character in the pattern
 *  string (i.e. the next getc(f) will yield the character
 *  in the file immediatley following the last character of
 *  the next occurence of the pattern string in the file).
 *  If the pattern is not found, return NOTFOUND, and leave
 *  the IO pointer intact.
 */

#define FSEEK_BEGIN	0
#define NOTFOUND 	((long)-1)

long
fseek_pat(f,pat)
register FILE *f;
register char *pat;
{
	register int c;
	register char *p;
	long patoff, saveoff = ftell(f);

	while(TRUE) {
		p = pat;
		while(*p != EOS && (c = getc(f)) != EOF && c == *p)
			p++;
		if(c == EOF) {
			fseek(f,saveoff,FSEEK_BEGIN);
			return(NOTFOUND);
		}
		if(*p == EOS) {
			return(saveoff);
		}
	}
}
