#include "cdef.h"

/*
 *  creplace
 *
 *  Substitue new char for old in
 *  string `s'.  Return string `s'.
 */

creplace(s,old,new)
register char *s, old, new;
{
	for( ; *s != EOS ; s++)
		if(*s == old)
			*s = new;
}
