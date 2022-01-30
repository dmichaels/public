/* ------------------------------------------------------------------------
 *  NAME
 *	select - select a mega-bucks number series
 *
 *  SYNOPSIS
 *	select [n_selections]
 *	select all
 *
 *  DESCRIPTION
 *	Prints to standard output, six numbers selected at random
 *	between 1 and 36 (inclusive).  If a number arguement is
 *	given, then that many selections will be printed (one
 *	per line).  If the "all" option is given, then all of the
 *	36! / (6! * (36 - 6)!) = 1,947,792  possible combinations
 *	will be printed.
 *
 *  AUTHOR
 *	Nick Bonifanti  (nick@ll-sst) ? 1984
 *	David Michaels (david@ll-sst) Modified, September 1984
 * ---------------------------------------------------------------------- */

#include <stdio.h>

typedef int		bool;
typedef unsigned long	ulong;

#define FALSE		0
#define TRUE		(!FALSE)

#define POSSIBILITIES	36
#define SELECTIONS	6

main(ac,av)
int	ac;
char	*av[];
{
	int		atoi();
	ulong		pcomb_norep();
	void		usage(), select(), pr_select();
	bool		list[POSSIBILITIES + 1];
	unsigned int	count;

	if(ac > 2){
		usage();
	}
	if(ac == 1){
		count = 1;
	}
	else if(strcmp(av[1],"all") == 0){
		pcomb_norep(stdout,POSSIBILITIES,SELECTIONS);
		exit(0);
	}
	else if((count = atoi(av[1])) <= 0){
		usage();
	}
	while(count-- > 0){
		select(list,SELECTIONS,POSSIBILITIES);
		pr_select(list,POSSIBILITIES);
	}
}

void
usage()
{
	fprintf(stderr,"usage: select [n_selections]\n");
	fprintf(stderr,"       select all\n");
	exit(1);
}

/*
*  pr_select
*
*  Print to the standard output, the index number of each of
*  the `possibilities' slots in the `list' array which is set
*  to TRUE starting at index number 1 (the 0th slot is wasted).
*  The `list' array is assumed to have `possiblities' + 1 slots
*  allocated to for its use.
*/

void
pr_select(list,possibilities)
register unsigned bool list[];
register unsigned int possibilities;
{
	register unsigned int n, nums;

	for(n = 1 ; n <= possibilities ; n++)
		if(list[n] == TRUE)
			printf("%4d",n);
	putchar('\n');
}

/*
*  select
*
*  Select at random `selections' number in the range
*  1 through `possibilities', and set the corresponding
*  slots in the `list' array to TRUE, set all the others
*  to FALSE.  For simplicity, the `list' array is to be
*  1 indexed, and thus the 0th slot will be wasted, and
*  the `list' array must have `possibilities' + 1 slots
*  allocated for its use.
*/

void
select(list,selections,possibilities)
register unsigned bool list[];
register unsigned int selections, possibilities;
{
	static long	k = 3;
	register int	nums = 0, r;

	/* Initialize the list */

	for(r = 1 ; r <= possibilities ; r++)
		list[r] = FALSE;

	/* Initialize random number generator */

	srand((int)(time(0L) * k++));

	/* Select the numbers */

	while (nums < selections){
		r = rand() % possibilities;
		if (list[r + 1] == TRUE)
			continue;
		list[r + 1] = TRUE;
		nums++;
	}
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
*
*  Return the number of combinations printed, or return
*  -1 if an error occurred.
*/

ulong
pcomb_norep(f,n,r)
FILE *f;
register int n, r;
{
	register int *a, i, j, k;
	register int max, n_minus_r_plus_1, r_minus_1;
	register ulong ncomb;

	if((a = (int *)malloc(r * sizeof(int))) == NULL)
		return(-1);

	for(i = 1 ; i <= r ; i++)
		a[i-1] = i;

	/*
	*  The following three
	*  are done for efficiency.
	*/
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
