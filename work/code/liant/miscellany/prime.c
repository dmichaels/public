/* ------------------------------------------------------------------------
 *	usage: prime number
 *
 *	Print to the standard output the first 'number' primes
 *	up to some unknown reasonable number.
 *
 *	AUTHOR:	David Michaels (david at ll-sst)
 *	DATE:	February 1984
 * ---------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>

#define TRUE	1
#define FALSE	0

typedef int bool;
typedef int ulong;

/* ------------------------------------------------------------------------
 * main
 * ---------------------------------------------------------------------- */

main (argc, argv)
int argc;
char **argv;
{
	void	primes ();
	long	i, n;
	ulong	*p;

	if (argc != 2){
		fprintf (stderr, "usage: prime number\n");
		exit (1);
	}
	if ((n = atol(argv[1])) <= 0){
		fprintf (stderr, "prime: bad number argument - %d\n", argv[1]);
		exit (1);
	}
	if ((p = (int *)malloc(n+5)) == NULL){
		fprintf (stderr, "prime: memory shortage for %d primes\n", n);
		exit (1);
	}
	primes (p, (ulong)n);
	for (i = 0 ; i < n ; i++)
		printf ("%d\n", *p++);
}

/* ------------------------------------------------------------------------
 * ipow
 * ---------------------------------------------------------------------- */

ulong
ipow (x, y)
register ulong x, y;
{
	register ulong z;

	if (y == 0L)
		return (1L);
	for (z = 1 ; x > 0 ; x--)
		z *= y;
	return (z);
}

/* ------------------------------------------------------------------------
 * ipow
 * ---------------------------------------------------------------------- */

void
primes (p, np)
ulong *p, np;
{
	register ulong j, k, n, inc, ord;
	register bool j_is_prime;

	p[0] = 2L;
	p[1] = 3L;
	k = 2L;
	j = 1L;
	inc = 4L;
	ord = 0L;

	while (k < np) {
		do {
			j += inc;
			inc = 6 - inc;
			while (ipow((ulong)p[ord],(ulong)2) <= j)
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
