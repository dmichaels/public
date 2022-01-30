/* prime - Print the first N prime numbers */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/***********************************************************************
/*
/*  USAGE
/*
/*	prime number
/*
/*  DESCRIPTION
/*
/*	Print to the standard output the first "number" primes
/*	(up to some unknown reasonable number).
/*
/**********************************************************************/

/***********************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  06.01.89  DGM  Updated at LPI.
/*  02.xx.84  DGM  Original at Lincoln Laboratory.
/*
/**********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>

/* ---------------------------------------------------------------------
/* Type definitions
/* ------------------------------------------------------------------- */

typedef int	boolean;
typedef int	integer;

/* ---------------------------------------------------------------------
/* Definitions
/* ------------------------------------------------------------------- */

#define TRUE	((boolean)1)
#define FALSE	((boolean)0)

/* ---------------------------------------------------------------------
/* Internal function declarations
/* ------------------------------------------------------------------- */

static integer	ipow	(integer, integer);
static void	primes	(integer *, integer);

/* ------------------------------------------------------------------------
 * main
 * ---------------------------------------------------------------------- */

void
main (int argc, char *argv[])
{
	long	i, n;
	integer	*p;

	if (argc != 2){
		fprintf (stderr, "usage: prime number\n");
		exit (1);
	}
	if ((n = atol (argv[1])) <= 0){
		fprintf (stderr, "prime: bad number argument - %d\n", argv[1]);
		exit (1);
	}
	if ((p = (int *) malloc (n + 5)) == NULL){
		fprintf (stderr, "prime: memory shortage for %d primes\n", n);
		exit (1);
	}
	primes (p, (integer)n);
	for (i = 0 ; i < n ; i++)
		printf ("%d\n", *p++);
}

/* ------------------------------------------------------------------------
 * primes
 * ---------------------------------------------------------------------- */

static
void
primes (integer *p, integer np)
{
	register integer j, k, n, inc, ord;
	register boolean j_is_prime;

	p [0]	= 2L;
	p [1]	= 3L;
	k	= 2L;
	j	= 1L;
	inc	= 4L;
	ord	= 0L;

	while (k < np) {
		do {
			j += inc;
			inc = 6 - inc;
			while (ipow ((integer)p[ord],(integer)2) <= j)
				ord++;
			n = 2;
			j_is_prime = TRUE;
			while ((n < ord) && (j_is_prime == TRUE)) {
				j_is_prime = (((j % p[n]) != 0) ? TRUE : FALSE);
				n++;
			}
		} while (j_is_prime == FALSE);
		p[k] = j;
		k++;
	}
}

/* ------------------------------------------------------------------------
 * ipow
 * ---------------------------------------------------------------------- */

static
integer
ipow (register integer x, register integer y)
{
	register integer z;

	if (y == 0L)
		return (1L);
	for (z = 1 ; x > 0 ; x--)
		z *= y;
	return (z);
}

