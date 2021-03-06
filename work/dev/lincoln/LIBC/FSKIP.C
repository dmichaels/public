#include "cdef.h"
#include <stdio.h>
#include <ctype.h>

/*
 *  fskip
 *
 *  Skip past the next `n' characters specified in `charset',
 *  in a file, and return the next character on the input.
 *
 *  If the `charset' string is NULL or empty, then it will be
 *  assumed to represent all white space characters.
 *
 *  If the first character in the string `charset' is not a caret (^),
 *  then the file IO pointer of the given file `f' will be set just
 *  past the `n'th occurrence (from the current position) of a character
 *  which also appears in string `charset' will be returned, or NULL if
 *  none are found.  The `charset' string in this case, specifies a set
 *  of characters to skip TO in a file.  In other words, this usage
 *  will yield the next character in the file which appears in the
 *  string `charset' or EOF if none found.
 *
 *  If the first character in the string `charset' is a caret (^), then
 *  the file IO pointer of the given file `f' will be set just past the
 *  `n'th occurrence (from the current position) of a character which
 *  does not appear in string the `charset' will be returned, or NULL if
 *  none are found.  The `charset' string in this case, specifies a set of
 *  characters to skip PAST in a file.  In other words, this usage will
 *  yield the next character in the file which does not appear in the
 *  string `charset' or EOF if none found.
 */

#define CARET	'^'

int
fskip(f, charset, n)
register FILE *f;
register char *charset;
register unsigned int n;
{
	register int c;

	/*
	 *  Check boundry conditions.
	 */

	 if(n <= 0 || f == NULL || feof(f))
		return(EOF);

	/*
	 *  The charset string is NULL or empty,
	 *  assume it to represent white space.
	 */

	if(charset == NULL || *charset == EOS)	 {
		do {
			while((c=getc(f)) != EOF && isspace(c))
				;
			if(c == EOF)	
				return(EOF);
		} while(--n > 0);
		return(c);
	}

	/*
	 *  First charset character is not a CARET.
	 *  Return the next character in the file
	 *  which is also in the charset string.
	 *
	 *  The BACKSLASH escape is handled here.
	 *  If the first charset character is a BACKSLASH,
	 *  and the second is a CARET, then the CARET is to
	 *  be taken literally, and the BACKSLASH is ignored.
	 */

	if(*charset != CARET) {
		if(*charset == BACKSLASH &&
		   (charset[1] == CARET || charset[1] == BACKSLASH))
			charset++;
		do {
			while((c=getc(f)) != EOF && index(charset,c) == NULL)
				;
			if(c == EOF)	
				return(EOF);
		} while(--n > 0);
		return(c);
	}

	/*
	 *  First charset character is a CARET.
	 *  Return the next character in the file
	 *  which is not in the charset string.
	 */

	else {
		charset++;
		do {
			while((c=getc(f)) != EOF && index(charset,c) != NULL)
				;
			if(c == EOF)	
				return(EOF);
		} while(--n > 0);
		return(c);
	}
}
