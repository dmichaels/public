/* mega - Output random Massachusetts MegaBucks Lottery selections */

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
/*  02.12.89  DGM  Original.
/*
/**********************************************************************/

/***********************************************************************
/*
/*  USAGE
/*
/*	mega [number_of_selections]
/*	mega -all
/*
/*  DESCRIPTION
/*
/*	Prints to standard output, six numbers selected at random
/*	between 1 and 36 (inclusive).  If a number arguement is
/*	given, then that many selections will be printed (one
/*	per line).  If the "-all" option is given, then all of the
/*	36! / (6! * (36 - 6)!) = 1,947,792  possible combinations
/*	will be printed.
/*
/**********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------------------------------------------------------------------
/* Type definitions
/* ------------------------------------------------------------------- */

typedef int		boolean;
typedef unsigned int	uint;
typedef unsigned long	ulong;

/* ---------------------------------------------------------------------
/* Macro definitions
/* ------------------------------------------------------------------- */

#define PROGRAM_NAME	mega
#define __PROGRAM__	STR(PROGRAM_NAME)

#define NXSTR(s)	#s		/* Stringize (no argument expansion) */
#define STR(s)		NXSTR(s)	/* Stringize (argument expanded) */

#define FALSE		((boolean)0)
#define TRUE		((boolean)(!FALSE))

#define POSSIBILITIES	36
#define SELECTIONS	6

/* ---------------------------------------------------------------------
/* Function declarations
/* ------------------------------------------------------------------- */

static void	pr_select	(const boolean [], uint);
static void	select		(boolean [], uint , uint);
static ulong	pr_comb_norep	(FILE *, int, int);
static void	usage		(void);

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (int argc, char *argv[])
{
	boolean	list [POSSIBILITIES + 1];
	uint	count;

	if (argc > 2)
		usage();

	if (argc == 1)
		count = 1;

	else if (strcmp (argv[1], "-all") == 0) {
		pr_comb_norep (stdout, POSSIBILITIES, SELECTIONS);
		exit (0);
	}

	else if ((count = atoi(argv[1])) <= 0)
		usage ();

	while (count-- > 0) {
		select (list, (uint)SELECTIONS, (uint)POSSIBILITIES);
		pr_select (list, (uint)POSSIBILITIES);
	}
}

/* ---------------------------------------------------------------------
/* pr_select
/*
/* Print to the standard output, the index number of each of
/* the "possibilities" slots in the "list" array which is set
/* to TRUE starting at index number 1 (the 0th slot is wasted).
/* The "list" array is assumed to have "possiblities" + 1 slots
/* allocated to for its use.
/* ------------------------------------------------------------------- */

static
void
pr_select (register const boolean list[], register uint possibilities)
{
	register uint	n, nums;

	for (n = 1 ; n <= possibilities ; n++)
		if (list[n] == TRUE)
			printf ("%4d", n);
	putchar ('\n');
}

/* ---------------------------------------------------------------------
/* select
/*
/* Select at random, "selections" number in the range 1 through
/* "possibilities", and set the corresponding slots in the "list"
/* array to TRUE, set all the others to FALSE.  For simplicity, the
/* "list" array is to be 1 indexed, and thus the 0th slot will be
/* wasted, and the "list" array must have "possibilities" + 1 slots
/* allocated for its use.
/* ------------------------------------------------------------------- */

static
void
select (register boolean list [],
	register uint selections, register uint possibilities)
{
	static long	k = 3;
	register int	nums = 0, r;

	/* Initialize the list */

	for (r = 1 ; r <= possibilities ; r++)
		list[r] = FALSE;

	/* Initialize random number generator */

	srand ((uint)(time ((time_t *)0) * k++));

	/* Select the numbers */

	while (nums < selections) {
		r = rand () % possibilities;
		if (list[r + 1] == TRUE)
			continue;
		list[r + 1] = TRUE;
		nums++;
	}
}

/* ---------------------------------------------------------------------
/*  pr_comb_norep
/*
/* Print to the given file "f", all of combinations of "n" items
/* (numbers 1 through "n") taken "r" at a time with no repetitions
/* (one set per line).  For example, pr_comb_norep(f,3,2) gives:
/*
/*			1   2
/*			1   3
/*			2   3
/*
/* Return the number of combinations printed, or -1 if error.
/* ------------------------------------------------------------------- */

static
ulong
pr_comb_norep (FILE *f, register int n, register int r)
{
	register int	*a, i, j, k;
	register int	max, n_minus_r_plus_1, r_minus_1;
	register ulong	ncomb;

	if ((a = (int *) malloc ((size_t)(r * sizeof (int)))) == NULL)
		return (-1);

	for (i = 1 ; i <= r ; i++)
		a[i-1] = i;

	/*  The following three are done for efficiency */

	max = n - (r - 1);
	n_minus_r_plus_1 = n - r + 1;
	r_minus_1 = r - 1;

	ncomb = 0;

	while (1) {
		for (i = 0 ; i < r ; i++)
			fprintf (f, "%4d", a[i]);
		fputc ('\n',f);
		ncomb++;
		if (*a == max)
			break;
		for (i = r_minus_1 ; i >= 0 ; i--) {
			if (a[i] == (n_minus_r_plus_1 + i))
				continue;
			k = ++a[i];
			for (j = i + 1 ; j < r ; j++)
				a[j] = ++k;
			break;
		}
	}
	return (ncomb);
}

/* ---------------------------------------------------------------------
/* usage
/* ------------------------------------------------------------------- */

static
void
usage (void)
{
	fprintf (stderr,
	
	"usage: " __PROGRAM__ " [number_of_selections]\n"
	"       " __PROGRAM__ " -all\n"
	
	);

	exit (1);
}

