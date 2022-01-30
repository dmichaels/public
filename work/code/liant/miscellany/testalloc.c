/*
 *  alloc -- Test allocation.
 *
 *  usage: alloc number_or_units_to_allocate [size_of_allocation_unit_in_bytes]
 */

#include <stdio.h>

main (argc, argv)
int	argc;
char	*argv[];
{
	extern long etext, edata, end;

	char	*_malloc ();
	void	_free ();
	char	*malloc ();
	void	free ();
	void	usage ();

	typedef struct block {
		long		size;	/* Size in bytes of the memory block */
		struct block	*next;	/* Pointer to the next memory block */
	} BLOCK;

	register long	n, nunits, unitsize = 1;
	register BLOCK	*p, *q;
	BLOCK		*list = (BLOCK *)NULL;
	int		verbose_flag = 0;
	int		system_alloc = 0;
	char		*(*doalloc)() = _malloc;
	void		(*dofree)() = _free;

	setbuf (stdout, NULL);

        for (argc--, argv++ ; argc > 0 ; argc--, argv++) {
                if ((*argv)[0] == '-') {
                        switch ((*argv)[1]) {
                        case 'v':
				verbose_flag = 1;
                                break;
                        case 's':
				doalloc = malloc;
				dofree = free;
				system_alloc = 1;
                                break;
                        default:
				printf ("alloc: bad flag (%s)\n",*argv);
                                usage ();
                        }
                }
                else    break;
        }

	if ((argc < 1) || (argc > 2))
		usage ();

	if ((nunits = atol(argv[0])) < 0)
		usage ();

	if ((argc == 2) && ((unitsize = atol(argv[1])) < 0))
		usage ();

	printf ("etext: 0x%lx edata: 0x%lx end: 0x%lx\n", &etext, &edata, &end);

	printf ("Doing %ld allocations of (%ld + %d) bytes each (brk: 0x%lx) ...\n",
		nunits, unitsize, sizeof(BLOCK), sbrk(0));

	for (n = 1 ; n <= nunits ; n++) {
		if ((p = (BLOCK *)(*doalloc)(unitsize+sizeof(BLOCK))) == NULL) {
			printf ("Alloc %ld of %ld bytes failed (end: 0x%lx)\n",
				n, unitsize, sbrk(0));
		}
		else {
			if (verbose_flag)
			printf (
			"Alloc %ld of (%ld + %ld) bytes (0x%lx) (end: 0x%lx)\n",
			n, unitsize, sizeof(BLOCK), p, sbrk(0));

			p->size = unitsize + sizeof(BLOCK);
			p->next = list;
			list = p;
		}
	}
	printf ("Allocations done (brk: 0x%lx).\n",sbrk(0));
	if (verbose_flag)
		debug_alloc();

	printf ("Freeing...\n");
	for (p = list ; p != (BLOCK *)NULL ; p = q) {
		q = p->next;
		(*dofree)(p);
	}
	printf ("Freeing done (brk: 0x%lx).\n",sbrk(0));
	if (verbose_flag)
		debug_alloc();
}

void
usage ()
{
	fprintf (stderr, "usage: alloc nunits_to_alloc [size_of_unit]\n");
	exit (1);
}
