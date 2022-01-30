/* Sieve of Eratosthenes */

#include <stdio.h>

#define FALSE		(0)
#define TRUE		(-1)
#define MAX_VALUE	10000
#define MAX_PRIMES	5000

typedef int	bool;
typedef char	sieve[MAX_VALUE];
typedef int	results[MAX_PRIMES];

void
read_input(maxv)
int *maxv;
{
	bool ok;

	ok = FALSE;

	do {
		printf (" Input maximum prime boundry: ");
		scanf ("%d",maxv);
		if (*maxv > MAX_VALUE)
			printf(" Value too big.  Try again.\n");
		else
			ok = TRUE;
	} while (!ok);
}

void
print_out(values,total)
int *values;
int total;
{
	int i;

	printf (" Number of primes found was %d\n\n",total);
	for (i = 0 ; i < total ; i++) {
		printf ("%7d",values[i]);
		if (((i + 1) % 10) == 0)
			printf("\n");
	}
	printf ("\n\n");
}

void
sift(n)
int n;
{
	int i, k, count;
	sieve flags;
	results primes;
	int this_prime;

	for (i = 0 ; i < n ; i++)
		flags[i] = TRUE;
	
	count = 0;
	primes[0] = 1;

	for (i = 0 ; i < n ; i++) {
		if (flags[i]) {
			this_prime = i + 2;
			count++;
			primes[count] = this_prime;
			k = i + this_prime;
			while (k < n) {
				/* cancel all multiples */
				flags[k] = FALSE;
				k += this_prime;
			}
		}
	}
	print_out (primes,count);
}

main()
{
	int n;

	setbuf(stdout,NULL);

	printf("\n *** Sieve of Eratosthenes ***\n\n");

	read_input (&n);

	while (n > 1) {
		sift (n);
		read_input(&n);
	}
}
