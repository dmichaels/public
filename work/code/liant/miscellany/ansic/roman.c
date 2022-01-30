/* roman - Roman numeral conversion program */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/************************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  01.09.89  DGM  Original.
/*
/***********************************************************************/

/***********************************************************************
/*
/*  USAGE
/*
/*	roman
/*	roman roman_number...
/*	roman lo_roman_number - hi_roman_number
/*	roman decimal_number...
/*	roman lo_decimal_number - hi_decimal_number
/*
/*  DESCRIPTION
/*
/*	Converts normal arabic decimal numerals to roman numerals.
/*	The range of nubers must be 1..9999.
/*
/***********************************************************************/

/* ----------------------------------------------------------------------
/* Include files
/* -------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ----------------------------------------------------------------------
/* Macro definitions
/* -------------------------------------------------------------------- */

#define PROGRAM_NAME	roman
#define __PROGRAM__	STR(PROGRAM_NAME)

#define NXSTR(s)	#s		/* Stringize (no argument expansion) */
#define STR(s)		NXSTR(s)	/* Stringize (argument expanded) */

#define EOS		'\0'		/* End of string marker */

#define MAX_ROMAN_NUMERAL	9999	/* highest roman number */
#define MAX_ROMAN_LENGTH	64	/* max length of a roman numeral */
#define SIZE_ROMAN_MAP_TABLE	13	/* number of mappings below */

/* ----------------------------------------------------------------------
/* Data type definitions
/* -------------------------------------------------------------------- */

typedef struct {
	int		 decimal;
	char		*roman;
} roman_map;

/* ----------------------------------------------------------------------
/* Static data
/* -------------------------------------------------------------------- */

static const roman_map RomanMap [SIZE_ROMAN_MAP_TABLE] =
{
	  { 1000 , "M"  }
	, {  900 , "CM" }
	, {  500 , "D"  }
	, {  400 , "CD" }
	, {  100 , "C"  }
	, {   90 , "XC" }
	, {   50 , "L"  }
	, {   40 , "XL" }
	, {   10 , "X"  }
	, {    9 , "IX" }
	, {    5 , "V"  }
	, {    4 , "IV" }
	, {    1 , "I"  }
};

/* ----------------------------------------------------------------------
/* Function declarations
/* -------------------------------------------------------------------- */

static void	  interactive	    (void);
static int	  string_to_decimal (const char *);
static char	* decimal_to_roman  (long);
static int	  strncmp_no_case   (const char *, const char *, size_t);
static void	  greeting	    (void);
static void	  prompt	    (void);
static void	  usage		    (void);

/* ----------------------------------------------------------------------
/* main
/* -------------------------------------------------------------------- */

main (int argc, char *argv[])
{
	char		**av = argv;
	int		ac   = argc;
	register int	i;
	register long	lo, hi;

	/* --------------------------------------------------------------
	/* roman (interactive use)
	/* ------------------------------------------------------------ */

	if (ac < 2) {
		interactive ();
		exit (0);
	}

	if (string_to_decimal (av[1]))
		goto Romanize;

	/* --------------------------------------------------------------
	/* roman lo_roman_numeral - hi_roman_numeral
	/* ------------------------------------------------------------ */

	if ((ac == 4) && (strcmp(av[2],"-") == 0)) {
		char *r;
		if ((lo = roman_to_decimal (av[1])) == 0)
			fprintf (stderr,
			 "%s: %s malformed\n", __PROGRAM__, av[1]);
		if ((hi = roman_to_decimal (av[3])) == 0)
			fprintf (stderr,
			 "%s: %s malformed\n", __PROGRAM__, av[3]);
		for ( ; lo <= hi ; lo++) {
			r = decimal_to_roman (lo);
			printf ("%s %u\n", r, roman_to_decimal (r));
		}
		exit (0);
	}

	/* --------------------------------------------------------------
	/* roman roman_numeral...
	/* ------------------------------------------------------------ */

	else {
		for (i = 1 ; i < ac ; i++) {
			if ((lo = roman_to_decimal (av[i])) == 0)
				fprintf(stderr,
				"%s: %s malformed\n", __PROGRAM__, av[i]);
			else	printf ("%s %u\n", av[i], lo);
		}
		exit (0);
	}

	Romanize:

	/* --------------------------------------------------------------
	/* roman lo_decimal_number - hi_decimal_number
	/* ------------------------------------------------------------ */

	if ((ac == 4) && (strcmp(av[2],"-") == 0)) {
		if (((lo = string_to_decimal (av[1])) == 0) ||
		    ((hi = string_to_decimal (av[3])) == 0) ||
		    (lo > hi))
			usage ();
		for ( ; lo <= hi ; lo++)
			printf ("%5u %s\n", lo, decimal_to_roman (lo));
		exit (0);
	}

	/* --------------------------------------------------------------
	/* roman decimal_number...
	/* ------------------------------------------------------------ */

	else {
		for (i = 1 ; i < ac ; i++) {
			if ((lo = string_to_decimal (av[i])) == 0) {
				fprintf (stderr, __PROGRAM__
				": malformed number (%s)\n", av[i]);
				continue;
			}
			printf ("%5u %s\n", lo, decimal_to_roman (lo));
		}
		exit (0);
	}

	exit (1);
}

