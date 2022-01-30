/*
 *  fact
 *
 *  Return `n' factorial.
 */

long
fact(n)
register int n;
{
	register int	i;
	register long	f;

	if(n < 0)
		return(-1);

	if(n == 0)
		return(1);

	for(f = 1, i = 2 ; i <= n ; i++)
		f *= i;

	return(f);
}
