/* trigraph - Count ANSI C trigraph sequences */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/***********************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  01.09.89  DGM  Original.
/*
/**********************************************************************/

/***********************************************************************
/*
/*  USAGE
/*
/*	trigraph [-t] [-p] [-f file_list] [file]...
/*
/*  DESCRIPTION
/*
/*  This program counts ANSI C trigraphs sequences in the named
/*  file(s) or standard input and reports the total number found.
/*
/*  If the "-t" option is given, then only the grand total will be
/*  given rather than the total for each file.
/*
/*  If the "-f" option is given, then the named file will be assumed
/*  to be a file containing a list of file names (one to a line) which
/*  will the object of the trigraph count.
/*
/*  If the "-p" option is given, then the named file(s) will be printed
/*  to the standard output with trigraphs translated.
/*
/**********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <ctype.h>

/* ---------------------------------------------------------------------
/* Macro definitions
/* ------------------------------------------------------------------- */

#define PROGRAM_NAME	trigraph
#define __PROGRAM__	STR(PROGRAM_NAME)

#define NXSTR(s)	#s	  /* Non-macro-expanding stringize */
#define STR(s)		NXSTR(s)  /* Macro-expanding stringize */

#define BANG		'!'
#define BACKSLASH	'\\'
#define CARAT		'^'
#define DASH		'-'
#define EOS		'\0'
#define EQUALS		'='
#define LANGLE		'<'
#define LCURLY		'{'
#define LPAREN 		'('
#define LSQUARE		'['
#define NEWLINE		'\n'
#define PIPE		'|'
#define POUND		'#'
#define QUESTION	'?'
#define RANGLE		'>'
#define RCURLY		'}'
#define RPAREN		')'
#define RSQUARE		']'
#define SQUOTE		'\''
#define SLASH		'/'
#define TILDE		'~'

/* ---------------------------------------------------------------------
/* Macro functions definitions
/* ------------------------------------------------------------------- */

#define GETC(f)		(getc(f))
#define UNGETC(c,f)	(ungetc(c,f))

/* ---------------------------------------------------------------------
/* Internal static data definitions
/* ------------------------------------------------------------------- */

static int	TotalOnly	= 0;		/* -t */
static int	PrintFile	= 0;		/* -p */
static char *	FileList	= NULL;		/* -f */

/* ---------------------------------------------------------------------
/* Internal function declarations
/* ------------------------------------------------------------------- */

static char		* getline		(register FILE *);
static void		  report		(unsigned long, char *);
static void		  usage			(void);
static unsigned long	  trigraph_count	(register FILE *f);
static int		  nextc			(register FILE *, int *);

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (int argc, char *argv[])
{
	register int	ac;
	register char	**av;
	FILE		*f, *fl;
	char		*filename;
	unsigned long	count, total = 0L;

	/* --------------------------------
	 * Parse the command line arguments
	 * ------------------------------ */

	ac = argc;
	av = argv;

        for (ac--, av++ ; ac > 0 ; ac--, av++) {
                if (((*av)[0] == DASH) && ((*av)[2] == EOS)) {
                        switch ((*av)[1]) {
                        case 'p':
				PrintFile = 1;
				break;
                        case 't':
				TotalOnly = 1;
				break;
			case 'f':
				if (ac > 1)
					ac--, av++, FileList = *av;
				else	usage ();
				break;
                        default:
				fprintf (stderr,
				"%s: bad flag (%s)\n", __PROGRAM__, *av);
                                usage ();
                        }
			continue;
                }
                break;
        }

	/* -------------------------------------------------
	 * Count trigraphs of files listed in the given file
	 * ----------------------------------------------- */

	if (FileList != NULL) {
		if (ac > 0)
			usage ();
		if ((fl = fopen (FileList,"r")) == NULL) {
			fprintf (stderr,
			"%s: can't open \"%s\"\n", __PROGRAM__, FileList);
			usage ();
		}
		while ((filename = getline (fl)) != NULL) {
			if ((f = fopen (filename,"r")) == NULL) {
				fprintf (stderr,
				"%s: can't open \"%s\"\n",
				__PROGRAM__, filename);
				continue;
			}
			count = trigraph_count (f);
			total += count;
			if (!TotalOnly)
				report (count, filename);
			fclose (f);
		}
		fclose (fl);
	}

	/* --------------------------------------------
	 * Count trigraphs read from the standard input
	 * ------------------------------------------ */

	else if (ac == 0) {
		count = trigraph_count (stdin);
		total += count;
		report (count, "(stdin)");
	}

	/* -------------------------------------------------------------
	 * Count trigraphs of file(s) given on the command line argument
	 * ----------------------------------------------------------- */

	else for ( ; ac > 0 ; av++, ac--) {
		if ((f = fopen (*av,"r")) == NULL) {
			fprintf (stderr,
			"%s: can't open \"%s\"\n", __PROGRAM__ ,*av);
			continue;
		}
		count = trigraph_count (f);
		total += count;
		if (!TotalOnly)
			report (count, *av);
		fclose (f);
	}

	report (total, "Total");

	exit (0);
}

