/* lc - Line counting program */

/* ---------------------------------------------------------------------- */
/*
/* USAGE
/*
/*     lc [-b] [-t] [-asm|-c|-csh|-par] [file]...
/*     lc [-b] [-t] [-asm|-c|-csh|-par] -f file_list_file
/*
/* DESCRIPTION
/*
/*     This program will count the total number of lines in one or more
/*     files (or the standard input) optionally deleting blank lines, or
/*     comments.
/*
/*     If one or more file arguments are given then the lines of those files
/*     will be counted.
/*
/*     If no file arguments are specified then lines in the standard input
/*     will be counted.
/*
/*     If the "-f" option is given with a file name following it, then that
/*     file is assumed to contain a list of file names whose lines will be
/*     counted.
/*
/*     If the "-t" option is given then only the total line count of the
/*     lines in all the files speicified will be given rather than the
/*     default file by file count.
/*
/*     If the "-b" option is given then blank lines will not be counted.
/*
/*     If the "-asm" option is given then assembler style comments will
/*     be interpreted as a single NEWLINE.
/*     Comment format:  PIPE [comment] NEWLINE.
/*
/*     If the "-c" is given then C (and PL/I) style comments will be
/*     will be interpreted as a single SPACE.
/*     Comment format:  SLASH STAR [comment] STAR SLASH.
/*
/*     If the "-par" option is given then LPI parser/lexer generator
/*     language style comments will be interpreted as a single NEWLINE.
/*     Comment format:  DASH DASH [comment] NEWLINE.
/*
/*     If the "-csh" option is given then UNIX C-Shell style comments
/*     will be interpreted as a single NEWLINE.
/*     Comment format:  POUND [comment] NEWLINE.
/*
/* BUGS
/*
/*     There is no way to ignore multiple types of comments.
/*
/*     A start of comment indicator within a string-literal or
/*     character-constant is taken to be the start of a comment
/*     (this is not what is wanted for most languages); i.e. this
/*     is not a full lexer.
/*	
/* LPI EDIT HISTORY
/*
/*     05.12.88  DGM  000  Original.
/*
/* ---------------------------------------------------------------------- */

static char * _VERSION_ =

"@(#)lc.c  000  5/12/88  (c) 1990 Language Processors, Inc.";

/* ---------------------------------------------------------------------- */
/* Include files
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <ctype.h>

/* ---------------------------------------------------------------------- */
/* Definitions
/* ---------------------------------------------------------------------- */

#define DASH		'-'
#define EOS		'\0'
#define NEWLINE		'\n'
#define PIPE		'|'
#define POUND		'#'
#define SLASH		'/'
#define SPACE		' '
#define STAR		'*'

/* ---------------------------------------------------------------------- */
/* Macro functions
/* ---------------------------------------------------------------------- */

#define seq(a,b)	(strcmp((a),(b)) == 0)
#define readc(f)	(getc(f))
#define unreadc(c,f)	(ungetc(c,f))

/* ---------------------------------------------------------------------- */
/* Static variables
/* ---------------------------------------------------------------------- */

static int  IgnoreBlankLines = 0;
static int  IgnoreCommentLines = 0;
static int  StartCommentChar = EOS;
static int  (*SkipComment)() = (int (*)())NULL;
static int  ReplaceCommentChar = EOS;
static char *Program = NULL;

/* ---------------------------------------------------------------------- */
/* c_style_comment
/*
/* Skip past a C (or PL/I) type comment in the given file "f".
/* The comment syntax is as follows:
/*
/* Comment:  SLASH STAR [comment text] STAR SLASH
/*
/* The IO pointer upon entry to this routine is ASSUMED to be just
/* past a SLASH character.
/*
/* If the next readc(f) == STAR then this is a comment, and  we will read
/* until we find the closing STAR and SLASH; in this case a 1 will be
/* returned.  The next readc(f) will return the character immediately
/* following the terminating SLASH.  If the the next readc(f) != STAR
/* then this is NOT a comment.  Whatever character was just read is pushed
/* back onto the input stream and 0 is returned.  If we are indeed in a
/* comment and end-of-file is encountered before we find the end of the
/* comment then then EOF will be returned.
/*
/* Return:   0  ==>  No comment.
/*           1  ==>  Skipped a comment.
/*          -1  ==>  Hit end-of-file inside a comment!
/* ---------------------------------------------------------------------- */

static
int
c_style_comment(f)
register FILE *f;
{
	register int c;

	if ((c = readc(f)) == STAR) {
		for (c = readc(f) ; c != EOF ; ) {
			if (c != STAR)
				c = readc(f);
			else if ((c = readc(f)) == SLASH)
				return (1);
		}
		return (-1);
	}
	unreadc (c, f);
	return (0);
}

