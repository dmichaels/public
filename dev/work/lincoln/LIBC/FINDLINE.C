#include "cdef.h"
#include <stdio.h>

/*
 *  findline
 *
 *  Read into the character buffer `line', the next line in the
 *  file `f' which contains an occurrence of the pattern `pat'.
 *  The line will contain only `maxline' - 1 characters, or all
 *  characters up to a newline (whichever comes first).  The
 *  newline (if present) is kept (ala fgets(3S)).
 */

char *
findline(pat,line,maxline,f)
register char *pat, *line;
register int maxline;
register FILE *f;
{
	char *sindex(), *fgets();

	while(fgets(line,maxline,f) != NULL)
		if(sindex(line,pat,1))
			return(line);
	return(NULL);
}
