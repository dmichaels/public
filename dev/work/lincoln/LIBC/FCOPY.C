#include "cdef.h"
#include <stdio.h>

/*
 *  fcopy
 *
 *  Copy the file `f1' to the file `f2'.
 *  The two files may not be the same.
 */

void
fcopy(f1,f2)
register FILE *f1, *f2;
{
	register char c;

	if(f1 == NULL || f2 == NULL || f1 == f2)
		return;
	while((c = getc(f1)) != EOF)
		putc(c,f2);
}
