#include "cdef.h"
#include <stdio.h>

/*
 *  getline
 *
 *  Reads `maxline' - 1 characters, or up to a newline, whichever
 *  comes first, from the given file `f' into the string `line'
 *  after first skipping any leading blanks in the input stream
 *  (i.e. spaces, tabs, newlines, and backspaces).  The last character
 *  read into `line' is followed by NULL character.  The newline
 *  is NOT included in the string.  The IO pointer is set to the
 *  beginning of the next line (or to EOF if there is no next line. 
 *  Returns the `line' pointer or NULL upon EOF, or error.
 *
 *  This is exactly like fgets(3S) except that leading white-space
 *  is skipped, the newline is not kept, and we skip to the beginning
 *  of the next line in the file after a line is read.
 *
 *  If `maxline' == 1, then blanks will be skipped but no characters
 *  will be read (actually one non-white character will be read, and
 *  then will be pushed back onto the stream using ungetc(3S)).
 */

char *
getline(line,maxline,f)
char *line;
register int maxline;
register FILE *f;
{
	register int c;
	register char *p = line;

	/*
	 *  Check arguments.
	 */

	if(p == NULL || maxline < 0)
		return(NULL);

	/*
	 *  Skip leading
	 *  white spaces.
	 */

 	while((c = getc(f)) != EOF){
		if(!isspace(c)){
			*p++ = c;
			maxline--;
			break;
		}
	}

	/*
	 *  No non-trivial lines.
	 *  Empty line; return NULL.
	 */

	if(c == EOF){
		*line = EOS;
		return(NULL);
	}

	/*
	 *  Put back the read non-white
	 *  character.  Return empty line.
	 *  (wanted to skip blanks first).
	 */

	if(maxline == 0){
		ungetc(*p,f);
		*line = EOS;
		return(line);
	}

	/*
	 *  Already got one character;
	 *  now read maxline-2 characters
	 *  or up to a newline whichever
	 *  comes first.
	 */

	while(--maxline > 0 && (c = getc(f)) != EOF) {
		if (c == NEWLINE)
			break;
		*p++ = c;
	}

	*p = EOS;

	/*
	 *  If not already there, skip
	 *  to beginning of next line.
	 */

	if(c != NEWLINE)
		while((c = getc(f)) != EOF && c != NEWLINE)
				;
	return(line);
}