/* ---------------------------------------------------------------------
/* usage
/*
/* Print correct program usage and die.
/* ------------------------------------------------------------------- */

static
void
usage (void)
{
	fprintf (stderr,

	"usage: " __PROGRAM__ " [-t] [-p] [file]...\n"
	"       " __PROGRAM__ " [-t] [-p] -f file_list\n"

	);

	exit (1);
}

/* ---------------------------------------------------------------------
/* report
/* ------------------------------------------------------------------- */

static
void
report (unsigned long count, char *file)
{
	printf ("*** ANSI C Trigraphs: %4lu  %s\n", count, file);
}

/* ---------------------------------------------------------------------
/* trigraph_count
/*
/* Return the total number of ANSI C trigraph sequences in the given file.
/* ------------------------------------------------------------------- */

static
unsigned long
trigraph_count (register FILE *f)
{
	int is_trigraph;
	register int c;
	register unsigned long count;

	for (count = 0L ; (c = nextc (f, &is_trigraph)) != EOF ; ) {
		if (is_trigraph)
			count++;
		if (PrintFile)
			putc (c, stdout);
	}
	return (count);
}

/* ---------------------------------------------------------------------
/* getline
/*
/* Reads MAXLENGTH characters or up to a NEWLINE whichever comes first,
/* from the given file "f", and returns the line in static storage which
/* is overwritten with each call to this function.  Leading and trailing
/* blanks (which include the trailing NEWLINE) will NOT be included in the
/* line string.  The file IO pointer is set to the beginning of the next
/* line (or to EOF if there is no next line).  Returns NULL upon EOF.
/* (MAXLENGTH must be greater than 1 for this to work properly).
/* ------------------------------------------------------------------- */

#define MAXLENGTH	512

static
char *
getline (register FILE *f)
{
        static char linebuffer [MAXLENGTH + 1];
        register int c, nchars = 0;
        register char *p = linebuffer;

        while (!feof(f) && ((c = GETC (f)) != EOF))
                if (!isspace(c)) {
                        *p++ = c, nchars++;
			break;
		}
        if (c == EOF)
                return (NULL);
        for ( ; (nchars < MAXLENGTH) && ((c = GETC (f)) != EOF) ; nchars++) {
                if (c == NEWLINE)
                        break;
                *p++ = c;
        }
        for (*p = EOS ; (c != NEWLINE) && ((c = GETC (f)) != EOF) ; )
                ;
        return (linebuffer);
}

/* ---------------------------------------------------------------------
/* nextc
/*
/* Return the next character for preprocessing in the input stream.
/* Will map a trigraph sequence to the corresponding character.
/* ------------------------------------------------------------------- */

static
int
nextc (register FILE *f, int *is_trigraph)
{
	register int ca, cb, cc;

	*is_trigraph = 0;

	/* Check for a trigraph sequence */

	if (feof(f) || (ca = GETC (f)) == EOF)
		return (EOF);

	if (ca == QUESTION) {
		if ((cb = GETC (f)) == EOF)
			return (EOF);
		if (cb == QUESTION) {
			if ((cc = GETC (f)) == EOF)
				return (EOF);
			*is_trigraph = 1;
			switch (cc) {
			case EQUALS : return (POUND);	   /* ??= : # */
			case LPAREN : return (LSQUARE);	   /* ??( : [ */
			case RPAREN : return (RSQUARE);	   /* ??) : ] */
			case LANGLE : return (LCURLY);	   /* ??< : { */
			case RANGLE : return (RCURLY);	   /* ??> : } */
			case SQUOTE : return (CARAT);	   /* ??' : ^ */
			case BANG   : return (PIPE);	   /* ??! : | */
			case DASH   : return (TILDE);	   /* ??- : ~ */
			case SLASH  : return (BACKSLASH);  /* ??/ : \ */
			default     : *is_trigraph = 0;
				      UNGETC (cc, f);
				      break;
			}
		}
		UNGETC (cb, f);
		return (ca);
	}
	return (ca);
}

