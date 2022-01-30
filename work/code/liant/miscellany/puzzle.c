/* Puzzle #1 */

#include <stdio.h>

#define DEFAULT_NUMBER	10000
#define MAX_OPERAND(n)	(((n) / 2) + ((n) % 2))

main (argc, argv)
int	 argc;
char	*argv[];
{
	unsigned long		i;
	register unsigned long	j;
	register unsigned long	sum;
	register unsigned long	number;

	if (argc < 2)
		number = DEFAULT_NUMBER;
	else if ((number = atol(argv[1])) <= 0) {
		fprintf (stderr, "usage: %s positive_number\n", argv[0]);
		exit (1);
	}

	for (i = 1 ; i < MAX_OPERAND(number) ; i++) {
		for (j = i, sum = 0 ; sum < number ; sum += j++)
			;
		if (sum == number)
			printf ("Sum of %d .. %d is %d\n", i, j - 1, sum);
	}

	exit (0);
}