/* ---------------------------------------------------------------------- */
/* asm_style_comment
/*
/* Skip past a (typical) assembler style comment in the given file "f".
/* The comment syntax is as follows:
/*
/* Comment:  PIPE [comment text] NEWLINE
/*
/* The IO pointer upon entry to this routine is ASSUMED to be just past a
/* PIPE character.  We will read until we find the closing NEWLINE; a 1
/* will be returned.  The next readc(f) will return the character immediately
/* following the terminating NEWLINE.  If end-of-file is encountered before
/* we find the end of the comment then then -1 will be returned.
/*
/* Return:   1  ==>  Skipped a comment.
/*          -1  ==>  Hit end-of-file inside a comment!
/* ---------------------------------------------------------------------- */

static
int
asm_style_comment(f)
register FILE *f;
{
	register int c;

	while ((c = readc(f)) != EOF)
		if (c == NEWLINE)
			return (1);
	return (-1);
}

/* ---------------------------------------------------------------------- */
/* par_style_comment
/*
/* Skip past a typical LPI parser-generator language style comment
/* in the given file "f".  The comment syntax is as follows:
/*
/* Comment:  DASH DASH [comment text] NEWLINE
/*
/* The IO pointer upon entry to this routine is ASSUMED to be just
/* past a DASH character.
/*
/* If the next readc(f) == DASH then this is a comment, and  we will read
/* until we find the closing NEWLINE; in this case 1 will be returned.
/* The next readc(f) will return the character immediately following the
/* terminating NEWLINE.  If the the next readc(f) != DASH then this is NOT
/* a comment.  Whatever character was just read is pushed back onto the
/* input stream and 0 is returned.  If we are indeed in a comment and
/* end-of-file is encountered before we find the end of the comment then
/* -1 will be returned.
/*
/* Return:   0  ==>  No comment.
/*           1  ==>  Skipped a comment.
/*          -1  ==>  Hit end-of-file inside a comment!
/* ---------------------------------------------------------------------- */

static
int
par_style_comment(f)
register FILE *f;
{
	register int c;

	if ((c = readc(f)) == DASH) {
		while ((c = readc(f)) != EOF)
			if (c == NEWLINE)
				return (1);
		return (-1);
	}
	unreadc (c, f);
	return (0);
}

/* ---------------------------------------------------------------------- */
/* csh_style_comment
/*
/* Skip past a Unix C-Shell (or makefile) style comment in
/* the given file "f".  The comment syntax is as follows:
/*
/* Comment:  POUND [comment text] NEWLINE
/*
/* The IO pointer upon entry to this routine is ASSUMED to be just past a
/* POUND character.  We will read until we find the closing NEWLINE; a 1
/* will be returned.  The next readc(f) will return the character immediately
/* following the terminating NEWLINE.  If end-of-file is encountered before
/* we find the end of the comment then -1 will be returned.
/*
/* Return:   1  ==>  Skipped a comment.
/*          -1  ==>  Hit end-of-file inside a comment!
/* ---------------------------------------------------------------------- */

static
int
csh_style_comment(f)
register FILE *f;
{
	register int c;

	while ((c = readc(f)) != EOF)
		if (c == NEWLINE)
			return (1);
	return (-1);
}

/* ---------------------------------------------------------------------- */
/* nextchar
/*
/* Returns next character for preprocessing in the input stream.
/* Will skip past comments and return "ReplaceCommentChar" instead
/* if appropriate (i.e. if IgnoreCommentLines is set).
/* ---------------------------------------------------------------------- */

static
int
nextchar (f)
register FILE *f;
{
	register int c, status;

	if ((c = readc(f)) == EOF)
		return (EOF);
	if (!IgnoreCommentLines)
		return (c);
	if (c == StartCommentChar) {
		if ((status = (*SkipComment)(f)) == -1)
			return (EOF);
		if (status == 1)
			return (ReplaceCommentChar);
	}
	return (c);
}

/* ---------------------------------------------------------------------- */
/* linecount
/*
/* Return the total number of lines in the given file while
/* ignoring blank lines and/or comment lines if it's appropriate
/* (i.e. if IgnoreBlankLines or IgnoreCommentLines is set respectively).
/* ---------------------------------------------------------------------- */

static
unsigned long
linecount (f)
register FILE *f;
{
	extern int nextchar ();
	register unsigned long nlines;
	register int c, non_space_on_this_line = 0;

	for (nlines = 0 ; !feof(f) && (c = nextchar(f)) != EOF ; ) {
		if (c == NEWLINE) {
			if (!IgnoreBlankLines || non_space_on_this_line)
				nlines++;
			non_space_on_this_line = 0;
		}
		else if (!isspace(c))
			non_space_on_this_line = 1;
	}
	return (nlines);
}

/* ---------------------------------------------------------------------- */
/* getline
/*
/* Reads MAXLENGTH characters or up to a NEWLINE whichever comes first,
/* from the given file "f", and returns the line in static storage which
/* is overwritten with each call to this function.  Leading and trailing
/* blanks (which include the trailing NEWLINE) will NOT be included in the
/* line string.  The file IO pointer is set to the beginning of the next
/* line (or to EOF if there is no next line).  Returns NULL upon EOF.
/* (MAXLENGTH must be greater than 1 for this to work properly).
/* ---------------------------------------------------------------------- */

#define MAXLENGTH	512

