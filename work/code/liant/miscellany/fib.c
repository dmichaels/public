/*
*  fib
*
*  Return the `n'th fibonacci number.
*/

long
fib(n)
register n;
{
	register long a = 1, b = 1, f, i;

	if(n < 1)
		return(0);

	if(n < 3)
		return(a);

	for(i = 3 ; i <= n ; i++){
		f = a + b;
		a = b;
		b = f;
	}
	return(f);
}
