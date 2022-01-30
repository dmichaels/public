/* isprime.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int
isprime (unsigned long n)
{
	unsigned long	 ndivisors	= sqrt (n);
	unsigned char	*divisibleby	= new unsigned char [ndivisors];
	unsigned long	 i, j;

	for (i = 2 ; i < ndivisors ; i++)
		divisibleby[i] = 1;

	for (i = 2 ; i < ndivisors ; i++) {
		if (!divisibleby[i])
			continue;
		else if ((n % i) == 0) {
			delete divisibleby;
			return (0);
		}
		else for (j = (i + 2) ; j < ndivisors ; j += (i + 2))
			divisibleby[j] = 0;
	}

	delete divisibleby;

	return (1);
}

int
main (int argc, char *argv[])
{
	unsigned long i, n;

	if (argc < 2) {
		for (n = 0 ; n <= ULONG_MAX ; n++) {
			if (isprime (n))
				printf ("%lu IS PRIME.\n", n);
		}
	}
	else for (i = 1 ; i < argc ; i++) {
		if ((n = strtoul (argv[i], 0, 10)) == 0)
			printf ("%s IS ILLEGAL.\n", argv[i]);
		else if (isprime (n))
			printf ("%lu IS PRIME.\n", n);
		else	printf ("%lu IS NOT PRIME.\n", n);
	}
	return (0);
}

