/***********************************************************************
* PROGRAM:	Roman Numeral Conversion Program
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		December 1983
***********************************************************************/

/*
 *  NAME
 *	roman - convert between decimal and roman numbers
 *
 *  SYNOPSIS
 *	roman number...
 *	roman lo_number - hi_number
 *	roman roman_numeral...
 *	roman lo_roman_numeral - hi_roman_numeral
 *
 *  DESCRIPTION
 *	Converts between roman numerals and normal arabic
 *	numbers. The range of nubers must be 1..9999.
 *	Self-explanatory.
 *
 *  BUGS
 *	Roman numerals such as iiiiiiiiii will be accepted
 *	and translated accordingly (i.e. in this case, the
 *	roman numeral will be translated into the number 10).
 *
 *  HISTORY
 *
 *	??.??.84  DGM  Original at MIT Lincoln Laboratory.
 *		       David G. Michaels.
 *
 *	01.20.88  DGM  Updated at LPI.
 */

/***********************************************************************
 * INCLUDE FILES
 **********************************************************************/

#include <stdio.h>
#include <ctype.h>

/***********************************************************************
 * DEFINITONS
 **********************************************************************/

#define EOS		'\0'		/* end of string marker */
#define FALSE		(0)		/* boolean false */
#define LIMIT		9999		/* highest roman number */
#define MAXROMAN	30		/* max length of a roman numeral */
#define NMAPS		13		/* number of mappings below */
#define TRUE		(!FALSE)	/* boolean true */

typedef int		bool_t;

/***********************************************************************
 * DATA STRUCTURES
 **********************************************************************/

struct map_tbl {
	unsigned int	 m_decimal;
	char		*m_roman;
};

/***********************************************************************
 * GLOBAL DATA
 **********************************************************************/

struct map_tbl Map[NMAPS] =
{
	 1000	,"m"
	,900	,"cm"
	,500	,"d"
	,400	,"cd"
	,100	,"c"
	,90	,"xc"
	,50	,"l"
	,40	,"xl"
	,10	,"x"
	,9	,"ix"
	,5	,"v"
	,4	,"iv"
	,1	,"i"
};

char	*Program;	/* Name of this program (av[0]) */

/***********************************************************************
 *  FUNCTIONS
 **********************************************************************/

main (argc,argv)
int	 argc;
char	*argv[];
{
	void			usage ();
	char			*toroman ();
	unsigned int		todecimal (), checknum ();
	register unsigned int	i, lo, hi;
	char			**av;
	int			ac;

	ac = argc;
	av = argv;

	Program = *av;

	if (ac < 2)
		usage ();

	if (isint(av[1]))
		goto Romanize;

	/* ----------------
	 * ROMAN TO DECIMAL
	 * -------------- */

	/* -----------------------------------------------
	 * Case: roman lo_roman_numeral - hi_roman_numeral
	 * --------------------------------------------- */

	if ((ac == 4) && (strcmp(av[2],"-") == 0)) {
		char *r;
		if ((lo = todecimal(av[1])) == 0)
			fprintf (stderr, "%s: %s malformed\n", Program, av[1]);
		if ((hi = todecimal(av[3])) == 0)
			fprintf (stderr, "%s: %s malformed\n", Program, av[3]);
		for ( ; lo <= hi ; lo++) {
			r = toroman(lo);
			printf ("%s %u\n", r, todecimal(r));
		}
	}

	/* ----------------------------
	 * Case: roman roman_numeral...
	 * -------------------------- */

	else {
		for (i = 1 ; i < ac ; i++)
			if ((lo = todecimal(av[i])) == 0)
				fprintf(stderr,
				"%s: %s malformed\n", Program, av[i]);
			else	printf ("%s %u\n", av[i], lo);
	}

	exit (0);

Romanize:

	/* ----------------
	 * DECIMAL TO ROMAN
	 * -------------- */

	/* ---------------------------------
	 * Case: roman lo_number - hi_number
	 * ------------------------------- */

	if ((ac == 4) && (strcmp(av[2],"-") == 0)) {
		lo = checknum(av[1]);
		hi = checknum(av[3]);
		if (lo > hi)
			usage();
		for ( ; lo <= hi ; lo++)
			printf ("%u %s\n", lo, toroman(lo));
	}

	/* ---------------------
	 * Case: roman number...
	 * ------------------- */

	else {
		for (i = 1 ; i < ac ; i++) {
			lo = checknum(av[i]);
			printf ("%u %s\n", lo, toroman(lo));
		}
	}

	exit (0);
}

/* ---------------------------------------------------------------------
 * toroman
 *
 * Given a decimal number, return the equivalent roman numeral string.
 */

char *
toroman (decimal)
register int decimal;
{
	static char	roman[MAXROMAN];	
	register int	i;

	roman[0] = EOS;
	for (i = 0; i < NMAPS ; i++)
		while (decimal >= Map[i].m_decimal) {
			strcat (roman, Map[i].m_roman);
			decimal -= Map[i].m_decimal;
		}
	return (roman);
}

/* ---------------------------------------------------------------------
 * todecimal
 *
 * Given a roman numeral string, return the equivalent unsigned integer.
 * Return zero if the roman numeral is malformed.
 */

unsigned int
todecimal (roman)
register char *roman;
{
	register unsigned int	decimal = 0;
	register unsigned int	i, len;

	for (i = 0 ; i < NMAPS ; i++) {
		len = strlen(Map[i].m_roman);
		while ((*roman != EOS) &&
		       (strncmp(roman, Map[i].m_roman, len) == 0)) {
			decimal += Map[i].m_decimal;
			roman += len;
			if (len == 2)	/* prefix modifier */
				break;
		}
	}

	if (*roman != EOS)
		return (0);

	return (decimal);
}

/* ---------------------------------------------------------------------
 * checknum
 *
 * See if the number string is ok to translate to a roman numeral,
 * and return its decimal value if so.  Give usage if not.
 */

unsigned int
checknum (s)
register char *s;
{
	int n;

	if (((n = atoi(s)) < 1) || (n > LIMIT))
		usage ();
	return (n);
}

/* ---------------------------------------------------------------------
 * isint
 *
 * Return TRUE if the given string consists entirely of digits (0-9),
 * otherwise return FALSE.
 */

bool_t
isint (s)
register char *s;
{
	for ( ; *s ; s++)
		if (!isdigit(*s))
			return (FALSE);
	return (TRUE);
}

void
usage()
{

  fprintf (stderr, "usage: %s number...\n", Program);
  fprintf (stderr, "       %s lo_number - hi_number\n", Program);
  fprintf (stderr, "       %s roman_numeral...\n", Program);
  fprintf (stderr, "       %s lo_roman_numeral - hi_roman_numeral\n", Program);
  fprintf (stderr, "       (Numbers must be in the range 1..%d)\n", LIMIT);
  exit (1);

}
