/* ---------------------------------------------------------------------
 * PROGRAM:	Binary file copy program
 * AUTHOR:	David Michaels (david@ll-sst)
 * DATE:	November 1984
 * ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
 *  NAME
 *	bcat - raw binary file copy utility
 *
 *  SYNOPSIS
 *	bcat [-o offset] [-n nbytes] [-v] [file]...
 *
 *  DESCRIPTION
 *	Copies the given file(s), or the standard input if no files are
 *	given, to the standard output.  If the "-o" option is given,
 *	then copying will start at the given "offset" (rather than the
 *	default zero).  If the "-n" option is given, then only "nbytes"
 *	bytes will be copied rather than to the end of the file (the
 *	default).  Note that negative values for "offset" or "nbytes"
 *	will be taken as positive.  The "-v" (verbose) option will cause
 *	the "offset" and "nbytes" values to be printed to the standard
 *	error output. 	The flags apply to all file arguments.
 *
 *  AUTHOR
 *	David Michaels (david@ll-sst) November 1984
 * ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
 * Include files
 * ------------------------------------------------------------------- */

#include <stdio.h>

/* ---------------------------------------------------------------------
 * Local static data
 * ------------------------------------------------------------------- */

static char *Program = "bcat";

/* ---------------------------------------------------------------------
 * usage
 * ------------------------------------------------------------------- */

static
void
usage ()
{
	fprintf (stderr,
	"usage: %s [-o offset] [-n nbytes] [file]...\n", Program);
	exit (1);
}

/* ---------------------------------------------------------------------
 * main
 * ------------------------------------------------------------------- */

main (argc, argv)
int	argc;
char	*argv[];
{
	extern long		atol ();
	int			ac;
	char			**av;
	register FILE		*f;
	register int		c;
	register unsigned long	n;
	unsigned long		offset = 0L;
	unsigned long		nbytes = 0L;
	int			allbytes = 1;
	int			verbose = 0;

	for (ac = (argc - 1), av = (argv + 1) ; (ac > 0) ; ac--, av++) {
		if (((*av)[0] == '-') && ((*av)[2] == '\0')) {
			switch((*av)[1]) {
			case 'o':
				ac--; av++;
				offset = (unsigned long)atol(*av);
				break;
			case 'n':
				ac--; av++;
				allbytes = 0;
				nbytes = (unsigned long)atol(*av);
				break;
			case 'v':
				verbose = 1;
				break;
			default:
				usage ();
			}
			continue;
		}
		break;
	}

	if (verbose) {
		fprintf (stderr,
		"%s: offset = %lu nbytes = %lu\n", Program, offset, nbytes);
	}

	f = ac == 0 ? stdin : NULL;

	for ( ; (ac > 0) || (f == stdin) ; av++, ac--) {
		if ((f != stdin) && (f = fopen(*av,"r")) == NULL) {
			fprintf (stderr,
			"%s: can't open \"%s\"\n", Program, *av);
			continue;
		}
		if (offset > 0L)
			fseek (f, offset, 0);
		if (allbytes) {
			while ((c = getc(f)) != EOF)
				putchar (c);
		}
		else {
			n = nbytes;
			while (((c = getc(f)) != EOF) && (n != 0L)) {
				putchar (c);
				n--;
			}
		}
	}
	exit (0);
}
