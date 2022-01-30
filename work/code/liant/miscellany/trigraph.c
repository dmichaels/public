/* trigraph - Count ANSI-C trigraph sequences */

/* ---------------------------------------------------------------------- */
/* Include files
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <ctype.h>

/* ---------------------------------------------------------------------- */
/* Definitions
/* ---------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------- */
/* Macro functions
/* ---------------------------------------------------------------------- */

#define readc(f)	(getc(f))
#define unreadc(c,f)	(ungetc(c,f))

/* ---------------------------------------------------------------------- */
/* Static data
/* ---------------------------------------------------------------------- */

static char		*Program = (char *)NULL;

/* ---------------------------------------------------------------------- */
/* nextc
/*
/* Return the next character for preprocessing in the input stream.
/* Will map a trigraph sequence to the corresponding character.
/* ---------------------------------------------------------------------- */

static
int
nextc (f, is_trigraph)
register FILE *f;
int *is_trigraph;
{
	register int ca, cb, cc;

	*is_trigraph = 0;

	/* Check for a trigraph sequence */

	if (feof(f) || (ca = readc(f)) == EOF)
		return (EOF);

	if (ca == QUESTION) {
		if ((cb = readc(f)) == EOF)
			return (EOF);
		if (cb == QUESTION) {
			if ((cc = readc(f)) == EOF)
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
				      unreadc (cc, f);
				      break;
			}
		}
		unreadc (cb, f);
		return (ca);
	}
	return (ca);
}

/* ---------------------------------------------------------------------- */
/* trigraph_count
/*
/* Return the total number of ANSI-C trigraph sequences in the given file.
/* ---------------------------------------------------------------------- */

static
unsigned long
trigraph_count (f)
register FILE *f;
{
	int is_trigraph;
	register int c;
	register unsigned long count;

	for (count = 0L ; (c = nextc(f,&is_trigraph)) != EOF ; )
		if (is_trigraph)
			count++;
	return (count);
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
/* report
/* ---------------------------------------------------------------------- */

static
void
report (count, file)
unsigned long count;
char *file;
{
	printf ("ANSI-C Trigraphs: %4lu  %s\n", count, file);
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
	fprintf (stderr, "usage: %s [-t] [file]...\n", Program);
	fprintf (stderr, "       %s [-t] -f file_list\n", Program);
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
                if (((*av)[0] == DASH) && ((*av)[2] != EOS)) {
                        switch ((*av)[1]) {
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

	/* -------------------------------------------------
	 * Count trigraphs of files listed in the given file
	 * ----------------------------------------------- */

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
			count = trigraph_count(f);
			total += count;
			if (!totalonly)
				report (count, filename);
			fclose (f);
		}
		fclose (fl);
	}

	/* --------------------------------------------
	 * Count trigraphs read from the standard input
	 * ------------------------------------------ */

	else if (ac == 0) {
		count = trigraph_count(stdin);
		total += count;
		report (count, "(stdin)");
	}

	/* -------------------------------------------------------------
	 * Count trigraphs of file(s) given on the command line argument
	 * ----------------------------------------------------------- */

	else for ( ; ac > 0 ; av++, ac--) {
		if ((f = fopen(*av,"r")) == NULL) {
			fprintf (stderr,
			"%s: can't open \"%s\"\n", Program ,*av);
			continue;
		}
		count = trigraph_count(f);
		total += count;
		if (!totalonly)
			report (count, *av);
		fclose (f);
	}

	report (total, "Total");

	exit (0);
}
