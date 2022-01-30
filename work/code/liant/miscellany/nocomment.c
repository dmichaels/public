/* ---------------------------------------------------------------------- */
/* nocomment
/*
/* This program will copy standard input to standard output while
/* replacing each C or PL/I style comment with one space character.
/* ---------------------------------------------------------------------- */

#include <stdio.h>

#define SLASH		'/'
#define STAR		'*'
#define SPACE		' '
#define NEWLINE		'\n'

#define readc(f)	(getc(f))
#define unreadc(c,f)	(ungetc(c,f))

/* ---------------------------------------------------------------------- */
/* comment
/*
/* Skip past an C or PL/I type comment (i.e. SLASH-STAR/STAR-SLASH) in
/* a the given file "f".  The IO pointer upon entry to this routine is
/* ASSUMED to be just past a SLASH character.
/*
/* If the next readc(f) == STAR then this is a comment, and  we will read
/* until we find the closing STAR and SLASH; in this case a SPACE will be
/* returned (representing the comment).  The next readc(f) will return the
/* character immediately following the terminating SLASH.
/*
/* If the the next readc(f) != STAR then this is NOT a comment.
/* Whatever character was just read is returned.
/*
/* If we are indeed in a comment and end-of-file is encountered before we
/* find the end of the comment then then EOF will be returned.
/*
/* Return values:  SPACE ==> Skipped a comment.
/*                 SLASH ==> No comment.
/*                 EOF   ==> Hit end-of-file inside a comment!
/* ---------------------------------------------------------------------- */

static
int
comment(f)
register FILE *f;
{
	extern int readc();
	register int c;

	if ((c = readc(f)) == STAR) {
		for (c = readc(f) ; c != EOF ; ) {
			if (c != STAR)
				c = readc(f);
			else if ((c = readc(f)) == SLASH)
				return (SPACE);
		}
		return (EOF);
	}
	return (c);
}

/* ---------------------------------------------------------------------- */
/* nextchar
/*
/* Returns next character for preprocessing in the input stream.
/* Will skip past comments and return a space.
/* ---------------------------------------------------------------------- */

int
nextchar (f)
register FILE *f;
{
	register int ca, cb;

	if ((ca = readc(f)) == EOF)
		return (EOF);
	if (ca == SLASH) {
		if ((cb = comment(f)) == SPACE)
			return (SPACE);
		else if (cb == EOF)
			return (EOF);
		else	unreadc (cb, f);
	}
	return (ca);
}

/* ---------------------------------------------------------------------- */
/* main
/*
/* Copy standard input to standard output while replacing each C or PL/I
/* type comment with one space character.
/* ---------------------------------------------------------------------- */

main ()
{
	register int c;

	while ((c = nextchar(stdin)) != EOF)
		putchar (c);
	exit (0);
}
