#include <stdio.h>

typedef long big_t;	/* largest integer quantity */

#define MAX(a,b)	((a) >= (b) ? (a) : (b))
#define MIN(a,b)	((a) <= (b) ? (a) : (b))

main(ac,av)
int	ac;
char	*av[];
{
	int	pcomb_norep();
	big_t	ncomb_norep();
	big_t	atol();
	big_t	ncomb;
	big_t	n, r;

	if(ac != 3 && ac != 4){
		fprintf(stderr,"usage: %s N_items R_at_a_time [all]\n",av[0]);
		exit(1);
	}
	if((n = atol(av[1])) < 0 || (r = atol(av[2])) < 0){
		fprintf(stderr,
		"%s: arguments must be positive integers\n",av[0]);
		exit(1);
	}
	if(n < r){
		fprintf(stderr,"%s: n (%D) must be <= r (%D)\n",av[0],n,r);
		exit(1);
	}
	if(ac == 4 && strcmp(av[3],"all") == 0){
		pcomb_norep(stdout,n,r);
		exit(0);
	}
	printf("The number of combinations of %D items\n",n);
	printf("taken %D at a time (without repetition) is ",r);
	fflush(stdout);
	if((ncomb = ncomb_norep(n,r)) < 0)
		printf("out of range");
	else
		printf("%D\n",ncomb);
}


/*
 *  ncomb_norep
 *
 *  Return the number of combinations there
 *  are of `n' items taken `r' at a time with
 *  no repetitions (where r <= n).  Recursive.
 *
 *  Algorithm:
 *
 *  In "The Art of Compuer Programming"  Vol.I
 *  by Donald Knuth, we find that:
 *
 *   C(n+1,r+1) == C(0,r) + C(1,r) +..+ C(n,r)
 *
 *  Where  C(n,r)  is defined as  n!/r!(n-r)!
 *
 *  This leads to the following:
 *
 *   C(n,r) == 1 + r + C(r+1,r) +..+ C(n-1,r-1)
 *
 *  Since  C(n,r) == 0  for n < r
 *	   C(n,r) == 1  for n == r
 *	   C(n,r) == r  for n == r + 1
 */

big_t
ncomb_norep(n,r)
register big_t n, r;
{
	big_t ncomb_norep();
	register big_t c, i;
	register big_t r_minus_1;

	if(n < r || r < 0 || n <= 0)
		return(0);
	if(n == r || r == 0)
		return(1);
	if(n == r + 1 || r == 1)
		return(n);

	r_minus_1 = r - 1;
	c = r + 1;

	for(i = n - 1 ; i > r ; i--)
		c += ncomb_norep(i,r_minus_1);

	return(c);
}


/*
 *  pcomb_norep
 *
 *  Print to the given file `f', all of combinations
 *  of `n' items (numbers 1 through `n') taken `r' at
 *  a time with no repetitions (one set per line).
 *  For example the call pcomb_norep(stdout,3,2) gives:
 *
 *			1   2
 *			1   3
 *			2   3
 */

pcomb_norep(f,n,r)
FILE *f;
register int n, r;
{
	register int *a, i, j, k;
	register int max, n_minus_r_plus_1, r_minus_1;
	register big_t ncomb;

	if((a = (int *)malloc(r * sizeof(int))) == NULL)
		return(-1);

	for(i = 1 ; i <= r ; i++)
		a[i-1] = i;

	max = n - (r - 1);
	n_minus_r_plus_1 = n - r + 1;
	r_minus_1 = r - 1;
	ncomb = 0;

	while(1){
		for(i = 0 ; i < r ; i++)
			fprintf(f,"%4d",a[i]);
		fputc('\n',f);
		ncomb++;
		if(*a == max)
			break;
		for(i = r_minus_1 ; i >= 0 ; i--){
			if(a[i] == n_minus_r_plus_1 + i)
				continue;
			k = ++a[i];
			for(j = i + 1 ; j < r ; j++)
				a[j] = ++k;
			break;
		}
	}
	return(ncomb);
}

/*
 *  fact
 *
 *  Return `n' factorial.
 */

big_t
fact(n)
register big_t n;
{
	register big_t f, i;

	if(n < 0)
		return((big_t)-1);
	if(n == 0)
		return((big_t)1);
	for(f = 1, i = 2 ; i <= n ; i++)
		f *= (big_t)i;
	return(f);
}
