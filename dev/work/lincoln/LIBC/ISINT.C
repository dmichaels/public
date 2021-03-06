#include "cdef.h"
#include <ctype.h>

/*
 *  isint
 *
 *  Return TRUE if string `s' consists
 *  of all digits, otherwise return FALSE.
 */

bool
isint(s)
register char *s;
{
	for( ; *s != EOS ; s++) 
		if(!isdigit(*s))
			return(FALSE);
	return(TRUE);
}
