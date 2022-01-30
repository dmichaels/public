int aa[2][3] = { {1,2,3}, {4,5,6} };

main ()
{
	int (*p)[3] = aa;

	printf ("%d\n", aa[0][0]);
	printf ("%d\n", aa[0][1]);
	printf ("%d\n", aa[0][2]);
	printf ("%d\n", aa[1][0]);
	printf ("%d\n", aa[1][1]);
	printf ("%d\n", aa[1][2]);

	printf ("%d\n", p[0][0]);
	printf ("%d\n", p[0][1]);
	printf ("%d\n", p[0][2]);
	printf ("%d\n", p[1][0]);
	printf ("%d\n", p[1][1]);
	printf ("%d\n", p[1][2]);


#if 0
	int **x /*= new int [4]*/;

	int **p = x;

	p[0][0] = 1;
	p[0][1] = 2;
	p[1][0] = 3;
	p[1][1] = 4;
#endif
}