/* ----------------------------------------------------------------------
/* interactive
/* -------------------------------------------------------------------- */

static
void
interactive (void)
{
	char		s [512];
	register long	d;
	register int	i;

	greeting ();

	for (prompt () ; gets (s) != NULL ; prompt ()) {
		for (i = strlen(s) ; --i >= 0 ; )
			if (isspace (s[i])) s[i] = EOS;
		if (*s == EOS) continue;
		if ((d = string_to_decimal (s)))
			printf ("-----> %u %s\n", d, decimal_to_roman (d));
		else if ((d = roman_to_decimal (s)) != 0)
			printf ("-----> %s %u\n", s, roman_to_decimal (s));
		else	printf ("-----> Malformed number (%s).\n", s);
	}
}

/* ----------------------------------------------------------------------
/* greeting
/* -------------------------------------------------------------------- */

static
void
greeting (void)
{

printf (

"**************************************"
"**************************************\n"
"*  Program:  " __PROGRAM__ "\n"
"*  Compiled: " __DATE__ ", " __TIME__ "\n"
"*  Copyright (c) Language Processors, Inc. 1989\n"
"**************************************"
"**************************************\n"
"*  This program will convert back and forth between roman and decimal\n"
"*  numbers.  Enter a roman number and the equivalent decimal number will\n"
"*  printed.  Enter a decimal number and the equivalent roman number will\n"
"*  be printed (decimal numbers must be in the range 1.." STR(MAX_ROMAN_NUMERAL)
    ").\n"
"*\n"
"*  For example:  mmi\n"
"*  Would give:   2001\n"
"*\n"
"*  Enter <control-D> to terminate.\n"
"**************************************"
"**************************************\n"
	
);

}

/* ---------------------------------------------------------------------
/* prompt
/* ------------------------------------------------------------------- */

static
void
prompt (void)
{
	printf (__PROGRAM__ "> ");
}

/* ----------------------------------------------------------------------
/* decimal_to_roman
/*
/* Given a number, return the equivalent roman numeral string.
/* -------------------------------------------------------------------- */

static
char *
decimal_to_roman (long d)
{
	static char	r [MAX_ROMAN_LENGTH];	
	register int	i;

	r [0] = EOS;
	for (i = 0; i < SIZE_ROMAN_MAP_TABLE ; i++) {
		while (d >= RomanMap[i].decimal) {
			strcat (r, RomanMap[i].roman);
			d -= RomanMap[i].decimal;
		}
	}
	return (r);
}

/* ----------------------------------------------------------------------
/* string_to_decimal
/*
/* See if the given string "s" is a numeric string and if it is within
/* the range of values to be converted to a roman numeral, and return
/* its decimal value (greater than zero) if so, otherwise return 0.
/* -------------------------------------------------------------------- */

static
int
string_to_decimal (const char * s)
{
	long n;

	if (((n = atol (s)) < 1) || (n > MAX_ROMAN_NUMERAL))
		return  (0);
	return (n);
}

/* ----------------------------------------------------------------------
/* roman_to_decimal
/*
/* Given a string "roman" representing a roman numeral, return the
/* equivalent integer.  Return zero if the roman numeral is malformed.
/* -------------------------------------------------------------------- */

int
roman_to_decimal (rs)
register char *rs;
{
	register long	d = 0;
	register long	i, len;

	for (i = 0 ; i < SIZE_ROMAN_MAP_TABLE ; i++) {
		len = strlen (RomanMap[i].roman);
		while ((*rs != EOS) &&
		       (strncmp_no_case (rs, RomanMap[i].roman, len)) == 0) {
			d += RomanMap[i].decimal;
			rs += len;
			if (len == 2) break;  /* prefix modifier */
		}
	}
	return (*rs != EOS ? 0 : d);
}

/* ----------------------------------------------------------------------
/* strncmp_no_case
/*
/* Perform a case-insensitive "strncmp" comparison.
/* -------------------------------------------------------------------- */

static
int
strncmp_no_case (const char *a, const char *b, size_t n)
{
	char ca, cb;

	if (n == 0) return (0);

	do {	ca = *a++;
		cb = *b++;
		ca = toupper (ca);
		cb = toupper (cb);

	} while ((ca != EOS) && (ca == cb) && (--n > 0));
    
	return (((int)ca) - ((int)cb));
}

/* ----------------------------------------------------------------------
/* usage
/*
/* Print usage and exit
/* -------------------------------------------------------------------- */

static
void
usage (void)
{

	fprintf (stderr,
	
	"usage: " __PROGRAM__ "\n"
	"       " __PROGRAM__ " roman_number...\n"
	"       " __PROGRAM__ " lo_roman_number - hi_roman_number\n"
	"       " __PROGRAM__ " decimal_number...\n"
	"       " __PROGRAM__ " lo_decimal_number - hi_decimal_number\n"
	"       [Numbers must be in the range 1.." STR (MAX_ROMAN_NUMERAL) "]\n"

	);

	exit (1);
}