static
char *
getline (f)
register FILE *f;
{
        static char linebuffer [MAXLENGTH + 1];
        register int c, nchars = 0;
        register char *p = linebuffer;

        while (!feof(f) && ((c = readc(f)) != EOF))
                if (!isspace(c)) {
                        *p++ = c, nchars++;
			break;
		}
        if (c == EOF)
                return (NULL);
        for ( ; (nchars < MAXLENGTH) && ((c = readc(f)) != EOF) ; nchars++) {
                if (c == NEWLINE)
                        break;
                *p++ = c;
        }
        for (*p = EOS ; (c != NEWLINE) && ((c = readc(f)) != EOF) ; )
                ;
        return (linebuffer);
}

/* ---------------------------------------------------------------------- */
/* usage
/*
/* Print correct program usage and die.
/* ---------------------------------------------------------------------- */

static
void
usage ()
{
	fprintf (stderr,
	"usage: %s [-b] [-t] [-asm|-c|-csh|-par] [file]...\n",
	Program);
	fprintf (stderr,
	"       %s [-b] [-t] [-asm|-c|-csh|-par] -f file_list\n",
	Program);
	exit (1);
}

/* ---------------------------------------------------------------------- */
/* main
/* ---------------------------------------------------------------------- */

main (argc, argv)
int argc;
char *argv[];
{
	register int	ac;
	register char	**av;
	FILE		*f, *fl;
	char		*filename, *filelist;
	unsigned long	count, total;
	int		totalonly;

	/* ----------
	 * Initialize
	 * -------- */

	IgnoreBlankLines = 0;
	IgnoreCommentLines = 0;

	totalonly = 0;
	total = 0L;
	filelist = NULL;

	/* ----------------------------------
	 * Examine the command line arguments
	 * -------------------------------- */

	Program = argv[0];
	ac = argc;
	av = argv;

        for (ac--, av++ ; ac > 0 ; ac--, av++) {
                if ((*av)[0] == DASH) {
			if (seq(&((*av)[1]), "asm")) {
				if (SkipComment != (int (*)())NULL)
					usage ();
				IgnoreCommentLines = 1;
				StartCommentChar = PIPE;
				SkipComment = asm_style_comment;
				ReplaceCommentChar = NEWLINE;
				continue;
			}
			if (seq(&((*av)[1]), "par")) {
				if (SkipComment != (int (*)())NULL)
					usage ();
				IgnoreCommentLines = 1;
				StartCommentChar = DASH;
				SkipComment = par_style_comment;
				ReplaceCommentChar = NEWLINE;
				continue;
			}
			if (seq(&((*av)[1]), "csh")) {
				if (SkipComment != (int (*)())NULL)
					usage ();
				IgnoreCommentLines = 1;
				StartCommentChar = POUND;
				SkipComment = csh_style_comment;
				ReplaceCommentChar = NEWLINE;
				continue;
			}
			if ((*av)[2] != EOS)
				usage ();
                        switch ((*av)[1]) {
                        case 'c':
				if (SkipComment != (int (*)())NULL)
					usage ();
				IgnoreCommentLines = 1;
				StartCommentChar = SLASH;
				SkipComment = c_style_comment;
				ReplaceCommentChar = SPACE;
				break;
                        case 'b':
				IgnoreBlankLines = 1;
				break;
                        case 't':
				totalonly = 1;
				break;
			case 'f':
				if (ac > 1)
					ac--, av++, filelist = *av;
				else	usage ();
				break;
                        default:
				fprintf (stderr,
				"%s: bad flag (%s)\n", Program, *av);
                                usage ();
                        }
			continue;
                }
                break;
        }

	/* ---------------------------------------------
	 * Count lines of files listed in the given file
	 * ------------------------------------------- */

	if (filelist != NULL) {
		if (ac > 0)
			usage ();
		if ((fl = fopen(filelist,"r")) == NULL) {
			fprintf (stderr,
			"%s: can't open \"%s\"\n", Program, filelist);
			usage ();
		}
		while ((filename = getline(fl)) != NULL) {
			if ((f = fopen(filename,"r")) == NULL) {
				fprintf (stderr,
				"%s: can't open \"%s\"\n", Program, filename);
				continue;
			}
			count = linecount(f);
			total += count;
			if (!totalonly)
				printf ("%10lu  %s\n", count, filename);
			fclose (f);
		}
		fclose (fl);
	}

	/* ----------------------------------------
	 * Count lines read from the standard input
	 * -------------------------------------- */

	else if (ac == 0) {
		count = linecount(stdin);
		total += count;
		printf ("%10lu  %s\n", count, "(stdin)");
	}

	/* ---------------------------------------------------------
	 * Count lines of file(s) given on the command line argument
	 * ------------------------------------------------------- */

	else for ( ; ac > 0 ; av++, ac--) {
		if ((f = fopen(*av,"r")) == NULL) {
			fprintf (stderr,
			"%s: can't open \"%s\"\n", Program ,*av);
			continue;
		}
		count = linecount(f);
		total += count;
		if (!totalonly)
			printf ("%10lu  %s\n", count, *av);
		fclose (f);
	}

	printf ("%10lu  %s\n", total, "Total");

	exit (0);
}
