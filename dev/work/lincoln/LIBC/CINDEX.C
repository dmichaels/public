#include "cdef.h"
#include <stdio.h>
#include <ctype.h>

/*
 *  cindex
 * 
 *  If the first character in the string `charset' is not
 *  a caret (^), then a pointer to the `n'th occurrence of
 *  a character in the string `s' which also appears in
 *  string `charset' will be returned, or NULL if none are
 *  found.
 *
 *  If the first character in the string `charset' is a
 *  caret (^), then a pointer to the `n'th occurrence of
 *  a characrter in string `s' which does not appear in
 *  string `charset' will be returned, or NULL if none are
 *  found.
 */

#define CARET	'^'

char *
cindex(s, charset, n)
register char *s, *charset;
register unsigned int n;
{
	/*
	 *  Check boundry conditions.
	 */

	 if(n <= 0 || s == NULL || *s == EOS)
		return(NULL);

	/*
	 *  The charset string is NULL or empty,
	 *  assume it to represent white space.
	 *  Return a pointer to the nth non-white
	 *  character in the string.
	 */

	if(charset == NULL || *charset == EOS) {
		while(TRUE) {
			while(isspace(*s)) {
				if(*++s == EOS)
					return(NULL);
			}
			if(--n <= 0)
				return(s);
			if(*++s == EOS)
				return(NULL);
		}
	}

	/*
	 *  First charset character is not a CARET.
	 *  Return a pointer to the nth character in
	 *  the string which is also in the charset string.
	 *
	 *  The BACKSLASH escape is handled here.
	 *  If the first charset character is a BACKSLASH, and the second
	 *  is a CARET, then the CARET is to be taken literally, and the
	 *  the BACKSLASH is ignored.  If the first and second characters
	 *  are both BACKSLASHs, then they are to be taken as one.
	 */

	if(*charset != CARET) {
		if(*charset == BACKSLASH &&
		   (charset[1] == CARET || charset[1] == BACKSLASH))
			charset++;
		while(TRUE) {
			while(index(charset,*s) == NULL) {
				if(*++s == EOS)
					return(NULL);
			}
			if(--n <= 0)
				return(s);
			if(*++s == EOS)
				return(NULL);
		}
	}

	/*
	 *  First charset character is a CARET.
	 *  Return a pointer to the nth character in
	 *  the string which is not in the charset string.
	 */  

	else {
		charset++;
		while(TRUE) {
			while(index(charset,*s) != NULL) {
				if(*++s == EOS)
					return(NULL);
			}
			if(--n <= 0)
				return(s);
			if(*++s == EOS)
				return(NULL);
		}
	}
}
