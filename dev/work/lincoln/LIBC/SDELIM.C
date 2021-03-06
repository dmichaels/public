#include "cdef.h"
#include <stdio.h>

/*
 *  sdelim
 *
 *  Given a string `str', and an opening and closing delimeter string,
 *  this function will put the portion of the string between the
 *  delimeters into the target string.  The target is assumed to
 *  have enough space allocated to it.  If either of the delimeters
 *  is not found, or  the opening one is after he closing one, then
 *  NULL is returned, otherwise a pointer to the target is returned.
 *
 *  `str'	- string to be parsed.
 *  `open'	- opening delimeter.
 *  `close'	- closing delimeter.
 *  `target'	- string into which will be put the contents of
 *		  what is between the open and close delimeters.
 */

char *
sdelim(str,open,close,target)
char *str, *open, *close;
register char *target;
{
	char *sindex();
	register char *p, *q;

	if((p = sindex(str,open,1)) == NULL)
		return(NULL);
	if((q = sindex(++p,close,0)) == NULL)
		return(NULL);
	while(p != q)
		*target++ = *p++;
	*target = EOS;
	return(target);
}
