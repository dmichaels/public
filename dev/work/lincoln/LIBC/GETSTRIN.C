#include "cdef.h"
#include <stdio.h>
#include <ctype.h>

/*
 *  getstring
 *
 *  Reads one string from file `f' into destination string 
 *  `dst'.  A maximum of `len' - 1 characters is read, and
 *  an end of string marker is put at the end.  The format
 *  string `fmt', describes what characters the string to
 *  be read is to be delimited by.
 *
 *  If the format string is NULL or empty, then the string
 *  to be read will be delimited by white space (i.e. tabs,
 *  blanks, and newlines;  ala isspace() -> ctype(3)).
 *
 *  If the first character in the format string is not a
 *  caret (^), then the input field is all the  characters
 *  until the first character not in the format string.
 *  That is, the format string describes the set of characters
 *  which make up the string to be read.
 *
 *  If the first character in the format string is a caret
 *  (^), then the input field is all the characters until
 *  the first character which is in the remaining set of
 *  characters in the format string.  That is, the format
 *  string describes the set of characters to delimit the
 *  string to be read.
 *
 *  Returns the destination string pointer, or NULL if the
 *  end of file is reached.
 *
 *  Note that the destination string `dst' must have at least
 *  `len' characters allocated for its use since the end of
 *  string character is appended.
 *
 *  To use the caret character literally in the format string,
 *  either make sure it is not the first character in the string,
 *  or if it must be the first in the string (because its the only
 *  character wanted in the string for example), then escape it by
 *  putting a backslash in front of it (the backslash will be ignored).
 *  The backslash itself can be escaped by using two backslashes
 *  instead of one. The only situation where this will be needed
 *  is when it is desired to read a string consisting of only
 *  backslashes and carets ==>  getstring(f, "\\^", dst).
 *
 *  This function is like reading a string using the `fscanf'
 *  function, except that a maximum number of characters may be given.
 */

#define CARET		'^'

char *
getstring(f,fmt,dst,len)
register FILE *f;
char *fmt, *dst;
register unsigned int len;
{
	char			*index();
	register int		c;
	register unsigned int	n = 0;
	register char		*s = dst;

	/*
	 *  Check boundry conditions.
	 */

	if(feof(f))
		return(NULL);
	if(--len <= 0)
		*s = EOS;
	if(s == NULL || len <= 0)
		return(dst);
		
	/*
	 *  The fmt string is NULL or empty.
	 *  Read a string delimited by white space.
	 */

	if(fmt == NULL || *fmt == EOS) {
		while((c=fgetc(f)) != EOF && isspace(c))
				;
		if(c == EOF) {
			*s = EOS;
			return(NULL);
		}
		*s++ = c;
		n++;
		while(n < len && (c=fgetc(f)) != EOF && !isspace(c)) {
			*s++ = c;
			n++;
		}
		*s = EOS;
		if(c == EOF)
			return(dst);
		if(isspace(c))
			ungetc(c,f);
		return(dst);
	}

	/*
	 *  First fmt character is not a CARET.
	 *  Read a string consisting of the
	 *  characters in the format string.
	 *
	 *  The BACKSLASH escape is handled here.
	 *  If the first fmt character is a BACKSLASH,
	 *  AND the second is a CARET, then the CARET
	 *  is to be taken literally.
	 */

	else if(*fmt != CARET) {
		if(*fmt == BACKSLASH && fmt[1] == CARET)
			fmt++;
		while((c=getc(f)) != EOF && index(fmt,c) == NULL)
			;
		if(c == EOF) {
			*s = EOS;
			return(NULL);
		}
		*s++ = c;
		n++;
		while(n < len && (c=getc(f))!=EOF && index(fmt,c) != NULL) {
			*s++ = c;
			n++;
		}
		*s = EOS;
		if(c == EOF)
			return(dst);
		if(index(fmt,c) == NULL)
			ungetc(c,f);
		return(dst);
	}

	/*
	 *  First fmt character is a CARET.
	 *  Read a string delimited by the
	 *  characters in the format string.
	 */

	else {
		fmt++;
		while((c=getc(f)) != EOF && index(fmt,c) != NULL)
			;
		if(c == EOF) {
			*s = EOS;
			return(NULL);
		}
		*s++ = c;
		n++;
		while(n < len && (c=getc(f)) != EOF && index(fmt,c) == NULL) {
			*s++ = c;
			n++;
		}
		*s = EOS;
		if(c == EOF)
			return(dst);
		if(index(fmt,c) != NULL)
			ungetc(c,f);
		return(dst);
	}
}
