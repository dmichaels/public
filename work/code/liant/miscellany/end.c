main()
{
	extern long etext, edata, end;

	printf ("etext: 0x%lx edata: 0x%lx end: 0x%lx\n", &etext, &edata, &end);
}
